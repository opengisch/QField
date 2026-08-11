/***************************************************************************
  qf3dgeometryconfiguration.cpp - Qf3DGeometryConfiguration

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

#include "qf3dgeometryconfiguration.h"

Qf3DGeometryConfiguration::Qf3DGeometryConfiguration( QQuickItem *parent )
  : QQuickItem( parent )
{
  setVisible( false );
}

void Qf3DGeometryConfiguration::setWkt( const QString &wkt )
{
  if ( mWkt == wkt )
  {
    return;
  }

  mWkt = wkt;
  emit wktChanged();
}

void Qf3DGeometryConfiguration::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
  {
    return;
  }

  mCrs = crs;
  emit crsChanged();
}

void Qf3DGeometryConfiguration::setHeightOffset( float offset )
{
  if ( mHeightOffset == offset )
  {
    return;
  }

  mHeightOffset = offset;
  emit heightOffsetChanged();
}

void Qf3DGeometryConfiguration::setAltitudeClamping( Qf3DGeometry::AltitudeClamping clamping )
{
  if ( mAltitudeClamping == clamping )
  {
    return;
  }

  mAltitudeClamping = clamping;
  emit altitudeClampingChanged();
}

void Qf3DGeometryConfiguration::setColor( const QColor &color )
{
  if ( mColor == color )
  {
    return;
  }

  mColor = color;
  emit colorChanged();
}

void Qf3DGeometryConfiguration::setLineWidth( float width )
{
  if ( qFuzzyCompare( mLineWidth, width ) )
  {
    return;
  }

  mLineWidth = width;
  emit lineWidthChanged();
}
