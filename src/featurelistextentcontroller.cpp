/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurelistextentcontroller.h"

#include <qgsgeometry.h>

FeatureListExtentController::FeatureListExtentController( QObject* parent )
  : QObject( parent )
  , mModel( 0 )
  , mSelection( 0 )
  , mMapSettings( 0 )
  , mAutoZoom( false )
{
  connect( this, SIGNAL( autoZoomChanged() ), SLOT( zoomToSelected() ) );
  connect( this, SIGNAL( modelChanged() ), SLOT( onModelChanged() ) );
  connect( this, SIGNAL( selectionChanged() ), SLOT( onModelChanged() ) );
}

FeatureListExtentController::~FeatureListExtentController()
{
}

void FeatureListExtentController::zoomToSelected() const
{
  if ( mModel && mSelection && mMapSettings )
  {
    Feature feat = mSelection->selectedFeature().value<Feature>();
    QgsRectangle featureExtent = feat.qgsFeature().geometry()->boundingBox();
    QgsRectangle bufferedExtent = featureExtent.buffer( qMax( featureExtent.width(), featureExtent.height() ) );

    mMapSettings->setExtent( bufferedExtent );
  }
}

void FeatureListExtentController::onModelChanged()
{
  if ( mModel && mSelection )
  {
    connect( mSelection, SIGNAL( selectionChanged() ), SLOT( onCurrentSelectionChanged() ) );
  }
}

void FeatureListExtentController::onCurrentSelectionChanged()
{
  if ( mAutoZoom )
  {
    zoomToSelected();
  }
}
