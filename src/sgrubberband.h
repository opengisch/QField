#ifndef QGSSGRUBBERBAND_H
#define QGSSGRUBBERBAND_H

#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>

#include <qgspoint.h>
#include <qgis.h>


class RubberbandModel;

/**
 * This is used to render a rubberband on the scene graph.
 *
 * This cannot be considered stable API.
 */

class SGRubberband : public QSGNode
{
  public:
    SGRubberband( const QVector<QgsPoint>& points, QGis::GeometryType type, bool lastPointPending, const QColor& color, qreal width );

  private:
    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGRUBBERBAND_H
