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
#include <qgsvectorlayer.h>
#include <qgslayertreemodellegendnode.h>

LayerTreeModel::LayerTreeModel( QgsLayerTreeGroup* rootNode, QObject* parent )
  : QSortFilterProxyModel( parent )
{
  mLayerTreeModel = new QgsLayerTreeModel( rootNode, this );
  setSourceModel( mLayerTreeModel );
}

QVariant LayerTreeModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case VectorLayer:
    {
      QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
      {
        QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
        QgsVectorLayer* layer = qobject_cast<QgsVectorLayer*>( nodeLayer->layer() );
        return QVariant::fromValue<QgsVectorLayer*>( layer );
      }
      else
      {
        return QVariant();
      }
    }

    case LegendImage:
    {
      QString id;

      if ( QgsLayerTreeModelLegendNode* sym = mLayerTreeModel->index2legendNode( mapToSource( index ) ) )
      {
        id += QStringLiteral( "legend" );
        id += '/' + sym->layerNode()->layerId();
        id += '/' + sym->data( Qt::DisplayRole ).toString();
      }
      else
      {
        QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );

        if ( QgsLayerTree::isLayer( node ) )
        {
          QgsLayerTreeLayer* nodeLayer = QgsLayerTree::toLayer( node );
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
      QgsLayerTreeNode* node = mLayerTreeModel->index2node( mapToSource( index ) );
      if ( QgsLayerTree::isLayer( node ) )
        return QStringLiteral( "layer" );
      else if ( QgsLayerTree::isGroup( node ) )
        return QStringLiteral( "group" );
      else
        return QStringLiteral( "legend" );
    }

    default:
      return QSortFilterProxyModel::data( index, role );
  }
}

QHash<int, QByteArray> LayerTreeModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QSortFilterProxyModel::roleNames();

  roleNames[VectorLayer] = "VectorLayer";

  return roleNames;
}

QgsLayerTreeModel* LayerTreeModel::layerTreeModel() const
{
  return mLayerTreeModel;
}
