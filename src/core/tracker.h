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

    explicit Tracker( QgsVectorLayer *layer, bool visible );

    RubberbandModel *model() const;
    void setModel( RubberbandModel *model );

    //! the (minimum) time interval between setting trackpoints
    double timeInterval() const { return mTimeInterval; }
    //! the (minimum) time interval between setting trackpoints
    void setTimeInterval( const double timeInterval ) { mTimeInterval = timeInterval; }

    //! the minimum distance between setting trackpoints
    double minimumDistance() const { return mMinimumDistance; }
    //! the minimum distance between setting trackpoints
    void setMinimumDistance( const double minimumDistance ) { mMinimumDistance = minimumDistance; };

    //! if both, the minimum distance and the time interval, needs to be fulfilled before setting trackpoints
    bool conjunction() const { return mConjunction; }
    //! if both, the minimum distance and the time interval, needs to be fulfilled before setting trackpoints
    void setConjunction( const bool conjunction ) { mConjunction = conjunction; }

    //! the timestamp of the first recorded position
    QDateTime startPositionTimestamp() const { return mStartPositionTimestamp; }
    //! the timestamp of the first recorded position
    void setStartPositionTimestamp( const QDateTime &startPositionTimestamp ) { mStartPositionTimestamp = startPositionTimestamp; }

    //! the current layer
    QgsVectorLayer *layer() const { return mLayer.data(); }
    //! the current layer
    void setLayer( QgsVectorLayer *layer ) { mLayer = layer; }
    //! the created feature
    QgsFeature feature() const { return mFeature; }
    //! the created feature
    void setFeature( const QgsFeature &feature ) { mFeature = feature; }
    //! if the layer (and the rubberband ) is visible
    bool visible() const { return mVisible; }
    //! if the layer (and the rubberband ) is visible
    void setVisible( const bool visible ) { mVisible = visible; }

    MeasureType measureType() const { return mMeasureType; }
    void setMeasureType( MeasureType type ) { mMeasureType = type; }

    void start();
    void stop();

  signals:
    void startPositionTimestampChanged();

  private slots:
    void positionReceived();
    void timeReceived();

  private:
    RubberbandModel *mRubberbandModel = nullptr;

    QTimer mTimer;
    double mTimeInterval = 0;
    double mMinimumDistance = 0;
    bool mConjunction = true;
    bool mTimeIntervalFulfilled = false;
    bool mMinimumDistanceFulfilled = false;

    QPointer<QgsVectorLayer> mLayer;
    QgsFeature mFeature;

    bool mVisible = true;

    QDateTime mStartPositionTimestamp;

    MeasureType mMeasureType = Tracker::SecondsSinceStart;

    void trackPosition();
};

#endif // TRACKER_H
