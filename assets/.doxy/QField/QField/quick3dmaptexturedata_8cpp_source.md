

# File quick3dmaptexturedata.cpp

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dmaptexturedata.cpp**](quick3dmaptexturedata_8cpp.md)

[Go to the documentation of this file](quick3dmaptexturedata_8cpp.md)


```C++
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


Quick3DMapTextureData::Quick3DMapTextureData( QQuick3DObject *parent )
  : QQuick3DTextureData( parent )
{
  connect( &mMapUpdateTimer, &QTimer::timeout, this, &Quick3DMapTextureData::onRenderJobUpdated );
  mMapUpdateTimer.setSingleShot( false );
  mMapUpdateTimer.setInterval( 500 );

  connect( &mRefreshTimer, &QTimer::timeout, this, &Quick3DMapTextureData::render );
  mRefreshTimer.setSingleShot( true );

  setFormat( QQuick3DTextureData::RGBA8 );
  setHasTransparency( true );
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
  return mIsReady;
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

  QSize outputSize = renderSettings.outputSize();
  if ( outputSize.height() > outputSize.width() )
  {
    outputSize.setWidth( outputSize.height() * mExtent.width() / mExtent.height() );
  }
  else
  {
    outputSize.setHeight( outputSize.width() * mExtent.height() / mExtent.width() );
  }

  if ( !mExtent.isEmpty() )
  {
    renderSettings.setRotation( 0 );
    renderSettings.setExtent( mExtent );
    renderSettings.setOutputSize( outputSize );
  }

  if ( renderSettings.layers().isEmpty() )
  {
    QImage fallbackImage( outputSize, QImage::Format_RGBA8888 );
    fallbackImage.fill( QColor( 100, 140, 100 ) );
    updateTextureData( fallbackImage );
    return;
  }

  renderSettings.setFlag( Qgis::MapSettingsFlag::UseRenderingOptimization );
  renderSettings.setFlag( Qgis::MapSettingsFlag::RenderPartialOutput, mIncrementalRendering );
  renderSettings.setOutputImageFormat( QImage::Format_RGBA8888 );

  mRenderJob.reset( new QgsMapRendererParallelJob( renderSettings ) );

  if ( mIncrementalRendering )
  {
    mMapUpdateTimer.start();
  }

  connect( mRenderJob.get(), &QgsMapRendererJob::finished, this, &Quick3DMapTextureData::onRenderFinished );
  mRenderJob->start();

  emit isRenderingChanged();
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
  emit isRenderingChanged();

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

bool Quick3DMapTextureData::isRendering() const
{
  return mRenderJob && mRenderJob->isActive();
}

void Quick3DMapTextureData::updateTextureData( const QImage &image )
{
  const qsizetype dataSize = image.sizeInBytes();
  QByteArray textureData( reinterpret_cast<const char *>( image.constBits() ), dataSize );

  setSize( image.size() );
  setTextureData( textureData );

  if ( !mIsReady )
  {
    mIsReady = true;
    emit isReadyChanged();
  }

  emit textureUpdated();
}
```


