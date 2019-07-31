/***************************************************************************
  badlayerhandler.cpp - BadLayerHandler

 ---------------------
 begin                : 3.10.2016
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
#include "badlayerhandler.h"
#include <qgsproject.h>
#include <QDebug>
#include "qgscredentials.h"

BadLayerHandler::BadLayerHandler( QObject *parent )
  : QStandardItemModel( parent )
{

}

QHash<int, QByteArray> BadLayerHandler::roleNames() const
{
  QHash<int, QByteArray> roleNames = QStandardItemModel::roleNames();

  roleNames[DataSourceRole] = "DataSource";
  roleNames[LayerNameRole] = "LayerName";

  return roleNames;
}

QgsProject *BadLayerHandler::project() const
{
  return mProject;
}

void BadLayerHandler::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;

  mProject->setBadLayerHandler( this );
  emit projectChanged();
}

void BadLayerHandler::handleBadLayers( const QList<QDomNode> &layers )
{
  clear();

  for ( const QDomNode &node : layers )
  {
    QStandardItem *item = new QStandardItem();
    item->setData( dataSource( node ), DataSourceRole );
    item->setData( layerName( node ), LayerNameRole );
    //item->setData( node, LayerNodeRole );
    appendRow( item );


    QDomElement element = node.toElement();
    qDebug() << "Text is: \n" << element.text();


    QDomNodeList childNodes = node.childNodes();

    for ( int i = 0; i < childNodes.count(); i++ )
    {

      const QDomNode &child = childNodes.at( i );
      qDebug() << "Kiddo " << i << " " << child.nodeName() << " " << child.toElement().text();
    }
  }

  emit badLayersFound();
}

QString BadLayerHandler::layerName( const QDomNode &layerNode ) const
{
  return layerNode.namedItem( "layername" ).toElement().text();
}

