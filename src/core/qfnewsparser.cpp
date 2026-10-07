/***************************************************************************
                        qfnewsparser.cpp
                        ----------------
  begin                : October 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfnewsparser.h"
#include "qfstringutils.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <qgsnetworkaccessmanager.h>

#include <algorithm>


QfNewsParser::QfNewsParser( QObject *parent )
  : QObject( parent )
{
}

void QfNewsParser::setFeedUrl( const QUrl &url )
{
  if ( mFeedUrl == url )
  {
    return;
  }

  mFeedUrl = url;
  emit feedUrlChanged();

  clearItems();

  if ( enabled() && !mFeedUrl.isEmpty() )
  {
    QFile cacheFile( cacheFilePath() );
    if ( cacheFile.exists() && cacheFile.open( QIODevice::ReadOnly ) )
    {
      const QList<QfNewsItem> feedItems = parseFeed( cacheFile.readAll() );
      if ( !feedItems.isEmpty() )
      {
        updateItems( feedItems );
      }
    }
  }
}

bool QfNewsParser::enabled() const
{
  return QSettings().value( ENABLED_SETTINGS_KEY, false ).toBool();
}

void QfNewsParser::setEnabled( bool enabled )
{
  if ( this->enabled() == enabled )
  {
    return;
  }

  QSettings settings;
  settings.setValue( ENABLED_SETTINGS_KEY, enabled );
  settings.remove( DISMISSED_UUIDS_SETTINGS_KEY );
  if ( enabled )
  {
    settings.setValue( ENABLED_SINCE_SETTINGS_KEY, QDateTime::currentDateTimeUtc() );
  }
  else
  {
    settings.remove( ENABLED_SINCE_SETTINGS_KEY );
  }

  emit enabledChanged();

  clearItems();
}

void QfNewsParser::fetch()
{
  if ( !enabled() || mFeedUrl.isEmpty() || mIsFetching )
  {
    return;
  }

  mIsFetching = true;
  emit isFetchingChanged();

  const QUrl url = mFeedUrl;
  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( QNetworkRequest( url ) );
  connect( reply, &QNetworkReply::finished, this, [this, reply, url]() {
    reply->deleteLater();

    if ( reply->error() == QNetworkReply::NoError && enabled() && mFeedUrl == url )
    {
      const QByteArray data = reply->readAll();
      const QList<QfNewsItem> feedItems = parseFeed( data );
      if ( !feedItems.isEmpty() )
      {
        QDir().mkpath( QFileInfo( cacheFilePath() ).absolutePath() );
        QFile cacheFile( cacheFilePath() );
        if ( cacheFile.open( QIODevice::WriteOnly ) )
        {
          cacheFile.write( data );
        }

        updateItems( feedItems );
      }
    }

    mIsFetching = false;
    emit isFetchingChanged();
  } );
}

void QfNewsParser::dismiss( const QString &uuid )
{
  QSettings settings;
  QStringList dismissedUuids = settings.value( DISMISSED_UUIDS_SETTINGS_KEY ).toStringList();
  if ( !dismissedUuids.contains( uuid ) )
  {
    dismissedUuids << uuid;
    settings.setValue( DISMISSED_UUIDS_SETTINGS_KEY, dismissedUuids );
  }

  const qsizetype removedCount = mItems.removeIf( [&uuid]( const QfNewsItem &item ) { return item.uuid == uuid; } );
  if ( removedCount > 0 )
  {
    emit itemsChanged();
  }
}

QList<QfNewsItem> QfNewsParser::parseFeed( const QByteArray &data )
{
  QList<QfNewsItem> items;
  QXmlStreamReader xml( data );

  bool insideItem = false;
  QfNewsItem item;
  while ( !xml.atEnd() )
  {
    xml.readNext();

    if ( xml.isStartElement() )
    {
      if ( xml.name() == QLatin1String( "item" ) )
      {
        insideItem = true;
        item = QfNewsItem();
      }
      else if ( insideItem )
      {
        if ( xml.name() == QLatin1String( "guid" ) )
        {
          item.uuid = xml.readElementText().trimmed();
        }
        else if ( xml.name() == QLatin1String( "title" ) )
        {
          item.title = xml.readElementText().trimmed();
        }
        else if ( xml.name() == QLatin1String( "link" ) )
        {
          item.url = QUrl( xml.readElementText().trimmed() );
        }
        else if ( xml.name() == QLatin1String( "pubDate" ) )
        {
          item.publicationDate = QDateTime::fromString( xml.readElementText().trimmed(), Qt::RFC2822Date );
        }
        else if ( xml.name() == QLatin1String( "description" ) )
        {
          item.description = QfStringUtils::htmlToPlainText( xml.readElementText() );
        }
        else if ( xml.name() == QLatin1String( "content" ) && xml.attributes().hasAttribute( QLatin1String( "url" ) ) )
        {
          item.imageUrl = QUrl( xml.attributes().value( QLatin1String( "url" ) ).toString() );
        }
      }
    }
    else if ( xml.isEndElement() && xml.name() == QLatin1String( "item" ) )
    {
      insideItem = false;

      // The guid element is optional in RSS feeds, fall back to the item URL
      if ( item.uuid.isEmpty() )
      {
        item.uuid = item.url.toString();
      }

      if ( !item.uuid.isEmpty() && !item.title.isEmpty() && item.publicationDate.isValid() )
      {
        items << item;
      }
    }
  }

  if ( xml.hasError() )
  {
    return QList<QfNewsItem>();
  }

  return items;
}

QString QfNewsParser::cacheFilePath() const
{
  const QString urlHash = QCryptographicHash::hash( mFeedUrl.toString().toUtf8(), QCryptographicHash::Sha256 ).toHex();
  return QStringLiteral( "%1/news/%2.xml" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppConfigLocation ), urlHash );
}

void QfNewsParser::clearItems()
{
  if ( !mItems.isEmpty() )
  {
    mItems.clear();
    emit itemsChanged();
  }
}

void QfNewsParser::updateItems( const QList<QfNewsItem> &feedItems )
{
  QSettings settings;
  const QDateTime enabledSince = settings.value( ENABLED_SINCE_SETTINGS_KEY ).toDateTime();
  const QDateTime publishedAfter = std::max( enabledSince, QDateTime::currentDateTimeUtc().addDays( -MAXIMUM_AGE_DAYS ) );
  const QStringList dismissedUuids = settings.value( DISMISSED_UUIDS_SETTINGS_KEY ).toStringList();

  QList<QfNewsItem> items;
  QStringList feedDismissedUuids;
  for ( const QfNewsItem &item : feedItems )
  {
    if ( dismissedUuids.contains( item.uuid ) )
    {
      feedDismissedUuids << item.uuid;
    }
    else if ( item.publicationDate > publishedAfter )
    {
      items << item;
    }
  }

  // Forget dismissed items that are no longer part of the feed
  if ( feedDismissedUuids.size() != dismissedUuids.size() )
  {
    settings.setValue( DISMISSED_UUIDS_SETTINGS_KEY, feedDismissedUuids );
  }

  std::sort( items.begin(), items.end(), []( const QfNewsItem &a, const QfNewsItem &b ) { return a.publicationDate > b.publicationDate; } );

  if ( mItems != items )
  {
    mItems = items;
    emit itemsChanged();
  }
}
