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

#include <qgslayertreemodel.h>
#include <qgslayertreenode.h>
#include <qgslayertree.h>
#include <qgslayertreemodellegendnode.h>
#include <qgsmapthemecollection.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

FlatLayerTreeModel::FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new FlatLayerTreeModelBase( layerTree, project, parent ) )
{
  setSourceModel( mSourceModel );
  connect( mSourceModel, &FlatLayerTreeModelBase::mapThemeChanged, this, &FlatLayerTreeModel::mapThemeChanged );
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

FlatLayerTreeModelBase::FlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QAbstractProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  mLayerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree, true );
  setSourceModel( mLayerTreeModel );
  connect( mProject, &QgsProject::cleared, this, [ = ] { buildMap( nullptr ); } );
  connect( mProject, &QgsProject::readProject, this, [ = ] { buildMap( mLayerTreeModel ); } );
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

  if ( insertedAt >  -1 )
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
      if ( mTreeLevelMap[modifiedUntil]  > treeLevelRemovedAt )
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

      if ( row >= removedAt  && row <= removedAt + ( last - first ) )
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

      if ( node && !node->isExpanded() )
        mCollapsedItems << index;

      mRowMap[index] = row;
      mIndexMap[row] = index;
      mTreeLevelMap[row] = treeLevel;
      row++;
      if ( model->hasChildren( index ) )
      {
        if ( QgsLayerTree::isLayer( node ) )
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

void FlatLayerTreeModelBase::setSourceModel( QgsLayerTreeModel *sourceModel )
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
  switch ( role )
  {
    case FlatLayerTreeModel::VectorLayerPointer:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
        return QVariant::fromValue<QgsVectorLayer *>( layer );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
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
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );

        return QVariant::fromValue<QgsMapLayer *>( layer );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        QgsMapLayer *layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
        return QVariant::fromValue<QgsMapLayer *>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    case FlatLayerTreeModel::LegendImage:
    {
      QString id;

      QModelIndex sourceIndex = mapToSource( index );
      if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( sourceIndex ) )
      {
        id += QStringLiteral( "legend" );
        id += '/' + sym->layerNode()->layerId();
        QStringList legendParts;
        while ( sym )
        {
          legendParts << sym->data( Qt::DisplayRole ).toString();
          sourceIndex = sourceIndex.parent();
          sym = mLayerTreeModel->index2legendNode( sourceIndex );
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
              id += '/' +  nodeLayer->layerId();
            }
          }
        }
      }
      return id;
    }

    case FlatLayerTreeModel::Type:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
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
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
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
        }
      }
      return layerType;
    }

    case FlatLayerTreeModel::Name:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      QString name;
      if ( QgsLayerTree::isLayer( node ) || QgsLayerTree::isGroup( node ) )
      {
        name = node->name();
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        name = sym->data( Qt::DisplayRole ).toString();
      }

      return name;
    }

    case FlatLayerTreeModel::Visible:
    {
      QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) );
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
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
        return node->isVisible();
      }
    }

    case FlatLayerTreeModel::Trackable:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
        if ( layer && layer->isValid() )
        {
          return ( layer->geometryType() == QgsWkbTypes::LineGeometry || layer->geometryType() == QgsWkbTypes::PolygonGeometry );
        }
      }
      return false;
    }

    case FlatLayerTreeModel::InTracking:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );

        return ( mLayersInTracking.contains( nodeLayer ) );
      }
      return false;
    }

    case FlatLayerTreeModel::ReadOnly:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );

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
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );

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
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );

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
      QModelIndex sourceIndex = mapToSource( index );
      if ( !sourceIndex.isValid() )
        return QVariant();
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsMapLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        layer = qobject_cast<QgsMapLayer *>( sym->layerNode()->layer() );
      }

      if ( !layer ) // Group
        return true;

      return layer->isValid();
    }

    case FlatLayerTreeModel::FeatureCount:
    {
      QgsVectorLayer *layer = nullptr;
      QModelIndex sourceIndex = mapToSource( index );
      if ( !sourceIndex.isValid() )
        return -1;
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( sourceIndex );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
      }
      else if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        layer = qobject_cast<QgsVectorLayer *>( sym->layerNode()->layer() );
      }

      if ( !layer ) // Group
        return -1;

      return QVariant::fromValue<long>( layer->featureCount() );
    }

    case FlatLayerTreeModel::IsCollapsed:
    {
      QModelIndex sourceIndex = mapToSource( index );
      return mCollapsedItems.contains( sourceIndex );
    }

    case FlatLayerTreeModel::IsParentCollapsed:
    {
      QModelIndex sourceIndex = mapToSource( index );
      while ( sourceIndex.isValid() )
      {
        sourceIndex = sourceIndex.parent();
        if ( sourceIndex.isValid() && mCollapsedItems.contains( sourceIndex ) )
          return true;
      }
      return false;
    }

    case FlatLayerTreeModel::HasChildren:
    {
      return mTreeLevelMap.contains( index.row() + 1 ) && mTreeLevelMap[index.row() + 1] > mTreeLevelMap[index.row()];
    }

    default:
      return QAbstractProxyModel::data( index, role );
  }
}

bool FlatLayerTreeModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  switch ( role )
  {
    case FlatLayerTreeModel::Visible:
    {
      QModelIndex sourceIndex = mapToSource( index );
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

    case FlatLayerTreeModel::IsCollapsed:
    {
      QModelIndex sourceIndex = mapToSource( index );
      const bool collapsed = value.toBool();
      if ( collapsed && !mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems << sourceIndex;
      }
      else if ( !collapsed && mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems.removeAll( sourceIndex );
      }

      //the node's children are also impacted, use the tree level value to identify those
      int treeLevel = mTreeLevelMap[index.row()];
      int endRow = index.row();
      while ( mTreeLevelMap.contains( endRow + 1 ) && mTreeLevelMap[endRow + 1] > treeLevel )
        endRow++;

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << FlatLayerTreeModel::IsCollapsed << FlatLayerTreeModel::IsParentCollapsed );
      return true;
    }

    default:
      return false;
  }
}

QHash<int, QByteArray> FlatLayerTreeModelBase::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[FlatLayerTreeModel::VectorLayerPointer] = "VectorLayerPointer";
  roleNames[FlatLayerTreeModel::MapLayerPointer] = "MapLayerPointer";
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
