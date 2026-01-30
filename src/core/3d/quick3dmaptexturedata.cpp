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

#include <qgsmaprenderersequentialjob.h>
#include <qgsmapsettings.h>

#include <algorithm>

Quick3DMapTextureData::Quick3DMapTextureData( QQuick3DObject *parent )
  : QQuick3DTextureData( parent )
{
}

Quick3DMapTextureData::~Quick3DMapTextureData()
{
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

  mMapSettings = mapSettings;

  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::layersChanged, this, &Quick3DMapTextureData::render );
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

void Quick3DMapTextureData::render()
{
  if ( !mMapSettings )
  {
    return;
  }

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

  // Use custom extent if set, otherwise use mapSettings extent
  if ( !mExtent.isEmpty() )
  {
    renderSettings.setExtent( mExtent );

    // Adjust output size to match the aspect ratio of the custom extent
    const QSize originalSize = renderSettings.outputSize();
    const double extentAspectRatio = mExtent.width() / mExtent.height();
    const int baseSize = std::max( originalSize.width(), originalSize.height() );

    int newWidth, newHeight;
    if ( extentAspectRatio >= 1.0 )
    {
      newWidth = baseSize;
      newHeight = static_cast<int>( baseSize / extentAspectRatio );
    }
    else
    {
      newHeight = baseSize;
      newWidth = static_cast<int>( baseSize * extentAspectRatio );
    }
    renderSettings.setOutputSize( QSize( newWidth, newHeight ) );
  }

  // Generate fallback if no layers to render
  if ( renderSettings.layers().isEmpty() )
  {
    const QSize outputSize = renderSettings.outputSize();
    QImage fallbackImage( outputSize, QImage::Format_RGBA8888 );
    fallbackImage.fill( QColor( 100, 140, 100 ) );
    updateTextureData( fallbackImage );
    return;
  }

  mRenderJob = std::make_unique<QgsMapRendererSequentialJob>( renderSettings );
  connect( mRenderJob.get(), &QgsMapRendererSequentialJob::finished, this, &Quick3DMapTextureData::onRenderFinished );
  mRenderJob->start();
}

void Quick3DMapTextureData::onRenderFinished()
{
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
}

void Quick3DMapTextureData::updateTextureData( const QImage &image )
{
  // Convert to RGBA8888 format for Qt Quick 3D
  QImage rgbaImage = image.convertToFormat( QImage::Format_RGBA8888 );

  setSize( rgbaImage.size() );
  setFormat( QQuick3DTextureData::RGBA8 );
  setHasTransparency( true );

  const int dataSize = rgbaImage.sizeInBytes();
  QByteArray textureData( reinterpret_cast<const char *>( rgbaImage.constBits() ), dataSize );
  setTextureData( textureData );

  if ( !mReady )
  {
    mReady = true;
    emit readyChanged();
  }
}
