/***************************************************************************
                        test_pendingdeltamodel
                        ----------------------
  begin                : August 2026
  copyright            : (C) 2026 by Mohsen Dehghanzadeh
  email                : mohsen@opengis.ch
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
#include "qfdeltafilewrapper.h"
#include "qfpendingdeltamodel.h"

#include <QTemporaryDir>
#include <QUuid>
#include <qgsgeometry.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

static QgsFeature makeFeature( const QgsVectorLayer *layer, int fid, const QString &name, int height = 0 )
{
  QgsFeature feature( layer->fields(), fid );
  feature.setAttribute( QStringLiteral( "fid" ), fid );
  feature.setAttribute( QStringLiteral( "name" ), name );
  feature.setAttribute( QStringLiteral( "height" ), height );
  return feature;
}


TEST_CASE( "PendingDeltaModel" )
{
  QgsProject *project = QgsProject::instance();
  QTemporaryDir workDir;

  REQUIRE( workDir.isValid() );

  std::unique_ptr<QgsVectorLayer> layer1 = std::make_unique<QgsVectorLayer>( QStringLiteral( "Polygon?crs=EPSG:3857&field=fid:integer&field=name:string&field=height:integer" ), QStringLiteral( "layer1" ), QStringLiteral( "memory" ) );
  std::unique_ptr<QgsVectorLayer> layer2 = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=name:string&field=height:integer" ), QStringLiteral( "layer2" ), QStringLiteral( "memory" ) );

  REQUIRE( layer1->isValid() );
  REQUIRE( layer2->isValid() );

  layer1->setDisplayExpression( QStringLiteral( "\"name\"" ) );
  layer2->setDisplayExpression( QStringLiteral( "\"name\"" ) );

  REQUIRE( project->addMapLayer( layer1.get(), false, false ) );
  REQUIRE( project->addMapLayer( layer2.get(), false, false ) );

  QgsFeature f1 = makeFeature( layer2.get(), 1, QStringLiteral( "name1" ), 10 );
  const QgsFeature f2 = makeFeature( layer1.get(), 1, QStringLiteral( "name2" ) );
  const QgsFeature f3 = makeFeature( layer2.get(), 3, QStringLiteral( "name3" ), 7 );

  // only f1 is stored in the layer, the others are known through their deltas alone
  REQUIRE( layer2->startEditing() );
  REQUIRE( layer2->addFeature( f1 ) );
  REQUIRE( layer2->commitChanges() );

  QfDeltaFileWrapper dfw( QStringLiteral( "TEST_PROJECT_ID" ), workDir.filePath( QUuid::createUuid().toString() ) );
  QfPendingDeltaModel model;

  SECTION( "starts empty without a delta file wrapper" )
  {
    REQUIRE( model.deltaFileWrapper() == nullptr );
    REQUIRE( model.rowCount() == 0 );
  }

  SECTION( "lists one row per delta, grouped by layer" )
  {
    QgsFeature newFeature = f1;
    newFeature.setAttribute( QStringLiteral( "height" ), 12 );

    // added out of layer order on purpose
    dfw.addPatch( project, layer2->id(), layer2->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1, newFeature );
    dfw.addCreate( project, layer1->id(), layer1->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );
    dfw.addDelete( project, layer2->id(), layer2->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f3 );

    model.setDeltaFileWrapper( &dfw );

    REQUIRE( model.rowCount() == 3 );

    const QModelIndex idx0 = model.index( 0, 0 );
    const QModelIndex idx1 = model.index( 1, 0 );
    const QModelIndex idx2 = model.index( 2, 0 );

    REQUIRE( model.data( idx0, QfPendingDeltaModel::LayerIdRole ).toString() == layer1->id() );
    REQUIRE( model.data( idx1, QfPendingDeltaModel::LayerIdRole ).toString() == layer2->id() );
    REQUIRE( model.data( idx2, QfPendingDeltaModel::LayerIdRole ).toString() == layer2->id() );

    REQUIRE( model.data( idx0, QfPendingDeltaModel::MethodRole ).value<QfPendingDeltaModel::DeltaMethod>() == QfPendingDeltaModel::CreateMethod );
    REQUIRE( model.data( idx1, QfPendingDeltaModel::MethodRole ).value<QfPendingDeltaModel::DeltaMethod>() == QfPendingDeltaModel::PatchMethod );
    REQUIRE( model.data( idx2, QfPendingDeltaModel::MethodRole ).value<QfPendingDeltaModel::DeltaMethod>() == QfPendingDeltaModel::DeleteMethod );


    REQUIRE( model.layerName( layer1->id() ) == QStringLiteral( "layer1" ) );
    REQUIRE( model.layerGeometryType( layer1->id() ) == Qgis::GeometryType::Polygon );
    REQUIRE( model.layerChangesCount( layer1->id() ) == 1 );

    REQUIRE( model.layerName( layer2->id() ) == QStringLiteral( "layer2" ) );
    REQUIRE( model.layerGeometryType( layer2->id() ) == Qgis::GeometryType::Point );
    REQUIRE( model.layerChangesCount( layer2->id() ) == 2 );

    // f1 is still in the layer, f3 is only described by its delta
    REQUIRE( model.data( idx1, QfPendingDeltaModel::FeatureNameRole ).toString() == QStringLiteral( "name1" ) );
    REQUIRE( model.data( idx2, QfPendingDeltaModel::FeatureNameRole ).toString() == QStringLiteral( "name3" ) );
  }

  SECTION( "patch rows list the changed attributes only" )
  {
    QgsFeature newFeature = f1;
    newFeature.setAttribute( QStringLiteral( "height" ), 12 );

    dfw.addPatch( project, layer2->id(), layer2->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1, newFeature );
    model.setDeltaFileWrapper( &dfw );

    REQUIRE( model.rowCount() == 1 );

    // the delta keeps a full snapshot of the old attributes, only "height" actually changed
    const QList<QfDeltaAttributeChange> changes = model.data( model.index( 0, 0 ), QfPendingDeltaModel::AttributeChangesRole ).value<QList<QfDeltaAttributeChange>>();

    REQUIRE( changes.size() == 1 );

    const QfDeltaAttributeChange change = changes.at( 0 );

    REQUIRE( change.name == QStringLiteral( "height" ) );
    REQUIRE( change.oldValue.toInt() == 10 );
    REQUIRE( change.newValue.toInt() == 12 );
  }

  SECTION( "delete rows list the removed values" )
  {
    dfw.addDelete( project, layer2->id(), layer2->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f3 );
    model.setDeltaFileWrapper( &dfw );

    REQUIRE( model.rowCount() == 1 );

    QVariantMap changeByName;
    const QList<QfDeltaAttributeChange> changes = model.data( model.index( 0, 0 ), QfPendingDeltaModel::AttributeChangesRole ).value<QList<QfDeltaAttributeChange>>();
    for ( const QfDeltaAttributeChange &change : changes )
    {
      changeByName.insert( change.name, change.oldValue );
    }

    REQUIRE( changeByName.size() == 3 );
    REQUIRE( changeByName.value( QStringLiteral( "fid" ) ).toInt() == 3 );
    REQUIRE( changeByName.value( QStringLiteral( "name" ) ).toString() == QStringLiteral( "name3" ) );
    REQUIRE( changeByName.value( QStringLiteral( "height" ) ).toInt() == 7 );
  }

  SECTION( "only patches touching the geometry flag a geometry change" )
  {
    QgsFeature newFeature = f1;
    newFeature.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 1, 2 ) ) );

    dfw.addPatch( project, layer2->id(), layer2->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1, newFeature );
    // a create also carries a geometry, but nothing was modified
    dfw.addCreate( project, layer1->id(), layer1->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );

    model.setDeltaFileWrapper( &dfw );

    REQUIRE( model.rowCount() == 2 );

    const QModelIndex idx0 = model.index( 0, 0 );
    const QModelIndex idx1 = model.index( 1, 0 );

    REQUIRE( !model.data( idx0, QfPendingDeltaModel::HasGeometryChangeRole ).toBool() );
    REQUIRE( model.data( idx1, QfPendingDeltaModel::HasGeometryChangeRole ).toBool() );
    REQUIRE( model.data( idx1, QfPendingDeltaModel::AttributeChangesRole ).value<QList<QfDeltaAttributeChange>>().isEmpty() );
  }

  SECTION( "rows follow the attached delta file wrapper" )
  {
    model.setDeltaFileWrapper( &dfw );

    REQUIRE( model.rowCount() == 0 );

    dfw.addCreate( project, layer1->id(), layer1->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );

    REQUIRE( model.rowCount() == 1 );

    model.setDeltaFileWrapper( nullptr );

    REQUIRE( model.rowCount() == 0 );

    // the wrapper keeps collecting, a detached model must not follow it
    dfw.addCreate( project, layer1->id(), layer1->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), makeFeature( layer1.get(), 2, QStringLiteral( "name4" ) ) );

    REQUIRE( model.rowCount() == 0 );
  }
}
