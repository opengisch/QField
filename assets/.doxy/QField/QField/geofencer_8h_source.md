

# File geofencer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**geofencer.h**](geofencer_8h.md)

[Go to the documentation of this file](geofencer_8h.md)


```C++
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

    Q_INVOKABLE void applyProjectSettings( QgsProject *project );

    bool active() const { return mActive; }

    void setActive( bool active );

    Behaviors behavior() const { return mBehavior; }

    void setBehavior( Behaviors behavior );

    QgsPoint position() const { return mPosition; }

    void setPosition( const QgsPoint &position );

    QgsCoordinateReferenceSystem positionCrs() const { return mPositionCrs; }

    void setPositionCrs( const QgsCoordinateReferenceSystem &crs );

    QgsVectorLayer *areasLayer() const { return mAreasLayer.data(); }

    void setAreasLayer( QgsVectorLayer *layer );

    bool isAlerting() const { return mIsAlerting; }

    bool isWithin() const;

    QString isWithinAreaName() const;

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
```


