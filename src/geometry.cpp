#include "geometry.h"

#include <qgspointv2.h>
#include <qgslinestringv2.h>
#include <qgsvectorlayer.h>

Geometry::Geometry( QObject* parent )
  : QObject( parent )
{

}

QgsGeometry Geometry::asQgsGeometry() const
{
  QgsAbstractGeometryV2* geom = nullptr;

  switch ( mVectorLayer->geometryType() )
  {
    case QGis::Point:
    {
      geom = new QgsPointV2( mRubberbandModel->currentCoordinate().x(), mRubberbandModel->currentCoordinate().y() );
      break;
    }
    case QGis::Line:
    {
      QgsLineStringV2* line = new QgsLineStringV2();
      line->setPoints( mRubberbandModel->pointSequenceV2() );
      geom = line;
      break;
    }
    case QGis::Polygon:
      break;
    case QGis::UnknownGeometry:
      break;
    case QGis::NoGeometry:
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

void Geometry::setVectorLayer(QgsVectorLayer* vectorLayer)
{
  if ( mVectorLayer == vectorLayer )
    return;

  mVectorLayer = vectorLayer;
  emit vectorLayerChanged();
}

