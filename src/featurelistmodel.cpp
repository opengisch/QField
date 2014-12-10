/***************************************************************************
                            featurelistmodel.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
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

  QSet<QgsMapLayer*> layers;

  Q_FOREACH( const QgsMapToolIdentify::IdentifyResult& res, results )
  {
    Feature* f = new Feature( res.mFeature, qobject_cast<QgsVectorLayer*>( res.mLayer ) );
    mFeatures.append( f );
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

  return roleNames;
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex& parent ) const
{
  Q_UNUSED( parent )

  if ( row < 0 )
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
  Q_UNUSED( parent );
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
  }

  return QVariant();
}

int FeatureListModel::count() const
{
  return mFeatures.size();
}
