

# File qffeaturelistmodelselection.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qffeaturelistmodelselection.cpp**](qffeaturelistmodelselection_8cpp.md)

[Go to the documentation of this file](qffeaturelistmodelselection_8cpp.md)


```C++
/***************************************************************************
                            qffeaturelistmodelselection.cpp
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

#include "qffeaturelistmodelselection.h"

#include <qgsvectorlayer.h>

QfFeatureListModelSelection::QfFeatureListModelSelection( QObject *parent )
  : QObject( parent )
{
}

int QfFeatureListModelSelection::focusedItem() const
{
  return mFocusedItem;
}

void QfFeatureListModelSelection::setFocusedItem( int item )
{
  if ( mFocusedItem == item )
    return;

  mFocusedItem = item;
  emit focusedItemChanged();
}

void QfFeatureListModelSelection::toggleSelectedItem( int item )
{
  mModel->toggleSelectedItem( item );
  emit selectedFeaturesChanged();
}

void QfFeatureListModelSelection::clear()
{
  mFocusedItem = -1;
  emit focusedItemChanged();
}

QfMultiFeatureListModel *QfFeatureListModelSelection::model() const
{
  return mModel;
}

void QfFeatureListModelSelection::setModel( QfMultiFeatureListModel *model )
{
  if ( mModel != model )
  {
    mFocusedItem = -1;
    mModel = model;
    emit modelChanged();
  }
}

QgsVectorLayer *QfFeatureListModelSelection::focusedLayer() const
{
  if ( mFocusedItem > -1 )
  {
    return mModel->data( mModel->index( mFocusedItem, 0 ), QfMultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
  }
  return nullptr;
}

QgsFeature QfFeatureListModelSelection::focusedFeature() const
{
  if ( mFocusedItem > -1 )
  {
    QgsFeature feature = mModel->data( mModel->index( mFocusedItem, 0 ), QfMultiFeatureListModel::FeatureRole ).value<QgsFeature>();
    return feature;
  }
  return QgsFeature();
}

QgsGeometry QfFeatureListModelSelection::focusedGeometry() const
{
  return focusedFeature().geometry();
}
```


