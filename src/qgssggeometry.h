#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include "qgsgeometry.h"
/*
class QSGSmoothColorMaterial : public QSGMaterial
{
  public:
    QSGSmoothColorMaterial();

    int compare(const QSGMaterial *other) const;

  protected:
    virtual QSGMaterialType *type() const;
     virtual QSGMaterialShader *createShader() const;
};
*/
class QgsSGGeometry : public QSGNode
{
  public:
    QgsSGGeometry();
    QgsSGGeometry( const QgsGeometry& geom , const QColor& color );

  private:
    void applyStyle( QSGGeometryNode* geomNode );

    static QSGGeometry* qgsPolylineToQSGGeometry( const QgsPolyline& line );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGGEOMETRY_H
