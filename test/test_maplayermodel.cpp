/***************************************************************************
                        test_maplayermodel
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

#include "catch2.h"
#include "maplayermodel.h"
#include "trackingmodel.h"

#include <QSignalSpy>
#include <qgsfield.h>
#include <qgsmemoryproviderutils.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

static QgsVectorLayer *makeLayer( const QString &name, Qgis::WkbType geomType = Qgis::WkbType::Point )
{
  QgsFields fields;
  fields.append( QgsField( QStringLiteral( "id" ), QMetaType::Type::Int ) );
  QgsVectorLayer *layer = QgsMemoryProviderUtils::createMemoryLayer( name, fields, geomType, QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
  Q_ASSERT( layer && layer->isValid() );
  return layer;
}


TEST_CASE( "MapLayerBaseModel" )
{
  SECTION( "starts empty with no project" )
  {
    MapLayerBaseModel model;
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    REQUIRE( model.project() == nullptr );
    REQUIRE( model.enabled() );
  }

  SECTION( "reflects project layers on setProject" )
  {
    QgsProject project;
    project.addMapLayers( { makeLayer( QStringLiteral( "A" ) ), makeLayer( QStringLiteral( "B" ) ) } );
    MapLayerBaseModel model;
    model.setProject( &project );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
  }

  SECTION( "tracks layer addition and removal via project signals" )
  {
    QgsProject project;
    MapLayerBaseModel model;
    model.setProject( &project );
    QgsVectorLayer *layer = makeLayer( QStringLiteral( "Dynamic" ) );
    const QString layerId = layer->id();
    project.addMapLayer( layer ); // project takes ownership; emits layersAdded
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    project.removeMapLayer( layerId ); // emits layersWillBeRemoved before deletion
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }

  SECTION( "data roles return correct values" )
  {
    QgsProject project;
    QgsVectorLayer *layer = makeLayer( QStringLiteral( "RoleLayer" ), Qgis::WkbType::Polygon );
    project.addMapLayer( layer );
    MapLayerBaseModel model;
    model.setProject( &project );
    const QModelIndex idx = model.index( 0, 0 );
    REQUIRE( model.data( idx, Qt::DisplayRole ).toString() == QStringLiteral( "RoleLayer" ) );
    REQUIRE( model.data( idx, MapLayerModel::NameRole ).toString() == QStringLiteral( "RoleLayer" ) );
    REQUIRE( model.data( idx, MapLayerModel::IdRole ).toString() == layer->id() );
    REQUIRE( model.data( idx, MapLayerModel::LayerRole ).value<QgsMapLayer *>() == layer );
    REQUIRE( model.data( idx, MapLayerModel::LayerTypeRole ).value<Qgis::LayerType>() == Qgis::LayerType::Vector );
    REQUIRE( model.data( idx, MapLayerModel::GeometryTypeRole ).value<Qgis::GeometryType>() == Qgis::GeometryType::Polygon );
  }

  SECTION( "data returns invalid variant for out-of-range index" )
  {
    MapLayerBaseModel model;
    // Empty model: index(0,0) has row=0 >= mLayers.size()=0, returns QVariant()
    REQUIRE_FALSE( model.data( model.index( 0, 0 ), Qt::DisplayRole ).isValid() );
  }

  SECTION( "rowCount returns 0 for a valid parent (list model contract)" )
  {
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "P" ) ) );
    MapLayerBaseModel model;
    model.setProject( &project );
    REQUIRE( model.rowCount( model.index( 0, 0 ) ) == 0 );
  }

  SECTION( "disabled model ignores signal-driven changes; re-enabling resets from project" )
  {
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "Pre" ) ) );
    MapLayerBaseModel model;
    model.setProject( &project );
    model.setEnabled( false );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    // While disabled, layersAdded signal is disconnected, addition is not reflected
    project.addMapLayer( makeLayer( QStringLiteral( "Post" ) ) );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    model.setEnabled( true );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
  }

  SECTION( "disabled + switching project: enabling repopulates from latest project" )
  {
    QgsProject projectA;
    projectA.addMapLayer( makeLayer( QStringLiteral( "A" ) ) );
    QgsProject projectB;
    projectB.addMapLayer( makeLayer( QStringLiteral( "B" ) ) );
    MapLayerBaseModel model;
    model.setProject( &projectA );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    model.setEnabled( false );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    // setProject while disabled  -> stores the pointer, disconnects old, but does NOT connect new
    // resetModel() is still called but mEnabled=false so mLayers stays empty
    model.setProject( &projectB );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    // setEnabled(true)-> reconnects signals on projectB, then resetModel() populates from projectB
    model.setEnabled( true );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "B" ) );
  }

  SECTION( "setEnabled and setProject emit signals only on actual change" )
  {
    MapLayerBaseModel model;
    QSignalSpy enabledSpy( &model, &MapLayerBaseModel::enabledChanged );
    QSignalSpy projectSpy( &model, &MapLayerBaseModel::projectChanged );
    model.setEnabled( false );
    model.setEnabled( false ); // duplicate guard: if ( mEnabled == enabled ) return
    REQUIRE( enabledSpy.count() == 1 );
    QgsProject project;
    model.setProject( &project );
    model.setProject( &project ); // duplicate guard: if ( mProject == project ) return
    REQUIRE( projectSpy.count() == 1 );
  }

  SECTION( "switching projects resets content correctly" )
  {
    QgsProject projectA;
    projectA.addMapLayer( makeLayer( QStringLiteral( "A" ) ) );
    QgsProject projectB;
    projectB.addMapLayers( { makeLayer( QStringLiteral( "B1" ) ), makeLayer( QStringLiteral( "B2" ) ) } );
    MapLayerBaseModel model;
    model.setProject( &projectA );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    model.setProject( &projectB );
    REQUIRE( model.rowCount( QModelIndex() ) == 2 );
    model.setProject( nullptr );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }
}


TEST_CASE( "MapLayerModel" )
{
  SECTION( "default state" )
  {
    MapLayerModel model;
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
    REQUIRE( model.enabled() );
    REQUIRE( model.project() == nullptr );
    REQUIRE( model.filters() == Qgis::LayerFilter::All );
    REQUIRE_FALSE( model.requiresTrackingAvailability() );
  }

  SECTION( "results are sorted alphabetically" )
  {
    QgsProject project;
    project.addMapLayers( { makeLayer( QStringLiteral( "Zebra" ) ), makeLayer( QStringLiteral( "Apple" ) ), makeLayer( QStringLiteral( "Mango" ) ) } );
    MapLayerModel model;
    model.setProject( &project );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Apple" ) );
    REQUIRE( model.index( 1, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Mango" ) );
    REQUIRE( model.index( 2, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Zebra" ) );
  }

  SECTION( "geometry-type filters" )
  {
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "Point" ), Qgis::WkbType::Point ) );
    project.addMapLayer( makeLayer( QStringLiteral( "Line" ), Qgis::WkbType::LineString ) );
    project.addMapLayer( makeLayer( QStringLiteral( "Polygon" ), Qgis::WkbType::Polygon ) );
    project.addMapLayer( makeLayer( QStringLiteral( "None" ), Qgis::WkbType::NoGeometry ) );

    MapLayerModel model;
    model.setProject( &project );
    model.setFilters( Qgis::LayerFilter::PointLayer );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Point" ) );
    model.setFilters( Qgis::LayerFilter::LineLayer );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Line" ) );
    model.setFilters( Qgis::LayerFilter::PolygonLayer );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "Polygon" ) );
    model.setFilters( Qgis::LayerFilter::NoGeometry );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "None" ) );
    // HasGeometry is checked inside the detectGeometry block, which requires at least one of
    // PointLayer|LineLayer|PolygonLayer|NoGeometry to be set. When used in combination it works
    model.setFilters( Qgis::LayerFilter::HasGeometry | Qgis::LayerFilter::PointLayer );
    REQUIRE( model.rowCount( QModelIndex() ) == 3 );
    model.setFilters( Qgis::LayerFilter::All );
    REQUIRE( model.rowCount( QModelIndex() ) == 4 );
  }

  SECTION( "setFilters emits filtersChanged only on actual change" )
  {
    MapLayerModel model;
    QSignalSpy spy( &model, &MapLayerModel::filtersChanged );
    model.setFilters( Qgis::LayerFilter::PointLayer );
    model.setFilters( Qgis::LayerFilter::PointLayer ); // duplicate guard: if ( mFilters == filters ) return
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "findLayer returns correct row; -1 for nullptr and unknown layer" )
  {
    QgsProject project;
    QgsVectorLayer *alpha = makeLayer( QStringLiteral( "Alpha" ) );
    QgsVectorLayer *beta = makeLayer( QStringLiteral( "Beta" ) );
    project.addMapLayers( { alpha, beta } );
    MapLayerModel model;
    model.setProject( &project );
    REQUIRE( model.findLayer( alpha ) == 0 ); // "Alpha" sorts before "Beta"
    REQUIRE( model.findLayer( beta ) == 1 );
    REQUIRE( model.findLayer( nullptr ) == -1 );
    std::unique_ptr<QgsVectorLayer> orphan( makeLayer( QStringLiteral( "Orphan" ) ) );
    REQUIRE( model.findLayer( orphan.get() ) == -1 );
  }

  SECTION( "get() returns populated map for valid row; empty for invalid" )
  {
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "GetTest" ) ) );
    MapLayerModel model;
    model.setProject( &project );
    const QVariantMap data = model.get( 0 );
    REQUIRE( data.value( QStringLiteral( "Name" ) ).toString() == QStringLiteral( "GetTest" ) );
    REQUIRE( !data.value( QStringLiteral( "Id" ) ).toString().isEmpty() );
    REQUIRE( model.get( -1 ).isEmpty() );
    REQUIRE( model.get( 99 ).isEmpty() );
  }

  SECTION( "disabled model hides all layers" )
  {
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "Hidden" ) ) );
    MapLayerModel model;
    model.setProject( &project );
    model.setEnabled( false );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }

  SECTION( "requiresTrackingAvailability with no TrackingModel filters all layers" )
  {
    // !mModel->trackingModel() is true -> layerMatchesFilters returns false for every layer
    QgsProject project;
    project.addMapLayer( makeLayer( QStringLiteral( "Point" ) ) );
    MapLayerModel model;
    model.setProject( &project );
    model.setRequiresTrackingAvailability( true );
    REQUIRE( model.rowCount( QModelIndex() ) == 0 );
  }

  SECTION( "requiresTrackingAvailability allows layer when trackingModel exists and layer is not actively tracked" )
  {
    QgsProject project;
    QgsVectorLayer *layer = makeLayer( QStringLiteral( "TrackCandidate" ), Qgis::WkbType::LineString );
    project.addMapLayer( layer );
    TrackingModel trackingModel;
    REQUIRE_FALSE( trackingModel.layerInActiveTracking( layer ) );
    MapLayerModel model;
    model.setProject( &project );
    model.setTrackingModel( &trackingModel );
    model.setRequiresTrackingAvailability( true );
    REQUIRE( model.rowCount( QModelIndex() ) == 1 );
    REQUIRE( model.index( 0, 0 ).data( MapLayerModel::NameRole ).toString() == QStringLiteral( "TrackCandidate" ) );
  }

  SECTION( "requiresTrackingAvailability emits signal only on actual change" )
  {
    MapLayerModel model;
    QSignalSpy spy( &model, &MapLayerModel::requiresTrackingAvailabilityChanged );
    model.setRequiresTrackingAvailability( true );
    model.setRequiresTrackingAvailability( true );
    REQUIRE( spy.count() == 1 );
  }
}
