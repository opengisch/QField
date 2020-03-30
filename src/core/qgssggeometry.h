#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include "qgsgeometry.h"

#include <QtQuick/QSGFlatColorMaterial>
#include <QtQuick/QSGGeometryNode>

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
