

# File qfbookmarklocatorfilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qfbookmarklocatorfilter.cpp**](qfbookmarklocatorfilter_8cpp.md)

[Go to the documentation of this file](qfbookmarklocatorfilter_8cpp.md)


```C++
/***************************************************************************
  qfbookmarklocatorfilter.cpp

 ---------------------
 begin                : 11.01.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfbookmarklocatorfilter.h"
#include "qfbookmarkmodel.h"
#include "qflocatormodelsuperbridge.h"

#include <QAction>
#include <qgsfeedback.h>
#include <qgspoint.h>


QfBookmarkLocatorFilter::QfBookmarkLocatorFilter( QfLocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( true );
}

QfBookmarkLocatorFilter *QfBookmarkLocatorFilter::clone() const
{
  return new QfBookmarkLocatorFilter( mLocatorBridge );
}

void QfBookmarkLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  Q_UNUSED( feedback )

  if ( !mLocatorBridge->bookmarks() )
    return;

  for ( int i = 0; i < mLocatorBridge->bookmarks()->rowCount(); i++ )
  {
    QgsLocatorResult result;
    result.displayString = mLocatorBridge->bookmarks()->data( mLocatorBridge->bookmarks()->index( i, 0 ), QfBookmarkModel::BookmarkName ).toString();
    result.score = fuzzyScore( result.displayString, string );
    if ( result.score > 0 )
    {
      result.filter = this;
      result.setUserData( i );
      result.actions << QgsLocatorResult::ResultAction( QfNavigation, tr( "Navigate to bookmark" ), QStringLiteral( "qrc:/themes/qfield/nodpi/ic_navigation_flag_purple_24dp.svg" ) );
      emit resultFetched( result );
    }
  }
}

void QfBookmarkLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void QfBookmarkLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  const int row = result.userData().toInt();

  if ( actionId == QfNavigation )
  {
    if ( !mLocatorBridge->navigation() )
      return;

    const QgsPoint point = mLocatorBridge->bookmarks()->getBookmarkPoint( row );
    if ( !point.isEmpty() )
      mLocatorBridge->navigation()->setDestination( point );
    return;
  }

  mLocatorBridge->bookmarks()->setExtentFromBookmark( mLocatorBridge->bookmarks()->index( row, 0 ) );

  const QgsGeometry geom( mLocatorBridge->bookmarks()->data( mLocatorBridge->bookmarks()->index( row, 0 ), QfBookmarkModel::BookmarkPoint ).value<QgsGeometry>() );
  mLocatorBridge->geometryHighlighter()->setProperty( "qgsGeometry", geom );
  mLocatorBridge->geometryHighlighter()->setProperty( "crs", mLocatorBridge->mapSettings()->mapSettings().destinationCrs() );
}
```


