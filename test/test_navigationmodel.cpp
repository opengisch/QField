/***************************************************************************
                        test_navigationmodel
                        --------------------
  begin                : September 2026
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
#include "qfnavigation.h"
#include "qfnavigationmodel.h"

#include <QCoreApplication>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include <qgspoint.h>
#include <qgsproject.h>
#include <qgsquickmapsettings.h>
#include <qgsvectorlayer.h>

#include <cmath>


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


TEST_CASE( "NavigationModel: data roles" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QfNavigationModel model;
  model.setDestination( QgsPoint( 7.0, 46.0 ) );

  SECTION( "Point role returns the destination as a point geometry" )
  {
    const QgsGeometry geometry = model.index( 0, 0 ).data( QfNavigationModel::Point ).value<QgsGeometry>();
    REQUIRE_FALSE( geometry.isNull() );

    const QgsPointXY point = geometry.asPoint();
    INFO( "geometry point (" << QString::number( point.x(), 'f', 10 ) << ", " << QString::number( point.y(), 'f', 10 ) << ")" );
    REQUIRE( point.x() == 7.0 );
    REQUIRE( point.y() == 46.0 );
  }

  SECTION( "PointType role marks the single point as the destination" )
  {
    const int pointType = model.index( 0, 0 ).data( QfNavigationModel::PointType ).toInt();
    CAPTURE( pointType, static_cast<int>( QfNavigationModel::Destination ) );
    REQUIRE( pointType == QfNavigationModel::Destination );
  }

  SECTION( "an out-of-range row is an invalid variant" )
  {
    REQUIRE_FALSE( model.data( model.index( 1, 0 ), QfNavigationModel::Point ).isValid() );
    REQUIRE_FALSE( model.data( model.index( -1, 0 ), QfNavigationModel::Point ).isValid() );
  }

  SECTION( "an unhandled role is an invalid variant" )
  {
    REQUIRE_FALSE( model.index( 0, 0 ).data( Qt::UserRole + 999 ).isValid() );
  }

  SECTION( "roleNames exposes the custom role byte arrays" )
  {
    const QHash<int, QByteArray> roleNames = model.roleNames();
    REQUIRE( roleNames.value( QfNavigationModel::Point ) == QByteArray( "Point" ) );
    REQUIRE( roleNames.value( QfNavigationModel::PointType ) == QByteArray( "PointType" ) );
  }
}


TEST_CASE( "NavigationModel: setCrs" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  const QgsCoordinateReferenceSystem wgs84( QStringLiteral( "EPSG:4326" ) );
  const QgsCoordinateReferenceSystem webMercator( QStringLiteral( "EPSG:3857" ) );

  SECTION( "reprojects stored points into the new coordinate reference system" )
  {
    QfNavigationModel model;
    model.setCrs( wgs84 );
    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );
    model.setCrs( webMercator );

    const QgsPoint reprojected = model.destination();
    INFO( "reprojected " << describePoint( reprojected ) << " expected (779236.4358021, 5780349.2199191)" );
    CAPTURE( model.rowCount( QModelIndex() ), destinationChangedSpy.count() );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( destinationChangedSpy.count() == 1 );
    REQUIRE( reprojected.x() == Catch::Approx( 779236.4358021 ).margin( 1e-3 ) );
    REQUIRE( reprojected.y() == Catch::Approx( 5780349.2199191 ).margin( 1e-3 ) );
  }

  SECTION( "setting the identical crs is a no-op that leaves points untouched" )
  {
    QfNavigationModel model;
    model.setCrs( wgs84 );
    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );
    model.setCrs( wgs84 );

    INFO( "destination " << describePoint( model.destination() ) );
    CAPTURE( destinationChangedSpy.count() );
    REQUIRE( destinationChangedSpy.count() == 0 );
    REQUIRE( model.destination().x() == 7.0 );
    REQUIRE( model.destination().y() == 46.0 );
  }

  SECTION( "changing the crs on an empty model stores it without touching rows" )
  {
    QfNavigationModel model;

    QSignalSpy destinationChangedSpy( &model, &QfNavigationModel::destinationChanged );
    model.setCrs( webMercator );

    CAPTURE( destinationChangedSpy.count(), model.rowCount( QModelIndex() ) );
    REQUIRE( destinationChangedSpy.count() == 0 );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }
}


TEST_CASE( "NavigationModel: clear" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QfNavigationModel model;
  model.setDestination( QgsPoint( 7.0, 46.0 ) );
  REQUIRE( model.rowCount( QModelIndex() ) == 1 );

  SECTION( "empties the model and resets it" )
  {
    QSignalSpy modelResetSpy( &model, &QAbstractItemModel::modelReset );
    model.clear();

    CAPTURE( modelResetSpy.count(), model.rowCount( QModelIndex() ) );
    REQUIRE( modelResetSpy.count() == 1 );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    REQUIRE( model.destination().isEmpty() );
  }

  SECTION( "removes the persisted navigation settings" )
  {
    model.clear();

    QSettings settings;
    const bool pointsKeyPresent = settings.contains( navigationPointsKey );
    CAPTURE( pointsKeyPresent );
    REQUIRE_FALSE( pointsKeyPresent );
  }
}


TEST_CASE( "NavigationModel: persistence" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  SECTION( "setDestination writes the point and crs to settings" )
  {
    QfNavigationModel model;
    model.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
    model.setDestination( QgsPoint( 7.0, 46.0 ) );

    QSettings settings;
    const QStringList persistedPoints = settings.value( navigationPointsKey ).toStringList();
    const QString persistedCrs = settings.value( navigationCrsKey ).toString();
    const QString expectedWkt = QgsPoint( 7.0, 46.0 ).asWkt();

    INFO( "persisted points=[" << persistedPoints.join( QStringLiteral( "; " ) ) << "] expectedWkt=" << expectedWkt );
    CAPTURE( persistedPoints.size(), persistedCrs.isEmpty() );
    REQUIRE( persistedPoints.size() == 1 );
    REQUIRE( persistedPoints.first() == expectedWkt );
    REQUIRE_FALSE( persistedCrs.isEmpty() );
  }

  SECTION( "restore rebuilds the destination saved by a previous model" )
  {
    {
      QfNavigationModel savingModel;
      savingModel.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
      savingModel.setDestination( QgsPoint( 7.0, 46.0 ) );
    }

    QfNavigationModel restoringModel;
    REQUIRE( restoringModel.rowCount( QModelIndex() ) == 0 );

    restoringModel.restore();

    const QgsPoint restored = restoringModel.destination();
    INFO( "restored " << describePoint( restored ) );
    CAPTURE( restoringModel.rowCount( QModelIndex() ) );
    REQUIRE( restoringModel.rowCount( QModelIndex() ) == 1 );
    REQUIRE( restored.x() == 7.0 );
    REQUIRE( restored.y() == 46.0 );
  }

  SECTION( "restore does nothing when no navigation settings are stored" )
  {
    QfNavigationModel model;
    model.restore();
    CAPTURE( model.rowCount( QModelIndex() ) );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }
}


TEST_CASE( "Navigation: destination point" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
  mapSettings.setOutputSize( QSize( 1000, 500 ) );
  mapSettings.setExtent( QgsRectangle( 0.0, 0.0, 20.0, 60.0 ) );

  QfNavigation navigation;
  navigation.setMapSettings( &mapSettings );

  SECTION( "is inactive until a destination is set" )
  {
    REQUIRE_FALSE( navigation.isActive() );
    REQUIRE( navigation.destination().isEmpty() );
  }

  SECTION( "setting a destination point activates navigation" )
  {
    QSignalSpy destinationChangedSpy( &navigation, &QfNavigation::destinationChanged );
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );

    INFO( "destination " << describePoint( navigation.destination() ) );
    CAPTURE( destinationChangedSpy.count() );
    REQUIRE( navigation.isActive() );
    REQUIRE( destinationChangedSpy.count() == 1 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 8.0 ) );
    REQUIRE( navigation.destination().y() == Catch::Approx( 47.0 ) );
  }

  SECTION( "clear resets the destination and deactivates navigation" )
  {
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );
    REQUIRE( navigation.isActive() );

    navigation.clear();
    REQUIRE_FALSE( navigation.isActive() );
    REQUIRE( navigation.destination().isEmpty() );
  }
}


TEST_CASE( "Navigation: distance and bearing" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
  mapSettings.setOutputSize( QSize( 1000, 500 ) );
  mapSettings.setExtent( QgsRectangle( 0.0, 0.0, 20.0, 60.0 ) );

  QfNavigation navigation;
  navigation.setMapSettings( &mapSettings );

  SECTION( "details are undefined without a location" )
  {
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );
    CAPTURE( navigation.distance(), navigation.bearing() );
    REQUIRE( std::isnan( navigation.distance() ) );
    REQUIRE( std::isnan( navigation.bearing() ) );
  }

  SECTION( "a location and destination yield a finite distance and the exact bearing" )
  {
    navigation.setLocation( QgsPoint( 7.0, 46.0 ) );
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );

    const double distance = navigation.distance();
    const double bearing = navigation.bearing();
    INFO( "distance=" << QString::number( distance, 'f', 6 ) << " bearing=" << QString::number( bearing, 'f', 6 ) );
    CAPTURE( distance, bearing );
    REQUIRE( std::isfinite( distance ) );
    REQUIRE( distance > 0.0 );
    REQUIRE( bearing == Catch::Approx( 45.0 ).margin( 1e-6 ) );
  }

  SECTION( "bearing is measured clockwise from north" )
  {
    navigation.setLocation( QgsPoint( 7.0, 46.0 ) );

    navigation.setDestination( QgsPoint( 7.0, 47.0 ) );
    INFO( "north bearing=" << QString::number( navigation.bearing(), 'f', 6 ) );
    REQUIRE( navigation.bearing() == Catch::Approx( 0.0 ).margin( 1e-6 ) );

    navigation.setDestination( QgsPoint( 8.0, 46.0 ) );
    INFO( "east bearing=" << QString::number( navigation.bearing(), 'f', 6 ) );
    REQUIRE( navigation.bearing() == Catch::Approx( 90.0 ).margin( 1e-6 ) );
  }

  SECTION( "vertical distance is undefined when the points carry no z value" )
  {
    navigation.setLocation( QgsPoint( 7.0, 46.0 ) );
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );
    CAPTURE( navigation.verticalDistance() );
    REQUIRE( std::isnan( navigation.verticalDistance() ) );
  }

  SECTION( "vertical distance is the z delta when both points carry a z value" )
  {
    navigation.setLocation( QgsPoint( 7.0, 46.0, 100.0 ) );
    navigation.setDestination( QgsPoint( 8.0, 47.0, 250.0 ) );
    const double verticalDistance = navigation.verticalDistance();
    INFO( "verticalDistance=" << QString::number( verticalDistance, 'f', 6 ) );
    REQUIRE( verticalDistance == Catch::Approx( 150.0 ).margin( 1e-6 ) );
  }

  SECTION( "an empty location clears the computed details" )
  {
    navigation.setLocation( QgsPoint( 7.0, 46.0 ) );
    navigation.setDestination( QgsPoint( 8.0, 47.0 ) );
    REQUIRE( std::isfinite( navigation.distance() ) );

    navigation.setLocation( QgsPoint() );
    CAPTURE( navigation.distance() );
    REQUIRE( std::isnan( navigation.distance() ) );
  }
}
TEST_CASE( "Navigation: feature as destination" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
  mapSettings.setOutputSize( QSize( 1000, 500 ) );
  mapSettings.setExtent( QgsRectangle( 0.0, 0.0, 20.0, 60.0 ) );

  QfNavigation navigation;
  navigation.setMapSettings( &mapSettings );

  SECTION( "a point feature becomes the destination with a single vertex" )
  {
    QgsVectorLayer layer( QStringLiteral( "Point?crs=epsg:4326&field=name:string" ), QStringLiteral( "points" ), QStringLiteral( "memory" ) );
    REQUIRE( layer.isValid() );

    QgsFeature feature( layer.fields() );
    feature.setAttribute( QStringLiteral( "name" ), QStringLiteral( "Target" ) );
    feature.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 8.0, 47.0 ) ) );

    navigation.setDestinationFeature( feature, &layer );

    INFO( "destination " << describePoint( navigation.destination() ) );
    CAPTURE( navigation.destinationFeatureVertexCount(), navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.isActive() );
    REQUIRE( navigation.destinationFeatureVertexCount() == 1 );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 0 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 8.0 ) );
    REQUIRE( navigation.destination().y() == Catch::Approx( 47.0 ) );
  }

  SECTION( "an empty geometry feature is ignored" )
  {
    QgsVectorLayer layer( QStringLiteral( "Point?crs=epsg:4326&field=name:string" ), QStringLiteral( "points" ), QStringLiteral( "memory" ) );
    REQUIRE( layer.isValid() );

    QgsFeature feature( layer.fields() );
    feature.setAttribute( QStringLiteral( "name" ), QStringLiteral( "Empty" ) );

    navigation.setDestinationFeature( feature, &layer );

    REQUIRE_FALSE( navigation.isActive() );
    REQUIRE( navigation.destinationFeatureVertexCount() == 0 );
  }

  SECTION( "a line feature exposes its centroid and vertices as destinations" )
  {
    QgsVectorLayer layer( QStringLiteral( "LineString?crs=epsg:4326&field=name:string" ), QStringLiteral( "lines" ), QStringLiteral( "memory" ) );
    REQUIRE( layer.isValid() );

    QgsPolylineXY line;
    line << QgsPointXY( 2.0, 2.0 ) << QgsPointXY( 4.0, 2.0 ) << QgsPointXY( 6.0, 2.0 );
    QgsFeature feature( layer.fields() );
    feature.setAttribute( QStringLiteral( "name" ), QStringLiteral( "Route" ) );
    feature.setGeometry( QgsGeometry::fromPolylineXY( line ) );

    navigation.setDestinationFeature( feature, &layer );

    CAPTURE( navigation.destinationFeatureVertexCount(), navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.isActive() );
    REQUIRE( navigation.destinationFeatureVertexCount() == 3 );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 0 );

    navigation.nextDestinationVertex();
    INFO( "first vertex destination " << describePoint( navigation.destination() ) );
    CAPTURE( navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 1 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 2.0 ) );
    REQUIRE( navigation.destination().y() == Catch::Approx( 2.0 ) );

    navigation.nextDestinationVertex();
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 2 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 4.0 ) );

    navigation.nextDestinationVertex();
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 3 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 6.0 ) );

    navigation.nextDestinationVertex();
    CAPTURE( navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 0 );
  }

  SECTION( "previousDestinationVertex wraps from the centroid to the last vertex" )
  {
    QgsVectorLayer layer( QStringLiteral( "LineString?crs=epsg:4326&field=name:string" ), QStringLiteral( "lines" ), QStringLiteral( "memory" ) );
    REQUIRE( layer.isValid() );

    QgsPolylineXY line;
    line << QgsPointXY( 2.0, 2.0 ) << QgsPointXY( 4.0, 2.0 ) << QgsPointXY( 6.0, 2.0 );
    QgsFeature feature( layer.fields() );
    feature.setGeometry( QgsGeometry::fromPolylineXY( line ) );

    navigation.setDestinationFeature( feature, &layer );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 0 );

    navigation.previousDestinationVertex();
    CAPTURE( navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == 3 );
    REQUIRE( navigation.destination().x() == Catch::Approx( 6.0 ) );
  }

  SECTION( "clearDestinationFeature resets the feature vertex state" )
  {
    QgsVectorLayer layer( QStringLiteral( "Point?crs=epsg:4326&field=name:string" ), QStringLiteral( "points" ), QStringLiteral( "memory" ) );
    REQUIRE( layer.isValid() );

    QgsFeature feature( layer.fields() );
    feature.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 8.0, 47.0 ) ) );
    navigation.setDestinationFeature( feature, &layer );
    REQUIRE( navigation.destinationFeatureVertexCount() == 1 );

    navigation.clearDestinationFeature();
    CAPTURE( navigation.destinationFeatureVertexCount(), navigation.destinationFeatureCurrentVertex() );
    REQUIRE( navigation.destinationFeatureVertexCount() == 0 );
    REQUIRE( navigation.destinationFeatureCurrentVertex() == -1 );
  }
}


TEST_CASE( "Navigation: proximity alarm" )
{
  QTemporaryDir settingsDirectory;
  REQUIRE( settingsDirectory.isValid() );
  const ScopedIniSettings scopedSettings( settingsDirectory.path() );

  QfNavigation navigation;

  SECTION( "proximity alarm is disabled by default" )
  {
    REQUIRE_FALSE( navigation.proximityAlarm() );
  }

  SECTION( "enabling the proximity alarm emits and round-trips" )
  {
    QSignalSpy spy( &navigation, &QfNavigation::proximityAlarmChanged );
    navigation.setProximityAlarm( true );
    CAPTURE( spy.count() );
    REQUIRE( navigation.proximityAlarm() );
    REQUIRE( spy.count() == 1 );

    navigation.setProximityAlarm( true );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "the proximity alarm threshold emits and round-trips" )
  {
    QSignalSpy spy( &navigation, &QfNavigation::proximityAlarmThresholdChanged );
    navigation.setProximityAlarmThreshold( 25.0 );
    CAPTURE( navigation.proximityAlarmThreshold(), spy.count() );
    REQUIRE( navigation.proximityAlarmThreshold() == Catch::Approx( 25.0 ) );
    REQUIRE( spy.count() == 1 );

    navigation.setProximityAlarmThreshold( 25.0 );
    REQUIRE( spy.count() == 1 );
  }
}
