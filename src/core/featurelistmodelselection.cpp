/***************************************************************************
                            featurelistmodelselection.cpp
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

#include "featurelistmodelselection.h"

#include <qgsvectorlayer.h>

FeatureListModelSelection::FeatureListModelSelection( QObject *parent )
  : QObject( parent )
{
}

int FeatureListModelSelection::focusedItem() const
{
  return mFocusedItem;
}

void FeatureListModelSelection::setFocusedItem( int item )
{
  if ( mFocusedItem == item )
    return;

  mFocusedItem = item;
  emit focusedItemChanged();
}

void FeatureListModelSelection::toggleSelectedItem( int item )
{
  mModel->toggleSelectedItem( item );
  emit selectedFeaturesChanged();
}

void FeatureListModelSelection::clear()
{
  mFocusedItem = -1;
  emit focusedItemChanged();
}

MultiFeatureListModel *FeatureListModelSelection::model() const
{
  return mModel;
}

void FeatureListModelSelection::setModel( MultiFeatureListModel *model )
{
  if ( mModel != model )
  {
    mFocusedItem = -1;
    mModel = model;
    emit modelChanged();
  }
}

QgsVectorLayer *FeatureListModelSelection::focusedLayer() const
{
  if ( mFocusedItem > -1 )
  {
    return mModel->data( mModel->index( mFocusedItem, 0 ), MultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
  }
  return nullptr;
}

QgsFeature FeatureListModelSelection::focusedFeature() const
{
  if ( mFocusedItem > -1 )
  {
    QgsFeature feature = mModel->data( mModel->index( mFocusedItem, 0 ), MultiFeatureListModel::FeatureRole ).value<QgsFeature>();
    return feature;
  }
  return QgsFeature();
}

QgsGeometry FeatureListModelSelection::focusedGeometry() const
{
  return focusedFeature().geometry();
}
