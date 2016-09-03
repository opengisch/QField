/***************************************************************************
                            qgsquickmapcanvasmap.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
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
#include "mapsettings.h"

#include <qgsmaprendererparalleljob.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmessagelog.h>
#include <QQuickWindow>
#include <QScreen>
#include <qgspallabeling.h>
#include <QSGSimpleTextureNode>

QgsQuickMapCanvasMap::QgsQuickMapCanvasMap(  QQuickItem* parent )
  : QQuickItem( parent )
  , mMapSettings( new MapSettings() )
  , mPinching( false )
  , mJobCancelled( false )
  , mJob( nullptr )
  , mCache( nullptr )
  , mLabelingResults( nullptr )
  , mDirty( false )
  , mFreeze( false )
{
  connect( this, SIGNAL( windowChanged( QQuickWindow* ) ), this, SLOT( onWindowChanged( QQuickWindow* ) ) );
  connect( &mRefreshTimer, SIGNAL( timeout() ), this, SLOT( refreshMap() ) );

  connect( mMapSettings, SIGNAL( extentChanged() ), this,SLOT( onExtentChanged() ) );
  connect( mMapSettings, SIGNAL( layersChanged() ), this,SLOT( onLayersChanged() ) );

  mRefreshTimer.setSingleShot( true );
  setTransformOrigin( QQuickItem::TopLeft );
  setFlags( QQuickItem::ItemHasContents );
}

QgsQuickMapCanvasMap::~QgsQuickMapCanvasMap()
{
}

MapSettings* QgsQuickMapCanvasMap::mapSettings() const
{
  return mMapSettings;
}

QgsUnitTypes::DistanceUnit QgsQuickMapCanvasMap::mapUnits() const
{
  return mMapSettings->mapUnits();
}

void QgsQuickMapCanvasMap::setMapUnits( const QgsUnitTypes::DistanceUnit& mapUnits )
{
  mMapSettings->setMapUnits( mapUnits );
}

void QgsQuickMapCanvasMap::zoom( QPointF center, qreal scale )
{
  QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  QgsPoint oldCenter( visibleExtent.center() );
  QgsPoint mousePos( mMapSettings->screenToCoordinate( center ) );
  QgsPoint newCenter( mousePos.x() + ( ( oldCenter.x() - mousePos.x() ) * scale ),
                      mousePos.y() + ( ( oldCenter.y() - mousePos.y() ) * scale ) );

  // same as zoomWithCenter (no coordinate transformations are needed)
  visibleExtent.scale( scale, &newCenter );
  mMapSettings->setExtent( visibleExtent );
}

void QgsQuickMapCanvasMap::pan( QPointF oldPos, QPointF newPos )
{
  QgsPoint start = mMapSettings->screenToCoordinate( oldPos.toPoint() );
  QgsPoint end = mMapSettings->screenToCoordinate( newPos.toPoint() );

  double dx = qAbs( end.x() - start.x() );
  double dy = qAbs( end.y() - start.y() );

  // modify the extent
  QgsRectangle visibleExtent = mMapSettings->visibleExtent();

  if ( end.x() > start.x() )
  {
    visibleExtent.setXMinimum( visibleExtent.xMinimum() + dx );
    visibleExtent.setXMaximum( visibleExtent.xMaximum() + dx );
  }
  else
  {
    visibleExtent.setXMinimum( visibleExtent.xMinimum() - dx );
    visibleExtent.setXMaximum( visibleExtent.xMaximum() - dx );
  }

  if ( end.y() > start.y() )
  {
    visibleExtent.setYMaximum( visibleExtent.yMaximum() + dy );
    visibleExtent.setYMinimum( visibleExtent.yMinimum() + dy );

  }
  else
  {
    visibleExtent.setYMaximum( visibleExtent.yMaximum() - dy );
    visibleExtent.setYMinimum( visibleExtent.yMinimum() - dy );

  }

  mMapSettings->setExtent( visibleExtent );
}

void QgsQuickMapCanvasMap::refreshMap()
{
  stopRendering(); // if any...

  QgsMapSettings mapSettings = mMapSettings->mapSettings();

  //build the expression context
  QgsExpressionContext expressionContext;
  expressionContext << QgsExpressionContextUtils::globalScope()
                    << QgsExpressionContextUtils::projectScope()
                    << QgsExpressionContextUtils::mapSettingsScope( mapSettings );

  mapSettings.setExpressionContext( expressionContext );

  // create the renderer job
  Q_ASSERT( !mJob );
  mJobCancelled = false;
  mJob = new QgsMapRendererParallelJob( mapSettings );
  connect( mJob, SIGNAL( finished() ), SLOT( renderJobFinished() ) );
  mJob->setCache( mCache );

  QStringList layersForGeometryCache;
  Q_FOREACH ( QgsMapLayer* layer, mMapSettings->layers() )
  {
    if ( QgsVectorLayer* vl = qobject_cast<QgsVectorLayer*>( layer ) )
    {
      if ( vl->isEditable() )
        layersForGeometryCache << vl->id();
    }
  }
  mJob->setRequestedGeometryCacheForLayers( layersForGeometryCache );

  mJob->start();

  emit renderStarting();
}

void QgsQuickMapCanvasMap::renderJobFinished()
{
  Q_FOREACH ( const QgsMapRendererJob::Error& error, mJob->errors() )
  {
    QgsMessageLog::logMessage( error.layerID + " :: " + error.message, tr( "Rendering" ) );
  }

  if ( !mJobCancelled )
  {
    // take labeling results before emitting renderComplete, so labeling map tools
    // connected to signal work with correct results
    delete mLabelingResults;
    mLabelingResults = mJob->takeLabelingResults();

    mImage = mJob->renderedImage();
    mImageMapSettings = mJob->mapSettings();
  }

  // now we are in a slot called from mJob - do not delete it immediately
  // so the class is still valid when the execution returns to the class
  mJob->deleteLater();
  mJob = nullptr;

  mDirty = true;

  updateTransform();

  update();
  emit mapCanvasRefreshed();
}

void QgsQuickMapCanvasMap::onWindowChanged( QQuickWindow* window )
{
  disconnect( this, SLOT( onScreenChanged( QScreen* ) ) );
  if ( window )
    connect( window, SIGNAL( screenChanged( QScreen* ) ), this, SLOT( onScreenChanged( QScreen* ) ) );
}

void QgsQuickMapCanvasMap::onScreenChanged( QScreen* screen )
{
  mMapSettings->setOutputDpi( screen->physicalDotsPerInch() );
}

void QgsQuickMapCanvasMap::onExtentChanged()
{
  updateTransform();

  // And trigger a new rendering job
  refresh();
}

void QgsQuickMapCanvasMap::updateTransform()
{
  QgsMapSettings currentMapSettings = mMapSettings->mapSettings();
  QgsMapToPixel mtp = currentMapSettings.mapToPixel();

  QgsPoint pixelPt = mtp.transform( mImageMapSettings.visibleExtent().xMinimum(), mImageMapSettings.visibleExtent().yMaximum() );
  setScale( mImageMapSettings.scale() / currentMapSettings.scale() );

  setX( pixelPt.x() );
  setY( pixelPt.y() );
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

QgsRectangle QgsQuickMapCanvasMap::extent() const
{
  return mMapSettings->extent();
}

void QgsQuickMapCanvasMap::setExtent( const QgsRectangle& extent )
{
  mMapSettings->setExtent( extent );
}

QSGNode* QgsQuickMapCanvasMap::updatePaintNode( QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* )
{
  if ( mDirty )
  {
    delete oldNode;
    oldNode = nullptr;
    mDirty = false;
  }

  QSGSimpleTextureNode* node = static_cast<QSGSimpleTextureNode*>( oldNode );
  if ( !node )
  {
    node = new QSGSimpleTextureNode();
    QSGTexture* texture = window()->createTextureFromImage( mImage );
    node->setTexture( texture );
  }

  QRectF rect( boundingRect() );

  // Check for resizes that change the w/h ratio
  if ( !rect.isEmpty() && !mImage.size().isEmpty() && rect.width() / rect.height() != mImage.width() / mImage.height() )
  {
    if ( rect.height() == mImage.height() )
    {
      rect.setHeight( rect.width() / mImage.width() * mImage.height() );
    }
    else
    {
      rect.setWidth( rect.height() / mImage.height() * mImage.width() );
    }
  }

  node->setRect( rect );

  return node;
}

QgsCoordinateReferenceSystem QgsQuickMapCanvasMap::destinationCrs() const
{
  return mMapSettings->destinationCrs();
}

void QgsQuickMapCanvasMap::setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs )
{
  mMapSettings->setDestinationCrs( destinationCrs );
}

void QgsQuickMapCanvasMap::geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry )
{
  Q_UNUSED( oldGeometry )
  // The Qt documentation advices to call the base method here.
  // However, this introduces instabilities and heavy performance impacts on Android.
  // It seems on desktop disabling it prevents us from downsizing the window...
  // Be careful when re-enabling it.
  // QQuickItem::geometryChanged( newGeometry, oldGeometry );

  mMapSettings->setOutputSize( newGeometry.size().toSize() );
  refresh();
}

bool QgsQuickMapCanvasMap::hasCrsTransformEnabled() const
{
  return mMapSettings->hasCrsTransformEnabled();
}

void QgsQuickMapCanvasMap::setCrsTransformEnabled( bool crsTransformEnabled )
{
  mMapSettings->setCrsTransformEnabled( crsTransformEnabled );
}

void QgsQuickMapCanvasMap::onLayersChanged()
{
  if ( mMapSettings->extent().isEmpty() )
    zoomToFullExtent();
  refresh();
}

void QgsQuickMapCanvasMap::stopRendering()
{
  if ( mJob )
  {
    mJobCancelled = true;
    mJob->cancel();
    Q_ASSERT( !mJob ); // no need to delete here: already deleted in finished()
  }
}

void QgsQuickMapCanvasMap::zoomToFullExtent()
{
  QgsRectangle extent;
  Q_FOREACH( QgsMapLayer* layer, mMapSettings->layers() )
  {
    extent.combineExtentWith( layer->extent() );
  }
  mMapSettings->setExtent( extent );

  refresh();
}

void QgsQuickMapCanvasMap::refresh()
{
  if ( !mFreeze )
    mRefreshTimer.start( 1 );
}
