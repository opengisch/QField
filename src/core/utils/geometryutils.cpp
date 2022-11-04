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
#include "rubberbandmodel.h"

#include <qgscoordinatetransform.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

GeometryUtils::GeometryUtils( QObject *parent )
  : QObject( parent )
{
}

QgsGeometry GeometryUtils::polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs )
{
  QgsPointSequence ring = rubberBandModel->pointSequence( crs, QgsWkbTypes::Point, true );
  QgsLineString ext( ring );
  std::unique_ptr<QgsPolygon> polygon = std::make_unique<QgsPolygon>();
  polygon->setExteriorRing( ext.clone() );
  QgsGeometry g( std::move( polygon ) );
  return g;
}

QgsGeometry GeometryUtils::lineFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs )
{
  QgsPointSequence points = rubberBandModel->pointSequence( crs, QgsWkbTypes::Point, false );
  std::unique_ptr<QgsLineString> line = std::make_unique<QgsLineString>( points );
  QgsGeometry g( std::move( line ) );
  return g;
}

GeometryUtils::GeometryOperationResult GeometryUtils::reshapeFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
{
  QgsFeature feature = layer->getFeature( fid );
  QgsGeometry geom = feature.geometry();
  if ( geom.isNull() || ( QgsWkbTypes::geometryType( geom.wkbType() ) != QgsWkbTypes::LineGeometry && QgsWkbTypes::geometryType( geom.wkbType() ) != QgsWkbTypes::PolygonGeometry ) )
    return GeometryUtils::GeometryOperationResult::InvalidBaseGeometry;

  QgsPointSequence points = rubberBandModel->pointSequence( layer->crs(), QgsWkbTypes::Point, false );
  QgsLineString reshapeLineString( points );

  GeometryUtils::GeometryOperationResult reshapeReturn = static_cast<GeometryUtils::GeometryOperationResult>( geom.reshapeGeometry( reshapeLineString ) );
  if ( reshapeReturn == GeometryUtils::GeometryOperationResult::Success )
  {
    //avoid intersections on polygon layers
    if ( layer->geometryType() == QgsWkbTypes::PolygonGeometry )
    {
      QList<QgsVectorLayer *> avoidIntersectionsLayers;
      switch ( QgsProject::instance()->avoidIntersectionsMode() )
      {
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
#else
        case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
#endif
          avoidIntersectionsLayers.append( layer );
          break;
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsLayers:
#else
        case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsLayers:
#endif
          avoidIntersectionsLayers = QgsProject::instance()->avoidIntersectionsLayers();
          break;
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AllowIntersections:
#else
        case QgsProject::AvoidIntersectionsMode::AllowIntersections:
#endif
          break;
      }
      if ( !avoidIntersectionsLayers.isEmpty() )
      {
        QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
        ignoredFeature.insert( layer, QSet<QgsFeatureId>() << fid );
        geom.avoidIntersections( avoidIntersectionsLayers, ignoredFeature );
      }

      if ( geom.isEmpty() ) //intersection removal might have removed the whole geometry
      {
        return GeometryUtils::GeometryOperationResult::NothingHappened;
      }
    }
    layer->changeGeometry( fid, geom );

    // Add topological points
    if ( QgsProject::instance()->topologicalEditing() )
    {
      layer->addTopologicalPoints( geom );
    }
  }

  return reshapeReturn;
}

GeometryUtils::GeometryOperationResult GeometryUtils::addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
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
  return static_cast<GeometryUtils::GeometryOperationResult>( layer->addRing( ring, &fid ) );
}

GeometryUtils::GeometryOperationResult GeometryUtils::splitFeatureFromRubberband( QgsVectorLayer *layer, RubberbandModel *rubberBandModel )
{
  QgsPointSequence line = rubberBandModel->pointSequence( layer->crs(), QgsWkbTypes::Point, false );
  return static_cast<GeometryUtils::GeometryOperationResult>( layer->splitFeatures( line, true ) );
}

QgsPoint GeometryUtils::coordinateToPoint( const QGeoCoordinate &coor )
{
  return QgsPoint( coor.longitude(), coor.latitude(), coor.altitude() );
}

double GeometryUtils::distanceBetweenPoints( const QgsPoint &start, const QgsPoint &end )
{
  return start.distance( end );
}

QgsPoint GeometryUtils::reprojectPointToWgs84( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs )
{
  const QgsCoordinateReferenceSystem wgs84Crs = QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) );
  const QgsCoordinateTransform ct( crs, wgs84Crs, QgsProject::instance() );
  QgsPointXY reprojectedPoint;
  try
  {
    ct.transform( point.x(), point.y() );
    reprojectedPoint = ct.transform( point );
  }
  catch ( QgsCsException & )
  {
    return QgsPoint();
  }

  return QgsPoint( reprojectedPoint.x(),
                   reprojectedPoint.y(),
                   point.is3D() ? point.z() : std::numeric_limits<double>::quiet_NaN(),
                   point.isMeasure() ? point.m() : std::numeric_limits<double>::quiet_NaN() );
}

QgsPoint GeometryUtils::reprojectPoint( const QgsPoint &point, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( sourceCrs == destinationCrs )
    return point;

  const QgsCoordinateTransform ct( sourceCrs, destinationCrs, QgsProject::instance() );
  QgsPointXY reprojectedPoint;
  try
  {
    ct.transform( point.x(), point.y() );
    reprojectedPoint = ct.transform( point );
  }
  catch ( QgsCsException & )
  {
    return QgsPoint();
  }

  return QgsPoint( reprojectedPoint.x(),
                   reprojectedPoint.y(),
                   point.is3D() ? point.z() : std::numeric_limits<double>::quiet_NaN(),
                   point.isMeasure() ? point.m() : std::numeric_limits<double>::quiet_NaN() );
}
