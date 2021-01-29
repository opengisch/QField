/***************************************************************************
  rubberband.cpp - Rubberband

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


#include "vertexmodel.h"
#include "rubberband.h"
#include "rubberbandmodel.h"
#include "sgrubberband.h"
#include "qgsquickmapsettings.h"

Rubberband::Rubberband( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

RubberbandModel *Rubberband::model() const
{
  return mRubberbandModel;
}

void Rubberband::setModel( RubberbandModel *model )
{
  if ( model )
    setVertexModel( nullptr );

  if ( mRubberbandModel == model )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Rubberband::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &Rubberband::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &Rubberband::markDirty );
  }


  mRubberbandModel = model;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Rubberband::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &Rubberband::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &Rubberband::markDirty );
  }

  markDirty();

  emit modelChanged();
}

VertexModel *Rubberband::vertexModel() const
{
  return mVertexModel;
}

void Rubberband::setVertexModel( VertexModel *vertexModel )
{
  if ( vertexModel )
    setModel( nullptr );

  if ( mVertexModel == vertexModel )
    return;

  if ( mVertexModel )
  {
    disconnect( mVertexModel, &VertexModel::dataChanged, this, &Rubberband::markDirty );
    disconnect( mVertexModel, &VertexModel::vertexCountChanged, this, &Rubberband::markDirty );
    disconnect( mVertexModel, &VertexModel::geometryChanged, this, &Rubberband::markDirty );
  }

  mVertexModel = vertexModel;

  if ( mVertexModel )
  {
    connect( mVertexModel, &VertexModel::dataChanged, this, &Rubberband::markDirty );
    connect( mVertexModel, &VertexModel::vertexCountChanged, this, &Rubberband::markDirty );
    connect( mVertexModel, &VertexModel::geometryChanged, this, &Rubberband::markDirty );
  }

  markDirty();

  emit vertexModelChanged();
}


QgsQuickMapSettings *Rubberband::mapSettings() const
{
  return mMapSettings;
}

void Rubberband::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &Rubberband::visibleExtentChanged );
  }

  mMapSettings = mapSettings;
  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &Rubberband::visibleExtentChanged );

  markDirty();

  emit mapSettingsChanged();
}

void Rubberband::visibleExtentChanged()
{
  mDirty = true;
  update();
}

void Rubberband::markDirty()
{
  mDirty = true;
  update();
}

void Rubberband::transformPoints( QVector<QgsPoint> & points )
{
  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  const double scaleFactor = 1.0 / mMapSettings->mapUnitsPerPoint();

  for( QgsPoint &point : points )
  {
    point.setX( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor );
    point.setY( ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor );
  }
}

QSGNode *Rubberband::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty )
  {
    delete n;
    n = new QSGNode;

    bool frozen = mRubberbandModel && mRubberbandModel->frozen();

    QVector<QgsPoint> allVertices = QVector<QgsPoint>();
    QgsWkbTypes::GeometryType geomType = QgsWkbTypes::LineGeometry;

    if ( mRubberbandModel && !mRubberbandModel->isEmpty() )
    {
      allVertices = mRubberbandModel->flatVertices();
      geomType = mRubberbandModel->geometryType();
    }
    else if ( mVertexModel && mVertexModel->vertexCount() > 0 )
    {
      allVertices = mVertexModel->flatVertices();
      geomType = mVertexModel->geometryType();
    }

    if ( !allVertices.isEmpty() )
    {
      transformPoints( allVertices );

      SGRubberband *rb = new SGRubberband( allVertices, geomType, mColor, mWidth );
      rb->setFlag( QSGNode::OwnedByParent );
      n->appendChildNode( rb );

      if ( mRubberbandModel && !frozen )
      {
        QVector<QgsPoint> allButCurrentVertices = mRubberbandModel->flatVertices( true );
        transformPoints( allButCurrentVertices );

        SGRubberband *rbCurrentPoint = new SGRubberband( allButCurrentVertices, geomType, mColorCurrentPoint, mWidthCurrentPoint );
        rbCurrentPoint->setFlag( QSGNode::OwnedByParent );
        n->appendChildNode( rbCurrentPoint );
      }
    }
  }

  mDirty = false;
  return n;
}

qreal Rubberband::width() const
{
  return mWidth;
}

void Rubberband::setWidth( qreal width )
{
  if ( mWidth == width )
    return;

  mWidth = width;

  emit widthChanged();
}

QColor Rubberband::color() const
{
  return mColor;
}

void Rubberband::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;

  emit colorChanged();
}

qreal Rubberband::widthCurrentPoint() const
{
  return mWidthCurrentPoint;
}

void Rubberband::setWidthCurrentPoint( qreal width )
{
  if ( mWidthCurrentPoint == width )
    return;

  mWidthCurrentPoint = width;

  emit widthCurrentPointChanged();
}

QColor Rubberband::colorCurrentPoint() const
{
  return mColorCurrentPoint;
}

void Rubberband::setColorCurrentPoint( const QColor &color )
{
  if ( mColorCurrentPoint == color )
    return;

  mColorCurrentPoint = color;

  emit colorCurrentPointChanged();
}

