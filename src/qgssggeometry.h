#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

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
    static QSGGeometry* qgsPointToQSGGeometry( const QgsPointXY& point, int width );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGGEOMETRY_H
