

# File qfnewsparser.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfnewsparser.cpp**](qfnewsparser_8cpp.md)

[Go to the documentation of this file](qfnewsparser_8cpp.md)


```C++
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
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QStandardPaths>
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

  if ( mEnabled && !mFeedUrl.isEmpty() )
  {
    restoreItemsFromCache();
  }
}

bool QfNewsParser::enabled() const
{
  return mEnabled;
}

void QfNewsParser::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
  {
    return;
  }

  mEnabled = enabled;
  emit enabledChanged();

  clearItems();

  if ( mEnabled && !mFeedUrl.isEmpty() )
  {
    restoreItemsFromCache();
  }
}

void QfNewsParser::restoreItemsFromCache()
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

void QfNewsParser::fetch()
{
  if ( !mEnabled || mFeedUrl.isEmpty() || mIsFetching )
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

  QDomDocument document;
  if ( !document.setContent( data, QDomDocument::ParseOption::UseNamespaceProcessing ) )
  {
    return items;
  }

  const QDomNodeList itemNodes = document.elementsByTagName( QStringLiteral( "item" ) );
  for ( int i = 0; i < itemNodes.size(); ++i )
  {
    const QDomElement itemElement = itemNodes.at( i ).toElement();
    if ( itemElement.parentNode().toElement().tagName() != QLatin1String( "channel" ) )
    {
      continue;
    }

    QfNewsItem item;
    item.uuid = itemElement.firstChildElement( QStringLiteral( "guid" ) ).text().trimmed();
    item.title = itemElement.firstChildElement( QStringLiteral( "title" ) ).text().trimmed();
    item.url = QUrl( itemElement.firstChildElement( QStringLiteral( "link" ) ).text().trimmed() );
    item.publicationDate = QDateTime::fromString( itemElement.firstChildElement( QStringLiteral( "pubDate" ) ).text().trimmed(), Qt::RFC2822Date );
    item.description = QfStringUtils::htmlToPlainText( itemElement.firstChildElement( QStringLiteral( "description" ) ).text() );

    for ( QDomElement child = itemElement.firstChildElement(); !child.isNull(); child = child.nextSiblingElement() )
    {
      if ( child.localName() == QLatin1String( "content" ) && child.hasAttribute( QStringLiteral( "url" ) ) )
      {
        item.imageUrl = QUrl( child.attribute( QStringLiteral( "url" ) ) );
        break;
      }
    }

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
  const QStringList dismissedUuids = settings.value( DISMISSED_UUIDS_SETTINGS_KEY ).toStringList();

  QList<QfNewsItem> items;
  QStringList feedDismissedUuids;
  for ( const QfNewsItem &item : feedItems )
  {
    if ( dismissedUuids.contains( item.uuid ) )
    {
      feedDismissedUuids << item.uuid;
    }
    else if ( item.publicationDate.daysTo( QDateTime::currentDateTime() ) <= MAXIMUM_AGE_DAYS )
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
```


