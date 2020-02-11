#include <QTimer>
#include <QDebug>

#include "track.h"

#include "rubberbandmodel.h"
#include "qgsproject.h"

Track::Track( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

RubberbandModel *Track::model() const
{
  return mRubberbandModel;
}

void Track::setModel( RubberbandModel *model )
{
  if ( mRubberbandModel == model )
    return;
  mRubberbandModel = model;
  emit modelChanged();
}

int Track::timeInterval() const
{
  return mTimeInterval;
}

void Track::setTimeInterval( int timeInterval )
{
  mTimeInterval = timeInterval;
}

int Track::minimumDistance() const
{
  return mMinimumDistance;
}

void Track::setConjunction( bool conjunction )
{
  mConjunction = conjunction;
}

bool Track::conjunction() const
{
  return mConjunction;
}

void Track::setMinimumDistance( int minimumDistance )
{
  mMinimumDistance = minimumDistance;
}

void Track::trackPosition()
{
  if ( std::isnan( model()->currentCoordinate().x() ) || std::isnan( model()->currentCoordinate().y() ) )
  {
    qDebug() << QString( "Coordinates not available " ) << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y();
    return;
  }
  qDebug() << QString( "Collect " ) << model()->vectorLayer() << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y() << " z:" << model()->currentCoordinate().z();
  model()->addVertex();
  mTimeIntervalFulfilled = false;
  mMinimumDistanceFulfilled = false;
}

void Track::positionReceived()
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

void Track::timeReceived()
{
  mTimeIntervalFulfilled = true;
  if ( !mConjunction || mMinimumDistanceFulfilled )
    trackPosition();
}

void Track::start()
{
  if ( mTimeInterval > 0 )
  {
    connect( &mTimer, &QTimer::timeout, this, &Track::timeReceived );
    mTimer.start( mTimeInterval * 1000 );
  }
  if ( mMinimumDistance > 0 )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Track::positionReceived );
  }

  qDebug() << QString( "Tracos startos with time" ) << mTimeInterval << " and distance " << mMinimumDistance;

  //track first position
  trackPosition();
}

void Track::stop()
{
  if ( mTimeInterval > 0 )
  {
    mTimer.stop();
    disconnect( &mTimer, &QTimer::timeout, this, &Track::trackPosition );
  }
  if ( mMinimumDistance > 0 )
  {
    disconnect( mRubberbandModel,  &RubberbandModel::currentCoordinateChanged, this, &Track::positionReceived );
  }

  qDebug() << QString( "Tracos stoppos" );
}
