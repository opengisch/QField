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

LayerTreeMapCanvasBridge::LayerTreeMapCanvasBridge( LayerTreeModel *model, QgsQuickMapSettings *mapSettings, QObject *parent )
  : QObject( parent )
  , mModel( model )
  , mRoot( model->layerTree() )
  , mMapSettings( mapSettings )
  , mPendingCanvasUpdate( false )
  , mHasCustomLayerOrder( false )
  , mAutoSetupOnFirstLayer( false )
  , mAutoEnableCrsTransform( true )
  , mHasLayersLoaded( !mRoot->findLayers().isEmpty() )
{
  connect( mRoot, &QgsLayerTreeGroup::visibilityChanged, this, &LayerTreeMapCanvasBridge::nodeVisibilityChanged );
  connect( model, &LayerTreeModel::mapThemeChanged, this, &LayerTreeMapCanvasBridge::mapThemeChanged );

  setCanvasLayers();
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
    Q_FOREACH ( QgsLayerTreeLayer *layerNode, layerNodes )
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
        canvasLayers << nodeLayer->layer();
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
}


bool LayerTreeMapCanvasBridge::findRecordForLayer( QgsMapLayer *layer, const QgsMapThemeCollection::MapThemeRecord &rec, QgsMapThemeCollection::MapThemeLayerRecord &layerRec )
{
  Q_FOREACH ( const QgsMapThemeCollection::MapThemeLayerRecord &lr, rec.layerRecords() )
  {
    if ( lr.layer() == layer )
    {
      layerRec = lr;
      return true;
    }
  }
  return false;
}

void LayerTreeMapCanvasBridge::applyThemeToLayer( QgsLayerTreeLayer *nodeLayer, const QgsMapThemeCollection::MapThemeRecord &rec )
{
  QgsMapThemeCollection::MapThemeLayerRecord layerRec;
  bool isVisible = findRecordForLayer( nodeLayer->layer(), rec, layerRec );

  nodeLayer->setItemVisibilityChecked( isVisible );

  if ( !isVisible )
    return;

  if ( layerRec.usingCurrentStyle )
  {
    // apply desired style first
    nodeLayer->layer()->styleManager()->setCurrentStyle( layerRec.currentStyle );
  }
#if 0
  if ( layerRec.usingLegendItems )
  {
    // some nodes are not checked
    Q_FOREACH ( QgsLayerTreeModelLegendNode *legendNode, model->layerLegendNodes( nodeLayer, true ) )
    {
      QString ruleKey = legendNode->data( QgsLayerTreeModelLegendNode::RuleKeyRole ).toString();
      Qt::CheckState shouldHaveState = layerRec.checkedLegendItems.contains( ruleKey ) ? Qt::Checked : Qt::Unchecked;
      if ( ( legendNode->flags() & Qt::ItemIsUserCheckable ) &&
           legendNode->data( Qt::CheckStateRole ).toInt() != shouldHaveState )
        legendNode->setData( shouldHaveState, Qt::CheckStateRole );
    }
  }
  else
  {
    // all nodes should be checked
    Q_FOREACH ( QgsLayerTreeModelLegendNode *legendNode, model->layerLegendNodes( nodeLayer, true ) )
    {
      if ( ( legendNode->flags() & Qt::ItemIsUserCheckable ) &&
           legendNode->data( Qt::CheckStateRole ).toInt() != Qt::Checked )
        legendNode->setData( Qt::Checked, Qt::CheckStateRole );
    }
  }
#endif
}


void LayerTreeMapCanvasBridge::applyThemeToGroup( QgsLayerTreeGroup *parent, const QgsMapThemeCollection::MapThemeRecord &rec )
{
  Q_FOREACH ( QgsLayerTreeNode *node, parent->children() )
  {
    if ( QgsLayerTree::isGroup( node ) )
      applyThemeToGroup( QgsLayerTree::toGroup( node ), rec );
    else if ( QgsLayerTree::isLayer( node ) )
      applyThemeToLayer( QgsLayerTree::toLayer( node ), rec );
  }
}
