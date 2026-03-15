

# File maptoview3d.cpp

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**maptoview3d.cpp**](maptoview3d_8cpp.md)

[Go to the documentation of this file](maptoview3d_8cpp.md)


```C++
/***************************************************************************
  maptoview3d.cpp - MapToView3D

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

#include "maptoview3d.h"

MapToView3D::MapToView3D( QObject *parent )
  : QObject( parent )
{
}

Quick3DTerrainProvider *MapToView3D::terrainProvider() const
{
  return mTerrainProvider;
}

void MapToView3D::setTerrainProvider( Quick3DTerrainProvider *provider )
{
  if ( mTerrainProvider == provider )
  {
    return;
  }

  if ( mTerrainProvider )
  {
    disconnect( mTerrainProvider, &Quick3DTerrainProvider::extentChanged, this, &MapToView3D::transformPoint );
  }

  mTerrainProvider = provider;

  if ( mTerrainProvider )
  {
    connect( mTerrainProvider, &Quick3DTerrainProvider::extentChanged, this, &MapToView3D::transformPoint );
  }

  emit terrainProviderChanged();
  transformPoint();
}

QgsPoint MapToView3D::mapPoint() const
{
  return mMapPoint;
}

void MapToView3D::setMapPoint( const QgsPoint &point )
{
  if ( mMapPoint == point )
  {
    return;
  }

  mMapPoint = point;
  emit mapPointChanged();
  transformPoint();
}

float MapToView3D::heightOffset() const
{
  return mHeightOffset;
}

void MapToView3D::setHeightOffset( float offset )
{
  if ( qFuzzyCompare( mHeightOffset, offset ) )
  {
    return;
  }

  mHeightOffset = offset;
  emit heightOffsetChanged();
  transformPoint();
}

QVector3D MapToView3D::viewPoint() const
{
  return mViewPoint;
}

void MapToView3D::transformPoint()
{
  QVector3D newPoint;
  if ( mTerrainProvider && !mMapPoint.isEmpty() )
  {
    newPoint = mTerrainProvider->geoTo3D( mMapPoint.x(), mMapPoint.y(), mHeightOffset );
  }

  if ( mViewPoint != newPoint )
  {
    mViewPoint = newPoint;
    emit viewPointChanged();
  }
}
```


