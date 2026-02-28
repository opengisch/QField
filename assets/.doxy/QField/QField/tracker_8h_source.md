

# File tracker.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**tracker.h**](tracker_8h.md)

[Go to the documentation of this file](tracker_8h.md)


```C++
/***************************************************************************
 tracker.h - Tracker
  ---------------------
 begin                : 20.02.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRACKER_H
#define TRACKER_H

#include "gnsspositioninformation.h"

#include <QColor>
#include <QPointer>
#include <QTimer>
#include <qgsdistancearea.h>
#include <qgspoint.h>
#include <qgsvectorlayer.h>

class FeatureModel;
class QgsQuickCoordinateTransformer;
class RubberbandModel;

class Tracker : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isActive READ isActive NOTIFY isActiveChanged )
    Q_PROPERTY( bool isSuspended READ isSuspended NOTIFY isSuspendedChanged )
    Q_PROPERTY( bool isReplaying READ isReplaying NOTIFY isReplayingChanged )

    Q_PROPERTY( bool visible READ visible WRITE setVisible NOTIFY visibleChanged )
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )

    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )

    Q_PROPERTY( RubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( FeatureModel *featureModel READ featureModel WRITE setFeatureModel NOTIFY featureModelChanged )

    Q_PROPERTY( double timeInterval READ timeInterval WRITE setTimeInterval NOTIFY timeIntervalChanged )
    Q_PROPERTY( double minimumDistance READ minimumDistance WRITE setMinimumDistance NOTIFY minimumDistanceChanged )
    Q_PROPERTY( double maximumDistance READ maximumDistance WRITE setMaximumDistance NOTIFY maximumDistanceChanged )
    Q_PROPERTY( bool sensorCapture READ sensorCapture WRITE setSensorCapture NOTIFY sensorCaptureChanged )
    Q_PROPERTY( bool conjunction READ conjunction WRITE setConjunction NOTIFY conjunctionChanged )
    Q_PROPERTY( MeasureType measureType READ measureType WRITE setMeasureType NOTIFY measureTypeChanged )

    Q_PROPERTY( QDateTime startPositionTimestamp READ startPositionTimestamp WRITE setStartPositionTimestamp NOTIFY startPositionTimestampChanged )

    Q_PROPERTY( bool filterAccuracy READ filterAccuracy WRITE setFilterAccuracy NOTIFY filterAccuracyChanged )

  public:
    enum MeasureType
    {
      SecondsSinceStart = 0,
      Timestamp,
      GroundSpeed,
      Bearing,
      HorizontalAccuracy,
      VerticalAccuracy,
      PDOP,
      HDOP,
      VDOP
    };
    Q_ENUM( MeasureType )

    explicit Tracker( QgsVectorLayer *vectorLayer );

    QColor color() const { return mColor; }
    void setColor( const QColor &color );

    RubberbandModel *rubberbandModel() const;
    void setRubberbandModel( RubberbandModel *rubberbandModel );

    FeatureModel *featureModel() const;
    void setFeatureModel( FeatureModel *featureModel );

    double timeInterval() const { return mTimeInterval; }
    void setTimeInterval( double timeInterval );

    double minimumDistance() const { return mMinimumDistance; }
    void setMinimumDistance( double minimumDistance );

    double maximumDistance() const { return mMaximumDistance; }
    void setMaximumDistance( double maximumDistance );

    bool sensorCapture() const { return mSensorCapture; }
    void setSensorCapture( bool capture );

    bool conjunction() const { return mConjunction; }
    void setConjunction( bool conjunction );

    QDateTime startPositionTimestamp() const { return mStartPositionTimestamp; }
    void setStartPositionTimestamp( const QDateTime &startPositionTimestamp ) { mStartPositionTimestamp = startPositionTimestamp; }

    QgsVectorLayer *vectorLayer() const { return mVectorLayer.data(); }
    void setVectorLayer( QgsVectorLayer *vectorLayer );

    QgsFeature feature() const;
    void setFeature( const QgsFeature &feature );

    bool visible() const { return mVisible; }
    void setVisible( bool visible );

    MeasureType measureType() const { return mMeasureType; }
    void setMeasureType( MeasureType type );

    bool isActive() const { return mIsActive; }

    bool isSuspended() const { return mIsSuspended; }

    bool isReplaying() const { return mIsReplaying; }

    void start( const GnssPositionInformation &positionInformation = GnssPositionInformation(), const QgsPoint &projectedPosition = QgsPoint() );
    void stop();

    Q_INVOKABLE void processPositionInformation( const GnssPositionInformation &positionInformation, const QgsPoint &projectedPosition );

    void replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer = nullptr );

    void suspendUntilReplay();

    bool filterAccuracy() const;

    void setFilterAccuracy( bool enabled );

  signals:
    void isActiveChanged();
    void isSuspendedChanged();
    void isReplayingChanged();
    void visibleChanged();
    void vectorLayerChanged();
    void rubberbandModelChanged();
    void featureModelChanged();

    void timeIntervalChanged();
    void minimumDistanceChanged();
    void maximumDistanceChanged();
    void sensorCaptureChanged();
    void conjunctionChanged();
    void measureTypeChanged();

    void featureCreated();
    void featureChanged();

    void startPositionTimestampChanged();

    void filterAccuracyChanged();
    void colorChanged();

  private slots:
    void positionReceived();
    void sensorDataReceived();
    void rubberbandModelVertexCountChanged();

  private:
    void trackPosition();

    bool mIsActive = false;
    bool mIsSuspended = false;
    bool mIsReplaying = false;

    RubberbandModel *mRubberbandModel = nullptr;
    FeatureModel *mFeatureModel = nullptr;
    qint64 mLastFeatureModelSaveMSSecsSinceEpoch = 0;

    double mTimeInterval = 0.0;
    double mMinimumDistance = 0.0;
    double mMaximumDistance = 0.0;
    int mMaximumDistanceFailuresCount = 0;
    double mCurrentDistance = 0.0;
    bool mSensorCapture = false;
    bool mConjunction = true;
    bool mTimeIntervalFulfilled = false;
    bool mMinimumDistanceFulfilled = false;
    bool mSensorCaptureFulfilled = false;
    bool mSkipPositionReceived = false;
    bool mSkipBadPositionReceived = false;

    QPointer<QgsVectorLayer> mVectorLayer;
    QgsFeature mFeature;

    bool mVisible = true;
    bool mFilterAccuracy = false;

    QColor mColor;

    QDateTime mStartPositionTimestamp;
    qint64 mLastDevicePositionTimestampMSecsSinceEpoch = 0;
    qint64 mLastVertexPositionTimestampMSecsSinceEpoch = 0;

    MeasureType mMeasureType = Tracker::SecondsSinceStart;

    QgsDistanceArea mDa;
};

#endif // TRACKER_H
```


