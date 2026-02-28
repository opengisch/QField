

# File layertreemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**layertreemodel.cpp**](layertreemodel_8cpp.md)

[Go to the documentation of this file](layertreemodel_8cpp.md)


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

#include "layertreemodel.h"

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

FlatLayerTreeModel::FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new FlatLayerTreeModelBase( layerTree, project, parent ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &FlatLayerTreeModelBase::layersAdded, this, &FlatLayerTreeModel::layersAdded );
  connect( mSourceModel, &FlatLayerTreeModelBase::layersRemoved, this, &FlatLayerTreeModel::layersRemoved );
  connect( mSourceModel, &FlatLayerTreeModelBase::mapThemeChanged, this, &FlatLayerTreeModel::mapThemeChanged );
  connect( mSourceModel, &FlatLayerTreeModelBase::hasCollapsibleItemsChanged, this, &FlatLayerTreeModel::hasCollapsibleItemsChanged );
  connect( mSourceModel, &FlatLayerTreeModelBase::isCollapsedChanged, this, &FlatLayerTreeModel::isCollapsedChanged );
  connect( mSourceModel, &FlatLayerTreeModelBase::isTemporalChanged, this, &FlatLayerTreeModel::isTemporalChanged );
  connect( mSourceModel, &FlatLayerTreeModelBase::isFrozenChanged, this, &FlatLayerTreeModel::isFrozenChanged );
}

QVariant FlatLayerTreeModel::data( const QModelIndex &index, int role ) const
{
  return QSortFilterProxyModel::data( index, role );
}

bool FlatLayerTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  return QSortFilterProxyModel::setData( index, value, role );
}

QString FlatLayerTreeModel::mapTheme() const
{
  return mSourceModel->mapTheme();
}

void FlatLayerTreeModel::setMapTheme( const QString &mapTheme )
{
  mSourceModel->setMapTheme( mapTheme );
}

bool FlatLayerTreeModel::isTemporal() const
{
  return mSourceModel->isTemporal();
}

void FlatLayerTreeModel::updateCurrentMapTheme()
{
  mSourceModel->updateCurrentMapTheme();
}

bool FlatLayerTreeModel::isFrozen() const
{
  return mSourceModel->isFrozen();
}

void FlatLayerTreeModel::freeze()
{
  mSourceModel->freeze();
}

void FlatLayerTreeModel::unfreeze( bool resetModel )
{
  mSourceModel->unfreeze( resetModel );
}

void FlatLayerTreeModel::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
{
  mSourceModel->setLayerInTracking( nodeLayer, tracking );
}

QgsProject *FlatLayerTreeModel::project() const
{
  return mSourceModel->project();
}

QgsLayerTreeModel *FlatLayerTreeModel::layerTreeModel() const
{
  return mSourceModel->layerTreeModel();
}

QgsLayerTree *FlatLayerTreeModel::layerTree() const
{
  return mSourceModel->layerTree();
}

bool FlatLayerTreeModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  return !mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), FlatLayerTreeModel::IsParentCollapsed ).toBool();
}

QgsRectangle FlatLayerTreeModel::nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer )
{
  return mSourceModel->nodeExtent( mapToSource( index ), mapSettings, buffer );
}

void FlatLayerTreeModel::setAllCollapsed( bool collapsed )
{
  mSourceModel->setAllCollapsed( collapsed );
}

bool FlatLayerTreeModel::hasCollapsibleItems() const
{
  return mSourceModel->hasCollapsibleItems();
}
bool FlatLayerTreeModel::isCollapsed() const
{
  return mSourceModel->isCollapsed();
}

FlatLayerTreeModelBase::FlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
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
  connect( mLayerTreeModel, &QAbstractItemModel::rowsRemoved, this, &FlatLayerTreeModelBase::removeFromMap );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsInserted, this, &FlatLayerTreeModelBase::insertInMap );
}

bool FlatLayerTreeModelBase::isFrozen() const
{
  return mFrozen > 0;
}

void FlatLayerTreeModelBase::freeze()
{
  mFrozen++;
  emit isFrozenChanged();
}

void FlatLayerTreeModelBase::unfreeze( bool resetModel )
{
  mFrozen = 0;
  emit isFrozenChanged();

  if ( resetModel )
  {
    buildMap( mLayerTreeModel );
  }
}

void FlatLayerTreeModelBase::updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( mFrozen )
    return;

  Q_UNUSED( bottomRight )
  QModelIndex modifiedIndex = mapFromSource( topLeft );
  if ( modifiedIndex.isValid() )
  {
    emit dataChanged( modifiedIndex, modifiedIndex, QVector<int>() << Qt::DisplayRole << FlatLayerTreeModel::Name << FlatLayerTreeModel::FeatureCount << FlatLayerTreeModel::Visible );
  }
}

void FlatLayerTreeModelBase::insertInMap( const QModelIndex &parent, int first, int last )
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

void FlatLayerTreeModelBase::removeFromMap( const QModelIndex &parent, int first, int last )
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

void FlatLayerTreeModelBase::clearMap()
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

int FlatLayerTreeModelBase::buildMap( QgsLayerTreeModel *model, const QModelIndex &parent, int row, int treeLevel )
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
            if ( rasterLayer->source().contains( QStringLiteral( "type=xyz" ), Qt::CaseInsensitive ) )
            {
              // XYZ layers have no legend items, skip those.
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

void FlatLayerTreeModelBase::setSourceModel( QAbstractItemModel *sourceModel )
{
  QAbstractProxyModel::setSourceModel( sourceModel );
  buildMap( mLayerTreeModel );
}

QModelIndex FlatLayerTreeModelBase::mapToSource( const QModelIndex &proxyIndex ) const
{
  if ( !proxyIndex.isValid() || !mIndexMap.contains( proxyIndex.row() ) )
    return QModelIndex();
  return mIndexMap[proxyIndex.row()];
}

QModelIndex FlatLayerTreeModelBase::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mRowMap.contains( sourceIndex ) )
    return QModelIndex();
  return createIndex( mRowMap[sourceIndex], sourceIndex.column() );
}

QModelIndex FlatLayerTreeModelBase::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )
  return QModelIndex();
}
int FlatLayerTreeModelBase::columnCount( const QModelIndex &parent ) const
{
  return sourceModel()->columnCount( mapToSource( parent ) );
}
int FlatLayerTreeModelBase::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mRowMap.size() ) : 0;
}

QModelIndex FlatLayerTreeModelBase::index( int row, int column, const QModelIndex &parent ) const
{
  return !parent.isValid() ? createIndex( row, column ) : QModelIndex();
}

QVariant FlatLayerTreeModelBase::data( const QModelIndex &index, int role ) const
{
  if ( mFrozen )
    return QVariant();

  const QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() || !sourceIndex.internalPointer() )
    return QVariant();

  switch ( role )
  {
    case FlatLayerTreeModel::VectorLayerPointer:
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

    case FlatLayerTreeModel::MapLayerPointer:
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

    case FlatLayerTreeModel::HasSpatialExtent:
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

    case FlatLayerTreeModel::LegendImage:
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

    case FlatLayerTreeModel::Type:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        return FlatLayerTreeModel::Layer;
      }
      else if ( QgsLayerTree::isGroup( node ) )
      {
        return FlatLayerTreeModel::Group;
      }
      else if ( QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        if ( qobject_cast<QgsWmsLegendNode *>( legendNode ) || qobject_cast<QgsImageLegendNode *>( legendNode ) )
        {
          return FlatLayerTreeModel::Image;
        }
      }

      return FlatLayerTreeModel::Legend;
    }

    case FlatLayerTreeModel::LayerType:
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

    case FlatLayerTreeModel::Name:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QString name;
      if ( QgsLayerTree::isLayer( node ) || QgsLayerTree::isGroup( node ) )
      {
        name = node->name();
        if ( node->customProperty( QStringLiteral( "showFeatureCount" ), 0 ).toInt() )
        {
          int count = data( index, FlatLayerTreeModel::FeatureCount ).toInt();
          name += QStringLiteral( " [%1]" ).arg( count );
        }
      }
      else if ( QgsLayerTreeModelLegendNode *symbol = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        name = symbol->data( Qt::DisplayRole ).toString();
      }

      return name;
    }

    case FlatLayerTreeModel::Visible:
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

    case FlatLayerTreeModel::Trackable:
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

    case FlatLayerTreeModel::InTracking:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        const QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return ( mLayersInTracking.contains( nodeLayer ) );
      }
      return false;
    }

    case FlatLayerTreeModel::ReadOnly:
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

    case FlatLayerTreeModel::FeatureAdditionLocked:
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

    case FlatLayerTreeModel::AttributeEditingLocked:
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

    case FlatLayerTreeModel::GeometryEditingLocked:
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

    case FlatLayerTreeModel::FeatureDeletionLocked:
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

    case FlatLayerTreeModel::CanReloadData:
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


    case FlatLayerTreeModel::TreeLevel:
    {
      return mTreeLevelMap.contains( index.row() ) ? mTreeLevelMap[index.row()] : 0;
    }

    case FlatLayerTreeModel::IsValid:
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

    case FlatLayerTreeModel::FeatureCount:
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
          connect( layer, &QgsVectorLayer::symbolFeatureCountMapChanged, this, &FlatLayerTreeModelBase::featureCountChanged, Qt::UniqueConnection );
          layer->countSymbolFeatures();
        }
        return QVariant::fromValue<long>( count );
      }
      return QVariant::fromValue<long>( layer->featureCount() );
    }

    case FlatLayerTreeModel::IsCollapsed:
    {
      return mCollapsedItems.contains( sourceIndex );
    }

    case FlatLayerTreeModel::IsParentCollapsed:
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

    case FlatLayerTreeModel::HasChildren:
    {
      return mTreeLevelMap.contains( index.row() + 1 ) && mTreeLevelMap[index.row() + 1] > mTreeLevelMap[index.row()];
    }

    case FlatLayerTreeModel::HasLabels:
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

    case FlatLayerTreeModel::LabelsVisible:
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

    case FlatLayerTreeModel::Opacity:
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

    case FlatLayerTreeModel::FilterExpression:
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

    case FlatLayerTreeModel::Credits:
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

    case FlatLayerTreeModel::SnappingEnabled:
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

    case FlatLayerTreeModel::HasNotes:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return QgsLayerNotesUtils::layerHasNotes( nodeLayer->layer() );
      }

      return false;
    }

    case FlatLayerTreeModel::Notes:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        return QgsLayerNotesUtils::layerNotes( nodeLayer->layer() );
      }

      return QString();
    }

    case FlatLayerTreeModel::Checkable:
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

bool FlatLayerTreeModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  const QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() )
    return false;

  switch ( role )
  {
    case FlatLayerTreeModel::Visible:
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

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << FlatLayerTreeModel::Visible );
      return true;
    }

    case FlatLayerTreeModel::LabelsVisible:
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

          emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::LabelsVisible );
          return true;
        }
        else if ( QgsVectorTileLayer *vectorTileLayer = qobject_cast<QgsVectorTileLayer *>( nodeLayer->layer() ) )
        {
          if ( !vectorTileLayer->labeling() )
            return false;

          vectorTileLayer->setLabelsEnabled( !vectorTileLayer->labelsEnabled() );
          vectorTileLayer->emitStyleChanged();
          vectorTileLayer->triggerRepaint();

          emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::LabelsVisible );
          return true;
        }
      }

      return false;
    }

    case FlatLayerTreeModel::IsCollapsed:
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

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << FlatLayerTreeModel::IsCollapsed << FlatLayerTreeModel::IsParentCollapsed );
      checkIsCollapsed();
      return true;
    }

    case FlatLayerTreeModel::Opacity:
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

      emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::Opacity );
      return true;
    }

    case FlatLayerTreeModel::SnappingEnabled:
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
          emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::SnappingEnabled );
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

void FlatLayerTreeModelBase::featureCountChanged()
{
  emit dataChanged( createIndex( 0, 0 ), createIndex( rowCount() - 1, 0 ), QVector<int>() << FlatLayerTreeModel::Name << FlatLayerTreeModel::FeatureCount );
}

QHash<int, QByteArray> FlatLayerTreeModelBase::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[FlatLayerTreeModel::VectorLayerPointer] = "VectorLayerPointer";
  roleNames[FlatLayerTreeModel::MapLayerPointer] = "MapLayerPointer";
  roleNames[FlatLayerTreeModel::HasSpatialExtent] = "HasSpatialExtent";
  roleNames[FlatLayerTreeModel::LegendImage] = "LegendImage";
  roleNames[FlatLayerTreeModel::Visible] = "Visible";
  roleNames[FlatLayerTreeModel::Type] = "Type";
  roleNames[FlatLayerTreeModel::Name] = "Name";
  roleNames[FlatLayerTreeModel::InTracking] = "InTracking";
  roleNames[FlatLayerTreeModel::ReadOnly] = "ReadOnly";
  roleNames[FlatLayerTreeModel::FeatureAdditionLocked] = "FeatureAdditionLocked";
  roleNames[FlatLayerTreeModel::AttributeEditingLocked] = "AttributeEditingLocked";
  roleNames[FlatLayerTreeModel::GeometryEditingLocked] = "GeometryEditingLocked";
  roleNames[FlatLayerTreeModel::FeatureDeletionLocked] = "FeatureDeletionLocked";
  roleNames[FlatLayerTreeModel::TreeLevel] = "TreeLevel";
  roleNames[FlatLayerTreeModel::LayerType] = "LayerType";
  roleNames[FlatLayerTreeModel::IsValid] = "IsValid";
  roleNames[FlatLayerTreeModel::FeatureCount] = "FeatureCount";
  roleNames[FlatLayerTreeModel::IsCollapsed] = "IsCollapsed";
  roleNames[FlatLayerTreeModel::IsParentCollapsed] = "IsParentCollapsed";
  roleNames[FlatLayerTreeModel::HasChildren] = "HasChildren";
  roleNames[FlatLayerTreeModel::CanReloadData] = "CanReloadData";
  roleNames[FlatLayerTreeModel::HasLabels] = "HasLabels";
  roleNames[FlatLayerTreeModel::LabelsVisible] = "LabelsVisible";
  roleNames[FlatLayerTreeModel::Opacity] = "Opacity";
  roleNames[FlatLayerTreeModel::FilterExpression] = "FilterExpression";
  roleNames[FlatLayerTreeModel::Credits] = "Credits";
  roleNames[FlatLayerTreeModel::SnappingEnabled] = "SnappingEnabled";
  roleNames[FlatLayerTreeModel::HasNotes] = "HasNotes";
  roleNames[FlatLayerTreeModel::Notes] = "Notes";
  roleNames[FlatLayerTreeModel::Checkable] = "Checkable";
  return roleNames;
}

QgsProject *FlatLayerTreeModelBase::project() const
{
  return mProject;
}

QgsLayerTreeModel *FlatLayerTreeModelBase::layerTreeModel() const
{
  return mLayerTreeModel;
}

QgsLayerTree *FlatLayerTreeModelBase::layerTree() const
{
  return mLayerTreeModel->rootGroup();
}

QString FlatLayerTreeModelBase::mapTheme() const
{
  return mMapTheme;
}

void FlatLayerTreeModelBase::setMapTheme( const QString &mapTheme )
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

void FlatLayerTreeModelBase::updateCurrentMapTheme()
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

void FlatLayerTreeModelBase::updateTemporalState()
{
  mIsTemporal = false;
  const QVector<QgsMapLayer *> mapLayers = mProject->layers<QgsMapLayer *>();
  if ( std::any_of( mapLayers.begin(), mapLayers.end(), []( QgsMapLayer *mapLayer ) { return mapLayer->temporalProperties() && mapLayer->temporalProperties()->isActive(); } ) )
  {
    mIsTemporal = true;
  }
  emit isTemporalChanged();
}

void FlatLayerTreeModelBase::adjustTemporalStateFromAddedLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !mIsTemporal && std::any_of( layers.begin(), layers.end(), []( QgsMapLayer *layer ) { return layer->temporalProperties() && layer->temporalProperties()->isActive(); } ) )
  {
    mIsTemporal = true;
    emit isTemporalChanged();
  }
}

void FlatLayerTreeModelBase::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
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

  emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::InTracking );
}

QgsRectangle FlatLayerTreeModelBase::nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer )
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

void FlatLayerTreeModelBase::setAllCollapsed( bool collapsed )
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
      if ( data( idx, FlatLayerTreeModel::HasChildren ).toBool() && data( idx, FlatLayerTreeModel::IsCollapsed ).toBool() != collapsed )
      {
        setData( idx, collapsed, FlatLayerTreeModel::IsCollapsed );
        anyChanged = true;
      }
    }
  }
}

bool FlatLayerTreeModelBase::hasCollapsibleItems() const
{
  return mHasCollapsibleItems;
}

void FlatLayerTreeModelBase::checkHasCollapsibleItems()
{
  bool hasCollpasibleItems = false;
  const int count = rowCount();
  for ( int i = 0; i < count; i++ )
  {
    const QModelIndex idx = index( i, 0 );
    if ( data( idx, FlatLayerTreeModel::HasChildren ).toBool() )
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

bool FlatLayerTreeModelBase::isCollapsed() const
{
  return mIsCollapsed;
}

void FlatLayerTreeModelBase::checkIsCollapsed()
{
  bool isCollapsed = false;
  if ( mHasCollapsibleItems )
  {
    isCollapsed = true;
    const int count = rowCount();
    for ( int i = 0; i < count; i++ )
    {
      const QModelIndex idx = index( i, 0 );
      if ( data( idx, FlatLayerTreeModel::HasChildren ).toBool() && data( idx, FlatLayerTreeModel::TreeLevel ).toInt() == 0 )
      {
        if ( !data( idx, FlatLayerTreeModel::IsCollapsed ).toBool() )
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


