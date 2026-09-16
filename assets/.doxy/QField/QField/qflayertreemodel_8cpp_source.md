

# File qflayertreemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qflayertreemodel.cpp**](qflayertreemodel_8cpp.md)

[Go to the documentation of this file](qflayertreemodel_8cpp.md)


```C++
/***************************************************************************
  layertree.cpp - LayerTree

 ---------------------
 begin                : 6.12.2016
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

#include "qflayertreemodel.h"

#include <qgscolorramplegendnode.h>
#include <qgslayernotesutils.h>
#include <qgslayertree.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemodellegendnode.h>
#include <qgslayertreenode.h>
#include <qgsmaplayertemporalproperties.h>
#include <qgsmapthemecollection.h>
#include <qgsquickmapsettings.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerfeaturecounter.h>
#include <qgsvectortilelayer.h>

QfFlatLayerTreeModel::QfFlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfFlatLayerTreeModelBase( layerTree, project, parent ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::layersAdded, this, &QfFlatLayerTreeModel::layersAdded );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::layersRemoved, this, &QfFlatLayerTreeModel::layersRemoved );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::mapThemeChanged, this, &QfFlatLayerTreeModel::mapThemeChanged );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::hasCollapsibleItemsChanged, this, &QfFlatLayerTreeModel::hasCollapsibleItemsChanged );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::isCollapsedChanged, this, &QfFlatLayerTreeModel::isCollapsedChanged );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::isTemporalChanged, this, &QfFlatLayerTreeModel::isTemporalChanged );
  connect( mSourceModel, &QfFlatLayerTreeModelBase::isFrozenChanged, this, &QfFlatLayerTreeModel::isFrozenChanged );
}

QVariant QfFlatLayerTreeModel::data( const QModelIndex &index, int role ) const
{
  return QSortFilterProxyModel::data( index, role );
}

bool QfFlatLayerTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  return QSortFilterProxyModel::setData( index, value, role );
}

QString QfFlatLayerTreeModel::mapTheme() const
{
  return mSourceModel->mapTheme();
}

void QfFlatLayerTreeModel::setMapTheme( const QString &mapTheme )
{
  mSourceModel->setMapTheme( mapTheme );
}

bool QfFlatLayerTreeModel::isTemporal() const
{
  return mSourceModel->isTemporal();
}

void QfFlatLayerTreeModel::updateCurrentMapTheme()
{
  mSourceModel->updateCurrentMapTheme();
}

bool QfFlatLayerTreeModel::isFrozen() const
{
  return mSourceModel->isFrozen();
}

void QfFlatLayerTreeModel::freeze()
{
  mSourceModel->freeze();
}

void QfFlatLayerTreeModel::unfreeze( bool resetModel )
{
  mSourceModel->unfreeze( resetModel );
}

void QfFlatLayerTreeModel::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
{
  mSourceModel->setLayerInTracking( nodeLayer, tracking );
}

QgsProject *QfFlatLayerTreeModel::project() const
{
  return mSourceModel->project();
}

QgsLayerTreeModel *QfFlatLayerTreeModel::layerTreeModel() const
{
  return mSourceModel->layerTreeModel();
}

QgsLayerTree *QfFlatLayerTreeModel::layerTree() const
{
  return mSourceModel->layerTree();
}

bool QfFlatLayerTreeModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  return !mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QfFlatLayerTreeModel::IsParentCollapsed ).toBool();
}

QgsRectangle QfFlatLayerTreeModel::nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer )
{
  return mSourceModel->nodeExtent( mapToSource( index ), mapSettings, buffer );
}

void QfFlatLayerTreeModel::setAllCollapsed( bool collapsed )
{
  mSourceModel->setAllCollapsed( collapsed );
}

bool QfFlatLayerTreeModel::hasCollapsibleItems() const
{
  return mSourceModel->hasCollapsibleItems();
}
bool QfFlatLayerTreeModel::isCollapsed() const
{
  return mSourceModel->isCollapsed();
}

QfFlatLayerTreeModelBase::QfFlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QAbstractProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  mLayerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree, true );
  QAbstractProxyModel::setSourceModel( mLayerTreeModel );
  connect( mProject, &QgsProject::aboutToBeCleared, this, [this] {
    mFrozen++;
    clearMap();
  } );
  connect( mProject, &QgsProject::cleared, this, [this] {
    mFrozen--;
  } );
  connect( mProject, &QgsProject::readProject, this, [this] {
    buildMap( mLayerTreeModel );
  } );
  connect( mProject, &QgsProject::layersAdded, this, [this]( const QList<QgsMapLayer *> &layers ) {
    if ( !mFrozen )
    {
      mProjectLayersChanged = true;
      emit layersAdded();
    }

    adjustTemporalStateFromAddedLayers( layers );
  } );
  connect( mProject, static_cast<void ( QgsProject::* )( const QList<QgsMapLayer *> &layers )>( &QgsProject::layersWillBeRemoved ), this, [this]( const QList<QgsMapLayer *> &layers ) {
    if ( !mFrozen )
    {
      mProjectLayersChanged = true;
      emit layersRemoved();
    }
  } );
  connect( mLayerTreeModel, &QAbstractItemModel::dataChanged, this, [this]( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles ) {
    updateMap( topLeft, bottomRight, roles );
  } );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsRemoved, this, &QfFlatLayerTreeModelBase::removeFromMap );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsInserted, this, &QfFlatLayerTreeModelBase::insertInMap );
}

bool QfFlatLayerTreeModelBase::isFrozen() const
{
  return mFrozen > 0;
}

void QfFlatLayerTreeModelBase::freeze()
{
  mFrozen++;
  emit isFrozenChanged();
}

void QfFlatLayerTreeModelBase::unfreeze( bool resetModel )
{
  mFrozen = 0;
  emit isFrozenChanged();

  if ( resetModel )
  {
    buildMap( mLayerTreeModel );
  }
}

void QfFlatLayerTreeModelBase::updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( mFrozen )
    return;

  Q_UNUSED( bottomRight )
  QModelIndex modifiedIndex = mapFromSource( topLeft );
  if ( modifiedIndex.isValid() )
  {
    emit dataChanged( modifiedIndex, modifiedIndex, QVector<int>() << Qt::DisplayRole << QfFlatLayerTreeModel::Name << QfFlatLayerTreeModel::FeatureCount << QfFlatLayerTreeModel::Visible );
  }
}

void QfFlatLayerTreeModelBase::insertInMap( const QModelIndex &parent, int first, int last )
{
  if ( mFrozen )
    return;

  if ( mProjectLayersChanged )
  {
    mProjectLayersChanged = false;
    buildMap( mLayerTreeModel );
    return;
  }

  bool resetNeeded = false;
  for ( int i = 0; first + i <= last; i++ )
  {
    QModelIndex index = mLayerTreeModel->index( first + i, 0, parent );
    if ( mLayerTreeModel->hasChildren( index ) )
    {
      resetNeeded = true;
      break;
    }
  }

  if ( resetNeeded )
  {
    // Added rows with pre-existing children can't be handled, model reset needed
    buildMap( mLayerTreeModel );
    return;
  }

  int insertedAt = -1;
  if ( first == 0 )
  {
    if ( !parent.isValid() )
    {
      insertedAt = 0;
    }
    else if ( mRowMap.contains( parent ) )
    {
      insertedAt = mRowMap[parent] + 1;
    }
  }
  else
  {
    QModelIndex index = mLayerTreeModel->index( first - 1, 0, parent );
    if ( mRowMap.contains( index ) )
    {
      insertedAt = mRowMap[index] + 1;
    }
  }

  if ( insertedAt > -1 )
  {
    beginInsertRows( QModelIndex(), insertedAt, insertedAt + ( last - first ) );

    QMap<int, int> treeLevelMap;
    mIndexMap.clear();
    const QList<QModelIndex> keys = mRowMap.keys();
    for ( const auto &index : keys )
    {
      int row = mRowMap[index];
      int treeLevel = mTreeLevelMap[row];

      if ( row >= insertedAt )
      {
        row = row + ( last - first ) + 1;
        mRowMap[index] = row;
      }

      mIndexMap.insert( row, index );
      treeLevelMap.insert( row, treeLevel );
    }
    mTreeLevelMap = treeLevelMap;

    int treeLevel = 0;
    QModelIndex checkParent = parent;
    while ( checkParent.isValid() )
    {
      treeLevel++;
      checkParent = checkParent.parent();
    }

    for ( int i = 0; first + i <= last; i++ )
    {
      QModelIndex index = mLayerTreeModel->index( first + i, 0, parent );
      mRowMap[index] = insertedAt + i;
      mIndexMap[insertedAt + i] = index;
      mTreeLevelMap[insertedAt + i] = treeLevel;
    }

    endInsertRows();
  }
}

void QfFlatLayerTreeModelBase::removeFromMap( const QModelIndex &parent, int first, int last )
{
  if ( mFrozen )
    return;

  if ( mProjectLayersChanged )
  {
    mProjectLayersChanged = false;
    buildMap( mLayerTreeModel );
    return;
  }

  int removedAt = -1;
  if ( first == 0 )
  {
    if ( !parent.isValid() )
    {
      removedAt = 0;
    }
    else
    {
      if ( mRowMap.contains( parent ) )
      {
        removedAt = mRowMap[parent] + 1;
      }
    }
  }
  else
  {
    QModelIndex index = mLayerTreeModel->index( first - 1, 0, parent );
    if ( mRowMap.contains( index ) )
    {
      removedAt = mRowMap[index] + 1;
    }
  }

  if ( removedAt > -1 )
  {
    bool resetNeeded = false;
    int modifiedUntil = removedAt;
    const int treeLevelRemovedAt = mTreeLevelMap[removedAt];
    while ( modifiedUntil < mTreeLevelMap.size() && mTreeLevelMap[modifiedUntil] >= treeLevelRemovedAt )
    {
      if ( mTreeLevelMap[modifiedUntil] > treeLevelRemovedAt )
      {
        resetNeeded = true;
        break;
      }
      modifiedUntil++;
    }
    modifiedUntil--;

    if ( resetNeeded )
    {
      // Removed rows can't be handled, model reset needed
      buildMap( mLayerTreeModel );
      return;
    }

    beginRemoveRows( QModelIndex(), removedAt, removedAt + ( last - first ) );

    QMap<int, int> treeLevelMap;
    mIndexMap.clear();
    const QList<QModelIndex> keys = mRowMap.keys();
    for ( const auto &index : keys )
    {
      int row = mRowMap[index];
      int treeLevel = mTreeLevelMap[row];

      if ( row >= removedAt && row <= removedAt + ( last - first ) )
      {
        mRowMap.remove( index );
        continue;
      }
      else if ( row > removedAt + ( last - first ) )
      {
        int oldrow = row;
        row = row - ( last - first + 1 );
        if ( oldrow <= modifiedUntil && treeLevel == treeLevelRemovedAt )
        {
          mRowMap.remove( index );
          QModelIndex movedIndex = mLayerTreeModel->index( index.row() - ( last - first + 1 ), 0, parent );
          mRowMap.insert( movedIndex, row );
          mIndexMap[row] = movedIndex;
        }
        else
        {
          mRowMap[index] = row;
          mIndexMap[row] = index;
        }
        treeLevelMap[row] = treeLevel;
      }
      else
      {
        mIndexMap[row] = index;
        treeLevelMap[row] = treeLevel;
      }
    }
    mTreeLevelMap = treeLevelMap;
    endRemoveRows();
  }
}

void QfFlatLayerTreeModelBase::clearMap()
{
  if ( mIsTemporal )
  {
    mIsTemporal = false;
    emit isTemporalChanged();
  }

  beginResetModel();
  mRowMap.clear();
  mIndexMap.clear();
  mCollapsedItems.clear();
  mTreeLevelMap.clear();
  endResetModel();
}

int QfFlatLayerTreeModelBase::buildMap( QgsLayerTreeModel *model, const QModelIndex &parent, int row, int treeLevel )
{
  if ( mFrozen )
  {
    return 0;
  }

  bool reset = false;
  if ( row == 0 )
  {
    reset = true;
    beginResetModel();
    mRowMap.clear();
    mIndexMap.clear();
    mCollapsedItems.clear();
    mTreeLevelMap.clear();
  }

  if ( model )
  {
    int nbRows = model->rowCount( parent );
    for ( int i = 0; i < nbRows; i++ )
    {
      QModelIndex index = model->index( i, 0, parent );
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( index );
      if ( node && node->customProperty( QStringLiteral( "nodeHidden" ), QStringLiteral( "false" ) ).toString() == QStringLiteral( "true" ) )
        continue;

      const bool isLayer = QgsLayerTree::isLayer( node );
      if ( isLayer )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsMapLayer *layer = nodeLayer->layer();
        if ( layer && layer->flags().testFlag( QgsMapLayer::Private ) )
          continue;
      }

      QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->index2legendNode( index );
      if ( qobject_cast<QgsColorRampLegendNode *>( legendNode ) || qobject_cast<QgsDataDefinedSizeLegendNode *>( legendNode ) )
      {
        // Skip unsupported legend types
        continue;
      }

      if ( node && !node->isExpanded() )
        mCollapsedItems << index;

      mRowMap[index] = row;
      mIndexMap[row] = index;
      mTreeLevelMap[row] = treeLevel;
      row++;
      if ( model->hasChildren( index ) )
      {
        if ( isLayer )
        {
          QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
          QgsRasterLayer *rasterLayer = qobject_cast<QgsRasterLayer *>( nodeLayer->layer() );
          if ( rasterLayer && rasterLayer->dataProvider() && rasterLayer->dataProvider()->name() == QStringLiteral( "wms" ) )
          {
            if ( rasterLayer->source().contains( QStringLiteral( "type=xyz" ), Qt::CaseInsensitive ) || rasterLayer->source().contains( QStringLiteral( "type=mbtiles" ), Qt::CaseInsensitive ) )
            {
              // XYZ and mbtiles layers have no legend items, skip those.
              continue;
            }
          }
        }
        row = buildMap( model, index, row, treeLevel + 1 );
      }
    }
  }

  if ( reset )
  {
    endResetModel();
    checkHasCollapsibleItems();
    checkIsCollapsed();
  }

  return row;
}

void QfFlatLayerTreeModelBase::setSourceModel( QAbstractItemModel *sourceModel )
{
  QAbstractProxyModel::setSourceModel( sourceModel );
  buildMap( mLayerTreeModel );
}

QModelIndex QfFlatLayerTreeModelBase::mapToSource( const QModelIndex &proxyIndex ) const
{
  if ( !proxyIndex.isValid() || !mIndexMap.contains( proxyIndex.row() ) )
    return QModelIndex();
  return mIndexMap[proxyIndex.row()];
}

QModelIndex QfFlatLayerTreeModelBase::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mRowMap.contains( sourceIndex ) )
    return QModelIndex();
  return createIndex( mRowMap[sourceIndex], sourceIndex.column() );
}

QModelIndex QfFlatLayerTreeModelBase::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )
  return QModelIndex();
}
int QfFlatLayerTreeModelBase::columnCount( const QModelIndex &parent ) const
{
  return sourceModel()->columnCount( mapToSource( parent ) );
}
int QfFlatLayerTreeModelBase::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mRowMap.size() ) : 0;
}

QModelIndex QfFlatLayerTreeModelBase::index( int row, int column, const QModelIndex &parent ) const
{
  return !parent.isValid() ? createIndex( row, column ) : QModelIndex();
}

QVariant QfFlatLayerTreeModelBase::data( const QModelIndex &index, int role ) const
{
  if ( mFrozen )
    return QVariant();

  const QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() || !sourceIndex.internalPointer() )
    return QVariant();

  switch ( role )
  {
    case QfFlatLayerTreeModel::VectorLayerPointer:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
        return QVariant::fromValue<QgsVectorLayer *>( layer );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( sym->layerNode()->layer() );
        return QVariant::fromValue<QgsVectorLayer *>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    case QfFlatLayerTreeModel::MapLayerPointer:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );

        return QVariant::fromValue<QgsMapLayer *>( layer );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
        return QVariant::fromValue<QgsMapLayer *>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    case QfFlatLayerTreeModel::HasSpatialExtent:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        const QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        const QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );

        if ( layer )
        {
          return layer->isSpatial();
        }
      }
      else if ( QgsLayerTree::isGroup( node ) )
      {
        const QgsLayerTreeGroup *groupNode = QgsLayerTree::toGroup( node );
        QList<QgsMapLayer *> layers;

        const QStringList findLayerIds = groupNode->findLayerIds();

        if ( findLayerIds.empty() )
        {
          return false;
        }

        for ( const QString &layerId : findLayerIds )
          layers << QgsProject::instance()->mapLayer( layerId );

        for ( int i = 0; i < layers.size(); ++i )
        {
          if ( layers[i] )
          {
            const QgsRectangle extent = layers[i]->extent();
            if ( layers[i]->isValid() && layers[i]->isSpatial() && !extent.isEmpty() && extent.isFinite() )
            {
              return true;
            }
          }
        }
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
        if ( layer )
        {
          return layer->isSpatial();
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::LegendImage:
    {
      QString id;
      if ( QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        if ( qobject_cast<QgsWmsLegendNode *>( legendNode ) )
        {
          QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex.parent() );
          if ( QgsLayerTree::isLayer( node ) )
          {
            QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
            QgsRasterLayer *rasterLayer = qobject_cast<QgsRasterLayer *>( nodeLayer->layer() );
            if ( rasterLayer && rasterLayer->dataProvider() && rasterLayer->dataProvider()->supportsLegendGraphic() )
            {
              id += QStringLiteral( "image://asynclegend/layer" );
              id += '/' + nodeLayer->layerId();
            }
          }
        }
        else if ( qobject_cast<QgsImageLegendNode *>( legendNode ) )
        {
          QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex.parent() );
          if ( QgsLayerTree::isLayer( node ) )
          {
            QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
            QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );
            if ( layer && !layer->legendPlaceholderImage().isEmpty() )
            {
              id += QStringLiteral( "image://legend/image" );
              id += '/' + nodeLayer->layerId();
            }
          }
        }
        else
        {
          id += QStringLiteral( "image://legend/legend" );
          id += '/' + legendNode->layerNode()->layerId();
          QStringList legendParts;
          QModelIndex currentIndex = sourceIndex;
          while ( legendNode )
          {
            legendParts << QString::number( currentIndex.internalId() );
            currentIndex = currentIndex.parent();
            legendNode = qobject_cast<QgsSymbolLegendNode *>( mLayerTreeModel->index2legendNode( currentIndex ) );
          }
          std::reverse( legendParts.begin(), legendParts.end() );
          id += '/' + legendParts.join( QStringLiteral( "~__~" ) );
        }
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
        if ( QgsLayerTree::isLayer( node ) )
        {
          QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
          if ( !mLayerTreeModel->hasChildren( sourceIndex ) )
          {
            QgsVectorLayer *vectorLayer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
            if ( vectorLayer && vectorLayer->geometryType() != Qgis::GeometryType::Null )
            {
              id += QStringLiteral( "image://legend/layer" );
              id += '/' + nodeLayer->layerId();
            }
          }
        }
      }
      return id;
    }

    case QfFlatLayerTreeModel::Type:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        return QfFlatLayerTreeModel::Layer;
      }
      else if ( QgsLayerTree::isGroup( node ) )
      {
        return QfFlatLayerTreeModel::Group;
      }
      else if ( QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        if ( qobject_cast<QgsWmsLegendNode *>( legendNode ) || qobject_cast<QgsImageLegendNode *>( legendNode ) )
        {
          return QfFlatLayerTreeModel::Image;
        }
      }

      return QfFlatLayerTreeModel::Legend;
    }

    case QfFlatLayerTreeModel::LayerType:
    {
      QgsMapLayer *layer = nullptr;
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
      }

      QString layerType;
      if ( layer )
      {
        switch ( layer->type() )
        {
          case Qgis::LayerType::Vector:
            layerType = QStringLiteral( "vectorlayer" );
            break;
          case Qgis::LayerType::Raster:
            layerType = QStringLiteral( "rasterlayer" );
            break;
          case Qgis::LayerType::Plugin:
            layerType = QStringLiteral( "pluginlayer" );
            break;
          case Qgis::LayerType::Mesh:
            layerType = QStringLiteral( "meshlayer" );
            break;
          case Qgis::LayerType::VectorTile:
            layerType = QStringLiteral( "vectortilelayer" );
            break;
          case Qgis::LayerType::Annotation:
            layerType = QStringLiteral( "annotationlayer" );
            break;
          case Qgis::LayerType::PointCloud:
            layerType = QStringLiteral( "pointcloudlayer" );
            break;
          case Qgis::LayerType::Group:
            layerType = QStringLiteral( "grouplayer" );
            break;
          case Qgis::LayerType::TiledScene:
            layerType = QStringLiteral( "tiledscenelayer" );
            break;
        }
      }
      return layerType;
    }

    case QfFlatLayerTreeModel::Name:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QString name;
      if ( QgsLayerTree::isLayer( node ) || QgsLayerTree::isGroup( node ) )
      {
        name = node->name();
        if ( node->customProperty( QStringLiteral( "showFeatureCount" ), 0 ).toInt() )
        {
          int count = data( index, QfFlatLayerTreeModel::FeatureCount ).toInt();
          name += QStringLiteral( " [%1]" ).arg( count );
        }
      }
      else if ( QgsLayerTreeModelLegendNode *symbol = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        name = symbol->data( Qt::DisplayRole ).toString();
      }

      return name;
    }

    case QfFlatLayerTreeModel::Visible:
    {
      QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex );
      if ( sym )
      {
        if ( sym->flags() & Qt::ItemIsUserCheckable )
        {
          return sym->data( Qt::CheckStateRole ).toBool() && sym->layerNode()->isVisible();
        }
        else
        {
          return sym->layerNode()->isVisible();
        }
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
        return node->isVisible();
      }
    }

    case QfFlatLayerTreeModel::Trackable:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
        if ( layer && layer->isValid() )
        {
          return ( layer->geometryType() == Qgis::GeometryType::Point || layer->geometryType() == Qgis::GeometryType::Line || layer->geometryType() == Qgis::GeometryType::Polygon );
        }
      }
      return false;
    }

    case QfFlatLayerTreeModel::InTracking:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        const QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return ( mLayersInTracking.contains( nodeLayer ) );
      }
      return false;
    }

    case QfFlatLayerTreeModel::ReadOnly:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );

        if ( layer )
          return layer->readOnly();
      }

      return false;
    }

    case QfFlatLayerTreeModel::FeatureAdditionLocked:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );

        if ( layer )
        {
          return ( ( layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked_expression_active" ), false ).toBool() ) || ( layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool() ) );
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::AttributeEditingLocked:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          return layer->customProperty( QStringLiteral( "QFieldSync/is_attribute_editing_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_attribute_editing_locked_expression_active" ), false ).toBool();
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::GeometryEditingLocked:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          return ( ( layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked_expression_active" ), false ).toBool() ) || ( layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool() ) );
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::FeatureDeletionLocked:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          return ( ( layer->customProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked_expression_active" ), false ).toBool() ) || ( layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() && !layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool() ) );
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::CanReloadData:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );

        if ( layer && layer->dataProvider() )
          return static_cast<bool>( layer->dataProvider()->capabilities() & Qgis::VectorProviderCapability::ReloadData );
      }

      return false;
    }


    case QfFlatLayerTreeModel::TreeLevel:
    {
      return mTreeLevelMap.contains( index.row() ) ? mTreeLevelMap[index.row()] : 0;
    }

    case QfFlatLayerTreeModel::IsValid:
    {
      QgsMapLayer *layer = nullptr;
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
      }
      else
      {
        // Probably a group or other legend entry
        return true;
      }

      if ( !layer )
      {
        return false;
      }

      return layer->isValid();
    }

    case QfFlatLayerTreeModel::FeatureCount:
    {
      QgsVectorLayer *layer = nullptr;
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        layer = qobject_cast<QgsVectorLayer *>( sym->layerNode()->layer() );
      }

      if ( !layer ) // Group
        return -1;

      // For now, do not count feature on WFS layers, it can lead to long hangs
      if ( layer->dataProvider() && layer->dataProvider()->name() == QStringLiteral( "WFS" ) )
        return QVariant();

      if ( layer->renderer() && layer->renderer()->type() == QStringLiteral( "singleSymbol" ) && layer->renderer()->legendSymbolItems().size() > 0 )
      {
        const long count = layer->featureCount( layer->renderer()->legendSymbolItems().at( 0 ).ruleKey() );
        if ( count == -1 )
        {
          connect( layer, &QgsVectorLayer::symbolFeatureCountMapChanged, this, &QfFlatLayerTreeModelBase::featureCountChanged, Qt::UniqueConnection );
          layer->countSymbolFeatures();
        }
        return QVariant::fromValue<long>( count );
      }
      return QVariant::fromValue<long>( layer->featureCount() );
    }

    case QfFlatLayerTreeModel::IsCollapsed:
    {
      return mCollapsedItems.contains( sourceIndex );
    }

    case QfFlatLayerTreeModel::IsParentCollapsed:
    {
      QModelIndex currentIndex = sourceIndex;
      while ( currentIndex.isValid() )
      {
        currentIndex = currentIndex.parent();
        if ( sourceIndex.isValid() && mCollapsedItems.contains( currentIndex ) )
          return true;
      }
      return false;
    }

    case QfFlatLayerTreeModel::HasChildren:
    {
      return mTreeLevelMap.contains( index.row() + 1 ) && mTreeLevelMap[index.row() + 1] > mTreeLevelMap[index.row()];
    }

    case QfFlatLayerTreeModel::HasLabels:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *vectorLayer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          return vectorLayer->isSpatial() && vectorLayer->labeling();
        }
        else if ( QgsVectorTileLayer *vectorTileLayer = qobject_cast<QgsVectorTileLayer *>( nodeLayer->layer() ) )
        {
          return vectorTileLayer->labeling() ? true : false;
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::LabelsVisible:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *vectorLayer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          return vectorLayer->isSpatial() && vectorLayer->labeling() && vectorLayer->labelsEnabled();
        }
        else if ( QgsVectorTileLayer *vectorTileLayer = qobject_cast<QgsVectorTileLayer *>( nodeLayer->layer() ) )
        {
          return vectorTileLayer->labeling() && vectorTileLayer->labelsEnabled();
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::Opacity:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QgsMapLayer *layer = nullptr;
      if ( !QgsLayerTree::isLayer( node ) )
        return -1.0;

      QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
      layer = nodeLayer->layer();

      if ( !layer )
        return -1.0;

      if ( !layer->isSpatial() )
        return -1.0;

      return layer->opacity();
    }

    case QfFlatLayerTreeModel::FilterExpression:
    {
      QgsLayerTreeModelLegendNode *node = mLayerTreeModel->index2legendNode( sourceIndex );
      if ( QgsSymbolLegendNode *symbolNode = qobject_cast<QgsSymbolLegendNode *>( node ) )
      {
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( QgsLayerTree::toLayer( node->layerNode() )->layer() );
        if ( layer && layer->renderer() )
        {
          bool ok = false;
          const QString ruleKey = symbolNode->data( static_cast<int>( QgsLayerTreeModelLegendNode::CustomRole::RuleKey ) ).toString();
          return layer->renderer()->legendKeyToExpression( ruleKey, layer, ok );
        }
      }
      return QString();
    }

    case QfFlatLayerTreeModel::Credits:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QgsMapLayer *layer = nullptr;
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = nodeLayer->layer();
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        layer = sym->layerNode()->layer();
      }

      if ( layer )
      {
        QStringList credits = layer->metadata().rights();
        std::for_each( credits.begin(), credits.end(), []( QString &credit ) { credit = credit.trimmed(); } );
        credits.removeAll( QStringLiteral( "" ) ); // skip-keyword-check
        return credits.join( QStringLiteral( "; " ) );
      }
      return QVariant();
    }

    case QfFlatLayerTreeModel::SnappingEnabled:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          const QgsSnappingConfig::IndividualLayerSettings settings = mProject->snappingConfig().individualLayerSettings( layer );
          return settings.enabled();
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::HasNotes:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return QgsLayerNotesUtils::layerHasNotes( nodeLayer->layer() );
      }

      return false;
    }

    case QfFlatLayerTreeModel::Notes:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return QgsLayerNotesUtils::layerNotes( nodeLayer->layer() );
      }

      return QString();
    }

    case QfFlatLayerTreeModel::Checkable:
    {
      QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->index2legendNode( sourceIndex );
      if ( legendNode )
      {
        return ( legendNode->flags() & Qt::ItemIsUserCheckable ) ? true : false;
      }

      return true;
    }

    default:
      return QAbstractProxyModel::data( index, role );
  }
}

bool QfFlatLayerTreeModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  const QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() )
    return false;

  switch ( role )
  {
    case QfFlatLayerTreeModel::Visible:
    {
      QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex );
      if ( sym )
      {
        QVariant checked = value.toBool() ? Qt::Checked : Qt::Unchecked;
        sym->setData( checked, Qt::CheckStateRole );
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
        node->setItemVisibilityChecked( value.toBool() );
      }

      //visibility of the node's children are also impacted, use the tree level value to identify those
      int treeLevel = mTreeLevelMap[index.row()];
      int endRow = index.row();
      while ( mTreeLevelMap.contains( endRow + 1 ) && mTreeLevelMap[endRow + 1] > treeLevel )
        endRow++;

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << QfFlatLayerTreeModel::Visible );
      return true;
    }

    case QfFlatLayerTreeModel::LabelsVisible:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *vectorLayer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          if ( !vectorLayer->isSpatial() || !vectorLayer->labeling() )
            return false;

          vectorLayer->setLabelsEnabled( !vectorLayer->labelsEnabled() );
          vectorLayer->emitStyleChanged();
          vectorLayer->triggerRepaint();

          emit dataChanged( index, index, QVector<int>() << QfFlatLayerTreeModel::LabelsVisible );
          return true;
        }
        else if ( QgsVectorTileLayer *vectorTileLayer = qobject_cast<QgsVectorTileLayer *>( nodeLayer->layer() ) )
        {
          if ( !vectorTileLayer->labeling() )
            return false;

          vectorTileLayer->setLabelsEnabled( !vectorTileLayer->labelsEnabled() );
          vectorTileLayer->emitStyleChanged();
          vectorTileLayer->triggerRepaint();

          emit dataChanged( index, index, QVector<int>() << QfFlatLayerTreeModel::LabelsVisible );
          return true;
        }
      }

      return false;
    }

    case QfFlatLayerTreeModel::IsCollapsed:
    {
      const bool collapsed = value.toBool();
      if ( collapsed && !mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems << sourceIndex;
      }
      else if ( !collapsed && mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems.removeAll( sourceIndex );
      }

      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( node )
        node->setExpanded( !collapsed );

      //the node's children are also impacted, use the tree level value to identify those
      int treeLevel = mTreeLevelMap[index.row()];
      int endRow = index.row();
      while ( mTreeLevelMap.contains( endRow + 1 ) && mTreeLevelMap[endRow + 1] > treeLevel )
      {
        endRow++;
      }

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << QfFlatLayerTreeModel::IsCollapsed << QfFlatLayerTreeModel::IsParentCollapsed );
      checkIsCollapsed();
      return true;
    }

    case QfFlatLayerTreeModel::Opacity:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QgsMapLayer *layer = nullptr;
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = nodeLayer->layer();
      }

      if ( !layer || !layer->isSpatial() )
        return false;

      layer->setOpacity( value.toDouble() );
      layer->emitStyleChanged();
      layer->triggerRepaint();

      emit dataChanged( index, index, QVector<int>() << QfFlatLayerTreeModel::Opacity );
      return true;
    }

    case QfFlatLayerTreeModel::SnappingEnabled:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        if ( QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() ) )
        {
          QgsSnappingConfig snappingConfig = mProject->snappingConfig();
          QgsSnappingConfig::IndividualLayerSettings settings = snappingConfig.individualLayerSettings( layer );
          settings.setEnabled( !settings.enabled() );
          snappingConfig.setIndividualLayerSettings( layer, settings );
          mProject->setSnappingConfig( snappingConfig );
          emit dataChanged( index, index, QVector<int>() << QfFlatLayerTreeModel::SnappingEnabled );
          return true;
        }
      }

      return false;
    }

    default:
      return false;
  }
  BUILTIN_UNREACHABLE
}

void QfFlatLayerTreeModelBase::featureCountChanged()
{
  emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount() - 1, 0 ), QVector<int>() << QfFlatLayerTreeModel::Name << QfFlatLayerTreeModel::FeatureCount );
}

QHash<int, QByteArray> QfFlatLayerTreeModelBase::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[QfFlatLayerTreeModel::VectorLayerPointer] = "VectorLayerPointer";
  roleNames[QfFlatLayerTreeModel::MapLayerPointer] = "MapLayerPointer";
  roleNames[QfFlatLayerTreeModel::HasSpatialExtent] = "HasSpatialExtent";
  roleNames[QfFlatLayerTreeModel::LegendImage] = "LegendImage";
  roleNames[QfFlatLayerTreeModel::Visible] = "Visible";
  roleNames[QfFlatLayerTreeModel::Type] = "Type";
  roleNames[QfFlatLayerTreeModel::Name] = "Name";
  roleNames[QfFlatLayerTreeModel::InTracking] = "InTracking";
  roleNames[QfFlatLayerTreeModel::ReadOnly] = "ReadOnly";
  roleNames[QfFlatLayerTreeModel::FeatureAdditionLocked] = "FeatureAdditionLocked";
  roleNames[QfFlatLayerTreeModel::AttributeEditingLocked] = "AttributeEditingLocked";
  roleNames[QfFlatLayerTreeModel::GeometryEditingLocked] = "GeometryEditingLocked";
  roleNames[QfFlatLayerTreeModel::FeatureDeletionLocked] = "FeatureDeletionLocked";
  roleNames[QfFlatLayerTreeModel::TreeLevel] = "TreeLevel";
  roleNames[QfFlatLayerTreeModel::LayerType] = "LayerType";
  roleNames[QfFlatLayerTreeModel::IsValid] = "IsValid";
  roleNames[QfFlatLayerTreeModel::FeatureCount] = "FeatureCount";
  roleNames[QfFlatLayerTreeModel::IsCollapsed] = "IsCollapsed";
  roleNames[QfFlatLayerTreeModel::IsParentCollapsed] = "IsParentCollapsed";
  roleNames[QfFlatLayerTreeModel::HasChildren] = "HasChildren";
  roleNames[QfFlatLayerTreeModel::CanReloadData] = "CanReloadData";
  roleNames[QfFlatLayerTreeModel::HasLabels] = "HasLabels";
  roleNames[QfFlatLayerTreeModel::LabelsVisible] = "LabelsVisible";
  roleNames[QfFlatLayerTreeModel::Opacity] = "Opacity";
  roleNames[QfFlatLayerTreeModel::FilterExpression] = "FilterExpression";
  roleNames[QfFlatLayerTreeModel::Credits] = "Credits";
  roleNames[QfFlatLayerTreeModel::SnappingEnabled] = "SnappingEnabled";
  roleNames[QfFlatLayerTreeModel::HasNotes] = "HasNotes";
  roleNames[QfFlatLayerTreeModel::Notes] = "Notes";
  roleNames[QfFlatLayerTreeModel::Checkable] = "Checkable";
  return roleNames;
}

QgsProject *QfFlatLayerTreeModelBase::project() const
{
  return mProject;
}

QgsLayerTreeModel *QfFlatLayerTreeModelBase::layerTreeModel() const
{
  return mLayerTreeModel;
}

QgsLayerTree *QfFlatLayerTreeModelBase::layerTree() const
{
  return mLayerTreeModel->rootGroup();
}

QString QfFlatLayerTreeModelBase::mapTheme() const
{
  return mMapTheme;
}

void QfFlatLayerTreeModelBase::setMapTheme( const QString &mapTheme )
{
  if ( mMapTheme == mapTheme )
    return;

  mMapTheme = mapTheme;
  emit mapThemeChanged();

  if ( !mapTheme.isEmpty() )
  {
    // Setting a map theme likely changes the layer tree structure, rebuild
    buildMap( mLayerTreeModel );
  }
}

void QfFlatLayerTreeModelBase::updateCurrentMapTheme()
{
  mMapTheme.clear();

  const QgsMapThemeCollection::MapThemeRecord rec = QgsMapThemeCollection::createThemeFromCurrentState( mLayerTreeModel->rootGroup(), mLayerTreeModel );
  const QStringList mapThemes = QgsProject::instance()->mapThemeCollection()->mapThemes();

  // only compare layer records as the legend does not offer collapse info for now
  // TODO check the whole rec equality whenever the layer tree is a tree and not a list anymore
  auto match = std::find_if( mapThemes.begin(), mapThemes.end(), [&rec]( const QString &name ) {
    return rec.validLayerRecords() == QgsProject::instance()->mapThemeCollection()->mapThemeState( name ).validLayerRecords();
  } );
  if ( match != mapThemes.end() )
  {
    mMapTheme = *match;
  }
}

void QfFlatLayerTreeModelBase::updateTemporalState()
{
  mIsTemporal = false;
  const QVector<QgsMapLayer *> mapLayers = mProject->layers<QgsMapLayer *>();
  if ( std::any_of( mapLayers.begin(), mapLayers.end(), []( QgsMapLayer *mapLayer ) { return mapLayer->temporalProperties() && mapLayer->temporalProperties()->isActive(); } ) )
  {
    mIsTemporal = true;
  }
  emit isTemporalChanged();
}

void QfFlatLayerTreeModelBase::adjustTemporalStateFromAddedLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !mIsTemporal && std::any_of( layers.begin(), layers.end(), []( QgsMapLayer *layer ) { return layer->temporalProperties() && layer->temporalProperties()->isActive(); } ) )
  {
    mIsTemporal = true;
    emit isTemporalChanged();
  }
}

void QfFlatLayerTreeModelBase::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
{
  if ( tracking )
  {
    if ( !mLayersInTracking.contains( nodeLayer ) )
      mLayersInTracking.append( nodeLayer );
  }
  else
  {
    if ( mLayersInTracking.contains( nodeLayer ) )
      mLayersInTracking.removeOne( nodeLayer );
  }
  QgsLayerTreeNode *node = static_cast<QgsLayerTreeNode *>( nodeLayer );
  QModelIndex sourceIndex = mLayerTreeModel->node2index( node );
  QModelIndex index = mapFromSource( sourceIndex );

  emit dataChanged( index, index, QVector<int>() << QfFlatLayerTreeModel::InTracking );
}

QgsRectangle QfFlatLayerTreeModelBase::nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer )
{
  QgsRectangle extent;
  extent.setNull();

  const QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() )
    return extent;

  QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
  if ( QgsLayerTree::isGroup( node ) )
  {
    QgsLayerTreeGroup *groupNode = QgsLayerTree::toGroup( node );
    QList<QgsMapLayer *> layers;

    const QStringList findLayerIds = groupNode->findLayerIds();

    if ( findLayerIds.empty() )
      return extent;

    for ( const QString &layerId : findLayerIds )
      layers << QgsProject::instance()->mapLayer( layerId );

    QgsMapSettings ms = mapSettings->mapSettings();
    for ( int i = 0; i < layers.size(); ++i )
    {
      QgsMapLayer *layer = layers.at( i );
      QgsRectangle layerExtent = ms.layerToMapCoordinates( layer, layer->extent() );

      QgsVectorLayer *vLayer = qobject_cast<QgsVectorLayer *>( layer );
      if ( vLayer )
      {
        if ( vLayer->geometryType() == Qgis::GeometryType::Null )
          continue;

        if ( layerExtent.isEmpty() )
        {
          vLayer->updateExtents();
          layerExtent = ms.layerToMapCoordinates( layer, layer->extent() );
        }
      }

      if ( layerExtent.isNull() )
        continue;

      //transform extent
      extent.combineExtentWith( layerExtent );
    }
  }
  else if ( QgsLayerTree::isLayer( node ) )
  {
    QgsMapLayer *layer = QgsLayerTree::toLayer( node )->layer();
    if ( layer )
    {
      extent = mapSettings->mapSettings().layerToMapCoordinates( layer, layer->extent() );
    }
  }
  else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
  {
    QgsMapLayer *layer = sym->layerNode()->layer();
    if ( layer )
    {
      extent = mapSettings->mapSettings().layerToMapCoordinates( layer, layer->extent() );
    }
  }

  if ( extent.width() == 0.0 || extent.height() == 0.0 )
  {
    // If all of the features are at the one point, buffer the
    // rectangle a bit. If they are all at zero, do something a bit
    // more crude.
    if ( extent.xMinimum() == 0.0 && extent.xMaximum() == 0.0 && extent.yMinimum() == 0.0 && extent.yMaximum() == 0.0 )
    {
      extent.set( -1.0, -1.0, 1.0, 1.0 );
    }
    else
    {
      const double padFactor = 1e-8;
      const double widthPad = extent.xMinimum() * padFactor;
      const double heightPad = extent.yMinimum() * padFactor;
      const double xmin = extent.xMinimum() - widthPad;
      const double xmax = extent.xMaximum() + widthPad;
      const double ymin = extent.yMinimum() - heightPad;
      const double ymax = extent.yMaximum() + heightPad;
      extent.set( xmin, ymin, xmax, ymax );
    }
  }

  if ( buffer )
  {
    extent = extent.buffered( extent.width() * buffer );
  }

  return extent;
}

void QfFlatLayerTreeModelBase::setAllCollapsed( bool collapsed )
{
  if ( !mHasCollapsibleItems || mIsCollapsed == collapsed )
  {
    return;
  }

  bool anyChanged = true;
  while ( anyChanged )
  {
    anyChanged = false;
    const int count = rowCount();
    // Iterate backwards when collapsing, forwards when expanding
    const int start = collapsed ? count - 1 : 0;
    const int end = collapsed ? -1 : count;
    const int step = collapsed ? -1 : 1;

    for ( int i = start; i != end; i += step )
    {
      const QModelIndex idx = index( i, 0 );
      if ( data( idx, QfFlatLayerTreeModel::HasChildren ).toBool() && data( idx, QfFlatLayerTreeModel::IsCollapsed ).toBool() != collapsed )
      {
        setData( idx, collapsed, QfFlatLayerTreeModel::IsCollapsed );
        anyChanged = true;
      }
    }
  }
}

bool QfFlatLayerTreeModelBase::hasCollapsibleItems() const
{
  return mHasCollapsibleItems;
}

void QfFlatLayerTreeModelBase::checkHasCollapsibleItems()
{
  bool hasCollpasibleItems = false;
  const int count = rowCount();
  for ( int i = 0; i < count; i++ )
  {
    const QModelIndex idx = index( i, 0 );
    if ( data( idx, QfFlatLayerTreeModel::HasChildren ).toBool() )
    {
      hasCollpasibleItems = true;
      break;
    }
  }

  if ( mHasCollapsibleItems != hasCollpasibleItems )
  {
    mHasCollapsibleItems = hasCollpasibleItems;
    emit hasCollapsibleItemsChanged();
  }
}

bool QfFlatLayerTreeModelBase::isCollapsed() const
{
  return mIsCollapsed;
}

void QfFlatLayerTreeModelBase::checkIsCollapsed()
{
  bool isCollapsed = false;
  if ( mHasCollapsibleItems )
  {
    isCollapsed = true;
    const int count = rowCount();
    for ( int i = 0; i < count; i++ )
    {
      const QModelIndex idx = index( i, 0 );
      if ( data( idx, QfFlatLayerTreeModel::HasChildren ).toBool() && data( idx, QfFlatLayerTreeModel::TreeLevel ).toInt() == 0 )
      {
        if ( !data( idx, QfFlatLayerTreeModel::IsCollapsed ).toBool() )
        {
          isCollapsed = false;
          break;
        }
      }
    }
  }

  if ( mIsCollapsed != isCollapsed )
  {
    mIsCollapsed = isCollapsed;
    emit isCollapsedChanged();
  }
}
```


