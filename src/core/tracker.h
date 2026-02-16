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

/**
 * \ingroup core
 */
class Tracker : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool isActive READ isActive NOTIFY isActiveChanged )
    Q_PROPERTY( bool isSuspended READ isSuspended NOTIFY isSuspendedChanged )
    Q_PROPERTY( bool isReplaying READ isReplaying NOTIFY isReplayingChanged )

    Q_PROPERTY( bool visible READ visible WRITE setVisible NOTIFY visibleChanged )

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

    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )

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

    //! Returns the track rubberband color
    QColor color() const { return mColor; }
    //! Sets the track rubberband color
    void setColor( const QColor &color );

    //! Returns the rubber band model used to generate the track geometry
    RubberbandModel *rubberbandModel() const;
    //! Sets the rubber band model used to generate the track geometry
    void setRubberbandModel( RubberbandModel *rubberbandModel );

    //! Returns the feature model used to generate the track attributes
    FeatureModel *featureModel() const;
    //! Sets the feature model used to generate the track attributes
    void setFeatureModel( FeatureModel *featureModel );

    //! Returns the minimum time interval constraint between each tracked point
    double timeInterval() const { return mTimeInterval; }
    //! Sets the minimum time interval constraint between each tracked point
    void setTimeInterval( double timeInterval );

    //! Returns the minimum distance constraint between each tracked point
    double minimumDistance() const { return mMinimumDistance; }
    //! Sets the minimum distance constraint between each tracked point
    void setMinimumDistance( double minimumDistance );

    //! Returns the maximum distance tolerated beyond which a position will be considered errenous
    double maximumDistance() const { return mMaximumDistance; }
    //! Sets the maximum distance tolerated beyond which a position will be considered errenous
    void setMaximumDistance( double maximumDistance );

    //! Returns if TRUE, newly captured sensor data is needed between each tracked point
    bool sensorCapture() const { return mSensorCapture; }
    //! Sets whether newly captured sensor data is needed between each tracked point
    void setSensorCapture( bool capture );

    //! Returns TRUE if all constraints need to be fulfilled between each tracked point
    bool conjunction() const { return mConjunction; }
    //! Sets where all constraints need to be fulfilled between each tracked point
    void setConjunction( bool conjunction );

    //! Returns the timestamp of the first recorded point
    QDateTime startPositionTimestamp() const { return mStartPositionTimestamp; }
    //! Sets the timestamp of the first recorded point
    void setStartPositionTimestamp( const QDateTime &startPositionTimestamp ) { mStartPositionTimestamp = startPositionTimestamp; }

    //! Returns the current layer
    QgsVectorLayer *vectorLayer() const { return mVectorLayer.data(); }
    //! Sets the current layer
    void setVectorLayer( QgsVectorLayer *vectorLayer );

    //! Returns the created feature
    QgsFeature feature() const;
    //! Sets the created feature
    void setFeature( const QgsFeature &feature );

    //! Returns TRUE if the tracker rubberband is visible
    bool visible() const { return mVisible; }
    //! Sets whether the tracker rubberband is visible
    void setVisible( bool visible );

    //! Returns the measure type used with the tracker geometry's M dimension when available
    MeasureType measureType() const { return mMeasureType; }
    //! Sets the measure type used with the tracker geometry's M dimension when available
    void setMeasureType( MeasureType type );

    //! Returns whether the tracker has been started
    bool isActive() const { return mIsActive; }

    //! Returns whether the track has been suspended
    bool isSuspended() const { return mIsSuspended; }

    //! Returns whether the tracker is replaying positions
    bool isReplaying() const { return mIsReplaying; }

    //! Starts tracking
    void start( const GnssPositionInformation &positionInformation = GnssPositionInformation(), const QgsPoint &projectedPosition = QgsPoint() );
    //! Stops tracking
    void stop();

    //! Process the given position information and projected position passed onto the tracker
    Q_INVOKABLE void processPositionInformation( const GnssPositionInformation &positionInformation, const QgsPoint &projectedPosition );

    //! Replays a list of position information taking into account the tracker settings
    void replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer = nullptr );

    void suspendUntilReplay();

    //! Returns TRUE if GNSS accuracy filtering is enabled
    bool filterAccuracy() const;

    //! Sets whether GNSS accuracy filtering is enabled
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
