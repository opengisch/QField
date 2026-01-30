/***************************************************************************
  quick3dterrainprovider.cpp - Quick3DTerrainProvider

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

#include "qgsquickmapsettings.h"
#include "quick3dterrainprovider.h"

#include <QTimer>
#include <QtConcurrent>
#include <qgscoordinatetransform.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsrasterblock.h>
#include <qgsrasterdataprovider.h>
#include <qgsrasterlayer.h>
#include <qgsterrainprovider.h>

#include <algorithm>
#include <cmath>

Quick3DTerrainProvider::Quick3DTerrainProvider( QObject *parent )
  : QObject( parent )
  , mFutureWatcher( new QFutureWatcher<QVector<double>>( this ) )
{
  connect( mFutureWatcher, &QFutureWatcher<QVector<double>>::finished,
           this, &Quick3DTerrainProvider::onTerrainDataCalculated );

  QTimer *progressTimer = new QTimer( this );
  connect( progressTimer, &QTimer::timeout, this, [this]() {
    if ( mIsLoading )
    {
      const int progress = mProgressCounter.loadAcquire();
      if ( progress != mLoadingProgress )
      {
        mLoadingProgress = progress;
        emit loadingProgressChanged();
      }
    }
  } );
  progressTimer->start( 100 );
}

Quick3DTerrainProvider::~Quick3DTerrainProvider() = default;

QgsProject *Quick3DTerrainProvider::project() const
{
  return mProject;
}

void Quick3DTerrainProvider::setProject( QgsProject *project )
{
  if ( mProject == project )
  {
    return;
  }

  mProject = project;
  emit projectChanged();
  updateTerrainProvider();
}

QgsQuickMapSettings *Quick3DTerrainProvider::mapSettings() const
{
  return mMapSettings;
}

void Quick3DTerrainProvider::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, nullptr, this, nullptr );
  }

  mMapSettings = mapSettings;
  emit mapSettingsChanged();

  updateFromMapSettings();
}

int Quick3DTerrainProvider::resolution() const
{
  return mResolution;
}

QgsRectangle Quick3DTerrainProvider::extent() const
{
  return mExtent;
}

void Quick3DTerrainProvider::updateFromMapSettings()
{
  if ( !mMapSettings )
  {
    return;
  }

  QgsRectangle visibleExtent = mMapSettings->visibleExtent();

  // If we have a DEM layer, intersect with its extent to only show areas with data
  if ( mDemLayer && mDemLayer->isValid() )
  {
    QgsRectangle demExtent = mDemLayer->extent();

    // Transform DEM extent to project CRS if needed
    if ( mProject && mDemLayer->crs() != mProject->crs() )
    {
      try
      {
        QgsCoordinateTransform transform( mDemLayer->crs(), mProject->crs(), mProject->transformContext() );
        demExtent = transform.transformBoundingBox( demExtent );
      }
      catch ( const QgsCsException & )
      {
        // Keep original DEM extent if transform fails
      }
    }

    QgsRectangle intersection = visibleExtent.intersect( demExtent );
    if ( !intersection.isEmpty() )
    {
      const double pixelWidth = demExtent.width() / 1000.0;
      const double pixelHeight = demExtent.height() / 1000.0;
      intersection = QgsRectangle(
        intersection.xMinimum() + pixelWidth,
        intersection.yMinimum() + pixelHeight,
        intersection.xMaximum() - pixelWidth,
        intersection.yMaximum() - pixelHeight );

      visibleExtent = intersection;
    }
  }

  bool changed = false;

  if ( mExtent != visibleExtent )
  {
    mExtent = visibleExtent;
    emit extentChanged();
    changed = true;
  }

  const int newResolution = calculateResolution();
  if ( mResolution != newResolution )
  {
    mResolution = newResolution;
    emit resolutionChanged();
    changed = true;
  }

  if ( changed && ( ( mDemLayer && mDemLayer->isValid() ) || mQgisTerrainProvider ) )
  {
    calcNormalizedData();
  }
}

int Quick3DTerrainProvider::calculateResolution() const
{
  if ( mExtent.isEmpty() )
  {
    return 32;
  }

  const double extentSize = std::max( mExtent.width(), mExtent.height() );

  if ( mDemLayer && mDemLayer->isValid() )
  {
    return 64 * 5;
  }

  if ( extentSize < 2000 )
    return 64;
  if ( extentSize < 10000 )
    return 48;
  if ( extentSize < 50000 )
    return 32;
  return 24;
}

QVariantList Quick3DTerrainProvider::normalizedData() const
{
  return mNormalizedData;
}

int Quick3DTerrainProvider::terrainBaseSize() const
{
  return mTerrainBaseSize;
}

double Quick3DTerrainProvider::heightAt( double x, double y ) const
{
  if ( mDemLayer && mDemLayer->isValid() )
  {
    const double h = sampleHeightFromRaster( mDemLayer, x, y );
    if ( !std::isnan( h ) )
    {
      return h;
    }
  }

  if ( mQgisTerrainProvider )
  {
    const double h = sampleHeightFromTerrainProvider( x, y );
    if ( !std::isnan( h ) )
    {
      return h;
    }
  }

  return 0.0;
}

double Quick3DTerrainProvider::normalizedHeightAt( double x, double y ) const
{
  const double realHeight = heightAt( x, y );
  const double extentSize = std::max( mExtent.width(), mExtent.height() );
  const double scale = ( mTerrainBaseSize / extentSize ) * calculateVisualExaggeration();
  return ( realHeight - mMinRealHeight ) * scale;
}

double Quick3DTerrainProvider::calculateVisualExaggeration() const
{
  const double extentSize = std::max( mExtent.width(), mExtent.height() );
  if ( extentSize > 100000 )
  {
    return 3.0;
  }
  if ( extentSize > 50000 )
  {
    return 2.0;
  }
  return 1.0;
}

void Quick3DTerrainProvider::calcNormalizedData()
{
  const int totalSamples = mResolution * mResolution;

  if ( mExtent.isEmpty() || ( !mDemLayer && !mQgisTerrainProvider ) )
  {
    mNormalizedData.fill( 0.0, totalSamples );
    emit normalizedDataChanged();
    emit terrainDataReady();
    return;
  }

  if ( mFutureWatcher->isRunning() )
  {
    mFutureWatcher->cancel();
    mFutureWatcher->waitForFinished();
  }

  mIsLoading = true;
  emit isLoadingChanged();
  mProgressCounter.storeRelease( 0 );
  mLoadingProgress = 0;
  emit loadingProgressChanged();

  QgsAbstractTerrainProvider *terrainProviderClone = mQgisTerrainProvider ? mQgisTerrainProvider->clone() : nullptr;
  QgsRasterLayer *demLayer = mDemLayer.data();
  QgsProject *project = mProject;
  QgsRectangle extent = mExtent;
  int resolution = mResolution;
  QAtomicInt *progressCounter = &mProgressCounter;

  const QFuture<QVector<double>> future = QtConcurrent::run( [demLayer, terrainProviderClone, project, extent, resolution, progressCounter]() {
    QVector<double> heights( resolution * resolution, 0.0 );

    progressCounter->storeRelease( 10 );

    // Option 1: Use local DEM layer with raster block API
    if ( demLayer && demLayer->isValid() && demLayer->dataProvider() )
    {
      QgsRectangle blockExtent = extent;
      if ( project && demLayer->crs() != project->crs() )
      {
        try
        {
          QgsCoordinateTransform transform( project->crs(), demLayer->crs(), project->transformContext() );
          blockExtent = transform.transformBoundingBox( extent );
        }
        catch ( const QgsCsException & )
        {
        }
      }

      std::unique_ptr<QgsRasterBlock> block( demLayer->dataProvider()->block( 1, blockExtent, resolution, resolution ) );
      progressCounter->storeRelease( 80 );

      if ( block && block->isValid() )
      {
        for ( int i = 0; i < resolution * resolution; ++i )
        {
          bool isNoData = false;
          const double value = block->valueAndNoData( i / resolution, i % resolution, isNoData );
          if ( !isNoData && !std::isnan( value ) )
          {
            heights[i] = value;
          }
        }
      }
    }
    // Option 2: Use online terrain provider
    else if ( terrainProviderClone && project )
    {
      terrainProviderClone->prepare();

      const double xStep = extent.width() / ( resolution - 1 );
      const double yStep = extent.height() / ( resolution - 1 );

      QgsCoordinateTransform transform;
      const QgsCoordinateReferenceSystem terrainCrs = terrainProviderClone->crs();
      if ( terrainCrs.isValid() && terrainCrs != project->crs() )
      {
        transform = QgsCoordinateTransform( project->crs(), terrainCrs, project->transformContext() );
      }

      for ( int row = 0; row < resolution; ++row )
      {
        for ( int col = 0; col < resolution; ++col )
        {
          QgsPointXY point( extent.xMinimum() + col * xStep, extent.yMaximum() - row * yStep );

          if ( transform.isValid() )
          {
            try
            {
              point = transform.transform( point );
            }
            catch ( const QgsCsException & )
            {
              continue;
            }
          }

          const double h = terrainProviderClone->heightAt( point.x(), point.y() );
          if ( !std::isnan( h ) )
          {
            heights[row * resolution + col] = h;
          }
        }
        progressCounter->storeRelease( 10 + ( row * 80 ) / resolution );
      }
    }

    delete terrainProviderClone;
    progressCounter->storeRelease( 100 );
    return heights;
  } );

  mFutureWatcher->setFuture( future );
}

double Quick3DTerrainProvider::sampleHeightFromRaster( QgsRasterLayer *layer, double x, double y ) const
{
  if ( !layer || !layer->dataProvider() )
  {
    return std::nan( "" );
  }

  QgsPointXY point( x, y );

  if ( mProject && layer->crs() != mProject->crs() )
  {
    try
    {
      QgsCoordinateTransform transform( mProject->crs(), layer->crs(), mProject->transformContext() );
      point = transform.transform( point );
    }
    catch ( const QgsCsException & )
    {
      return std::nan( "" );
    }
  }

  if ( !layer->extent().contains( point ) )
  {
    return std::nan( "" );
  }

  bool ok = false;
  double value = layer->dataProvider()->sample( point, 1, &ok );
  return ok ? value : std::nan( "" );
}

double Quick3DTerrainProvider::sampleHeightFromTerrainProvider( double x, double y ) const
{
  if ( !mQgisTerrainProvider || !mProject )
  {
    return std::nan( "" );
  }

  QgsPointXY point( x, y );

  const QgsCoordinateReferenceSystem terrainCrs = mQgisTerrainProvider->crs();
  if ( terrainCrs.isValid() && terrainCrs != mProject->crs() )
  {
    try
    {
      QgsCoordinateTransform transform( mProject->crs(), terrainCrs, mProject->transformContext() );
      point = transform.transform( point );
    }
    catch ( const QgsCsException & )
    {
      return std::nan( "" );
    }
  }

  return mQgisTerrainProvider->heightAt( point.x(), point.y() );
}

void Quick3DTerrainProvider::updateTerrainProvider()
{
  if ( !mProject )
  {
    return;
  }

  mDemLayer = nullptr;
  mQgisTerrainProvider.reset();

  // Find DEM layer (single-band local raster)
  for ( QgsMapLayer *layer : mProject->mapLayers() )
  {
    QgsRasterLayer *raster = qobject_cast<QgsRasterLayer *>( layer );
    if ( !raster || !raster->isValid() )
    {
      continue;
    }

    const QString provider = raster->dataProvider() ? raster->dataProvider()->name() : QString();
    const bool isOnline = provider == QStringLiteral( "wms" ) || provider == QStringLiteral( "wmts" );

    if ( raster->bandCount() == 1 && !isOnline )
    {
      mDemLayer = raster;
      break;
    }
  }

  // Fallback to QGIS terrain provider
  if ( !mDemLayer )
  {
    QgsAbstractTerrainProvider *const terrain = mProject->elevationProperties()->terrainProvider();
    if ( terrain && terrain->type() != QStringLiteral( "flat" ) )
    {
      mQgisTerrainProvider.reset( terrain->clone() );
      mQgisTerrainProvider->prepare();
    }
  }

  emit hasDemLayerChanged();

  if ( ( mDemLayer || mQgisTerrainProvider ) && !mExtent.isEmpty() )
  {
    calcNormalizedData();
  }
}

bool Quick3DTerrainProvider::hasDemLayer() const
{
  return mDemLayer && mDemLayer->isValid();
}

bool Quick3DTerrainProvider::isLoading() const
{
  return mIsLoading;
}

int Quick3DTerrainProvider::loadingProgress() const
{
  return mLoadingProgress;
}

void Quick3DTerrainProvider::onTerrainDataCalculated()
{
  if ( !mFutureWatcher->isFinished() )
  {
    return;
  }

  const QVector<double> heights = mFutureWatcher->result();

  const std::pair<QVector<double>::const_iterator, QVector<double>::const_iterator> minmax = std::minmax_element( heights.begin(), heights.end() );
  mMinRealHeight = *minmax.first;
  mMaxRealHeight = *minmax.second;

  const double extentSize = std::max( mExtent.width(), mExtent.height() );
  const double scale = ( mTerrainBaseSize / extentSize ) * calculateVisualExaggeration();

  mNormalizedData.clear();
  mNormalizedData.reserve( heights.size() );
  for ( const double h : heights )
  {
    mNormalizedData.append( ( h - mMinRealHeight ) * scale );
  }

  mIsLoading = false;
  emit isLoadingChanged();

  mLoadingProgress = 100;
  emit loadingProgressChanged();

  emit normalizedDataChanged();
  emit terrainDataReady();
}
