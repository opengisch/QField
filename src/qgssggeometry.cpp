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
          geomNode->setGeometry( qgsPolygonToQSGGeometry( polygon ) );
          geomNode->setFlag( QSGNode::OwnsGeometry );
          applyStyle( geomNode );
          appendChildNode( geomNode );
        }
      }
      else
      {
        QSGGeometryNode* geomNode = new QSGGeometryNode;
        geomNode->setGeometry( qgsPolygonToQSGGeometry( gg.asPolygon() ) );
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

QSGGeometry* QgsSGGeometry::qgsPolygonToQSGGeometry( const QgsPolygon& polygon )
{
  QgsPolygon::ConstIterator it = polygon.constBegin();

  const QgsPolyline& ring = *it;
  it++;

  std::vector<p2t::Point*> boundary;
  toPoly2Tri( ring, boundary );
  p2t::CDT cdt( boundary );
  cdt.Triangulate();

  std::vector<p2t::Triangle*> triangles = cdt.GetTriangles();

  QSGGeometry* sgGeom = new QSGGeometry( QSGGeometry::defaultAttributes_Point2D(), triangles.size() * 3 );

  int i = 0; // Index of the current vertex
  for( std::vector<p2t::Triangle*>::const_iterator tit = triangles.begin(); tit != triangles.end(); ++tit )
  {
    p2t::Triangle* p2tTri = *tit;

    QSGGeometry::Point2D* vertices = sgGeom->vertexDataAsPoint2D();

    for ( int j = 0; j < 3; j++ )
    {
      p2t::Point* p = p2tTri->GetPoint( j );
      vertices[i].set( p->x, p->y );
      i++;
    }

    sgGeom->setDrawingMode( GL_TRIANGLES );
  }

  return sgGeom;
}
