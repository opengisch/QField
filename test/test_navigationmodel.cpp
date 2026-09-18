/***************************************************************************
                        test_navigationmodel
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

#include "catch2.h"
#include "qfnavigationmodel.h"

#include <QCoreApplication>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <qgscoordinatereferencesystem.h>
#include <qgsgeometry.h>
#include <qgspoint.h>


class ScopedIniSettings
{
  public:
    explicit ScopedIniSettings( const QString &root )
      : mPreviousDefaultFormat( QSettings::defaultFormat() )
      , mPreviousOrganizationName( QCoreApplication::organizationName() )
      , mPreviousApplicationName( QCoreApplication::applicationName() )
    {
      QSettings::setDefaultFormat( QSettings::IniFormat );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, root );
      QCoreApplication::setOrganizationName( QStringLiteral( "QFieldUnitTests" ) );
      QCoreApplication::setApplicationName( QStringLiteral( "NavigationModelTests" ) );
    }

    ~ScopedIniSettings()
    {
      QCoreApplication::setOrganizationName( mPreviousOrganizationName );
      QCoreApplication::setApplicationName( mPreviousApplicationName );
      QSettings::setDefaultFormat( mPreviousDefaultFormat );
    }

  private:
    QSettings::Format mPreviousDefaultFormat;
    QString mPreviousOrganizationName;
    QString mPreviousApplicationName;
};


static const QLatin1String navigationPointsKey( "/QField/navigation/points" );
static const QLatin1String navigationCrsKey( "/QField/navigation/crs" );


static QString describePoint( const QgsPoint &point )
{
  return QStringLiteral( "(%1, %2)" )
  .arg( point.x(), 0, 'f', 10 )
    .arg( point.y(), 0, 'f', 10 );
}


TEST_CASE( "NavigationModel: empty model" )
{
  QfNavigationModel model;

  SECTION( "reports no rows and an empty destination" )
  {
    CAPTURE( model.rowCount( QModelIndex() ) );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    REQUIRE( model.points().isEmpty() );
    REQUIRE( model.destination().isEmpty() );
  }

  SECTION( "data on any index is an invalid variant" )
  {
    REQUIRE_FALSE( model.data( model.index( 0, 0 ), QfNavigationModel::Point ).isValid() );
    REQUIRE_FALSE( model.data( QModelIndex(), QfNavigationModel::Point ).isValid() );
  }
}


TEST_CASE( "NavigationModel: setDestination" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QfNavigationModel model;

  SECTION( "first call inserts one row and announces the destination" )
  {
    QSignalSpy rowsInsertedSpy( &model, &QAbstractItemModel::rowsInserted );
    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );

    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    INFO( "destination " << describePoint( model.destination() ) );
    CAPTURE( model.rowCount( QModelIndex() ), rowsInsertedSpy.count(), destinationChangedSpy.count() );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( rowsInsertedSpy.count() == 1 );
    REQUIRE( destinationChangedSpy.count() == 1 );
    REQUIRE( model.destination().x() == 7.0 );
    REQUIRE( model.destination().y() == 46.0 );
  }

  SECTION( "an unchanged destination is a no-op that emits nothing" )
  {
    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    QSignalSpy rowsInsertedSpy( &model, &QAbstractItemModel::rowsInserted );
    QSignalSpy dataChangedSpy( &model, &QAbstractItemModel::dataChanged );
    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );

    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    CAPTURE( rowsInsertedSpy.count(), dataChangedSpy.count(), destinationChangedSpy.count() );
    REQUIRE( rowsInsertedSpy.count() == 0 );
    REQUIRE( dataChangedSpy.count() == 0 );
    REQUIRE( destinationChangedSpy.count() == 0 );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
  }

  SECTION( "a changed destination replaces the point in place without adding a row" )
  {
    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    QSignalSpy rowsInsertedSpy( &model, &QAbstractItemModel::rowsInserted );
    QSignalSpy dataChangedSpy( &model, &QAbstractItemModel::dataChanged );
    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );

    model.setDestination( QgsPoint( 8.0, 47.0 ) );

    INFO( "destination " << describePoint( model.destination() ) );
    CAPTURE( model.rowCount( QModelIndex() ), rowsInsertedSpy.count(), dataChangedSpy.count(), destinationChangedSpy.count() );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( rowsInsertedSpy.count() == 0 );
    REQUIRE( dataChangedSpy.count() == 1 );
    REQUIRE( destinationChangedSpy.count() == 1 );
    REQUIRE( model.destination().x() == 8.0 );
    REQUIRE( model.destination().y() == 47.0 );
  }
}
