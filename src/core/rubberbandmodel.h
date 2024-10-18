/***************************************************************************
  rubberbandmodel.h - RubberbandModel

 ---------------------
 begin                : 10.6.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef RUBBERBANDMODEL_H
#define RUBBERBANDMODEL_H

#include "qfield_core_export.h"

#include <QDateTime>
#include <QObject>
#include <QPointF>
#include <QPointer>
#include <QVector>
#include <qgis.h>
#include <qgsabstractgeometry.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsgeometry.h>
#include <qgspoint.h>

class QgsVectorLayer;

/**
 * This model manages a list of vertices. It can be used as a linestring or
 * as a ring in a polygon.
 * \ingroup core
 */

class QFIELD_CORE_EXPORT RubberbandModel : public QObject
{
    Q_OBJECT

    //! First vertex point
    Q_PROPERTY( QgsPoint firstCoordinate READ firstCoordinate NOTIFY currentCoordinateChanged )
    //! Last vertex point
    Q_PROPERTY( QgsPoint lastCoordinate READ lastCoordinate NOTIFY currentCoordinateChanged )
    //! Penultimate (last but one) vertex point
    Q_PROPERTY( QgsPoint penultimateCoordinate READ penultimateCoordinate NOTIFY currentCoordinateChanged )
    //! Current vertex point
    Q_PROPERTY( QgsPoint currentCoordinate READ currentCoordinate WRITE setCurrentCoordinate NOTIFY currentCoordinateChanged )
    //! Current vertex index
    Q_PROPERTY( int currentCoordinateIndex READ currentCoordinateIndex WRITE setCurrentCoordinateIndex NOTIFY currentCoordinateIndexChanged )
    //! Geometry type of the model
    Q_PROPERTY( Qgis::GeometryType geometryType READ geometryType WRITE setGeometryType NOTIFY geometryTypeChanged )
    //! The vector layer the model is for
    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )
    //! Total count of vertices
    Q_PROPERTY( int vertexCount READ vertexCount NOTIFY vertexCountChanged )
    //! All vertex points
    Q_PROPERTY( QVector<QgsPoint> vertices READ vertices NOTIFY currentCoordinateChanged )
    //! Coordinate reference system
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    //! Freeze the rubberband so it doesn't get modified while panning map
    Q_PROPERTY( bool frozen READ frozen WRITE setFrozen NOTIFY frozenChanged )
    //! currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself
    Q_PROPERTY( QDateTime currentPositionTimestamp READ currentPositionTimestamp WRITE setCurrentPositionTimestamp NOTIFY currentPositionTimestampChanged )
    //! measureValue defines the M value of the coordinates
    Q_PROPERTY( double measureValue READ measureValue WRITE setMeasureValue NOTIFY measureValueChanged )

  public:
    explicit RubberbandModel( QObject *parent = nullptr );

    //! \copydoc vertexCount
    int vertexCount() const;

    //! \copydoc vertices
    QVector<QgsPoint> vertices() const;

    //! \copydoc currentCoordinateIndex
    int currentCoordinateIndex() const;

    //! \copydoc currentCoordinateIndex
    void setCurrentCoordinateIndex( int currentCoordinateIndex );

    //! \copydoc firstCoordinate
    QgsPoint firstCoordinate() const;

    //! \copydoc lastCoordinate
    QgsPoint lastCoordinate() const;

    //! \copydoc penultimateCoordinate
    QgsPoint penultimateCoordinate() const;

    //! \copydoc currentCoordinate
    QgsPoint currentCoordinate() const;

    //! \copydoc currentCoordinate
    void setCurrentCoordinate( const QgsPoint &currentCoordinate );

    //! \copydoc currentPositionTimestamp
    QDateTime currentPositionTimestamp() const;

    //! \copydoc currentPositionTimestamp
    void setCurrentPositionTimestamp( const QDateTime &currentPositionTimestamp );

    //! \copydoc measureValue
    double measureValue() const;

    //! \copydoc measureValue
    void setMeasureValue( const double measureValue );

    //! \copydoc geometryType
    Qgis::GeometryType geometryType() const;

    //! \copydoc geometryType
    void setGeometryType( const Qgis::GeometryType &geometryType );

    //! \copydoc vectorLayer
    QgsVectorLayer *vectorLayer() const;

    //! \copydoc vectorLayer
    void setVectorLayer( QgsVectorLayer *vectorLayer );

    //! \copydoc crs
    QgsCoordinateReferenceSystem crs() const;

    //! \copydoc crs
    void setCrs( const QgsCoordinateReferenceSystem &crs );

    //! \copydoc frozen
    bool frozen() const;

    //! \copydoc frozen
    void setFrozen( const bool &frozen );

    //! Returns a copy of all vertice points in the model, with the option to skip the current vertex
    QVector<QgsPoint> verticesCopy( bool skipCurrentPoint = false ) const;

    /**
     * The target CRS into which points should be reprojected.
     * To retrieve unprojected points pass an invalid QgsCoordinateReferenceSystem object.
     *
     * By default coordinates will be returned unprojected.
     */
    Q_INVOKABLE QgsPointSequence pointSequence( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType = Qgis::WkbType::Point, bool closeLine = false ) const;

    //! Add vertex from the current position
    Q_INVOKABLE void addVertex();

    //! Add vertex with a given point
    Q_INVOKABLE void addVertexFromPoint( const QgsPoint &point );

    //! Remove the vertex at the current index
    Q_INVOKABLE void removeVertex();

    //! Reset the model, remove all vertices and restart the vertex index
    Q_INVOKABLE void reset();

    /**
     * Sets the model data to match a given \a geometry
     * \note rings and multiparts are discarded
     */
    void setDataFromGeometry( QgsGeometry geometry, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    //! Returns whether there is at least one vertex in the model
    bool isEmpty() const;

    //! Sets a vertex point at given index
    void setVertex( int index, QgsPoint coordinate );

    //! Inserts vertices
    void insertVertices( int index, int count );

    //! Remove \a count vertices starting at \a index
    void removeVertices( int index, int count );

    //! Returns the current vertex point transformed to give \a crs
    QgsPoint currentPoint( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType = Qgis::WkbType::PointZ ) const;

    //! Returns all vertices points keeping only X and Y coordinates
    QVector<QgsPointXY> flatPointSequence( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() ) const;

  signals:
    void vertexChanged( int index );
    void verticesInserted( int index, int count );
    void verticesRemoved( int index, int count );
    void currentCoordinateIndexChanged();
    void currentCoordinateChanged();
    void geometryTypeChanged();
    void vectorLayerChanged();
    void vertexCountChanged();
    void crsChanged();
    //! \copydoc frozen
    void frozenChanged();
    void currentPositionTimestampChanged();
    void measureValueChanged();

  private:
    QVector<QgsPoint> mPointList;
    int mCurrentCoordinateIndex = 0;
    QDateTime mCurrentPositionTimestamp;
    Qgis::GeometryType mGeometryType = Qgis::GeometryType::Line;
    QPointer<QgsVectorLayer> mLayer;
    QgsCoordinateReferenceSystem mCrs;
    double mMeasureValue = std::numeric_limits<double>::quiet_NaN();
    bool mFrozen = false;
};

#endif // RUBBERBANDMODEL_H
