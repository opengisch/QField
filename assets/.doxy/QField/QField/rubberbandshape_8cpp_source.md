

# File rubberbandshape.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**rubberbandshape.cpp**](rubberbandshape_8cpp.md)

[Go to the documentation of this file](rubberbandshape_8cpp.md)


```C++
/***************************************************************************
  rubberbandshape.cpp - RubberbandShape

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


#include "qgsquickmapsettings.h"
#include "rubberbandmodel.h"
#include "rubberbandshape.h"
#include "vertexmodel.h"

RubberbandShape::RubberbandShape( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setTransformOrigin( QQuickItem::TopLeft );
  setAntialiasing( true );
}

RubberbandModel *RubberbandShape::model() const
{
  return mRubberbandModel;
}

void RubberbandShape::setModel( RubberbandModel *model )
{
  if ( model )
    setVertexModel( nullptr );

  if ( mRubberbandModel == model )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &RubberbandShape::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &RubberbandShape::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &RubberbandShape::markDirty );
  }


  mRubberbandModel = model;

  if ( mRubberbandModel && !mFreeze )
  {
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &RubberbandShape::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &RubberbandShape::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &RubberbandShape::markDirty );
  }

  markDirty();

  emit modelChanged();
}

VertexModel *RubberbandShape::vertexModel() const
{
  return mVertexModel;
}

void RubberbandShape::setVertexModel( VertexModel *vertexModel )
{
  if ( vertexModel )
    setModel( nullptr );

  if ( mVertexModel == vertexModel )
    return;

  if ( mVertexModel )
  {
    disconnect( mVertexModel, &VertexModel::dataChanged, this, &RubberbandShape::markDirty );
    disconnect( mVertexModel, &VertexModel::vertexCountChanged, this, &RubberbandShape::markDirty );
    disconnect( mVertexModel, &VertexModel::geometryChanged, this, &RubberbandShape::markDirty );
  }

  mVertexModel = vertexModel;

  if ( mVertexModel && !mFreeze )
  {
    connect( mVertexModel, &VertexModel::dataChanged, this, &RubberbandShape::markDirty );
    connect( mVertexModel, &VertexModel::vertexCountChanged, this, &RubberbandShape::markDirty );
    connect( mVertexModel, &VertexModel::geometryChanged, this, &RubberbandShape::markDirty );
  }

  markDirty();

  emit vertexModelChanged();
}

bool RubberbandShape::freeze() const
{
  return mFreeze;
}

void RubberbandShape::setFreeze( bool freeze )
{
  if ( mFreeze == freeze )
    return;

  mFreeze = freeze;
  emit freezeChanged();

  if ( mFreeze )
  {
    if ( mVertexModel )
    {
      disconnect( mVertexModel, &VertexModel::dataChanged, this, &RubberbandShape::markDirty );
      disconnect( mVertexModel, &VertexModel::vertexCountChanged, this, &RubberbandShape::markDirty );
      disconnect( mVertexModel, &VertexModel::geometryChanged, this, &RubberbandShape::markDirty );
    }
    if ( mRubberbandModel )
    {
      disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &RubberbandShape::markDirty );
      disconnect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &RubberbandShape::markDirty );
      disconnect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &RubberbandShape::markDirty );
    }
    if ( mMapSettings )
    {
      disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &RubberbandShape::visibleExtentChanged );
      disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &RubberbandShape::rotationChanged );
    }
  }
  else
  {
    if ( mVertexModel )
    {
      connect( mVertexModel, &VertexModel::dataChanged, this, &RubberbandShape::markDirty );
      connect( mVertexModel, &VertexModel::vertexCountChanged, this, &RubberbandShape::markDirty );
      connect( mVertexModel, &VertexModel::geometryChanged, this, &RubberbandShape::markDirty );
    }
    if ( mRubberbandModel )
    {
      connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &RubberbandShape::markDirty );
      connect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &RubberbandShape::markDirty );
      connect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &RubberbandShape::markDirty );
    }
    if ( mMapSettings && !mFreeze )
    {
      connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &RubberbandShape::visibleExtentChanged );
      connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &RubberbandShape::rotationChanged );
    }

    markDirty();
  }
}

QgsQuickMapSettings *RubberbandShape::mapSettings() const
{
  return mMapSettings;
}

void RubberbandShape::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &RubberbandShape::visibleExtentChanged );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &RubberbandShape::rotationChanged );
  }

  mMapSettings = mapSettings;

  if ( mMapSettings && !mFreeze )
  {
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &RubberbandShape::visibleExtentChanged );
    connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &RubberbandShape::rotationChanged );
  }

  markDirty();

  emit mapSettingsChanged();
}

void RubberbandShape::updateTransform()
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

void RubberbandShape::rotationChanged()
{
  updateTransform();
}

void RubberbandShape::visibleExtentChanged()
{
  const double scaleChange = mGeometryMUPP / mMapSettings->mapUnitsPerPoint();
  mDirty = mDirty || mGeometryMUPP == 0.0 || scaleChange > 1.75 || scaleChange < 0.25;
  updateTransform();
}

void RubberbandShape::markDirty()
{
  mDirty = true;
  updateTransform();
}

void RubberbandShape::createPolylines()
{
  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  const double scaleFactor = 1.0 / mMapSettings->mapUnitsPerPoint();

  mPolylines.clear();

  QList<QPointF> polyline;
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
  mPolylines << polyline;

  if ( geomType != mPolylinesType )
  {
    mPolylinesType = geomType;
    emit polylinesTypeChanged();
  }

  emit polylinesChanged();
}

void RubberbandShape::setGeometryType( const Qgis::GeometryType geometryType )
{
  if ( mGeometryType == geometryType )
    return;

  mGeometryType = geometryType;

  emit geometryTypeChanged();
}

float RubberbandShape::lineWidth() const
{
  return mWidth;
}

void RubberbandShape::setLineWidth( float width )
{
  if ( mWidth == width )
    return;

  mWidth = width;

  emit lineWidthChanged();
}

QColor RubberbandShape::color() const
{
  return mColor;
}

void RubberbandShape::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;

  emit colorChanged();
}

QColor RubberbandShape::outlineColor() const
{
  return mOutlineColor;
}

void RubberbandShape::setOutlineColor( const QColor &color )
{
  if ( mOutlineColor == color )
    return;

  mOutlineColor = color;

  emit outlineColorChanged();
}
```


