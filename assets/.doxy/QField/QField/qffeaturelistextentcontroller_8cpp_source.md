

# File qffeaturelistextentcontroller.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qffeaturelistextentcontroller.cpp**](qffeaturelistextentcontroller_8cpp.md)

[Go to the documentation of this file](qffeaturelistextentcontroller_8cpp.md)


```C++
/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qffeaturelistextentcontroller.h"
#include "qffeatureutils.h"
#include "qfmultifeaturelistmodel.h"

#include <qgsgeometry.h>
#include <qgsvectorlayer.h>

QfFeatureListExtentController::QfFeatureListExtentController( QObject *parent )
  : QObject( parent )
{
  connect( this, &QfFeatureListExtentController::autoZoomChanged, this, [this]() { zoomToSelected(); } );
  connect( this, &QfFeatureListExtentController::modelChanged, this, &QfFeatureListExtentController::onModelChanged );
  connect( this, &QfFeatureListExtentController::selectionChanged, this, &QfFeatureListExtentController::onModelChanged );
}

QfFeatureListExtentController::~QfFeatureListExtentController()
{
}

QfFeatureListModelSelection *QfFeatureListExtentController::selection() const
{
  return mSelection;
}

QfMultiFeatureListModel *QfFeatureListExtentController::model() const
{
  return mModel;
}

void QfFeatureListExtentController::requestFeatureFormState()
{
  emit featureFormStateRequested();
}

void QfFeatureListExtentController::zoomToSelected( bool skipIfIntersects ) const
{
  if ( mModel && mSelection && mSelection->focusedItem() > -1 && mMapSettings && mSelection->focusedLayer() )
  {
    QgsVectorLayer *layer = mSelection->focusedLayer();
    const QgsFeature feature = mSelection->focusedFeature();

    if ( layer->geometryType() != Qgis::GeometryType::Unknown && layer->geometryType() != Qgis::GeometryType::Null )
    {
      if ( feature.geometry().type() == Qgis::GeometryType::Point && feature.geometry().constGet()->partCount() == 1 )
      {
        try
        {
          const QgsCoordinateTransform ct( layer->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
          const QgsPoint point( ct.transform( feature.geometry().asPoint() ) );
          if ( !point.isEmpty() )
          {
            emit requestJumpToPoint( point, -1.0, true );
          }
        }
        catch ( const QgsException &e )
        {
          Q_UNUSED( e )
          return;
        }
      }
      else
      {
        try
        {
          const QgsRectangle extent = QfFeatureUtils::extent( mMapSettings, layer, feature );
          if ( !extent.isNull() && ( !skipIfIntersects || !mMapSettings->extent().intersects( extent ) ) )
          {
            const double scale = mKeepScale ? -1 : mMapSettings->computeScaleForExtent( extent, true );
            emit requestJumpToPoint( QgsPoint( extent.center() ), scale, true );
          }
        }
        catch ( const QgsException &e )
        {
          Q_UNUSED( e )
          return;
        }
      }
    }
  }
}

void QfFeatureListExtentController::zoomToAllFeatures() const
{
  if ( !mModel || !mMapSettings )
  {
    return;
  }

  const int rowCount = mModel->rowCount( QModelIndex() );
  if ( rowCount == 0 )
  {
    return;
  }

  bool isSinglePointGeometry = rowCount == 1; // Further assessed down below
  QgsRectangle combinedExtent;
  for ( int i = 0; i < mModel->rowCount( QModelIndex() ); ++i )
  {
    const QModelIndex index = mModel->index( i, 0 );
    QgsVectorLayer *layer = qvariant_cast<QgsVectorLayer *>( mModel->data( index, QfMultiFeatureListModel::LayerRole ) );
    if ( !layer || layer->geometryType() == Qgis::GeometryType::Unknown || layer->geometryType() == Qgis::GeometryType::Null )
    {
      continue;
    }

    try
    {
      const QgsFeature feature = mModel->data( index, QfMultiFeatureListModel::FeatureRole ).value<QgsFeature>();
      const QgsGeometry geom( feature.geometry() );
      if ( geom.isNull() )
      {
        continue;
      }

      if ( geom.type() != Qgis::GeometryType::Point || geom.constGet()->partCount() > 1 )
      {
        isSinglePointGeometry = false;
      }

      const QgsRectangle extent = QfFeatureUtils::extent( mMapSettings, layer, feature, !isSinglePointGeometry );
      if ( extent.isNull() )
      {
        continue;
      }

      if ( combinedExtent.isNull() )
      {
        combinedExtent = extent;
      }
      else
      {
        combinedExtent.combineExtentWith( extent );
      }
    }
    catch ( const QgsException &e )
    {
      Q_UNUSED( e )
    }
  }

  if ( !combinedExtent.isNull() )
  {
    // Add buffer to ensure features are fully visible within the view
    const double buffer = std::max( combinedExtent.width(), combinedExtent.height() ) * 0.5;
    combinedExtent = combinedExtent.buffered( buffer );

    const double scale = ( mKeepScale || isSinglePointGeometry ) ? -1 : mMapSettings->computeScaleForExtent( combinedExtent, true );
    emit requestJumpToPoint( QgsPoint( combinedExtent.center() ), scale, true );
  }
}

QgsPoint QfFeatureListExtentController::getCentroidFromSelected() const
{
  if ( mModel && mSelection && mSelection->focusedItem() > -1 && mMapSettings )
  {
    const QgsFeature feat = mSelection->focusedFeature();
    const QgsVectorLayer *layer = mSelection->focusedLayer();

    if ( layer && layer->geometryType() != Qgis::GeometryType::Unknown && layer->geometryType() != Qgis::GeometryType::Null )
    {
      QgsGeometry geom = feat.geometry();

      const QgsCoordinateTransform transf( layer->crs(), mMapSettings->destinationCrs(), mMapSettings->mapSettings().transformContext() );
      try
      {
        geom.transform( transf );
        return QgsPoint( geom.centroid().asPoint() );
      }
      catch ( const QgsException &e )
      {
        Q_UNUSED( e )
      }
    }
  }
  return QgsPoint();
}

void QfFeatureListExtentController::onModelChanged()
{
  if ( mModel && mSelection )
  {
    connect( mSelection, &QfFeatureListModelSelection::focusedItemChanged, this, &QfFeatureListExtentController::onCurrentSelectionChanged );
  }
}

void QfFeatureListExtentController::onCurrentSelectionChanged()
{
  if ( mAutoZoom )
  {
    zoomToSelected();
  }
}
```


