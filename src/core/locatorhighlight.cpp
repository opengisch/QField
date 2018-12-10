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

#include <QTimer>

#include <qgsgeometry.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsproject.h>

#include "locatorhighlight.h"

#include "qgssggeometry.h"


LocatorHighlight::LocatorHighlight( QQuickItem *parent )
  : QQuickItem( parent )
  , mDirty( false )
  , mMapSettings( nullptr )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );

  mTimer = new QTimer( this );
  mTimer->setSingleShot( false );
  connect( mTimer, &QTimer::timeout, this, [ = ]() {mGeometry = QgsGeometry(); mDirty = true; update();} );
  mTimer->start( 3000 );
}

void LocatorHighlight::highlightGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs )
{
  mTimer->stop();
  mGeometry = geometry;
  if ( mMapSettings )
  {
    QgsCoordinateTransform ct( crs, mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
    mGeometry.transform( ct );
  }

  mDirty = true;
  update();
  mTimer->start( 2000 );
}

QSGNode *LocatorHighlight::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty && mMapSettings )
  {
    delete n;
    n = new QSGNode;

    QgsSGGeometry *gn = new QgsSGGeometry( mGeometry, mColor, mWidth );
    gn->setFlag( QSGNode::OwnedByParent );
    n->appendChildNode( gn );

    mDirty = false;
  }

  return n;
}
