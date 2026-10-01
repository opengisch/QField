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

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>
#include <QTextDocumentFragment>
#include <QXmlStreamReader>
#include <qgsnetworkaccessmanager.h>

#include <algorithm>

namespace
{
  const QLatin1String sMediaNamespace( "http://search.yahoo.com/mrss/" );

  const QLatin1String sEnabledKey( "/QField/News/Enabled" );
  const QLatin1String sEnabledSinceKey( "/QField/News/EnabledSince" );
  const QLatin1String sDismissedUrlsKey( "/QField/News/DismissedUrls" );

  QString htmlToPlainText( const QString &html )
  {
    QString text = QTextDocumentFragment::fromHtml( html ).toPlainText();
    text.remove( QChar::ObjectReplacementCharacter );
    return text.simplified();
  }
} // namespace


QfNewsParser::QfNewsParser( QObject *parent )
  : QObject( parent )
{
}

void QfNewsParser::setFeedUrl( const QUrl &url )
{
  if ( mFeedUrl == url )
    return;

  mFeedUrl = url;
  emit feedUrlChanged();
}

bool QfNewsParser::enabled() const
{
  return QSettings().value( sEnabledKey, false ).toBool();
}

void QfNewsParser::setEnabled( bool enabled )
{
  if ( this->enabled() == enabled )
    return;

  QSettings settings;
  settings.setValue( sEnabledKey, enabled );
  settings.remove( sDismissedUrlsKey );
  if ( enabled )
  {
    settings.setValue( sEnabledSinceKey, QDateTime::currentDateTimeUtc() );
  }
  else
  {
    settings.remove( sEnabledSinceKey );
  }

  emit enabledChanged();

  if ( !mItems.isEmpty() )
  {
    mItems.clear();
    emit itemsChanged();
  }
}

void QfNewsParser::fetch()
{
  if ( !enabled() || mIsFetching )
    return;

  mIsFetching = true;
  emit isFetchingChanged();

  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( QNetworkRequest( mFeedUrl ) );
  connect( reply, &QNetworkReply::finished, this, [this, reply]() {
    reply->deleteLater();

    if ( reply->error() == QNetworkReply::NoError && enabled() )
    {
      const QList<QfNewsItem> feedItems = parseFeed( reply->readAll() );
      if ( !feedItems.isEmpty() )
      {
        updateItems( feedItems );
      }
    }

    mIsFetching = false;
    emit isFetchingChanged();
  } );
}

void QfNewsParser::dismiss( const QUrl &url )
{
  QSettings settings;
  QStringList dismissedUrls = settings.value( sDismissedUrlsKey ).toStringList();
  if ( !dismissedUrls.contains( url.toString() ) )
  {
    dismissedUrls << url.toString();
    settings.setValue( sDismissedUrlsKey, dismissedUrls );
  }

  const qsizetype removedCount = mItems.removeIf( [&url]( const QfNewsItem &item ) { return item.url == url; } );
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
      else if ( insideItem && xml.namespaceUri().isEmpty() )
      {
        if ( xml.name() == QLatin1String( "title" ) )
          item.title = xml.readElementText().trimmed();
        else if ( xml.name() == QLatin1String( "link" ) )
          item.url = QUrl( xml.readElementText().trimmed() );
        else if ( xml.name() == QLatin1String( "pubDate" ) )
          item.publicationDate = QDateTime::fromString( xml.readElementText().trimmed(), Qt::RFC2822Date );
        else if ( xml.name() == QLatin1String( "description" ) )
          item.description = htmlToPlainText( xml.readElementText() );
      }
      else if ( insideItem && xml.namespaceUri() == sMediaNamespace && xml.name() == QLatin1String( "content" ) )
      {
        item.imageUrl = QUrl( xml.attributes().value( QLatin1String( "url" ) ).toString() );
      }
    }
    else if ( xml.isEndElement() && xml.name() == QLatin1String( "item" ) )
    {
      insideItem = false;
      if ( !item.title.isEmpty() && item.url.isValid() && item.publicationDate.isValid() )
      {
        items << item;
      }
    }
  }

  if ( xml.hasError() )
    return QList<QfNewsItem>();

  return items;
}

void QfNewsParser::updateItems( const QList<QfNewsItem> &feedItems )
{
  QSettings settings;
  const QDateTime enabledSince = settings.value( sEnabledSinceKey ).toDateTime();
  const QStringList dismissedUrls = settings.value( sDismissedUrlsKey ).toStringList();

  QList<QfNewsItem> items;
  QStringList feedDismissedUrls;
  for ( const QfNewsItem &item : feedItems )
  {
    if ( dismissedUrls.contains( item.url.toString() ) )
    {
      feedDismissedUrls << item.url.toString();
    }
    else if ( item.publicationDate > enabledSince )
    {
      items << item;
    }
  }

  // Forget dismissed items that are no longer part of the feed
  if ( feedDismissedUrls.size() != dismissedUrls.size() )
  {
    settings.setValue( sDismissedUrlsKey, feedDismissedUrls );
  }

  std::sort( items.begin(), items.end(), []( const QfNewsItem &a, const QfNewsItem &b ) { return a.publicationDate > b.publicationDate; } );

  if ( mItems != items )
  {
    mItems = items;
    emit itemsChanged();
  }
}
