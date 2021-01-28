/***************************************************************************
    qgssggeometry.cpp
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
#include "math.h"

#include "qgssggeometry.h"
extern "C" {
#include "tessellate.h"
}


QgsSGGeometry::QgsSGGeometry()
{
}

QgsSGGeometry::QgsSGGeometry( const QgsGeometry &geom, const QColor &color, int width, const QgsRectangle visibleExtent, double scaleFactor )
{
  //TODO: Fix const-correcteness upstream
  QgsGeometry &gg = const_cast<QgsGeometry &>( geom );

  mMaterial.setColor( color );

  switch ( gg.type() )
  {
    case QgsWkbTypes::PointGeometry:
      // we should never get point here, use GeometryRenderer quick item to render geometries
      Q_ASSERT( false );
      break;

    case QgsWkbTypes::LineGeometry:
      if ( gg.isMultipart() )
      {
        const QgsMultiPolylineXY lines = gg.asMultiPolyline();

        for ( const QgsPolylineXY &line : lines )
        {
          QSGGeometryNode *geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolylineToQSGGeometry( line, width, visibleExtent, scaleFactor ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }
      }
      else
      {
        QSGGeometryNode *geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolylineToQSGGeometry( gg.asPolyline(), width, visibleExtent, scaleFactor ) );
        geomNode->setFlag( QSGNode::OwnsGeometry );
        applyStyle( geomNode );
        appendChildNode( geomNode );
      }
      break;

    case QgsWkbTypes::PolygonGeometry:
      if ( gg.isMultipart() )
      {
        const QgsMultiPolygonXY polygons = gg.asMultiPolygon();

        QSGOpacityNode *on = new QSGOpacityNode;
        on->setOpacity( 0.5 );

        for ( const QgsPolygonXY &polygon : polygons )
        {
          QSGGeometryNode *geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolygonToQSGGeometry( polygon, visibleExtent, scaleFactor ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          on->appendChildNode( geomNode );

          geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolylineToQSGGeometry( polygon.first(), width, visibleExtent, scaleFactor ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }

        appendChildNode( on );
      }
      else
      {
        QSGOpacityNode *on = new QSGOpacityNode;
        on->setOpacity( 0.5 );
        QSGGeometryNode *geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolygonToQSGGeometry( gg.asPolygon(), visibleExtent, scaleFactor ) );
        geomNode->setFlag( QSGNode::OwnsGeometry );
        applyStyle( geomNode );
        on->appendChildNode( geomNode );
        appendChildNode( on );
        geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolylineToQSGGeometry( gg.asPolygon().first(), width, visibleExtent, scaleFactor ) );
        geomNode->setFlag( QSGNode::OwnsGeometry );
        applyStyle( geomNode );
        appendChildNode( geomNode );
      }
      break;

    default:
      // Nothing to do
      break;
  }
}

void QgsSGGeometry::applyStyle( QSGGeometryNode *geomNode )
{
  geomNode->setMaterial( &mMaterial );
}

QSGGeometry *QgsSGGeometry::qgsPolylineToQSGGeometry( const QgsPolylineXY &line, int width, const QgsRectangle visibleExtent, double scaleFactor )
{
  QSGGeometry *sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), line.count() );
  QSGGeometry::Point2D *vertices = sgGeom->vertexDataAsPoint2D();

  int i = 0;
  for ( const QgsPointXY &point : line )
  {
    vertices[i].set( static_cast<float>( ( point.x() - visibleExtent.xMinimum() ) * scaleFactor ),
                     static_cast<float>( ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor ) );
    i++;
  }

  sgGeom->setLineWidth( width );
  sgGeom->setDrawingMode( QSGGeometry::DrawLineStrip );

  return sgGeom;
}

QSGGeometry *QgsSGGeometry::qgsPolygonToQSGGeometry( const QgsPolygonXY &polygon, const QgsRectangle visibleExtent, double scaleFactor )
{
  QgsPolygonXY::ConstIterator it = polygon.constBegin();

  double *coordinates_out;
  int *tris_out;
  int nverts, ntris;

  const QgsPolylineXY &ring = *it;

  double *vertices_in = ( double * )malloc( ring.size() * 2 * sizeof( double ) );
  const double *contours_array[] = { vertices_in, vertices_in + ring.size() * 2 };
  int i = 0;

  for ( const QgsPointXY &point : ring )
  {
    vertices_in[i++] = ( point.x() - visibleExtent.xMinimum() ) * scaleFactor;
    vertices_in[i++] = ( point.y() - visibleExtent.yMaximum() ) * -scaleFactor;
  }

  tessellate( &coordinates_out, &nverts,
              &tris_out, &ntris,
              contours_array, contours_array + 2 );

  QSGGeometry *sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), ntris * 3 );

  QSGGeometry::Point2D *vertices = sgGeom->vertexDataAsPoint2D();

  for ( int j = 0; j < ntris * 3; j++ )
  {
    vertices[j].x = coordinates_out[tris_out[j] * 2];
    vertices[j].y = coordinates_out[tris_out[j] * 2 + 1];
  }

  free( vertices_in );
  free( coordinates_out );
  free( tris_out );

  sgGeom->setDrawingMode( QSGGeometry::DrawTriangles );

  return sgGeom;
}
