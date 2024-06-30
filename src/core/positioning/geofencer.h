/***************************************************************************
  geofencer.h - Geofencer

 ---------------------
 begin                : 27.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GEOFENCER_H
#define GEOFENCER_H

#include "featureexpressionvaluesgatherer.h"

#include <QObject>
#include <QTimer>
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>
#include <qgsvectorlayer.h>

/**
 * This class provides an interface to manage geofencing of areas as well as
 * providing feedback whenever the position trespasses into or out of those
 * areas.
 */
class Geofencer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )

    Q_PROPERTY( QgsPoint position READ position WRITE setPosition NOTIFY positionChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem positionCrs READ positionCrs WRITE setPositionCrs NOTIFY positionCrsChanged )

    Q_PROPERTY( QgsVectorLayer *areasLayer READ areasLayer WRITE setAreasLayer NOTIFY areasLayerChanged )

    Q_PROPERTY( bool isWithin READ isWithin NOTIFY isWithinChanged );
    Q_PROPERTY( QString isWithinAreaName READ isWithinAreaName NOTIFY isWithinChanged );
    Q_PROPERTY( QString lastWithinAreaName READ lastWithinAreaName NOTIFY isWithinChanged );

  public:
    explicit Geofencer( QObject *parent = nullptr );
    virtual ~Geofencer();

    /**
     * Returns TRUE when geofencing is active.
     * \see setActive
     */
    bool active() const { return mActive; }

    /**
     * Sets the geofencing \a active state.
     * \see active
     */
    void setActive( bool active );

    /**
     * Returns the position to be used to check for overlap with areas.
     */
    QgsPoint position() const { return mPosition; }

    /**
     * Sets the \a position to be used to check for overlap with areas.
     */
    void setPosition( const QgsPoint &position );

    /**
     * Returns the position's coordinate reference system (CRS).
     */
    QgsCoordinateReferenceSystem positionCrs() const { return mPositionCrs; }

    /**
     * Sets the position's coordinate reference system (CRS).
     */
    void setPositionCrs( const QgsCoordinateReferenceSystem &crs );

    /**
     * Returns the polygon layer from which areas will be taken from.
     */
    QgsVectorLayer *areasLayer() const { return mAreasLayer.data(); }

    /**
     * Sets the polygon layer from which areas will be taken from
     */
    void setAreasLayer( QgsVectorLayer *layer );

    /**
     * Returns whether the current position is within an area.
     */
    bool isWithin() const;

    /**
     * Returns the area name within which the current position overlaps. If
     * no overlap, an empty string will be returned.
     */
    QString isWithinAreaName() const;

    /**
     * Returns the last area name within which the position had overlapped. If
     * no overlap has yet to occur, an empty string will be returned.
     */
    QString lastWithinAreaName() const;

  signals:

    void activeChanged();
    void invertLogicChanged();
    void positionChanged();
    void positionCrsChanged();
    void areasLayerChanged();
    void isWithinChanged();

  private:
    void cleanupGatherer();
    void gatherAreas();
    void processAreas();

    void checkWithin();

    bool mActive = false;

    QgsPoint mPosition;
    QgsCoordinateReferenceSystem mPositionCrs;

    QPointer<QgsVectorLayer> mAreasLayer;
    QList<FeatureExpressionValuesGatherer::Entry> mAreas;

    int mIsWithinIndex = -1;
    int mLastWithinIndex = -1;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;
};

#endif // GEOFENCER_H
