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

#include "qgsproject.h"
#include "rubberbandmodel.h"
#include "tracker.h"

#include <QTimer>

Tracker::Tracker( QgsVectorLayer *layer, bool visible )
  : mLayer( layer ), mVisible( visible )
{
}

RubberbandModel *Tracker::model() const
{
  return mRubberbandModel;
}

void Tracker::setModel( RubberbandModel *model )
{
  if ( mRubberbandModel == model )
    return;
  mRubberbandModel = model;
}

int Tracker::timeInterval() const
{
  return mTimeInterval;
}

void Tracker::setTimeInterval( const int timeInterval )
{
  mTimeInterval = timeInterval;
}

int Tracker::minimumDistance() const
{
  return mMinimumDistance;
}

void Tracker::setConjunction( const bool conjunction )
{
  mConjunction = conjunction;
}

QDateTime Tracker::startPositionTimestamp() const
{
  return mStartPositionTimestamp;
}

void Tracker::setStartPositionTimestamp( const QDateTime &startPositionTimestamp )
{
  mStartPositionTimestamp = startPositionTimestamp;
}

bool Tracker::conjunction() const
{
  return mConjunction;
}

void Tracker::setMinimumDistance( const int minimumDistance )
{
  mMinimumDistance = minimumDistance;
}

void Tracker::trackPosition()
{
  if ( std::isnan( model()->currentCoordinate().x() ) || std::isnan( model()->currentCoordinate().y() ) )
  {
    return;
  }

  model()->addVertex();
  mTimeIntervalFulfilled = false;
  mMinimumDistanceFulfilled = false;
}

void Tracker::positionReceived()
{
  QVector<QgsPointXY> points = mRubberbandModel->flatPointSequence( QgsProject::instance()->crs() );

  auto pointIt = points.constEnd() - 1;

  QVector<QgsPointXY> flatPoints;

  flatPoints << *pointIt;
  pointIt--;
  flatPoints << *pointIt;

  QgsDistanceArea distanceArea;
  distanceArea.setEllipsoid( QgsProject::instance()->ellipsoid() );
  distanceArea.setSourceCrs( QgsProject::instance()->crs(), QgsProject::instance()->transformContext() );

  if ( distanceArea.measureLine( flatPoints ) > mMinimumDistance )
  {
    mMinimumDistanceFulfilled = true;
    if ( !mConjunction || mTimeIntervalFulfilled )
      trackPosition();
  }
}

void Tracker::timeReceived()
{
  mTimeIntervalFulfilled = true;
  if ( !mConjunction || mMinimumDistanceFulfilled )
    trackPosition();
}

void Tracker::start()
{
  if ( mTimeInterval > 0 )
  {
    connect( &mTimer, &QTimer::timeout, this, &Tracker::timeReceived );
    mTimer.start( mTimeInterval * 1000 );
  }
  if ( mMinimumDistance > 0 )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }

  //set the start time
  setStartPositionTimestamp( QDateTime::currentDateTime() );
  model()->setMeasureValue( 0 );

  //track first position
  trackPosition();
}

void Tracker::stop()
{
  if ( mTimeInterval > 0 )
  {
    mTimer.stop();
    disconnect( &mTimer, &QTimer::timeout, this, &Tracker::trackPosition );
  }
  if ( mMinimumDistance > 0 )
  {
    disconnect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }
}
