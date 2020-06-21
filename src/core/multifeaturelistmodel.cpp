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

#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>
#include <qgsproject.h>
#include <qgsgeometry.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsexpressioncontextutils.h>
#include <qgsrelationmanager.h>
#include <qgsmessagelog.h>

#include "multifeaturelistmodel.h"

#include <QDebug>

MultiFeatureListModel::MultiFeatureListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new MultiFeatureListModelBase( this ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &MultiFeatureListModelBase::modelReset, this, &MultiFeatureListModel::countChanged );
  connect( mSourceModel, &MultiFeatureListModelBase::countChanged, this, &MultiFeatureListModel::countChanged );
  connect( mSourceModel, &MultiFeatureListModelBase::selectedCountChanged, this, &MultiFeatureListModel::selectedCountChanged );
}


void MultiFeatureListModel::setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> requests )
{
  mSourceModel->setFeatures( requests );
}

void MultiFeatureListModel::appendFeatures( const QList<IdentifyTool::IdentifyResult> &results )
{
  mSourceModel->appendFeatures( results );
}

void MultiFeatureListModel::clear( const bool keepSelected )
{
  if ( !keepSelected )
  {
    mFilterLayer = nullptr;
  }
  mSourceModel->clear( keepSelected );
}

int MultiFeatureListModel::count() const
{
  return mSourceModel->count();
}

int MultiFeatureListModel::selectedCount() const
{
  return mSourceModel->selectedCount();
}

bool MultiFeatureListModel::deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid )
{
  return mSourceModel->deleteFeature( layer, fid );
}

void MultiFeatureListModel::toggleSelectedItem( int item )
{
  QModelIndex sourceItem = mapToSource( index( item, 0 ) );
  mSourceModel->toggleSelectedItem( sourceItem.row() );
  if ( mSourceModel->selectedCount() > 0 && mFilterLayer == nullptr )
  {
    mFilterLayer =  mSourceModel->data( sourceItem, MultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
    invalidateFilter();
  }
  else if ( mSourceModel->selectedCount() == 0 && mFilterLayer != nullptr )
  {
    mFilterLayer = nullptr;
    invalidateFilter();
  }
}

QList<QgsFeature> MultiFeatureListModel::selectedFeatures()
{
  return mSourceModel->selectedFeatures();
}

bool MultiFeatureListModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( mFilterLayer != nullptr )
  {
    return mFilterLayer == mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), MultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
  }
  return true;
}
