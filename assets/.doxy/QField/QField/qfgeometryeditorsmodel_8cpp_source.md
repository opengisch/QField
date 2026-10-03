

# File qfgeometryeditorsmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfgeometryeditorsmodel.cpp**](qfgeometryeditorsmodel_8cpp.md)

[Go to the documentation of this file](qfgeometryeditorsmodel_8cpp.md)


```C++
/***************************************************************************
  qfgeometryeditorsmodel.cpp

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

#include "qfgeometryeditorsmodel.h"

#include <qgswkbtypes.h>

QfGeometryEditorsModel::QfGeometryEditorsModel( QObject *parent )
  : QStandardItemModel( parent )
{
  setColumnCount( 1 );
}


QHash<int, QByteArray> QfGeometryEditorsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IconPathRole] = "iconPath";
  roles[NameRole] = "name";
  roles[ToolbarRole] = "toolbar";
  roles[SupportedGeometriesRole] = "supportedGeometries";
  return roles;
}

void QfGeometryEditorsModel::addEditor( const QString &name, const QString &icon, const QString &toolbar, SupportedGeometries supportedGeometries )
{
  QStandardItem *item = new QStandardItem;
  item->setData( name, NameRole );
  item->setData( icon, IconPathRole );
  item->setData( toolbar, ToolbarRole );
  item->setData( static_cast<int>( supportedGeometries ), SupportedGeometriesRole );
  appendRow( item );
}

bool QfGeometryEditorsModel::supportsGeometry( const QgsGeometry &geometry, const SupportedGeometries &supportedGeometries )
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

void QfGeometryEditorsModel::setVertexModel( QfVertexModel *vertexModel )
{
  if ( mVertexModel == vertexModel )
    return;
  mVertexModel = vertexModel;
  emit vertexModelChanged();
}
```


