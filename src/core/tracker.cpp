#include <QTimer>
#include <QDebug>

#include "tracker.h"

#include "rubberbandmodel.h"
#include "qgsproject.h"

Tracker::Tracker( QgsVectorLayer *layer )
  : mLayer( layer )
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

void Tracker::setTimeInterval( int timeInterval )
{
  mTimeInterval = timeInterval;
}

int Tracker::minimumDistance() const
{
  return mMinimumDistance;
}

void Tracker::setConjunction( bool conjunction )
{
  mConjunction = conjunction;
}

bool Tracker::conjunction() const
{
  return mConjunction;
}

void Tracker::setMinimumDistance( int minimumDistance )
{
  mMinimumDistance = minimumDistance;
}

void Tracker::trackPosition()
{
  if ( std::isnan( model()->currentCoordinate().x() ) || std::isnan( model()->currentCoordinate().y() ) )
  {
    qDebug() << QString( "Coordinates not available " ) << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y();
    return;
  }
  qDebug() << QString( "Collect " ) << model()->vectorLayer() << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y() << " z:" << model()->currentCoordinate().z();

  if ( QgsWkbTypes::hasM( mLayer->wkbType() ) )
  {
    QgsPoint currentCoordinate = model()->currentCoordinate();
    currentCoordinate.addMValue( model()->currentSpeed() );
    model()->setCurrentCoordinate( currentCoordinate );
  }

  qDebug() << QString( "Coordinates are " ) << model()->currentSpeed() << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y() << " m:" << model()->currentCoordinate().m();
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

  qDebug() << QString( "distance is: " ) << distanceArea.measureLine( flatPoints ) << QString( " and the minimum is " ) << mMinimumDistance;

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

  qDebug() << QString( "Tracos startos with time" ) << mTimeInterval << " and distance " << mMinimumDistance;

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
    disconnect( mRubberbandModel,  &RubberbandModel::currentCoordinateChanged, this, &Tracker::positionReceived );
  }

  qDebug() << QString( "Tracos stoppos" );
}
