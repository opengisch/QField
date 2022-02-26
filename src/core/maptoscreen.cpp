/***************************************************************************
              maptoscreen.cpp
               ----------------------------------------------------
              date                 : 22.08.2018
              copyright            : (C) 2018 by Denis Rouzaud
              email                : denis (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "maptoscreen.h"
#include "qgspoint.h"

MapToScreen::MapToScreen( QObject *parent )
  : QObject( parent )
{
}

void MapToScreen::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &MapToScreen::transformPoint );
    disconnect( mMapSettings, &QgsQuickMapSettings::outputSizeChanged, this, &MapToScreen::transformPoint );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &MapToScreen::transformPoint );
  connect( mMapSettings, &QgsQuickMapSettings::outputSizeChanged, this, &MapToScreen::transformPoint );

  transformPoint();
  transformDistance();

  emit mapSettingsChanged();
}

QgsQuickMapSettings *MapToScreen::mapSettings() const
{
  return mMapSettings;
}

void MapToScreen::setMapPoint( const QgsPoint &point )
{
  if ( mMapPoint == point )
    return;

  mMapPoint = point;
  emit mapPointChanged();
  transformPoint();
}

QgsPoint MapToScreen::mapPoint() const
{
  return mMapPoint;
}

QPointF MapToScreen::screenPoint() const
{
  return mScreenPoint;
}

void MapToScreen::transformPoint()
{
  if ( !mMapSettings )
  {
    mScreenPoint = QPointF();
  }
  else
  {
    mScreenPoint = mMapSettings->coordinateToScreen( mMapPoint );
  }
  emit screenPointChanged();
}

void MapToScreen::setMapDistance( const double distance )
{
  if ( mMapDistance == distance )
    return;

  mMapDistance = distance;
  emit mapDistanceChanged();
  transformDistance();
}

double MapToScreen::mapDistance() const
{
  return mMapDistance;
}

double MapToScreen::screenDistance() const
{
  return mScreenDistance;
}

void MapToScreen::transformDistance()
{
  if ( !mMapSettings || qgsDoubleNear( mMapDistance, 0.0 ) || qgsDoubleNear( mMapSettings->mapUnitsPerPoint(), 0.0 ) )
  {
    mScreenDistance = 0.0;
  }
  else
  {
    mScreenDistance = mMapDistance / mMapSettings->mapUnitsPerPoint();
  }
  emit screenDistanceChanged();
}
