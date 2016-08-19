/***************************************************************************
                            featurelistmodel.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurelistmodel.h"

#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>

#include <QDebug>

FeatureListModel::FeatureListModel( QObject *parent )
  :  QAbstractItemModel( parent )
{
  connect( this, SIGNAL( modelReset() ), this, SIGNAL( countChanged() ) );
}

FeatureListModel::FeatureListModel( QList<QgsMapToolIdentify::IdentifyResult> features, QObject* parent )
  : QAbstractItemModel( parent )
{
  setFeatures( features );
}

void FeatureListModel::setFeatures( const QList<QgsMapToolIdentify::IdentifyResult>& results )
{
  beginResetModel();

  mFeatures.clear();

  disconnect( this, SLOT( layerDeleted(QObject*)) );

  Q_FOREACH( const QgsMapToolIdentify::IdentifyResult& res, results )
  {
    QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( res.mLayer );
    mFeatures.append( QPair< QgsVectorLayer*, QgsFeature >( layer, res.mFeature ) );
    connect( layer, SIGNAL( destroyed(QObject*) ), this, SLOT( layerDeleted( QObject* ) ), Qt::UniqueConnection );
    connect( layer, SIGNAL( featureDeleted( QgsFeatureId ) ), this, SLOT( featureDeleted( QgsFeatureId ) ), Qt::UniqueConnection );
    connect( layer, SIGNAL( attributeValueChanged( QgsFeatureId,int,QVariant ) ), this, SLOT( attributeValueChanged( QgsFeatureId,int,QVariant ) ), Qt::UniqueConnection );
  }

  endResetModel();
}

void FeatureListModel::setFeatures( const QMap<QgsVectorLayer*, QgsFeatureRequest> requests )
{
  beginResetModel();

  mFeatures.clear();

  QMap<QgsVectorLayer*, QgsFeatureRequest>::ConstIterator it;
  for ( it = requests.constBegin(); it != requests.constEnd(); it++ )
  {
    QgsFeature feat;
    QgsFeatureIterator fit = it.key()->getFeatures( it.value() );
    while ( fit.nextFeature( feat ) )
    {
      mFeatures.append( QPair< QgsVectorLayer*, QgsFeature >( it.key(), feat ) );
      connect( it.key(), SIGNAL( layerDeleted() ), this, SLOT( layerDeleted() ), Qt::UniqueConnection );
    }
  }

  endResetModel();
}

QHash<int, QByteArray> FeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roleNames;

  roleNames[Qt::DisplayRole] = "display";
  roleNames[FeatureIdRole] = "featureId";
  roleNames[FeatureRole] = "feature";
  roleNames[LayerNameRole] = "layerName";
  roleNames[LayerNameRole] = "layer";
  roleNames[DeleteFeatureRole] = "deleteFeatureCapability";

  return roleNames;
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex& parent ) const
{
  Q_UNUSED( parent )

  if ( row < 0 || row >= mFeatures.size() || column != 0 )
    return QModelIndex();

  return createIndex( row, column, const_cast<QPair< QgsVectorLayer*, QgsFeature >*>( &mFeatures.at( row ) ) );
}

QModelIndex FeatureListModel::parent( const QModelIndex& child ) const
{
  Q_UNUSED( child );
  return QModelIndex();
}

int FeatureListModel::rowCount( const QModelIndex& parent ) const
{
  if ( parent.isValid() )
    return 0;
  else
    return mFeatures.count();
}

int FeatureListModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant FeatureListModel::data( const QModelIndex& index, int role ) const
{
  QPair< QgsVectorLayer*, QgsFeature >* feature = toFeature( index );
  if ( !feature )
    return QVariant();

  switch( role )
  {
    case FeatureIdRole:
      return feature->second.id();

    case FeatureRole:
      return feature->second;

    case Qt::DisplayRole:
    {
      QgsExpressionContext context = QgsExpressionContext()
          << QgsExpressionContextUtils::globalScope()
          << QgsExpressionContextUtils::projectScope()
          << QgsExpressionContextUtils::layerScope( feature->first );
      context.setFeature( feature->second );
      return QgsExpression( feature->first->displayExpression() ).evaluate( &context ).toString();
    }

    case LayerNameRole:
      return feature->first->name();

    case LayerRole:
      return QVariant::fromValue<QgsVectorLayer*>( feature->first );

    case DeleteFeatureRole:
      bool a = !feature->first->readOnly() && ( feature->first->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures );
      return a;
  }

  return QVariant();
}

bool FeatureListModel::removeRows( int row, int count, const QModelIndex& parent = QModelIndex() )
{
  if ( !count )
    return true;

  int i = 0;
  QMutableListIterator<QPair< QgsVectorLayer*, QgsFeature >> it( mFeatures );
  while ( i < row )
  {
    it.next();
    i++;
  }

  int last = row + count - 1;

  beginRemoveRows( parent, row, last );
  while ( i <= last )
  {
    it.next();
    it.remove();
    i++;
  }
  endRemoveRows();

  return true;
}

int FeatureListModel::count() const
{
  return mFeatures.size();
}

void FeatureListModel::layerDeleted( QObject* object )
{
  int firstRowToRemove = -1;
  int count = 0;
  int currentRow = 0;

  /*
   * Features on the same layer are always subsequent.
   * We therefore can search for the first feature and
   * count all subsequent ones.
   * Once there is a feature of a different layer found
   * we can stop searching.
   */
  for( auto it = mFeatures.constBegin(); it != mFeatures.constEnd(); it++ )
  {
    if ( it->first == object )
    {
      if ( firstRowToRemove == -1 )
        firstRowToRemove = currentRow;

      count++;
    }
    else if ( firstRowToRemove != -1 )
    {
      break;
    }
    currentRow++;
  }

  removeRows( firstRowToRemove, count );
}

void FeatureListModel::featureDeleted( QgsFeatureId fid )
{
  QgsVectorLayer* l = qobject_cast<QgsVectorLayer*>( sender() );
  Q_ASSERT( l );

  int i = 0;
  for( auto it = mFeatures.constBegin(); it != mFeatures.constEnd(); it++ )
  {
    if ( it->first == l && it->second.id() == fid )
    {
      removeRows( i, 1 );
      break;
    }
    ++i;
  }
}

void FeatureListModel::attributeValueChanged( QgsFeatureId fid, int idx, const QVariant& value )
{
  QgsVectorLayer* l = qobject_cast<QgsVectorLayer*>( sender() );
  Q_ASSERT( l );

  int i = 0;
  for( auto it = mFeatures.begin(); it != mFeatures.end(); it++ )
  {
    if ( it->first == l && it->second.id() == fid )
    {
      it->second.setAttribute( idx, value );
      break;
    }
    ++i;
  }
}
