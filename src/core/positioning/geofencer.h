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
 * \ingroup core
 */
class Geofencer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( Behaviors behavior READ behavior WRITE setBehavior NOTIFY behaviorChanged )

    Q_PROPERTY( QgsPoint position READ position WRITE setPosition NOTIFY positionChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem positionCrs READ positionCrs WRITE setPositionCrs NOTIFY positionCrsChanged )

    Q_PROPERTY( QgsVectorLayer *areasLayer READ areasLayer WRITE setAreasLayer NOTIFY areasLayerChanged )

    Q_PROPERTY( bool isAlerting READ isAlerting NOTIFY isAlertingChanged )

    Q_PROPERTY( bool isWithin READ isWithin NOTIFY isWithinChanged );
    Q_PROPERTY( QString isWithinAreaName READ isWithinAreaName NOTIFY isWithinChanged );
    Q_PROPERTY( QString lastWithinAreaName READ lastWithinAreaName NOTIFY isWithinChanged );

  public:
    enum Behaviors
    {
      AlertWhenInsideGeofencedArea = 1,       //<! Alarm will be raised when the position is within an area
      AlertWhenOutsideGeofencedArea,          //<! Alarm will be raised when the position falls outside all areas
      InformWhenEnteringLeavingGeofencedArea, //<! No alarm will be raise but information on areas being entered and departed raised
    };
    Q_ENUM( Behaviors )

    explicit Geofencer( QObject *parent = nullptr );
    virtual ~Geofencer();

    /**
     * Sets the polygon layer holding areas from a given \a project.
     */
    Q_INVOKABLE void applyProjectSettings( QgsProject *project );

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
     * Returns thegeofencing behavior.
     */
    Behaviors behavior() const { return mBehavior; }

    /**
     * Sets the geofencing behavior.
     */
    void setBehavior( Behaviors behavior );

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
     * Returns the polygon layer holding areas.
     */
    QgsVectorLayer *areasLayer() const { return mAreasLayer.data(); }

    /**
     * Sets the polygon layer holding areas.
     */
    void setAreasLayer( QgsVectorLayer *layer );

    /**
     * Returns TRUE when the geofencer's behavior is triggering an alarm.
     */
    bool isAlerting() const { return mIsAlerting; }

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
    void behaviorChanged();
    void invertLogicChanged();
    void positionChanged();
    void positionCrsChanged();
    void areasLayerChanged();
    void isAlertingChanged();
    void isWithinChanged();
    void projectChanged();

  private:
    void cleanupGatherer();
    void gatherAreas();
    void processAreas();

    void checkWithin();
    void checkAlert();

    bool mActive = false;
    Behaviors mBehavior = AlertWhenInsideGeofencedArea;

    QgsPoint mPosition;
    QgsCoordinateReferenceSystem mPositionCrs;

    QPointer<QgsVectorLayer> mAreasLayer;
    QList<FeatureExpressionValuesGatherer::Entry> mAreas;

    bool mIsAlerting = false;

    int mIsWithinIndex = -1;
    int mLastWithinIndex = -1;

    FeatureExpressionValuesGatherer *mGatherer = nullptr;
};

#endif // GEOFENCER_H
