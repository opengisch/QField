/***************************************************************************
  attributeformmodelbase.cpp - AttributeFormModelBase

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

#include "attributeformmodelbase.h"
#include "attributeformmodel.h"
#include "qgsvectorlayer.h"

AttributeFormModelBase::AttributeFormModelBase( QObject* parent )
  : QStandardItemModel( 0, 1, parent )
  , mFeatureModel( nullptr )
  , mLayer( nullptr )
  , mTemporaryContainer( nullptr )
{
}

AttributeFormModelBase::~AttributeFormModelBase()
{
  delete mTemporaryContainer;
}

QHash<int, QByteArray> AttributeFormModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[AttributeFormModel::ElementType]  = "Type";
  roles[AttributeFormModel::Name]  = "Name";
  roles[AttributeFormModel::AttributeValue] = "AttributeValue";
  roles[AttributeFormModel::EditorWidget] = "EditorWidget";
  roles[AttributeFormModel::EditorWidgetConfig] = "EditorWidgetConfig";
  roles[AttributeFormModel::RememberValue] = "RememberValue";
  roles[AttributeFormModel::Field] = "Field";
  roles[AttributeFormModel::Group] = "Group";

  return roles;
}

bool AttributeFormModelBase::setData( const QModelIndex& index, const QVariant& value, int role )
{
  if ( data( index, role ) != value )
  {
    switch ( role )
    {
      case AttributeFormModel::RememberValue:
      {
        QStandardItem* item = itemFromIndex( index );
        int fieldIndex = item->data( AttributeFormModel::FieldIndex ).toInt();
        bool changed = mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, FeatureModel::RememberAttribute );
        if ( changed )
        {
          item->setData( value, AttributeFormModel::RememberValue );
          emit dataChanged( index, index, QVector<int>() << role );
        }
        return changed;
        break;
      }

      case AttributeFormModel::AttributeValue:
      {
        QStandardItem* item = itemFromIndex( index );
        int fieldIndex = item->data( AttributeFormModel::FieldIndex ).toInt();
        bool changed = mFeatureModel->setData( mFeatureModel->index( fieldIndex ), value, FeatureModel::AttributeValue );
        if ( changed )
        {
          item->setData( value, AttributeFormModel::AttributeValue );
          emit dataChanged( index, index, QVector<int>() << role );
        }
        updateVisibility( fieldIndex );
        return changed;
        break;
      }
    }
  }
  return false;
}

FeatureModel* AttributeFormModelBase::featureModel() const
{
  return mFeatureModel;
}

void AttributeFormModelBase::setFeatureModel( FeatureModel* featureModel )
{
  if ( mFeatureModel == featureModel )
    return;

  disconnect( mFeatureModel, &FeatureModel::currentLayerChanged, this, &AttributeFormModelBase::onLayerChanged );
  disconnect( mFeatureModel, &FeatureModel::featureChanged, this, &AttributeFormModelBase::onFeatureChanged );


  mFeatureModel = featureModel;

  connect( mFeatureModel, &FeatureModel::currentLayerChanged, this, &AttributeFormModelBase::onLayerChanged );
  connect( mFeatureModel, &FeatureModel::featureChanged, this, &AttributeFormModelBase::onFeatureChanged );

  emit featureModelChanged();
}

void AttributeFormModelBase::onLayerChanged()
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

    invisibleRootItem()->setColumnCount( 1 );
    if ( mHasTabs )
    {
      Q_FOREACH( QgsAttributeEditorElement* element, root->children() )
      {
        if ( element->type() == QgsAttributeEditorElement::AeTypeContainer )
        {
          QgsAttributeEditorContainer* container = static_cast<QgsAttributeEditorContainer*>( element );

          QStandardItem* item = new QStandardItem();
          item->setData( element->name(), AttributeFormModel::Name );
          item->setData( "container", AttributeFormModel::ElementType );
          item->setData( true, AttributeFormModel::CurrentlyVisible );
          invisibleRootItem()->appendRow( item );

          if ( container->visibilityExpression().enabled() )
          {
            mVisibilityExpressions.append( qMakePair( container->visibilityExpression().data(), QVector<QStandardItem*>() << item ) );
          }

          QVector<QStandardItem*> dummy;
          flatten( container, item, QString(), dummy );
        }
      }
    }
    else
    {
      QVector<QStandardItem*> dummy;
      flatten( invisibleRootContainer(), invisibleRootItem(), QString(), dummy );
    }

    mExpressionContext = mLayer->createExpressionContext();
  }
}

void AttributeFormModelBase::onFeatureChanged()
{
  for ( int i = 0 ; i < invisibleRootItem()->rowCount(); ++i )
  {
    updateAttributeValue( invisibleRootItem()->child( i ) );
  }

  updateVisibility();
}

QgsAttributeEditorContainer* AttributeFormModelBase::generateRootContainer() const
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

QgsAttributeEditorContainer* AttributeFormModelBase::invisibleRootContainer() const
{
  return mTemporaryContainer ? mTemporaryContainer : mLayer->editFormConfig().invisibleRootContainer();
}

void AttributeFormModelBase::updateAttributeValue( QStandardItem* item )
{
  if ( item->data( AttributeFormModel::ElementType ) == "field" )
  {
    item->setData( mFeatureModel->feature().attribute( item->data( AttributeFormModel::FieldIndex ).toInt() ), AttributeFormModel::AttributeValue );
  }
  else
    for ( int i = 0; i < item->rowCount(); ++i )
      updateAttributeValue( item->child( i ) );
}

void AttributeFormModelBase::flatten( QgsAttributeEditorContainer* container, QStandardItem* parent, const QString& visibilityExpressions, QVector<QStandardItem*>& items )
{
  QString visibilityExpression = visibilityExpressions;

  Q_FOREACH( QgsAttributeEditorElement* element, container->children() )
  {
    switch ( element->type() )
    {
      case QgsAttributeEditorElement::AeTypeContainer:
      {
        QgsAttributeEditorContainer* container = static_cast<QgsAttributeEditorContainer*>( element );
        if ( container->visibilityExpression().enabled() )
        {
          if ( visibilityExpression.isNull() )
            visibilityExpression = container->visibilityExpression().data().expression();
          else
            visibilityExpression += " AND " + container->visibilityExpression().data().expression();
        }

        QVector<QStandardItem*> newItems;
        flatten( container, parent, visibilityExpression, newItems );
        if ( !visibilityExpression.isEmpty() )
          mVisibilityExpressions.append( qMakePair( QgsExpression( visibilityExpression ), newItems ) );
        break;
      }

      case QgsAttributeEditorElement::AeTypeField:
      {
        QgsAttributeEditorField* editorField = static_cast<QgsAttributeEditorField*>( element );
        if ( editorField->idx() == -1 )
          continue;

        QStandardItem* item = new QStandardItem();

        item->setData( mLayer->attributeDisplayName( editorField->idx() ), AttributeFormModel::Name );
        item->setData( mFeatureModel->feature().attribute( editorField->idx() ), AttributeFormModel::AttributeValue );
        item->setData( mLayer->editFormConfig().readOnly( editorField->idx() ), AttributeFormModel::AttributeEditable );
        item->setData( mLayer->editFormConfig().widgetType( editorField->name() ), AttributeFormModel::EditorWidget );
        item->setData( mLayer->editFormConfig().widgetConfig( editorField->name() ), AttributeFormModel::EditorWidgetConfig );
        item->setData( mFeatureModel->rememberedAttributes().at( editorField->idx() ) ? Qt::Checked : Qt::Unchecked, AttributeFormModel::RememberValue );
        item->setData( mLayer->fields().at( editorField->idx() ), AttributeFormModel::Field );
        item->setData( "field", AttributeFormModel::ElementType );
        item->setData( editorField->idx(), AttributeFormModel::FieldIndex );
        item->setData( container->isGroupBox() ? container->name() : QString(), AttributeFormModel::Group );
        item->setData( true, AttributeFormModel::CurrentlyVisible );

        items.append( item );

        parent->appendRow( item );
        break;
      }

      case QgsAttributeEditorElement::AeTypeRelation:
        // todo
        break;

      case QgsAttributeEditorElement::AeTypeInvalid:
        // todo
        break;
    }
  }
}

void AttributeFormModelBase::updateVisibility( int fieldIndex )
{
  QgsFields fields = mFeatureModel->feature().fields();
  mExpressionContext.setFields( fields );
  mExpressionContext.setFeature( mFeatureModel->feature() );

  Q_FOREACH( const VisibilityExpression& it, mVisibilityExpressions )
  {
    if ( fieldIndex == -1 || it.first.referencedAttributeIndexes( fields ).contains( fieldIndex ) )
    {
      QgsExpression exp = it.first;
      exp.prepare( &mExpressionContext );

      bool visible = exp.evaluate( &mExpressionContext ).toInt();
      Q_FOREACH( QStandardItem* item, it.second )
      {
        if ( item->data( AttributeFormModel::CurrentlyVisible ).toBool() != visible )
        {
          item->setData( visible, AttributeFormModel::CurrentlyVisible );
        }
      }
    }
  }
}

bool AttributeFormModelBase::hasTabs() const
{
  return mHasTabs;
}

void AttributeFormModelBase::setHasTabs( bool hasTabs )
{
  if ( hasTabs == mHasTabs )
    return;

  mHasTabs = hasTabs;
  emit hasTabsChanged();
}

void AttributeFormModelBase::save()
{
  mFeatureModel->save();
}

void AttributeFormModelBase::create()
{
  mFeatureModel->create();
}
