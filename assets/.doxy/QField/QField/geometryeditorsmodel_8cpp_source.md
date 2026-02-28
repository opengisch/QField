

# File geometryeditorsmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**geometryeditorsmodel.cpp**](geometryeditorsmodel_8cpp.md)

[Go to the documentation of this file](geometryeditorsmodel_8cpp.md)


```C++
/***************************************************************************
  geometryeditorsmodel.cpp

 ---------------------
 begin                : 24.09.2019
 copyright            : (C) 2019 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "geometryeditorsmodel.h"

#include <qgswkbtypes.h>

GeometryEditorsModel::GeometryEditorsModel( QObject *parent )
  : QStandardItemModel( parent )
{
  setColumnCount( 1 );
}


QHash<int, QByteArray> GeometryEditorsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IconPathRole] = "iconPath";
  roles[NameRole] = "name";
  roles[ToolbarRole] = "toolbar";
  roles[SupportedGeometriesRole] = "supportedGeometries";
  return roles;
}

void GeometryEditorsModel::addEditor( const QString &name, const QString &icon, const QString &toolbar, SupportedGeometries supportedGeometries )
{
  QStandardItem *item = new QStandardItem;
  item->setData( name, NameRole );
  item->setData( icon, IconPathRole );
  item->setData( toolbar, ToolbarRole );
  item->setData( static_cast<int>( supportedGeometries ), SupportedGeometriesRole );
  appendRow( item );
}

bool GeometryEditorsModel::supportsGeometry( const QgsGeometry &geometry, const SupportedGeometries &supportedGeometries )
{
  switch ( geometry.type() )
  {
    case Qgis::GeometryType::Point:
      return supportedGeometries.testFlag( Point );
    case Qgis::GeometryType::Line:
      return supportedGeometries.testFlag( Line );
    case Qgis::GeometryType::Polygon:
      return supportedGeometries.testFlag( Polygon );
    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
      return false;
  }

  return false;
}

void GeometryEditorsModel::setVertexModel( VertexModel *vertexModel )
{
  if ( mVertexModel == vertexModel )
    return;
  mVertexModel = vertexModel;
  emit vertexModelChanged();
}
```


