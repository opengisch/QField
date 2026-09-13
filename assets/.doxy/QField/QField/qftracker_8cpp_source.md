

# File qftracker.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qftracker.cpp**](qftracker_8cpp.md)

[Go to the documentation of this file](qftracker_8cpp.md)


```C++
/***************************************************************************
 qftracker.cpp - QfTracker
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

#include "qffeaturemodel.h"
#include "qfrubberbandmodel.h"
#include "qftracker.h"
#include "qgsquickcoordinatetransformer.h"

#include <QRandomGenerator>
#include <qgsproject.h>
#include <qgssensormanager.h>

#define MAXIMUM_DISTANCE_FAILURES 20

QfTracker::QfTracker( QgsVectorLayer *vectorLayer )
  : mVectorLayer( vectorLayer )
{
  QRandomGenerator *rng = QRandomGenerator::global();
  mColor = QColor::fromRgbF( std::min( 0.75, rng->generateDouble() ), std::min( 0.75, rng->generateDouble() ), std::min( 0.75, rng->generateDouble() ), 0.6 );

  mDa.setEllipsoid( QgsProject::instance()->ellipsoid() );
  mDa.setSourceCrs( QgsProject::instance()->crs(), QgsProject::instance()->transformContext() );
}

void QfTracker::setColor( const QColor &color )
{
  if ( mColor == color )
  {
    return;
  }

  mColor = color;
  emit colorChanged();
}

void QfTracker::setVisible( bool visible )
{
  if ( mVisible == visible )
    return;

  mVisible = visible;
  emit visibleChanged();
}

void QfTracker::setVectorLayer( QgsVectorLayer *vectorLayer )
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}

QfRubberbandModel *QfTracker::rubberbandModel() const
{
  return mRubberbandModel;
}

void QfTracker::setRubberbandModel( QfRubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexCountChanged, this, &QfTracker::rubberbandModelVertexCountChanged );
  }

  mRubberbandModel = rubberbandModel;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &QfRubberbandModel::vertexCountChanged, this, &QfTracker::rubberbandModelVertexCountChanged );
  }

  emit rubberbandModelChanged();
}

QfFeatureModel *QfTracker::featureModel() const
{
  return mFeatureModel;
}

void QfTracker::setFeatureModel( QfFeatureModel *featureModel )
{
  if ( mFeatureModel == featureModel )
    return;

  mFeatureModel = featureModel;
  emit featureModelChanged();
}

QgsFeature QfTracker::feature() const
{
  return mFeature;
}

void QfTracker::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  emit featureChanged();
}

void QfTracker::setTimeInterval( double timeInterval )
{
  if ( mTimeInterval == timeInterval )
    return;

  mTimeInterval = timeInterval;
  emit timeIntervalChanged();
}

void QfTracker::setMinimumDistance( double minimumDistance )
{
  if ( mMinimumDistance == minimumDistance )
    return;

  mMinimumDistance = minimumDistance;
  emit minimumDistanceChanged();
}

void QfTracker::setMaximumDistance( double maximumDistance )
{
  if ( mMaximumDistance == maximumDistance )
    return;

  mMaximumDistance = maximumDistance;
  emit maximumDistanceChanged();
}

void QfTracker::setSensorCapture( bool capture )
{
  if ( mSensorCapture == capture )
    return;

  mSensorCapture = capture;
  emit sensorCaptureChanged();
}

void QfTracker::setConjunction( bool conjunction )
{
  if ( mConjunction == conjunction )
    return;

  mConjunction = conjunction;
  emit conjunctionChanged();
}

void QfTracker::setMeasureType( MeasureType type )
{
  if ( mMeasureType == type )
    return;

  mMeasureType = type;
  emit measureTypeChanged();
}

void QfTracker::trackPosition()
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

void QfTracker::positionReceived()
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
      mCurrentDistance = !qgsDoubleNear( mMaximumDistance, 0.0 ) ? mMaximumDistance + 1.0 : 0.0;
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

void QfTracker::sensorDataReceived()
{
  mSensorCaptureFulfilled = true;

  if ( !mConjunction || ( mMinimumDistanceFulfilled && mTimeIntervalFulfilled ) )
  {
    trackPosition();
  }
}

void QfTracker::start( const QfGnssPositionInformation &positionInformation, const QgsPoint &projectedPosition )
{
  mIsActive = true;
  emit isActiveChanged();

  mFeatureModel->layer()->startEditing();

  if ( mMinimumDistance > 0 || mTimeInterval > 0 || !mSensorCapture )
  {
    connect( mRubberbandModel, &QfRubberbandModel::currentCoordinateChanged, this, &QfTracker::positionReceived );
  }
  if ( mSensorCapture )
  {
    connect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &QfTracker::sensorDataReceived );
  }

  if ( mMeasureType == QfTracker::SecondsSinceStart )
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

void QfTracker::stop()
{
  //track last position
  trackPosition();

  mFeatureModel->layer()->commitChanges();

  mIsActive = false;
  emit isActiveChanged();

  if ( mMinimumDistance > 0 || mTimeInterval > 0 || !mSensorCapture )
  {
    disconnect( mRubberbandModel, &QfRubberbandModel::currentCoordinateChanged, this, &QfTracker::positionReceived );
  }
  if ( mSensorCapture )
  {
    disconnect( QgsProject::instance()->sensorManager(), &QgsSensorManager::sensorDataCaptured, this, &QfTracker::sensorDataReceived );
  }
}

void QfTracker::processPositionInformation( const QfGnssPositionInformation &positionInformation, const QgsPoint &projectedPosition )
{
  if ( !mIsActive && !mIsReplaying )
    return;

  if ( mFilterAccuracy && positionInformation.accuracyQuality() == QfGnssPositionInformation::AccuracyBad )
  {
    mSkipBadPositionReceived = true;
  }

  mLastDevicePositionTimestampMSecsSinceEpoch = positionInformation.utcDateTime().toMSecsSinceEpoch();

  double measureValue = 0.0;
  switch ( mMeasureType )
  {
    case QfTracker::SecondsSinceStart:
      measureValue = positionInformation.utcDateTime().toSecsSinceEpoch() - mStartPositionTimestamp.toSecsSinceEpoch();
      break;
    case QfTracker::Timestamp:
      measureValue = positionInformation.utcDateTime().toSecsSinceEpoch();
      break;
    case QfTracker::GroundSpeed:
      measureValue = positionInformation.speed();
      break;
    case QfTracker::Bearing:
      measureValue = positionInformation.direction();
      break;
    case QfTracker::HorizontalAccuracy:
      measureValue = positionInformation.hacc();
      break;
    case QfTracker::VerticalAccuracy:
      measureValue = positionInformation.vacc();
      break;
    case QfTracker::PDOP:
      measureValue = positionInformation.pdop();
      break;
    case QfTracker::HDOP:
      measureValue = positionInformation.hdop();
      break;
    case QfTracker::VDOP:
      measureValue = positionInformation.vdop();
      break;
  }

  whileBlocking( mRubberbandModel )->setMeasureValue( measureValue );
  mRubberbandModel->setCurrentCoordinate( projectedPosition );
}

void QfTracker::replayPositionInformationList( const QList<QfGnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer )
{
  const qint64 startTime = QDateTime::currentMSecsSinceEpoch();

  mIsReplaying = true;
  emit isReplayingChanged();

  mFeatureModel->layer()->startEditing();

  const Qgis::GeometryType geometryType = mRubberbandModel->geometryType();
  const bool isPointGeometry = geometryType == Qgis::GeometryType::Point;
  mFeatureModel->setBatchMode( isPointGeometry );

  connect( mRubberbandModel, &QfRubberbandModel::currentCoordinateChanged, this, &QfTracker::positionReceived );
  for ( const QfGnssPositionInformation &positionInformation : positionInformationList )
  {
    if ( mFilterAccuracy && positionInformation.accuracyQuality() == QfGnssPositionInformation::AccuracyBad )
      continue;

    if ( isPointGeometry && mFeatureModel->appExpressionContextScopesGenerator() )
    {
      mFeatureModel->appExpressionContextScopesGenerator()->setPositionInformation( positionInformation );
    }
    processPositionInformation( positionInformation,
                                coordinateTransformer ? coordinateTransformer->transformPosition( QgsPoint( positionInformation.longitude(), positionInformation.latitude(), positionInformation.elevation() ) ) : QgsPoint() );
  }
  disconnect( mRubberbandModel, &QfRubberbandModel::currentCoordinateChanged, this, &QfTracker::positionReceived );

  mFeatureModel->setBatchMode( false );
  const int vertexCount = mRubberbandModel->vertexCount();
  if ( ( geometryType == Qgis::GeometryType::Line && vertexCount > 2 ) || ( geometryType == Qgis::GeometryType::Polygon && vertexCount > 3 ) )
  {
    mFeatureModel->applyGeometry( false, true );
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

void QfTracker::suspendUntilReplay()
{
  if ( mIsActive )
  {
    mIsSuspended = true;
    emit isSuspendedChanged();
    stop();
  }
}

void QfTracker::rubberbandModelVertexCountChanged()
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
    mFeatureModel->applyGeometry( false, true );
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
          mFeatureModel->applyGeometry( false, true );
          // We must flush the buffer on feature creation to get the proper feature ID
          mFeatureModel->create();
          mFeature = mFeatureModel->feature();
          emit featureCreated();
        }
        else
        {
          mFeatureModel->applyGeometry( false, true );
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

bool QfTracker::filterAccuracy() const
{
  return mFilterAccuracy;
}

void QfTracker::setFilterAccuracy( bool enabled )
{
  if ( mFilterAccuracy == enabled )
    return;

  mFilterAccuracy = enabled;
  emit filterAccuracyChanged();
}
```


