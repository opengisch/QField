#include "catch2.h"
#include "identifytool.h"
#include "multifeaturelistmodel.h"
#include "qgsquickmapsettings.h"

#include <QPointF>
#include <QSettings>
#include <QSignalSpy>
#include <qgsgeometry.h>
#include <qgsvectorlayer.h>

TEST_CASE( "IdentifyTool" )
{
  IdentifyTool tool;
  QgsQuickMapSettings mapSettings;
  MultiFeatureListModel model;

  SECTION( "validation checks" )
  {
    QPointF point( 100, 100 );

    tool.setModel( &model );
    REQUIRE_NOTHROW( tool.identify( point ) );

    tool.setModel( nullptr );
    tool.setMapSettings( &mapSettings );
    REQUIRE_NOTHROW( tool.identify( point ) );
  }

  SECTION( "deactivation prevents identification" )
  {
    tool.setMapSettings( &mapSettings );
    tool.setModel( &model );
    QSignalSpy finishedSpy( &tool, &IdentifyTool::identifyFinished );

    tool.setDeactivated( true );
    REQUIRE( tool.deactivated() );

    // Should not crash even tho it will try to clear model
    tool.identify( QPointF( 100, 100 ) );
    REQUIRE( finishedSpy.count() == 0 );
  }

  SECTION( "signal emissions" )
  {
    QSignalSpy mapSettingsSpy( &tool, &IdentifyTool::mapSettingsChanged );
    QSignalSpy modelSpy( &tool, &IdentifyTool::modelChanged );
    QSignalSpy radiusSpy( &tool, &IdentifyTool::searchRadiusMmChanged );

    tool.setMapSettings( &mapSettings );
    tool.setModel( &model );
    tool.setSearchRadiusMm( 15.0 );

    REQUIRE( mapSettingsSpy.count() == 1 );
    REQUIRE( modelSpy.count() == 1 );
    REQUIRE( radiusSpy.count() == 1 );
  }

  SECTION( "identify completion emits signal" )
  {
    tool.setMapSettings( &mapSettings );
    tool.setModel( &model );

    QSignalSpy spy( &tool, &IdentifyTool::identifyFinished );
    tool.identify( QPointF( 0, 0 ) );

    REQUIRE( spy.count() == 1 );
  }

  SECTION( "search radius management" )
  {
    QSignalSpy radiusSpy( &tool, &IdentifyTool::searchRadiusMmChanged );

    tool.setSearchRadiusMm( 7.5 );
    REQUIRE( tool.searchRadiusMm() == 7.5 );
    REQUIRE( radiusSpy.count() == 1 );

    // Setting same value should not emit again
    tool.setSearchRadiusMm( 7.5 );
    REQUIRE( tool.searchRadiusMm() == 7.5 );
    REQUIRE( radiusSpy.count() == 1 );
  }
}

TEST_CASE( "IdentifyTool vector layer identification" )
{
  IdentifyTool tool;
  QgsQuickMapSettings mapSettings;
  MultiFeatureListModel model;

  tool.setMapSettings( &mapSettings );
  tool.setModel( &model );

  // Configureing map settings for deterministic search radius-
  mapSettings.setExtent( QgsRectangle( -1, -1, 1, 1 ) );
  mapSettings.setOutputSize( QSize( 512, 512 ) );
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );

  SECTION( "identifies features within search radius" )
  {
    std::unique_ptr<QgsVectorLayer> layer( new QgsVectorLayer( "Point?crs=epsg:4326&field=name:string", "test", "memory" ) );
    REQUIRE( layer->isValid() );

    QgsFeature f1( layer->fields() );
    f1.setAttribute( "name", "Feature1" );
    f1.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 0, 0 ) ) );

    QgsFeature f2( layer->fields() );
    f2.setAttribute( "name", "Feature2" );
    f2.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 0.0001, 0.0001 ) ) );

    layer->startEditing();
    layer->addFeature( f1 );
    layer->addFeature( f2 );
    layer->commitChanges();

    QgsPointXY testPoint( 0, 0 );
    QSettings().setValue( "/QField/identify/limit", 10 );
    QList<IdentifyTool::IdentifyResult> results = tool.identifyVectorLayer( layer.get(), testPoint );

    REQUIRE_FALSE( results.isEmpty() );
    REQUIRE( results.size() == 2 );
    REQUIRE( results.first().layer == layer.get() );
  }

  SECTION( "handles null and non spatial layers" )
  {
    QgsPointXY testPoint( 0, 0 );

    REQUIRE( tool.identifyVectorLayer( nullptr, testPoint ).isEmpty() );

    std::unique_ptr<QgsVectorLayer> nonSpatialLayer( new QgsVectorLayer( "None?field=name:string", "test", "memory" ) );
    REQUIRE( nonSpatialLayer->isValid() );
    REQUIRE_FALSE( nonSpatialLayer->isSpatial() );
    REQUIRE( tool.identifyVectorLayer( nonSpatialLayer.get(), testPoint ).isEmpty() );
  }

  SECTION( "coordinate transformation does not crash" )
  {
    std::unique_ptr<QgsVectorLayer> layer( new QgsVectorLayer( "Point?crs=epsg:3857", "test", "memory" ) );
    REQUIRE( layer->isValid() );

    QgsPointXY point( 0, 0 );
    REQUIRE_NOTHROW( tool.identifyVectorLayer( layer.get(), point ) );
  }
}
