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

#include "quick3dterrainprovider.h"

#include <QCoreApplication>
#include <QDebug>
#include <qgscoordinatetransform.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsrasterdataprovider.h>
#include <qgsrasterlayer.h>
#include <qgsterrainprovider.h>

#include <algorithm>
#include <cmath>

Quick3DTerrainProvider::Quick3DTerrainProvider( QObject *parent )
  : QObject( parent )
{
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

int Quick3DTerrainProvider::resolution() const
{
  return mResolution;
}

void Quick3DTerrainProvider::setResolution( int resolution )
{
  resolution = qBound( 8, resolution, 256 );
  if ( mResolution == resolution )
  {
    return;
  }

  mResolution = resolution;
  emit resolutionChanged();
}

QgsRectangle Quick3DTerrainProvider::extent() const
{
  return mExtent;
}

void Quick3DTerrainProvider::setExtent( const QgsRectangle &extent )
{
  if ( mExtent == extent )
  {
    return;
  }

  mExtent = extent;
  emit extentChanged();

  if ( ( mDemLayer && mDemLayer->isValid() ) || mQgisTerrainProvider )
  {
    calcNormalizedData();
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
  if ( mExtent.isEmpty() )
  {
    return 0.0;
  }

  const double realHeight = heightAt( x, y );
  const double extentSize = qMax( mExtent.width(), mExtent.height() );
  const double scale = ( mTerrainBaseSize / extentSize ) * calculateVisualExaggeration();
  return ( realHeight - mMinRealHeight ) * scale;
}

double Quick3DTerrainProvider::calculateVisualExaggeration() const
{
  const double extentSize = qMax( mExtent.width(), mExtent.height() );
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

  // Early exit for empty/invalid state
  if ( mExtent.isEmpty() || ( !mDemLayer && !mQgisTerrainProvider ) )
  {
    mNormalizedData.fill( 0.0, totalSamples );
    emit normalizedDataChanged();
    emit terrainDataReady();
    return;
  }

  mIsLoading = true;
  emit isLoadingChanged();

  mLoadingProgress = 0;
  emit loadingProgressChanged();

  // Sample heights - use (resolution-1) to cover full extent edge-to-edge
  QVector<double> heights;
  heights.reserve( totalSamples );

  const double xStep = mExtent.width() / ( mResolution - 1 );
  const double yStep = mExtent.height() / ( mResolution - 1 );

  double lastValidHeight = 0.0;

  for ( int row = 0; row < mResolution; ++row )
  {
    for ( int col = 0; col < mResolution; ++col )
    {
      const double x = mExtent.xMinimum() + col * xStep;
      const double y = mExtent.yMaximum() - row * yStep;
      double h = heightAt( x, y );

      if ( std::isnan( h ) )
      {
        h = lastValidHeight;
      }
      else
      {
        lastValidHeight = h;
      }

      heights.append( h );
    }

    mLoadingProgress = ( row * mResolution * 100 ) / totalSamples;
    emit loadingProgressChanged();
  }

  // Find min/max
  const std::pair<QVector<double>::const_iterator, QVector<double>::const_iterator> minmax = std::minmax_element( heights.begin(), heights.end() );
  mMinRealHeight = *minmax.first;
  mMaxRealHeight = *minmax.second;

  // Normalize
  const double extentSize = qMax( mExtent.width(), mExtent.height() );
  const double scale = ( mTerrainBaseSize / extentSize ) * calculateVisualExaggeration();

  mNormalizedData.clear();
  mNormalizedData.reserve( totalSamples );
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
