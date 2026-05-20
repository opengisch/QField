

# File badlayerhandler.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**badlayerhandler.cpp**](badlayerhandler_8cpp.md)

[Go to the documentation of this file](badlayerhandler_8cpp.md)


```C++
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
    appendRow( item );
  }

  emit badLayersFound();
}

QString BadLayerHandler::layerName( const QDomNode &layerNode ) const
{
  return layerNode.namedItem( "layername" ).toElement().text();
}
```


