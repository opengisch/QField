

# File qfrubberbandshape.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfrubberbandshape.cpp**](qfrubberbandshape_8cpp.md)

[Go to the documentation of this file](qfrubberbandshape_8cpp.md)


```C++
/***************************************************************************
  qfrubberbandshape.cpp - QfRubberbandShape

 ---------------------
 begin                : 11.6.2016
 copyright            : (C) 2016 by Matthias Kuhn (OPENGIS.ch)
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qfrubberbandmodel.h"
#include "qfrubberbandshape.h"
#include "qfvertexmodel.h"
#include "qgsquickmapsettings.h"

QfRubberbandShape::QfRubberbandShape( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setTransformOrigin( QQuickItem::TopLeft );
  setAntialiasing( true );
}

QfRubberbandModel *QfRubberbandShape::model() const
{
  return mRubberbandModel;
}

void QfRubberbandShape::setModel( QfRubberbandModel *model )
{
  if ( model )
    setVertexModel( nullptr );

  if ( mRubberbandModel == model )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfRubberbandShape::markDirty );
    disconnect( mRubberbandModel, &QfRubberbandModel::verticesRemoved, this, &QfRubberbandShape::markDirty );
    disconnect( mRubberbandModel, &QfRubberbandModel::verticesInserted, this, &QfRubberbandShape::markDirty );
  }


  mRubberbandModel = model;

  if ( mRubberbandModel && !mFreeze )
  {
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfRubberbandShape::markDirty );
    connect( mRubberbandModel, &QfRubberbandModel::verticesRemoved, this, &QfRubberbandShape::markDirty );
    connect( mRubberbandModel, &QfRubberbandModel::verticesInserted, this, &QfRubberbandShape::markDirty );
  }

  markDirty();

  emit modelChanged();
}

QfVertexModel *QfRubberbandShape::vertexModel() const
{
  return mVertexModel;
}

void QfRubberbandShape::setVertexModel( QfVertexModel *vertexModel )
{
  if ( vertexModel )
    setModel( nullptr );

  if ( mVertexModel == vertexModel )
    return;

  if ( mVertexModel )
  {
    disconnect( mVertexModel, &QfVertexModel::dataChanged, this, &QfRubberbandShape::markDirty );
    disconnect( mVertexModel, &QfVertexModel::vertexCountChanged, this, &QfRubberbandShape::markDirty );
    disconnect( mVertexModel, &QfVertexModel::geometryChanged, this, &QfRubberbandShape::markDirty );
  }

  mVertexModel = vertexModel;

  if ( mVertexModel && !mFreeze )
  {
    connect( mVertexModel, &QfVertexModel::dataChanged, this, &QfRubberbandShape::markDirty );
    connect( mVertexModel, &QfVertexModel::vertexCountChanged, this, &QfRubberbandShape::markDirty );
    connect( mVertexModel, &QfVertexModel::geometryChanged, this, &QfRubberbandShape::markDirty );
  }

  markDirty();

  emit vertexModelChanged();
}

bool QfRubberbandShape::freeze() const
{
  return mFreeze;
}

void QfRubberbandShape::setFreeze( bool freeze )
{
  if ( mFreeze == freeze )
    return;

  mFreeze = freeze;
  emit freezeChanged();

  if ( mFreeze )
  {
    if ( mVertexModel )
    {
      disconnect( mVertexModel, &QfVertexModel::dataChanged, this, &QfRubberbandShape::markDirty );
      disconnect( mVertexModel, &QfVertexModel::vertexCountChanged, this, &QfRubberbandShape::markDirty );
      disconnect( mVertexModel, &QfVertexModel::geometryChanged, this, &QfRubberbandShape::markDirty );
    }
    if ( mRubberbandModel )
    {
      disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfRubberbandShape::markDirty );
      disconnect( mRubberbandModel, &QfRubberbandModel::verticesRemoved, this, &QfRubberbandShape::markDirty );
      disconnect( mRubberbandModel, &QfRubberbandModel::verticesInserted, this, &QfRubberbandShape::markDirty );
    }
    if ( mMapSettings )
    {
      disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfRubberbandShape::visibleExtentChanged );
      disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfRubberbandShape::rotationChanged );
    }
  }
  else
  {
    if ( mVertexModel )
    {
      connect( mVertexModel, &QfVertexModel::dataChanged, this, &QfRubberbandShape::markDirty );
      connect( mVertexModel, &QfVertexModel::vertexCountChanged, this, &QfRubberbandShape::markDirty );
      connect( mVertexModel, &QfVertexModel::geometryChanged, this, &QfRubberbandShape::markDirty );
    }
    if ( mRubberbandModel )
    {
      connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfRubberbandShape::markDirty );
      connect( mRubberbandModel, &QfRubberbandModel::verticesRemoved, this, &QfRubberbandShape::markDirty );
      connect( mRubberbandModel, &QfRubberbandModel::verticesInserted, this, &QfRubberbandShape::markDirty );
    }
    if ( mMapSettings && !mFreeze )
    {
      connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfRubberbandShape::visibleExtentChanged );
      connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfRubberbandShape::rotationChanged );
    }

    markDirty();
  }
}

QgsQuickMapSettings *QfRubberbandShape::mapSettings() const
{
  return mMapSettings;
}

void QfRubberbandShape::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfRubberbandShape::visibleExtentChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfRubberbandShape::rotationChanged );
  }

  mMapSettings = mapSettings;

  if ( mMapSettings && !mFreeze )
  {
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfRubberbandShape::visibleExtentChanged );
    connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfRubberbandShape::rotationChanged );
  }

  markDirty();

  emit mapSettingsChanged();
}

void QfRubberbandShape::updateTransform()
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

void QfRubberbandShape::rotationChanged()
{
  updateTransform();
}

void QfRubberbandShape::visibleExtentChanged()
{
  const double scaleChange = mGeometryMUPP / mMapSettings->mapUnitsPerPoint();
  mDirty = mDirty || mGeometryMUPP == 0.0 || scaleChange > 1.75 || scaleChange < 0.25;
  updateTransform();
}

void QfRubberbandShape::markDirty()
{
  mDirty = true;
  updateTransform();
}

void QfRubberbandShape::createPolylines()
{
  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  const double scaleFactor = 1.0 / mMapSettings->mapUnitsPerPoint();

  mPolylines.clear();

  QPolygonF polyline;
  QVector<QgsPoint> allVertices = QVector<QgsPoint>();
  Qgis::GeometryType geomType = mGeometryType;
  if ( mRubberbandModel && !mRubberbandModel->isEmpty() )
  {
    allVertices = mRubberbandModel->vertices();
    if ( geomType == Qgis::GeometryType::Null )
    {
      geomType = mRubberbandModel->geometryType();
    }
  }
  else if ( mVertexModel && mVertexModel->vertexCount() > 0 )
  {
    allVertices = mVertexModel->flatVertices();
    if ( geomType == Qgis::GeometryType::Null )
    {
      geomType = mVertexModel->geometryType();
    }
  }
  for ( const QgsPoint &point : std::as_const( allVertices ) )
  {
    polyline << QPointF( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor, ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor );
  }

  const QRectF rect = polyline.boundingRect();
  if ( std::max( { std::abs( rect.left() ), std::abs( rect.right() ), std::abs( rect.top() ), std::abs( rect.bottom() ) } ) > MAX_SIZE )
  {
    const QPolygonF boundingRect( QRectF( QPointF( -MAX_SIZE, -MAX_SIZE ), QPointF( MAX_SIZE, MAX_SIZE ) ) );
    polyline = polyline.intersected( boundingRect );
  }

  mPolylines.append( polyline );

  if ( geomType != mPolylinesType )
  {
    mPolylinesType = geomType;
    emit polylinesTypeChanged();
  }

  emit polylinesChanged();
}

void QfRubberbandShape::setGeometryType( const Qgis::GeometryType geometryType )
{
  if ( mGeometryType == geometryType )
    return;

  mGeometryType = geometryType;

  emit geometryTypeChanged();
}

float QfRubberbandShape::lineWidth() const
{
  return mWidth;
}

void QfRubberbandShape::setLineWidth( float width )
{
  if ( mWidth == width )
    return;

  mWidth = width;

  emit lineWidthChanged();
}

QColor QfRubberbandShape::color() const
{
  return mColor;
}

void QfRubberbandShape::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;

  emit colorChanged();
}

QColor QfRubberbandShape::outlineColor() const
{
  return mOutlineColor;
}

void QfRubberbandShape::setOutlineColor( const QColor &color )
{
  if ( mOutlineColor == color )
    return;

  mOutlineColor = color;

  emit outlineColorChanged();
}
```


