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

QSize Quick3DTerrainProvider::gridSize() const
{
  return mGridSize;
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

  calculateResolution();

  if ( changed && ( ( mDemLayer && mDemLayer->isValid() ) || mQgisTerrainProvider ) )
  {
    calcNormalizedData();
  }
}

void Quick3DTerrainProvider::calculateResolution()
{
  if ( mExtent.isEmpty() )
  {
    if ( mGridSize != QSize( 32, 32 ) )
    {
      mGridSize = QSize( 32, 32 );
      emit gridSizeChanged();
    }
    return;
  }

  int baseResolution = 320;

  const double extentWidth = mExtent.width();
  const double extentHeight = mExtent.height();
  int newWidth, newHeight;

  if ( extentWidth >= extentHeight )
  {
    newWidth = baseResolution;
    newHeight = std::max( 16, static_cast<int>( baseResolution * ( extentHeight / extentWidth ) ) );
  }
  else
  {
    newHeight = baseResolution;
    newWidth = std::max( 16, static_cast<int>( baseResolution * ( extentWidth / extentHeight ) ) );
  }

  QSize newGridSize( newWidth, newHeight );
  if ( mGridSize != newGridSize )
  {
    mGridSize = newGridSize;
    emit gridSizeChanged();
  }
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
  if ( mExtent.isEmpty() || ( !mDemLayer && !mQgisTerrainProvider ) )
  {
    mNormalizedData.fill( 0.0, mGridSize.width() * mGridSize.height() );
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

  QgsRasterDataProvider *provider = nullptr;
  QgsCoordinateReferenceSystem rasterCrs;
  double scale = 1.0;
  double offset = 0.0;

  if ( mDemLayer && mDemLayer->isValid() && mDemLayer->dataProvider() )
  {
    provider = mDemLayer->dataProvider()->clone();
    rasterCrs = mDemLayer->crs();
  }
  else if ( mQgisTerrainProvider )
  {
    QgsRasterDemTerrainProvider *demProvider = dynamic_cast<QgsRasterDemTerrainProvider *>( mQgisTerrainProvider.get() );
    if ( demProvider && demProvider->layer() && demProvider->layer()->dataProvider() )
    {
      provider = demProvider->layer()->dataProvider()->clone();
      rasterCrs = demProvider->layer()->crs();
      scale = demProvider->scale();
      offset = demProvider->offset();
    }
  }

  if ( !provider )
  {
    mNormalizedData.fill( 0.0, mGridSize.width() * mGridSize.height() );
    emit normalizedDataChanged();
    emit terrainDataReady();
    mIsLoading = false;
    emit isLoadingChanged();
    return;
  }

  QgsRectangle extent = mExtent;
  QSize gridSize = mGridSize;
  QgsCoordinateReferenceSystem projectCrs = mProject ? mProject->crs() : QgsCoordinateReferenceSystem();
  QgsCoordinateTransformContext transformContext = mProject ? mProject->transformContext() : QgsCoordinateTransformContext();

  QFuture<QVector<double>> future = QtConcurrent::run( [provider, extent, gridSize, rasterCrs, projectCrs, transformContext, scale, offset]() {
    QVector<double> heights( gridSize.width() * gridSize.height(), 0.0 );

    QgsRectangle blockExtent = extent;
    if ( rasterCrs.isValid() && projectCrs.isValid() && rasterCrs != projectCrs )
    {
      try
      {
        QgsCoordinateTransform transform( projectCrs, rasterCrs, transformContext );
        blockExtent = transform.transformBoundingBox( extent );
      }
      catch ( const QgsCsException & )
      {
      }
    }

    std::unique_ptr<QgsRasterBlock> block( provider->block( 1, blockExtent, gridSize.width(), gridSize.height() ) );
    if ( block && block->isValid() )
    {
      for ( int row = 0; row < gridSize.height(); ++row )
      {
        for ( int col = 0; col < gridSize.width(); ++col )
        {
          bool isNoData = false;
          double value = block->valueAndNoData( row, col, isNoData );
          if ( !isNoData && !std::isnan( value ) )
          {
            heights[row * gridSize.width() + col] = value * scale + offset;
          }
        }
      }
    }

    delete provider;
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

  emit normalizedDataChanged();
  emit terrainDataReady();
}
