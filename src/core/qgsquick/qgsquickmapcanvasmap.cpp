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

#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"

#include <QQuickWindow>
#include <QSGSimpleTextureNode>
#include <QScreen>
#include <qgis.h>
#include <qgsannotationlayer.h>
#include <qgsexpressioncontextutils.h>
#include <qgslabelingresults.h>
#include <qgsmaplayertemporalproperties.h>
#include <qgsmaprenderercache.h>
#include <qgsmaprendererparalleljob.h>
#include <qgsmaprenderersequentialjob.h>
#include <qgsmessagelog.h>
#include <qgspallabeling.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>


QgsQuickMapCanvasMap::QgsQuickMapCanvasMap( QQuickItem *parent )
  : QQuickItem( parent )
  , mMapSettings( std::make_unique<QgsQuickMapSettings>() )
  , mCache( std::make_unique<QgsMapRendererCache>() )
{
  connect( this, &QQuickItem::windowChanged, this, &QgsQuickMapCanvasMap::onWindowChanged );
  connect( &mRefreshTimer, &QTimer::timeout, this, [=] { refreshMap(); } );
  connect( &mMapUpdateTimer, &QTimer::timeout, this, &QgsQuickMapCanvasMap::renderJobUpdated );

  connect( mMapSettings.get(), &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapCanvasMap::onExtentChanged );
  connect( mMapSettings.get(), &QgsQuickMapSettings::rotationChanged, this, &QgsQuickMapCanvasMap::onRotationChanged );
  connect( mMapSettings.get(), &QgsQuickMapSettings::layersChanged, this, &QgsQuickMapCanvasMap::onLayersChanged );
  connect( mMapSettings.get(), &QgsQuickMapSettings::temporalStateChanged, this, &QgsQuickMapCanvasMap::onTemporalStateChanged );

  connect( this, &QgsQuickMapCanvasMap::renderStarting, this, &QgsQuickMapCanvasMap::isRenderingChanged );
  connect( this, &QgsQuickMapCanvasMap::mapCanvasRefreshed, this, &QgsQuickMapCanvasMap::isRenderingChanged );

  mMapUpdateTimer.setSingleShot( false );
  mMapUpdateTimer.setInterval( 250 );
  mRefreshTimer.setSingleShot( true );
  setTransformOrigin( QQuickItem::Center );
  setFlags( QQuickItem::ItemHasContents );
}

QgsQuickMapCanvasMap::~QgsQuickMapCanvasMap()
{
  stopRendering();
}

QgsQuickMapSettings *QgsQuickMapCanvasMap::mapSettings() const
{
  return mMapSettings.get();
}

void QgsQuickMapCanvasMap::rotate( double degrees )
{
  mMapSettings->setRotation( mMapSettings->rotation() + degrees );
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
  stopRendering(); // if any...

  QgsMapSettings mapSettings = mMapSettings->mapSettings();
  if ( !mapSettings.hasValidSettings() )
    return;

  if ( !qgsDoubleNear( mQuality, 1.0 ) )
  {
    mapSettings.setOutputSize( mapSettings.outputSize() * mQuality );
    mapSettings.setOutputDpi( mapSettings.outputDpi() * mQuality );
  }

  //build the expression context
  QgsExpressionContext expressionContext;
  expressionContext << QgsExpressionContextUtils::globalScope()
                    << QgsExpressionContextUtils::mapSettingsScope( mapSettings );

  QgsProject *project = mMapSettings->project();
  if ( project )
  {
    expressionContext << QgsExpressionContextUtils::projectScope( project );

    mapSettings.setLabelingEngineSettings( project->labelingEngineSettings() );

    // render main annotation layer above all other layers
    QList<QgsMapLayer *> allLayers = mapSettings.layers();
    allLayers.insert( 0, project->mainAnnotationLayer() );
    mapSettings.setLayers( allLayers );
  }

  mapSettings.setExpressionContext( expressionContext );

  // enables on-the-fly simplification of geometries to spend less time rendering
  mapSettings.setFlag( Qgis::MapSettingsFlag::UseRenderingOptimization );
  // with incremental rendering - enables updates of partially rendered layers (good for WMTS, XYZ layers)
  mapSettings.setFlag( Qgis::MapSettingsFlag::RenderPartialOutput, mIncrementalRendering );

  // create the renderer job
  Q_ASSERT( !mJob );
  mJob = new QgsMapRendererParallelJob( mapSettings );

  if ( mIncrementalRendering )
    mMapUpdateTimer.start();

  connect( mJob, &QgsMapRendererJob::renderingLayersFinished, this, &QgsQuickMapCanvasMap::renderJobUpdated );
  connect( mJob, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::renderJobFinished );
  mJob->setCache( mCache.get() );
  mJob->setLayerRenderingTimeHints( mLastLayerRenderTime );

  mJob->start();

  if ( !mSilentRefresh )
  {
    emit renderStarting();
  }
}

void QgsQuickMapCanvasMap::renderJobUpdated()
{
  if ( !mJob )
    return;

  mImage = mJob->renderedImage();
  mImageMapSettings = mJob->mapSettings();
  mDirty = true;
  // Temporarily freeze the canvas, we only need to reset the geometry but not trigger a repaint
  bool freeze = mFreeze;
  mFreeze = true;
  updateTransform();
  mFreeze = freeze;

  update();
}

void QgsQuickMapCanvasMap::renderJobFinished()
{
  if ( !mJob )
    return;

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
  if ( !mSilentRefresh )
  {
    emit mapCanvasRefreshed();
  }
  else
  {
    mSilentRefresh = false;
  }

  if ( mDeferredRefreshPending )
  {
    mDeferredRefreshPending = false;
    mSilentRefresh = true;
    refresh();
  }
  else if ( mPreviewJobsEnabled )
  {
    startPreviewJobs();
  }
}

void QgsQuickMapCanvasMap::layerRepaintRequested( bool deferred )
{
  if ( mMapSettings->outputSize().isNull() )
    return; // the map image size has not been set yet

  if ( !mFreeze )
  {
    if ( deferred || mForceDeferredLayersRepaint )
    {
      if ( !mJob && !mRefreshTimer.isActive() )
      {
        mSilentRefresh = true;
        refresh();
      }
      else
      {
        mDeferredRefreshPending = true;
      }
    }
    else
    {
      refresh();
    }
  }
}

void QgsQuickMapCanvasMap::onWindowChanged( QQuickWindow *window )
{
  if ( mWindow == window )
    return;

  if ( mWindow )
    disconnect( mWindow, &QQuickWindow::screenChanged, this, &QgsQuickMapCanvasMap::onScreenChanged );

  if ( window )
  {
    connect( window, &QQuickWindow::screenChanged, this, &QgsQuickMapCanvasMap::onScreenChanged );
    onScreenChanged( window->screen() );
  }

  mWindow = window;
}

void QgsQuickMapCanvasMap::onScreenChanged( QScreen *screen )
{
  if ( screen )
  {
    if ( screen->devicePixelRatio() > 0 )
    {
      mMapSettings->setDevicePixelRatio( screen->devicePixelRatio() );
    }
    mMapSettings->setOutputDpi( screen->physicalDotsPerInch() );
  }
}

void QgsQuickMapCanvasMap::onExtentChanged()
{
  updateTransform();

  // And trigger a new rendering job
  refresh();
}

void QgsQuickMapCanvasMap::onRotationChanged()
{
  updateTransform();

  // And trigger a new rendering job
  refresh();
}

void QgsQuickMapCanvasMap::onTemporalStateChanged()
{
  clearTemporalCache();

  // And trigger a new rendering job
  refresh();
}
void QgsQuickMapCanvasMap::updateTransform()
{
  QgsRectangle imageExtent = mImageMapSettings.extent();
  QgsRectangle newExtent = mMapSettings->mapSettings().extent();

  setScale( static_cast<double>( imageExtent.width() ) / newExtent.width() );
  setRotation( mMapSettings->mapSettings().rotation() - mImageMapSettings.rotation() );

  QgsPointXY center = imageExtent.center();
  QgsPointXY pixelPt = mMapSettings->coordinateToScreen( QgsPoint( center.x(), center.y() ) );
  setX( pixelPt.x() - static_cast<qreal>( mMapSettings->outputSize().width() ) / mMapSettings->devicePixelRatio() / 2 );
  setY( pixelPt.y() - static_cast<qreal>( mMapSettings->outputSize().height() ) / mMapSettings->devicePixelRatio() / 2 );
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

double QgsQuickMapCanvasMap::quality() const
{
  return mQuality;
}

void QgsQuickMapCanvasMap::setQuality( double quality )
{
  quality = std::clamp( quality, 0.5, 1.0 );
  if ( mQuality == quality )
    return;

  mQuality = quality;

  emit qualityChanged();

  // And trigger a new rendering job
  refresh();
}

double QgsQuickMapCanvasMap::bottomMargin() const
{
  return mMapSettings->bottomMargin();
}

void QgsQuickMapCanvasMap::setBottomMargin( double bottomMargin )
{
  if ( mMapSettings->bottomMargin() == bottomMargin )
    return;

  mMapSettings->setBottomMargin( bottomMargin );
  emit bottomMarginChanged();
}

double QgsQuickMapCanvasMap::rightMargin() const
{
  return mMapSettings->rightMargin();
}

void QgsQuickMapCanvasMap::setRightMargin( double rightMargin )
{
  if ( mMapSettings->rightMargin() == rightMargin )
    return;

  mMapSettings->setRightMargin( rightMargin );
  emit rightMarginChanged();
}

bool QgsQuickMapCanvasMap::forceDeferredLayersRepaint() const
{
  return mForceDeferredLayersRepaint;
}

void QgsQuickMapCanvasMap::setForceDeferredLayersRepaint( bool deferred )
{
  if ( mForceDeferredLayersRepaint == deferred )
    return;

  mForceDeferredLayersRepaint = deferred;
  emit forceDeferredLayersRepaintChanged();
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
  {
    refresh();
  }

  emit freezeChanged();
}

bool QgsQuickMapCanvasMap::isRendering() const
{
  return mJob;
}

QSGNode *QgsQuickMapCanvasMap::updatePaintNode( QSGNode *oldNode, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty )
  {
    delete oldNode;
    oldNode = nullptr;
    mDirty = false;
  }

  if ( mImage.isNull() )
  {
    return nullptr;
  }

  QSGSimpleTextureNode *node = static_cast<QSGSimpleTextureNode *>( oldNode );
  if ( !node )
  {
    node = new QSGSimpleTextureNode();
    node->setFiltering( QSGTexture::Linear );
    QSGTexture *texture = window()->createTextureFromImage( mImage );
    node->setTexture( texture );
    node->setOwnsTexture( true );
  }

  QRectF rect( boundingRect() );
  QSizeF size = mImage.size();
  if ( !size.isEmpty() )
    size /= mMapSettings->devicePixelRatio();

  // Check for resizes that change the w/h ratio
  if ( !rect.isEmpty() && !size.isEmpty() && !qgsDoubleNear( rect.width() / rect.height(), ( size.width() ) / static_cast<double>( size.height() ), 3 ) )
  {
    if ( qgsDoubleNear( rect.height(), mImage.height() ) )
    {
      rect.setHeight( rect.width() / size.width() * size.height() );
    }
    else
    {
      rect.setWidth( rect.height() / size.height() * size.width() );
    }
  }

  node->setRect( rect );
  node->removeAllChildNodes();
  for ( auto [number, previewImage] : mPreviewImages.asKeyValueRange() )
  {
    QSGSimpleTextureNode *childNode = new QSGSimpleTextureNode();
    childNode->setFiltering( QSGTexture::Linear );
    QSGTexture *texture = window()->createTextureFromImage( previewImage );
    childNode->setTexture( texture );
    childNode->setOwnsTexture( true );

    QRectF childRect( rect );
    // Adjust left/right
    if ( number == 0 || number == 3 || number == 6 )
    {
      childRect.setLeft( rect.left() - rect.width() );
      childRect.setRight( rect.right() - rect.width() );
    }
    else if ( number == 2 || number == 5 || number == 8 )
    {
      childRect.setLeft( rect.left() + rect.width() );
      childRect.setRight( rect.right() + rect.width() );
    }
    //Adjust top/bottom
    if ( number < 3 )
    {
      childRect.setTop( rect.top() - rect.height() );
      childRect.setBottom( rect.bottom() - rect.height() );
    }
    else if ( number > 5 )
    {
      childRect.setTop( rect.top() + rect.height() );
      childRect.setBottom( rect.bottom() + rect.height() );
    }
    childNode->setRect( childRect );

    node->appendChildNode( childNode );
  }

  return node;
}

void QgsQuickMapCanvasMap::geometryChange( const QRectF &newGeometry, const QRectF &oldGeometry )
{
  QQuickItem::geometryChange( newGeometry, oldGeometry );
  if ( newGeometry.size() != oldGeometry.size() )
  {
    mMapSettings->setOutputSize( newGeometry.size().toSize() );
    refresh();
  }
}

void QgsQuickMapCanvasMap::onLayersChanged()
{
  if ( mMapSettings->extent().isEmpty() )
    zoomToFullExtent();

  for ( const QMetaObject::Connection &conn : std::as_const( mLayerConnections ) )
  {
    disconnect( conn );
  }
  mLayerConnections.clear();

  const QList<QgsMapLayer *> layers = mMapSettings->layers();
  for ( QgsMapLayer *layer : layers )
  {
    mLayerConnections << connect( layer, &QgsMapLayer::repaintRequested, this, &QgsQuickMapCanvasMap::layerRepaintRequested );
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
    mMapUpdateTimer.stop();

    disconnect( mJob, &QgsMapRendererJob::renderingLayersFinished, this, &QgsQuickMapCanvasMap::renderJobUpdated );
    disconnect( mJob, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::renderJobFinished );

    if ( !mJob->isActive() )
      mJob->deleteLater();
    else
      connect( mJob, &QgsMapRendererJob::finished, mJob, &QObject::deleteLater );
    mJob->cancelWithoutBlocking();
    mJob = nullptr;
  }
  stopPreviewJobs();
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
      try
      {
        extent.combineExtentWith( transform.transformBoundingBox( layer->extent() ) );
      }
      catch ( const QgsCsException & )
      {
        // Ignore extent if it can't be transformed
      }
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
  if ( mMapSettings->outputSize().isNull() )
    return; // the map image size has not been set yet

  if ( !mFreeze )
    mRefreshTimer.start( 1 );
}

void QgsQuickMapCanvasMap::clearTemporalCache()
{
  if ( mCache )
  {
    bool invalidateLabels = false;
    const QList<QgsMapLayer *> layerList = mMapSettings->mapSettings().layers();
    for ( QgsMapLayer *layer : layerList )
    {
      if ( layer->temporalProperties() && layer->temporalProperties()->isActive() )
      {
        if ( QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer ) )
        {
          if ( vl->labelsEnabled() || vl->diagramsEnabled() )
            invalidateLabels = true;
        }

        if ( layer->temporalProperties()->flags() & QgsTemporalProperty::FlagDontInvalidateCachedRendersWhenRangeChanges )
          continue;

        mCache->invalidateCacheForLayer( layer );
      }
    }

    if ( invalidateLabels )
    {
      mCache->clearCacheImage( QStringLiteral( "_labels_" ) );
      mCache->clearCacheImage( QStringLiteral( "_preview_labels_" ) );
    }
  }
}

QList<QgsMapLayer *> filterLayersForRender( const QList<QgsMapLayer *> &layers )
{
  QList<QgsMapLayer *> filteredLayers;
  for ( QgsMapLayer *layer : layers )
  {
    if ( QgsAnnotationLayer *annotationLayer = qobject_cast<QgsAnnotationLayer *>( layer ) )
    {
      if ( QgsMapLayer *linkedLayer = annotationLayer->linkedVisibilityLayer() )
      {
        if ( !layers.contains( linkedLayer ) )
          continue;
      }
    }
    filteredLayers.append( layer );
  }
  return filteredLayers;
}

bool QgsQuickMapCanvasMap::previewJobsEnabled() const
{
  return mPreviewJobsEnabled;
}

void QgsQuickMapCanvasMap::setPreviewJobsEnabled( bool enabled )
{
  if ( mPreviewJobsEnabled == enabled )
    return;

  mPreviewJobsEnabled = enabled;
  emit previewJobsEnabledChanged();

  if ( !mPreviewJobsEnabled )
  {
    // Clear previously stored preview images
    mPreviewImages.clear();
  }
}

void QgsQuickMapCanvasMap::startPreviewJobs()
{
  stopPreviewJobs();

  if ( mImage.isNull() )
  {
    return;
  }

  schedulePreviewJob( 0 );
}

void QgsQuickMapCanvasMap::startPreviewJob( int number )
{
  if ( number == 4 )
    number += 1;

  int j = number / 3;
  int i = number % 3;

  QgsMapSettings mapSettings = mImageMapSettings;
  mapSettings.setRotation( 0 );
  const QgsRectangle mapRect = mapSettings.visibleExtent();
  QgsPointXY jobCenter = mapRect.center();
  const double dx = ( i - 1 ) * mapRect.width();
  const double dy = ( 1 - j ) * mapRect.height();
  if ( !qgsDoubleNear( mImageMapSettings.rotation(), 0.0 ) )
  {
    const double radians = mImageMapSettings.rotation() * M_PI / 180;
    const double rdx = dx * cos( radians ) - dy * sin( radians );
    const double rdy = dy * cos( radians ) + dx * sin( radians );
    jobCenter.setX( jobCenter.x() + rdx );
    jobCenter.setY( jobCenter.y() + rdy );
  }
  else
  {
    jobCenter.setX( jobCenter.x() + dx );
    jobCenter.setY( jobCenter.y() + dy );
  }
  const QgsRectangle jobExtent = QgsRectangle::fromCenterAndSize( jobCenter, mapRect.width(), mapRect.height() );

  //copy settings, only update extent
  QgsMapSettings jobSettings = mImageMapSettings;
  jobSettings.setExtent( jobExtent );

  jobSettings.setFlag( Qgis::MapSettingsFlag::DrawLabeling, false );
  jobSettings.setFlag( Qgis::MapSettingsFlag::RenderPreviewJob, true );
  jobSettings.setFlag( Qgis::MapSettingsFlag::RecordProfile, false );

  // truncate preview layers to fast layers
  const QList<QgsMapLayer *> layers = jobSettings.layers();
  QList<QgsMapLayer *> previewLayers;
  QgsDataProvider::PreviewContext context;
  context.maxRenderingTimeMs = MAXIMUM_LAYER_PREVIEW_TIME_MS;
  for ( QgsMapLayer *layer : layers )
  {
    if ( layer->customProperty( QStringLiteral( "rendering/noPreviewJobs" ), false ).toBool() )
    {
      QgsDebugMsgLevel( QStringLiteral( "Layer %1 not rendered because it is explicitly blocked from preview jobs" ).arg( layer->id() ), 3 );
      continue;
    }
    context.lastRenderingTimeMs = mLastLayerRenderTime.value( layer->id(), 0 );
    QgsDataProvider *provider = layer->dataProvider();
    if ( provider && !provider->renderInPreview( context ) )
    {
      QgsDebugMsgLevel( QStringLiteral( "Layer %1 not rendered because it does not match the renderInPreview criterion %2" ).arg( layer->id() ).arg( mLastLayerRenderTime.value( layer->id() ) ), 3 );
      continue;
    }

    previewLayers << layer;
  }
  if ( QgsProject::instance()->mainAnnotationLayer()->dataProvider()->renderInPreview( context ) )
  {
    previewLayers.insert( 0, QgsProject::instance()->mainAnnotationLayer() );
  }
  jobSettings.setLayers( filterLayersForRender( previewLayers ) );

  QgsMapRendererQImageJob *job = new QgsMapRendererSequentialJob( jobSettings );
  job->setProperty( "number", number );
  mPreviewJobs.append( job );
  connect( job, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::previewJobFinished );
  job->start();
}

void QgsQuickMapCanvasMap::stopPreviewJobs()
{
  mPreviewTimer.stop();
  for ( auto previewJob = mPreviewJobs.constBegin(); previewJob != mPreviewJobs.constEnd(); ++previewJob )
  {
    if ( *previewJob )
    {
      disconnect( *previewJob, &QgsMapRendererJob::finished, this, &QgsQuickMapCanvasMap::previewJobFinished );
      connect( *previewJob, &QgsMapRendererQImageJob::finished, *previewJob, &QgsMapRendererQImageJob::deleteLater );
      ( *previewJob )->cancelWithoutBlocking();
    }
  }
  mPreviewJobs.clear();
  mPreviewImages.clear();
}

void QgsQuickMapCanvasMap::schedulePreviewJob( int number )
{
  mPreviewTimer.setSingleShot( true );
  mPreviewTimer.setInterval( PREVIEW_JOB_DELAY_MS );
  disconnect( mPreviewTimerConnection );
  mPreviewTimerConnection = connect( &mPreviewTimer, &QTimer::timeout, this, [=]() {
    startPreviewJob( number );
  } );
  mPreviewTimer.start();
}

void QgsQuickMapCanvasMap::previewJobFinished()
{
  QgsMapRendererQImageJob *job = qobject_cast<QgsMapRendererQImageJob *>( sender() );
  Q_ASSERT( job );

  const int number = job->property( "number" ).toInt();
  mPreviewImages.insert( number, job->renderedImage() );
  mPreviewJobs.removeAll( job );
  if ( number < 8 )
  {
    startPreviewJob( number + 1 );
  }
  delete job;

  mDirty = true;
  update();
}
