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
#include "mapsettings.h"

#include <qgslayertreegroup.h>
#include <qgslayertree.h>
#include <qgsmaplayer.h>
#include <qgslayertreeutils.h>

LayerTreeMapCanvasBridge::LayerTreeMapCanvasBridge( QgsLayerTreeGroup* root, MapSettings* mapSettings, QObject* parent )
  : QObject( parent )
  , mRoot( root )
  , mMapSettings( mapSettings )
  , mPendingCanvasUpdate( false )
  , mHasCustomLayerOrder( false )
  , mAutoSetupOnFirstLayer( true )
  , mAutoEnableCrsTransform( true )
  , mNoLayersLoaded( !root->findLayers().isEmpty() )
{
  connect( root, &QgsLayerTreeGroup::addedChildren, this, &LayerTreeMapCanvasBridge::nodeAddedChildren );
  connect( root, &QgsLayerTreeGroup::customPropertyChanged, this, &LayerTreeMapCanvasBridge::nodeCustomPropertyChanged );
  connect( root, &QgsLayerTreeGroup::removedChildren, this, &LayerTreeMapCanvasBridge::nodeRemovedChildren );
  connect( root, &QgsLayerTreeGroup::visibilityChanged, this, &LayerTreeMapCanvasBridge::nodeVisibilityChanged );

  setCanvasLayers();
}

void LayerTreeMapCanvasBridge::clear()
{
  setHasCustomLayerOrder( false );
  setCustomLayerOrder( defaultLayerOrder() );
}

QStringList LayerTreeMapCanvasBridge::defaultLayerOrder() const
{
  QStringList order;
  defaultLayerOrder( mRoot, order );
  return order;
}

void LayerTreeMapCanvasBridge::defaultLayerOrder( QgsLayerTreeNode* node, QStringList& order ) const
{
  if ( QgsLayerTree::isLayer( node ) )
  {
    QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
    order << nodeLayer->layerId();
  }

  Q_FOREACH ( QgsLayerTreeNode* child, node->children() )
    defaultLayerOrder( child, order );
}


void LayerTreeMapCanvasBridge::setHasCustomLayerOrder( bool state )
{
  if ( mHasCustomLayerOrder == state )
    return;

  mHasCustomLayerOrder = state;
  emit hasCustomLayerOrderChanged( mHasCustomLayerOrder );

  deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::setCustomLayerOrder( const QStringList& order )
{
  if ( mCustomLayerOrder == order )
    return;

  // verify that the new order is correct
  QStringList defOrder( defaultLayerOrder() );
  QStringList newOrder( order );
  QStringList sortedNewOrder( order );
  qSort( defOrder );
  qSort( sortedNewOrder );

  if ( defOrder.size() < sortedNewOrder.size() )
  {
    // might contain bad layers, but also duplicates
    QSet<QString> ids( defOrder.toSet() );

    for ( int i = 0; i < sortedNewOrder.size(); i++ )
    {
      if ( !ids.contains( sortedNewOrder[i] ) )
      {
        newOrder.removeAll( sortedNewOrder[i] );
        sortedNewOrder.removeAt( i-- );
      }
    }
  }

  if ( defOrder != sortedNewOrder )
    return; // must be permutation of the default order

  mCustomLayerOrder = newOrder;
  emit customLayerOrderChanged( mCustomLayerOrder );

  if ( mHasCustomLayerOrder )
    deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::setCanvasLayers()
{
  QList<QgsMapLayer*> layers;

  if ( mHasCustomLayerOrder )
  {
    Q_FOREACH ( const QString& layerId, mCustomLayerOrder )
    {
      QgsLayerTreeLayer* nodeLayer = mRoot->findLayer( layerId );
      if ( nodeLayer )
        layers << nodeLayer->layer();
    }
  }
  else
    setCanvasLayers( mRoot, layers );

  QList<QgsLayerTreeLayer*> layerNodes = mRoot->findLayers();
  bool firstLayers = mAutoSetupOnFirstLayer && mNoLayersLoaded == 0 && !layerNodes.empty();

  if ( firstLayers )
  {
    // also setup destination CRS and map units if the OTF projections are not yet enabled
    if ( !mMapSettings->hasCrsTransformEnabled() )
    {
      Q_FOREACH ( QgsLayerTreeLayer* layerNode, layerNodes )
      {
        if ( layerNode->layer() && layerNode->isVisible() && layerNode->layer()->isSpatial() )
        {
          mMapSettings->setDestinationCrs( layerNode->layer()->crs() );
          mMapSettings->setMapUnits( layerNode->layer()->crs().mapUnits() );
          break;
        }
      }
    }
  }

  mMapSettings->setLayers( layers );

  if ( !mFirstCRS.isValid() )
  {
    // find out what is the first used CRS in case we may need to turn on OTF projections later
    Q_FOREACH ( QgsLayerTreeLayer* layerNode, layerNodes )
    {
      if ( layerNode->layer() && layerNode->layer()->crs().isValid() )
      {
        mFirstCRS = layerNode->layer()->crs();
        break;
      }
    }
  }

  if ( mAutoEnableCrsTransform && mFirstCRS.isValid() && !mMapSettings->hasCrsTransformEnabled() )
  {
    // check whether all layers still have the same CRS
    Q_FOREACH ( QgsLayerTreeLayer* layerNode, layerNodes )
    {
      if ( layerNode->layer() && layerNode->layer()->crs().isValid() && layerNode->layer()->crs() != mFirstCRS )
      {
        mMapSettings->setDestinationCrs( mFirstCRS );
        mMapSettings->setCrsTransformEnabled( true );
        break;
      }
    }
  }

  mNoLayersLoaded = layerNodes.isEmpty();
  if ( mNoLayersLoaded )
    mFirstCRS = QgsCoordinateReferenceSystem();

  mPendingCanvasUpdate = false;
}

void LayerTreeMapCanvasBridge::readProject( const QDomDocument& doc )
{
  mFirstCRS = QgsCoordinateReferenceSystem(); // invalidate on project load

  QDomElement elem = doc.documentElement().firstChildElement( "layer-tree-canvas" );
  if ( elem.isNull() )
  {
    bool oldEnabled;
    QStringList oldOrder;
    if ( QgsLayerTreeUtils::readOldLegendLayerOrder( doc.documentElement().firstChildElement( "legend" ), oldEnabled, oldOrder ) )
    {
      setHasCustomLayerOrder( oldEnabled );
      setCustomLayerOrder( oldOrder );
    }
    return;
  }

  QDomElement customOrderElem = elem.firstChildElement( "custom-order" );
  if ( !customOrderElem.isNull() )
  {
    QStringList order;
    QDomElement itemElem = customOrderElem.firstChildElement( "item" );
    while ( !itemElem.isNull() )
    {
      order.append( itemElem.text() );
      itemElem = itemElem.nextSiblingElement( "item" );
    }

    setHasCustomLayerOrder( customOrderElem.attribute( "enabled", QString() ).toInt() );
    setCustomLayerOrder( order );
  }
}

void LayerTreeMapCanvasBridge::writeProject( QDomDocument& doc )
{
  QDomElement elem = doc.createElement( "layer-tree-canvas" );
  QDomElement customOrderElem = doc.createElement( "custom-order" );
  customOrderElem.setAttribute( "enabled", mHasCustomLayerOrder ? 1 : 0 );

  Q_FOREACH ( const QString& layerId, mCustomLayerOrder )
  {
    QDomElement itemElem = doc.createElement( "item" );
    itemElem.appendChild( doc.createTextNode( layerId ) );
    customOrderElem.appendChild( itemElem );
  }
  elem.appendChild( customOrderElem );

  doc.documentElement().appendChild( elem );
}

void LayerTreeMapCanvasBridge::setCanvasLayers( QgsLayerTreeNode* node, QList<QgsMapLayer*>& layers )
{
  if ( QgsLayerTree::isLayer( node ) )
  {
    QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
    layers << nodeLayer->layer();
  }

  Q_FOREACH ( QgsLayerTreeNode* child, node->children() )
    setCanvasLayers( child, layers );
}

void LayerTreeMapCanvasBridge::deferredSetCanvasLayers()
{
  if ( mPendingCanvasUpdate )
    return;

  mPendingCanvasUpdate = true;
  QMetaObject::invokeMethod( this, "setCanvasLayers", Qt::QueuedConnection );
}

void LayerTreeMapCanvasBridge::nodeAddedChildren( QgsLayerTreeNode* node, int indexFrom, int indexTo )
{
  Q_ASSERT( node );

  // collect layer IDs that have been added in order to put them into custom layer order
  QStringList layerIds;
  QList<QgsLayerTreeNode*> children = node->children();
  for ( int i = indexFrom; i <= indexTo; ++i )
  {
    QgsLayerTreeNode* child = children.at( i );
    if ( QgsLayerTree::isLayer( child ) )
    {
      layerIds << QgsLayerTree::toLayer( child )->layerId();
    }
    else if ( QgsLayerTree::isGroup( child ) )
    {
      Q_FOREACH ( QgsLayerTreeLayer* nodeL, QgsLayerTree::toGroup( child )->findLayers() )
        layerIds << nodeL->layerId();
    }
  }

  Q_FOREACH ( const QString& layerId, layerIds )
  {
    if ( !mCustomLayerOrder.contains( layerId ) )
      mCustomLayerOrder.append( layerId );
  }

  emit customLayerOrderChanged( mCustomLayerOrder );

  deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::nodeRemovedChildren()
{
  // no need to disconnect from removed nodes as they are deleted

  // check whether the layers are still there, if not, remove them from the layer order!
  QList<int> toRemove;
  for ( int i = 0; i < mCustomLayerOrder.count(); ++i )
  {
    QgsLayerTreeLayer* node = mRoot->findLayer( mCustomLayerOrder[i] );
    if ( !node )
      toRemove << i;
  }
  for ( int i = toRemove.count() - 1; i >= 0; --i )
    mCustomLayerOrder.removeAt( toRemove[i] );
  emit customLayerOrderChanged( mCustomLayerOrder );

  deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::nodeVisibilityChanged()
{
  deferredSetCanvasLayers();
}

void LayerTreeMapCanvasBridge::nodeCustomPropertyChanged( QgsLayerTreeNode* node, const QString& key )
{
  Q_UNUSED( node );
  if ( key == "overview" )
    deferredSetCanvasLayers();
}
