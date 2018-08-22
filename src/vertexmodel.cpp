/***************************************************************************
  vertexModel.cpp - VertexModel

 ---------------------
 begin                : 16.9.2016
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

#include <QDebug>
#include <QStandardItem>

#include "qgsgeometry.h"
#include "qgswkbtypes.h"
#include "qgscoordinatereferencesystem.h"

#include "vertexmodel.h"


VertexModel::VertexModel( QObject* parent )
  : QStandardItemModel( parent )
{

}

void VertexModel::setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs )
{
  clear();

  bool isMulti = QgsWkbTypes::isMultiType( geometry.wkbType() );

  setColumnCount( 1 );
  setRowCount( 0 );

  const QgsAbstractGeometry *geom = geometry.constGet();
  if ( !geom )
    return;

  QgsVertexId vertexId;
  QgsPoint pt;
  while ( geom->nextVertex( vertexId, pt ) )
  {
    if ( vertexId.part > 1 || vertexId.ring > 1 )
      return;

    QStandardItem *item = new QStandardItem();
    item->setData( QVariant::fromValue<QgsPoint>( pt ), PointRole );
    appendRow( QList<QStandardItem*>() << item );
  }
}

QHash<int, QByteArray> VertexModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PointRole] = "Point";
  return roles;
}

