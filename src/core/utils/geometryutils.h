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
#include <qgsfeature.h>
#include <qgsgeometry.h>

class QgsVectorLayer;
class RubberbandModel;


class GeometryUtils : public QObject
{
  Q_OBJECT
public:
  explicit GeometryUtils( QObject *parent = nullptr );

  //! Returns a QgsGeometry with a polygon by using the point sequence in the rubberband model.
  static Q_INVOKABLE QgsGeometry polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs );

  //! Add a ring to a polyon with given \a fid using the ring in the rubberband model.
  static Q_INVOKABLE QgsGeometry::OperationResult addRingFromRubberBand( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

  //! This will perform a split using the line in the rubberband model. It works with the layer selection if some features are selected.
  static Q_INVOKABLE QgsGeometry::OperationResult splitFeatureFromRubberBand( QgsVectorLayer *layer, RubberbandModel *rubberBandModel );
};

#endif // GEOMETRYUTILS_H
