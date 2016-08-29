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

QgsQuickMapCanvasMap::QgsQuickMapCanvasMap(  QQuickItem* parent )
  : QQuickPaintedItem( parent )
  , mMapSettings( new MapSettings() )
  , mPinching( false )
  , mJobCancelled( false )
  , mRefreshScheduled( false )
  , mCache( nullptr )
  , mLabelingResults( nullptr )
  , mJob( nullptr )
{
  setRenderTarget( QQuickPaintedItem::FramebufferObject );
}

QgsQuickMapCanvasMap::~QgsQuickMapCanvasMap()
{
}

void QgsQuickMapCanvasMap::paint( QPainter* painter )
{
  int w = qRound( boundingRect().width() ) - 2, h = qRound( boundingRect().height() ) - 2; // setRect() makes the size +2 :-(
  painter->drawImage( QRect( 0, 0, w, h ), mImage );
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
  /*
  QgsPoint oldCenter( mMapCanvas->mapSettings().visibleExtent().center() );
  QgsPoint mousePos( mMapCanvas->getCoordinateTransform()->toMapPoint( center.x(), center.y() ) );
  QgsPoint newCenter( mousePos.x() + ( ( oldCenter.x() - mousePos.x() ) * scale ),
                      mousePos.y() + ( ( oldCenter.y() - mousePos.y() ) * scale ) );

  // same as zoomWithCenter (no coordinate transformations are needed)
  QgsRectangle extent = mMapCanvas->mapSettings().visibleExtent();
  extent.scale( scale, &newCenter );
  mMapCanvas->setExtent( extent );

  update();
  */
}

void QgsQuickMapCanvasMap::pan( QPointF oldPos, QPointF newPos )
{
  /*

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

  update();
  */
}

void QgsQuickMapCanvasMap::refreshMap()
{
  Q_ASSERT( mRefreshScheduled );

  stopRendering(); // if any...

  // from now on we can accept refresh requests again
  mRefreshScheduled = false;

  QgsMapSettings mapSettings = prepareMapSettings();

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
  Q_FOREACH ( QgsMapLayer* layer, mMapLayers )
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
  }

  // now we are in a slot called from mJob - do not delete it immediately
  // so the class is still valid when the execution returns to the class
  mJob->deleteLater();
  mJob = nullptr;

  update();
  emit mapCanvasRefreshed();
}

QgsMapSettings QgsQuickMapCanvasMap::prepareMapSettings() const
{
  QgsMapSettings mapSettings = mMapSettings->mapSettings();

  QStringList ids;
  Q_FOREACH( QgsMapLayer* layer, mMapLayers )
    ids << layer->id();

  mapSettings.setLayers( ids );
  return mapSettings;
}

QgsRectangle QgsQuickMapCanvasMap::extent() const
{
  return mMapSettings->extent();
}

void QgsQuickMapCanvasMap::setExtent( const QgsRectangle& extent )
{
  mMapSettings->setExtent( extent );
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

QList<QgsMapLayer*> QgsQuickMapCanvasMap::layerSet() const
{
  return mMapLayers;
}

void QgsQuickMapCanvasMap::setLayerSet( const QList<QgsMapLayer*>& layerSet )
{
  mMapLayers = layerSet;
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
  Q_FOREACH( QgsMapLayer* layer, mMapLayers )
  {
    extent.combineExtentWith( layer->extent() );
  }
  mMapSettings->setExtent( extent );
}

void QgsQuickMapCanvasMap::refresh()
{
  if ( mRefreshScheduled )
    return;

  mRefreshScheduled = true;

  QTimer::singleShot( 1, this, SLOT( refreshMap() ) );
}
