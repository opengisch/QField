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

#include <qgsvectorlayer.h>
#include <qgsgeometry.h>

FeatureListExtentController::FeatureListExtentController( QObject* parent )
  : QObject( parent )
  , mModel( 0 )
  , mSelection( 0 )
  , mMapSettings( 0 )
  , mAutoZoom( false )
{
  connect( this, &FeatureListExtentController::autoZoomChanged, this, &FeatureListExtentController::zoomToSelected );
  connect( this, &FeatureListExtentController::modelChanged, this, &FeatureListExtentController::onModelChanged );
  connect( this, &FeatureListExtentController::selectionChanged, this, &FeatureListExtentController::onModelChanged );
}

FeatureListExtentController::~FeatureListExtentController()
{
}

void FeatureListExtentController::zoomToSelected() const
{
  if ( mModel && mSelection && mMapSettings )
  {
    QgsFeature feat = mSelection->selectedFeature();
    QgsVectorLayer* layer = mSelection->selectedLayer();

    QgsCoordinateTransform transf( layer->crs(), mMapSettings->destinationCrs(), mMapSettings->mapSettings().transformContext() );
    QgsGeometry geom( feat.geometry() );
    geom.transform( transf );

    if ( geom.type() == QgsWkbTypes::PointGeometry )
    {
      mMapSettings->setCenter( QgsPoint( geom.asPoint() ) );
    }
    else
    {
      QgsRectangle featureExtent = geom.boundingBox();
      QgsRectangle bufferedExtent = featureExtent.buffered( qMax( featureExtent.width(), featureExtent.height() ) );

      mMapSettings->setExtent( bufferedExtent );
    }
  }
}

void FeatureListExtentController::onModelChanged()
{
  if ( mModel && mSelection )
  {
    connect( mSelection, &FeatureListModelSelection::selectionChanged, this, &FeatureListExtentController::onCurrentSelectionChanged );
  }
}

void FeatureListExtentController::onCurrentSelectionChanged()
{
  if ( mAutoZoom )
  {
    zoomToSelected();
  }
}
