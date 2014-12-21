#include "qgssggeometry.h"

QgsSGGeometry::QgsSGGeometry()
{
}

QgsSGGeometry::QgsSGGeometry( const QgsGeometry& geom, const QColor& color , int width )
{
  //TODO: Fix const-correcteness upstream
  QgsGeometry& gg = const_cast<QgsGeometry&>( geom );

  mMaterial.setColor( color );

  switch ( gg.type() )
  {
    case QGis::Point:
      if ( gg.isMultipart() )
      {
        const QgsMultiPolyline& lines = gg.asMultiPolyline();

        Q_FOREACH( const QgsPolyline& line, lines )
        {
          QSGGeometryNode* geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolylineToQSGGeometry ( line, width ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }
      }
      else
      {
        QSGGeometryNode* geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolylineToQSGGeometry ( gg.asPolyline(), width ) );
        geomNode->setFlag( QSGNode::OwnsGeometry );
        applyStyle( geomNode );
        appendChildNode( geomNode );
      }

      break;

    case QGis::Line:
      if ( gg.isMultipart() )
      {
        const QgsMultiPolyline& lines = gg.asMultiPolyline();

        Q_FOREACH( const QgsPolyline& line, lines )
        {
          QSGGeometryNode* geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolylineToQSGGeometry ( line, width ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }
      }
      else
      {
        QSGGeometryNode* geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolylineToQSGGeometry ( gg.asPolyline(), width ) );
        geomNode->setFlag( QSGNode::OwnsGeometry );
        applyStyle( geomNode );
        appendChildNode( geomNode );
      }
      break;

    case QGis::Polygon:
      if ( gg.isMultipart() )
      {
        const QgsMultiPolygon& polygons = gg.asMultiPolygon();

        Q_FOREACH( const QgsPolygon& polygon, polygons )
        {
          QSGGeometryNode* geomNode = new QSGGeometryNode;
          geomNode->setGeometry( qgsPolylineToQSGGeometry ( polygon.first(), width ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }
      }
      else
      {
        QSGGeometryNode* geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolylineToQSGGeometry ( gg.asPolygon().first(), width ) );
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

void QgsSGGeometry::applyStyle( QSGGeometryNode* geomNode )
{
  geomNode->setMaterial( &mMaterial );
}

QSGGeometry* QgsSGGeometry::qgsPolylineToQSGGeometry( const QgsPolyline& line , int width )
{
  QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), line.count() );
  QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

  int i = 0;
  Q_FOREACH( const QgsPoint& pt, line )
  {
    vertices[i].set( pt.x(), pt.y() );
    i++;
  }

  sgGeom->setLineWidth( width );
  sgGeom->setDrawingMode( GL_LINE_STRIP );

  return sgGeom;
}
