/***************************************************************************
    qgssggeometry.h
    ---------------------
    begin                : March 2020
    copyright            : (C) 2020 by David Signer
    email                : david at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSSGGEOMETRY_H
#define QGSSGGEOMETRY_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

#include "qgsgeometry.h"

class QgsSGGeometry : public QSGNode
{
  public:
    QgsSGGeometry();
    QgsSGGeometry( const QgsGeometry &geom, const QColor &color, int width, const QgsRectangle visibleExtent, double scaleFactor );

  private:
    void applyStyle( QSGGeometryNode *geomNode );

    static QSGGeometry *qgsPolylineToQSGGeometry( const QgsPolylineXY &line, int width, const QgsRectangle visibleExtent, double scaleFactor );
    static QSGGeometry *qgsPolygonToQSGGeometry( const QgsPolygonXY &polygon, const QgsRectangle visibleExtent, double scaleFactor );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGGEOMETRY_H
