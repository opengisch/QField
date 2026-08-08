

# File quick3dgeometryconfiguration.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**quick3dgeometryconfiguration.cpp**](quick3dgeometryconfiguration_8cpp.md)

[Go to the documentation of this file](quick3dgeometryconfiguration_8cpp.md)


```C++
/***************************************************************************
  quick3dgeometryconfiguration.cpp - Quick3DGeometryConfiguration

 ---------------------
 begin                : 16.6.2026
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

#include "quick3dgeometryconfiguration.h"

Quick3DGeometryConfiguration::Quick3DGeometryConfiguration( QQuickItem *parent )
  : QQuickItem( parent )
{
  setVisible( false );
}

void Quick3DGeometryConfiguration::setWkt( const QString &wkt )
{
  if ( mWkt == wkt )
  {
    return;
  }

  mWkt = wkt;
  emit wktChanged();
}

void Quick3DGeometryConfiguration::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
  {
    return;
  }

  mCrs = crs;
  emit crsChanged();
}

void Quick3DGeometryConfiguration::setHeightOffset( float offset )
{
  if ( mHeightOffset == offset )
  {
    return;
  }

  mHeightOffset = offset;
  emit heightOffsetChanged();
}

void Quick3DGeometryConfiguration::setAltitudeClamping( Quick3DGeometry::AltitudeClamping clamping )
{
  if ( mAltitudeClamping == clamping )
  {
    return;
  }

  mAltitudeClamping = clamping;
  emit altitudeClampingChanged();
}

void Quick3DGeometryConfiguration::setColor( const QColor &color )
{
  if ( mColor == color )
  {
    return;
  }

  mColor = color;
  emit colorChanged();
}

void Quick3DGeometryConfiguration::setLineWidth( float width )
{
  if ( qFuzzyCompare( mLineWidth, width ) )
  {
    return;
  }

  mLineWidth = width;
  emit lineWidthChanged();
}
```


