/***************************************************************************
                            qgsquickmapcanvasmap.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickmapcanvasmap.h"

#include "qgsmapcanvasmap.h"

QgsQuickMapCanvasMap::QgsQuickMapCanvasMap(  QQuickItem* parent )
  : QQuickPaintedItem( parent )
  , mPinching( false )
{
  mMapCanvas.reset( new QgsMapCanvas() );
  // We just use this widget to access the paint engine...
  mMapCanvas->setAttribute( Qt::WA_DontShowOnScreen );
  mMapCanvas->setParallelRenderingEnabled( true );

  setRenderTarget( QQuickPaintedItem::FramebufferObject );
  connect( mapCanvas()->scene(), SIGNAL( changed( QList<QRectF> ) ), this, SLOT( update() ) );
}

QgsMapCanvas* QgsQuickMapCanvasMap::mapCanvas()
{
  return mMapCanvas.data();
}

void QgsQuickMapCanvasMap::paint( QPainter* painter )
{
  mMapCanvas->render( painter );
}

void QgsQuickMapCanvasMap::setParallelRendering( bool pr )
{
  if ( pr != mMapCanvas->isParallelRenderingEnabled() )
  {
    mMapCanvas->setParallelRenderingEnabled( true );
    emit parallelRenderingChanged();
  }
}

bool QgsQuickMapCanvasMap::parallelRendering()
{
  return mMapCanvas->isParallelRenderingEnabled();
}

QgsPoint QgsQuickMapCanvasMap::toMapCoordinates( QPoint canvasCoordinates )
{
  return mMapCanvas->getCoordinateTransform()->toMapPoint( canvasCoordinates.x(), canvasCoordinates.y() );
}

void QgsQuickMapCanvasMap::setMapSettings( const QVariant& mapSettings )
{
  // mMapCanvas->setExtent( mapSettings.value<QgsMapSettings>().extent() );
}

const QVariant QgsQuickMapCanvasMap::mapSettings()
{
  return QVariant::fromValue<QgsMapSettings>( mMapCanvas->mapSettings() );
}

void QgsQuickMapCanvasMap::zoom( QPointF center, qreal scale )
{
  QgsPoint oldCenter( mMapCanvas->mapSettings().visibleExtent().center() );
  QgsPoint mousePos( mMapCanvas->getCoordinateTransform()->toMapPoint( center.x(), center.y() ) );
  QgsPoint newCenter( mousePos.x() + ( ( oldCenter.x() - mousePos.x() ) * scale ),
                      mousePos.y() + ( ( oldCenter.y() - mousePos.y() ) * scale ) );

  // same as zoomWithCenter (no coordinate transformations are needed)
  QgsRectangle extent = mMapCanvas->mapSettings().visibleExtent();
  extent.scale( scale, &newCenter );
  mMapCanvas->setExtent( extent );

  emit mapSettingsChanged();

  update();
}

void QgsQuickMapCanvasMap::pan( QPointF oldPos, QPointF newPos )
{

  QgsPoint start = mMapCanvas->getCoordinateTransform()->toMapCoordinates( oldPos.toPoint() );
  QgsPoint end = mMapCanvas->getCoordinateTransform()->toMapCoordinates( newPos.toPoint() );

  double dx = qAbs( end.x() - start.x() );
  double dy = qAbs( end.y() - start.y() );

  // modify the extent
  QgsRectangle r = mMapCanvas->mapSettings().visibleExtent();

  if ( end.x() > start.x() )
  {
    r.setXMinimum( r.xMinimum() + dx );
    r.setXMaximum( r.xMaximum() + dx );
  }
  else
  {
    r.setXMinimum( r.xMinimum() - dx );
    r.setXMaximum( r.xMaximum() - dx );
  }

  if ( end.y() > start.y() )
  {
    r.setYMaximum( r.yMaximum() + dy );
    r.setYMinimum( r.yMinimum() + dy );

  }
  else
  {
    r.setYMaximum( r.yMaximum() - dy );
    r.setYMinimum( r.yMinimum() - dy );

  }

  mMapCanvas->setExtent( r );

  emit mapSettingsChanged();

  update();
}

void QgsQuickMapCanvasMap::refresh()
{
  mMapCanvas->refresh();
}

void QgsQuickMapCanvasMap::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry )
{
  mMapCanvas->resize( newGeometry.toRect().width()+1, newGeometry.toRect().height() + 1 );
  QQuickPaintedItem::geometryChanged( newGeometry, oldGeometry );

  // Todo: is this the correct way to deal with this?
  emit mapSettingsChanged();
}
