/***************************************************************************
  featurelistmodel.cpp - FeatureListModel

 ---------------------
 begin                : 1.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "featurelistmodel.h"
#include "qgsvectorlayer.h"

#include <qgsproject.h>
#include <qgsexpressioncontextutils.h>
#include <qgsvaluerelationfieldformatter.h>

FeatureListModel::FeatureListModel( QObject *parent )
  : QAbstractItemModel( parent )
  , mCurrentLayer( nullptr )
{
  mReloadTimer.setInterval( 100 );
  mReloadTimer.setSingleShot( true );
  connect( &mReloadTimer, &QTimer::timeout, this, &FeatureListModel::processReloadLayer );
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex FeatureListModel::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )

  // We live in flat structures here
  return QModelIndex();
}

int FeatureListModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mEntries.size();
}

int FeatureListModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant FeatureListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == Qt::DisplayRole || role == DisplayStringRole )
  {
    return mEntries.value( index.row() ).displayString;
  }
  else if ( role == KeyFieldRole )
  {
    return mEntries.value( index.row() ).key;
  }

  return QVariant();
}

QHash<int, QByteArray> FeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[KeyFieldRole] = "keyFieldValue";
  roles[DisplayStringRole] = "displayString";

  return roles;
}

QgsVectorLayer *FeatureListModel::currentLayer() const
{
  return mCurrentLayer;
}

void FeatureListModel::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  if ( mCurrentLayer == currentLayer )
    return;

  if ( mCurrentLayer )
  {
    disconnect( mCurrentLayer, &QgsVectorLayer::featureAdded, this, &FeatureListModel::onFeatureAdded );
    disconnect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &FeatureListModel::onAttributeValueChanged );
    disconnect( mCurrentLayer, &QgsVectorLayer::featureDeleted, this, &FeatureListModel::onFeatureDeleted );
  }

  mCurrentLayer = currentLayer;

  if ( mCurrentLayer )
  {
    connect( currentLayer, &QgsVectorLayer::featureAdded, this, &FeatureListModel::onFeatureAdded );
    connect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &FeatureListModel::onAttributeValueChanged );
    connect( currentLayer, &QgsVectorLayer::featureDeleted, this, &FeatureListModel::onFeatureDeleted );
  }

  reloadLayer();

  emit currentLayerChanged();
}

QString FeatureListModel::keyField() const
{
  return mKeyField;
}

void FeatureListModel::setKeyField( const QString &keyField )
{
  if ( mKeyField == keyField )
    return;

  mKeyField = keyField;

  reloadLayer();

  emit keyFieldChanged();
}

QString FeatureListModel::displayValueField() const
{
  return mDisplayValueField;
}

void FeatureListModel::setDisplayValueField( const QString &displayValueField )
{
  if ( mDisplayValueField == displayValueField )
    return;

  mDisplayValueField = displayValueField;

  reloadLayer();

  emit displayValueFieldChanged();
}

int FeatureListModel::findKey( const QVariant &key ) const
{
  int idx = 0;
  for ( const Entry &entry : mEntries )
  {
    if ( entry.key == key )
      return idx;

    ++idx;
  }

  if ( mAddNull )
    return 0;

  return -1;
}

void FeatureListModel::onFeatureAdded()
{
  reloadLayer();
}

void FeatureListModel::onAttributeValueChanged( QgsFeatureId, int idx, const QVariant & )
{
  QgsExpressionContext context = mCurrentLayer->createExpressionContext();
  QgsExpression expression( mCurrentLayer->displayExpression() );
  expression.prepare( &context );
  QSet<QString> referencedColumns = expression.referencedColumns();
  referencedColumns << mDisplayValueField;

  if ( referencedColumns.contains( mCurrentLayer->fields().at( idx ).name() ) )
    reloadLayer();
}

void FeatureListModel::onFeatureDeleted()
{
  reloadLayer();
}

QgsFeature FeatureListModel::getFeatureFromKeyValue( const QVariant &value ) const
{
  if ( !mCurrentLayer )
    return QgsFeature();

  QgsFeature feature;
  for ( auto &entry : mEntries )
  {
    if ( entry.key == value )
    {
      feature = mCurrentLayer->getFeature( entry.fid );
    }
  }

  return feature;
}

void FeatureListModel::processReloadLayer()
{
  mEntries.clear();

  if ( !mCurrentLayer )
    return;

  QgsFeatureRequest request;
  QgsExpressionContext context = mCurrentLayer->createExpressionContext();
  QgsExpression expression( mCurrentLayer->displayExpression() );
  expression.prepare( &context );

  QSet<QString> referencedColumns = expression.referencedColumns();

  if ( !keyField().isNull() )
    referencedColumns << mKeyField;

  referencedColumns << mDisplayValueField;

  QgsFields fields = mCurrentLayer->fields();

  request.setSubsetOfAttributes( referencedColumns, fields );

  if ( ! mFilterExpression.isEmpty()
       && ( ! QgsValueRelationFieldFormatter::expressionRequiresFormScope( mFilterExpression )
            || QgsValueRelationFieldFormatter::expressionIsUsable( mFilterExpression, mCurrentFormFeature )
          ) )
  {
    QgsExpression exp( mFilterExpression );
    QgsExpressionContext filterContext = QgsExpressionContext( QgsExpressionContextUtils::globalProjectLayerScopes( mCurrentLayer ) );

    if ( mCurrentFormFeature.isValid( ) && QgsValueRelationFieldFormatter::expressionRequiresFormScope( mFilterExpression ) )
      filterContext.appendScope( QgsExpressionContextUtils::formScope( mCurrentFormFeature ) );

    request.setExpressionContext( filterContext );
    request.setFilterExpression( mFilterExpression );
  }

  int keyIndex = fields.indexOf( mKeyField );
  int displayValueIndex = fields.indexOf( mDisplayValueField );

  QgsFeatureIterator iterator = mCurrentLayer->getFeatures( request );

  QgsFeature feature;

  QList<Entry> entries;

  if ( mAddNull )
    entries.append( Entry( QStringLiteral( "<i>NULL</i>" ), QVariant(), QgsFeatureId() ) );

  while ( iterator.nextFeature( feature ) )
  {
    context.setFeature( feature );
    if ( mDisplayValueField.isEmpty() )
      entries.append( Entry( expression.evaluate( &context ).toString(), feature.attribute( keyIndex ), feature.id() ) );
    else
      entries.append( Entry( feature.attribute( displayValueIndex ).toString(), feature.attribute( keyIndex ), feature.id() ) );
  }

  if ( mOrderByValue )
  {
    std::sort( entries.begin(), entries.end(), []( const Entry & entry1, const Entry & entry2 )
    {
      if ( entry1.key.isNull() )
        return true;

      if ( entry2.key.isNull() )
        return false;

      return entry1.displayString.toLower() < entry2.displayString.toLower();
    } );
  }

  beginResetModel();
  mEntries = entries;
  endResetModel();
}

void FeatureListModel::reloadLayer()
{
  mReloadTimer.start();
}

bool FeatureListModel::addNull() const
{
  return mAddNull;
}

void FeatureListModel::setAddNull( bool addNull )
{
  if ( mAddNull == addNull )
    return;

  mAddNull = addNull;
  reloadLayer();
  emit addNullChanged();
}

bool FeatureListModel::orderByValue() const
{
  return mOrderByValue;
}

void FeatureListModel::setOrderByValue( bool orderByValue )
{
  if ( mOrderByValue == orderByValue )
    return;

  mOrderByValue = orderByValue;
  reloadLayer();
  emit orderByValueChanged();
}

QString FeatureListModel::filterExpression() const
{
  return mFilterExpression;
}

void FeatureListModel::setFilterExpression( const QString &filterExpression )
{
  if ( mFilterExpression == filterExpression )
    return;

  mFilterExpression = filterExpression;
  reloadLayer();
  emit filterExpressionChanged();
}

QgsFeature FeatureListModel::currentFormFeature() const
{
  return mCurrentFormFeature;
}

void FeatureListModel::setCurrentFormFeature( const QgsFeature &feature )
{
  if ( mCurrentFormFeature == feature )
    return;

  mCurrentFormFeature = feature;
  reloadLayer();
  emit currentFormFeatureChanged();
}
