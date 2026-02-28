

# File layertreemapcanvasbridge.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**layertreemapcanvasbridge.cpp**](layertreemapcanvasbridge_8cpp.md)

[Go to the documentation of this file](layertreemapcanvasbridge_8cpp.md)


```C++
/***************************************************************************
  layertreemapcanvasbridge.cpp - LayerTreeMapCanvasBridge

 ---------------------
 begin                : 26.8.2016
 copyright            : (C) 2016 by Matthias Kuhn, OPENGIS.ch
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "layertreemapcanvasbridge.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"

#include <qgslayertree.h>
#include <qgslayertreegroup.h>
#include <qgslayertreemodel.h>
#include <qgslayertreeutils.h>
#include <qgsmaplayer.h>
#include <qgsmaplayerstylemanager.h>

LayerTreeMapCanvasBridge::LayerTreeMapCanvasBridge( FlatLayerTreeModel *model, QgsQuickMapSettings *mapSettings, TrackingModel *trackingModel, QObject *parent )
  : QObject( parent )
  , mRoot( model->layerTree() )
  , mModel( model )
  , mMapSettings( mapSettings )
  , mTrackingModel( trackingModel )
  , mPendingCanvasUpdate( false )
  , mHasCustomLayerOrder( false )
  , mAutoSetupOnFirstLayer( false )
  , mAutoEnableCrsTransform( true )
  , mHasLayersLoaded( !mRoot->findLayers().isEmpty() )
{
  connect( mRoot, &QgsLayerTreeGroup::visibilityChanged, this, &LayerTreeMapCanvasBridge::nodeVisibilityChanged );
  connect( model, &FlatLayerTreeModel::mapThemeChanged, this, &LayerTreeMapCanvasBridge::mapThemeChanged );
  connect( model, &FlatLayerTreeModel::layersAdded, this, &LayerTreeMapCanvasBridge::layersChanged );
  connect( model, &FlatLayerTreeModel::layersRemoved, this, &LayerTreeMapCanvasBridge::layersChanged );

  connect( mTrackingModel, &TrackingModel::layerInTrackingChanged, this, &LayerTreeMapCanvasBridge::layerInTrackingChanged );

  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &LayerTreeMapCanvasBridge::extentChanged );
  setCanvasLayers();
}

void LayerTreeMapCanvasBridge::extentChanged()
{
  // allow symbols in the legend update their preview if they use map units
  mModel->layerTreeModel()->setLegendMapViewData( mMapSettings->mapSettings().mapUnitsPerPixel(),
                                                  static_cast<int>( std::round( mMapSettings->outputDpi() ) ), mMapSettings->mapSettings().scale() );
}

void LayerTreeMapCanvasBridge::setCanvasLayers()
{
  QList<QgsMapLayer *> canvasLayers, allLayerOrder;

  if ( mRoot->hasCustomLayerOrder() )
  {
    const QList<QgsMapLayer *> customOrderLayers = mRoot->customLayerOrder();
    for ( const QgsMapLayer *layer : customOrderLayers )
    {
      QgsLayerTreeLayer *nodeLayer = mRoot->findLayer( layer->id() );
      if ( nodeLayer )
      {
        if ( !nodeLayer->layer()->isSpatial() )
          continue;

        allLayerOrder << nodeLayer->layer();
        if ( nodeLayer->isVisible() )
          canvasLayers << nodeLayer->layer();
      }
    }
  }
  else
  {
    setCanvasLayers( mRoot, canvasLayers, allLayerOrder );
  }

  const QList<QgsLayerTreeLayer *> layerNodes = mRoot->findLayers();
  int currentSpatialLayerCount = static_cast<int>( std::count_if( layerNodes.begin(), layerNodes.end(), []( QgsLayerTreeLayer *layerNode ) {
    return layerNode->layer() && layerNode->layer()->isSpatial();
  } ) );
  bool firstLayers = mAutoSetupOnFirstLayer && !mHasLayersLoaded && currentSpatialLayerCount != 0;

  mMapSettings->setLayers( canvasLayers );

  if ( !mFirstCRS.isValid() )
  {
    // find out what is the first used CRS in case we may need to turn on OTF projections later
    auto match = std::find_if( layerNodes.begin(), layerNodes.end(), []( QgsLayerTreeLayer *layerNode ) {
      return layerNode->layer() && layerNode->layer()->crs().isValid();
    } );
    if ( match != layerNodes.end() )
    {
      mFirstCRS = ( *match )->layer()->crs();
    }
  }

  if ( mFirstCRS.isValid() && firstLayers )
  {
    mMapSettings->setDestinationCrs( mFirstCRS );
    QgsProject::instance()->setCrs( mFirstCRS );
  }

  mHasLayersLoaded = currentSpatialLayerCount;
  if ( currentSpatialLayerCount == 0 )
    mFirstCRS = QgsCoordinateReferenceSystem();

  mPendingCanvasUpdate = false;
}

void LayerTreeMapCanvasBridge::setCanvasLayers( QgsLayerTreeNode *node, QList<QgsMapLayer *> &canvasLayers, QList<QgsMapLayer *> &allLayers )
{
  if ( QgsLayerTree::isLayer( node ) )
  {
    QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
    if ( nodeLayer->layer() && nodeLayer->layer()->isSpatial() )
    {
      allLayers << nodeLayer->layer();
      if ( nodeLayer->isVisible() )
      {
        canvasLayers << nodeLayer->layer();
      }
      QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      if ( layer )
      {
        mTrackingModel->setTrackerVisibility( layer, nodeLayer->isVisible() );
      }
    }
  }

  const QList<QgsLayerTreeNode *> children = node->children();
  for ( QgsLayerTreeNode *child : children )
    setCanvasLayers( child, canvasLayers, allLayers );
}

void LayerTreeMapCanvasBridge::deferredSetCanvasLayers()
{
  if ( mPendingCanvasUpdate )
    return;

  mPendingCanvasUpdate = true;
  QMetaObject::invokeMethod( this, "setCanvasLayers", Qt::QueuedConnection );
}

void LayerTreeMapCanvasBridge::nodeVisibilityChanged()
{
  deferredSetCanvasLayers();
}


void LayerTreeMapCanvasBridge::mapThemeChanged()
{
  if ( !mModel->mapTheme().isEmpty() )
    QgsProject::instance()->mapThemeCollection()->applyTheme( mModel->mapTheme(), mRoot, mModel->layerTreeModel() );
}

void LayerTreeMapCanvasBridge::layersChanged()
{
  deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::layerInTrackingChanged( QgsVectorLayer *layer, bool tracking )
{
  if ( layer )
  {
    QgsLayerTreeLayer *nodeLayer = mRoot->findLayer( layer->id() );
    if ( layer->geometryType() == Qgis::GeometryType::Point )
    {
      // Disable feature count while tracking to avoid needless CPU cycles wasted updating a collapsed legend
      if ( tracking )
      {
        QVariant showFeatureCountValue = nodeLayer->customProperty( QStringLiteral( "showFeatureCount" ) );
        if ( showFeatureCountValue.isValid() && showFeatureCountValue.toInt() != 0 )
        {
          nodeLayer->setCustomProperty( QStringLiteral( "showFeatureCount" ), 0 );
          nodeLayer->setCustomProperty( QStringLiteral( "previousShowFeatureCount" ), showFeatureCountValue );
        }
      }
      else
      {
        QVariant previousShowFeatureCount = nodeLayer->customProperty( QStringLiteral( "previousShowFeatureCount" ) );
        if ( previousShowFeatureCount.isValid() )
        {
          nodeLayer->setCustomProperty( QStringLiteral( "showFeatureCount" ), previousShowFeatureCount );
        }
      }
    }
    mModel->setLayerInTracking( nodeLayer, tracking );
  }
}
```


