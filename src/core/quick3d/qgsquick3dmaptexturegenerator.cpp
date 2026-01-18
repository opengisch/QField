/***************************************************************************
  qgsquick3dmaptexturegenerator.cpp - QgsQuick3DMapTextureGenerator

 ---------------------
 begin                : 6.1.2026
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

#include "qgsquick3dmaptexturegenerator.h"

#include <qgsmaprenderersequentialjob.h>
#include <qgsmapsettings.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>

int QgsQuick3DMapTextureGenerator::sInstanceCounter = 0;

QgsQuick3DMapTextureGenerator::QgsQuick3DMapTextureGenerator( QObject *parent )
  : QObject( parent )
{
  const QString tempPath = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
  mTextureFilePath = QStringLiteral( "%1/qfield_3d_texture_%2.png" ).arg( tempPath ).arg( ++sInstanceCounter );
}

QgsQuick3DMapTextureGenerator::~QgsQuick3DMapTextureGenerator() = default;

QgsProject *QgsQuick3DMapTextureGenerator::project() const
{
  return mProject;
}

void QgsQuick3DMapTextureGenerator::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;
  emit projectChanged();
}

QRectF QgsQuick3DMapTextureGenerator::extent() const
{
  return mExtent;
}

void QgsQuick3DMapTextureGenerator::setExtent( const QRectF &extent )
{
  if ( mExtent == extent )
    return;

  mExtent = extent;
  emit extentChanged();
}

QString QgsQuick3DMapTextureGenerator::textureFilePath() const
{
  return mTextureFilePath;
}

void QgsQuick3DMapTextureGenerator::render()
{
  if ( !mProject || mExtent.isEmpty() )
    return;

  if ( mRenderJob )
  {
    mRenderJob->cancel();
    mRenderJob.reset();
  }

  const QMap<QString, QgsMapLayer *> layers = mProject->mapLayers();

  // Finding dem layer (same as QgsQuick3DTerrainProvider)
  QgsRasterLayer *demLayer = nullptr;
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

  // Determine render extent from DEM
  QgsRectangle renderExtent;
  if ( demLayer )
  {
    const QgsCoordinateTransform transform( demLayer->crs(), mProject->crs(), mProject->transformContext() );
    renderExtent = transform.transformBoundingBox( demLayer->extent() );
  }
  else
  {
    renderExtent = QgsRectangle( mExtent.left(), mExtent.top(),
                                 mExtent.left() + mExtent.width(),
                                 mExtent.top() + mExtent.height() );
  }

  // Calculate texture dimensions maintaining aspect ratio
  const double width = renderExtent.width();
  const double height = renderExtent.height();
  int texWidth, texHeight;

  if ( width >= height )
  {
    texWidth = mTextureSize;
    texHeight = qMax( 256, static_cast<int>( mTextureSize * height / width ) );
  }
  else
  {
    texHeight = mTextureSize;
    texWidth = qMax( 256, static_cast<int>( mTextureSize * width / height ) );
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
  mapSettings.setExtent( renderExtent );
  mapSettings.setDestinationCrs( mProject->crs() );
  mapSettings.setTransformContext( mProject->transformContext() );
  mapSettings.setBackgroundColor( QColor( 80, 80, 80 ) );
  mapSettings.setLayers( layersToRender );

  mRenderJob = std::make_unique<QgsMapRendererSequentialJob>( mapSettings );
  connect( mRenderJob.get(), &QgsMapRendererSequentialJob::finished, this, &QgsQuick3DMapTextureGenerator::onRenderFinished );
  mRenderJob->start();
}

void QgsQuick3DMapTextureGenerator::onRenderFinished()
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
    mRenderedImage = mRenderedImage.mirrored( false, true );
  }

  mRenderedImage.save( mTextureFilePath );
  emit ready();
  emit textureFilePathChanged();
}
