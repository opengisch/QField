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
            emit requestJumpToPoint( point, true );
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
            if ( mKeepScale )
            {
              emit requestJumpToPoint( QgsPoint( extent.center() ), true );
            }
            else
            {
              mMapSettings->setExtent( extent, true );
            }
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
