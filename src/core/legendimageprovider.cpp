/***************************************************************************
  legendimageprovider.cpp - LegendImageProvider

 ---------------------
 begin                : 7.12.2016
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
#include "legendimageprovider.h"

#include <qgsproject.h>
#include <qgslayertree.h>
#include <qgslayertreelayer.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemodellegendnode.h>

LegendImageProvider::LegendImageProvider( QgsLayerTreeModel *layerTreeModel )
  : QQuickImageProvider( Pixmap )
  , mLayerTreeModel( layerTreeModel )
  , mRootNode( layerTreeModel->rootGroup() )
{
  mLayerTreeModel->setFlag( QgsLayerTreeModel::ShowLegendAsTree, true );
}

QPixmap LegendImageProvider::requestPixmap( const QString &id, QSize *size, const QSize &requestedSize )
{
  Q_UNUSED( size )
  const int iconSize = mLayerTreeModel->scaleIconSize( 16 );

  // the id is passed on as an encoded URL string which needs decoding
  const QString decodedId = QUrl::fromPercentEncoding( id.toUtf8() );
  QStringList idParts = decodedId.split( '/' ).mid( 0, 2 );
  idParts << decodedId.section( '/', 2 );

  if ( idParts.value( 0 ) == QStringLiteral( "legend" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );
    QModelIndex layerIndex = mLayerTreeModel->node2index( layerNode );
    QModelIndex index = mLayerTreeModel->index( 0, 0, layerIndex );
    QStringList legendParts;
    while( index.isValid() )
    {
      legendParts << mLayerTreeModel->data( index ).toString();
      if ( idParts.value( 2 ) == legendParts.join( QStringLiteral( "~__~" ) ) )
      {
        QPixmap pixmap = mLayerTreeModel->data( index, Qt::DecorationRole ).value<QPixmap>();
        if ( pixmap.isNull() )
        {
          QIcon icon = mLayerTreeModel->data( index, Qt::DecorationRole ).value<QIcon>();
          if ( !icon.isNull() )
            pixmap = icon.pixmap( iconSize, iconSize );
        }
        if ( pixmap.isNull() )
        {
          pixmap = QPixmap( iconSize, iconSize );
          pixmap.fill( QColor( 255, 255, 255 ) );
        }
        return pixmap;
      }
      else
      {
        if ( mLayerTreeModel->hasChildren( index ) )
        {
          index = mLayerTreeModel->index( 0, 0, index );
        }
        else
        {
          legendParts.removeLast();

          QModelIndex nextIndex = mLayerTreeModel->sibling( index.row() + 1, 0, index );
          if ( !nextIndex.isValid() && index.parent() != layerIndex )
          {
            legendParts.removeLast();
            nextIndex = mLayerTreeModel->sibling( index.parent().row() + 1, 0, index.parent() );
          }
          index = nextIndex;
        }
      }
    }
  }

  if ( idParts.value( 0 ) == QStringLiteral( "layer" ) )
  {
    QgsLayerTreeLayer *layerNode = mRootNode->findLayer( idParts.value( 1 ) );

    if ( layerNode )
    {
      QgsLayerTreeModelLegendNode *legendNode = mLayerTreeModel->legendNodeEmbeddedInParent( layerNode );
      if ( legendNode )
      {
        QPixmap pixmap = legendNode->data( Qt::DecorationRole ).value<QPixmap>();
        if ( pixmap.isNull() )
        {
         QIcon icon = legendNode->data( Qt::DecorationRole ).value<QIcon>();
         if ( !icon.isNull() )
           pixmap = icon.pixmap( iconSize, iconSize );
        }
        if ( pixmap.isNull() )
        {
          pixmap = QPixmap( iconSize, iconSize );
          pixmap.fill( QColor( 255, 255, 255 ) );
        }
        return pixmap;
      }
      else
      {
        QPixmap pixmap( iconSize, iconSize );
        pixmap.fill( QColor( 255, 255, 255 ) );
        return pixmap;
      }
    }
  }

  return QPixmap( requestedSize );
}
