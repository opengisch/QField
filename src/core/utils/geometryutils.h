/***************************************************************************
  geometryutils.h - GeometryUtils

 ---------------------
 begin                : 05.03.2020
 copyright            : (C) 2020 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

#include <QObject>

#include <qgsgeometry.h>
#include <qgsfeature.h>

class QgsVectorLayer;
class RubberbandModel;


class GeometryUtils : public QObject
{
  Q_OBJECT
public:
  explicit GeometryUtils(QObject *parent = nullptr);

  static Q_INVOKABLE QgsGeometry polygonFromRubberband(RubberbandModel *rubberBandModel , const QgsCoordinateReferenceSystem &crs);

  static Q_INVOKABLE QgsGeometry::OperationResult addRingFromRubberBand(QgsVectorLayer* layer, QgsFeatureId fid, RubberbandModel* rubberBandModel );

};

#endif // GEOMETRYUTILS_H
