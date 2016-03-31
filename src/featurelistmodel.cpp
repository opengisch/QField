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

  qDeleteAll( mFeatures );
  mFeatures.clear();

  disconnect( this, SLOT( layerDeleted() ) );

  Q_FOREACH( const QgsMapToolIdentify::IdentifyResult& res, results )
  {
    Feature* f = new Feature( res.mFeature, qobject_cast<QgsVectorLayer*>( res.mLayer ) );
    mFeatures.append( f );
    connect( f->layer(), SIGNAL( layerDeleted() ), this, SLOT( layerDeleted() ), Qt::UniqueConnection );
    connect( f->layer(), SIGNAL( featureDeleted( QgsFeatureId ) ), this, SLOT( featureDeleted( QgsFeatureId ) ), Qt::UniqueConnection );
  }

  endResetModel();
}

void FeatureListModel::setFeatures( const QMap<QgsVectorLayer*, QgsFeatureRequest> requests )
{
  beginResetModel();

  qDeleteAll( mFeatures );
  mFeatures.clear();

  QMap<QgsVectorLayer*, QgsFeatureRequest>::ConstIterator it;
  for ( it = requests.constBegin(); it != requests.constEnd(); it++ )
  {
    QgsFeature feat;
    QgsFeatureIterator fit = it.key()->getFeatures( it.value() );
    while ( fit.nextFeature( feat ) )
    {
      Feature* f = new Feature( feat, it.key() );
      mFeatures.append( f );
      connect( f->layer(), SIGNAL( layerDeleted() ), this, SLOT( layerDeleted() ), Qt::UniqueConnection );
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
  roleNames[DeleteFeatureRole] = "deleteFeatureCapability";

  return roleNames;
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex& parent ) const
{
  Q_UNUSED( parent )

  if ( row < 0 || row >= mFeatures.size() || column != 0 )
    return QModelIndex();

  return createIndex( row, column, mFeatures.at( row ) );
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
  Feature* feature = toFeature( index );
  if ( !feature )
    return QVariant();

  switch( role )
  {
    case FeatureIdRole:
      return feature->id();

    case FeatureRole:
      return QVariant::fromValue<Feature>( *feature );

    case Qt::DisplayRole:
      return feature->displayText();

    case LayerNameRole:
      return feature->layer()->name();

    case DeleteFeatureRole:
      bool a = !feature->layer()->readOnly() && ( feature->layer()->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures );
      return a;
  }

  return QVariant();
}

bool FeatureListModel::removeRows( int row, int count, const QModelIndex& parent = QModelIndex() )
{
  if ( !count )
    return true;

  int i = 0;
  QMutableListIterator<Feature*> it( mFeatures );
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
    delete ( it.value() );
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

void FeatureListModel::layerDeleted()
{
  QgsVectorLayer* l = qobject_cast<QgsVectorLayer*>( sender() );
  Q_ASSERT( l );

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
  Q_FOREACH( Feature* f, mFeatures )
  {
    if ( f->layer() == l )
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
  Q_FOREACH( Feature* f, mFeatures )
  {
    if ( f->layer() == l && f->id() == fid )
    {
      removeRows( i, 1 );
      break;
    }
    ++i;
  }
}
