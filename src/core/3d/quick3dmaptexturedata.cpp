/***************************************************************************
  quick3dmaptexturedata.cpp - Quick3DMapTextureData

 ---------------------
 begin                : 30.1.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquick/qgsquickmapsettings.h"
#include "quick3dmaptexturedata.h"

#include <QPainter>
#include <qgis.h>
#include <qgsmaplayer.h>
#include <qgsmaprendererparalleljob.h>
#include <qgsmapsettings.h>

#include <algorithm>

Quick3DMapTextureData::Quick3DMapTextureData( QQuick3DObject *parent )
  : QQuick3DTextureData( parent )
{
  connect( &mMapUpdateTimer, &QTimer::timeout, this, &Quick3DMapTextureData::onRenderJobUpdated );
  mMapUpdateTimer.setSingleShot( false );
  mMapUpdateTimer.setInterval( 250 );

  connect( &mRefreshTimer, &QTimer::timeout, this, &Quick3DMapTextureData::render );
  mRefreshTimer.setSingleShot( true );
}

Quick3DMapTextureData::~Quick3DMapTextureData()
{
  mMapUpdateTimer.stop();
  if ( mRenderJob )
  {
    mRenderJob->cancel();
  }
}

QgsQuickMapSettings *Quick3DMapTextureData::mapSettings() const
{
  return mMapSettings;
}

void Quick3DMapTextureData::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::layersChanged, this, &Quick3DMapTextureData::render );
  }

  for ( const QMetaObject::Connection &conn : std::as_const( mLayerConnections ) )
  {
    disconnect( conn );
  }
  mLayerConnections.clear();

  mMapSettings = mapSettings;

  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::layersChanged, this, &Quick3DMapTextureData::render );

    const QList<QgsMapLayer *> layers = mMapSettings->layers();
    for ( const QgsMapLayer *layer : layers )
    {
      mLayerConnections << connect( layer, &QgsMapLayer::repaintRequested, this, &Quick3DMapTextureData::layerRepaintRequested );
    }
  }

  emit mapSettingsChanged();
}

QgsRectangle Quick3DMapTextureData::extent() const
{
  return mExtent;
}

void Quick3DMapTextureData::setExtent( const QgsRectangle &extent )
{
  if ( mExtent == extent )
  {
    return;
  }

  mExtent = extent;
  emit extentChanged();
}

bool Quick3DMapTextureData::isReady() const
{
  return mReady;
}

bool Quick3DMapTextureData::incrementalRendering() const
{
  return mIncrementalRendering;
}

void Quick3DMapTextureData::setIncrementalRendering( bool incrementalRendering )
{
  if ( incrementalRendering == mIncrementalRendering )
  {
    return;
  }

  mIncrementalRendering = incrementalRendering;
  emit incrementalRenderingChanged();
}

bool Quick3DMapTextureData::forceDeferredLayersRepaint() const
{
  return mForceDeferredLayersRepaint;
}

void Quick3DMapTextureData::setForceDeferredLayersRepaint( bool deferred )
{
  if ( mForceDeferredLayersRepaint == deferred )
    return;

  mForceDeferredLayersRepaint = deferred;
  emit forceDeferredLayersRepaintChanged();
}

void Quick3DMapTextureData::layerRepaintRequested()
{
  if ( mForceDeferredLayersRepaint )
  {
    if ( !mRenderJob && !mRefreshTimer.isActive() )
    {
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

void Quick3DMapTextureData::refresh()
{
  mRefreshTimer.start( 1 );
}

void Quick3DMapTextureData::render()
{
  if ( !mMapSettings || mExtent.isEmpty() )
  {
    return;
  }

  mRefreshTimer.stop();
  mMapUpdateTimer.stop();
  if ( mRenderJob )
  {
    mRenderJob->cancel();
    mRenderJob.reset();
  }

  QgsMapSettings renderSettings = mMapSettings->mapSettings();
  if ( !renderSettings.hasValidSettings() )
  {
    return;
  }

  if ( !mExtent.isEmpty() )
  {
    renderSettings.setRotation( 0 );
    renderSettings.setExtent( mExtent );

    const double mupp = renderSettings.mapUnitsPerPixel();
    const int outputWidth = mExtent.width() / mupp;
    const int outputHeight = mExtent.height() / mupp;
    renderSettings.setOutputSize( QSize( outputWidth, outputHeight ) );
  }

  if ( renderSettings.layers().isEmpty() )
  {
    const QSize outputSize = renderSettings.outputSize();
    QImage fallbackImage( outputSize, QImage::Format_RGBA8888 );
    fallbackImage.fill( QColor( 100, 140, 100 ) );
    updateTextureData( fallbackImage );
    return;
  }

  renderSettings.setFlag( Qgis::MapSettingsFlag::UseRenderingOptimization );
  renderSettings.setFlag( Qgis::MapSettingsFlag::RenderPartialOutput, mIncrementalRendering );

  mRenderJob.reset( new QgsMapRendererParallelJob( renderSettings ) );

  if ( mIncrementalRendering )
  {
    connect( mRenderJob.get(), &QgsMapRendererJob::renderingLayersFinished, this, &Quick3DMapTextureData::onRenderJobUpdated );
    mMapUpdateTimer.start();
  }

  connect( mRenderJob.get(), &QgsMapRendererJob::finished, this, &Quick3DMapTextureData::onRenderFinished );
  mRenderJob->start();
}

void Quick3DMapTextureData::onRenderJobUpdated()
{
  if ( !mRenderJob )
    return;

  QImage image = mRenderJob->renderedImage();
  if ( !image.isNull() )
  {
    updateTextureData( image );
  }
}

void Quick3DMapTextureData::onRenderFinished()
{
  mMapUpdateTimer.stop();

  if ( !mRenderJob )
  {
    return;
  }

  QImage renderedImage = mRenderJob->renderedImage();
  mRenderJob.reset();

  if ( !renderedImage.isNull() )
  {
    updateTextureData( renderedImage );
  }

  if ( mDeferredRefreshPending )
  {
    mDeferredRefreshPending = false;
    refresh();
  }
}

void Quick3DMapTextureData::updateTextureData( const QImage &image )
{
  QImage rgbaImage = image.convertToFormat( QImage::Format_RGBA8888 );

  const int w = rgbaImage.width();
  const int h = rgbaImage.height();
  QImage metagrid( w * 3, h * 3, QImage::Format_RGBA8888 );
  metagrid.fill( QColor( 180, 180, 180 ) );

  // Draw crosshatch grid pattern on the gray areas
  {
    QPainter painter( &metagrid );
    const QColor lineColor( 160, 160, 160 );
    painter.setPen( QPen( lineColor, 1 ) );
    const int spacing = 40;
    const int totalW = w * 3;
    const int totalH = h * 3;

    for ( int y = 0; y < totalH; y += spacing )
    {
      painter.drawLine( 0, y, totalW, y );
    }
    for ( int x = 0; x < totalW; x += spacing )
    {
      painter.drawLine( x, 0, x, totalH );
    }
    painter.end();
  }

  const qsizetype bytesPerRow = static_cast<qsizetype>( w ) * 4;
  for ( int y = 0; y < h; ++y )
  {
    memcpy( metagrid.scanLine( y + h ) + bytesPerRow, rgbaImage.constScanLine( y ), bytesPerRow );
  }

  setSize( metagrid.size() );
  setFormat( QQuick3DTextureData::RGBA8 );
  setHasTransparency( true );

  const qsizetype dataSize = metagrid.sizeInBytes();
  QByteArray textureData( reinterpret_cast<const char *>( metagrid.constBits() ), dataSize );
  setTextureData( textureData );

  if ( !mReady )
  {
    mReady = true;
    emit readyChanged();
  }

  emit textureUpdated();
}
