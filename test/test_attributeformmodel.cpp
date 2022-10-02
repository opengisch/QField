/***************************************************************************
                        test_attributeformmodel.h
                        --------------------
  begin                : Jul 2021
  copyright            : (C) 2021 by Mathieu Pellerin
  email                : mathieu@opengis.ch
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
#include "attributeformmodel.h"
#include "catch2.h"
#include "featuremodel.h"


TEST_CASE( "AttributeFormModel" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=str:string&field=str2:string" ), QStringLiteral( "Input Layer" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature f1;
  f1.setAttributes( QgsAttributes() << 1 << QStringLiteral( "string_a1" ) << QStringLiteral( "string_b1" ) );
  qDebug() << f1.attributes();
  QgsFeature f2;
  f2.setAttributes( QgsAttributes() << 2 << QStringLiteral( "string_a2" ) << QStringLiteral( "string_b2" ) );
  QgsFeature f3;
  f3.setAttributes( QgsAttributes() << 1 << QStringLiteral( "string_a3" ) << QStringLiteral( "string_b3" ) );

  layer->startEditing();
  layer->addFeature( f1 );
  layer->addFeature( f2 );
  layer->addFeature( f3 );
  layer->commitChanges();
  REQUIRE( layer->featureCount() == 3 );

  layer->setDefaultValueDefinition( 2, QgsDefaultValue( QStringLiteral( " coalesce(\"str\",'') || '__'" ), true ) );

  std::unique_ptr<AttributeFormModel> attributeFormModel = std::make_unique<AttributeFormModel>();
  std::unique_ptr<FeatureModel> featureModel = std::make_unique<FeatureModel>();
  attributeFormModel->setFeatureModel( featureModel.get() );
  featureModel->setCurrentLayer( layer.get() );

  SECTION( "Attributes" )
  {
    featureModel->setFeature( layer->getFeature( 1 ) );
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str" ) ) == QStringLiteral( "string_a1" ) );
    REQUIRE( attributeFormModel->data( attributeFormModel->index( 1, 0 ), AttributeFormModel::AttributeValue ) == QStringLiteral( "string_a1" ) );
  }

  SECTION( "FeatureDefaultValue" )
  {
    featureModel->resetFeature();
    featureModel->resetAttributes();

    attributeFormModel->setData( attributeFormModel->index( 1, 0 ), QString( "new_feature" ), AttributeFormModel::AttributeValue );
    // test default value changed on update with new feature
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str2" ) ) == QStringLiteral( "new_feature__" ) );

    // create a feature through the attribute form model
    REQUIRE( attributeFormModel->create() );

    QgsFeatureId fid = featureModel->feature().id();
    REQUIRE( fid > 0 );

    attributeFormModel->setData( attributeFormModel->index( 1, 0 ), QString( "edit_feature" ), AttributeFormModel::AttributeValue );
    // test default value changed on update with existing feature being edited
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str2" ) ) == QStringLiteral( "edit_feature__" ) );

    REQUIRE( attributeFormModel->save() );

    QgsFeature feature = layer->getFeature( fid );
    REQUIRE( feature.attributes().at( 2 ) == QStringLiteral( "edit_feature__" ) );
  }
}
