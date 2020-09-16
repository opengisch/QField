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
#include <qgsproviderregistry.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

FlatLayerTreeModel::FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QAbstractProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  mLayerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree, true );
  setSourceModel( mLayerTreeModel );
  connect( mProject, &QgsProject::cleared, this, [ = ] { buildMap( nullptr ); } );
  connect( mProject, &QgsProject::readProject, this, [ = ] { buildMap( mLayerTreeModel ); } );
  connect( mLayerTreeModel, &QAbstractItemModel::dataChanged, this, &FlatLayerTreeModel::updateMap );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsRemoved, this, [ = ]( const QModelIndex &, int, int ) { buildMap( mLayerTreeModel ); } );
  connect( mLayerTreeModel, &QAbstractItemModel::rowsInserted, this, [ = ]( const QModelIndex &, int, int ) { buildMap( mLayerTreeModel ); } );
}

void FlatLayerTreeModel::updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  QModelIndex modifiedIndex = mapFromSource( topLeft );
  if ( modifiedIndex.isValid() )
  {
    emit dataChanged( modifiedIndex, modifiedIndex, roles );
  }
}

int FlatLayerTreeModel::buildMap( QgsLayerTreeModel *model, const QModelIndex &parent, int row, int treeLevel )
{
  bool reset = false;
  if ( row == 0 )
  {
    reset = true;
    beginResetModel();
    mRowMap.clear();
    mIndexMap.clear();
    mCollapsedItems.clear();
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
          if ( rasterLayer && rasterLayer->dataProvider() )
          {
            QVariantMap uriComponents = QgsProviderRegistry::instance()->decodeUri( rasterLayer->dataProvider()->name(), rasterLayer->source() );
            if ( uriComponents.contains( QStringLiteral( "type" ) ) &&
                 uriComponents.value( QStringLiteral( "type" ) ).toString() == QStringLiteral( "xyz" ) )
              // XYZ raster layers have no legend items, yet report so, skip those.
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

void FlatLayerTreeModel::setSourceModel( QgsLayerTreeModel *sourceModel )
{
  QAbstractProxyModel::setSourceModel( sourceModel );
  buildMap( mLayerTreeModel );
}

QModelIndex FlatLayerTreeModel::mapToSource( const QModelIndex &proxyIndex ) const
{
  if ( !proxyIndex.isValid() || !mIndexMap.contains( proxyIndex.row() ) )
    return QModelIndex();
  return mIndexMap[proxyIndex.row()];
}

QModelIndex FlatLayerTreeModel::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mRowMap.contains( sourceIndex ) )
    return QModelIndex();
  return createIndex( mRowMap[sourceIndex], sourceIndex.column() );
}

QModelIndex FlatLayerTreeModel::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )
  return QModelIndex();
}
int FlatLayerTreeModel::columnCount( const QModelIndex &parent ) const
{
  return sourceModel()->columnCount( mapToSource( parent ) );
}
int FlatLayerTreeModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? mRowMap.size() : 0;
}

QModelIndex FlatLayerTreeModel::index( int row, int column, const QModelIndex &parent ) const
{
  return !parent.isValid() ? createIndex( row, column ) : QModelIndex();
}

QVariant FlatLayerTreeModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case VectorLayerPointer:
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

    case MapLayerPointer:
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

    case LegendImage:
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

    case Type:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
        return QStringLiteral( "layer" );
      else if ( QgsLayerTree::isGroup( node ) )
        return QStringLiteral( "group" );
      else
        return QStringLiteral( "legend" );
    }

    case LayerType:
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

    case Name:
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

    case Visible:
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

    case Trackable:
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

    case InTracking:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );

        return ( mLayersInTracking.contains( nodeLayer ) );
      }
      return false;
    }

    case ReadOnly:
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

    case GeometryLocked:
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

    case TreeLevel:
    {
      return mTreeLevelMap.contains( index.row() ) ? mTreeLevelMap[index.row()] : 0;
    }

    case IsValid:
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

    case IsCollapsed:
    {
      QModelIndex sourceIndex = mapToSource( index );
      return mCollapsedItems.contains( sourceIndex );
    }

    case IsParentCollapsed:
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

    case HasChildren:
    {
      QModelIndex sourceIndex = mapToSource( index );
      return mLayerTreeModel->hasChildren( sourceIndex );
    }

    default:
      return QAbstractProxyModel::data( index, role );
  }
}

bool FlatLayerTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  switch ( role )
  {
    case Visible:
    {
      QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) );
      if ( sym )
      {
        QVariant checked = value.toBool() ? Qt::Checked : Qt::Unchecked;
        sym->setData( checked, Qt::CheckStateRole );
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
        node->setItemVisibilityCheckedRecursive( value.toBool() );
      }

      //visibility of the node's children are also impacted, use the tree level value to identify those
      int treeLevel = mTreeLevelMap[index.row()];
      int endRow = index.row();
      while ( mTreeLevelMap.contains( endRow + 1 ) && mTreeLevelMap[endRow + 1] > treeLevel )
        endRow++;

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << Visible );
      return true;
    }

    case IsCollapsed:
    {
      QModelIndex sourceIndex = mapToSource( index );
      const bool collapsed = value.toBool();
      if ( collapsed && !mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems << sourceIndex;
      }
      else if  ( !collapsed && mCollapsedItems.contains( sourceIndex ) )
      {
        mCollapsedItems.removeAll( sourceIndex );
      }

      //the node's children are also impacted, use the tree level value to identify those
      int treeLevel = mTreeLevelMap[index.row()];
      int endRow = index.row();
      while ( mTreeLevelMap.contains( endRow + 1 ) && mTreeLevelMap[endRow + 1] > treeLevel )
        endRow++;

      emit dataChanged( index, createIndex( endRow, 0 ), QVector<int>() << IsCollapsed << IsParentCollapsed );
      return true;
    }

    default:
      return false;
  }
}

QHash<int, QByteArray> FlatLayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[VectorLayerPointer] = "VectorLayerPointer";
  roleNames[MapLayerPointer] = "MapLayerPointer";
  roleNames[LegendImage] = "LegendImage";
  roleNames[Visible] = "Visible";
  roleNames[Type] = "Type";
  roleNames[Name] = "Name";
  roleNames[InTracking] = "InTracking";
  roleNames[ReadOnly] = "ReadOnly";
  roleNames[GeometryLocked] = "GeometryLocked";
  roleNames[TreeLevel] = "TreeLevel";
  roleNames[LayerType] = "LayerType";
  roleNames[IsValid] = "IsValid";
  roleNames[IsCollapsed] = "IsCollapsed";
  roleNames[IsParentCollapsed] = "IsParentCollapsed";
  roleNames[HasChildren] = "HasChildren";
  return roleNames;
}

QgsProject *FlatLayerTreeModel::project() const
{
  return mProject;
}

QgsLayerTreeModel *FlatLayerTreeModel::layerTreeModel() const
{
  return mLayerTreeModel;
}

QgsLayerTree *FlatLayerTreeModel::layerTree() const
{
  return mLayerTreeModel->rootGroup();
}

QString FlatLayerTreeModel::mapTheme() const
{
  return mMapTheme;
}

void FlatLayerTreeModel::setMapTheme( const QString &mapTheme )
{
  if ( mMapTheme == mapTheme )
    return;

  mMapTheme = mapTheme;
  emit mapThemeChanged();

  buildMap( mLayerTreeModel );
}

void FlatLayerTreeModel::updateCurrentMapTheme()
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

void FlatLayerTreeModel::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
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

  emit dataChanged( index, index, QVector<int>() << InTracking );
}
