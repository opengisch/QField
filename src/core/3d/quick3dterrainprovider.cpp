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
  connect( mFutureWatcher, &QFutureWatcher<QVector<double>>::finished, this, &Quick3DTerrainProvider::onTerrainDataCalculated );
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

  mMapSettings = mapSettings;
  emit mapSettingsChanged();

  updateFromMapSettings();
}

bool Quick3DTerrainProvider::forceSquareSize() const
{
  return mForceSquareSize;
}

void Quick3DTerrainProvider::setForceSquareSize( bool forceSquareSize )
{
  if ( mForceSquareSize == forceSquareSize )
  {
    return;
  }

  mForceSquareSize = forceSquareSize;
  emit forceSquareSizeChanged();

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

QSizeF Quick3DTerrainProvider::size() const
{
  return mSize;
}

void Quick3DTerrainProvider::updateFromMapSettings()
{
  if ( !mMapSettings || !mProject )
  {
    return;
  }

  QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  if ( mForceSquareSize )
  {
    if ( visibleExtent.width() >= visibleExtent.height() )
    {
      const double adjustement = ( visibleExtent.width() - visibleExtent.height() ) / 2;
      visibleExtent.setYMinimum( visibleExtent.yMinimum() - adjustement );
      visibleExtent.setYMaximum( visibleExtent.yMaximum() + adjustement );
    }
    else
    {
      const double adjustement = ( visibleExtent.height() - visibleExtent.width() ) / 2;
      visibleExtent.setXMinimum( visibleExtent.xMinimum() - adjustement );
      visibleExtent.setXMaximum( visibleExtent.xMaximum() + adjustement );
    }
  }

#if 0
  QgsRectangle visibleExtent = mMapSettings->visibleExtent();

  if ( mTerrainProvider )
  {
    QgsMapLayer *terrainLayer = nullptr;
    if ( QgsRasterDemTerrainProvider *rasterDemTerrainProvider = dynamic_cast<QgsRasterDemTerrainProvider *>( mTerrainProvider.get() ) )
    {
      terrainLayer = rasterDemTerrainProvider->layer();
    }
    else if ( QgsMeshTerrainProvider *meshTerrainProvider = dynamic_cast<QgsMeshTerrainProvider *>( mTerrainProvider.get() ) )
    {
      terrainLayer = meshTerrainProvider->layer();
    }

    if ( terrainLayer && terrainLayer->isValid() )
    {
      QgsRectangle terrainExtent = terrainLayer->extent();

      if ( terrainLayer->crs() != mProject->crs() )
      {
        try
        {
          QgsCoordinateTransform transform( terrainLayer->crs(), mProject->crs(), mProject->transformContext() );
          terrainExtent = transform.transformBoundingBox( terrainExtent );
        }
        catch ( const QgsCsException & )
        {
          terrainExtent = QgsRectangle();
        }
      }

      QgsRectangle intersection = visibleExtent.intersect( terrainExtent );
      if ( !intersection.isEmpty() )
      {
        visibleExtent = intersection;
      }
    }
  }
#endif

  bool changed = mExtent != visibleExtent;
  if ( changed )
  {
    mExtent = visibleExtent;
    if ( mExtent.width() >= mExtent.height() )
    {
      mSize = QSizeF( mBaseSize, mExtent.height() * mBaseSize / mExtent.width() );
    }
    else
    {
      mSize = QSizeF( mExtent.width() * mBaseSize / mExtent.height(), mBaseSize );
    }
    emit extentChanged();
  }

  calculateResolution();

  if ( changed && mTerrainProvider )
  {
    calcNormalizedData();
  }
}

void Quick3DTerrainProvider::calculateResolution()
{
  if ( mExtent.isEmpty() )
  {
    if ( mGridSize != QSize( 64, 64 ) )
    {
      mGridSize = QSize( 64, 64 );
      emit gridSizeChanged();
    }
    return;
  }

  int baseResolution = 320;

  const double width = mSize.width();
  const double height = mSize.height();
  int newGridWidth, newGridHeight;

  if ( width >= height )
  {
    newGridWidth = baseResolution;
    newGridHeight = std::max( 16, static_cast<int>( baseResolution * ( height / width ) ) );
  }
  else
  {
    newGridWidth = std::max( 16, static_cast<int>( baseResolution * ( width / height ) ) );
    newGridHeight = baseResolution;
  }

  QSize newGridSize( newGridWidth, newGridHeight );
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

double Quick3DTerrainProvider::heightAt( double x, double y ) const
{
  if ( mTerrainProvider )
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
  const double scale = ( mBaseSize / extentSize ) * calculateVisualExaggeration();
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
  if ( mExtent.isEmpty() || !mTerrainProvider )
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

  QgsAbstractTerrainProvider *terrainProvider;
  QgsCoordinateReferenceSystem terrainCrs;
  QgsRasterDataProvider *rasterProvider = nullptr;
  double scale = 1.0;
  double offset = 0.0;

  if ( mTerrainProvider )
  {
    terrainCrs = mTerrainProvider->crs();
    scale = mTerrainProvider->scale();
    offset = mTerrainProvider->offset();

    if ( QgsRasterDemTerrainProvider *rasterDemProvider = dynamic_cast<QgsRasterDemTerrainProvider *>( mTerrainProvider.get() ) )
    {
      QgsRasterLayer *layer = rasterDemProvider->layer();
      if ( layer && layer->dataProvider() )
      {
        rasterProvider = layer->dataProvider()->clone();
      }
    }
    else
    {
      terrainProvider = mTerrainProvider->clone();
      terrainProvider->prepare();
    }
  }

  if ( !rasterProvider && !terrainProvider )
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

  QFuture<QVector<double>> future = QtConcurrent::run( [terrainProvider, rasterProvider, extent, gridSize, terrainCrs, projectCrs, transformContext, scale, offset]() {
    QVector<double> heights( gridSize.width() * gridSize.height(), 0.0 );

    QgsRectangle blockExtent = extent;
    if ( terrainCrs.isValid() && projectCrs.isValid() && terrainCrs != projectCrs )
    {
      try
      {
        QgsCoordinateTransform transform( projectCrs, terrainCrs, transformContext );
        blockExtent = transform.transformBoundingBox( extent );
      }
      catch ( const QgsCsException & )
      {
      }
    }

    if ( rasterProvider )
    {
      std::unique_ptr<QgsRasterBlock> block( rasterProvider->block( 1, blockExtent, gridSize.width(), gridSize.height() ) );
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
      delete rasterProvider;
    }
    else if ( terrainProvider )
    {
      for ( int row = 0; row < gridSize.height(); ++row )
      {
        for ( int col = 0; col < gridSize.width(); ++col )
        {
          const double x = extent.xMinimum() + extent.width() / gridSize.width() * col;
          const double y = extent.xMinimum() - extent.height() / gridSize.height() * row;
          double value = terrainProvider->heightAt( x, y );
          if ( !std::isnan( value ) )
          {
            heights[row * gridSize.width() + col] = value * scale + offset;
          }
        }
      }
      delete terrainProvider;
    }

    return heights;
  } );

  mFutureWatcher->setFuture( future );
}

double Quick3DTerrainProvider::sampleHeightFromTerrainProvider( double x, double y ) const
{
  if ( !mTerrainProvider || !mProject )
  {
    return std::numeric_limits<double>::quiet_NaN();
  }

  QgsPointXY point( x, y );
  const QgsCoordinateReferenceSystem terrainCrs = mTerrainProvider->crs();
  if ( terrainCrs.isValid() && terrainCrs != mProject->crs() )
  {
    try
    {
      QgsCoordinateTransform transform( mProject->crs(), terrainCrs, mProject->transformContext() );
      point = transform.transform( point );
    }
    catch ( const QgsCsException & )
    {
      return std::numeric_limits<double>::quiet_NaN();
    }
  }

  return mTerrainProvider->heightAt( point.x(), point.y() );
}

void Quick3DTerrainProvider::updateTerrainProvider()
{
  mTerrainProvider.reset();

  if ( !mProject )
  {
    return;
  }

  QgsAbstractTerrainProvider *const terrain = mProject->elevationProperties()->terrainProvider();
  if ( terrain && terrain->type() != QStringLiteral( "flat" ) )
  {
    mTerrainProvider.reset( terrain->clone() );
    mTerrainProvider->prepare();
  }

  if ( mTerrainProvider && !mExtent.isEmpty() )
  {
    calcNormalizedData();
  }
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
  const double scale = ( mBaseSize / extentSize ) * calculateVisualExaggeration();

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
