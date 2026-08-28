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

#include <qgsfeature.h>
#include <qgsfeatureiterator.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <memory>

TEST_CASE( "LayerUtils AddFeature" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature f( layer->fields() );
  f.setAttribute( QStringLiteral( "fid" ), 1 );
  f.setAttribute( QStringLiteral( "name" ), QStringLiteral( "a" ) );

  SECTION( "NullLayer" )
  {
    REQUIRE_FALSE( QfLayerUtils::addFeature( nullptr, f ) );
  }

  SECTION( "NotEditing" )
  {
    REQUIRE_FALSE( QfLayerUtils::addFeature( layer.get(), f ) );
    REQUIRE( layer->featureCount() == 0 );
  }

  SECTION( "Editing" )
  {
    REQUIRE( layer->startEditing() );
    REQUIRE( QfLayerUtils::addFeature( layer.get(), f ) );
    REQUIRE( layer->commitChanges() );
    REQUIRE( layer->featureCount() == 1 );

    QgsFeature added = layer->getFeature( 1 );
    REQUIRE( added.isValid() );
    REQUIRE( added.attribute( QStringLiteral( "fid" ) ) == 1 );
    REQUIRE( added.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "a" ) );
  }
}

TEST_CASE( "LayerUtils DeleteFeature" )
{
  std::unique_ptr<QgsProject> project = std::make_unique<QgsProject>();
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature f1( layer->fields() );
  f1.setAttribute( QStringLiteral( "fid" ), 1 );
  f1.setAttribute( QStringLiteral( "name" ), QStringLiteral( "a" ) );
  QgsFeature f2( layer->fields() );
  f2.setAttribute( QStringLiteral( "fid" ), 2 );
  f2.setAttribute( QStringLiteral( "name" ), QStringLiteral( "b" ) );

  layer->startEditing();
  layer->addFeature( f1 );
  layer->addFeature( f2 );
  layer->commitChanges();
  REQUIRE( layer->featureCount() == 2 );

  SECTION( "NullProject" )
  {
    REQUIRE_FALSE( QfLayerUtils::deleteFeature( nullptr, layer.get(), 1 ) );
    REQUIRE( layer->featureCount() == 2 );
  }

  SECTION( "NullLayer" )
  {
    REQUIRE_FALSE( QfLayerUtils::deleteFeature( project.get(), nullptr, 1 ) );
  }

  SECTION( "NotEditingForcesFlush" )
  {
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), layer.get(), 1, false ) );
    REQUIRE_FALSE( layer->isEditable() );
    REQUIRE( layer->featureCount() == 1 );

    QgsFeatureIterator it = layer->getFeatures();
    QgsFeature f;
    REQUIRE( it.nextFeature( f ) );
    REQUIRE( f.id() == 2 );
    REQUIRE_FALSE( it.nextFeature( f ) );
  }

  SECTION( "NoFlushKeepsBufferOpenUntilRollback" )
  {
    REQUIRE( layer->startEditing() );
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), layer.get(), 1, false ) );
    REQUIRE( layer->isEditable() );
    REQUIRE( layer->isModified() );
    REQUIRE( layer->featureCount() == 1 );

    REQUIRE( layer->rollBack() );
    REQUIRE( layer->featureCount() == 2 );
  }

  SECTION( "FlushWhileAlreadyEditingStaysInEditMode" )
  {
    REQUIRE( layer->startEditing() );
    REQUIRE( QfLayerUtils::deleteFeature( project.get(), layer.get(), 1, true ) );
    REQUIRE( layer->isEditable() );
    REQUIRE_FALSE( layer->isModified() );
    REQUIRE( layer->featureCount() == 1 );
    REQUIRE( layer->commitChanges() );
  }
}

TEST_CASE( "LayerUtils DuplicateFeature" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature f( layer->fields() );
  f.setAttribute( QStringLiteral( "fid" ), 1 );
  f.setAttribute( QStringLiteral( "name" ), QStringLiteral( "a" ) );

  layer->startEditing();
  layer->addFeature( f );
  layer->commitChanges();
  REQUIRE( layer->featureCount() == 1 );

  QgsFeature source = layer->getFeature( 1 );
  REQUIRE( source.isValid() );

  SECTION( "InvalidLayer" )
  {
    QgsFeature duplicated = QfLayerUtils::duplicateFeature( nullptr, source );
    REQUIRE_FALSE( duplicated.isValid() );
  }

  SECTION( "InvalidFeature" )
  {
    QgsFeature duplicated = QfLayerUtils::duplicateFeature( layer.get(), QgsFeature() );
    REQUIRE_FALSE( duplicated.isValid() );
  }

  SECTION( "DefaultPolicyCarriesAttributes" )
  {
    QgsFeature duplicated = QfLayerUtils::duplicateFeature( layer.get(), source );
    REQUIRE( duplicated.isValid() );
    REQUIRE( duplicated.id() != source.id() );
    REQUIRE( duplicated.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "a" ) );
    REQUIRE( layer->featureCount() == 2 );
  }

  SECTION( "UnsetFieldPolicyDropsAttribute" )
  {
    const int nameIndex = layer->fields().indexFromName( QStringLiteral( "name" ) );
    REQUIRE( nameIndex >= 0 );
    layer->setFieldDuplicatePolicy( nameIndex, Qgis::FieldDuplicatePolicy::UnsetField );

    QgsFeature duplicated = QfLayerUtils::duplicateFeature( layer.get(), source );
    REQUIRE( duplicated.isValid() );
    REQUIRE( duplicated.attribute( QStringLiteral( "name" ) ).toString().isEmpty() );
  }

  SECTION( "SourcePrimaryKeyNulledOnDuplicate" )
  {
    layer->setCustomProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ), QStringLiteral( "fid" ) );

    QgsFeature duplicated = QfLayerUtils::duplicateFeature( layer.get(), source );
    REQUIRE( duplicated.isValid() );
    REQUIRE( duplicated.attribute( QStringLiteral( "fid" ) ).isNull() );
  }
}
