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
#include "catch2.h"
#include "qfattributeformmodel.h"
#include "qffeaturemodel.h"

#include <QAbstractItemModelTester>
#include <qgsattributeeditorcontainer.h>
#include <qgsattributeeditorfield.h>
#include <qgseditformconfig.h>
#include <qgsfieldconstraints.h>

static QModelIndex indexForField( QfAttributeFormModel *model, int fieldIndex )
{
  const QModelIndexList matches = model->match( model->index( 0, 0 ), QfAttributeFormModel::FieldIndex, fieldIndex, 1, Qt::MatchExactly | Qt::MatchRecursive );
  return matches.value( 0 );
}

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

  QgsEditFormConfig editFormConfig = layer->editFormConfig();
  QgsPropertyCollection properties = editFormConfig.dataDefinedFieldProperties( QStringLiteral( "str" ) );
  QgsProperty property;
  property.setExpressionString( QStringLiteral( "\"str\" is null" ) );
  property.setActive( true );
  properties.setProperty( QgsEditFormConfig::DataDefinedProperty::Editable, property );
  editFormConfig.setDataDefinedFieldProperties( QStringLiteral( "str" ), properties );
  layer->setEditFormConfig( editFormConfig );

  std::unique_ptr<QfAttributeFormModel> attributeFormModel = std::make_unique<QfAttributeFormModel>();
  std::unique_ptr<QfFeatureModel> featureModel = std::make_unique<QfFeatureModel>();
  attributeFormModel->setFeatureModel( featureModel.get() );
  featureModel->setCurrentLayer( layer.get() );

  SECTION( "Attributes" )
  {
    featureModel->setFeature( layer->getFeature( 1 ) );
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str" ) ) == QStringLiteral( "string_a1" ) );
    REQUIRE( attributeFormModel->data( attributeFormModel->index( 1, 0 ), QfAttributeFormModel::AttributeValue ) == QStringLiteral( "string_a1" ) );
  }

  SECTION( "FeatureDefaultValue" )
  {
    featureModel->resetFeature();
    featureModel->resetAttributes();

    attributeFormModel->setData( attributeFormModel->index( 1, 0 ), QString( "new_feature" ), QfAttributeFormModel::AttributeValue );
    // test default value changed on update with new feature
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str2" ) ) == QStringLiteral( "new_feature__" ) );

    // create a feature through the attribute form model
    REQUIRE( attributeFormModel->create() );

    QgsFeatureId fid = featureModel->feature().id();
    REQUIRE( fid > 0 );

    attributeFormModel->setData( attributeFormModel->index( 1, 0 ), QString( "edit_feature" ), QfAttributeFormModel::AttributeValue );
    // test default value changed on update with existing feature being edited
    REQUIRE( attributeFormModel->attribute( QStringLiteral( "str2" ) ) == QStringLiteral( "edit_feature__" ) );

    REQUIRE( attributeFormModel->save() );

    QgsFeature feature = layer->getFeature( fid );
    REQUIRE( feature.attributes().at( 2 ) == QStringLiteral( "edit_feature__" ) );
  }

  SECTION( "ReadOnlyDataDefinedProperty" )
  {
    featureModel->resetFeature();
    featureModel->resetAttributes();

    REQUIRE( attributeFormModel->data( attributeFormModel->index( 1, 0 ), QfAttributeFormModel::AttributeEditable ).toBool() == true );
    attributeFormModel->setData( attributeFormModel->index( 1, 0 ), QString( "data" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( attributeFormModel->data( attributeFormModel->index( 1, 0 ), QfAttributeFormModel::AttributeEditable ).toBool() == false );
  }

  SECTION( "HardConstraint" )
  {
    layer->setFieldConstraint( 1, QgsFieldConstraints::ConstraintNotNull, QgsFieldConstraints::ConstraintStrengthHard );

    std::unique_ptr<QfAttributeFormModel> model = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QfFeatureModel> fModel = std::make_unique<QfFeatureModel>();
    model->setFeatureModel( fModel.get() );
    fModel->setCurrentLayer( layer.get() );

    REQUIRE( model->hasConstraints() );

    const QModelIndex strField = indexForField( model.get(), 1 );
    REQUIRE( strField.isValid() );

    fModel->resetFeature();
    fModel->resetAttributes();

    model->setData( strField, QStringLiteral( "filled" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strField, QfAttributeFormModel::ConstraintHardValid ).toBool() == true );
    REQUIRE( model->constraintsHardValid() == true );

    model->setData( strField, QVariant(), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strField, QfAttributeFormModel::ConstraintHardValid ).toBool() == false );
    REQUIRE( model->constraintsHardValid() == false );

    model->setData( strField, QStringLiteral( "refilled" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strField, QfAttributeFormModel::ConstraintHardValid ).toBool() == true );
    REQUIRE( model->constraintsHardValid() == true );
  }

  SECTION( "SoftConstraint" )
  {
    layer->setConstraintExpression( 1, QStringLiteral( "length(\"str\") > 3" ) );
    layer->setFieldConstraint( 1, QgsFieldConstraints::ConstraintExpression, QgsFieldConstraints::ConstraintStrengthSoft );

    std::unique_ptr<QfAttributeFormModel> model = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QfFeatureModel> fModel = std::make_unique<QfFeatureModel>();
    model->setFeatureModel( fModel.get() );
    fModel->setCurrentLayer( layer.get() );

    const QModelIndex strField = indexForField( model.get(), 1 );
    REQUIRE( strField.isValid() );

    fModel->resetFeature();
    fModel->resetAttributes();

    model->setData( strField, QStringLiteral( "ab" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strField, QfAttributeFormModel::ConstraintSoftValid ).toBool() == false );
    REQUIRE( model->constraintsSoftValid() == false );

    model->setData( strField, QStringLiteral( "abcd" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strField, QfAttributeFormModel::ConstraintSoftValid ).toBool() == true );
    REQUIRE( model->constraintsSoftValid() == true );
  }

  SECTION( "ConstraintDescription" )
  {
    layer->setFieldConstraint( 1, QgsFieldConstraints::ConstraintNotNull, QgsFieldConstraints::ConstraintStrengthHard );

    std::unique_ptr<QfAttributeFormModel> model = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QfFeatureModel> fModel = std::make_unique<QfFeatureModel>();
    model->setFeatureModel( fModel.get() );
    fModel->setCurrentLayer( layer.get() );

    const QModelIndex strField = indexForField( model.get(), 1 );
    REQUIRE( strField.isValid() );
    REQUIRE( !model->data( strField, QfAttributeFormModel::ConstraintDescription ).toString().isEmpty() );
  }

  SECTION( "TabLayout" )
  {
    QgsEditFormConfig config = layer->editFormConfig();
    config.clearTabs();
    config.setLayout( Qgis::AttributeFormLayout::DragAndDrop );

    QgsAttributeEditorContainer *tab1 = new QgsAttributeEditorContainer( QStringLiteral( "First" ), nullptr );
    tab1->addChildElement( new QgsAttributeEditorField( QStringLiteral( "str" ), 1, tab1 ) );
    config.addTab( tab1 );

    QgsAttributeEditorContainer *tab2 = new QgsAttributeEditorContainer( QStringLiteral( "Second" ), nullptr );
    tab2->addChildElement( new QgsAttributeEditorField( QStringLiteral( "str2" ), 2, tab2 ) );
    config.addTab( tab2 );

    layer->setEditFormConfig( config );

    std::unique_ptr<QfAttributeFormModel> model = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QfFeatureModel> fModel = std::make_unique<QfFeatureModel>();
    model->setFeatureModel( fModel.get() );
    fModel->setCurrentLayer( layer.get() );

    REQUIRE( model->hasTabs() );
    REQUIRE( model->rowCount() == 2 );
    REQUIRE( model->data( model->index( 0, 0 ), QfAttributeFormModel::ElementType ) == QStringLiteral( "container" ) );
    REQUIRE( model->data( model->index( 1, 0 ), QfAttributeFormModel::ElementType ) == QStringLiteral( "container" ) );
  }

  SECTION( "FieldSynchronization" )
  {
    QgsEditFormConfig config = layer->editFormConfig();
    config.clearTabs();
    config.setLayout( Qgis::AttributeFormLayout::DragAndDrop );

    QgsAttributeEditorContainer *tab1 = new QgsAttributeEditorContainer( QStringLiteral( "Tab1" ), nullptr );
    tab1->addChildElement( new QgsAttributeEditorField( QStringLiteral( "str" ), 1, tab1 ) );
    config.addTab( tab1 );

    QgsAttributeEditorContainer *tab2 = new QgsAttributeEditorContainer( QStringLiteral( "Tab2" ), nullptr );
    tab2->addChildElement( new QgsAttributeEditorField( QStringLiteral( "str" ), 1, tab2 ) );
    config.addTab( tab2 );

    layer->setEditFormConfig( config );

    std::unique_ptr<QfAttributeFormModel> model = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QfFeatureModel> fModel = std::make_unique<QfFeatureModel>();
    model->setFeatureModel( fModel.get() );
    fModel->setCurrentLayer( layer.get() );

    REQUIRE( model->hasTabs() );
    fModel->setFeature( layer->getFeature( 1 ) );

    const QModelIndexList strItems = model->match( model->index( 0, 0 ), QfAttributeFormModel::FieldIndex, 1, -1, Qt::MatchExactly | Qt::MatchRecursive );
    REQUIRE( strItems.size() == 2 );

    model->setData( strItems.at( 0 ), QStringLiteral( "synced" ), QfAttributeFormModel::AttributeValue );
    REQUIRE( model->data( strItems.at( 0 ), QfAttributeFormModel::AttributeValue ) == QStringLiteral( "synced" ) );
    REQUIRE( model->data( strItems.at( 1 ), QfAttributeFormModel::AttributeValue ) == QStringLiteral( "synced" ) );
  }

  SECTION( "QAbstractItemModelTester" )
  {
    std::unique_ptr<QfAttributeFormModel> modelTest = std::make_unique<QfAttributeFormModel>();
    std::unique_ptr<QAbstractItemModelTester> modelTester = std::make_unique<QAbstractItemModelTester>( modelTest.get(), QAbstractItemModelTester::FailureReportingMode::Fatal );
  }
}
