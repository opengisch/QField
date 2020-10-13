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

#include <qgslayertreegroup.h>
#include <qgslayertree.h>
#include <qgsmaplayer.h>
#include <qgslayertreeutils.h>
#include <qgsmaplayerstylemanager.h>
#include <qgslayertreemodel.h>

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

  connect( mTrackingModel, &TrackingModel::layerInTrackingChanged, this, &LayerTreeMapCanvasBridge::layerInTrackingChanged );

  connect( mMapSettings, &QgsQuickMapSettings::extentChanged, this, &LayerTreeMapCanvasBridge::extentChanged );
  setCanvasLayers();
}

void LayerTreeMapCanvasBridge::extentChanged()
{
  // allow symbols in the legend update their preview if they use map units
  mModel->layerTreeModel()->setLegendMapViewData( mMapSettings->mapSettings().mapUnitsPerPixel(),
      static_cast< int >( std::round( mMapSettings->outputDpi() ) ), mMapSettings->mapSettings().scale() );
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
  int currentSpatialLayerCount = 0;
  for ( QgsLayerTreeLayer *layerNode : layerNodes )
  {
    if ( layerNode->layer() && layerNode->layer()->isSpatial() )
      currentSpatialLayerCount++;
  }

  bool firstLayers = mAutoSetupOnFirstLayer && !mHasLayersLoaded && currentSpatialLayerCount != 0;

  mMapSettings->setLayers( canvasLayers );

  if ( !mFirstCRS.isValid() )
  {
    // find out what is the first used CRS in case we may need to turn on OTF projections later
    for ( QgsLayerTreeLayer *layerNode : layerNodes )
    {
      if ( layerNode->layer() && layerNode->layer()->crs().isValid() )
      {
        mFirstCRS = layerNode->layer()->crs();
        break;
      }
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
        mTrackingModel->setLayerVisible( layer, nodeLayer->isVisible() );
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
  QgsProject::instance()->mapThemeCollection()->applyTheme( mModel->mapTheme(), mRoot, mModel->layerTreeModel() );
  // rebuilt the flat layer tree model
}

void LayerTreeMapCanvasBridge::layerInTrackingChanged( QgsVectorLayer *layer, bool tracking )
{
  QgsLayerTreeLayer *nodeLayer = mRoot->findLayer( layer->id() );
  mModel->setLayerInTracking( nodeLayer, tracking );
}




