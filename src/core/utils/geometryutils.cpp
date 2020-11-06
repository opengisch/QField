/***************************************************************************
  geometryutils.cpp - GeometryUtils

 ---------------------
 begin                : 05.03.2020
 copyright            : (C) 2020 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "geometryutils.h"

#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsvectorlayer.h>

#include "rubberbandmodel.h"

GeometryUtils::GeometryUtils( QObject *parent ) : QObject( parent )
{

}


QgsGeometry GeometryUtils::polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs )
{
  QgsPointSequence ring = rubberBandModel->pointSequence( crs, QgsWkbTypes::Point, true );
  QgsLineString ext( ring );
  std::unique_ptr< QgsPolygon > polygon = qgis::make_unique< QgsPolygon >( );
  polygon->setExteriorRing( ext.clone() );
  QgsGeometry g( std::move( polygon ) );
  return g;
}

QgsGeometry::OperationResult GeometryUtils::addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
{
  QgsPointSequence ring = rubberBandModel->pointSequence( layer->crs(), layer->wkbType(), true );

  //Try to fix invalid geometries, useful when being passed on a freehand digitized ring
  QgsGeometry geometry( new QgsLineString( ring ) );
  if ( !geometry.isNull() )
  {
    QVector<QgsGeometry::Error> errors;
    geometry = geometry.makeValid();
    if ( !geometry.isNull() )
      static_cast<QgsLineString *>( geometry.get() )->points( ring );
  }
  return layer->addRing( ring, &fid );
}

QgsGeometry::OperationResult GeometryUtils::splitFeatureFromRubberband( QgsVectorLayer *layer, RubberbandModel *rubberBandModel )
{
  QgsPointSequence line = rubberBandModel->pointSequence( layer->crs(), QgsWkbTypes::Point, false );
  return layer->splitFeatures( line, true );
}

