#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include "3rdparty/poly2tri/poly2tri.h"

#include "qgsgeometry.h"


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
