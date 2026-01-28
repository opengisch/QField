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

QgsProject *Quick3DMapTextureGenerator::project() const
{
  return mProject;
}

void Quick3DMapTextureGenerator::setProject( QgsProject *project )
{
  if ( mProject == project )
  {
    return;
  }

  mProject = project;
  emit projectChanged();
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
  if ( !mProject || mExtent.isEmpty() )
  {
    return;
  }

  if ( mRenderJob )
  {
    mRenderJob->cancel();
    mRenderJob.reset();
  }

  const QMap<QString, QgsMapLayer *> layers = mProject->mapLayers();

  // Finding dem layer (same as Quick3DTerrainProvider)
  const QgsRasterLayer *demLayer = nullptr;
  QList<QgsMapLayer *> layersToRender;

  for ( QgsMapLayer *const layer : layers )
  {
    if ( !layer || !layer->isValid() || layer->type() != Qgis::LayerType::Raster )
    {
      continue;
    }

    QgsRasterLayer *const rasterLayer = qobject_cast<QgsRasterLayer *>( layer );
    if ( !rasterLayer )
    {
      continue;
    }

    const QString providerName = rasterLayer->dataProvider() ? rasterLayer->dataProvider()->name() : QString();
    const bool isOnlineLayer = providerName == QStringLiteral( "wms" ) || providerName == QStringLiteral( "wmts" );

    if ( rasterLayer->bandCount() == 1 && !isOnlineLayer )
    {
      if ( !demLayer )
      {
        demLayer = rasterLayer;
      }
    }
    else if ( rasterLayer->bandCount() > 1 || isOnlineLayer )
    {
      layersToRender.append( layer );
    }
  }

  // Calculate texture dimensions maintaining aspect ratio
  const double width = mExtent.width();
  const double height = mExtent.height();
  int texWidth, texHeight;

  if ( width >= height )
  {
    texWidth = mTextureSize;
    texHeight = std::max( 256, static_cast<int>( mTextureSize * height / width ) );
  }
  else
  {
    texHeight = mTextureSize;
    texWidth = std::max( 256, static_cast<int>( mTextureSize * width / height ) );
  }

  // Generate fallback if no layers to render
  if ( layersToRender.isEmpty() )
  {
    mRenderedImage = QImage( texWidth, texHeight, QImage::Format_RGB32 );
    mRenderedImage.fill( QColor( 100, 140, 100 ) );
    mRenderedImage.save( mTextureFilePath );

    emit ready();
    emit textureFilePathChanged();
    return;
  }

  QgsMapSettings mapSettings;
  mapSettings.setOutputSize( QSize( texWidth, texHeight ) );
  mapSettings.setExtent( mExtent );
  mapSettings.setDestinationCrs( mProject->crs() );
  mapSettings.setTransformContext( mProject->transformContext() );
  mapSettings.setBackgroundColor( QColor( 80, 80, 80 ) );
  mapSettings.setLayers( layersToRender );

  mRenderJob = std::make_unique<QgsMapRendererSequentialJob>( mapSettings );
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

  if ( mRenderedImage.isNull() )
  {
    mRenderedImage = QImage( mTextureSize, mTextureSize, QImage::Format_RGB32 );
    mRenderedImage.fill( QColor( 100, 140, 100 ) );
  }
  else
  {
    mRenderedImage = mRenderedImage.flipped( Qt::Vertical );
  }

  mRenderedImage.save( mTextureFilePath );
  emit ready();
  emit textureFilePathChanged();
}
