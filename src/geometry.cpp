#include "geometry.h"

#include <qgspointv2.h>
#include <qgslinestring.h>
#include <qgsvectorlayer.h>

Geometry::Geometry( QObject* parent )
  : QObject( parent )
{

}

QgsGeometry Geometry::asQgsGeometry() const
{
  QgsAbstractGeometry* geom = nullptr;

  if ( !mVectorLayer )
    return QgsGeometry();

  switch ( mVectorLayer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
    {
      geom = new QgsPointV2( mRubberbandModel->currentCoordinate().x(), mRubberbandModel->currentCoordinate().y() );
      break;
    }
    case QgsWkbTypes::LineGeometry:
    {
      QgsLineString* line = new QgsLineString();
      line->setPoints( mRubberbandModel->pointSequence() );
      geom = line;
      break;
    }
    case QgsWkbTypes::PolygonGeometry:
      break;
    case QgsWkbTypes::UnknownGeometry:
      break;
    case QgsWkbTypes::NullGeometry:
      break;

  }

  return QgsGeometry( geom );
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

void Geometry::applyRubberband()
{
  // TODO: Will need to be implemented for multipart features or polygons with holes.
}

QgsVectorLayer* Geometry::vectorLayer() const
{
  return mVectorLayer;
}

void Geometry::setVectorLayer( QgsVectorLayer* vectorLayer )
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}

