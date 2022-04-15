/***************************************************************************
    sgrubberband.h
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
#ifndef QGSSGRUBBERBAND_H
#define QGSSGRUBBERBAND_H

#include <QtQuick/QSGFlatColorMaterial>
#include <QtQuick/QSGNode>
#include <qgspoint.h>
#include <qgswkbtypes.h>


class RubberbandModel;

/**
 * This is used to render a rubberband on the scene graph.
 *
 * This cannot be considered stable API.
 */

class SGRubberband : public QSGNode
{
  public:
    SGRubberband( const QVector<QgsPoint> &points, QgsWkbTypes::GeometryType type, const QColor &color, float width );

  private:
    QSGGeometryNode *createLineGeometry( const QVector<QgsPoint> &points, float width );
    QSGGeometryNode *createPolygonGeometry( const QVector<QgsPoint> &points );

    QSGFlatColorMaterial mMaterial;
};

#endif // QGSSGRUBBERBAND_H
