

# File rubberbandmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**rubberbandmodel.h**](rubberbandmodel_8h.md)

[Go to the documentation of this file](rubberbandmodel_8h.md)


```C++
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

class QFIELD_CORE_EXPORT RubberbandModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsPoint firstCoordinate READ firstCoordinate NOTIFY currentCoordinateChanged )
    Q_PROPERTY( QgsPoint lastCoordinate READ lastCoordinate NOTIFY currentCoordinateChanged )
    Q_PROPERTY( QgsPoint penultimateCoordinate READ penultimateCoordinate NOTIFY currentCoordinateChanged )
    Q_PROPERTY( QgsPoint currentCoordinate READ currentCoordinate WRITE setCurrentCoordinate NOTIFY currentCoordinateChanged )
    Q_PROPERTY( int currentCoordinateIndex READ currentCoordinateIndex WRITE setCurrentCoordinateIndex NOTIFY currentCoordinateIndexChanged )
    Q_PROPERTY( Qgis::GeometryType geometryType READ geometryType WRITE setGeometryType NOTIFY geometryTypeChanged )
    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )
    Q_PROPERTY( int vertexCount READ vertexCount NOTIFY vertexCountChanged )
    Q_PROPERTY( QVector<QgsPoint> vertices READ vertices NOTIFY currentCoordinateChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )
    Q_PROPERTY( bool frozen READ frozen WRITE setFrozen NOTIFY frozenChanged )
    Q_PROPERTY( QDateTime currentPositionTimestamp READ currentPositionTimestamp WRITE setCurrentPositionTimestamp NOTIFY currentPositionTimestampChanged )
    Q_PROPERTY( double measureValue READ measureValue WRITE setMeasureValue NOTIFY measureValueChanged )

  public:
    explicit RubberbandModel( QObject *parent = nullptr );

    int vertexCount() const;

    QVector<QgsPoint> vertices() const;

    int currentCoordinateIndex() const;

    void setCurrentCoordinateIndex( int currentCoordinateIndex );

    QgsPoint firstCoordinate() const;

    QgsPoint lastCoordinate() const;

    QgsPoint penultimateCoordinate() const;

    QgsPoint currentCoordinate() const;

    void setCurrentCoordinate( const QgsPoint &currentCoordinate );

    QDateTime currentPositionTimestamp() const;

    void setCurrentPositionTimestamp( const QDateTime &currentPositionTimestamp );

    double measureValue() const;

    void setMeasureValue( const double measureValue );

    Qgis::GeometryType geometryType() const;

    void setGeometryType( const Qgis::GeometryType &geometryType );

    QgsVectorLayer *vectorLayer() const;

    void setVectorLayer( QgsVectorLayer *vectorLayer );

    QgsCoordinateReferenceSystem crs() const;

    void setCrs( const QgsCoordinateReferenceSystem &crs );

    bool frozen() const;

    void setFrozen( const bool &frozen );

    QVector<QgsPoint> verticesCopy( bool skipCurrentPoint = false ) const;

    Q_INVOKABLE QgsPointSequence pointSequence( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType = Qgis::WkbType::Point, bool closeLine = false ) const;

    Q_INVOKABLE void addVertex();

    Q_INVOKABLE void addVertexFromPoint( const QgsPoint &point );

    Q_INVOKABLE void removeVertex();

    Q_INVOKABLE void reset( bool keepLast = true );

    void setDataFromGeometry( QgsGeometry geometry, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    bool isEmpty() const;

    void setVertex( int index, QgsPoint coordinate );

    void insertVertices( int index, int count );

    void removeVertices( int index, int count, bool keepLast = true );

    QgsPoint currentPoint( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType = Qgis::WkbType::PointZ ) const;

    QVector<QgsPointXY> flatPointSequence( const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() ) const;

    QgsPoint vertexAt( int index, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() ) const;

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
```


