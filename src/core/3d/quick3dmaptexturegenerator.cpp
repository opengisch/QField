/***************************************************************************
  quick3dmaptexturegenerator.cpp - Quick3DMapTextureGenerator

 ---------------------
 begin                : 26.1.2026
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
#include "quick3dmaptexturegenerator.h"

#include <qgsmaprenderersequentialjob.h>
#include <qgsmapsettings.h>
#include <qgsrasterlayer.h>

#include <algorithm>

int Quick3DMapTextureGenerator::sInstanceCounter = 0;

Quick3DMapTextureGenerator::Quick3DMapTextureGenerator( QObject *parent )
  : QObject( parent )
{
  const QString tempPath = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
  mTextureFilePath = QStringLiteral( "%1/qfield_3d_texture_%2.png" ).arg( tempPath ).arg( ++sInstanceCounter );
}

Quick3DMapTextureGenerator::~Quick3DMapTextureGenerator()
{
  if ( mRenderJob )
  {
    mRenderJob->cancel();
  }
  if ( QFile::exists( mTextureFilePath ) )
  {
    QFile::remove( mTextureFilePath );
  }
}

QgsQuickMapSettings *Quick3DMapTextureGenerator::mapSettings() const
{
  return mMapSettings;
}

void Quick3DMapTextureGenerator::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::layersChanged, this, &Quick3DMapTextureGenerator::render );
  }

  mMapSettings = mapSettings;

  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::layersChanged, this, &Quick3DMapTextureGenerator::render );
  }

  emit mapSettingsChanged();
}

QgsRectangle Quick3DMapTextureGenerator::extent() const
{
  return mExtent;
}

void Quick3DMapTextureGenerator::setExtent( const QgsRectangle &extent )
{
  if ( mExtent == extent )
  {
    return;
  }

  mExtent = extent;
  emit extentChanged();
}

QString Quick3DMapTextureGenerator::textureFilePath() const
{
  return mTextureFilePath;
}

void Quick3DMapTextureGenerator::render()
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
    mRenderedImage = QImage( outputSize, QImage::Format_RGB32 );
    mRenderedImage.fill( QColor( 100, 140, 100 ) );
    mRenderedImage.save( mTextureFilePath );

    emit ready();
    emit textureFilePathChanged();
    return;
  }

  mRenderJob = std::make_unique<QgsMapRendererSequentialJob>( renderSettings );
  connect( mRenderJob.get(), &QgsMapRendererSequentialJob::finished, this, &Quick3DMapTextureGenerator::onRenderFinished );
  mRenderJob->start();
}

void Quick3DMapTextureGenerator::onRenderFinished()
{
  if ( !mRenderJob )
  {
    return;
  }

  mRenderedImage = mRenderJob->renderedImage();
  mRenderJob.reset();

  if ( !mRenderedImage.isNull() )
  {
    mRenderedImage = mRenderedImage.flipped( Qt::Vertical );
  }

  mRenderedImage.save( mTextureFilePath );
  emit ready();
  emit textureFilePathChanged();
}
