/***************************************************************************
  geofencer.h - Geofencer

 ---------------------
 begin                : 27.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "geofencer.h"

#include <qgsgeometryengine.h>
#include <qgsproject.h>

Geofencer::Geofencer( QObject *parent )
  : QObject( parent )
{
}

Geofencer::~Geofencer()
{
}

void Geofencer::fetchAreas()
{
  clearAreas();

  if ( !mAreasLayer )
  {
    return;
  }

  QgsFeatureRequest request;
  request.setNoAttributes();
  if ( mPositionCrs.isValid() )
  {
    request.setDestinationCrs( mPositionCrs, QgsProject::instance()->transformContext() );
  }

  QgsFeatureIterator it = mAreasLayer->getFeatures( request );
  QgsFeature feature;
  while ( it.nextFeature( feature ) )
  {
    if ( !feature.geometry().isEmpty() )
    {
      mFetchedAreaGeometries[feature.id()] = QgsGeometry::createGeometryEngine( feature.geometry().get() );
    }
  }
}

void Geofencer::clearAreas()
{
  qDeleteAll( mFetchedAreaGeometries );
}

void Geofencer::checkWithin()
{
  bool isWithin = false;

  if ( mActive && !mPosition.isEmpty() && mPositionCrs.isValid() && mAreasLayer )
  {
    for ( auto [key, value] : mFetchedAreaGeometries.asKeyValueRange() )
    {
      if ( value->within( &mPosition ) )
      {
        isWithin = true;
      }
    }
  }

  if ( isWithin != mIsWithin )
  {
    mIsWithin = isWithin;

    emit isWithinChanged();
  }
}

void Geofencer::setActive( bool active )
{
  if ( mActive == active )
  {
    return;
  }

  mActive = active;
  emit activeChanged();

  checkWithin();
}

void Geofencer::setPosition( const QgsPoint &position )
{
  if ( mPosition == position )
  {
    return;
  }

  mPosition = position;
  emit positionChanged();

  checkWithin();
}

void Geofencer::setPositionCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mPositionCrs == crs )
  {
    return;
  }

  mPositionCrs = crs;
  emit positionCrsChanged();

  if ( mAreasLayer && mAreasLayer->crs() != mPositionCrs )
  {
    fetchAreas();
  }

  checkWithin();
}

void Geofencer::setAreasLayer( QgsVectorLayer *layer )
{
  if ( mAreasLayer == layer )
  {
    return;
  }

  mAreasLayer = layer;
  emit areasLayerChanged();

  fetchAreas();
}
