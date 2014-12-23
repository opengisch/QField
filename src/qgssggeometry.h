#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include "3rdparty/poly2tri/poly2tri.h"

#include "qgsgeometry.h"

static void toPoly2Tri( const QgsPolyline& polygon, std::vector<p2t::Point *>& p2tVector )
{
  // Q_ASSERT(polygon.first() != polygon.last());
  Q_FOREACH( const QgsPoint &point, polygon )
  {
    if ( p2tVector.size() != ( size_t ) ( polygon.size() - 1 ) )
      p2tVector.push_back( new p2t::Point( point.x(), point.y() ) );
  }
}


class QgsSGGeometry : public QSGNode
{
  public:
    QgsSGGeometry();
    QgsSGGeometry( const QgsGeometry& geom , const QColor& color, int width );

  private:
    void applyStyle( QSGGeometryNode* geomNode );

    static QSGGeometry* qgsPolylineToQSGGeometry( const QgsPolyline& line, int width );
    static QSGGeometry* qgsPolygonToQSGGeometry( const QgsPolygon& polygon );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGGEOMETRY_H
