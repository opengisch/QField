/***************************************************************************
              locatorhighlight.cpp
               ----------------------------------------------------
              date                 : 9.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "linepolygonhighlight.h"
#include "qgsgeometrywrapper.h"
#include "qgssggeometry.h"

#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgsgeometry.h>
#include <qgsproject.h>


LinePolygonHighlight::LinePolygonHighlight( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setTransformOrigin( QQuickItem::TopLeft );
  setAntialiasing( true );
}

QSGNode *LinePolygonHighlight::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty && mMapSettings )
  {
    delete n;
    n = new QSGNode;

    QgsGeometry geometry( mGeometry ? mGeometry->qgsGeometry() : QgsGeometry() );
    if ( mGeometry && !geometry.isEmpty() )
    {
      Q_ASSERT( geometry.type() != Qgis::GeometryType::Point );

      QgsCoordinateTransform ct( mGeometry->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
      geometry.transform( ct );
    }

    const QgsRectangle extent = mMapSettings->visibleExtent();
    mGeometryCorner = QgsPoint( extent.xMinimum(), extent.yMaximum() );
    mGeometryMUPP = mMapSettings->mapUnitsPerPoint();
    QgsSGGeometry *gn = new QgsSGGeometry( geometry, mColor, mWidth * mMapSettings->devicePixelRatio(), mMapSettings->visibleExtent(), 1.0 / mMapSettings->mapUnitsPerPoint() );
    gn->setFlag( QSGNode::OwnedByParent );
    n->appendChildNode( gn );

    mDirty = false;
    updateTransform();

    emit updated();
  }

  return n;
}

float LinePolygonHighlight::lineWidth() const
{
  return mWidth;
}

void LinePolygonHighlight::setLineWidth( float width )
{
  if ( mWidth == width )
    return;

  mWidth = width;
  mDirty = true;

  emit lineWidthChanged();
  update();
}

QColor LinePolygonHighlight::color() const
{
  return mColor;
}

void LinePolygonHighlight::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;
  mDirty = true;

  emit colorChanged();
  update();
}

QgsQuickMapSettings *LinePolygonHighlight::mapSettings() const
{
  return mMapSettings;
}

void LinePolygonHighlight::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonHighlight::mapCrsChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &LinePolygonHighlight::visibleExtentChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &LinePolygonHighlight::rotationChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonHighlight::mapCrsChanged );
  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &LinePolygonHighlight::visibleExtentChanged );
  connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &LinePolygonHighlight::rotationChanged );

  mDirty = true;
  updateTransform();

  emit mapSettingsChanged();
}

void LinePolygonHighlight::updateTransform()
{
  if ( !mMapSettings )
    return;

  const QgsPointXY pixelCorner = mMapSettings->coordinateToScreen( mGeometryCorner );

  setX( pixelCorner.x() );
  setY( pixelCorner.y() );
  setRotation( mMapSettings->rotation() );
  setScale( mGeometryMUPP / mMapSettings->mapUnitsPerPoint() );

  update();
}

void LinePolygonHighlight::rotationChanged()
{
  updateTransform();
}

void LinePolygonHighlight::visibleExtentChanged()
{
  const double scaleChange = mGeometryMUPP / mMapSettings->mapUnitsPerPoint();
  mDirty = mGeometryMUPP == 0.0 || scaleChange > 1.75 || scaleChange < 0.25;
  updateTransform();
}

void LinePolygonHighlight::mapCrsChanged()
{
  mDirty = true;
  update();
}

void LinePolygonHighlight::makeDirty()
{
  mDirty = true;
  update();
}

QgsGeometryWrapper *LinePolygonHighlight::geometry() const
{
  return mGeometry;
}

void LinePolygonHighlight::setGeometry( QgsGeometryWrapper *geometry )
{
  if ( mGeometry == geometry )
    return;

  if ( mGeometry )
  {
    disconnect( mGeometry, &QgsGeometryWrapper::qgsGeometryChanged, this, &LinePolygonHighlight::makeDirty );
    disconnect( mGeometry, &QgsGeometryWrapper::crsChanged, this, &LinePolygonHighlight::makeDirty );
  }

  mGeometry = geometry;

  if ( mGeometry )
  {
    connect( mGeometry, &QgsGeometryWrapper::qgsGeometryChanged, this, &LinePolygonHighlight::makeDirty );
    connect( mGeometry, &QgsGeometryWrapper::crsChanged, this, &LinePolygonHighlight::makeDirty );
  }

  mDirty = true;
  emit qgsGeometryChanged();

  update();
}
