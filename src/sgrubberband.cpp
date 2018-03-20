#include "sgrubberband.h"
extern "C" {
#include "tessellate.h"
}

SGRubberband::SGRubberband( const QVector<QgsPoint>& points, QgsWkbTypes::GeometryType type, const QColor& color, qreal width )
  : QgsQuickHighlightSGNode(points, type, color, width)
{
  // Everything else is implemented in base class
  if (type == QgsWkbTypes::PolygonGeometry)
  {
      appendChildNode( createLineGeometry( points, width ) );
      appendChildNode( createPolygonGeometry( points ) );
  }
}

SGRubberband::~SGRubberband()
{
}

QSGGeometryNode*SGRubberband::createPolygonGeometry( const QVector<QgsPoint>& points )
{
  double* coordinates_out;
  int* tris_out;
  int nverts, ntris;

  double* vertices_in = ( double* )malloc( points.size() * 2 * sizeof( double ) );
  const double* contours_array[] = { vertices_in, vertices_in + points.size() * 2 };
  int i = 0;

  Q_FOREACH( const QgsPoint& pt, points )
  {
    vertices_in[i++] = pt.x();
    vertices_in[i++] = pt.y();
  }

  tessellate( &coordinates_out, &nverts,
              &tris_out, &ntris,
              contours_array, contours_array + 2 );

  QSGGeometryNode* node = new QSGGeometryNode;
  QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), ntris * 3 );

  QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

  for ( int j = 0; j < ntris*3; j++ )
  {
    vertices[j].x = coordinates_out[tris_out[j]*2];
    vertices[j].y = coordinates_out[tris_out[j]*2+1];
  }

  free( vertices_in );
  free( coordinates_out );
  free( tris_out );

  sgGeom->setDrawingMode( GL_TRIANGLES );
  node->setGeometry( sgGeom );
  node->setMaterial( &mMaterial );
  node->setFlag( QSGNode::OwnsGeometry );
  node->setFlag( QSGNode::OwnedByParent );
  return node;
}
