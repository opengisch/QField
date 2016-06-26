#include "sgrubberband.h"

SGRubberband::SGRubberband( const QVector<QgsPoint>& points, QGis::GeometryType type )
  : QSGGeometryNode()
{
  QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), points.count() );
  QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

  mMaterial.setColor( QColor( 120, 250, 120, 150 ) );

  switch ( type )
  {
    case QGis::Point:
      vertices[0].set( points.at( 0 ).x(), points.at( 0 ).y() );
      break;

    case QGis::Line:
    {
      int i = 0;
      Q_FOREACH( const QgsPoint& pt, points )
        vertices[i++].set( pt.x(), pt.y() );

      sgGeom->setLineWidth( 4 );
      sgGeom->setDrawingMode( GL_LINE_STRIP );
      break;
    }

    case QGis::Polygon:
      // TODO: Implement
      break;

    case QGis::UnknownGeometry:
    case QGis::NoGeometry:
      break;
  }

  setGeometry( sgGeom );
  setFlag( QSGNode::OwnsGeometry );
  setMaterial( &mMaterial );
}
