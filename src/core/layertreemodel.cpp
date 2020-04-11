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
#include <qgsvectorlayer.h>

#include <QDebug>

FlatLayerTreeModel::FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QAbstractProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  setSourceModel ( mLayerTreeModel );
  connect( mProject, &QgsProject::readProject, this, [ = ] { buildMap( mLayerTreeModel ); } );
}

int FlatLayerTreeModel::buildMap( QgsLayerTreeModel *model, const QModelIndex &parent, int row )
{
  bool reset = false;
  if ( row == 0 )
  {
    reset = true;
    beginResetModel();
    mRowMap.clear();
    mIndexMap.clear();
  }

  int nbRows = model->rowCount( parent );
  for ( int i = 0; i < nbRows; i++ )
  {
    QModelIndex index = model->index( i, 0, parent );
    mRowMap[index] = row;
    mIndexMap[row] = index;
    row++;
    if ( model->hasChildren( index ) )
      row = buildMap( model, index, row );
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
int FlatLayerTreeModel::rowCount( const QModelIndex &parent) const
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

    case LegendImage:
    {
      QString id;

      if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        id += QStringLiteral( "legend" );
        id += '/' + sym->layerNode()->layerId();
        id += '/' + sym->data( Qt::DisplayRole ).toString();
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );

        if ( QgsLayerTree::isLayer( node ) )
        {
          QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
          id += QStringLiteral( "layer" );
          id += '/' +  nodeLayer->layerId();
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
        return sym->data( Qt::CheckStateRole ).toBool();
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
        if ( layer )
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

    default:
      return QAbstractProxyModel::data( index, role );
  }
}

bool FlatLayerTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( role == Visible )
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
    return true;
  }
  return false;
}

QHash<int, QByteArray> FlatLayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[VectorLayerPointer] = "VectorLayerPointer";
  roleNames[LegendImage] = "LegendImage";
  roleNames[Visible] = "Visible";
  roleNames[Type] = "Type";
  roleNames[Name] = "Name";
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

//------------------

LayerTreeModel::LayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mProject( project )
{
  mLayerTreeModel = new QgsLayerTreeModel( layerTree, this );
  setSourceModel( mLayerTreeModel );
}

LayerTreeModel::~LayerTreeModel()
{
  qDeleteAll( mLayersInTracking );
}

QVariant LayerTreeModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case VectorLayer:
    {
      QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( nodeLayer->layer() );
        return QVariant::fromValue<QgsVectorLayer *>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    case LegendImage:
    {
      QString id;

      if ( QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        id += QStringLiteral( "legend" );
        id += '/' + sym->layerNode()->layerId();
        id += '/' + sym->data( Qt::DisplayRole ).toString();
      }
      else
      {
        QgsLayerTreeNode *node = mLayerTreeModel->index2node( mapToSource( index ) );

        if ( QgsLayerTree::isLayer( node ) )
        {
          QgsLayerTreeLayer *nodeLayer = QgsLayerTree::toLayer( node );
          id += QStringLiteral( "layer" );
          id += '/' +  nodeLayer->layerId();
        }
        /*
        else if ( QgsLayerTree::isGroup( node ) )
        {
          id += QStringLiteral( "group" );
        }*/
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

    case Visible:
    {
      QgsLayerTreeModelLegendNode *sym = mLayerTreeModel->index2legendNode( mapToSource( index ) );
      if ( sym )
      {
        return sym->data( Qt::CheckStateRole ).toBool();
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
        if ( layer )
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

    default:
      return QSortFilterProxyModel::data( index, role );
  }
}

bool LayerTreeModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( role == Visible )
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
    return true;
  }
  return false;
}



QHash<int, QByteArray> LayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QSortFilterProxyModel::roleNames();

  roleNames[VectorLayer] = "VectorLayer";
  roleNames[Visible] = "Visible";
  roleNames[Type] = "Type";
  roleNames[Trackable] = "trackable";
  roleNames[InTracking] = "inTracking";
  return roleNames;
}

QgsLayerTreeModel *LayerTreeModel::layerTreeModel() const
{
  return mLayerTreeModel;
}

QgsLayerTree *LayerTreeModel::layerTree() const
{
  return mLayerTreeModel->rootGroup();
}

QString LayerTreeModel::mapTheme() const
{
  return mMapTheme;
}

void LayerTreeModel::setMapTheme( const QString &mapTheme )
{
  if ( mMapTheme == mapTheme )
    return;

  mMapTheme = mapTheme;
  emit mapThemeChanged();
}

QgsProject *LayerTreeModel::project() const
{
  return mProject;
}

void LayerTreeModel::updateCurrentMapTheme()
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

void LayerTreeModel::setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking )
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

bool LayerTreeModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  QgsLayerTreeNode *node = mLayerTreeModel->index2node( source_parent );
  QgsLayerTreeGroup *parentgroup = qobject_cast < QgsLayerTreeGroup *>( node );

  if ( parentgroup )
  {
    QList<QgsLayerTreeNode *> children = parentgroup->children();
    QgsLayerTreeNode *child = children.at( source_row );
    if ( child )
    {
      QVariant nodeHidden = child-> customProperty( QStringLiteral( "nodeHidden" ), QStringLiteral( "false" ) );
      if ( nodeHidden.toString() == QStringLiteral( "true" ) )
      {
        return false;
      }
      else
      {
        return true;
      }
    }
  }
  return true;
}
