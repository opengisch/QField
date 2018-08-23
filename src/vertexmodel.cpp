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
#include "qgswkbtypes.h"
#include "qgslinestring.h"
#include "qgspolygon.h"

#include "vertexmodel.h"
#include "mapsettings.h"


VertexModel::VertexModel( QObject* parent )
  : QStandardItemModel( parent )
{
}

void VertexModel::setMapSettings( MapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;

  emit mapSettingsChanged();
}

MapSettings *VertexModel::mapSettings()
{
  return mMapSettings;
}

void VertexModel::setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs )
{
  clear();
  mOriginalGeoemtry = geometry;
  mCurrentVertex = -1;
  mGeometryType = geometry.type();
  QgsGeometry geom = QgsGeometry( geometry );
  mCrs = crs;

  if ( mMapSettings && mCrs.isValid() )
  {
    QgsCoordinateTransform ct( mCrs, mMapSettings->destinationCrs(), mMapSettings->transformContext() );
    geom.transform( ct );
  }

  mIsMulti = QgsWkbTypes::isMultiType( geometry.wkbType() );

  setColumnCount( 1 );
  setRowCount( 0 );

  const QgsAbstractGeometry *abstractGeom = geom.constGet();
  if ( !abstractGeom )
    return;

  QgsVertexId vertexId;
  QgsPoint pt;
  int r = 0;
  while ( abstractGeom->nextVertex( vertexId, pt ) )
  {
    if ( vertexId.part > 1 || vertexId.ring > 1 )
      return;

    // skip first vertex on polygon, as it's duplicate from last
    if ( geometry.type() == QgsWkbTypes::PolygonGeometry && vertexId.vertex == 0 )
      continue;

    QStandardItem *item = new QStandardItem();
    item->setData( QVariant::fromValue<QgsPoint>( pt ), PointRole );
    item->setData( r == mCurrentVertex, CurrentVertexRole );
    appendRow( QList<QStandardItem*>() << item );
    r++;
  }

  emit vertexCountChanged();
}

QgsGeometry VertexModel::geometry() const
{
  if ( mIsMulti )
  {
    // TODO: handle multi, for now return original to avoid any data destruction
    return mOriginalGeoemtry;
  }

  QVector<QgsPoint> vertices = flatVertices();
  QgsGeometry geometry;

  switch ( mGeometryType )
  {
    case QgsWkbTypes::PointGeometry:
    {
      geometry.set( new QgsPoint( vertices.first() ) );
      break;
    }
    case QgsWkbTypes::LineGeometry:
    {
      geometry = QgsGeometry::fromPolyline( vertices );
      break;
    }
    case QgsWkbTypes::PolygonGeometry:
    {
      std::unique_ptr<QgsLineString> ls( qgis::make_unique<QgsLineString>( vertices ) );
      std::unique_ptr<QgsPolygon> polygon( qgis::make_unique<QgsPolygon>() );
      polygon->setExteriorRing( ls.release() );
      geometry.set( polygon.release() );
      break;
    }
    case QgsWkbTypes::NullGeometry:
    case QgsWkbTypes::UnknownGeometry:
      break;
  }

  if ( mMapSettings && mCrs.isValid() )
  {
    QgsCoordinateTransform ct( mMapSettings->destinationCrs(), mCrs, mMapSettings->transformContext() );
    geometry.transform( ct );
  }
  return geometry;

}

void VertexModel::clear()
{
  QStandardItemModel::clear();
  emit vertexCountChanged();
}

void VertexModel::previousVertex()
{
  setCurrentVertex( mCurrentVertex -1 );
}

void VertexModel::nextVertex()
{
  setCurrentVertex( mCurrentVertex+1 );
}

VertexModel::EditingMode VertexModel::editingMode() const
{
  return mMode;
}

QgsPoint VertexModel::currentPoint() const
{
  return mCurrentPoint;
}

void VertexModel::setCurrentPoint( const QgsPoint &point )
{
  if ( mMode == EditVertex )
  {
    QStandardItem *it = item( mCurrentVertex );
    if ( it )
      it->setData( QVariant::fromValue<QgsPoint>( point ), PointRole );
  }
}

int VertexModel::vertexCount() const
{
  return rowCount();
}

QgsWkbTypes::GeometryType VertexModel::geometryType() const
{
  return mGeometryType;
}

QVector<QgsPoint> VertexModel::flatVertices() const
{
  QVector<QgsPoint> vertices = QVector<QgsPoint>();
  for ( int r=0; r<rowCount(); r++ )
  {
    QStandardItem *it = item( r );
    vertices << qvariant_cast<QgsPoint>( it->data( PointRole ) );
  }
  // re-append
  if ( mGeometryType == QgsWkbTypes::PolygonGeometry )
  {
    vertices << vertices.constFirst();
  }
  return vertices;
}

QHash<int, QByteArray> VertexModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PointRole] = "Point";
  roles[CurrentVertexRole] = "CurrentVertex";
  return roles;
}

void VertexModel::setCurrentVertex( int newVertex )
{
  if ( newVertex < 0 )
    newVertex = rowCount()-1;

  if ( newVertex >= rowCount() )
    newVertex = 0;

  if ( rowCount() == 0 )
  {
    setEditingMode( NoEditing );
    newVertex = -1;
  }

  if ( mCurrentVertex == newVertex )
    return;

  mCurrentVertex = newVertex;

  for ( int r=0; r<rowCount(); r++ )
  {
    QStandardItem *it = item( r );
    it->setData( r == mCurrentVertex, CurrentVertexRole );

    if ( r == mCurrentVertex )
    {
      mCurrentPoint = qvariant_cast<QgsPoint>( it->data( PointRole ) );
      emit currentPointChanged();
      setEditingMode( EditVertex );
    }
  }
}

void VertexModel::setEditingMode( VertexModel::EditingMode mode )
{
  if ( mMode == mode )
    return;

  mMode = mode;
  emit editingModeChanged();
}

