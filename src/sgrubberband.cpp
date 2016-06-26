#include "sgrubberband.h"

SGRubberband::SGRubberband(const QVector<QgsPoint>& points, QGis::GeometryType type, bool lastPointPending, const QColor& color, qreal width )
  : QSGNode()
{
  mMaterial.setColor( color );
  QColor c2 = color;
  c2.setAlpha( 150 );
  mPendingMaterial.setColor( c2 );

  switch ( type )
  {
    case QGis::Point:
      // TODO: Implement
      break;

    case QGis::Line:
    {
      int length;

      if ( lastPointPending )
        length = points.size() - 1;
      else
        length = points.size();

      if ( length > 1 )
      {
        QSGGeometryNode* node = new QSGGeometryNode;
        QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), length );
        QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

        for ( int i = 0; i < length; ++i )
        {
          const QgsPoint& pt = points.at( i );
          vertices[i].set( pt.x(), pt.y() );
        }

        sgGeom->setLineWidth( width );
        node->setMaterial( &mMaterial );
        node->setFlag( QSGNode::OwnedByParent );
        appendChildNode( node );
        qDebug() << "TEST";
      }

      if ( lastPointPending && points.size() > 1 )
      {
        QSGGeometryNode* nodePending = new QSGGeometryNode;
        QSGGeometry* sgGeomPending = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), 2 );
        QSGGeometry::Point2D* verticesPending = sgGeomPending->vertexDataAsPoint2D();

        const QgsPoint& pt1 = points.at( points.length() - 2 );
        const QgsPoint& pt2 = points.at( points.length() - 1 );
        verticesPending[0].set( pt1.x(), pt1.y() );
        verticesPending[1].set( pt2.x(), pt2.y() );

        sgGeomPending->setLineWidth( width );
        nodePending->setMaterial( &mPendingMaterial );
        nodePending->setFlag( QSGNode::OwnedByParent );
        appendChildNode( nodePending );
        qDebug() << "TEST2";
      }
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
