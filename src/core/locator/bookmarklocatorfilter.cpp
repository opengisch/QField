/***************************************************************************
  bookmarklocatorfilter.cpp

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

#include "bookmarklocatorfilter.h"
#include "bookmarkmodel.h"
#include "locatormodelsuperbridge.h"

#include <QAction>
#include <qgsfeedback.h>
#include <qgspoint.h>


BookmarkLocatorFilter::BookmarkLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( true );
}

BookmarkLocatorFilter *BookmarkLocatorFilter::clone() const
{
  return new BookmarkLocatorFilter( mLocatorBridge );
}

void BookmarkLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback *feedback )
{
  Q_UNUSED( feedback )

  if ( !mLocatorBridge->bookmarks() )
    return;

  for ( int i = 0; i < mLocatorBridge->bookmarks()->rowCount(); i++ )
  {
    QgsLocatorResult result;
    result.displayString = mLocatorBridge->bookmarks()->data( mLocatorBridge->bookmarks()->index( i, 0 ), BookmarkModel::BookmarkName ).toString();
    result.score = fuzzyScore( result.displayString, string );
    if ( result.score > 0 )
    {
      result.filter = this;
      result.userData = i;

      emit resultFetched( result );
    }
  }
}

void BookmarkLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void BookmarkLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int )
{
  const int row = result.userData.toInt();

  mLocatorBridge->bookmarks()->setExtentFromBookmark( mLocatorBridge->bookmarks()->index( row, 0 ) );

  QgsGeometry geom( mLocatorBridge->bookmarks()->data( mLocatorBridge->bookmarks()->index( row, 0 ), BookmarkModel::BookmarkPoint ).value<QgsGeometry>() );
  mLocatorBridge->locatorHighlightGeometry()->setProperty( "qgsGeometry", geom );
  mLocatorBridge->locatorHighlightGeometry()->setProperty( "crs", mLocatorBridge->mapSettings()->mapSettings().destinationCrs() );
}
