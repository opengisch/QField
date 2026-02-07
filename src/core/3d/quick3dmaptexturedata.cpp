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

#include <qgsmaplayer.h>
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
      mLayerConnections << connect( layer, &QgsMapLayer::repaintRequested, this, &Quick3DMapTextureData::render );
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

void Quick3DMapTextureData::render()
{
  if ( !mMapSettings || mExtent.isEmpty() )
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

  if ( !mExtent.isEmpty() )
  {
    renderSettings.setRotation( 0 );
    renderSettings.setExtent( mExtent );


    const double mupp = mMapSettings->mapSettings().mapUnitsPerPixel();
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

  mRenderJob.reset( new QgsMapRendererSequentialJob( renderSettings ) );
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
  QImage rgbaImage = image.convertToFormat( QImage::Format_RGBA8888 );

  setSize( rgbaImage.size() );
  setFormat( QQuick3DTextureData::RGBA8 );
  setHasTransparency( true );

  const int dataSize = rgbaImage.sizeInBytes();
  QByteArray textureData( reinterpret_cast<const char *>( rgbaImage.constBits() ), dataSize );
  setTextureData( textureData );

  // Force Qt Quick 3D to reload the texture by toggling ready state
  if ( mReady )
  {
    mReady = false;
    emit readyChanged();
  }

  mReady = true;
  emit readyChanged();
}
