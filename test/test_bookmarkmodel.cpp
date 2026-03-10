/***************************************************************************
                        test_bookmarkmodel
                        --------------------
  begin                : March 2026
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

#define QFIELDTEST_MAIN

#include "bookmarkmodel.h"
#include "catch2.h"

#include <QSignalSpy>
#include <qgsapplication.h>
#include <qgsbookmarkmanager.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsrectangle.h>
#include <qgsreferencedgeometry.h>

static QgsBookmark makeBookmark( const QString &name, const QString &group = QString() )
{
  QgsBookmark bm;
  bm.setName( name );
  bm.setGroup( group );
  bm.setExtent( QgsReferencedRectangle( QgsRectangle( 0.0, 0.0, 10.0, 10.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
  return bm;
}


TEST_CASE( "BookmarkModel" )
{
  SECTION( "default state with empty manager" )
  {
    QgsBookmarkManager manager;
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    REQUIRE( model.mapSettings() == nullptr );
  }

  SECTION( "rowCount reflects bookmarks in user manager" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "Alpha" ) ) );
    manager.addBookmark( makeBookmark( QStringLiteral( "Beta" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
  }

  SECTION( "rowCount reflects bookmarks from both user and project managers" )
  {
    QgsBookmarkManager userManager;
    QgsBookmarkManager projectManager;
    userManager.addBookmark( makeBookmark( QStringLiteral( "UserBM" ) ) );
    projectManager.addBookmark( makeBookmark( QStringLiteral( "ProjectBM" ) ) );
    BookmarkModel model( &userManager, &projectManager );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
  }

  SECTION( "BookmarkName role returns correct name" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "MyBookmark" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( BookmarkModel::BookmarkName ).toString() == QStringLiteral( "MyBookmark" ) );
  }

  SECTION( "BookmarkGroup role returns correct group" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "BMInGroup" ), QStringLiteral( "Favourites" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.index( 0, 0 ).data( BookmarkModel::BookmarkGroup ).toString() == QStringLiteral( "Favourites" ) );
  }

  SECTION( "BookmarkId role returns non-empty id" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "IdTest" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE_FALSE( model.index( 0, 0 ).data( BookmarkModel::BookmarkId ).toString().isEmpty() );
  }

  SECTION( "BookmarkPoint role returns center of extent" )
  {
    QgsBookmarkManager manager;
    QgsBookmark bm;
    bm.setName( QStringLiteral( "PointTest" ) );
    bm.setExtent( QgsReferencedRectangle( QgsRectangle( 0.0, 0.0, 4.0, 6.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
    manager.addBookmark( bm );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    const QgsGeometry point = model.index( 0, 0 ).data( BookmarkModel::BookmarkPoint ).value<QgsGeometry>();
    REQUIRE_FALSE( point.isNull() );
    const QgsPointXY center = point.asPoint();
    REQUIRE( center.x() == Catch::Approx( 2.0 ) );
    REQUIRE( center.y() == Catch::Approx( 3.0 ) );
  }

  SECTION( "BookmarkCrs role returns the extent's CRS" )
  {
    QgsBookmarkManager manager;
    QgsBookmark bm;
    bm.setName( QStringLiteral( "CrsTest" ) );
    bm.setExtent( QgsReferencedRectangle( QgsRectangle( 0.0, 0.0, 1.0, 1.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
    manager.addBookmark( bm );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    const QgsCoordinateReferenceSystem crs = model.index( 0, 0 ).data( BookmarkModel::BookmarkCrs ).value<QgsCoordinateReferenceSystem>();
    REQUIRE( crs.authid() == QStringLiteral( "EPSG:4326" ) );
  }

  SECTION( "BookmarkUser is true for bookmarks in user manager, false for project manager" )
  {
    QgsBookmarkManager userManager;
    QgsBookmarkManager projectManager;
    userManager.addBookmark( makeBookmark( QStringLiteral( "UserBM" ) ) );
    projectManager.addBookmark( makeBookmark( QStringLiteral( "ProjectBM" ) ) );
    BookmarkModel model( &userManager, &projectManager );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
    bool foundUser = false;
    bool foundProject = false;
    for ( int i = 0; i < model.rowCount( QModelIndex() ); ++i )
    {
      const QModelIndex idx = model.index( i, 0 );
      const QString name = idx.data( BookmarkModel::BookmarkName ).toString();
      const bool isUser = idx.data( BookmarkModel::BookmarkUser ).toBool();
      if ( name == QStringLiteral( "UserBM" ) )
      {
        REQUIRE( isUser == true );
        foundUser = true;
      }
      else if ( name == QStringLiteral( "ProjectBM" ) )
      {
        REQUIRE( isUser == false );
        foundProject = true;
      }
    }
    REQUIRE( foundUser );
    REQUIRE( foundProject );
  }

  SECTION( "data returns invalid QVariant for invalid index" )
  {
    QgsBookmarkManager manager;
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE_FALSE( model.data( QModelIndex(), BookmarkModel::BookmarkName ).isValid() );
  }

  SECTION( "roleNames contains all custom roles" )
  {
    QgsBookmarkManager manager;
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    const QHash<int, QByteArray> roles = model.roleNames();
    REQUIRE( roles.contains( BookmarkModel::BookmarkId ) );
    REQUIRE( roles.contains( BookmarkModel::BookmarkName ) );
    REQUIRE( roles.contains( BookmarkModel::BookmarkGroup ) );
    REQUIRE( roles.contains( BookmarkModel::BookmarkPoint ) );
    REQUIRE( roles.contains( BookmarkModel::BookmarkCrs ) );
    REQUIRE( roles.contains( BookmarkModel::BookmarkUser ) );
  }

  SECTION( "removeBookmark reduces rowCount" )
  {
    QgsBookmarkManager manager;
    const QString id = manager.addBookmark( makeBookmark( QStringLiteral( "ToRemove" ) ) );
    manager.addBookmark( makeBookmark( QStringLiteral( "ToKeep" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
    model.removeBookmark( id );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( BookmarkModel::BookmarkName ).toString() == QStringLiteral( "ToKeep" ) );
  }

  SECTION( "updateBookmarkDetails changes name and group" )
  {
    QgsBookmarkManager manager;
    const QString id = manager.addBookmark( makeBookmark( QStringLiteral( "OldName" ), QStringLiteral( "OldGroup" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    model.updateBookmarkDetails( id, QStringLiteral( "NewName" ), QStringLiteral( "NewGroup" ) );
    REQUIRE( model.index( 0, 0 ).data( BookmarkModel::BookmarkName ).toString() == QStringLiteral( "NewName" ) );
    REQUIRE( model.index( 0, 0 ).data( BookmarkModel::BookmarkGroup ).toString() == QStringLiteral( "NewGroup" ) );
  }

  SECTION( "getBookmarkPoint returns center of bookmark extent" )
  {
    QgsBookmarkManager manager;
    QgsBookmark bm;
    bm.setName( QStringLiteral( "GP" ) );
    bm.setExtent( QgsReferencedRectangle( QgsRectangle( 2.0, 4.0, 6.0, 8.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
    const QString id = manager.addBookmark( bm );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    const QgsPoint pt = model.getBookmarkPoint( id );
    REQUIRE( pt.x() == Catch::Approx( 4.0 ) );
    REQUIRE( pt.y() == Catch::Approx( 6.0 ) );
  }

  SECTION( "getBookmarkCrs returns CRS of bookmark extent" )
  {
    QgsBookmarkManager manager;
    QgsBookmark bm;
    bm.setName( QStringLiteral( "CrsGet" ) );
    bm.setExtent( QgsReferencedRectangle( QgsRectangle( 0.0, 0.0, 1.0, 1.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) ) );
    const QString id = manager.addBookmark( bm );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    REQUIRE( model.getBookmarkCrs( id ).authid() == QStringLiteral( "EPSG:3857" ) );
  }

  SECTION( "setMapSettings emits mapSettingsChanged only on actual change" )
  {
    QgsBookmarkManager manager;
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    QSignalSpy spy( &model, &BookmarkModel::mapSettingsChanged );
    model.setMapSettings( nullptr ); // already nullptr, no change
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "removing non-existent bookmark id is a no-op" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "Stable" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    model.removeBookmark( QStringLiteral( "does-not-exist" ) );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
  }
}
