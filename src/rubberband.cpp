/***************************************************************************
  rubberband.cpp - Rubberband

 ---------------------
 begin                : 11.6.2016
 copyright            : (C) 2016 by mku
 email                : [your-email-here]
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
  , mModel ( nullptr )
  , mDirty ( false )
{
  setFlags( QQuickItem::ItemHasContents );
  // setAntialiasing( true ); // TODO : Check what this does
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

  connect( mModel, SIGNAL( vertexChanged( int ) ), this, SLOT( markDirty() ) );
  connect( mModel, SIGNAL( verticesRemoved( int,int ) ), this, SLOT( markDirty() ) );
  connect( mModel, SIGNAL( verticesInserted( int,int ) ), this, SLOT( markDirty() ) );

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
      SGRubberband* rb = new SGRubberband( mModel->flatVertices(), mModel->geometryType() );
      rb->setFlag( QSGNode::OwnedByParent );

      n->appendChildNode( rb );
    }
  }

  return n;
}
