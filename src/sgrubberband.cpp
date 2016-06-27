#include "sgrubberband.h"

SGRubberband::SGRubberband(const QVector<QgsPoint>& points, QGis::GeometryType type, const QColor& color, qreal width )
  : QSGNode()
{
  mMaterial.setColor( color );

  switch ( type )
  {
    case QGis::Point:
      // TODO: Implement
      break;

    case QGis::Line:
    {
      QSGGeometryNode* node = new QSGGeometryNode;
      QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), points.count() );
      QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

      int i = 0;
      Q_FOREACH( const QgsPoint& pt, points )
      {
        vertices[i++].set( pt.x(), pt.y() );
      }

      sgGeom->setLineWidth( width );
      sgGeom->setDrawingMode( GL_LINE_STRIP );
      node->setGeometry( sgGeom );
      node->setMaterial( &mMaterial );
      node->setFlag( QSGNode::OwnsGeometry );
      node->setFlag( QSGNode::OwnedByParent );
      appendChildNode( node );
      break;
    }

    case QGis::Polygon:
      // TODO: Implement
      break;

    case QGis::UnknownGeometry:
    case QGis::NoGeometry:
      break;
  }
}
