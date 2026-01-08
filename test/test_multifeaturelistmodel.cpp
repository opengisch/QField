/***************************************************************************
                        test_multifeaturelistmodel
                        --------------------
  begin                : January 2026
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
#include "multifeaturelistmodel.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <qgsfeature.h>
#include <qgsfeaturerequest.h>
#include <qgsgeometry.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <memory>


static int findRowByDisplayValue( const QAbstractItemModel &model, const QString &value )
{
  for ( int row = 0; row < model.rowCount(); ++row )
  {
    const QModelIndex idx = model.index( row, 0 );
    if ( model.data( idx, Qt::DisplayRole ).toString() == value )
    {
      return row;
    }
  }
  return -1;
}

static QgsFeatureId addPointFeature( QgsVectorLayer *layer, const QString &name, const QString &grp, const QgsPointXY &point )
{
  QgsFeature dummyFeature( layer->fields() );
  dummyFeature.setAttribute( QStringLiteral( "name" ), name );
  dummyFeature.setAttribute( QStringLiteral( "grp" ), grp );
  dummyFeature.setGeometry( QgsGeometry::fromPointXY( point ) );

  REQUIRE( layer->addFeature( dummyFeature ) );
  return dummyFeature.id();
}

static QgsFeature getFeatureById( QgsVectorLayer *layer, QgsFeatureId id )
{
  QgsFeature dummyFeature;
  QgsFeatureRequest req;
  req.setFilterFid( id );

  QgsFeatureIterator it = layer->getFeatures( req );
  REQUIRE( it.nextFeature( dummyFeature ) );
  REQUIRE( dummyFeature.isValid() );
  return dummyFeature;
}


TEST_CASE( "MultiFeatureListModel validation checks" )
{
  MultiFeatureListModel model;

  REQUIRE( model.rowCount() == 0 );
  REQUIRE( model.count() == 0 );
  REQUIRE( model.selectedCount() == 0 );
  REQUIRE( model.selectedFeatures().isEmpty() );
  REQUIRE( model.selectedLayer() == nullptr );

  REQUIRE_FALSE( model.canEditAttributesSelection() );
  REQUIRE_FALSE( model.canMergeSelection() );
  REQUIRE_FALSE( model.canDeleteSelection() );
  REQUIRE_FALSE( model.canDuplicateSelection() );
  REQUIRE_FALSE( model.canMoveSelection() );
  REQUIRE_FALSE( model.canRotateSelection() );
  REQUIRE_FALSE( model.canProcessSelection() );
}

TEST_CASE( "MultiFeatureListModel behaviours" )
{
  if ( !QCoreApplication::instance() )
  {
    static int applicationArgumentCount = 1;
    static char applicationArgumentZero[] = "qfield-tests";
    static char *applicationArgumentValues[] = { applicationArgumentZero, nullptr };
    static QCoreApplication coreApplication( applicationArgumentCount, applicationArgumentValues );
    Q_UNUSED( coreApplication );
  }

  static const int maximumWaitTimeMilliseconds = 2000;

  //Setup layers
  std::unique_ptr<QgsVectorLayer> roadsLayer(
    new QgsVectorLayer(
      "Point?crs=epsg:4326&field=name:string&field=grp:string",
      "roads",
      "memory" ) );

  std::unique_ptr<QgsVectorLayer> buildingsLayer(
    new QgsVectorLayer(
      "Point?crs=epsg:4326&field=name:string&field=grp:string",
      "buildings",
      "memory" ) );

  REQUIRE( roadsLayer->isValid() );
  REQUIRE( buildingsLayer->isValid() );

  // Stable display role for findRowByDisplayValue()
  roadsLayer->setDisplayExpression( QStringLiteral( "\"name\"" ) );
  buildingsLayer->setDisplayExpression( QStringLiteral( "\"name\"" ) );

  roadsLayer->startEditing();
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );
  roadsLayer->commitChanges();

  buildingsLayer->startEditing();
  addPointFeature( buildingsLayer.get(), QStringLiteral( "Building X" ), QStringLiteral( "B" ), QgsPointXY( 2, 2 ) );
  buildingsLayer->commitChanges();

  // Populate model with both layers
  MultiFeatureListModel model;

  QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
  requests.insert( roadsLayer.get(), QgsFeatureRequest() );
  requests.insert( buildingsLayer.get(), QgsFeatureRequest() );
  model.setFeatures( requests );

  // Wait for population (usually immediate)
  {
    QElapsedTimer timer;
    timer.start();
    while ( model.rowCount() != 3 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.count() == 3 );
  }

  // Roles sanity
  {
    const int buildingRow = findRowByDisplayValue( model, QStringLiteral( "Building X" ) );
    REQUIRE( buildingRow >= 0 );

    const QModelIndex idx = model.index( buildingRow, 0 );
    REQUIRE( model.data( idx, MultiFeatureListModel::FeatureSelectedRole ).toBool() == false );
    REQUIRE( model.data( idx, MultiFeatureListModel::LayerNameRole ).toString() == QStringLiteral( "buildings" ) );

    QgsVectorLayer *layerFromRole = model.data( idx, MultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
    REQUIRE( layerFromRole == buildingsLayer.get() );

    const QgsFeatureId fid = model.data( idx, MultiFeatureListModel::FeatureIdRole ).value<QgsFeatureId>();
    REQUIRE( fid >= 0 );
  }

  // Selection sets selectedLayer and filters by layer
  {
    const int buildingRow = findRowByDisplayValue( model, QStringLiteral( "Building X" ) );
    REQUIRE( buildingRow >= 0 );

    model.toggleSelectedItem( buildingRow );

    QElapsedTimer timer;
    timer.start();
    while ( ( model.selectedCount() != 1 || model.selectedLayer() != buildingsLayer.get() || model.rowCount() != 1 ) && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == buildingsLayer.get() );

    // Important behaviour here, proxy is filtered, but "count()" is source count
    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.count() == 3 );

    // Visible rows must all belong to the selected layer
    const QModelIndex visibleIdx = model.index( 0, 0 );
    REQUIRE( model.data( visibleIdx, MultiFeatureListModel::LayerNameRole ).toString() == QStringLiteral( "buildings" ) );
    REQUIRE( model.data( visibleIdx, Qt::DisplayRole ).toString() == QStringLiteral( "Building X" ) );

    // Clear selection restores full view
    model.clearSelection();

    timer.restart();
    while ( ( model.selectedCount() != 0 || model.selectedLayer() != nullptr || model.rowCount() != 3 ) && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.count() == 3 );
  }

  // clear(keepSelected) behaviour
  {
    const int roadARow = findRowByDisplayValue( model, QStringLiteral( "Road A" ) );
    REQUIRE( roadARow >= 0 );

    model.toggleSelectedItem( roadARow );

    QElapsedTimer timer;
    timer.start();
    while ( model.selectedCount() != 1 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );

    model.clear( true ); // keepSelected

    timer.restart();
    while ( ( model.count() != 1 || model.rowCount() != 1 || model.selectedCount() != 1 ) && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( model.count() == 1 );    // source reduced to selected only
    REQUIRE( model.rowCount() == 1 ); // proxy shows the same single feature
    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );

    model.clear( false );

    timer.restart();
    while ( ( model.count() != 0 || model.rowCount() != 0 || model.selectedCount() != 0 ) && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( model.count() == 0 );
    REQUIRE( model.rowCount() == 0 );
    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );

    // repopulate for next checks
    model.setFeatures( requests );

    timer.restart();
    while ( model.rowCount() != 3 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.rowCount() == 3 );
  }

  // DeleteFeatureRole or EditGeometryRole reflect layer locks
  {
    const int roadARow = findRowByDisplayValue( model, QStringLiteral( "Road A" ) );
    REQUIRE( roadARow >= 0 );

    model.toggleSelectedItem( roadARow );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );

    const QModelIndex idx = model.index( 0, 0 ); // filtered to roads layer, first visible should be Road A or Road B
    REQUIRE( model.data( idx, MultiFeatureListModel::LayerNameRole ).toString() == QStringLiteral( "roads" ) );

    // With memory provider, these are expected to be editable by default
    REQUIRE( model.canMoveSelection() == true );
    REQUIRE( model.canDeleteSelection() == true );

    REQUIRE( model.data( idx, MultiFeatureListModel::EditGeometryRole ).toBool() == true );
    REQUIRE( model.data( idx, MultiFeatureListModel::DeleteFeatureRole ).toBool() == true );

    // Lock deletion
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), true );
    REQUIRE( model.canDeleteSelection() == false );
    REQUIRE( model.data( idx, MultiFeatureListModel::DeleteFeatureRole ).toBool() == false );

    // Lock geometry editing
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), false );
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), true );
    REQUIRE( model.data( idx, MultiFeatureListModel::EditGeometryRole ).toBool() == false );

    model.clearSelection();
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), false );
  }
}

TEST_CASE( "MultiFeatureListModel setFeatures filter and extent" )
{
  if ( !QCoreApplication::instance() )
  {
    static int applicationArgumentCount = 1;
    static char applicationArgumentZero[] = "qfield-tests";
    static char *applicationArgumentValues[] = { applicationArgumentZero, nullptr };
    static QCoreApplication coreApplication( applicationArgumentCount, applicationArgumentValues );
    Q_UNUSED( coreApplication );
  }

  static const int maximumWaitTimeMilliseconds = 2000;

  std::unique_ptr<QgsVectorLayer> layer(
    new QgsVectorLayer(
      "Point?crs=epsg:4326&field=name:string&field=grp:string",
      "test",
      "memory" ) );

  REQUIRE( layer->isValid() );
  layer->setDisplayExpression( QStringLiteral( "\"name\"" ) );

  layer->startEditing();
  addPointFeature( layer.get(), QStringLiteral( "A1" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( layer.get(), QStringLiteral( "A2" ), QStringLiteral( "A" ), QgsPointXY( 10, 10 ) );
  addPointFeature( layer.get(), QStringLiteral( "B1" ), QStringLiteral( "B" ), QgsPointXY( 100, 100 ) );
  layer->commitChanges();

  // Avoid CRS transform branch in the implementation (keep project CRS same as layer CRS)
  QgsProject::instance()->setCrs( layer->crs() );

  MultiFeatureListModel model;

  model.setFeatures( layer.get(), QStringLiteral( "\"grp\" = 'A'" ) );
  {
    QElapsedTimer timer;
    timer.start();
    while ( model.rowCount() != 2 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.rowCount() == 2 );
    REQUIRE( findRowByDisplayValue( model, QStringLiteral( "B1" ) ) == -1 );
  }

  model.setFeatures( layer.get(), QStringLiteral( "\"grp\" = 'A'" ), QgsRectangle( -1, -1, 5, 5 ) );
  {
    QElapsedTimer timer;
    timer.start();
    while ( model.rowCount() != 1 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.rowCount() == 1 );

    const QModelIndex idx = model.index( 0, 0 );
    const QgsFeatureId fid = model.data( idx, MultiFeatureListModel::FeatureIdRole ).value<QgsFeatureId>();
    const QgsFeature f = getFeatureById( layer.get(), fid );
    REQUIRE( f.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "A1" ) );
  }
}
