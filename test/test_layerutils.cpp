/***************************************************************************
                        test_layerutils.h
                        --------------------
  begin                : Aug 2026
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
#include "utils/qflayerutils.h"

#include <qgscoordinatereferencesystem.h>
#include <qgsfeature.h>
#include <qgsfeatureiterator.h>
#include <qgsfeaturerequest.h>
#include <qgsgeometry.h>
#include <qgspointxy.h>
#include <qgsproject.h>
#include <qgsrelation.h>
#include <qgsrelationcontext.h>
#include <qgsrelationmanager.h>
#include <qgsvariantutils.h>
#include <qgsvectorlayer.h>

#include <memory>

static void addParentChildRelation( QgsProject *project, QgsVectorLayer *referencedLayer, QgsVectorLayer *referencingLayer, Qgis::RelationshipStrength strength )
{
  const QgsRelationContext relationContext( project );
  QgsRelation relation( relationContext );
  relation.setId( QStringLiteral( "parent_child" ) );
  relation.setName( QStringLiteral( "parent_child" ) );
  relation.setReferencedLayer( referencedLayer->id() );
  relation.setReferencingLayer( referencingLayer->id() );
  relation.addFieldPair( QStringLiteral( "parent_pk" ), QStringLiteral( "pk" ) );
  relation.setStrength( strength );
  REQUIRE( relation.isValid() );

  project->relationManager()->addRelation( relation );
}

static QMap<int, QgsAttributes> attributesByPrimaryKey( QgsVectorLayer *layer )
{
  QMap<int, QgsAttributes> attributesByKey;
  QgsFeatureIterator features = layer->getFeatures();
  QgsFeature feature;
  while ( features.nextFeature( feature ) )
  {
    attributesByKey.insert( feature.attribute( QStringLiteral( "pk" ) ).toInt(), feature.attributes() );
  }
  return attributesByKey;
}

static QgsFeatureId featureIdByExpression( QgsVectorLayer *layer, const QString &expression )
{
  QgsFeatureRequest request;
  request.setFilterExpression( expression );
  QgsFeature feature;
  REQUIRE( layer->getFeatures( request ).nextFeature( feature ) );
  return feature.id();
}

TEST_CASE( "LayerUtils AddFeature" )
{
  std::unique_ptr<QgsVectorLayer> pointLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( pointLayer->isValid() );

  QgsFeature featureToAdd( pointLayer->fields() );
  featureToAdd.setAttributes( QgsAttributes() << 1 << QStringLiteral( "a" ) );

  SECTION( "NullLayer" )
  {
    REQUIRE_FALSE( QfLayerUtils::addFeature( nullptr, featureToAdd ) );
  }

  SECTION( "NotEditingStartsAndCommits" )
  {
    REQUIRE( QfLayerUtils::addFeature( pointLayer.get(), featureToAdd ) );
    REQUIRE_FALSE( pointLayer->isEditable() );
    REQUIRE( pointLayer->featureCount() == 1 );

    QgsFeatureIterator addedFeatures = pointLayer->getFeatures();
    QgsFeature addedFeature;
    REQUIRE( addedFeatures.nextFeature( addedFeature ) );
    REQUIRE( addedFeature.attributes() == ( QgsAttributes() << 1 << QStringLiteral( "a" ) ) );
    REQUIRE_FALSE( addedFeatures.nextFeature( addedFeature ) );
  }

  SECTION( "AlreadyEditingKeepsBufferOpenUntilRollback" )
  {
    REQUIRE( pointLayer->startEditing() );
    REQUIRE( QfLayerUtils::addFeature( pointLayer.get(), featureToAdd ) );
    REQUIRE( pointLayer->isEditable() );
    REQUIRE( pointLayer->isModified() );
    REQUIRE( pointLayer->featureCount() == 1 );

    REQUIRE( pointLayer->rollBack() );
    REQUIRE( pointLayer->featureCount() == 0 );
  }

  SECTION( "AlreadyEditingCommittedByCaller" )
  {
    REQUIRE( pointLayer->startEditing() );
    REQUIRE( QfLayerUtils::addFeature( pointLayer.get(), featureToAdd ) );
    REQUIRE( pointLayer->commitChanges() );
    REQUIRE( pointLayer->featureCount() == 1 );

    QgsFeatureIterator addedFeatures = pointLayer->getFeatures();
    QgsFeature addedFeature;
    REQUIRE( addedFeatures.nextFeature( addedFeature ) );
    REQUIRE( addedFeature.attributes() == ( QgsAttributes() << 1 << QStringLiteral( "a" ) ) );
    REQUIRE_FALSE( addedFeatures.nextFeature( addedFeature ) );
  }
}

TEST_CASE( "LayerUtils DeleteFeature" )
{
  std::unique_ptr<QgsProject> project = std::make_unique<QgsProject>();
  std::unique_ptr<QgsVectorLayer> pointLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( pointLayer->isValid() );

  QgsFeature firstFeature( pointLayer->fields() );
  firstFeature.setAttributes( QgsAttributes() << 1 << QStringLiteral( "a" ) );
  QgsFeature secondFeature( pointLayer->fields() );
  secondFeature.setAttributes( QgsAttributes() << 2 << QStringLiteral( "b" ) );

  pointLayer->startEditing();
  pointLayer->addFeature( firstFeature );
  pointLayer->addFeature( secondFeature );
  pointLayer->commitChanges();
  REQUIRE( pointLayer->featureCount() == 2 );

  QgsFeatureRequest firstFeatureRequest;
  firstFeatureRequest.setFilterExpression( QStringLiteral( "\"name\" = 'a'" ) );
  QgsFeature committedFirstFeature;
  REQUIRE( pointLayer->getFeatures( firstFeatureRequest ).nextFeature( committedFirstFeature ) );
  const QgsFeatureId firstFeatureId = committedFirstFeature.id();

  SECTION( "NullProject" )
  {
    REQUIRE_FALSE( QfLayerUtils::deleteFeature( nullptr, pointLayer.get(), firstFeatureId ) );
    REQUIRE( pointLayer->featureCount() == 2 );
  }

  SECTION( "NullLayer" )
  {
    REQUIRE_FALSE( QfLayerUtils::deleteFeature( project.get(), nullptr, firstFeatureId ) );
  }

  SECTION( "NotEditingForcesFlush" )
  {
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), pointLayer.get(), firstFeatureId, false ) );
    REQUIRE_FALSE( pointLayer->isEditable() );
    REQUIRE( pointLayer->featureCount() == 1 );

    QgsFeatureIterator remainingFeatures = pointLayer->getFeatures();
    QgsFeature remainingFeature;
    REQUIRE( remainingFeatures.nextFeature( remainingFeature ) );
    REQUIRE( remainingFeature.attributes() == ( QgsAttributes() << 2 << QStringLiteral( "b" ) ) );
    REQUIRE_FALSE( remainingFeatures.nextFeature( remainingFeature ) );
  }

  SECTION( "NoFlushKeepsBufferOpenUntilRollback" )
  {
    REQUIRE( pointLayer->startEditing() );
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), pointLayer.get(), firstFeatureId, false ) );
    REQUIRE( pointLayer->isEditable() );
    REQUIRE( pointLayer->isModified() );
    REQUIRE( pointLayer->featureCount() == 1 );

    REQUIRE( pointLayer->rollBack() );
    REQUIRE( pointLayer->featureCount() == 2 );
  }

  SECTION( "FlushWhileAlreadyEditingStaysInEditMode" )
  {
    REQUIRE( pointLayer->startEditing() );
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), pointLayer.get(), firstFeatureId, true ) );
    REQUIRE( pointLayer->isEditable() );
    REQUIRE_FALSE( pointLayer->isModified() );
    REQUIRE( pointLayer->featureCount() == 1 );
    REQUIRE( pointLayer->commitChanges() );
  }

  SECTION( "NonExistentFeature" )
  {
    const QgsFeatureId nonExistentFeatureId = 999;
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), pointLayer.get(), nonExistentFeatureId ) );
    REQUIRE( pointLayer->featureCount() == 2 );
  }
}

TEST_CASE( "LayerUtils DeleteFeature with related features" )
{
  std::unique_ptr<QgsVectorLayer> parentLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=pk:integer&field=name:string" ), QStringLiteral( "parent" ), QStringLiteral( "memory" ) );
  std::unique_ptr<QgsVectorLayer> childLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=pk:integer&field=parent_pk:integer" ), QStringLiteral( "child" ), QStringLiteral( "memory" ) );
  REQUIRE( parentLayer->isValid() );
  REQUIRE( childLayer->isValid() );

  std::unique_ptr<QgsProject> project = std::make_unique<QgsProject>();
  project->addMapLayer( parentLayer.get(), false, false );
  project->addMapLayer( childLayer.get(), false, false );

  QgsFeature firstParent( parentLayer->fields() );
  firstParent.setAttributes( QgsAttributes() << 1 << QStringLiteral( "a" ) );
  QgsFeature secondParent( parentLayer->fields() );
  secondParent.setAttributes( QgsAttributes() << 2 << QStringLiteral( "b" ) );

  parentLayer->startEditing();
  parentLayer->addFeature( firstParent );
  parentLayer->addFeature( secondParent );
  parentLayer->commitChanges();
  REQUIRE( parentLayer->featureCount() == 2 );

  QgsFeature firstParentChild( childLayer->fields() );
  firstParentChild.setAttributes( QgsAttributes() << 10 << 1 );
  QgsFeature firstParentOtherChild( childLayer->fields() );
  firstParentOtherChild.setAttributes( QgsAttributes() << 11 << 1 );
  QgsFeature secondParentChild( childLayer->fields() );
  secondParentChild.setAttributes( QgsAttributes() << 12 << 2 );

  childLayer->startEditing();
  childLayer->addFeature( firstParentChild );
  childLayer->addFeature( firstParentOtherChild );
  childLayer->addFeature( secondParentChild );
  childLayer->commitChanges();
  REQUIRE( childLayer->featureCount() == 3 );

  const QgsFeatureId firstParentId = featureIdByExpression( parentLayer.get(), QStringLiteral( "\"pk\" = 1" ) );

  SECTION( "CompositionDeletesRelatedFeatures" )
  {
    addParentChildRelation( project.get(), parentLayer.get(), childLayer.get(), Qgis::RelationshipStrength::Composition );

    REQUIRE( QfLayerUtils::deleteFeature( project.get(), parentLayer.get(), firstParentId ) );

    REQUIRE_FALSE( parentLayer->isEditable() );

    const QMap<int, QgsAttributes> remainingParents = attributesByPrimaryKey( parentLayer.get() );
    REQUIRE( remainingParents.keys() == QList<int>( { 2 } ) );
    REQUIRE( remainingParents.value( 2 ) == ( QgsAttributes() << 2 << QStringLiteral( "b" ) ) );

    const QMap<int, QgsAttributes> remainingChildren = attributesByPrimaryKey( childLayer.get() );
    REQUIRE( remainingChildren.keys() == QList<int>( { 12 } ) );
    REQUIRE( remainingChildren.value( 12 ) == ( QgsAttributes() << 12 << 2 ) );
  }

  SECTION( "AssociationKeepsRelatedFeatures" )
  {
    addParentChildRelation( project.get(), parentLayer.get(), childLayer.get(), Qgis::RelationshipStrength::Association );

    REQUIRE( QfLayerUtils::deleteFeature( project.get(), parentLayer.get(), firstParentId ) );

    REQUIRE_FALSE( childLayer->isEditable() );

    const QMap<int, QgsAttributes> remainingParents = attributesByPrimaryKey( parentLayer.get() );
    REQUIRE( remainingParents.keys() == QList<int>( { 2 } ) );
    REQUIRE( remainingParents.value( 2 ) == ( QgsAttributes() << 2 << QStringLiteral( "b" ) ) );

    const QMap<int, QgsAttributes> remainingChildren = attributesByPrimaryKey( childLayer.get() );
    REQUIRE( remainingChildren.keys() == QList<int>( { 10, 11, 12 } ) );
    REQUIRE( remainingChildren.value( 10 ) == ( QgsAttributes() << 10 << 1 ) );
    REQUIRE( remainingChildren.value( 11 ) == ( QgsAttributes() << 11 << 1 ) );
    REQUIRE( remainingChildren.value( 12 ) == ( QgsAttributes() << 12 << 2 ) );
  }

  SECTION( "NoRelationKeepsOtherLayerUntouched" )
  {
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), parentLayer.get(), firstParentId ) );

    REQUIRE_FALSE( childLayer->isEditable() );

    const QMap<int, QgsAttributes> remainingParents = attributesByPrimaryKey( parentLayer.get() );
    REQUIRE( remainingParents.keys() == QList<int>( { 2 } ) );
    REQUIRE( remainingParents.value( 2 ) == ( QgsAttributes() << 2 << QStringLiteral( "b" ) ) );

    const QMap<int, QgsAttributes> remainingChildren = attributesByPrimaryKey( childLayer.get() );
    REQUIRE( remainingChildren.keys() == QList<int>( { 10, 11, 12 } ) );
    REQUIRE( remainingChildren.value( 10 ) == ( QgsAttributes() << 10 << 1 ) );
    REQUIRE( remainingChildren.value( 11 ) == ( QgsAttributes() << 11 << 1 ) );
    REQUIRE( remainingChildren.value( 12 ) == ( QgsAttributes() << 12 << 2 ) );
  }
}

TEST_CASE( "LayerUtils DuplicateFeature" )
{
  std::unique_ptr<QgsVectorLayer> pointLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( pointLayer->isValid() );

  QgsFeature featureToAdd( pointLayer->fields() );
  featureToAdd.setAttributes( QgsAttributes() << 1 << QStringLiteral( "a" ) );
  featureToAdd.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 5.0, 6.0 ) ) );

  pointLayer->startEditing();
  pointLayer->addFeature( featureToAdd );
  pointLayer->commitChanges();
  REQUIRE( pointLayer->featureCount() == 1 );

  QgsFeature sourceFeature;
  REQUIRE( pointLayer->getFeatures().nextFeature( sourceFeature ) );
  REQUIRE( sourceFeature.isValid() );

  const int fidFieldIndex = pointLayer->fields().indexFromName( QStringLiteral( "fid" ) );
  const int nameFieldIndex = pointLayer->fields().indexFromName( QStringLiteral( "name" ) );
  REQUIRE( fidFieldIndex >= 0 );
  REQUIRE( nameFieldIndex >= 0 );

  SECTION( "NullLayer" )
  {
    QgsFeature duplicatedFeature = QfLayerUtils::duplicateFeature( nullptr, sourceFeature );
    REQUIRE_FALSE( duplicatedFeature.isValid() );
  }

  SECTION( "InvalidFeature" )
  {
    QgsFeature duplicatedFeature = QfLayerUtils::duplicateFeature( pointLayer.get(), QgsFeature() );
    REQUIRE_FALSE( duplicatedFeature.isValid() );
    REQUIRE( pointLayer->featureCount() == 1 );
  }

  SECTION( "DefaultPolicyCarriesAttributes" )
  {
    QgsFeature duplicatedFeature = QfLayerUtils::duplicateFeature( pointLayer.get(), sourceFeature );
    REQUIRE( duplicatedFeature.isValid() );
    REQUIRE( duplicatedFeature.id() != sourceFeature.id() );
    REQUIRE( duplicatedFeature.attributes() == ( QgsAttributes() << 1 << QStringLiteral( "a" ) ) );
    REQUIRE( duplicatedFeature.geometry().asPoint().x() == Catch::Approx( 5.0 ) );
    REQUIRE( duplicatedFeature.geometry().asPoint().y() == Catch::Approx( 6.0 ) );
    REQUIRE( pointLayer->featureCount() == 2 );
  }

  SECTION( "UnsetFieldPolicyLeavesAttributeUnset" )
  {
    pointLayer->setFieldDuplicatePolicy( nameFieldIndex, Qgis::FieldDuplicatePolicy::UnsetField );

    QgsFeature duplicatedFeature = QfLayerUtils::duplicateFeature( pointLayer.get(), sourceFeature );
    REQUIRE( duplicatedFeature.isValid() );
    REQUIRE( QgsVariantUtils::isUnsetAttributeValue( duplicatedFeature.attribute( nameFieldIndex ) ) );
    REQUIRE( duplicatedFeature.attribute( fidFieldIndex ) == 1 );
    REQUIRE( pointLayer->featureCount() == 2 );

    const QgsFeature storedFeature = pointLayer->getFeature( duplicatedFeature.id() );
    REQUIRE( storedFeature.attributes() == duplicatedFeature.attributes() );
  }

  SECTION( "SourcePrimaryKeyNulledOnDuplicate" )
  {
    pointLayer->setCustomProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ), QStringLiteral( "fid" ) );

    QgsFeature duplicatedFeature = QfLayerUtils::duplicateFeature( pointLayer.get(), sourceFeature );
    REQUIRE( duplicatedFeature.isValid() );
    REQUIRE( duplicatedFeature.attribute( fidFieldIndex ).isNull() );
    REQUIRE( duplicatedFeature.attribute( nameFieldIndex ).toString() == QStringLiteral( "a" ) );
    REQUIRE( pointLayer->featureCount() == 2 );
  }
}

TEST_CASE( "LayerUtils MemoryLayerFromJsonString" )
{
  SECTION( "ValidFeatureCollection" )
  {
    const QString geoJson = QStringLiteral( "{\"type\":\"FeatureCollection\",\"features\":[{\"type\":\"Feature\",\"geometry\":{\"type\":\"Point\",\"coordinates\":[1,2]},\"properties\":{\"name\":\"a\",\"val\":10}},{\"type\":\"Feature\",\"geometry\":{\"type\":\"Point\",\"coordinates\":[3,4]},\"properties\":{\"name\":\"b\",\"val\":20}}]}" );

    std::unique_ptr<QgsVectorLayer> parsedLayer( QfLayerUtils::memoryLayerFromJsonString( QStringLiteral( "parsed_layer" ), geoJson, QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
    REQUIRE( parsedLayer != nullptr );
    REQUIRE( parsedLayer->isValid() );
    REQUIRE( parsedLayer->name() == QStringLiteral( "parsed_layer" ) );
    REQUIRE( parsedLayer->crs().authid() == QStringLiteral( "EPSG:4326" ) );
    REQUIRE( parsedLayer->wkbType() == Qgis::WkbType::Point );
    REQUIRE( parsedLayer->fields().names() == QStringList( { QStringLiteral( "name" ), QStringLiteral( "val" ) } ) );
    REQUIRE( parsedLayer->featureCount() == 2 );

    QMap<QString, QgsFeature> parsedFeaturesByName;
    QgsFeatureIterator parsedFeatures = parsedLayer->getFeatures();
    QgsFeature parsedFeature;
    while ( parsedFeatures.nextFeature( parsedFeature ) )
      parsedFeaturesByName.insert( parsedFeature.attribute( QStringLiteral( "name" ) ).toString(), parsedFeature );

    REQUIRE( parsedFeaturesByName.keys() == QStringList( { QStringLiteral( "a" ), QStringLiteral( "b" ) } ) );

    const QgsFeature firstParsedFeature = parsedFeaturesByName.value( QStringLiteral( "a" ) );
    REQUIRE( firstParsedFeature.attribute( QStringLiteral( "val" ) ).toInt() == 10 );
    REQUIRE( firstParsedFeature.geometry().asPoint().x() == Catch::Approx( 1.0 ) );
    REQUIRE( firstParsedFeature.geometry().asPoint().y() == Catch::Approx( 2.0 ) );

    const QgsFeature secondParsedFeature = parsedFeaturesByName.value( QStringLiteral( "b" ) );
    REQUIRE( secondParsedFeature.attribute( QStringLiteral( "val" ) ).toInt() == 20 );
    REQUIRE( secondParsedFeature.geometry().asPoint().x() == Catch::Approx( 3.0 ) );
    REQUIRE( secondParsedFeature.geometry().asPoint().y() == Catch::Approx( 4.0 ) );
  }

  SECTION( "EmptyFeatureCollectionReturnsNull" )
  {
    const QString geoJson = QStringLiteral( "{\"type\":\"FeatureCollection\",\"features\":[]}" );
    REQUIRE( QfLayerUtils::memoryLayerFromJsonString( QStringLiteral( "parsed_layer" ), geoJson ) == nullptr );
  }
}
