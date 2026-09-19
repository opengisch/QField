

# File qfmaptoview3d.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qfmaptoview3d.cpp**](qfmaptoview3d_8cpp.md)

[Go to the documentation of this file](qfmaptoview3d_8cpp.md)


```C++
/***************************************************************************
  qfmaptoview3d.cpp - QfMapToView3D

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

#include "qfmaptoview3d.h"

QfMapToView3D::QfMapToView3D( QObject *parent )
  : QObject( parent )
{
}

Qf3DTerrainProvider *QfMapToView3D::terrainProvider() const
{
  return mTerrainProvider;
}

void QfMapToView3D::setTerrainProvider( Qf3DTerrainProvider *provider )
{
  if ( mTerrainProvider == provider )
  {
    return;
  }

  if ( mTerrainProvider )
  {
    disconnect( mTerrainProvider, &Qf3DTerrainProvider::extentChanged, this, &QfMapToView3D::transformPoint );
  }

  mTerrainProvider = provider;

  if ( mTerrainProvider )
  {
    connect( mTerrainProvider, &Qf3DTerrainProvider::extentChanged, this, &QfMapToView3D::transformPoint );
  }

  emit terrainProviderChanged();
  transformPoint();
}

QgsPoint QfMapToView3D::mapPoint() const
{
  return mMapPoint;
}

void QfMapToView3D::setMapPoint( const QgsPoint &point )
{
  if ( mMapPoint == point )
  {
    return;
  }

  mMapPoint = point;
  emit mapPointChanged();
  transformPoint();
}

float QfMapToView3D::heightOffset() const
{
  return mHeightOffset;
}

void QfMapToView3D::setHeightOffset( float offset )
{
  if ( qFuzzyCompare( mHeightOffset, offset ) )
  {
    return;
  }

  mHeightOffset = offset;
  emit heightOffsetChanged();
  transformPoint();
}

QVector3D QfMapToView3D::viewPoint() const
{
  return mViewPoint;
}

void QfMapToView3D::transformPoint()
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


