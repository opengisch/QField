

# File qfgeofencer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfgeofencer.h**](qfgeofencer_8h.md)

[Go to the documentation of this file](qfgeofencer_8h.md)


```C++
/***************************************************************************
  qfgeofencer.h - QfGeofencer

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

#ifndef QFGEOFENCER_H
#define QFGEOFENCER_H

#include "qffeatureexpressionvaluesgatherer.h"

#include <QObject>
#include <QSoundEffect>
#include <QTimer>
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>
#include <qgsvectorlayer.h>

#ifdef WITH_TEXTTOSPEECH
#include <QTextToSpeech>
#endif

class QfGeofencer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( Behaviors behavior READ behavior WRITE setBehavior NOTIFY behaviorChanged )
    Q_PROPERTY( bool vibration READ vibration WRITE setVibration NOTIFY vibrationChanged )
    Q_PROPERTY( AudioTypes audio READ audio WRITE setAudio NOTIFY audioChanged )

    Q_PROPERTY( QgsPoint position READ position WRITE setPosition NOTIFY positionChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem positionCrs READ positionCrs WRITE setPositionCrs NOTIFY positionCrsChanged )

    Q_PROPERTY( QgsVectorLayer *areasLayer READ areasLayer WRITE setAreasLayer NOTIFY areasLayerChanged )
    Q_PROPERTY( QString areasFilterExpression READ areasFilterExpression WRITE setAreasFilterExpression NOTIFY areasFilterExpressionChanged )

    Q_PROPERTY( bool isAlerting READ isAlerting NOTIFY isAlertingChanged )

    Q_PROPERTY( bool isWithin READ isWithin NOTIFY isWithinChanged );
    Q_PROPERTY( QString isWithinAreaName READ isWithinAreaName NOTIFY isWithinChanged );
    Q_PROPERTY( QString lastWithinAreaName READ lastWithinAreaName NOTIFY isWithinChanged );
    Q_PROPERTY( QString lastFeedback READ lastFeedback NOTIFY lastFeedbackChanged );

  public:
    enum Behaviors
    {
      AlertWhenInsideGeofencedArea = 1,       //<! Alarm will be raised when the position is within an area
      AlertWhenOutsideGeofencedArea,          //<! Alarm will be raised when the position falls outside all areas
      InformWhenEnteringLeavingGeofencedArea, //<! No alarm will be raise but information on areas being entered and departed raised
    };
    Q_ENUM( Behaviors )

    enum AudioTypes
    {
      AudioMuted = 1,
      AudioAlarm = 2,
      AudioSpeech = 3,
    };
    Q_ENUM( AudioTypes )

    explicit QfGeofencer( QObject *parent = nullptr );
    virtual ~QfGeofencer();

    Q_INVOKABLE void applyProjectSettings( QgsProject *project );

    bool active() const { return mActive; }

    void setActive( bool active );

    Behaviors behavior() const { return mBehavior; }

    void setBehavior( Behaviors behavior );

    bool vibration() const { return mVibration; }

    void setVibration( bool vibration );

    AudioTypes audio() const { return mAudio; }

    void setAudio( AudioTypes audio );

    QgsPoint position() const { return mPosition; }

    void setPosition( const QgsPoint &position );

    QgsCoordinateReferenceSystem positionCrs() const { return mPositionCrs; }

    void setPositionCrs( const QgsCoordinateReferenceSystem &crs );

    QgsVectorLayer *areasLayer() const { return mAreasLayer.data(); }

    void setAreasLayer( QgsVectorLayer *layer );

    QString areasFilterExpression() const { return mAreasFilterExpression; }

    void setAreasFilterExpression( const QString &areasFilterExpression );

    bool isAlerting() const { return mIsAlerting; }

    bool isWithin() const;

    QString isWithinAreaName() const;

    QString lastWithinAreaName() const;

    QString lastFeedback() const { return mLastFeedback; }

  signals:

    void activeChanged();
    void behaviorChanged();
    void vibrationChanged();
    void audioChanged();
    void invertLogicChanged();
    void positionChanged();
    void positionCrsChanged();
    void areasLayerChanged();
    void areasFilterExpressionChanged();
    void isAlertingChanged();
    void isWithinChanged();
    void lastFeedbackChanged();
    void projectChanged();

  private:
    void cleanupGatherer();
    void gatherAreas();
    void processAreas();

    void checkWithin( bool forceProcess = false );
    void processWithinChanged();

    bool mActive = false;
    Behaviors mBehavior = AlertWhenInsideGeofencedArea;
    bool mVibration = false;
    AudioTypes mAudio = AudioMuted;

    QgsPoint mPosition;
    QgsCoordinateReferenceSystem mPositionCrs;

    QPointer<QgsVectorLayer> mAreasLayer;
    QString mAreasFilterExpression;
    QList<QfFeatureExpressionValuesGatherer::Entry> mAreas;

    bool mIsAlerting = false;

    int mIsWithinIndex = -1;
    int mLastWithinIndex = -1;
    QString mLastFeedback;

    QfFeatureExpressionValuesGatherer *mGatherer = nullptr;

#ifdef WITH_TEXTTOSPEECH
    std::unique_ptr<QTextToSpeech> mTextToSpeech;
#endif
    std::unique_ptr<QSoundEffect> mAudioAlarmSound;
};

#endif // QFGEOFENCER_H
```


