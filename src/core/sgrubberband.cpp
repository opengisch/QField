/***************************************************************************
    sgrubberband.cpp
    ---------------------
    begin                : March 2020
    copyright            : (C) 2020 by David Signer
    email                : david at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sgrubberband.h"

#include <qgscurvepolygon.h>
#include <qgsgeometry.h>
#include <qgsgeos.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgssurface.h>
#include <qgstessellator.h>

SGRubberband::SGRubberband( const QVector<QgsPoint> &points, QgsWkbTypes::GeometryType type, const QColor &color, float width )
  : QSGNode()
{
  mMaterial.setColor( color );

  if ( points.isEmpty() )
    return;

  switch ( type )
  {
    case QgsWkbTypes::PointGeometry:
      // TODO: Implement
      break;

    case QgsWkbTypes::LineGeometry:
    {
      appendChildNode( createLineGeometry( points, width ) );
      break;
    }

    case QgsWkbTypes::PolygonGeometry:
    {
      appendChildNode( createLineGeometry( points, width ) );

      if ( points.size() > 2 )
      {
        QString error;
        QgsPolygon polygon = QgsPolygon( new QgsLineString( points ) );
        if ( !polygon.isValid( error ) )
        {
          QgsGeos geos( &polygon );
          std::unique_ptr<QgsAbstractGeometry> validPolygon = geos.makeValid();
          QgsGeometryPartIterator parts = validPolygon->parts();
          while ( parts.hasNext() )
          {
            QgsPolygon *p = qgsgeometry_cast<QgsPolygon *>( parts.next() );
            if ( p )
            {
              QgsCurve *l = p->exteriorRing();
              if ( l )
              {
                QgsPointSequence pts;
                l->points( pts );
                appendChildNode( createPolygonGeometry( pts ) );
              }
            }
          }
        }
        else
        {
          appendChildNode( createPolygonGeometry( points ) );
        }
      }
      break;
    }

    case QgsWkbTypes::UnknownGeometry:
    case QgsWkbTypes::NullGeometry:
      break;
  }
}

QSGGeometryNode *SGRubberband::createLineGeometry( const QVector<QgsPoint> &points, float width )
{
  QSGGeometryNode *node = new QSGGeometryNode;
  QSGGeometry *sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), points.count() );
  QSGGeometry::Point2D *vertices = sgGeom->vertexDataAsPoint2D();

  int i = 0;
  for ( const QgsPoint &pt : points )
  {
    vertices[i++].set( static_cast<float>( pt.x() ), static_cast<float>( pt.y() ) );
  }

  sgGeom->setLineWidth( static_cast<float>( width ) );
  sgGeom->setDrawingMode( QSGGeometry::DrawLineStrip );
  node->setGeometry( sgGeom );
  node->setMaterial( &mMaterial );
  node->setFlag( QSGNode::OwnsGeometry );
  node->setFlag( QSGNode::OwnedByParent );
  return node;
}

QSGGeometryNode *SGRubberband::createPolygonGeometry( const QVector<QgsPoint> &points )
{
  QgsPolygon polygon = QgsPolygon( new QgsLineString( points ) );
  QgsTessellator t( 0, 0, false, false, false, true );
  if ( points.size() > 2 )
  {
    t.addPolygon( polygon, 0 );
  }

  QSGGeometryNode *node = new QSGGeometryNode;
  QSGGeometry *sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), t.dataVerticesCount() );
  QSGGeometry::Point2D *vertices = sgGeom->vertexDataAsPoint2D();

  const QVector<float> triangleData = t.data();
  int currentVertex = 0;
  for ( auto it = triangleData.constBegin(); it != triangleData.constEnd(); )
  {
    vertices[currentVertex].x = ( *it++ );
    ( void ) it++; // z
    vertices[currentVertex].y = -( *it++ );
    currentVertex++;

    vertices[currentVertex].x = ( *it++ );
    ( void ) it++; // z
    vertices[currentVertex].y = -( *it++ );
    currentVertex++;

    vertices[currentVertex].x = ( *it++ );
    ( void ) it++; // z
    vertices[currentVertex].y = -( *it++ );
    currentVertex++;
  }

  sgGeom->setDrawingMode( QSGGeometry::DrawTriangles );
  node->setGeometry( sgGeom );
  node->setMaterial( &mMaterial );
  node->setFlag( QSGNode::OwnsGeometry );
  node->setFlag( QSGNode::OwnedByParent );
  return node;
}
