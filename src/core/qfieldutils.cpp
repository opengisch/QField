#include "qfieldutils.h"

#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsvectorlayer.h>

#include "rubberbandmodel.h"

QFieldUtils::QFieldUtils(QObject *parent) : QObject(parent)
{

}

QgsFeature QFieldUtils::initFeature(QgsVectorLayer *layer, QgsGeometry geometry)
{
  QgsFeature f(layer->fields());
  f.setGeometry(geometry);
  return f;
}

QgsGeometry QFieldUtils::polygonFromRubberband(RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs)
{
  QgsPointSequence ring = rubberBandModel->pointSequence(crs, QgsWkbTypes::Point, true);
  QgsLineString ext( ring );
  std::unique_ptr< QgsPolygon > polygon = qgis::make_unique< QgsPolygon >( );
  polygon->setExteriorRing( ext.clone() );
  QgsGeometry g( std::move( polygon ) );
  return g;
}

QgsGeometry::OperationResult QFieldUtils::addRingFromRubberBand(QgsVectorLayer* layer, QgsFeatureId fid, RubberbandModel *rubberBandModel)
{
  QgsPointSequence ring = rubberBandModel->pointSequence(layer->crs(), layer->wkbType(), true);
  return layer->addRing(ring, &fid);
}

