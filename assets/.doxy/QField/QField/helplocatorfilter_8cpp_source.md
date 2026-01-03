

# File helplocatorfilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**helplocatorfilter.cpp**](helplocatorfilter_8cpp.md)

[Go to the documentation of this file](helplocatorfilter_8cpp.md)


```C++
/***************************************************************************
  helplocatorfilter.cpp

 ---------------------
 begin                : 02.07.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "helplocatorfilter.h"
#include "locatormodelsuperbridge.h"

#include <QDesktopServices>
#include <QNetworkRequest>
#include <QTextDocument>
#include <qgsblockingnetworkrequest.h>
#include <qgsfeedback.h>
#include <qgsstringutils.h>

HelpLocatorFilter::HelpLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setFetchResultsDelay( 1000 );
  setUseWithoutPrefix( false );
}

HelpLocatorFilter *HelpLocatorFilter::clone() const
{
  return new HelpLocatorFilter( mLocatorBridge );
}

void HelpLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  Q_UNUSED( feedback )

  if ( string.length() < 3 )
  {
    return;
  }

  const QString searchString = string.trimmed().toLower();
  const QStringList words = searchString.split( ' ', Qt::SkipEmptyParts );
  if ( string.length() < 3 || words.isEmpty() )
  {
    return;
  }

  QNetworkRequest request( QUrl( "https://docs.qfield.org/search/search_index.json" ) );
  QgsBlockingNetworkRequest blockingRequest;
  const QgsBlockingNetworkRequest::ErrorCode errorCode = blockingRequest.get( request, false, feedback );
  if ( errorCode != QgsBlockingNetworkRequest::NoError )
  {
    return;
  }

  QJsonParseError err;
  const QJsonDocument jsonDoc = QJsonDocument::fromJson( blockingRequest.reply().content(), &err );
  if ( jsonDoc.isNull() )
  {
    return;
  }
  const QVariantMap searchMap = jsonDoc.object().toVariantMap();
  const QStringList lang = searchMap.value( QStringLiteral( "config" ) ).toMap().value( QStringLiteral( "lang" ) ).toStringList();
  const QList<QVariant> docs = searchMap.value( QStringLiteral( "docs" ) ).toList();

  const QLocale locale;
  QString userLocale = locale.name().mid( 0, 2 );
  if ( !lang.contains( userLocale ) || userLocale == QStringLiteral( "en" ) )
  {
    userLocale.clear();
  }

  QRegularExpression rx( QStringLiteral( "\\A([a-z]{2})\\/" ) );
  for ( const QVariant &doc : docs )
  {
    QVariantMap details = doc.toMap();
    const QString title = details.value( QStringLiteral( "title" ) ).toString().toLower();
    const QString text = details.value( QStringLiteral( "text" ) ).toString().toLower();

    if ( text.isEmpty() )
    {
      continue;
    }

    const QString location = details.value( QStringLiteral( "location" ) ).toString();
    QString locationLocale;
    QRegularExpressionMatch rxMatch = rx.match( location );
    if ( rxMatch.hasMatch() )
    {
      locationLocale = rxMatch.captured( 1 );
    }

    if ( locationLocale == userLocale )
    {
      if ( location.startsWith( QStringLiteral( "reference/" ) ) || location.startsWith( QStringLiteral( "%1/reference/" ).arg( userLocale ) ) || location.startsWith( QStringLiteral( "success-stories/" ) ) || location.startsWith( QStringLiteral( "%1/success-stories/" ).arg( userLocale ) ) )
      {
        continue;
      }

      bool match = false;
      int matchScore = 0;
      for ( const QString &word : words )
      {
        match = title.contains( word ) || text.contains( word );
        matchScore += title.count( word ) * 3 + text.count( word );
      }

      if ( match )
      {
        if ( !location.isEmpty() )
        {
          if ( QgsStringUtils::soundex( title ) == QgsStringUtils::soundex( searchString ) )
          {
            // When the search term is a near-match to the title, add a big bonus (e.g. search term project matching page title projects)
            matchScore += 100;
          }

          QTextDocument htmlDoc;
          htmlDoc.setHtml( details.value( QStringLiteral( "text" ) ).toString() );

          QgsLocatorResult result;
          result.displayString = details.value( QStringLiteral( "title" ) ).toString();
          result.description = htmlDoc.toPlainText();
          result.score = matchScore;
          result.filter = this;
          result.setUserData( QStringLiteral( "https://docs.qfield.org/%1" ).arg( location ) );
          emit resultFetched( result );
        }
      }
    }
  }
}

void HelpLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void HelpLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int )
{
  const QString url = result.userData().toString();
  QDesktopServices::openUrl( url );
}
```


