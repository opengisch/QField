

# File legendimageprovider.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**legendimageprovider.cpp**](legendimageprovider_8cpp.md)

[Go to the documentation of this file](legendimageprovider_8cpp.md)


```C++
/***************************************************************************
  legendimageprovider.cpp - LegendImageProvider

 ---------------------
 begin                : 7.12.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "legendimageprovider.h"
#include "qgsapplication.h"
#include "qgsimagecache.h"

#include <QQuickTextureFactory>
#include <qgslayertree.h>
#include <qgslayertreelayer.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemodellegendnode.h>
#include <qgsproject.h>
#include <qgsrasterlayer.h>

LegendImageProvider::LegendImageProvider( QgsLayerTreeModel *layerTreeModel )
  : QQuickImageProvider( Pixmap )
  , mLayerTreeModel( layerTreeModel )
  , mRootNode( layerTreeModel->rootGroup() )
{
}

QPixmap LegendImageProvider::requestPixmap( const QString &id, QSize *, const QSize & )
{
  const int iconSize = mLayerTreeModel->scaleIconSize( 16 );

  // the id is passed on as an encoded URL string which needs decoding
  const QString decodedId = QUrl::fromPercentEncoding( id.toUtf8() );
  QStringList idParts = decodedId.split( '/' ).mid( 0, 2 );
  idParts << decodedId.section( '/', 2 );

  if ( idParts.value( 0 ) == QStringLiteral( "legend" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );
    QModelIndex layerIndex = mLayerTreeModel->node2index( layerNode );
    QModelIndex index = mLayerTreeModel->index( 0, 0, layerIndex );
    QStringList legendParts;
    while ( index.isValid() )
    {
      legendParts << QString::number( index.internalId() );
      if ( idParts.value( 2 ) == legendParts.join( QStringLiteral( "~__~" ) ) )
      {
        QPixmap pixmap = mLayerTreeModel->data( index, Qt::DecorationRole ).value<QPixmap>();
        if ( pixmap.isNull() )
        {
          QIcon icon = mLayerTreeModel->data( index, Qt::DecorationRole ).value<QIcon>();
          if ( !icon.isNull() )
            pixmap = icon.pixmap( iconSize, iconSize );
        }
        if ( pixmap.isNull() )
        {
          pixmap = QPixmap( iconSize, iconSize );
          pixmap.fill( QColor( 255, 255, 255 ) );
        }
        return pixmap;
      }
      else
      {
        if ( mLayerTreeModel->hasChildren( index ) )
        {
          index = mLayerTreeModel->index( 0, 0, index );
        }
        else
        {
          legendParts.removeLast();

          QModelIndex nextIndex = mLayerTreeModel->sibling( index.row() + 1, 0, index );
          while ( !nextIndex.isValid() && index != layerIndex )
          {
            legendParts.removeLast();
            index = index.parent();
            nextIndex = mLayerTreeModel->sibling( index.row() + 1, 0, index );
          }
          index = nextIndex;
        }
      }
    }
  }
  else if ( idParts.value( 0 ) == QStringLiteral( "layer" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );
    if ( layerNode )
    {
      QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->legendNodeEmbeddedInParent( layerNode );
      if ( legendNode )
      {
        QPixmap pixmap = legendNode->data( Qt::DecorationRole ).value<QPixmap>();
        if ( pixmap.isNull() )
        {
          QIcon icon = legendNode->data( Qt::DecorationRole ).value<QIcon>();
          if ( !icon.isNull() )
            pixmap = icon.pixmap( iconSize, iconSize );
        }
        if ( pixmap.isNull() )
        {
          pixmap = QPixmap( iconSize, iconSize );
          pixmap.fill( QColor( 255, 255, 255 ) );
        }
        return pixmap;
      }
      else
      {
        QPixmap pixmap( iconSize, iconSize );
        pixmap.fill( QColor( 255, 255, 255 ) );
        return pixmap;
      }
    }
  }
  else if ( idParts.value( 0 ) == QStringLiteral( "image" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );
    if ( layerNode )
    {
      QgsMapLayer *mapLayer = layerNode->layer();
      if ( mapLayer && !mapLayer->legendPlaceholderImage().isEmpty() )
      {
        bool fitsInCache = false;
        const QImage image = QgsApplication::imageCache()->pathAsImage( mapLayer->legendPlaceholderImage(), QSize(), false, 1.0, fitsInCache );
        return QPixmap::fromImage( image );
      }
    }
  }

  return QPixmap( QSize( iconSize, iconSize ) );
}


AsyncLegendImageProvider::AsyncLegendImageProvider( QgsLayerTreeModel *layerTreeModel )
  : mLayerTreeModel( layerTreeModel )
  , mRootNode( layerTreeModel->rootGroup() )
{
}

void AsyncLegendImageProvider::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  mMapSettings = mapSettings;
}

QQuickImageResponse *AsyncLegendImageProvider::requestImageResponse( const QString &id, const QSize &requestedSize )
{
  // the id is passed on as an encoded URL string which needs decoding
  const QString decodedId = QUrl::fromPercentEncoding( id.toUtf8() );
  QStringList idParts = decodedId.split( '/' ).mid( 0, 2 );
  idParts << decodedId.section( '/', 2 );

  if ( idParts.value( 0 ) == QStringLiteral( "layer" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );
    if ( layerNode )
    {
      QgsRasterLayer *rasterLayer = qobject_cast<QgsRasterLayer *>( layerNode->layer() );
      if ( rasterLayer && rasterLayer->dataProvider() && rasterLayer->dataProvider()->supportsLegendGraphic() )
      {
        QgsMapSettings mapSettings;
        if ( mMapSettings )
        {
          mapSettings = mMapSettings->mapSettings();
        }
        AsyncLegendImageResponse *response = new AsyncLegendImageResponse( rasterLayer->dataProvider()->clone(), mMapSettings ? &mapSettings : nullptr );
        return response;
      }
    }
  }

  AsyncLegendImageResponse *response = new AsyncLegendImageResponse();
  return response;
}


AsyncLegendImageResponse::AsyncLegendImageResponse( QgsRasterDataProvider *dataProvider, const QgsMapSettings *mapSettings )
  : mDataProvider( dataProvider )
{
  if ( dataProvider )
  {
    mFetcher.reset( mDataProvider->getLegendGraphicFetcher( mapSettings ) );
    if ( mFetcher )
    {
      mFetcher->setParent( nullptr );
      connect( mFetcher.get(), &QgsImageFetcher::finish, this, &AsyncLegendImageResponse::handleFinish );
      connect( mFetcher.get(), &QgsImageFetcher::error, this, &AsyncLegendImageResponse::handleError );
      mFetcher->start();
      return;
    }
  }

  mImage = QImage();
  emit finished();
}

void AsyncLegendImageResponse::handleFinish( const QImage &image )
{
  if ( !mFetcher )
  {
    // invalid signal, QGIS tells us it must be coming after error
    return;
  }

  mImage = image;
  emit finished();

  mFetcher->deleteLater();
  mFetcher.release();
}

void AsyncLegendImageResponse::handleError( const QString & )
{
  if ( !mFetcher )
  {
    // invalid signal, QGIS tells us it must be coming after finish
    return;
  }

  mImage = QImage();
  emit finished();

  mFetcher->deleteLater();
  mFetcher.release();
}

QQuickTextureFactory *AsyncLegendImageResponse::textureFactory() const
{
  return QQuickTextureFactory::textureFactoryForImage( mImage );
}
```


