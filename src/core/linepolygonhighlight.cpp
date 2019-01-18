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

#include "linepolygonhighlight.h"

#include "qgsgeometrywrapper.h"
#include "qgssggeometry.h"


LinePolygonHighlight::LinePolygonHighlight( QQuickItem *parent )
  : QQuickItem( parent )
  , mDirty( false )
  , mMapSettings( nullptr )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );

//  mTimer = new QTimer( this );
//  mTimer->setSingleShot( false );
//  connect( mTimer, &QTimer::timeout, this, [ = ]() {setGeometry(nullptr);} );
//  mTimer->start( 3000 );

  connect( this, &LinePolygonHighlight::qgsGeometryChanged, this, &LinePolygonHighlight::update );

}

void LinePolygonHighlight::highlightGeometry( QgsGeometryWrapper *geometry )
{
  mTimer->stop();
  update();
  mTimer->start( 2000 );
}

QSGNode *LinePolygonHighlight::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty && mMapSettings )
  {
    delete n;
    n = new QSGNode;

    QgsSGGeometry *gn = new QgsSGGeometry( mQgsGeometry, mColor, mWidth );
    gn->setFlag( QSGNode::OwnedByParent );
    n->appendChildNode( gn );

    mDirty = false;
  }

  return n;
}

QgsGeometryWrapper *LinePolygonHighlight::geometry() const
{
  return mGeometry;
}

void LinePolygonHighlight::setGeometry( QgsGeometryWrapper *geometry )
{
  if ( mGeometry == geometry )
    return;

  mGeometry->deleteLater();
  mGeometry = geometry;

  if ( geometry )
    mQgsGeometry = geometry->qgsGeometry();
  else
    mQgsGeometry = QgsGeometry();

  if ( mMapSettings && geometry )
  {
    QgsCoordinateTransform ct( geometry->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
    mQgsGeometry.transform( ct );
  }
  qDebug() << mQgsGeometry.asWkt();

  mDirty = true;
  emit qgsGeometryChanged();
}
