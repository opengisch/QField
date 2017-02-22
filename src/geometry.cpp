#include "geometry.h"

#include <qgspointv2.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
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
      geom = new QgsPointV2( mRubberbandModel->currentPoint(  mVectorLayer->crs() ) );
      break;
    }
    case QgsWkbTypes::LineGeometry:
    {
      QgsLineString* line = new QgsLineString();
      line->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs() ) );
      geom = line;
      break;
    }
    case QgsWkbTypes::PolygonGeometry:
    {
      QgsPolygonV2* polygon = new QgsPolygonV2();
      QgsLineString* ring = new QgsLineString();
      ring->setPoints( mRubberbandModel->pointSequence( mVectorLayer->crs() ) );
      polygon->setExteriorRing( ring );
      geom = polygon;
      break;
    }

    break;
    case QgsWkbTypes::UnknownGeometry:
      break;
    case QgsWkbTypes::NullGeometry:
      break;

  }

  // QgsCoordinateTransform ct( mRubberbandModel->crs(), mVectorLayer->crs() );
  // return ct.transform( QgsGeometry( geom ) );
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

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::lengthChanged );
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::areaChanged );
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::segmentLengthChanged );
    disconnect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Geometry::areaValidChanged );
    disconnect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Geometry::lengthValidChanged );
  }
  mRubberbandModel = rubberbandModel;
  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::lengthChanged );
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::areaChanged );
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Geometry::segmentLengthChanged );
    connect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Geometry::areaValidChanged );
    connect( mRubberbandModel, &RubberbandModel::vertexCountChanged, this, &Geometry::lengthValidChanged );
  }
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

qreal Geometry::length() const
{
  return asQgsGeometry().length();
}

bool Geometry::lengthValid() const
{
  if ( !mVectorLayer )
    return false;

  switch ( mVectorLayer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
      return false;

    case QgsWkbTypes::LineGeometry:
      FALLTHROUGH;
    case QgsWkbTypes::PolygonGeometry:
      return mRubberbandModel->vertexCount() >= 2;

    default:
      return false;
  }
}

qreal Geometry::area() const
{
  return asQgsGeometry().area();
}

bool Geometry::areaValid() const
{
  if ( !mVectorLayer )
    return false;

  switch ( mVectorLayer->geometryType() )
  {
    case QgsWkbTypes::PointGeometry:
      return false;

    case QgsWkbTypes::LineGeometry:
      return false;

    case QgsWkbTypes::PolygonGeometry:
      return mRubberbandModel->vertexCount() >= 3;

    default:
      return false;
  }
}

qreal Geometry::segmentLength() const
{
  if ( !mRubberbandModel )
    return qQNaN();

  if ( mRubberbandModel->vertexCount() < 2 )
    return qQNaN();

  QgsPointSequence points = mRubberbandModel->pointSequence( mVectorLayer->crs() );

  auto pointIt = points.constEnd() - 1;

  const QgsPointV2& pt1 = *pointIt;
  pointIt--;
  const QgsPointV2& pt2 = *pointIt;

  return qSqrt( qPow( pt1.x() - pt2.x(), 2 ) + qPow( pt1.y() - pt2.y(), 2 ) );
}
