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

void Quick3DGeometryConfiguration::setLineColor( const QColor &color )
{
  if ( mLineColor == color )
  {
    return;
  }
  mLineColor = color;
  emit lineColorChanged();
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
