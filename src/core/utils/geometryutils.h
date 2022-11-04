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

#include "qfield_core_export.h"

#include <QObject>
#include <QtPositioning/QGeoCoordinate>
#include <qgis.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeature.h>
#include <qgsgeometry.h>

class QgsVectorLayer;
class RubberbandModel;

class QFIELD_CORE_EXPORT GeometryUtils : public QObject
{
    Q_OBJECT
  public:
    // Copy of the QGIS GeometryOperationResult enum, needed to keep compatibility with QGIS < 3.22
    enum class GeometryOperationResult : int
    {
      Success = 0,               //!< Operation succeeded
      NothingHappened = 1000,    //!< Nothing happened, without any error
      InvalidBaseGeometry,       //!< The base geometry on which the operation is done is invalid or empty
      InvalidInputGeometryType,  //!< The input geometry (ring, part, split line, etc.) has not the correct geometry type
      SelectionIsEmpty,          //!< No features were selected
      SelectionIsGreaterThanOne, //!< More than one features were selected
      GeometryEngineError,       //!< Geometry engine misses a method implemented or an error occurred in the geometry engine
      LayerNotEditable,          //!< Cannot edit layer
      /* Add part issues */
      AddPartSelectedGeometryNotFound, //!< The selected geometry cannot be found
      AddPartNotMultiGeometry,         //!< The source geometry is not multi
      /* Add ring issues*/
      AddRingNotClosed,            //!< The input ring is not closed
      AddRingNotValid,             //!< The input ring is not valid
      AddRingCrossesExistingRings, //!< The input ring crosses existing rings (it is not disjoint)
      AddRingNotInExistingFeature, //!< The input ring doesn't have any existing ring to fit into
      /* Split features */
      SplitCannotSplitPoint, //!< Cannot split points
    };
    Q_ENUM( GeometryOperationResult )

    explicit GeometryUtils( QObject *parent = nullptr );

    //! Returns a QgsGeometry with a polygon by using the point sequence in the rubberband model.
    static Q_INVOKABLE QgsGeometry polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs );

    //! Returns a QgsGeometry with a line by using the point sequence in the rubberband model.
    static Q_INVOKABLE QgsGeometry lineFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs );

    //! Reshape a polyon with given \a fid using the ring in the rubberband model.
    static Q_INVOKABLE GeometryOperationResult reshapeFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    //! Add a ring to a polyon with given \a fid using the ring in the rubberband model.
    static Q_INVOKABLE GeometryOperationResult addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    //! This will perform a split using the line in the rubberband model. It works with the layer selection if some features are selected.
    static Q_INVOKABLE GeometryOperationResult splitFeatureFromRubberband( QgsVectorLayer *layer, RubberbandModel *rubberBandModel );

    //! Converts QGeoCoordinate to QgsPoint.
    static Q_INVOKABLE QgsPoint coordinateToPoint( const QGeoCoordinate &coor );

    //! Returns the distance between a pair of \a start and \a end points.
    static Q_INVOKABLE double distanceBetweenPoints( const QgsPoint &start, const QgsPoint &end );

    //! Returns a reprojected \a point from the stated \a crs to WGS84.
    static Q_INVOKABLE QgsPoint reprojectPointToWgs84( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs );

    //! Returns a reprojected \a point from the stated \a sourceCrs to a \a destinationCrs
    static Q_INVOKABLE QgsPoint reprojectPoint( const QgsPoint &point, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs );

    //! Returns an empty (i.e. null) point
    static Q_INVOKABLE QgsPoint emptyPoint() { return QgsPoint(); }
};

#endif // GEOMETRYUTILS_H
