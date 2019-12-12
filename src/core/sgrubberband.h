#ifndef QGSSGRUBBERBAND_H
#define QGSSGRUBBERBAND_H

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

#include <qgspoint.h>
#include <qgswkbtypes.h>
#include "qfieldcore_global.h"

class RubberbandModel;

/**
 * This is used to render a rubberband on the scene graph.
 *
 * This cannot be considered stable API.
 */
class QFIELDCORE_EXPORT SGRubberband : public QSGNode
{
  public:
    SGRubberband( const QVector<QgsPoint> &points, QgsWkbTypes::GeometryType type, const QColor &color, qreal width );

  private:
    QSGGeometryNode *createLineGeometry( const QVector<QgsPoint> &points, qreal width );
    QSGGeometryNode *createPolygonGeometry( const QVector<QgsPoint> &points );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGRUBBERBAND_H
