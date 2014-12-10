/***************************************************************************
                            featuremodel.cpp
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

#include "featuremodel.h"

#include "feature.h"

FeatureModel::FeatureModel( QObject *parent ) :
  QAbstractListModel( parent )
{
  connect( this, SIGNAL( modelReset() ), this, SIGNAL( featureChanged() ) );
}

void FeatureModel::setFeature( QVariant v )
{
  const Feature& feature = v.value<Feature>();

  if ( feature.layer() != mFeature.layer() || feature.id() != mFeature.id() )
  {
    beginResetModel();
    mFeature = feature;
    endResetModel();
  }
}

QVariant FeatureModel::feature()
{
  return QVariant::fromValue<Feature>( mFeature );
}

QHash<int, QByteArray> FeatureModel::roleNames() const
{

  QHash<int, QByteArray> roles;
  roles[AttributeName]  = "attributeName";
  roles[AttributeValue] = "attributeValue";

  return roles;
}


int FeatureModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  return mFeature.attributes().count();
}

QVariant FeatureModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case AttributeName:
      return mFeature.fields()->at( index.row() ).name();
      break;

    case AttributeValue:
      return mFeature.attribute( index.row() );
      break;

    case AttributeEditable:
      return false;
      break;
  }

  return QVariant();
}
