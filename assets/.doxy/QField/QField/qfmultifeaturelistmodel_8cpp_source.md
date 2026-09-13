

# File qfmultifeaturelistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmultifeaturelistmodel.cpp**](qfmultifeaturelistmodel_8cpp.md)

[Go to the documentation of this file](qfmultifeaturelistmodel_8cpp.md)


```C++
/***************************************************************************
                            qffeaturelistmodel.cpp
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

#include "qfmultifeaturelistmodel.h"

#include <qgscoordinatereferencesystem.h>
#include <qgsexpressioncontextutils.h>
#include <qgsgeometry.h>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectordataprovider.h>
#include <qgsvectorlayer.h>

QfMultiFeatureListModel::QfMultiFeatureListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfMultiFeatureListModelBase( this ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &QfMultiFeatureListModelBase::modelReset, this, &QfMultiFeatureListModel::countChanged );
  connect( mSourceModel, &QfMultiFeatureListModelBase::countChanged, this, &QfMultiFeatureListModel::countChanged );
  connect( mSourceModel, &QfMultiFeatureListModelBase::selectedCountChanged, this, &QfMultiFeatureListModel::adjustFilterToSelectedCount );
}

void QfMultiFeatureListModel::setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> &requests )
{
  mSourceModel->setFeatures( requests );
}

void QfMultiFeatureListModel::setFeatures( QgsVectorLayer *vl, const QString &filter, const QgsRectangle &extent )
{
  QgsFeatureRequest request;
  if ( !filter.isEmpty() )
  {
    request.setFilterExpression( filter );
  }
  if ( !extent.isEmpty() )
  {
    QgsRectangle filterExtent = extent;
    if ( QgsProject::instance()->crs() != vl->crs() )
    {
      QgsCoordinateTransform transform( QgsProject::instance()->crs(), vl->crs(), QgsProject::instance()->transformContext() );
      try
      {
        filterExtent = transform.transform( extent );
      }
      catch ( const QgsException &e )
      {
        Q_UNUSED( e )
        return;
      }
      catch ( ... )
      {
        // catch any other errors
        return;
      }
    }
    request.setFilterRect( filterExtent );
  }
  QMap<QgsVectorLayer *, QgsFeatureRequest> requests( { { vl, request } } );
  mSourceModel->setFeatures( requests );
}

void QfMultiFeatureListModel::appendFeatures( const QList<QfIdentifyTool::IdentifyResult> &results )
{
  mSourceModel->appendFeatures( results );
}

void QfMultiFeatureListModel::clear( const bool keepSelected )
{
  if ( !keepSelected )
  {
    mFilterLayer = nullptr;
  }
  mSourceModel->clear( keepSelected );
}

void QfMultiFeatureListModel::clearSelection()
{
  mFilterLayer = nullptr;
  mSourceModel->clearSelection();
}

int QfMultiFeatureListModel::count() const
{
  return mSourceModel->count();
}

int QfMultiFeatureListModel::selectedCount() const
{
  return mSourceModel->selectedCount();
}

bool QfMultiFeatureListModel::canEditAttributesSelection() const
{
  return mSourceModel->canEditAttributesSelection();
}

bool QfMultiFeatureListModel::canMergeSelection() const
{
  return mSourceModel->canMergeSelection();
}

bool QfMultiFeatureListModel::canDeleteSelection() const
{
  return mSourceModel->canDeleteSelection();
}

bool QfMultiFeatureListModel::canDuplicateSelection() const
{
  return mSourceModel->canDuplicateSelection();
}

bool QfMultiFeatureListModel::canMoveSelection() const
{
  return mSourceModel->canMoveSelection();
}

bool QfMultiFeatureListModel::canRotateSelection() const
{
  return mSourceModel->canRotateSelection();
}

bool QfMultiFeatureListModel::canProcessSelection() const
{
  return mSourceModel->canProcessSelection();
}

bool QfMultiFeatureListModel::mergeSelection()
{
  return mSourceModel->mergeSelection();
}

bool QfMultiFeatureListModel::deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid )
{
  return mSourceModel->deleteFeature( layer, fid );
}

bool QfMultiFeatureListModel::deleteSelection()
{
  return mSourceModel->deleteSelection();
}

bool QfMultiFeatureListModel::duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature )
{
  return mSourceModel->duplicateFeature( layer, feature );
}

bool QfMultiFeatureListModel::duplicateSelection()
{
  return mSourceModel->duplicateSelection();
}

bool QfMultiFeatureListModel::moveSelection( const double x, const double y, const QgsPoint &destinationPoint )
{
  return mSourceModel->moveSelection( x, y, destinationPoint );
}

bool QfMultiFeatureListModel::rotateSelection( const double angle )
{
  return mSourceModel->rotateSelection( angle );
}

void QfMultiFeatureListModel::toggleSelectedItem( int item )
{
  QModelIndex sourceItem = mapToSource( index( item, 0 ) );
  mSourceModel->toggleSelectedItem( sourceItem.row() );
  if ( mSourceModel->selectedCount() > 0 && mFilterLayer == nullptr )
  {
    beginFilterChange();
    mFilterLayer = mSourceModel->data( sourceItem, QfMultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
    emit selectedLayerChanged();
  }
  else if ( mSourceModel->selectedCount() == 0 && mFilterLayer != nullptr )
  {
    beginFilterChange();
    mFilterLayer = nullptr;
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
    emit selectedLayerChanged();
  }
}

void QfMultiFeatureListModel::adjustFilterToSelectedCount()
{
  if ( mSourceModel->selectedCount() > 0 && mFilterLayer == nullptr )
  {
    beginFilterChange();
    mFilterLayer = mSourceModel->selectedLayer();
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
    emit selectedLayerChanged();
  }
  else if ( mSourceModel->selectedCount() == 0 && mFilterLayer != nullptr )
  {
    beginFilterChange();
    mFilterLayer = nullptr;
    endFilterChange( QSortFilterProxyModel::Direction::Rows );
    emit selectedLayerChanged();
  }
  emit selectedCountChanged();
}

QList<QgsFeature> QfMultiFeatureListModel::selectedFeatures()
{
  return mSourceModel->selectedFeatures();
}

QgsVectorLayer *QfMultiFeatureListModel::selectedLayer()
{
  return mFilterLayer.data();
}

bool QfMultiFeatureListModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( mFilterLayer != nullptr )
  {
    return mFilterLayer == mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QfMultiFeatureListModel::LayerRole ).value<QgsVectorLayer *>();
  }
  return true;
}
```


