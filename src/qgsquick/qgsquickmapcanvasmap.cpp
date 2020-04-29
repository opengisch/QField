/***************************************************************************
  qgsquickmapcanvasmap.cpp
  --------------------------------------
  Date                 : 10.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QQuickWindow>
#include <QScreen>
#include <QSGSimpleTextureNode>

#include <qgsmaprendererparalleljob.h>
#include <qgsmessagelog.h>
#include <qgspallabeling.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsexpressioncontextutils.h>
#include <qgis.h>

#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"


QgsQuickMapCanvasMap::QgsQuickMapCanvasMap( QQuickItem *parent )
  : QQuickItem( parent )
  , mMapSettings( qgis::make_unique<QgsQuickMapSettings>() )
{
  mMapSettings->setOutputDpi( 96 );

  connect( this, &QQuickItem::windowChanged, this, &QgsQuickMapCanvasMap::onWindowChanged );
  connect( &mRefreshTimer, &QTimer::timeout, this, &QgsQuickMapCanvasMap::refreshMap );
  connect( &mMapUpdateTimer, &QTimer::timeout, this, &QgsQuickMapCanvasMap::renderJobUpdated );

  connect( mMapSettings.get(), &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapCanvasMap::onExtentChanged );
  connect( mMapSettings.get(), &QgsQuickMapSettings::layersChanged, this, &QgsQuickMapCanvasMap::onLayersChanged );

  connect( this, &QgsQuickMapCanvasMap::renderStarting, this, &QgsQuickMapCanvasMap::isRenderingChanged );
  connect( this, &QgsQuickMapCanvasMap::mapCanvasRefreshed, this, &QgsQuickMapCanvasMap::isRenderingChanged );

  mMapUpdateTimer.setSingleShot( false );
  mMapUpdateTimer.setInterval( 250 );
  mRefreshTimer.setSingleShot( true );
  setTransformOrigin( QQuickItem::TopLeft );
  setFlags( QQuickItem::ItemHasContents );
}

QgsQuickMapSettings *QgsQuickMapCanvasMap::mapSettings() const
{
  return mMapSettings.get();
}

void QgsQuickMapCanvasMap::zoom( QPointF center, qreal scale )
{
  QgsRectangle extent = mMapSettings->extent();
  QgsPoint oldCenter( extent.center() );
  QgsPoint mousePos( mMapSettings->screenToCoordinate( center ) );

  QgsPointXY newCenter( mousePos.x() + ( ( oldCenter.x() - mousePos.x() ) * scale ),
                        mousePos.y() + ( ( oldCenter.y() - mousePos.y() ) * scale ) );

  // same as zoomWithCenter (no coordinate transformations are needed)
  extent.scale( scale, &newCenter );
  mMapSettings->setExtent( extent );
}

void QgsQuickMapCanvasMap::pan( QPointF oldPos, QPointF newPos )
{
  qDebug() << "pan";
  QgsPoint start = mMapSettings->screenToCoordinate( oldPos.toPoint() );
  QgsPoint end = mMapSettings->screenToCoordinate( newPos.toPoint() );

  double dx = end.x() - start.x();
  double dy = end.y() - start.y();

  // modify the extent
  QgsRectangle extent = mMapSettings->extent();

  extent.setXMinimum( extent.xMinimum() + dx );
  extent.setXMaximum( extent.xMaximum() + dx );
  extent.setYMaximum( extent.yMaximum() + dy );
  extent.setYMinimum( extent.yMinimum() + dy );

  mMapSettings->setExtent( extent );
}

void QgsQuickMapCanvasMap::refreshMap()
{
  qDebug() << "refreshMap";
  stopRendering(); // if any...

  QgsMapSettings mapSettings = mMapSettings->mapSettings();

  //build the expression context
  QgsExpressionContext expressionContext;
  expressionContext << QgsExpressionContextUtils::globalScope()
                    << QgsExpressionContextUtils::mapSettingsScope( mapSettings );

  QgsProject *project = mMapSettings->project();
  if ( project )
  {
    expressionContext << QgsExpressionContextUtils::projectScope( project );
  }

  mapSettings.setExpressionContext( expressionContext );

  // create the renderer job
  Q_ASSERT( !mJob );
  mJob = new QgsMapRendererParallelJob( mapSettings );

  if ( mIncrementalRendering )
    mMapUpdateTimer.start();

  connect( mJob, &QgsMapRendererJob::renderingLayersFinished, this, &QgsQuickMapCanvasMap::renderJobUpdated );
  connect( mJob, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::renderJobFinished );
  mJob->setCache( mCache );

  mJob->start();

  emit renderStarting();
}

void QgsQuickMapCanvasMap::renderJobUpdated()
{
  mImage = mJob->renderedImage();
  mImageMapSettings = mJob->mapSettings();
  mDirty = true;
  // Temporarily freeze the canvas, we only need to reset the geometry but not trigger a repaint
  bool freeze = mFreeze;
  mFreeze = true;
  updateTransform();
  mFreeze = freeze;

  update();
  emit mapCanvasRefreshed();
}

void QgsQuickMapCanvasMap::renderJobFinished()
{
  const QgsMapRendererJob::Errors errors = mJob->errors();
  for ( const QgsMapRendererJob::Error &error : errors )
  {
    QgsMessageLog::logMessage( QStringLiteral( "%1 :: %2" ).arg( error.layerID, error.message ), tr( "Rendering" ) );
  }

  // take labeling results before emitting renderComplete, so labeling map tools
  // connected to signal work with correct results
  delete mLabelingResults;
  mLabelingResults = mJob->takeLabelingResults();

  mImage = mJob->renderedImage();
  mImageMapSettings = mJob->mapSettings();

  // now we are in a slot called from mJob - do not delete it immediately
  // so the class is still valid when the execution returns to the class
  mJob->deleteLater();
  mJob = nullptr;
  mDirty = true;
  mMapUpdateTimer.stop();

  // Temporarily freeze the canvas, we only need to reset the geometry but not trigger a repaint
  bool freeze = mFreeze;
  mFreeze = true;
  updateTransform();
  mFreeze = freeze;

  update();
  emit mapCanvasRefreshed();
}

void QgsQuickMapCanvasMap::onWindowChanged( QQuickWindow *window )
{
  qDebug() << "onWindowChanged";
  disconnect( window, &QQuickWindow::screenChanged, this, &QgsQuickMapCanvasMap::onScreenChanged );
  if ( window )
  {
    connect( window, &QQuickWindow::screenChanged, this, &QgsQuickMapCanvasMap::onScreenChanged );
    onScreenChanged( window->screen() );
  }
}

void QgsQuickMapCanvasMap::onScreenChanged( QScreen *screen )
{
  qDebug() << "onScreenChanged";
  if ( screen )
  {
    if ( screen->devicePixelRatio() > 0 )
    {
      qDebug() << QString( "new devicePixelRatio %1" ).arg( screen->devicePixelRatio() );
      mMapSettings->setDevicePixelRatio( screen->devicePixelRatio() );
    }
    mMapSettings->setOutputDpi( screen->physicalDotsPerInch() );
  }
}

void QgsQuickMapCanvasMap::onExtentChanged()
{
  qDebug() << "onExtentChanged";
  updateTransform();

  // And trigger a new rendering job
  refresh();
}

void QgsQuickMapCanvasMap::updateTransform()
{
  qDebug() << "updateTransform";
  QgsRectangle imageExtent = mImageMapSettings.visibleExtent();
  QgsRectangle newExtent = mMapSettings->mapSettings().visibleExtent();
  qDebug() << QString( "scale %1" ).arg( imageExtent.width() / newExtent.width() );
  setScale( imageExtent.width() / newExtent.width() );

  QgsPointXY pixelPt = mMapSettings->coordinateToScreen( QgsPoint( imageExtent.xMinimum(), imageExtent.yMaximum() ) );
  qDebug() << QString( "x %1 y %2" ).arg( pixelPt.x() ).arg( pixelPt.y() );
  setX( pixelPt.x() );
  setY( pixelPt.y() );
}

int QgsQuickMapCanvasMap::mapUpdateInterval() const
{
  return mMapUpdateTimer.interval();
}

void QgsQuickMapCanvasMap::setMapUpdateInterval( int mapUpdateInterval )
{
  if ( mMapUpdateTimer.interval() == mapUpdateInterval )
    return;

  mMapUpdateTimer.setInterval( mapUpdateInterval );

  emit mapUpdateIntervalChanged();
}

bool QgsQuickMapCanvasMap::incrementalRendering() const
{
  return mIncrementalRendering;
}

void QgsQuickMapCanvasMap::setIncrementalRendering( bool incrementalRendering )
{
  if ( incrementalRendering == mIncrementalRendering )
    return;

  mIncrementalRendering = incrementalRendering;
  emit incrementalRenderingChanged();
}

bool QgsQuickMapCanvasMap::freeze() const
{
  return mFreeze;
}

void QgsQuickMapCanvasMap::setFreeze( bool freeze )
{
  if ( freeze == mFreeze )
    return;

  mFreeze = freeze;

  if ( !mFreeze )
    refresh();

  emit freezeChanged();
}

bool QgsQuickMapCanvasMap::isRendering() const
{
  return mJob;
}

QSGNode *QgsQuickMapCanvasMap::updatePaintNode( QSGNode *oldNode, QQuickItem::UpdatePaintNodeData * )
{
  qDebug() << "updatePaintNode";
  if ( mDirty )
  {
    qDebug() << "delete old";
    delete oldNode;
    qDebug() << "null old";
    oldNode = nullptr;
    mDirty = false;
  }

  qDebug() << "cast old node";
  QSGTexture *texture;
  QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>( oldNode );
  qDebug() << "casted old node";
  if ( !node )
  {
    qDebug() << "create node";
    node = new QSGSimpleTextureNode();
    qDebug() << "create texture from image";
    texture = window()->createTextureFromImage( mImage );
    qDebug() << "set node texture";
    node->setTexture( texture );
    qDebug() << "set node owns texture";
    node->setOwnsTexture( true );
  }

  qDebug() << "bounding rect";
  QRectF rect( boundingRect() );
  qDebug() << QString( "image size width %1 height %2" ).arg( mImage.size().width() ).arg( mImage.size().height() );
  QSizeF size = mImage.size();
  if ( !size.isEmpty() )
    size /= mMapSettings->devicePixelRatio();

  // Check for resizes that change the w/h ratio
  if ( !rect.isEmpty() &&
       !size.isEmpty() &&
       !qgsDoubleNear( rect.width() / rect.height(), ( size.width() ) / static_cast<double>( size.height() ), 3 ) )
  {
    qDebug() << "in if";
    if ( qgsDoubleNear( rect.height(), mImage.height() ) )
    {
      qDebug() << "height equal";
      rect.setHeight( rect.width() / size.width() * size.height() );
    }
    else
    {
      qDebug() << "width equal";
      rect.setWidth( rect.height() / size.height() * size.width() );
    }
  }

  qDebug() << QString( "set node rect x %1 y %2 width %3 height %4" ).arg( rect.x() ).arg( rect.y() ).arg( rect.width() ).arg( rect.height() );
  node->setRect( rect );

  qDebug() << "return node";
  return node;
}

void QgsQuickMapCanvasMap::geometryChanged( const QRectF &newGeometry, const QRectF &oldGeometry )
{
  qDebug() << "geometryChanged";
  // The Qt documentation advices to call the base method here.
  // However, this introduces instabilities and heavy performance impacts on Android.
  // It seems on desktop disabling it prevents us from downsizing the window...
  // Be careful when re-enabling it.
  QQuickItem::geometryChanged( newGeometry, oldGeometry );
  if ( newGeometry.size() != oldGeometry.size() )
  {
    qDebug() << QString( "new geometry width %1 height %2" ).arg( newGeometry.size().width() ).arg( newGeometry.size().height() );
    mMapSettings->setOutputSize( newGeometry.size().toSize() );
    refresh();
  }
}

void QgsQuickMapCanvasMap::onLayersChanged()
{
  if ( mMapSettings->extent().isEmpty() )
    zoomToFullExtent();

  for ( const QMetaObject::Connection &conn : qgis::as_const( mLayerConnections ) )
  {
    disconnect( conn );
  }
  mLayerConnections.clear();

  const QList<QgsMapLayer *> layers = mMapSettings->layers();
  for ( QgsMapLayer *layer : layers )
  {
    mLayerConnections << connect( layer, &QgsMapLayer::repaintRequested, this, &QgsQuickMapCanvasMap::refresh );
  }

  refresh();
}

void QgsQuickMapCanvasMap::destroyJob( QgsMapRendererJob *job )
{
  job->cancel();
  job->deleteLater();
}

void QgsQuickMapCanvasMap::stopRendering()
{
  if ( mJob )
  {
    disconnect( mJob, &QgsMapRendererJob::renderingLayersFinished, this, &QgsQuickMapCanvasMap::renderJobUpdated );
    disconnect( mJob, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::renderJobFinished );

    mJob->cancelWithoutBlocking();
    mJob = nullptr;
  }
}

void QgsQuickMapCanvasMap::zoomToFullExtent()
{
  QgsRectangle extent;
  const QList<QgsMapLayer *> layers = mMapSettings->layers();
  for ( QgsMapLayer *layer : layers )
  {
    if ( mMapSettings->destinationCrs() != layer->crs() )
    {
      QgsCoordinateTransform transform( layer->crs(), mMapSettings->destinationCrs(), mMapSettings->transformContext() );
      extent.combineExtentWith( transform.transformBoundingBox( layer->extent() ) );
    }
    else
    {
      extent.combineExtentWith( layer->extent() );
    }
  }
  mMapSettings->setExtent( extent );

  refresh();
}

void QgsQuickMapCanvasMap::refresh()
{
  qDebug() << "refresh";
  if ( mMapSettings->outputSize().isNull() )
    return;  // the map image size has not been set yet

  if ( !mFreeze )
    mRefreshTimer.start( 1 );
}
