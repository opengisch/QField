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

FlatLayerTreeModel::FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new FlatLayerTreeModelBase( layerTree, project, parent ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &FlatLayerTreeModelBase::mapThemeChanged, this, &FlatLayerTreeModel::mapThemeChanged );
  connect( mSourceModel, &FlatLayerTreeModelBase::isTemporalChanged, this, &FlatLayerTreeModel::isTemporalChanged );
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

FlatLayerTreeModelBase::FlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QAbstractProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  mLayerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree, true );
  QAbstractProxyModel::setSourceModel( mLayerTreeModel );
  connect( mProject, &QgsProject::cleared, this, [=] { updateTemporalState(); buildMap( nullptr ); } );
  connect( mProject, &QgsProject::readProject, this, [=] { buildMap( mLayerTreeModel ); } );
  connect( mProject, &QgsProject::layersAdded, this, &FlatLayerTreeModelBase::adjustTemporalStateFromAddedLayers );
  connect( mLayerTreeModel, &QAbstractItemModel::dataChanged, this, &FlatLayerTreeModelBase::updateMap );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsRemoved, this, &FlatLayerTreeModelBase::removeFromMap );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsInserted, this, &FlatLayerTreeModelBase::insertInMap );
}

void FlatLayerTreeModelBase::freeze()
{
  mFrozen = true;
}

void FlatLayerTreeModelBase::unfreeze( bool resetModel )
{
  mFrozen = false;
  if ( resetModel )
    buildMap( mLayerTreeModel );
}

void FlatLayerTreeModelBase::updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( mFrozen )
    return;

  Q_UNUSED( bottomRight )
  QModelIndex modifiedIndex = mapFromSource( topLeft );
  if ( modifiedIndex.isValid() )
  {
    emit dataChanged( modifiedIndex, modifiedIndex, roles );
  }
}

void FlatLayerTreeModelBase::insertInMap( const QModelIndex &parent, int first, int last )
{
  if ( mFrozen )
    return;

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

int FlatLayerTreeModelBase::buildMap( QgsLayerTreeModel *model, const QModelIndex &parent, int row, int treeLevel )
{
  if ( mFrozen )
    return 0;

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
            // WMS layers have no legend items, skip those.
            continue;
          }
        }
        row = buildMap( model, index, row, treeLevel + 1 );
      }
    }
  }

  if ( reset )
    endResetModel();
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
  return !parent.isValid() ? mRowMap.size() : 0;
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
  if ( !sourceIndex.isValid() )
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
      if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        id += QStringLiteral( "legend" );
        id += '/' + sym->layerNode()->layerId();
        QStringList legendParts;
        QModelIndex currentIndex = sourceIndex;
        while ( sym )
        {
          legendParts << QString::number( currentIndex.internalId() );
          currentIndex = currentIndex.parent();
          sym = mLayerTreeModel->index2legendNode( currentIndex );
        }
        std::reverse( legendParts.begin(), legendParts.end() );
        id += '/' + legendParts.join( QStringLiteral( "~__~" ) );
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
            if ( vectorLayer && vectorLayer->geometryType() != QgsWkbTypes::NullGeometry )
            {
              id += QStringLiteral( "layer" );
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
        return QStringLiteral( "layer" );
      else if ( QgsLayerTree::isGroup( node ) )
        return QStringLiteral( "group" );
      else
        return QStringLiteral( "legend" );
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
          case QgsMapLayerType::VectorLayer:
            layerType = QStringLiteral( "vectorlayer" );
            break;
          case QgsMapLayerType::RasterLayer:
            layerType = QStringLiteral( "rasterlayer" );
            break;
          case QgsMapLayerType::PluginLayer:
            layerType = QStringLiteral( "pluginlayer" );
            break;
          case QgsMapLayerType::MeshLayer:
            layerType = QStringLiteral( "meshlayer" );
            break;
          case QgsMapLayerType::VectorTileLayer:
            layerType = QStringLiteral( "vectortilelayer" );
            break;
          case QgsMapLayerType::AnnotationLayer:
            layerType = QStringLiteral( "annotationlayer" );
            break;
          case QgsMapLayerType::PointCloudLayer:
            layerType = QStringLiteral( "pointcloudlayer" );
            break;
          case QgsMapLayerType::GroupLayer:
            layerType = QStringLiteral( "grouplayer" );
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
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        name = sym->data( Qt::DisplayRole ).toString();
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
          return ( layer->geometryType() == QgsWkbTypes::PointGeometry || layer->geometryType() == QgsWkbTypes::LineGeometry || layer->geometryType() == QgsWkbTypes::PolygonGeometry );
        }
      }
      return false;
    }

    case FlatLayerTreeModel::InTracking:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );

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

    case FlatLayerTreeModel::GeometryLocked:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );

      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );

        if ( layer )
          return layer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();
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
          return static_cast<bool>( layer->dataProvider()->capabilities() & QgsVectorDataProvider::Capability::ReloadData );
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
      QgsVectorLayer *layer = nullptr;
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      }

      return layer && layer->isSpatial() && layer->labeling();
    }

    case FlatLayerTreeModel::LabelsVisible:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      QgsVectorLayer *layer = nullptr;
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      }

      return layer && layer->isSpatial() && layer->labeling() && layer->labelsEnabled();
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
          return layer->renderer()->legendKeyToExpression( symbolNode->data( QgsLayerTreeModelLegendNode::RuleKeyRole ).toString(),
                                                           layer, ok );
        }
      }
      return QString();
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
      QgsVectorLayer *layer = nullptr;
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      }

      if ( !layer || !layer->isSpatial() || !layer->labeling() )
        return false;

      layer->setLabelsEnabled( !layer->labelsEnabled() );
      layer->emitStyleChanged();
      layer->triggerRepaint();

      emit dataChanged( index, index, QVector<int>() << FlatLayerTreeModel::LabelsVisible );
      return true;
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
        endRow++;

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << FlatLayerTreeModel::IsCollapsed << FlatLayerTreeModel::IsParentCollapsed );
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

    default:
      return false;
  }
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
  roleNames[FlatLayerTreeModel::GeometryLocked] = "GeometryLocked";
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

  buildMap( mLayerTreeModel );
}

void FlatLayerTreeModelBase::updateCurrentMapTheme()
{
  mMapTheme.clear();

  const QgsMapThemeCollection::MapThemeRecord rec = QgsMapThemeCollection::createThemeFromCurrentState( mLayerTreeModel->rootGroup(), mLayerTreeModel );
  const QStringList mapThemes = QgsProject::instance()->mapThemeCollection()->mapThemes();
  for ( const QString &grpName : mapThemes )
  {
    // only compare layer records as the legend does not offer collapse info for now
    // TODO check the whole rec equality whenever the layer tree is a tree and not a list anymore
    if ( rec.validLayerRecords() == QgsProject::instance()->mapThemeCollection()->mapThemeState( grpName ).validLayerRecords() )
    {
      mMapTheme = grpName;
      return;
    }
  }
}

void FlatLayerTreeModelBase::updateTemporalState()
{
  mIsTemporal = false;
  const QVector<QgsMapLayer *> mapLayers = mProject->layers<QgsMapLayer *>();
  for ( QgsMapLayer *mapLayer : mapLayers )
  {
    if ( mapLayer->temporalProperties() && mapLayer->temporalProperties()->isActive() )
    {
      mIsTemporal = true;
      break;
    }
  }
  emit isTemporalChanged();
}

void FlatLayerTreeModelBase::adjustTemporalStateFromAddedLayers( const QList<QgsMapLayer *> &layers )
{
  if ( !mIsTemporal )
  {
    for ( QgsMapLayer *layer : layers )
    {
      if ( layer->temporalProperties() && layer->temporalProperties()->isActive() )
      {
        mIsTemporal = true;
        emit isTemporalChanged();
        break;
      }
    }
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
  extent.setMinimal();

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
        if ( vLayer->geometryType() == QgsWkbTypes::NullGeometry )
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

  if ( buffer )
  {
    extent = extent.buffered( extent.width() * buffer );
  }

  return extent;
}
