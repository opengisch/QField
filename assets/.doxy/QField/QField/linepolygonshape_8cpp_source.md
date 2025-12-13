

# File linepolygonshape.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**linepolygonshape.cpp**](linepolygonshape_8cpp.md)

[Go to the documentation of this file](linepolygonshape_8cpp.md)


```C++
/***************************************************************************
              linepolygonshape.cpp
               ----------------------------------------------------
              date                 : 21.11.2023
              copyright            : (C) 2023 by Mathieu Pellerin
              email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "linepolygonshape.h"

#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgscurve.h>
#include <qgsgeometry.h>
#include <qgspolygon.h>
#include <qgsproject.h>


LinePolygonShape::LinePolygonShape( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setTransformOrigin( QQuickItem::TopLeft );
  setAntialiasing( true );
}

void LinePolygonShape::createPolylines()
{
  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  const double scaleFactor = 1.0 / mMapSettings->mapUnitsPerPoint();

  mPolylines.clear();

  QgsGeometry geometry( mGeometry ? mGeometry->qgsGeometry() : QgsGeometry() );
  Qgis::GeometryType geomType = Qgis::GeometryType::Null;
  if ( mGeometry && !geometry.isEmpty() && geometry.type() != Qgis::GeometryType::Point )
  {
    QgsCoordinateTransform ct( mGeometry->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
    geometry.transform( ct );

    geomType = geometry.type();
    switch ( geomType )
    {
      case Qgis::GeometryType::Line:
      {
        const QgsMultiPolylineXY lines = geometry.isMultipart() ? geometry.asMultiPolyline() : QgsMultiPolylineXY() << geometry.asPolyline();
        for ( const QgsPolylineXY &line : lines )
        {
          QPolygonF polyline;
          for ( const QgsPointXY &point : line )
          {
            polyline << QPointF( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor, ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor );
          }
          mPolylines << polyline;
        }
        break;
      }

      case Qgis::GeometryType::Polygon:
      {
        const QgsMultiPolygonXY polygons = geometry.isMultipart() ? geometry.asMultiPolygon() : QgsMultiPolygonXY() << geometry.asPolygon();
        for ( const QgsPolygonXY &polygon : polygons )
        {
          for ( const QgsPolylineXY line : polygon )
          {
            QPolygonF polyline;
            for ( const QgsPointXY &point : line )
            {
              polyline << QPointF( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor, ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor );
            }
            mPolylines << polyline;
          }
        }
        break;
      }

      case Qgis::GeometryType::Point:
      case Qgis::GeometryType::Unknown:
      case Qgis::GeometryType::Null:
        break;
    }
  }

  if ( geomType != mPolylinesType )
  {
    mPolylinesType = geomType;
    emit polylinesTypeChanged();
  }

  emit polylinesChanged();
}

float LinePolygonShape::lineWidth() const
{
  return mWidth;
}

void LinePolygonShape::setLineWidth( float width )
{
  if ( mWidth == width )
    return;

  mWidth = width;
  mDirty = true;

  emit lineWidthChanged();
}

QColor LinePolygonShape::color() const
{
  return mColor;
}

void LinePolygonShape::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;
  mDirty = true;

  emit colorChanged();
}

QgsQuickMapSettings *LinePolygonShape::mapSettings() const
{
  return mMapSettings;
}

void LinePolygonShape::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonShape::mapCrsChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &LinePolygonShape::visibleExtentChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &LinePolygonShape::rotationChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonShape::mapCrsChanged );
  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &LinePolygonShape::visibleExtentChanged );
  connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &LinePolygonShape::rotationChanged );

  mDirty = true;
  updateTransform();

  emit mapSettingsChanged();
}

void LinePolygonShape::updateTransform()
{
  if ( !mMapSettings )
    return;

  if ( mDirty )
  {
    const QgsRectangle extent = mMapSettings->visibleExtent();
    mGeometryCorner = QgsPoint( extent.xMinimum(), extent.yMaximum() );
    mGeometryMUPP = mMapSettings->mapUnitsPerPoint();

    createPolylines();
    mDirty = false;
  }

  const QgsPointXY pixelCorner = mMapSettings->coordinateToScreen( mGeometryCorner );

  setX( pixelCorner.x() );
  setY( pixelCorner.y() );
  setScale( mGeometryMUPP / mMapSettings->mapUnitsPerPoint() );
  setRotation( mMapSettings->rotation() );
}

void LinePolygonShape::rotationChanged()
{
  updateTransform();
}

void LinePolygonShape::visibleExtentChanged()
{
  const double scaleChange = mGeometryMUPP / mMapSettings->mapUnitsPerPoint();
  mDirty = mDirty || mGeometryMUPP == 0.0 || scaleChange > 1.75 || scaleChange < 0.25;
  updateTransform();
}

void LinePolygonShape::mapCrsChanged()
{
  mDirty = true;
  updateTransform();
}

void LinePolygonShape::makeDirty()
{
  mDirty = true;
  updateTransform();
}

QgsGeometryWrapper *LinePolygonShape::geometry() const
{
  return mGeometry;
}

void LinePolygonShape::setGeometry( QgsGeometryWrapper *geometry )
{
  if ( mGeometry == geometry )
    return;

  if ( mGeometry )
  {
    disconnect( mGeometry, &QgsGeometryWrapper::qgsGeometryChanged, this, &LinePolygonShape::makeDirty );
    disconnect( mGeometry, &QgsGeometryWrapper::crsChanged, this, &LinePolygonShape::makeDirty );
  }

  mGeometry = geometry;

  if ( mGeometry )
  {
    connect( mGeometry, &QgsGeometryWrapper::qgsGeometryChanged, this, &LinePolygonShape::makeDirty );
    connect( mGeometry, &QgsGeometryWrapper::crsChanged, this, &LinePolygonShape::makeDirty );
  }

  mDirty = true;
  emit qgsGeometryChanged();

  updateTransform();
}
```


