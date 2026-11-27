/***************************************************************************
                        test_snappingutils
                        --------------------
  begin                : Nov 2026
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
#include "qgsquickmapsettings.h"
#include "utils/qfsnappingutils.h"

#include <QSignalSpy>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include <qgsproject.h>
#include <qgsrectangle.h>
#include <qgssnappingconfig.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorlayer.h>

#include <cmath>
#include <memory>

static std::unique_ptr<QgsVectorLayer> createLayer( const QString &uri, const QString &wkt )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( uri, QStringLiteral( "layer" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature feature( layer->fields() );
  feature.setGeometry( QgsGeometry::fromWkt( wkt ) );
  REQUIRE( layer->dataProvider()->addFeature( feature ) );
  return layer;
}

static QgsSnappingConfig snappingConfig( Qgis::SnappingMode mode, Qgis::SnappingType type )
{
  QgsSnappingConfig config;
  config.setEnabled( true );
  config.setMode( mode );
  config.setTypeFlag( type );
  config.setTolerance( 10 );
  config.setUnits( Qgis::MapToolUnit::Pixels );
  return config;
}

static void setupMapSettings( QgsQuickMapSettings &mapSettings, const QList<QgsMapLayer *> &layers )
{
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
  mapSettings.setOutputSize( QSize( 100, 100 ) );
  mapSettings.setOutputDpi( 96 );
  mapSettings.setLayers( layers );
  mapSettings.setExtent( QgsRectangle( 0, 0, 100, 100 ) );
}

TEST_CASE( "SnappingUtils NewPoint" )
{
  SECTION( "TwoDimensionalTypeDropsZAndM" )
  {
    REQUIRE( QfSnappingUtils::newPoint( QgsPoint( 1, 2, 3, 4 ), Qgis::WkbType::LineString ).asWkt() == QStringLiteral( "Point (1 2)" ) );
  }

  SECTION( "ZTypeKeepsZ" )
  {
    REQUIRE( QfSnappingUtils::newPoint( QgsPoint( 1, 2, 3, 4 ), Qgis::WkbType::LineStringZ ).asWkt() == QStringLiteral( "Point Z (1 2 3)" ) );
  }

  SECTION( "MTypeKeepsM" )
  {
    REQUIRE( QfSnappingUtils::newPoint( QgsPoint( 1, 2, 3, 4 ), Qgis::WkbType::LineStringM ).asWkt() == QStringLiteral( "Point M (1 2 4)" ) );
  }

  SECTION( "ZMTypeKeepsZAndM" )
  {
    REQUIRE( QfSnappingUtils::newPoint( QgsPoint( 1, 2, 3, 4 ), Qgis::WkbType::PolygonZM ).asWkt() == QStringLiteral( "Point ZM (1 2 3 4)" ) );
  }

  SECTION( "MissingZAndMStayNaN" )
  {
    const QgsPoint point = QfSnappingUtils::newPoint( QgsPoint( 1, 2 ), Qgis::WkbType::PolygonZM );
    REQUIRE( point.wkbType() == Qgis::WkbType::PointZM );
    REQUIRE( point.x() == 1 );
    REQUIRE( point.y() == 2 );
    REQUIRE( std::isnan( point.z() ) );
    REQUIRE( std::isnan( point.m() ) );
  }
}

TEST_CASE( "SnappingUtils Properties" )
{
  SECTION( "DefaultState" )
  {
    const QfSnappingUtils snappingUtils;
    REQUIRE_FALSE( snappingUtils.enabled() );
    REQUIRE( snappingUtils.mapSettings() == nullptr );
    REQUIRE( snappingUtils.currentLayer() == nullptr );
    REQUIRE( snappingUtils.inputCoordinate() == QPointF() );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "NotifyOnlyOnChange" )
  {
    std::unique_ptr<QgsVectorLayer> lines = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (10 10, 50 10, 50 40)" ) );
    QgsQuickMapSettings mapSettings;
    QfSnappingUtils snappingUtils;

    QSignalSpy mapSettingsSpy( &snappingUtils, &QfSnappingUtils::mapSettingsChanged );
    QSignalSpy enabledSpy( &snappingUtils, &QfSnappingUtils::enabledChanged );
    QSignalSpy currentLayerSpy( &snappingUtils, &QfSnappingUtils::currentLayerChanged );
    QSignalSpy inputCoordinateSpy( &snappingUtils, &QfSnappingUtils::inputCoordinateChanged );

    snappingUtils.setMapSettings( &mapSettings );
    snappingUtils.setMapSettings( &mapSettings );
    snappingUtils.setEnabled( true );
    snappingUtils.setEnabled( true );
    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setInputCoordinate( QPointF( 20, 20 ) );
    snappingUtils.setInputCoordinate( QPointF( 20, 20 ) );

    REQUIRE( mapSettingsSpy.count() == 1 );
    REQUIRE( enabledSpy.count() == 1 );
    REQUIRE( currentLayerSpy.count() == 1 );
    REQUIRE( inputCoordinateSpy.count() == 1 );
    REQUIRE( snappingUtils.mapSettings() == &mapSettings );
    REQUIRE( snappingUtils.enabled() );
    REQUIRE( snappingUtils.currentLayer() == lines.get() );
    REQUIRE( snappingUtils.inputCoordinate() == QPointF( 20, 20 ) );
  }
}

TEST_CASE( "SnappingUtils" )
{
  std::unique_ptr<QgsVectorLayer> lines = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (10 10, 50 10, 50 40)" ) );

  QfSnappingUtils snappingUtils;
  snappingUtils.setConfig( snappingConfig( Qgis::SnappingMode::AllLayers, Qgis::SnappingType::Vertex ) );

  QgsQuickMapSettings mapSettings;
  snappingUtils.setMapSettings( &mapSettings );
  setupMapSettings( mapSettings, { lines.get() } );

  SECTION( "DisabledDoesNotSnap" )
  {
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "EnablingSnapsAtInputCoordinate" )
  {
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );

    QSignalSpy snappingResultSpy( &snappingUtils, &QfSnappingUtils::snappingResultChanged );
    snappingUtils.setEnabled( true );

    REQUIRE( snappingResultSpy.count() == 1 );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (50 10)" ) );
  }

  SECTION( "SnapsToVertex" )
  {
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );

    const QfSnappingResult result = snappingUtils.snappingResult();
    REQUIRE( result.type() == QfSnappingResult::Vertex );
    REQUIRE( result.point().asWkt() == QStringLiteral( "Point (50 10)" ) );
    REQUIRE( result.distance() == 3 );
    REQUIRE( result.vertexIndex() == 1 );
    REQUIRE( result.featureId() == 1 );
    REQUIRE( result.layer() == lines.get() );
  }

  SECTION( "SnapsToEdge" )
  {
    snappingUtils.setConfig( snappingConfig( Qgis::SnappingMode::AllLayers, Qgis::SnappingType::Segment ) );
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 30, 87 ) );

    const QfSnappingResult result = snappingUtils.snappingResult();
    REQUIRE( result.type() == QfSnappingResult::Edge );
    REQUIRE( result.point().asWkt() == QStringLiteral( "Point (30 10)" ) );
    REQUIRE( result.distance() == 3 );
    REQUIRE( result.vertexIndex() == 0 );
  }

  SECTION( "ToleranceLimit" )
  {
    snappingUtils.setEnabled( true );

    snappingUtils.setInputCoordinate( QPointF( 60, 90 ) );
    REQUIRE( snappingUtils.snappingResult().distance() == 10 );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (50 10)" ) );

    snappingUtils.setInputCoordinate( QPointF( 61, 90 ) );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "DisablingClearsResult" )
  {
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE( snappingUtils.snappingResult().isValid() );

    QSignalSpy snappingResultSpy( &snappingUtils, &QfSnappingUtils::snappingResultChanged );
    snappingUtils.setEnabled( false );

    REQUIRE( snappingResultSpy.count() == 1 );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "PanningResnaps" )
  {
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE( snappingUtils.snappingResult().isValid() );

    QSignalSpy snappingResultSpy( &snappingUtils, &QfSnappingUtils::snappingResultChanged );
    mapSettings.setExtent( QgsRectangle( 20, 0, 120, 100 ) );

    REQUIRE( snappingResultSpy.count() == 1 );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "RemovingMapLayersResnaps" )
  {
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE( snappingUtils.snappingResult().isValid() );

    QSignalSpy snappingResultSpy( &snappingUtils, &QfSnappingUtils::snappingResultChanged );
    mapSettings.setLayers( {} );

    REQUIRE( snappingResultSpy.count() == 1 );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );
  }

  SECTION( "AllLayersModeSnapsToClosestLayer" )
  {
    std::unique_ptr<QgsVectorLayer> others = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (54 10, 90 10)" ) );
    mapSettings.setLayers( { lines.get(), others.get() } );
    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );

    REQUIRE( snappingUtils.snappingResult().layer() == others.get() );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (54 10)" ) );
    REQUIRE( snappingUtils.snappingResult().distance() == 1 );
  }

  SECTION( "ActiveLayerModeSnapsToCurrentLayerOnly" )
  {
    std::unique_ptr<QgsVectorLayer> others = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (54 10, 90 10)" ) );
    mapSettings.setLayers( { lines.get(), others.get() } );
    snappingUtils.setConfig( snappingConfig( Qgis::SnappingMode::ActiveLayer, Qgis::SnappingType::Vertex ) );
    snappingUtils.setEnabled( true );

    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE_FALSE( snappingUtils.snappingResult().isValid() );

    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setInputCoordinate( QPointF( 52, 90 ) );
    REQUIRE( snappingUtils.snappingResult().layer() == lines.get() );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (50 10)" ) );
    REQUIRE( snappingUtils.snappingResult().distance() == 2 );
  }

  SECTION( "IndexingSignals" )
  {
    std::unique_ptr<QgsVectorLayer> others = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (54 10, 90 10)" ) );
    mapSettings.setLayers( { lines.get(), others.get() } );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );

    QSignalSpy startedSpy( &snappingUtils, &QfSnappingUtils::indexingStarted );
    QSignalSpy progressSpy( &snappingUtils, &QfSnappingUtils::indexingProgress );
    QSignalSpy finishedSpy( &snappingUtils, &QfSnappingUtils::indexingFinished );
    snappingUtils.setEnabled( true );

    REQUIRE( startedSpy.count() == 1 );
    REQUIRE( startedSpy.at( 0 ).at( 0 ).toInt() == 2 );
    REQUIRE( progressSpy.count() == 1 );
    REQUIRE( progressSpy.at( 0 ).at( 0 ).toInt() == 1 );
    REQUIRE( finishedSpy.count() == 1 );

    snappingUtils.setInputCoordinate( QPointF( 20, 90 ) );

    REQUIRE( startedSpy.count() == 1 );
    REQUIRE( progressSpy.count() == 1 );
    REQUIRE( finishedSpy.count() == 1 );
  }

  SECTION( "RemovingProjectLayerRebuildsIndexes" )
  {
    QgsVectorLayer *projectLines = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (10 10, 50 10, 50 40)" ) ).release();
    QgsVectorLayer *projectOthers = createLayer( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "LineString (54 10, 90 10)" ) ).release();
    QgsProject::instance()->addMapLayers( { projectLines, projectOthers } );
    mapSettings.setLayers( { projectLines, projectOthers } );
    snappingUtils.setEnabled( true );
    snappingUtils.setInputCoordinate( QPointF( 53, 90 ) );
    REQUIRE( snappingUtils.snappingResult().layer() == projectOthers );

    QSignalSpy startedSpy( &snappingUtils, &QfSnappingUtils::indexingStarted );
    QgsProject::instance()->removeMapLayer( projectOthers );
    mapSettings.setLayers( { projectLines } );

    REQUIRE( startedSpy.count() == 1 );
    REQUIRE( startedSpy.at( 0 ).at( 0 ).toInt() == 1 );
    REQUIRE( snappingUtils.snappingResult().layer() == projectLines );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (50 10)" ) );

    QgsProject::instance()->removeAllMapLayers();
  }
}

TEST_CASE( "SnappingUtils ZM" )
{
  std::unique_ptr<QgsVectorLayer> lines = createLayer( QStringLiteral( "LineStringZM?crs=EPSG:3857" ), QStringLiteral( "LineStringZM (10 10 1 100, 50 10 2 200, 50 40 3 300)" ) );

  QfSnappingUtils snappingUtils;
  snappingUtils.setConfig( snappingConfig( Qgis::SnappingMode::AllLayers, Qgis::SnappingType::Vertex ) );

  QgsQuickMapSettings mapSettings;
  snappingUtils.setMapSettings( &mapSettings );
  setupMapSettings( mapSettings, { lines.get() } );
  snappingUtils.setEnabled( true );

  SECTION( "VertexOnCurrentLayerKeepsZAndM" )
  {
    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setInputCoordinate( QPointF( 52, 90 ) );

    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point ZM (50 10 2 200)" ) );
    REQUIRE( snappingUtils.snappingResult().distance() == 2 );
  }

  SECTION( "VertexWithoutCurrentLayerIsTwoDimensional" )
  {
    snappingUtils.setInputCoordinate( QPointF( 52, 90 ) );

    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (50 10)" ) );
  }

  SECTION( "EdgeIsTwoDimensional" )
  {
    snappingUtils.setConfig( snappingConfig( Qgis::SnappingMode::AllLayers, Qgis::SnappingType::Segment ) );
    snappingUtils.setCurrentLayer( lines.get() );
    snappingUtils.setInputCoordinate( QPointF( 30, 87 ) );

    REQUIRE( snappingUtils.snappingResult().type() == QfSnappingResult::Edge );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point (30 10)" ) );
  }

  SECTION( "CurrentLayerDecidesDimensions" )
  {
    std::unique_ptr<QgsVectorLayer> zLines = createLayer( QStringLiteral( "LineStringZ?crs=EPSG:3857" ), QStringLiteral( "LineStringZ (0 90 0, 1 90 0)" ) );
    snappingUtils.setCurrentLayer( zLines.get() );
    snappingUtils.setInputCoordinate( QPointF( 52, 90 ) );

    REQUIRE( snappingUtils.snappingResult().layer() == lines.get() );
    REQUIRE( snappingUtils.snappingResult().point().asWkt() == QStringLiteral( "Point Z (50 10 2)" ) );
  }

  SECTION( "VertexIsReprojectedToMapCrs" )
  {
    std::unique_ptr<QgsVectorLayer> wgs84Lines = createLayer( QStringLiteral( "LineStringZM?crs=EPSG:4326" ), QStringLiteral( "LineStringZM (0 0 1 100, 1 1 2 200, 2 2 3 300)" ) );
    mapSettings.setLayers( { wgs84Lines.get() } );
    mapSettings.setCenter( QgsPoint( 111319.49079327357, 111325.14286638486 ) );
    snappingUtils.setCurrentLayer( wgs84Lines.get() );
    snappingUtils.setInputCoordinate( QPointF( 50, 50 ) );

    REQUIRE( snappingUtils.snappingResult().vertexIndex() == 1 );
    REQUIRE( snappingUtils.snappingResult().point().asWkt( 3 ) == QStringLiteral( "Point ZM (111319.491 111325.143 2 200)" ) );
  }
}
