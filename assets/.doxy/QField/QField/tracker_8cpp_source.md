

# File tracker.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**tracker.cpp**](tracker_8cpp.md)

[Go to the documentation of this file](tracker_8cpp.md)


```C++
/***************************************************************************
 tracker.cpp - Tracker
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

#include "featuremodel.h"
#include "qgsquickcoordinatetransformer.h"
#include "rubberbandmodel.h"
#include "tracker.h"

#include <QRandomGenerator>
#include <qgsproject.h>
#include <qgssensormanager.h>

#define MAXIMUM_DISTANCE_FAILURES 20

Tracker::Tracker( QgsVectorLayer *vectorLayer )
  : mVectorLayer( vectorLayer )
{
  QRandomGenerator *rng = QRandomGenerator::global();
  mColor = QColor::fromRgbF( std::min( 0.75, rng->generateDouble() ), std::min( 0.75, rng->generateDouble() ), std::min( 0.75, rng->generateDouble() ), 0.6 );

  mDa.setEllipsoid( QgsProject::instance()->ellipsoid() );
  mDa.setSourceCrs( QgsProject::instance()->crs(), QgsProject::instance()->transformContext() );
}

void Tracker::setColor( const QColor &color )
{
  if ( mColor == color )
  {
    return;
  }

  mColor = color;
  emit colorChanged();
}

void Tracker::setVisible( bool visible )
{
  if ( mVisible == visible )
    return;

  mVisible = visible;
  emit visibleChanged();
}

void Tracker::setVectorLayer( QgsVectorLayer *vectorLayer )
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}

RubberbandModel *Tracker::rubberbandModel() const
{
  return mRubberbandModel;
}

void Tracker::setRubberbandModel( RubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Tracker::rubberbandModelVertexCountChanged );
  }

  mRubberbandModel = rubberbandModel;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Tracker::rubberbandModelVertexCountChanged );
  }

  emit rubberbandModelChanged();
}

FeatureModel *Tracker::featureModel() const
{
  return mFeatureModel;
}

void Tracker::setFeatureModel( FeatureModel *featureModel )
{
  if ( mFeatureModel == featureModel )
    return;

  mFeatureModel = featureModel;
  emit featureModelChanged();
}

QgsFeature Tracker::feature() const
{
  return mFeature;
}

void Tracker::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  emit featureChanged();
}

void Tracker::setTimeInterval( double timeInterval )
{
  if ( mTimeInterval == timeInterval )
    return;

  mTimeInterval = timeInterval;
  emit timeIntervalChanged();
}

void Tracker::setMinimumDistance( double minimumDistance )
{
  if ( mMinimumDistance == minimumDistance )
    return;

  mMinimumDistance = minimumDistance;
  emit minimumDistanceChanged();
}

void Tracker::setMaximumDistance( double maximumDistance )
{
  if ( mMaximumDistance == maximumDistance )
    return;

  mMaximumDistance = maximumDistance;
  emit maximumDistanceChanged();
}

void Tracker::setSensorCapture( bool capture )
{
  if ( mSensorCapture == capture )
    return;

  mSensorCapture = capture;
  emit sensorCaptureChanged();
}

void Tracker::setConjunction( bool conjunction )
{
  if ( mConjunction == conjunction )
    return;

  mConjunction = conjunction;
  emit conjunctionChanged();
}

void Tracker::setMeasureType( MeasureType type )
{
  if ( mMeasureType == type )
    return;

  mMeasureType = type;
  emit measureTypeChanged();
}

void Tracker::trackPosition()
{
  if ( !mRubberbandModel || std::isnan( mRubberbandModel->currentCoordinate().x() ) || std::isnan( mRubberbandModel->currentCoordinate().y() ) )
  {
    return;
  }

  if ( mRubberbandModel->vertexCount() > 1 && !qgsDoubleNear( mMaximumDistance, 0.0 ) && mCurrentDistance > mMaximumDistance )
  {
    // Simple logic to avoid getting stuck in an infinite erroneous distance having somehow actually moved beyond the safeguard threshold
    if ( ++mMaximumDistanceFailuresCount < MAXIMUM_DISTANCE_FAILURES )
    {
      return;
    }
  }

  mSkipPositionReceived = true;
  mRubberbandModel->addVertex();

  mLastVertexPositionTimestampMSecsSinceEpoch = mLastDevicePositionTimestampMSecsSinceEpoch;
  mMaximumDistanceFailuresCount = 0;
  mCurrentDistance = 0.0;
  mTimeIntervalFulfilled = qgsDoubleNear( mTimeInterval, 0.0 );
  mMinimumDistanceFulfilled = qgsDoubleNear( mMinimumDistance, 0.0 );
  mSensorCaptureFulfilled = !mSensorCapture;
}

void Tracker::positionReceived()
{
  if ( mSkipPositionReceived )
  {
    // When calling mRubberbandModel->addVertex(), the signal we listen to for new position received is triggered, skip that one
    mSkipPositionReceived = false;
    return;
  }

  if ( mSkipBadPositionReceived )
  {
    // Occurs when filterAccuracy property is true and the received position accuracy quality was determined to be bad
    mSkipBadPositionReceived = false;
    return;
  }

  if ( !qgsDoubleNear( mTimeInterval, 0.0 ) )
  {
    mTimeIntervalFulfilled = mRubberbandModel->vertexCount() == 1 || ( ( mLastDevicePositionTimestampMSecsSinceEpoch - mLastVertexPositionTimestampMSecsSinceEpoch ) >= mTimeInterval * 1000 );

    if ( !mConjunction && mTimeIntervalFulfilled )
    {
      trackPosition();
      return;
    }
  }

  if ( mRubberbandModel->vertexCount() > 1 && ( !qgsDoubleNear( mMinimumDistance, 0.0 ) || !qgsDoubleNear( mMaximumDistance, 0.0 ) ) )
  {
    const QgsPoint lastVertex = mRubberbandModel->vertexAt( mRubberbandModel->vertexCount() - 1, QgsProject::instance()->crs() );
    const QgsPoint vertexBeforeLast = mRubberbandModel->vertexAt( mRubberbandModel->vertexCount() - 2, QgsProject::instance()->crs() );

    try
    {
      mCurrentDistance = mDa.measureLine( lastVertex, vertexBeforeLast );
    }
    catch ( const QgsException & )
    {
      mCurrentDistance = 0.0;
    }
  }

  if ( !qgsDoubleNear( mMinimumDistance, 0.0 ) )
  {
    mMinimumDistanceFulfilled = mRubberbandModel->vertexCount() == 1 || mCurrentDistance >= mMinimumDistance;

    if ( !mConjunction && mMinimumDistanceFulfilled )
    {
      trackPosition();
      return;
    }
  }

  if ( mMinimumDistanceFulfilled && mTimeIntervalFulfilled && mSensorCaptureFulfilled )
  {
    trackPosition();
  }
}

void Tracker::sensorDataReceived()
{
  mSensorCaptureFulfilled = true;

  if ( !mConjunction || ( mMinimumDistanceFulfilled && mTimeIntervalFulfilled ) )
  {
    trackPosition();
  }
}

void Tracker::start( const GnssPositionInformation &positionInformation, const QgsPoint &projectedPosition )
{
  mIsActive = true;
  emit isActiveChanged();

  mFeatureModel->layer()->startEditing();

  if ( mMinimumDistance > 0 || mTimeInterval > 0 || !mSensorCapture )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }
  if ( mSensorCapture )
  {
    connect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &Tracker::sensorDataReceived );
  }

  if ( mMeasureType == Tracker::SecondsSinceStart )
  {
    mRubberbandModel->setMeasureValue( 0 );
  }

  mSkipPositionReceived = false;
  mMaximumDistanceFailuresCount = 0;
  mCurrentDistance = mMaximumDistance;
  mTimeIntervalFulfilled = qgsDoubleNear( mTimeInterval, 0.0 );
  mMinimumDistanceFulfilled = qgsDoubleNear( mMinimumDistance, 0.0 );
  mSensorCaptureFulfilled = !mSensorCapture;

  if ( !projectedPosition.isEmpty() )
  {
    //set the start time of first position
    setStartPositionTimestamp( positionInformation.utcDateTime().isValid() ? positionInformation.utcDateTime() : QDateTime::currentDateTime() );

    //ignore maximum distance when starting/restarting a track
    mMaximumDistanceFailuresCount = MAXIMUM_DISTANCE_FAILURES + 1;

    //track first position
    processPositionInformation( positionInformation, projectedPosition );
  }
}

void Tracker::stop()
{
  //track last position
  trackPosition();

  mFeatureModel->layer()->commitChanges();

  mIsActive = false;
  emit isActiveChanged();

  if ( mMinimumDistance > 0 || mTimeInterval > 0 || !mSensorCapture )
  {
    disconnect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }
  if ( mSensorCapture )
  {
    disconnect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &Tracker::sensorDataReceived );
  }
}

void Tracker::processPositionInformation( const GnssPositionInformation &positionInformation, const QgsPoint &projectedPosition )
{
  if ( !mIsActive && !mIsReplaying )
    return;

  if ( mFilterAccuracy && positionInformation.accuracyQuality() == GnssPositionInformation::AccuracyBad )
  {
    mSkipBadPositionReceived = true;
  }

  mLastDevicePositionTimestampMSecsSinceEpoch = positionInformation.utcDateTime().toMSecsSinceEpoch();

  double measureValue = 0.0;
  switch ( mMeasureType )
  {
    case Tracker::SecondsSinceStart:
      measureValue = positionInformation.utcDateTime().toSecsSinceEpoch() - mStartPositionTimestamp.toSecsSinceEpoch();
      break;
    case Tracker::Timestamp:
      measureValue = positionInformation.utcDateTime().toSecsSinceEpoch();
      break;
    case Tracker::GroundSpeed:
      measureValue = positionInformation.speed();
      break;
    case Tracker::Bearing:
      measureValue = positionInformation.direction();
      break;
    case Tracker::HorizontalAccuracy:
      measureValue = positionInformation.hacc();
      break;
    case Tracker::VerticalAccuracy:
      measureValue = positionInformation.vacc();
      break;
    case Tracker::PDOP:
      measureValue = positionInformation.pdop();
      break;
    case Tracker::HDOP:
      measureValue = positionInformation.hdop();
      break;
    case Tracker::VDOP:
      measureValue = positionInformation.vdop();
      break;
  }

  whileBlocking( mRubberbandModel )->setMeasureValue( measureValue );
  mRubberbandModel->setCurrentCoordinate( projectedPosition );
}

void Tracker::replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer )
{
  const qint64 startTime = QDateTime::currentMSecsSinceEpoch();

  mIsReplaying = true;
  emit isReplayingChanged();

  mFeatureModel->layer()->startEditing();

  const Qgis::GeometryType geometryType = mRubberbandModel->geometryType();
  const bool isPointGeometry = geometryType == Qgis::GeometryType::Point;
  mFeatureModel->setBatchMode( isPointGeometry );

  connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  for ( const GnssPositionInformation &positionInformation : positionInformationList )
  {
    if ( mFilterAccuracy && positionInformation.accuracyQuality() == GnssPositionInformation::AccuracyBad )
      continue;

    if ( isPointGeometry && mFeatureModel->appExpressionContextScopesGenerator() )
    {
      mFeatureModel->appExpressionContextScopesGenerator()->setPositionInformation( positionInformation );
    }
    processPositionInformation( positionInformation,
                                coordinateTransformer ? coordinateTransformer->transformPosition( QgsPoint( positionInformation.longitude(), positionInformation.latitude(), positionInformation.elevation() ) ) : QgsPoint() );
  }
  disconnect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );

  mFeatureModel->setBatchMode( false );
  const int vertexCount = mRubberbandModel->vertexCount();
  if ( ( geometryType == Qgis::GeometryType::Line && vertexCount > 2 ) || ( geometryType == Qgis::GeometryType::Polygon && vertexCount > 3 ) )
  {
    mFeatureModel->applyGeometry();
    if ( mFeature.id() == FID_NULL )
    {
      mFeatureModel->create( false );
      mFeature = mFeatureModel->feature();
      emit featureCreated();
    }
    else
    {
      mFeatureModel->save( false );
    }
  }

  // Flush editing buffer
  mFeatureModel->layer()->commitChanges();

  mIsReplaying = false;
  emit isReplayingChanged();

  if ( mIsSuspended )
  {
    mIsSuspended = false;
    emit isSuspendedChanged();
    start();
  }

  const qint64 endTime = QDateTime::currentMSecsSinceEpoch();
  qInfo() << QStringLiteral( "Tracker position information replay duration: %1ms" ).arg( endTime - startTime ); // cppcheck-suppress [knownArgument,duplicateExpression]
}

void Tracker::suspendUntilReplay()
{
  if ( mIsActive )
  {
    mIsSuspended = true;
    emit isSuspendedChanged();
    stop();
  }
}

void Tracker::rubberbandModelVertexCountChanged()
{
  if ( ( !mIsActive && !mIsReplaying ) || mRubberbandModel->vertexCount() == 0 )
  {
    return;
  }

  const qint64 currentMSecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();
  bool flushBuffer = !mIsReplaying && currentMSecsSinceEpoch - mLastFeatureModelSaveMSSecsSinceEpoch > 15000;

  const Qgis::GeometryType geometryType = mRubberbandModel->geometryType();
  const int vertexCount = mRubberbandModel->vertexCount();
  if ( geometryType == Qgis::GeometryType::Point )
  {
    mFeatureModel->applyGeometry();
    mFeatureModel->resetFeatureId();
    mFeatureModel->resetAttributes( true );
    mFeatureModel->create( flushBuffer );
  }
  else
  {
    // When replaying, we can optimize things and do this only once
    if ( mIsActive )
    {
      if ( ( geometryType == Qgis::GeometryType::Line && vertexCount > 2 ) || ( geometryType == Qgis::GeometryType::Polygon && vertexCount > 3 ) )
      {
        if ( ( geometryType == Qgis::GeometryType::Line && vertexCount == 3 ) || ( geometryType == Qgis::GeometryType::Polygon && vertexCount == 4 ) )
        {
          mFeatureModel->applyGeometry();
          // We must flush the buffer on feature creation to get the proper feature ID
          mFeatureModel->create();
          mFeature = mFeatureModel->feature();
          emit featureCreated();
        }
        else
        {
          mFeatureModel->applyGeometry();
          mFeatureModel->save( flushBuffer );
        }
      }
    }
  }

  if ( flushBuffer )
  {
    mLastFeatureModelSaveMSSecsSinceEpoch = currentMSecsSinceEpoch;
  }
}

bool Tracker::filterAccuracy() const
{
  return mFilterAccuracy;
}

void Tracker::setFilterAccuracy( bool enabled )
{
  if ( mFilterAccuracy == enabled )
    return;

  mFilterAccuracy = enabled;
  emit filterAccuracyChanged();
}
```


