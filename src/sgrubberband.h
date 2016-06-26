#ifndef QGSSGRUBBERBAND_H
#define QGSSGRUBBERBAND_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include <qgspoint.h>
#include <qgis.h>


class RubberbandModel;

/**
 * This is used to render a rubberband on the scene graph.
 *
 * This cannot be considered stable API.
 */

class SGRubberband : public QSGGeometryNode
{
  public:
    SGRubberband( const QVector<QgsPoint>& points, QGis::GeometryType type );

  private:
    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGRUBBERBAND_H
