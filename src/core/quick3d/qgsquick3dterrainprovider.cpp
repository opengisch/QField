/***************************************************************************
  qgsquick3dterrainprovider.cpp - QgsQuick3DTerrainProvider

 ---------------------
 begin                : 5.1.2026
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

#include "qgsquick3dterrainprovider.h"

#include <qgscoordinatetransform.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsprojectviewsettings.h>
#include <qgsrasterdataprovider.h>
#include <qgsrasterlayer.h>
#include <qgsterrainprovider.h>

#include <cmath>
#include <limits>

QgsQuick3DTerrainProvider::QgsQuick3DTerrainProvider( QObject *parent )
  : QObject( parent )
{
}

QgsQuick3DTerrainProvider::~QgsQuick3DTerrainProvider() = default;

QgsProject *QgsQuick3DTerrainProvider::project() const
{
  return mProject;
}

void QgsQuick3DTerrainProvider::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;
  emit projectChanged();
  updateTerrainProvider();
}

int QgsQuick3DTerrainProvider::resolution() const
{
  return mResolution;
}

void QgsQuick3DTerrainProvider::setResolution( int resolution )
{
  resolution = qBound( 8, resolution, 256 );
  if ( mResolution == resolution )
    return;

  mResolution = resolution;
  emit resolutionChanged();
}

QgsRectangle QgsQuick3DTerrainProvider::extent() const
{
  return mExtent;
}

void QgsQuick3DTerrainProvider::setExtent( const QgsRectangle &extent )
{
  if ( mExtent == extent )
    return;

  mExtent = extent;
  emit extentChanged();

  if ( mDemLayer && mDemLayer->isValid() )
  {
    calcData();
    emit terrainDataReady();
  }
}

double QgsQuick3DTerrainProvider::heightAt( double x, double y ) const
{
  if ( !mDemLayer || !mDemLayer->isValid() )
  {
    return 0.0;
  }

  const double height = sampleHeightFromRaster( mDemLayer, x, y );
  if ( !std::isnan( height ) )
  {
    return height;
  }

  return 0.0;
}

QVariantList QgsQuick3DTerrainProvider::normalizedData() const
{
  return mNormalizedData;
}

double QgsQuick3DTerrainProvider::normalizedHeightAt( double x, double y ) const
{
  const double realHeight = heightAt( x, y );

  // Apply same normalization as calcData()
  const double extentSize = qMax( mExtent.width(), mExtent.height() );
  const double realHeightScale = mTerrainBaseSize / extentSize;
  const double visualExaggeration = calculateVisualExaggeration();
  const double totalScale = realHeightScale * visualExaggeration;

  return ( realHeight - mMinRealHeight ) * totalScale;
}

double QgsQuick3DTerrainProvider::calculateVisualExaggeration() const
{
  const double extentSize = qMax( mExtent.width(), mExtent.height() );

  if ( extentSize > 100000 )
    return 3;
  else if ( extentSize > 50000 )
    return 2;
  else if ( extentSize > 10000 )
    return 1;
  else
    return 1;
}

void QgsQuick3DTerrainProvider::calcData()
{
  const int totalSize = mResolution * mResolution;
  mNormalizedData.clear();
  mNormalizedData.reserve( totalSize );
  if ( mExtent.isEmpty() )
  {
    for ( int i = 0; i < totalSize; ++i )
    {
      mNormalizedData.append( 0.0 );
    }
    return;
  }

  if ( !mDemLayer || !mDemLayer->isValid() )
  {
    for ( int i = 0; i < totalSize; ++i )
    {
      mNormalizedData.append( 0.0 );
    }
    return;
  }


  QVariantList tmpData;
  tmpData.reserve( totalSize );
  const double xStep = mExtent.width() / mResolution;
  const double yStep = mExtent.height() / mResolution;

  // Collect raw heights
  for ( int row = 0; row < mResolution; ++row )
  {
    const int flippedRow = mResolution - 1 - row;
    const double y = mExtent.yMinimum() + flippedRow * yStep;

    for ( int col = 0; col < mResolution; ++col )
    {
      const double x = mExtent.xMinimum() + col * xStep;
      double height = sampleHeightFromRaster( mDemLayer, x, y );

      if ( std::isnan( height ) )
      {
        height = 0.0;
      }

      tmpData.append( height );
    }
  }

  // Calculate min/max for normalization
  double minH = std::numeric_limits<double>::max();
  double maxH = std::numeric_limits<double>::lowest();

  for ( const QVariant &v : tmpData )
  {
    const double h = v.toDouble();
    minH = qMin( minH, h );
    maxH = qMax( maxH, h );
  }

  // Store real height range for bookmark positioning
  mMinRealHeight = minH;
  mMaxRealHeight = maxH;

  // Calculate scaling factors
  const double extentSize = qMax( mExtent.width(), mExtent.height() );
  const double realHeightScale = mTerrainBaseSize / extentSize;
  const double visualExaggeration = calculateVisualExaggeration();
  const double totalScale = realHeightScale * visualExaggeration;

  // Normalize heights
  for ( const QVariant &v : tmpData )
  {
    const double height = v.toDouble();
    const double normalized = ( height - minH ) * totalScale;
    mNormalizedData.append( normalized );
  }

  emit normalizedDataChanged();
}

double QgsQuick3DTerrainProvider::sampleHeightFromRaster( QgsRasterLayer *layer, double x, double y ) const
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
      const QgsCoordinateTransform transform( mProject->crs(), layer->crs(), mProject->transformContext() );
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
  const double value = layer->dataProvider()->sample( point, 1, &ok );
  return ok ? value : std::nan( "" );
}

QVariantMap QgsQuick3DTerrainProvider::terrainStats() const
{
  QVariantMap stats;

  stats[QStringLiteral( "minHeight" )] = 0.0;
  stats[QStringLiteral( "maxHeight" )] = 0.0;

  if ( mExtent.isEmpty() )
  {
    return stats;
  }

  double minH = std::numeric_limits<double>::max();
  double maxH = std::numeric_limits<double>::lowest();

  for ( const QVariant &data : mNormalizedData )
  {
    double height = data.toDouble();
    minH = qMin( minH, height );
    maxH = qMax( maxH, height );
  }

  if ( mNormalizedData.size() > 0 )
  {
    stats[QStringLiteral( "minHeight" )] = minH;
    stats[QStringLiteral( "maxHeight" )] = maxH;
  }

  return stats;
}

void QgsQuick3DTerrainProvider::updateTerrainProvider()
{
  if ( !mProject )
  {
    return;
  }

  // Finding dem layer (same as QgsQuick3DMapTextureGenerator)
  const QMap<QString, QgsMapLayer *> layers = mProject->mapLayers();

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
      mDemLayer = rasterLayer;
      break;
    }
  }

  if ( mDemLayer && mDemLayer->isValid() && !mExtent.isEmpty() )
  {
    calcData();
    emit terrainDataReady();
  }
}

int QgsQuick3DTerrainProvider::terrainBaseSize() const
{
  return mTerrainBaseSize;
}
