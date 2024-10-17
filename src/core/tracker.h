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

#include "qgsvectorlayer.h"

#include <QPointer>
#include <QTimer>

class RubberbandModel;

/**
 * \ingroup core
 */
class Tracker : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QDateTime startPositionTimestamp READ startPositionTimestamp WRITE setStartPositionTimestamp NOTIFY startPositionTimestampChanged )

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

    explicit Tracker( QgsVectorLayer *layer );

    RubberbandModel *model() const;
    void setModel( RubberbandModel *model );

    //! Returns the minimum time interval constraint between each tracked point
    double timeInterval() const { return mTimeInterval; }
    //! Sets the minimum time interval constraint between each tracked point
    void setTimeInterval( const double timeInterval ) { mTimeInterval = timeInterval; }

    //! Returns the minimum distance constraint between each tracked point
    double minimumDistance() const { return mMinimumDistance; }
    //! Sets the minimum distance constraint between each tracked point
    void setMinimumDistance( const double minimumDistance ) { mMinimumDistance = minimumDistance; };

    //! Returns the maximum distance tolerated beyond which a position will be considered errenous
    double maximumDistance() const { return mMaximumDistance; }
    //! Sets the maximum distance tolerated beyond which a position will be considered errenous
    void setMaximumDistance( const double maximumDistance ) { mMaximumDistance = maximumDistance; };

    //! Returns if TRUE, newly captured sensor data is needed between each tracked point
    bool sensorCapture() const { return mSensorCapture; }
    //! Sets whether newly captured sensor data is needed between each tracked point
    void setSensorCapture( const bool capture ) { mSensorCapture = capture; }

    //! Returns TRUE if all constraints need to be fulfilled between each tracked point
    bool conjunction() const { return mConjunction; }
    //! Sets where all constraints need to be fulfilled between each tracked point
    void setConjunction( const bool conjunction ) { mConjunction = conjunction; }

    //! Returns the timestamp of the first recorded point
    QDateTime startPositionTimestamp() const { return mStartPositionTimestamp; }
    //! Sets the timestamp of the first recorded point
    void setStartPositionTimestamp( const QDateTime &startPositionTimestamp ) { mStartPositionTimestamp = startPositionTimestamp; }

    //! Returns the current layer
    QgsVectorLayer *layer() const { return mLayer.data(); }
    //! Sets the current layer
    void setLayer( QgsVectorLayer *layer ) { mLayer = layer; }

    //! Returns the created feature
    QgsFeature feature() const;
    //! Sets the created feature
    void setFeature( const QgsFeature &feature );

    //! Returns TRUE if the tracker rubberband is visible
    bool visible() const { return mVisible; }
    //! Sets whether the tracker rubberband is visible
    void setVisible( const bool visible ) { mVisible = visible; }

    //! Returns the measure type used with the tracker geometry's M dimension when available
    MeasureType measureType() const { return mMeasureType; }
    //! Sets the measure type used with the tracker geometry's M dimension when available
    void setMeasureType( MeasureType type ) { mMeasureType = type; }

    //! Returns whether the tracker has been started
    bool isActive() const { return mIsActive; }

    void start();
    void stop();

  signals:
    void startPositionTimestampChanged();
    void isActiveChanged();

  private slots:
    void positionReceived();
    void timeReceived();
    void sensorDataReceived();

  private:
    void trackPosition();

    bool mIsActive = false;

    RubberbandModel *mRubberbandModel = nullptr;

    QTimer mTimer;
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

    QPointer<QgsVectorLayer> mLayer;
    QgsFeature mFeature;

    bool mVisible = true;

    QDateTime mStartPositionTimestamp;

    MeasureType mMeasureType = Tracker::SecondsSinceStart;
};

#endif // TRACKER_H
