/***************************************************************************
  referencingfeaturelistmodel.cpp - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "referencingfeaturelistmodel.h"

ReferencingFeatureListModel::ReferencingFeatureListModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

QHash<int, QByteArray> ReferencingFeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayString] = "displayString";
  roles[ReferencingFeature] = "referencingFeature";

  return roles;
}

QModelIndex ReferencingFeatureListModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex ReferencingFeatureListModel::parent(const QModelIndex &index) const
{
  Q_UNUSED( index )

  return QModelIndex();
}

int ReferencingFeatureListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent )
  return mEntries.size();
}

int ReferencingFeatureListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant ReferencingFeatureListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == DisplayString )
    return mEntries.value( index.row() ).displayString;
  if ( role == ReferencingFeature )
    return mRelation.referencingLayer()->getFeature( mEntries.value( index.row() ).referencingFeatureId );
  return QVariant();
}

void ReferencingFeatureListModel::setFeature(const QgsFeature &feature)
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  reload();
}

QgsFeature ReferencingFeatureListModel::feature() const
{
  return mFeature;
}

void ReferencingFeatureListModel::setRelation(const QgsRelation &relation)
{
  mRelation = relation;
  reload();
}

QgsRelation ReferencingFeatureListModel::relation() const
{
  return mRelation;
}

void ReferencingFeatureListModel::reload()
{
  if( !mRelation.isValid() || !mFeature.isValid() )
    return;
  mEntries.clear();
  QgsFeatureIterator relatedFeaturesIt = mRelation.getRelatedFeatures( mFeature );
  QgsExpressionContext context = mRelation.referencingLayer()->createExpressionContext();
  QgsExpression expression( mRelation.referencingLayer()->displayExpression() );

  beginResetModel();
  QgsFeature childFeature;
  while ( relatedFeaturesIt.nextFeature( childFeature ) )
  {
   context.setFeature( childFeature );
   mEntries.append( Entry( expression.evaluate( &context ).toString(), childFeature.id() ) );
  }
  endResetModel();
}

void ReferencingFeatureListModel::deleteFeature( QgsFeatureId referencingFeatureId )
{
  mRelation.referencingLayer()->startEditing();
  mRelation.referencingLayer()->deleteFeature( referencingFeatureId );
  mRelation.referencingLayer()->commitChanges();
  reload();
}
