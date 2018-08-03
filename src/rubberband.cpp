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

#include "rubberband.h"

#include "rubberbandmodel.h"
#include "sgrubberband.h"

Rubberband::Rubberband( QQuickItem* parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

RubberbandModel* Rubberband::model() const
{
  return mModel;
}

void Rubberband::setModel( RubberbandModel* model )
{
  if ( mModel == model )
    return;

  mModel = model;

  // TODO connect to changes

  connect( mModel, &RubberbandModel::vertexChanged, this, &Rubberband::markDirty );
  connect( mModel, &RubberbandModel::verticesRemoved, this, &Rubberband::markDirty );
  connect( mModel, &RubberbandModel::verticesInserted, this, &Rubberband::markDirty );

  markDirty();

  emit modelChanged();
}

MapSettings* Rubberband::mapSettings() const
{
  return mMapSettings;
}

void Rubberband::setMapSettings( MapSettings* mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;
  markDirty();

  emit mapSettingsChanged();
}

void Rubberband::markDirty()
{
  mDirty = true;
  update();
}

QSGNode* Rubberband::updatePaintNode( QSGNode* n, QQuickItem::UpdatePaintNodeData* )
{
  if ( mDirty && mModel )
  {
    delete n;
    n = new QSGNode;

    if ( !mModel->isEmpty() )
    {
      SGRubberband* rb = new SGRubberband( mModel->flatVertices(), mModel->geometryType(), mColor, mWidth );
      rb->setFlag( QSGNode::OwnedByParent );
      n->appendChildNode( rb );

      if ( !mModel->frozen() )
      {
        SGRubberband* rbCurrentPoint = new SGRubberband( mModel->flatVertices(true), mModel->geometryType(), mColor, mWidth );
        rbCurrentPoint->setFlag( QSGNode::OwnedByParent );
        n->appendChildNode( rbCurrentPoint );
      }
    }
  }

  return n;
}

qreal Rubberband::width() const
{
  return mWidth;
}

void Rubberband::setWidth( qreal width )
{
  mWidth = width;
}

QColor Rubberband::color() const
{
  return mColor;
}

void Rubberband::setColor( const QColor& color )
{
  mColor = color;
}


