#include "geometry.h"

#include <qgspointv2.h>

Geometry::Geometry( QObject* parent )
  : QObject( parent )
{

}

QgsGeometry Geometry::asQgsGeometry() const
{
  QgsPointV2* geom = new QgsPointV2();

  geom->setX( mCurrentCoordinate.x() );
  geom->setY( mCurrentCoordinate.y() );

  return QgsGeometry( geom );
}

QPointF Geometry::currentCoordinate()
{
  return mCurrentCoordinate;
}

void Geometry::setCurrentCoordinate( QPointF coord )
{
  mCurrentCoordinate = coord;
}

RubberbandModel* Geometry::rubberbandModel() const
{
  return mRubberbandModel;
}

void Geometry::setRubberbandModel( RubberbandModel* rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  mRubberbandModel = rubberbandModel;
  emit rubberbandModelChanged();
}

