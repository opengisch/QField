

# File featurelistextentcontroller.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurelistextentcontroller.cpp**](featurelistextentcontroller_8cpp.md)

[Go to the documentation of this file](featurelistextentcontroller_8cpp.md)


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

#include "featurelistextentcontroller.h"
#include "featureutils.h"
#include "multifeaturelistmodel.h"

#include <qgsgeometry.h>
#include <qgsvectorlayer.h>

FeatureListExtentController::FeatureListExtentController( QObject *parent )
  : QObject( parent )
{
  connect( this, &FeatureListExtentController::autoZoomChanged, this, [this]() { zoomToSelected(); } );
  connect( this, &FeatureListExtentController::modelChanged, this, &FeatureListExtentController::onModelChanged );
  connect( this, &FeatureListExtentController::selectionChanged, this, &FeatureListExtentController::onModelChanged );
}

FeatureListExtentController::~FeatureListExtentController()
{
}

FeatureListModelSelection *FeatureListExtentController::selection() const
{
  return mSelection;
}

MultiFeatureListModel *FeatureListExtentController::model() const
{
  return mModel;
}

void FeatureListExtentController::requestFeatureFormState()
{
  emit featureFormStateRequested();
}

void FeatureListExtentController::zoomToSelected( bool skipIfIntersects ) const
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
          const QgsRectangle extent = FeatureUtils::extent( mMapSettings, layer, feature );
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

void FeatureListExtentController::zoomToAllFeatures() const
{
  if ( !mModel || !mMapSettings || mModel->rowCount( QModelIndex() ) == 0 )
    return;

  QgsRectangle combinedExtent;
  bool hasNonPointGeometry = false;

  for ( int i = 0; i < mModel->rowCount( QModelIndex() ); ++i )
  {
    const QModelIndex index = mModel->index( i, 0 );
    QgsVectorLayer *layer = qvariant_cast<QgsVectorLayer *>( mModel->data( index, MultiFeatureListModel::LayerRole ) );
    const QgsFeature feature = mModel->data( index, MultiFeatureListModel::FeatureRole ).value<QgsFeature>();

    if ( !layer || layer->geometryType() == Qgis::GeometryType::Unknown || layer->geometryType() == Qgis::GeometryType::Null )
    {
      continue;
    }

    try
    {
      const QgsGeometry geom( feature.geometry() );
      if ( geom.isNull() )
      {
        continue;
      }

      if ( geom.type() != Qgis::GeometryType::Point || geom.constGet()->partCount() > 1 )
      {
        hasNonPointGeometry = true;
      }

      const QgsRectangle extent = FeatureUtils::extent( mMapSettings, layer, feature );
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

    const double scale = ( mKeepScale || !hasNonPointGeometry ) ? -1 : mMapSettings->computeScaleForExtent( combinedExtent, true );
    emit requestJumpToPoint( QgsPoint( combinedExtent.center() ), scale, true );
  }
}

QgsPoint FeatureListExtentController::getCentroidFromSelected() const
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

void FeatureListExtentController::onModelChanged()
{
  if ( mModel && mSelection )
  {
    connect( mSelection, &FeatureListModelSelection::focusedItemChanged, this, &FeatureListExtentController::onCurrentSelectionChanged );
  }
}

void FeatureListExtentController::onCurrentSelectionChanged()
{
  if ( mAutoZoom )
  {
    zoomToSelected();
  }
}
```


