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

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "qfmultifeaturelistmodel.h"

#include <QColor>
#include <QFont>
#include <qgsconditionalstyle.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeature.h>
#include <qgsfeatureiterator.h>
#include <qgsfeaturerequest.h>
#include <qgsgeometry.h>
#include <qgspointxy.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <memory>

static std::unique_ptr<QgsVectorLayer> createPointLayer( const QString &name )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=name:string&field=grp:string" ), name, QStringLiteral( "memory" ) );
  layer->setDisplayExpression( QStringLiteral( "\"name\"" ) );
  return layer;
}

static void addPointFeature( QgsVectorLayer *layer, const QString &name, const QString &grp, const QgsPointXY &point )
{
  QgsFeature feature( layer->fields() );
  feature.setAttributes( QgsAttributes() << name << grp );
  feature.setGeometry( QgsGeometry::fromPointXY( point ) );

  REQUIRE( layer->startEditing() );
  REQUIRE( layer->addFeature( feature ) );
  REQUIRE( layer->commitChanges() );
}

static QgsFeature committedFeature( QgsVectorLayer *layer, const QString &name )
{
  QgsFeatureRequest request;
  request.setFilterExpression( QStringLiteral( "\"name\" = '%1'" ).arg( name ) );

  QgsFeature feature;
  REQUIRE( layer->getFeatures( request ).nextFeature( feature ) );
  return feature;
}

static int rowForDisplayValue( const QAbstractItemModel &model, const QString &value )
{
  for ( int row = 0; row < model.rowCount(); ++row )
  {
    if ( model.data( model.index( row, 0 ), Qt::DisplayRole ).toString() == value )
    {
      return row;
    }
  }
  return -1;
}

static QStringList displayValues( const QAbstractItemModel &model )
{
  QStringList values;
  for ( int row = 0; row < model.rowCount(); ++row )
  {
    values << model.data( model.index( row, 0 ), Qt::DisplayRole ).toString();
  }
  return values;
}

static QStringList featureNames( QgsVectorLayer *layer )
{
  QStringList names;
  QgsFeatureIterator features = layer->getFeatures();
  QgsFeature feature;
  while ( features.nextFeature( feature ) )
  {
    names << feature.attribute( QStringLiteral( "name" ) ).toString();
  }
  names.sort();
  return names;
}


TEST_CASE( "MultiFeatureListModel empty state" )
{
  QfMultiFeatureListModel model;

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

TEST_CASE( "MultiFeatureListModel population" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  std::unique_ptr<QgsVectorLayer> buildingsLayer = createPointLayer( QStringLiteral( "buildings" ) );
  REQUIRE( roadsLayer->isValid() );
  REQUIRE( buildingsLayer->isValid() );

  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( buildingsLayer.get(), QStringLiteral( "Building X" ), QStringLiteral( "B" ), QgsPointXY( 2, 2 ) );

  QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
  requests.insert( roadsLayer.get(), QgsFeatureRequest() );
  requests.insert( buildingsLayer.get(), QgsFeatureRequest() );

  QfMultiFeatureListModel model;
  model.setFeatures( requests );

  REQUIRE( model.rowCount() == 3 );
  REQUIRE( model.count() == 3 );

  SECTION( "Roles" )
  {
    const int buildingRow = rowForDisplayValue( model, QStringLiteral( "Building X" ) );
    REQUIRE( buildingRow >= 0 );

    const QgsFeature expectedFeature = committedFeature( buildingsLayer.get(), QStringLiteral( "Building X" ) );
    const QModelIndex index = model.index( buildingRow, 0 );

    REQUIRE( model.data( index, Qt::DisplayRole ).toString() == QStringLiteral( "Building X" ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::FeatureNameRole ).toString() == QStringLiteral( "Building X" ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::FeatureIdRole ).value<QgsFeatureId>() == expectedFeature.id() );
    REQUIRE( model.data( index, QfMultiFeatureListModel::FeatureSelectedRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::LayerNameRole ).toString() == QStringLiteral( "buildings" ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>() == buildingsLayer.get() );
    REQUIRE( model.data( index, QfMultiFeatureListModel::CrsRole ).value<QgsCoordinateReferenceSystem>().authid() == QStringLiteral( "EPSG:4326" ) );

    const QgsFeature roleFeature = model.data( index, QfMultiFeatureListModel::FeatureRole ).value<QgsFeature>();
    REQUIRE( roleFeature.attributes() == ( QgsAttributes() << QStringLiteral( "Building X" ) << QStringLiteral( "B" ) ) );

    const QgsGeometry roleGeometry = model.data( index, QfMultiFeatureListModel::GeometryRole ).value<QgsGeometry>();
    REQUIRE( roleGeometry.asPoint().x() == Catch::Approx( 2.0 ) );
    REQUIRE( roleGeometry.asPoint().y() == Catch::Approx( 2.0 ) );
  }

  SECTION( "InvalidIndexReturnsInvalidData" )
  {
    REQUIRE_FALSE( model.index( 3, 0 ).isValid() );
    REQUIRE_FALSE( model.index( 0, 1 ).isValid() );
  }
}

TEST_CASE( "MultiFeatureListModel selection" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  std::unique_ptr<QgsVectorLayer> buildingsLayer = createPointLayer( QStringLiteral( "buildings" ) );

  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );
  addPointFeature( buildingsLayer.get(), QStringLiteral( "Building X" ), QStringLiteral( "B" ), QgsPointXY( 2, 2 ) );

  QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
  requests.insert( roadsLayer.get(), QgsFeatureRequest() );
  requests.insert( buildingsLayer.get(), QgsFeatureRequest() );

  QfMultiFeatureListModel model;
  model.setFeatures( requests );
  REQUIRE( model.rowCount() == 3 );

  const int buildingRow = rowForDisplayValue( model, QStringLiteral( "Building X" ) );
  REQUIRE( buildingRow >= 0 );

  SECTION( "SelectingFiltersTheProxyToThatLayer" )
  {
    model.toggleSelectedItem( buildingRow );

    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == buildingsLayer.get() );

    // the proxy is filtered to the selected layer while count() stays the source count
    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.count() == 3 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Building X" ) } ) );

    REQUIRE( model.data( model.index( 0, 0 ), QfMultiFeatureListModel::FeatureSelectedRole ).toBool() == true );

    const QList<QgsFeature> selected = model.selectedFeatures();
    REQUIRE( selected.size() == 1 );
    REQUIRE( selected.first().attributes() == ( QgsAttributes() << QStringLiteral( "Building X" ) << QStringLiteral( "B" ) ) );
  }

  SECTION( "TogglingTheSameItemDeselectsIt" )
  {
    model.toggleSelectedItem( buildingRow );
    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.rowCount() == 1 );

    model.toggleSelectedItem( 0 );

    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );
    REQUIRE( model.rowCount() == 3 );
  }

  SECTION( "ClearSelectionRestoresTheFullView" )
  {
    model.toggleSelectedItem( buildingRow );
    REQUIRE( model.selectedCount() == 1 );

    model.clearSelection();

    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );
    REQUIRE( model.rowCount() == 3 );
    REQUIRE( model.count() == 3 );
  }

  SECTION( "ClearKeepingSelectionReducesTheSourceToTheSelection" )
  {
    const int roadRow = rowForDisplayValue( model, QStringLiteral( "Road A" ) );
    REQUIRE( roadRow >= 0 );
    model.toggleSelectedItem( roadRow );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );

    model.clear( true );

    REQUIRE( model.count() == 1 );
    REQUIRE( model.rowCount() == 1 );
    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Road A" ) } ) );
  }

  SECTION( "ClearEmptiesTheModel" )
  {
    model.toggleSelectedItem( buildingRow );

    model.clear( false );

    REQUIRE( model.count() == 0 );
    REQUIRE( model.rowCount() == 0 );
    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );
  }
}

TEST_CASE( "MultiFeatureListModel capabilities reflect layer locks" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );

  QfMultiFeatureListModel model;
  model.setFeatures( roadsLayer.get(), QString() );
  REQUIRE( model.rowCount() == 1 );

  model.toggleSelectedItem( 0 );
  REQUIRE( model.selectedLayer() == roadsLayer.get() );

  const QModelIndex index = model.index( 0, 0 );
  REQUIRE( model.data( index, Qt::DisplayRole ).toString() == QStringLiteral( "Road A" ) );

  SECTION( "MemoryLayerIsFullyCapableByDefault" )
  {
    REQUIRE( model.canEditAttributesSelection() == true );
    REQUIRE( model.canDeleteSelection() == true );
    REQUIRE( model.canDuplicateSelection() == true );
    REQUIRE( model.canMoveSelection() == true );
    REQUIRE( model.canRotateSelection() == true );
    REQUIRE( model.data( index, QfMultiFeatureListModel::DeleteFeatureRole ).toBool() == true );
    REQUIRE( model.data( index, QfMultiFeatureListModel::EditGeometryRole ).toBool() == true );
  }

  SECTION( "DeletionLock" )
  {
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), true );

    REQUIRE( model.canDeleteSelection() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::DeleteFeatureRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::EditGeometryRole ).toBool() == true );
  }

  SECTION( "GeometryEditingLock" )
  {
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), true );

    REQUIRE( model.data( index, QfMultiFeatureListModel::EditGeometryRole ).toBool() == false );
    REQUIRE( model.canMoveSelection() == false );
    REQUIRE( model.canRotateSelection() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::DeleteFeatureRole ).toBool() == true );
  }

  SECTION( "GeometryLockAffectsBothDeletionAndGeometryEditing" )
  {
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), true );

    REQUIRE( model.data( index, QfMultiFeatureListModel::DeleteFeatureRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::EditGeometryRole ).toBool() == false );
  }

  SECTION( "AttributeEditingLock" )
  {
    roadsLayer->setCustomProperty( QStringLiteral( "QFieldSync/is_attribute_editing_locked" ), true );

    REQUIRE( model.canEditAttributesSelection() == false );
  }

  SECTION( "ReadOnlyLayer" )
  {
    roadsLayer->setReadOnly( true );

    REQUIRE( model.canEditAttributesSelection() == false );
    REQUIRE( model.canDeleteSelection() == false );
    REQUIRE( model.canDuplicateSelection() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::DeleteFeatureRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::EditGeometryRole ).toBool() == false );
  }
}

TEST_CASE( "MultiFeatureListModel setFeatures filter and extent" )
{
  std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "test" ) );
  addPointFeature( layer.get(), QStringLiteral( "A1" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( layer.get(), QStringLiteral( "A2" ), QStringLiteral( "A" ), QgsPointXY( 10, 10 ) );
  addPointFeature( layer.get(), QStringLiteral( "B1" ), QStringLiteral( "B" ), QgsPointXY( 20, 20 ) );

  const QgsCoordinateReferenceSystem previousProjectCrs = QgsProject::instance()->crs();
  QgsProject::instance()->setCrs( layer->crs() );

  QfMultiFeatureListModel model;

  SECTION( "NoFilterReturnsEverything" )
  {
    model.setFeatures( layer.get(), QString() );

    REQUIRE( model.rowCount() == 3 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "A1" ), QStringLiteral( "A2" ), QStringLiteral( "B1" ) } ) );
  }

  SECTION( "FilterExpression" )
  {
    model.setFeatures( layer.get(), QStringLiteral( "\"grp\" = 'A'" ) );

    REQUIRE( model.rowCount() == 2 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "A1" ), QStringLiteral( "A2" ) } ) );
  }

  SECTION( "FilterExpressionAndExtent" )
  {
    model.setFeatures( layer.get(), QStringLiteral( "\"grp\" = 'A'" ), QgsRectangle( -1, -1, 5, 5 ) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "A1" ) } ) );
  }

  SECTION( "ExtentIsTransformedFromTheProjectCrs" )
  {
    QgsProject::instance()->setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );

    // roughly the 0,0 to 5,5 degree box expressed in web mercator metres
    model.setFeatures( layer.get(), QString(), QgsRectangle( -100000, -100000, 550000, 550000 ) );

    REQUIRE( model.rowCount() == 1 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "A1" ) } ) );
  }

  QgsProject::instance()->setCrs( previousProjectCrs );
}

TEST_CASE( "MultiFeatureListModel deleteSelection" )
{
  std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( layer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( layer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );
  REQUIRE( layer->featureCount() == 2 );

  QfMultiFeatureListModel model;
  model.setFeatures( layer.get(), QString() );
  REQUIRE( model.rowCount() == 2 );

  SECTION( "DeletesTheSelectedFeatureAndCommits" )
  {
    const int roadARow = rowForDisplayValue( model, QStringLiteral( "Road A" ) );
    REQUIRE( roadARow >= 0 );
    model.toggleSelectedItem( roadARow );

    REQUIRE( model.deleteSelection() );

    REQUIRE( layer->featureCount() == 1 );
    REQUIRE_FALSE( layer->isEditable() );
    REQUIRE( featureNames( layer.get() ) == QStringList( { QStringLiteral( "Road B" ) } ) );
  }

  SECTION( "FailsWithoutASelection" )
  {
    REQUIRE_FALSE( model.deleteSelection() );
    REQUIRE( layer->featureCount() == 2 );
  }

  SECTION( "FailsWhenDeletionIsLocked" )
  {
    model.toggleSelectedItem( 0 );
    layer->setCustomProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), true );

    REQUIRE_FALSE( model.deleteSelection() );
    REQUIRE( layer->featureCount() == 2 );
  }
}

TEST_CASE( "MultiFeatureListModel duplicateSelection" )
{
  std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( layer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 3, 4 ) );
  REQUIRE( layer->featureCount() == 1 );

  QfMultiFeatureListModel model;
  model.setFeatures( layer.get(), QString() );
  REQUIRE( model.rowCount() == 1 );

  SECTION( "DuplicatesTheSelectedFeature" )
  {
    model.toggleSelectedItem( 0 );

    REQUIRE( model.duplicateSelection() );

    REQUIRE( layer->featureCount() == 2 );
    REQUIRE( featureNames( layer.get() ) == QStringList( { QStringLiteral( "Road A" ), QStringLiteral( "Road A" ) } ) );

    REQUIRE( model.count() == 1 );
    REQUIRE( model.selectedCount() == 1 );

    const QList<QgsFeature> selected = model.selectedFeatures();
    REQUIRE( selected.size() == 1 );
    REQUIRE( selected.first().attributes() == ( QgsAttributes() << QStringLiteral( "Road A" ) << QStringLiteral( "A" ) ) );
    REQUIRE( selected.first().geometry().asPoint().x() == Catch::Approx( 3.0 ) );
    REQUIRE( selected.first().geometry().asPoint().y() == Catch::Approx( 4.0 ) );
  }

  SECTION( "FailsWithoutASelection" )
  {
    REQUIRE_FALSE( model.duplicateSelection() );
    REQUIRE( layer->featureCount() == 1 );
  }

  SECTION( "FailsWhenFeatureAdditionIsLocked" )
  {
    model.toggleSelectedItem( 0 );
    layer->setCustomProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), true );

    REQUIRE_FALSE( model.duplicateSelection() );
    REQUIRE( layer->featureCount() == 1 );
  }
}

TEST_CASE( "MultiFeatureListModel moveSelection" )
{
  std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( layer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 1, 2 ) );

  QfMultiFeatureListModel model;
  model.setFeatures( layer.get(), QString() );
  REQUIRE( model.rowCount() == 1 );

  SECTION( "TranslatesByTheGivenOffset" )
  {
    model.toggleSelectedItem( 0 );

    REQUIRE( model.moveSelection( 10.0, 20.0, QgsPoint() ) );

    const QgsFeature moved = committedFeature( layer.get(), QStringLiteral( "Road A" ) );
    REQUIRE( moved.geometry().asPoint().x() == Catch::Approx( 11.0 ) );
    REQUIRE( moved.geometry().asPoint().y() == Catch::Approx( 22.0 ) );
    REQUIRE_FALSE( layer->isEditable() );
  }

  SECTION( "SingleSelectionSnapsToTheDestinationPoint" )
  {
    model.toggleSelectedItem( 0 );

    REQUIRE( model.moveSelection( 10.0, 20.0, QgsPoint( 7.0, 8.0 ) ) );

    const QgsFeature moved = committedFeature( layer.get(), QStringLiteral( "Road A" ) );
    REQUIRE( moved.geometry().asPoint().x() == Catch::Approx( 7.0 ) );
    REQUIRE( moved.geometry().asPoint().y() == Catch::Approx( 8.0 ) );
  }

  SECTION( "FailsWithoutASelection" )
  {
    REQUIRE_FALSE( model.moveSelection( 10.0, 20.0, QgsPoint() ) );

    const QgsFeature unmoved = committedFeature( layer.get(), QStringLiteral( "Road A" ) );
    REQUIRE( unmoved.geometry().asPoint().x() == Catch::Approx( 1.0 ) );
    REQUIRE( unmoved.geometry().asPoint().y() == Catch::Approx( 2.0 ) );
  }
}

TEST_CASE( "MultiFeatureListModel rotateSelection" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "LineString?crs=EPSG:4326&field=name:string" ), QStringLiteral( "lines" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );
  layer->setDisplayExpression( QStringLiteral( "\"name\"" ) );

  QgsFeature line( layer->fields() );
  line.setAttributes( QgsAttributes() << QStringLiteral( "Line A" ) );
  line.setGeometry( QgsGeometry::fromWkt( QStringLiteral( "LineString (0 0, 0 10)" ) ) );
  REQUIRE( layer->startEditing() );
  REQUIRE( layer->addFeature( line ) );
  REQUIRE( layer->commitChanges() );

  QfMultiFeatureListModel model;
  model.setFeatures( layer.get(), QString() );
  REQUIRE( model.rowCount() == 1 );

  SECTION( "RotatesAroundTheGeometryCentroid" )
  {
    model.toggleSelectedItem( 0 );

    // 180 degrees is direction agnostic: the endpoints swap around the centroid
    REQUIRE( model.rotateSelection( 180.0 ) );

    QgsFeature rotated;
    REQUIRE( layer->getFeatures().nextFeature( rotated ) );
    REQUIRE( rotated.geometry().asWkt( 1 ) == QStringLiteral( "LineString (0 10, 0 0)" ) );
    REQUIRE_FALSE( layer->isEditable() );
  }

  SECTION( "FailsWithoutASelection" )
  {
    REQUIRE_FALSE( model.rotateSelection( 180.0 ) );
  }
}

TEST_CASE( "MultiFeatureListModel mergeSelection" )
{
  std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( layer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( layer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );

  QfMultiFeatureListModel model;
  model.setFeatures( layer.get(), QString() );
  REQUIRE( model.rowCount() == 2 );

  SECTION( "FailsOnASingleTypeLayerBecauseTheUnionIsMultipart" )
  {
    model.toggleSelectedItem( 0 );
    model.toggleSelectedItem( 1 );
    REQUIRE( model.selectedCount() == 2 );

    REQUIRE_FALSE( model.mergeSelection() );

    REQUIRE( layer->featureCount() == 2 );
    REQUIRE( featureNames( layer.get() ) == QStringList( { QStringLiteral( "Road A" ), QStringLiteral( "Road B" ) } ) );
  }

  SECTION( "FailsWithoutASelection" )
  {
    REQUIRE_FALSE( model.mergeSelection() );
    REQUIRE( layer->featureCount() == 2 );
  }
}

TEST_CASE( "MultiFeatureListModel reflects layer edits" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );

  QfMultiFeatureListModel model;
  model.setFeatures( roadsLayer.get(), QString() );

  // a single layer is ordered by its display expression, so the rows are deterministic
  REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Road A" ), QStringLiteral( "Road B" ) } ) );

  const QgsFeature roadA = committedFeature( roadsLayer.get(), QStringLiteral( "Road A" ) );
  const QgsFeature roadB = committedFeature( roadsLayer.get(), QStringLiteral( "Road B" ) );
  const int nameFieldIndex = roadsLayer->fields().indexFromName( QStringLiteral( "name" ) );
  REQUIRE( nameFieldIndex >= 0 );

  SECTION( "AttributeChangeUpdatesTheRow" )
  {
    REQUIRE( roadsLayer->startEditing() );
    REQUIRE( roadsLayer->changeAttributeValue( roadA.id(), nameFieldIndex, QStringLiteral( "Road Z" ) ) );
    REQUIRE( roadsLayer->commitChanges() );

    const QModelIndex index = model.index( 0, 0 );
    REQUIRE( model.data( index, Qt::DisplayRole ).toString() == QStringLiteral( "Road Z" ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::FeatureNameRole ).toString() == QStringLiteral( "Road Z" ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::FeatureRole ).value<QgsFeature>().attributes() == ( QgsAttributes() << QStringLiteral( "Road Z" ) << QStringLiteral( "A" ) ) );

    // the untouched row is unaffected
    REQUIRE( model.data( model.index( 1, 0 ), Qt::DisplayRole ).toString() == QStringLiteral( "Road B" ) );
  }

  SECTION( "AttributeChangeUpdatesTheSelectedCopy" )
  {
    model.toggleSelectedItem( 0 );

    REQUIRE( roadsLayer->startEditing() );
    REQUIRE( roadsLayer->changeAttributeValue( roadA.id(), nameFieldIndex, QStringLiteral( "Road Z" ) ) );
    REQUIRE( roadsLayer->commitChanges() );

    const QList<QgsFeature> selected = model.selectedFeatures();
    REQUIRE( selected.size() == 1 );
    REQUIRE( selected.first().attributes() == ( QgsAttributes() << QStringLiteral( "Road Z" ) << QStringLiteral( "A" ) ) );
  }

  SECTION( "GeometryChangeUpdatesTheRow" )
  {
    QgsGeometry movedGeometry = QgsGeometry::fromPointXY( QgsPointXY( 9, 8 ) );
    REQUIRE( roadsLayer->startEditing() );
    REQUIRE( roadsLayer->changeGeometry( roadA.id(), movedGeometry ) );
    REQUIRE( roadsLayer->commitChanges() );

    const QgsGeometry geometry = model.data( model.index( 0, 0 ), QfMultiFeatureListModel::GeometryRole ).value<QgsGeometry>();
    REQUIRE( geometry.asPoint().x() == Catch::Approx( 9.0 ) );
    REQUIRE( geometry.asPoint().y() == Catch::Approx( 8.0 ) );
  }

  SECTION( "FeatureDeletionRemovesTheRowAndClearsTheWholeSelection" )
  {
    model.toggleSelectedItem( 0 );
    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( model.selectedLayer() == roadsLayer.get() );

    REQUIRE( roadsLayer->startEditing() );
    REQUIRE( roadsLayer->deleteFeature( roadB.id() ) );
    REQUIRE( roadsLayer->commitChanges() );

    REQUIRE( model.count() == 1 );
    REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Road A" ) } ) );

    // deleting any feature drops the entire selection, including features that still exist
    REQUIRE( model.selectedCount() == 0 );
    REQUIRE( model.selectedLayer() == nullptr );
  }
}

TEST_CASE( "MultiFeatureListModel drops the rows of a destroyed layer" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  std::unique_ptr<QgsVectorLayer> buildingsLayer = createPointLayer( QStringLiteral( "buildings" ) );

  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );
  addPointFeature( buildingsLayer.get(), QStringLiteral( "Building X" ), QStringLiteral( "B" ), QgsPointXY( 2, 2 ) );

  QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
  requests.insert( roadsLayer.get(), QgsFeatureRequest() );
  requests.insert( buildingsLayer.get(), QgsFeatureRequest() );

  QfMultiFeatureListModel model;
  model.setFeatures( requests );
  REQUIRE( model.count() == 3 );

  roadsLayer.reset();

  REQUIRE( model.count() == 1 );
  REQUIRE( model.rowCount() == 1 );
  REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Building X" ) } ) );
  REQUIRE( model.selectedCount() == 0 );
  REQUIRE( model.data( model.index( 0, 0 ), QfMultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>() == buildingsLayer.get() );
}

TEST_CASE( "MultiFeatureListModel ExtrusionRole" )
{
  SECTION( "ReturnsTheGuessedHeightFieldValue" )
  {
    std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=name:string&field=height:double" ), QStringLiteral( "buildings" ), QStringLiteral( "memory" ) );
    REQUIRE( layer->isValid() );
    layer->setDisplayExpression( QStringLiteral( "\"name\"" ) );

    QgsFeature feature( layer->fields() );
    feature.setAttributes( QgsAttributes() << QStringLiteral( "Tower" ) << 12.5 );
    feature.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 0, 0 ) ) );
    REQUIRE( layer->startEditing() );
    REQUIRE( layer->addFeature( feature ) );
    REQUIRE( layer->commitChanges() );

    QfMultiFeatureListModel model;
    model.setFeatures( layer.get(), QString() );
    REQUIRE( model.rowCount() == 1 );

    REQUIRE( model.data( model.index( 0, 0 ), QfMultiFeatureListModel::ExtrusionRole ).toDouble() == Catch::Approx( 12.5 ) );
  }

  SECTION( "ReturnsZeroWithoutAHeightField" )
  {
    std::unique_ptr<QgsVectorLayer> layer = createPointLayer( QStringLiteral( "roads" ) );
    addPointFeature( layer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );

    QfMultiFeatureListModel model;
    model.setFeatures( layer.get(), QString() );
    REQUIRE( model.rowCount() == 1 );

    REQUIRE( model.data( model.index( 0, 0 ), QfMultiFeatureListModel::ExtrusionRole ).toDouble() == Catch::Approx( 0.0 ) );
  }
}

TEST_CASE( "MultiFeatureListModel conditional styling roles" )
{
  std::unique_ptr<QgsVectorLayer> roadsLayer = createPointLayer( QStringLiteral( "roads" ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road A" ), QStringLiteral( "A" ), QgsPointXY( 0, 0 ) );
  addPointFeature( roadsLayer.get(), QStringLiteral( "Road B" ), QStringLiteral( "A" ), QgsPointXY( 1, 1 ) );

  QgsConditionalStyle style;
  style.setRule( QStringLiteral( "\"name\" = 'Road A'" ) );
  style.setBackgroundColor( QColor( 255, 0, 0 ) );
  style.setTextColor( QColor( 0, 0, 255 ) );
  QFont font;
  font.setBold( true );
  font.setItalic( true );
  style.setFont( font );

  roadsLayer->conditionalStyles()->setRowStyles( QList<QgsConditionalStyle>() << style );

  QfMultiFeatureListModel model;
  model.setFeatures( roadsLayer.get(), QString() );
  REQUIRE( displayValues( model ) == QStringList( { QStringLiteral( "Road A" ), QStringLiteral( "Road B" ) } ) );

  SECTION( "MatchingRowCarriesTheStyle" )
  {
    const QModelIndex index = model.index( 0, 0 );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalBackgroundColorRole ).value<QColor>() == QColor( 255, 0, 0 ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalTextColorRole ).value<QColor>() == QColor( 0, 0, 255 ) );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontBoldRole ).toBool() == true );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontItalicRole ).toBool() == true );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontUnderlineRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontStrikeOutRole ).toBool() == false );
  }

  SECTION( "NonMatchingRowCarriesNoStyle" )
  {
    const QModelIndex index = model.index( 1, 0 );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalBackgroundColorRole ).isNull() );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalTextColorRole ).isNull() );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontBoldRole ).toBool() == false );
    REQUIRE( model.data( index, QfMultiFeatureListModel::ConditionalFontItalicRole ).toBool() == false );
  }
}
