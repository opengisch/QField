

# File geometryutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**geometryutils.h**](geometryutils_8h.md)

[Go to the documentation of this file](geometryutils_8h.md)


```C++
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
      Success = 0,               
      NothingHappened = 1000,    
      InvalidBaseGeometry,       
      InvalidInputGeometryType,  
      SelectionIsEmpty,          
      SelectionIsGreaterThanOne, 
      GeometryEngineError,       
      LayerNotEditable,          
      /* Add part issues */
      AddPartSelectedGeometryNotFound, 
      AddPartNotMultiGeometry,         
      /* Add ring issues*/
      AddRingNotClosed,            
      AddRingNotValid,             
      AddRingCrossesExistingRings, 
      AddRingNotInExistingFeature, 
      /* Split features */
      SplitCannotSplitPoint, 
    };
    Q_ENUM( GeometryOperationResult )

    explicit GeometryUtils( QObject *parent = nullptr );

    static Q_INVOKABLE QgsGeometry polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType = Qgis::WkbType::Unknown );

    static Q_INVOKABLE QgsGeometry lineFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType = Qgis::WkbType::Unknown );

    static Q_INVOKABLE QgsGeometry variableWidthBufferByMFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs );

    static Q_INVOKABLE GeometryOperationResult reshapeFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    static Q_INVOKABLE GeometryOperationResult eraseFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    static Q_INVOKABLE GeometryOperationResult addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    static Q_INVOKABLE GeometryOperationResult splitFeatureFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel );

    static Q_INVOKABLE QgsPoint coordinateToPoint( const QGeoCoordinate &coor );

    static Q_INVOKABLE double distanceBetweenPoints( const QgsPoint &start, const QgsPoint &end );

    static Q_INVOKABLE QgsPoint reprojectPointToWgs84( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs );

    static Q_INVOKABLE QgsPoint reprojectPoint( const QgsPoint &point, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs );

    static Q_INVOKABLE QgsPoint emptyPoint() { return QgsPoint(); }

    static Q_INVOKABLE QgsPoint point( double x, double y, double z = std::numeric_limits<double>::quiet_NaN(), double m = std::numeric_limits<double>::quiet_NaN() ) { return QgsPoint( x, y, z, m ); }

    static Q_INVOKABLE QgsPoint centroid( const QgsGeometry &geometry );

    static Q_INVOKABLE QgsGeometry createGeometryFromWkt( const QString &wkt );

    static Q_INVOKABLE QgsRectangle boundingBox( const QgsGeometry &geometry ) { return geometry.boundingBox(); }

    static Q_INVOKABLE QgsRectangle reprojectRectangle( const QgsRectangle &rectangle, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs );

    static Q_INVOKABLE QgsRectangle createRectangleFromPoints( const QgsPoint &p1, const QgsPoint &p2 );
};

#endif // GEOMETRYUTILS_H
```


