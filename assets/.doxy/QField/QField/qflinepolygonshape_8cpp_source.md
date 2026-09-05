

# File qflinepolygonshape.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qflinepolygonshape.cpp**](qflinepolygonshape_8cpp.md)

[Go to the documentation of this file](qflinepolygonshape_8cpp.md)


```C++
/***************************************************************************
              qflinepolygonshape.cpp
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

#include "qflinepolygonshape.h"

#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgscurve.h>
#include <qgsgeometry.h>
#include <qgspolygon.h>
#include <qgsproject.h>


QfLinePolygonShape::QfLinePolygonShape( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setTransformOrigin( QQuickItem::TopLeft );
  setAntialiasing( true );
}

void QfLinePolygonShape::createPolylines()
{
  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  const double scaleFactor = 1.0 / mMapSettings->mapUnitsPerPoint();

  if ( !mGeometry || !mGeometry->crs().isValid() || mGeometry->qgsGeometry().isNull() )
  {
    if ( mPolylinesType != Qgis::GeometryType::Null )
    {
      mPolylinesType = Qgis::GeometryType::Null;
      emit polylinesTypeChanged();
    }

    if ( !mPolylines.isEmpty() )
    {
      mPolylines.clear();
      emit polylinesChanged();
    }

    return;
  }

  mPolylines.clear();

  QgsGeometry geometry( mGeometry->qgsGeometry() );
  if ( mGeometry->crs() != mMapSettings->destinationCrs() )
  {
    QgsCoordinateTransform ct( mGeometry->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
    try
    {
      geometry.transform( ct );
    }
    catch ( const QgsException & )
    {
      geometry = QgsGeometry();
    }
  }

  Qgis::GeometryType geomType = Qgis::GeometryType::Null;
  if ( !geometry.isEmpty() && geometry.type() != Qgis::GeometryType::Point )
  {
    const QPolygonF boundingRect = QPolygonF( QRectF( QPointF( -MAX_SIZE, -MAX_SIZE ), QPointF( MAX_SIZE, MAX_SIZE ) ) );

    geometry = geometry.simplify( mMapSettings->mapUnitsPerPoint() );
    geometry = geometry.makeValid();
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
          const QRectF rect = polyline.boundingRect();
          if ( std::max( { std::abs( rect.left() ), std::abs( rect.right() ), std::abs( rect.top() ), std::abs( rect.bottom() ) } ) > MAX_SIZE )
          {
            polyline = polyline.intersected( boundingRect );
          }
          mPolylines.append( polyline );
        }
        break;
      }

      case Qgis::GeometryType::Polygon:
      {
        const QgsMultiPolygonXY polygons = geometry.isMultipart() ? geometry.asMultiPolygon() : QgsMultiPolygonXY() << geometry.asPolygon();
        for ( const QgsPolygonXY &polygon : polygons )
        {
          for ( const QgsPolylineXY &line : polygon )
          {
            QPolygonF polyline;
            for ( const QgsPointXY &point : line )
            {
              polyline << QPointF( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor, ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor );
            }
            const QRectF rect = polyline.boundingRect();
            if ( std::max( { std::abs( rect.left() ), std::abs( rect.right() ), std::abs( rect.top() ), std::abs( rect.bottom() ) } ) > MAX_SIZE )
            {
              polyline = polyline.intersected( boundingRect );
            }
            mPolylines.append( polyline );
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

float QfLinePolygonShape::lineWidth() const
{
  return mWidth;
}

void QfLinePolygonShape::setLineWidth( float width )
{
  if ( mWidth == width )
    return;

  mWidth = width;
  mDirty = true;

  emit lineWidthChanged();
}

QColor QfLinePolygonShape::color() const
{
  return mColor;
}

void QfLinePolygonShape::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;
  mDirty = true;

  emit colorChanged();
}

QgsQuickMapSettings *QfLinePolygonShape::mapSettings() const
{
  return mMapSettings;
}

void QfLinePolygonShape::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfLinePolygonShape::mapCrsChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfLinePolygonShape::visibleExtentChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfLinePolygonShape::rotationChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfLinePolygonShape::mapCrsChanged );
  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfLinePolygonShape::visibleExtentChanged );
  connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfLinePolygonShape::rotationChanged );

  mDirty = true;
  updateTransform();

  emit mapSettingsChanged();
}

void QfLinePolygonShape::updateTransform()
{
  if ( !mMapSettings )
    return;

  if ( !mDirty && !mGeometryCorner.isEmpty() )
  {
    const QgsPointXY pixelCorner = mMapSettings->coordinateToScreen( mGeometryCorner );
    mDirty = std::abs( x() ) > MAX_OFFSET || std::abs( y() ) > MAX_OFFSET;
  }

  if ( mDirty )
  {
    const QgsRectangle extent = mMapSettings->visibleExtent();
    mGeometryCorner = QgsPointXY( extent.xMinimum(), extent.yMaximum() );
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

void QfLinePolygonShape::rotationChanged()
{
  updateTransform();
}

void QfLinePolygonShape::visibleExtentChanged()
{
  const double scaleChange = mGeometryMUPP / mMapSettings->mapUnitsPerPoint();
  mDirty = mDirty || mGeometryMUPP == 0.0 || scaleChange > 1.75 || scaleChange < 0.25;
  updateTransform();
}

void QfLinePolygonShape::mapCrsChanged()
{
  mDirty = true;
  updateTransform();
}

void QfLinePolygonShape::makeDirty()
{
  mDirty = true;
  updateTransform();
}

QfGeometryWrapper *QfLinePolygonShape::geometry() const
{
  return mGeometry;
}

void QfLinePolygonShape::setGeometry( QfGeometryWrapper *geometry )
{
  if ( mGeometry == geometry )
    return;

  if ( mGeometry )
  {
    disconnect( mGeometry, &QfGeometryWrapper::qgsGeometryChanged, this, &QfLinePolygonShape::catchGeometryWrapperChange );
    disconnect( mGeometry, &QfGeometryWrapper::crsChanged, this, &QfLinePolygonShape::catchGeometryWrapperChange );
  }

  mGeometry = geometry;

  if ( mGeometry )
  {
    connect( mGeometry, &QfGeometryWrapper::qgsGeometryChanged, this, &QfLinePolygonShape::catchGeometryWrapperChange );
    connect( mGeometry, &QfGeometryWrapper::crsChanged, this, &QfLinePolygonShape::catchGeometryWrapperChange );

    if ( mGeometry->crs().isValid() && !mGeometry->qgsGeometry().isEmpty() )
    {
      qInfo() << "QfLinePolygonShape::setGeometry crs:" << mGeometry->crs().authid();
      qInfo() << "QfLinePolygonShape::setGeometry geometry:" << mGeometry->qgsGeometry().asWkt();
    }
  }

  mDirty = true;
  emit geometryChanged();

  updateTransform();
}

void QfLinePolygonShape::catchGeometryWrapperChange()
{
  if ( mGeometry->crs().isValid() && !mGeometry->qgsGeometry().isEmpty() )
  {
    qInfo() << "QfLinePolygonShape::catchGeometryWrapperChange crs:" << mGeometry->crs().authid();
    qInfo() << "QfLinePolygonShape::catchGeometryWrapperChange geometry:" << mGeometry->qgsGeometry().asWkt();
  }

  makeDirty();
}
```


