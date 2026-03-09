/***************************************************************************
  maptoscreen3d.cpp - MapToScreen3D

 ---------------------
 begin                : 9.3.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "maptoscreen3d.h"

MapToScreen3D::MapToScreen3D( QObject *parent )
  : QObject( parent )
{
}

Quick3DTerrainProvider *MapToScreen3D::terrainProvider() const
{
  return mTerrainProvider;
}

void MapToScreen3D::setTerrainProvider( Quick3DTerrainProvider *provider )
{
  if ( mTerrainProvider == provider )
  {
    return;
  }

  if ( mTerrainProvider )
  {
    disconnect( mTerrainProvider, &Quick3DTerrainProvider::extentChanged, this, &MapToScreen3D::transformPoint );
  }

  mTerrainProvider = provider;

  if ( mTerrainProvider )
  {
    connect( mTerrainProvider, &Quick3DTerrainProvider::extentChanged, this, &MapToScreen3D::transformPoint );
  }

  emit terrainProviderChanged();
  transformPoint();
}

QgsPoint MapToScreen3D::mapPoint() const
{
  return mMapPoint;
}

void MapToScreen3D::setMapPoint( const QgsPoint &point )
{
  if ( mMapPoint == point )
  {
    return;
  }

  mMapPoint = point;
  emit mapPointChanged();
  transformPoint();
}

float MapToScreen3D::heightOffset() const
{
  return mHeightOffset;
}

void MapToScreen3D::setHeightOffset( float offset )
{
  if ( qFuzzyCompare( mHeightOffset, offset ) )
  {
    return;
  }

  mHeightOffset = offset;
  emit heightOffsetChanged();
  transformPoint();
}

QVector3D MapToScreen3D::scenePoint() const
{
  return mScenePoint;
}

void MapToScreen3D::transformPoint()
{
  QVector3D newPoint;
  if ( mTerrainProvider && !mMapPoint.isEmpty() )
  {
    newPoint = mTerrainProvider->geoTo3D( mMapPoint.x(), mMapPoint.y(), mHeightOffset );
  }

  if ( mScenePoint != newPoint )
  {
    mScenePoint = newPoint;
    emit scenePointChanged();
  }
}
