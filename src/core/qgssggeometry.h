#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include "qgsgeometry.h"

class QgsSGGeometry : public QSGNode
{
  public:
    QgsSGGeometry();
    QgsSGGeometry( const QgsGeometry &geom, const QColor &color, int width );

  private:
    void applyStyle( QSGGeometryNode *geomNode );

    static QSGGeometry *qgsPolylineToQSGGeometry( const QgsPolylineXY &line, int width );
    static QSGGeometry *qgsPolygonToQSGGeometry( const QgsPolygonXY &polygon );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGGEOMETRY_H
