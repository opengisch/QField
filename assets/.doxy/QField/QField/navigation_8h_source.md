

# File navigation.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**navigation.h**](navigation_8h.md)

[Go to the documentation of this file](navigation_8h.md)


```C++
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
    Q_PROPERTY( Qgis::DistanceUnit distanceUnits READ distanceUnits NOTIFY detailsChanged )
    Q_PROPERTY( double bearing READ bearing NOTIFY detailsChanged )

    Q_PROPERTY( int destinationFeatureCurrentVertex READ destinationFeatureCurrentVertex NOTIFY destinationFeatureCurrentVertexChanged )
    Q_PROPERTY( int destinationFeatureVertexCount READ destinationFeatureVertexCount NOTIFY destinationFeatureVertexCountChanged )

    Q_PROPERTY( bool proximityAlarm READ proximityAlarm WRITE setProximityAlarm NOTIFY proximityAlarmChanged )
    Q_PROPERTY( double proximityAlarmThreshold READ proximityAlarmThreshold WRITE setProximityAlarmThreshold NOTIFY proximityAlarmThresholdChanged )

    Q_PROPERTY( bool isActive READ isActive NOTIFY isActiveChanged )

  public:
    Navigation();

    ~Navigation();

    bool isActive() const;

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    NavigationModel *model() const { return mModel.get(); }

    QgsPoint location() const;

    void setLocation( const QgsPoint &point );

    QgsPoint destination() const;

    void setDestination( const QgsPoint &point );

    QString destinationName() const;

    Q_INVOKABLE void setDestinationFeature( const QgsFeature &feature, QgsVectorLayer *layer );

    Q_INVOKABLE void clearDestinationFeature();

    Q_INVOKABLE void nextDestinationVertex();

    Q_INVOKABLE void previousDestinationVertex();

    int destinationFeatureCurrentVertex() const;
    int destinationFeatureVertexCount() const;

    QgsGeometry path() const { return mPath; }

    double distance() const { return mDistance; }

    double verticalDistance() const { return mVerticalDistance; }

    Qgis::DistanceUnit distanceUnits() const { return mDa.lengthUnits(); }

    double bearing() const { return mBearing; }

    bool proximityAlarm() const { return mProximityAlarm; }

    void setProximityAlarm( const bool enabled );

    double proximityAlarmThreshold() const { return mProximityAlarmThreshold; }

    void setProximityAlarmThreshold( const double &threshold );

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
```


