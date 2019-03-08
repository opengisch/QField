/***************************************************************************
  referencingfeaturelistmodel.cpp - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer Kuhn
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
  roles[FeatureId] = "featureId";

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
  if ( role == FeatureId )
    return mEntries.value( index.row() ).featureId;

  return QVariant();
}

void ReferencingFeatureListModel::setFeatureId(const QgsFeatureId &featureId)
{
  if ( mFeatureId == featureId )
    return;

  mFeatureId = featureId;
  reload();
}

QgsFeatureId ReferencingFeatureListModel::featureId() const
{
  return mFeatureId;
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

AttributeFormModel *ReferencingFeatureListModel::attributeFormModel() const
{
  return mAttributeFormModel;
}

void ReferencingFeatureListModel::setAttributeFormModel( AttributeFormModel *attributeFormModel )
{
  mAttributeFormModel = attributeFormModel;
  connect( mAttributeFormModel, &AttributeFormModel::setRelationFeatureId, this, [this]( QgsFeatureId featureId )
    {
      setFeatureId( featureId );
    }
  );
}

void ReferencingFeatureListModel::reload()
{
  if( !mRelation.isValid() || mFeatureId<0 )
    return;
  mEntries.clear();
  QgsFeatureIterator relatedFeaturesIt = mRelation.getRelatedFeatures( mRelation.referencedLayer()->getFeature( mFeatureId ) );
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

void ReferencingFeatureListModel::deleteFeature( QgsFeatureId fid )
{
  mRelation.referencingLayer()->startEditing();
  mRelation.referencingLayer()->deleteFeature( fid );
  mRelation.referencingLayer()->commitChanges();
  reload();
}
