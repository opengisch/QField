/***************************************************************************
 navigation.h - Navigation

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "navigationmodel.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <QSoundEffect>
#include <QTimer>
#include <qgsdistancearea.h>

class Navigation : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( NavigationModel *model READ model() NOTIFY modelChanged )

    Q_PROPERTY( QgsPoint location READ location WRITE setLocation NOTIFY locationChanged )
    Q_PROPERTY( QgsPoint destination READ destination WRITE setDestination NOTIFY destinationChanged )
    Q_PROPERTY( QString destinationName READ destinationName NOTIFY destinationNameChanged )

    Q_PROPERTY( QgsGeometry path READ path NOTIFY detailsChanged )
    Q_PROPERTY( double distance READ distance NOTIFY detailsChanged )
    Q_PROPERTY( double verticalDistance READ verticalDistance NOTIFY detailsChanged )
    Q_PROPERTY( QgsUnitTypes::DistanceUnit distanceUnits READ distanceUnits NOTIFY detailsChanged )
    Q_PROPERTY( double bearing READ bearing NOTIFY detailsChanged )

    Q_PROPERTY( int destinationFeatureCurrentVertex READ destinationFeatureCurrentVertex NOTIFY destinationFeatureCurrentVertexChanged )
    Q_PROPERTY( int destinationFeatureVertexCount READ destinationFeatureVertexCount NOTIFY destinationFeatureVertexCountChanged )

    Q_PROPERTY( bool proximityAlarm READ proximityAlarm WRITE setProximityAlarm NOTIFY proximityAlarmChanged )
    Q_PROPERTY( double proximityAlarmThreshold READ proximityAlarmThreshold WRITE setProximityAlarmThreshold NOTIFY proximityAlarmThresholdChanged )

    Q_PROPERTY( bool isActive READ isActive NOTIFY isActiveChanged )

  public:
    Navigation();

    ~Navigation();

    /**
     * Returns TRUE when navigation is active.
     */
    bool isActive() const;

    /**
     * Returns the map settings object used to project points and calculate navigation details.
     */
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    /**
     * Sets the map settings object used to project points and calculate navigation details.
     */
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    /**
     * Returns the navigation model containing the destination point.
     * \note In the future, the model could contain intermediary stops
     */
    NavigationModel *model() const { return mModel.get(); }

    /**
     * Returns the current location point.
     */
    QgsPoint location() const;

    /**
     * Sets the current location \a point.
     * \note A null/empty geometry will be threated as a lack of available location information
     */
    void setLocation( const QgsPoint &point );

    /**
     * Returns the current destination point.
     */
    QgsPoint destination() const;

    /**
     * Sets the current destination \a point.
     */
    void setDestination( const QgsPoint &point );

    /**
     * Returns the current destination name.
     * \see setDestinationFeature
     */
    QString destinationName() const;

    /**
     * Sets a provided feature as navigation destination, which allows for users to cycle through the
     * feature centroid and its individual vertices as destination point.
     * \param feature the feature used as destination
     * \param layer the vector layer associated to the feature
     */
    Q_INVOKABLE void setDestinationFeature( const QgsFeature &feature, QgsVectorLayer *layer );

    /**
     * Clears the current destination feature, as well as the current destination point.
     */
    Q_INVOKABLE void clearDestinationFeature();

    /**
     * Sets the destination point to the next vertex or centroid of the current destination feature.
     * \note if a destination feature has not been provided, calling this function does nothing
     */
    Q_INVOKABLE void nextDestinationVertex();

    /**
     * Sets the destination point to the previous vertex or centroid of the current destination feature.
     * \note if a destination feature has not been provided, calling this function does nothing
     */
    Q_INVOKABLE void previousDestinationVertex();

    /**
     * Returns the current focused vertex serving as destination point when a feature is used as destination.
     * \note for line and polygon geometries, vertex '0' represents the centroid of the geometry
     * while it presents the first (or only) vertex in point geometries
     */
    int destinationFeatureCurrentVertex() const;
    /**
     * Returns the vertex count available to cycle through as destination points when a feature
     * is used as destination.
     */
    int destinationFeatureVertexCount() const;

    /**
     * Returns the path between the current location and the destination points
     */
    QgsGeometry path() const { return mPath; }

    /**
     * Returns the 2D distance between the current location and the destination points
     */
    double distance() const { return mDistance; }

    /**
     * Retuens the vertical distance between the current location and the destination points
     * \note if either points miss a Z value, the returned vertical distance value will be NaN
     */
    double verticalDistance() const { return mVerticalDistance; }

    /**
     * Returns the distance unit used
     */
    QgsUnitTypes::DistanceUnit distanceUnits() const { return mDa.lengthUnits(); }

    /**
     * Returns the bearing between the current location and the destination points
     */
    double bearing() const { return mBearing; }

    /**
     * Returns TRUE if the proximity alarm is enabled.
     */
    bool proximityAlarm() const { return mProximityAlarm; }

    /**
     * Sets whether the proximity alarm is \a enabled.
     */
    void setProximityAlarm( const bool enabled );

    /**
     * Returns the current distance threshold below which the proximity alarm will
     * turn on when enabled.
     */
    double proximityAlarmThreshold() const { return mProximityAlarmThreshold; }

    /**
     * Sets the distance \a threshold below which the proximity alarm will
     * turn on when enabled. Once the distance threshold is met, the alarm sound will loop at
     * a varying interval based on the distance between the device's current location and
     * the destination. The closer to the destination, the shorter the interval.
     */
    void setProximityAlarmThreshold( const double &threshold );

    /**
     * Clears any existing destination point and feature.
     */
    Q_INVOKABLE void clear();

  signals:
    void isActiveChanged();

    void mapSettingsChanged();
    void modelChanged();

    void locationChanged();
    void destinationChanged();
    void destinationNameChanged();

    void destinationFeatureCurrentVertexChanged();
    void destinationFeatureVertexCountChanged();

    void detailsChanged();

    void proximityAlarmChanged();
    void proximityAlarmThresholdChanged();

  private slots:
    void crsChanged();

  private:
    void updateDetails();
    void setDestinationFromCurrentVertex();
    void updateProximityAlarmState();

    std::unique_ptr<NavigationModel> mModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsPoint mLocation;
    QgsGeometry mPath;
    QgsDistanceArea mDa;
    double mDistance = std::numeric_limits<double>::quiet_NaN();
    double mVerticalDistance = std::numeric_limits<double>::quiet_NaN();
    double mBearing = std::numeric_limits<double>::quiet_NaN();
    QString mDestinationName;

    QString mFeatureName;
    QgsGeometry mGeometry;
    int mCurrentVertex = -1;
    int mVertexCount = 0;

    bool mProximityAlarm = false;
    double mProximityAlarmThreshold = 0.0;
    QTimer mProximityAlarmTimer;
    int mProximityAlarmInterval;
    qint64 mLastProximityAlarm;
    QSoundEffect mProximitySound;
};

#endif // NAVIGATION_H
