/***************************************************************************
  attributeformmodel.cpp - AttributeFormModel

 ---------------------
 begin                : 16.8.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "attributeformmodel.h"
#include "qgsvectorlayer.h"

#include <QDebug>

AttributeFormModel::AttributeFormModel( QObject* parent )
  : QStandardItemModel( 0, 1, parent )
  , mFeatureModel( nullptr )
  , mLayer( nullptr )
  , mTemporaryContainer( nullptr )
{
}

AttributeFormModel::~AttributeFormModel()
{
  delete mTemporaryContainer;
}

QHash<int, QByteArray> AttributeFormModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[ElementType]  = "Type";
  roles[Name]  = "Name";
  roles[AttributeValue] = "AttributeValue";
  roles[EditorWidget] = "EditorWidget";
  roles[EditorWidgetConfig] = "EditorWidgetConfig";
  roles[RememberValue] = "RememberValue";
  roles[Field] = "Field";
  roles[Group] = "Group";

  return roles;
}

#if 0
QVariant AttributeFormModel::data( const QModelIndex& index, int role ) const
{
  if ( !index.isValid() )
    return QVariant();

  if ( role == ElementType )
  {
    switch ( indexToElement( index )->type() )
    {
      case QgsAttributeEditorElement::AeTypeContainer:
        return "container";
        break;

      case QgsAttributeEditorElement::AeTypeField:
        return "field";
        break;

      case QgsAttributeEditorElement::AeTypeRelation:
        return "relation";
        break;

      case QgsAttributeEditorElement::AeTypeInvalid:
        return QVariant();
        break;
    }
  }

  switch( indexToElement( index )->type() )
  {
    case QgsAttributeEditorElement::AeTypeContainer:
    {
      QgsAttributeEditorContainer* container = indexToElement<QgsAttributeEditorContainer*>( index );

      switch ( role )
      {
        case Name:
          return container->name();
      }
      break;
    }

    case QgsAttributeEditorElement::AeTypeField:
    {
      QgsAttributeEditorField* editorField = indexToElement<QgsAttributeEditorField*>( index );
      if ( editorField->idx() < 0 )
        return QVariant();

      switch ( role )
      {
        case Name:
          return mLayer->attributeDisplayName( editorField->idx() );

        case AttributeValue:
          return mFeatureModel->feature().attribute( editorField->idx() );

        case AttributeEditable:
          return mLayer->editFormConfig().readOnly( editorField->idx() );

        case EditorWidget:
          return mLayer->editFormConfig().widgetType( editorField->name() );

        case EditorWidgetConfig:
          return mLayer->editFormConfig().widgetConfig( editorField->name() );

        case RememberValue:
          return mFeatureModel->rememberedAttributes().at ( editorField->idx() ) ? Qt::Checked : Qt::Unchecked;

        case Field:
          return mLayer->fields().at( editorField->idx() );

        case Qt::DisplayRole:
          return mLayer->attributeDisplayName( editorField->idx() );
      }
      break;
    }

    case QgsAttributeEditorElement::AeTypeRelation:
      // TODO
      break;

    case QgsAttributeEditorElement::AeTypeInvalid:
      break;
  }

  return QVariant();
}

bool AttributeFormModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
  if ( data( index, role ) != value )
  {
    switch ( role )
    {
      case RememberValue:
      {
        QgsAttributeEditorField* editorField = indexToElement<QgsAttributeEditorField*>( index );
        return mFeatureModel->setData( mFeatureModel->index( editorField->idx() ), value, FeatureModel::RememberAttribute );
        break;
      }

      case AttributeValue:
      {
        QgsAttributeEditorField* editorField = indexToElement<QgsAttributeEditorField*>( index );
        return mFeatureModel->setData( mFeatureModel->index( editorField->idx() ), value, FeatureModel::AttributeValue );
        break;
      }
    }
  }
  return false;
}
#endif
FeatureModel* AttributeFormModel::featureModel() const
{
  return mFeatureModel;
}

void AttributeFormModel::setFeatureModel( FeatureModel* featureModel )
{
  if ( mFeatureModel == featureModel )
    return;

  disconnect( mFeatureModel, &FeatureModel::layerChanged, this, &AttributeFormModel::onLayerChanged );
  disconnect( mFeatureModel, &FeatureModel::featureChanged, this, &AttributeFormModel::onFeatureChanged );


  mFeatureModel = featureModel;

  connect( mFeatureModel, &FeatureModel::layerChanged, this, &AttributeFormModel::onLayerChanged );
  connect( mFeatureModel, &FeatureModel::featureChanged, this, &AttributeFormModel::onFeatureChanged );

  emit featureModelChanged();
}

void AttributeFormModel::onLayerChanged()
{
  clear();

  mLayer = mFeatureModel->layer();

  if ( mLayer )
  {
    QgsAttributeEditorContainer* root;
    delete mTemporaryContainer;
    mTemporaryContainer = nullptr;

    if ( mLayer->editFormConfig().layout() == QgsEditFormConfig::TabLayout )
    {
      root = mLayer->editFormConfig().invisibleRootContainer();
    }
    else
    {
      root = generateRootContainer();
      mTemporaryContainer = root;
    }

    setHasTabs( !root->children().isEmpty() && QgsAttributeEditorElement::AeTypeContainer == root->children().first()->type() );

    QList<QStandardItem*> items;
    if ( mHasTabs )
    {
      Q_FOREACH( QgsAttributeEditorElement* element, root->children() )
      {
        QStandardItem* item = new QStandardItem();
        item->setData( element->name(), Name );
        item->setData( "container", ElementType );
        item->appendRows( flatten( static_cast<QgsAttributeEditorContainer*>( element ) ) );
        items.append( item );
      }
    }
    else
    {
      items += flatten( invisibleRootContainer() );
    }

    invisibleRootItem()->setColumnCount( 1 );
    invisibleRootItem()->appendRows( items );
  }
}

void AttributeFormModel::onFeatureChanged()
{
  for ( int i = 0 ; i < invisibleRootItem()->rowCount(); ++i )
  {
    updateAttributeValue( invisibleRootItem()->child( i ) );
  }
}

QgsAttributeEditorContainer* AttributeFormModel::generateRootContainer() const
{
  QgsAttributeEditorContainer* root = new QgsAttributeEditorContainer( QString(), nullptr );
  QgsFields fields = mLayer->fields();
  for ( int i = 0; i < fields.size(); ++i )
  {
    QgsAttributeEditorField* field = new QgsAttributeEditorField( fields.at( i ).name(), i, root );
    root->addChildElement( field );
  }
  return root;
}

QgsAttributeEditorContainer* AttributeFormModel::invisibleRootContainer() const
{
  return mTemporaryContainer ? mTemporaryContainer : mLayer->editFormConfig().invisibleRootContainer();
}

void AttributeFormModel::updateAttributeValue( QStandardItem* item )
{
  if ( item->data( ElementType ) == "field" )
    item->setData( mFeatureModel->feature().attribute( item->data( FieldIndex ).toInt() ), AttributeValue );
  else
    for ( int i = 0; i < item->rowCount(); ++i )
      updateAttributeValue( item->child( i ) );
}

QList<QStandardItem*> AttributeFormModel::flatten( QgsAttributeEditorContainer* container )
{
  QList<QStandardItem*> items;

  Q_FOREACH( QgsAttributeEditorElement* element, container->children() )
  {
    switch ( element->type() )
    {
      case QgsAttributeEditorElement::AeTypeContainer:
        items += flatten( static_cast<QgsAttributeEditorContainer*>( element ) );
        break;

      case QgsAttributeEditorElement::AeTypeField:
      {
        QgsAttributeEditorField* editorField = static_cast<QgsAttributeEditorField*>( element );
        if ( editorField->idx() == -1 )
          continue;

        QStandardItem* item = new QStandardItem();

        item->setData( mLayer->attributeDisplayName( editorField->idx() ), Name );
        item->setData( mFeatureModel->feature().attribute( editorField->idx() ), AttributeValue );
        item->setData( mLayer->editFormConfig().readOnly( editorField->idx() ), AttributeEditable );
        item->setData( mLayer->editFormConfig().widgetType( editorField->name() ), EditorWidget );
        qWarning() << mLayer->editFormConfig().widgetType( editorField->name() );
        item->setData( mLayer->editFormConfig().widgetConfig( editorField->name() ), EditorWidgetConfig );
        item->setData( mFeatureModel->rememberedAttributes().at( editorField->idx() ) ? Qt::Checked : Qt::Unchecked, RememberValue );
        item->setData( mLayer->fields().at( editorField->idx() ), Field );
        item->setData( "field", ElementType );
        item->setData( editorField->idx(), FieldIndex );
        item->setData( container->isGroupBox() ? container->name() : QString(), Group );

        items.append( item );
        break;
      }
    }
  }
  return items;
}

bool AttributeFormModel::hasTabs() const
{
  return mHasTabs;
}

void AttributeFormModel::setHasTabs( bool hasTabs )
{
  if ( hasTabs == mHasTabs )
    return;

  mHasTabs = hasTabs;
  emit hasTabsChanged();
}

void AttributeFormModel::save()
{
  mFeatureModel->save();
}

void AttributeFormModel::create()
{
  mFeatureModel->create();
}
