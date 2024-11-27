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

QgsGeometry GeometryUtils::polygonFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType )
{
  QgsPointSequence ring = rubberBandModel->pointSequence( crs, Qgis::WkbType::Point, true );
  QgsLineString ext( ring );
  std::unique_ptr<QgsPolygon> polygon = std::make_unique<QgsPolygon>();
  polygon->setExteriorRing( ext.clone() );

  QgsGeometry g( std::move( polygon ) );
  if ( QgsWkbTypes::isMultiType( wkbType ) )
  {
    g.convertToMultiType();
  }
  return g;
}

QgsGeometry GeometryUtils::lineFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType )
{
  QgsPointSequence points = rubberBandModel->pointSequence( crs, Qgis::WkbType::Point, false );
  std::unique_ptr<QgsLineString> line = std::make_unique<QgsLineString>( points );

  QgsGeometry g( std::move( line ) );
  if ( QgsWkbTypes::isMultiType( wkbType ) )
  {
    g.convertToMultiType();
  }
  return g;
}

QgsGeometry GeometryUtils::variableWidthBufferByMFromRubberband( RubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs )
{
  QgsPointSequence points = rubberBandModel->pointSequence( crs, Qgis::WkbType::PointM, false );
  std::unique_ptr<QgsLineString> line = std::make_unique<QgsLineString>( points );
  QgsGeometry g( std::move( line ) );
  return g.variableWidthBufferByM( 5 );
}

GeometryUtils::GeometryOperationResult GeometryUtils::reshapeFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
{
  QgsFeature feature = layer->getFeature( fid );
  QgsGeometry geom = feature.geometry();
  if ( geom.isNull() || ( QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Line && QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Polygon ) )
  {
    return GeometryUtils::GeometryOperationResult::InvalidBaseGeometry;
  }

  QgsPointSequence points = rubberBandModel->pointSequence( layer->crs(), Qgis::WkbType::Point, false );
  QgsLineString reshapeLineString( points );

  GeometryUtils::GeometryOperationResult reshapeReturn = static_cast<GeometryUtils::GeometryOperationResult>( geom.reshapeGeometry( reshapeLineString ) );
  if ( reshapeReturn == GeometryUtils::GeometryOperationResult::Success )
  {
    //avoid intersections on polygon layers
    if ( layer->geometryType() == Qgis::GeometryType::Polygon )
    {
      QList<QgsVectorLayer *> avoidIntersectionsLayers;
      switch ( QgsProject::instance()->avoidIntersectionsMode() )
      {
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
          avoidIntersectionsLayers.append( layer );
          break;
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsLayers:
          avoidIntersectionsLayers = QgsProject::instance()->avoidIntersectionsLayers();
          break;
        case Qgis::AvoidIntersectionsMode::AllowIntersections:
          break;
      }
      if ( !avoidIntersectionsLayers.isEmpty() )
      {
        QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
        ignoredFeature.insert( layer, QSet<QgsFeatureId>() << fid );
        geom.avoidIntersectionsV2( avoidIntersectionsLayers, ignoredFeature );
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

GeometryUtils::GeometryOperationResult GeometryUtils::eraseFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
{
  QgsFeature feature = layer->getFeature( fid );
  QgsGeometry geom = feature.geometry();
  if ( geom.isNull() || ( QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Line && QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Polygon ) )
  {
    return GeometryUtils::GeometryOperationResult::InvalidBaseGeometry;
  }

  const QgsGeometry diffGeom = variableWidthBufferByMFromRubberband( rubberBandModel, layer->crs() );
  QgsGeometry resultGeom = geom.difference( diffGeom );
  if ( !resultGeom.isNull() )
  {
    if ( QgsWkbTypes::isMultiType( resultGeom.wkbType() ) && !QgsWkbTypes::isMultiType( layer->wkbType() ) )
    {
      return GeometryUtils::GeometryOperationResult::AddPartNotMultiGeometry;
    }

    layer->changeGeometry( fid, resultGeom );
    // Add topological points
    if ( QgsProject::instance()->topologicalEditing() )
    {
      layer->addTopologicalPoints( resultGeom );
    }
    return GeometryUtils::GeometryOperationResult::Success;
  }

  return GeometryUtils::GeometryOperationResult::InvalidInputGeometryType;
}

GeometryUtils::GeometryOperationResult GeometryUtils::addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, RubberbandModel *rubberBandModel )
{
  QgsPointSequence ring = rubberBandModel->pointSequence( layer->crs(), layer->wkbType(), true );

  if ( ring.size() < 3 )
  {
    return GeometryUtils::GeometryOperationResult::AddRingNotValid;
  }

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
  QgsPointSequence line = rubberBandModel->pointSequence( layer->crs(), Qgis::WkbType::Point, false );
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

QgsPoint GeometryUtils::centroid( const QgsGeometry &geometry )
{
  const QgsGeometry centroid = geometry.centroid();
  if ( !centroid.isEmpty() )
  {
    return QgsPoint( *static_cast<const QgsPoint *>( centroid.constGet() ) );
  }
  return QgsPoint();
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

QgsRectangle GeometryUtils::reprojectRectangle( const QgsRectangle &rectangle, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( sourceCrs == destinationCrs )
    return rectangle;

  const QgsCoordinateTransform ct( sourceCrs, destinationCrs, QgsProject::instance() );
  QgsRectangle reprojectedRectangle;
  try
  {
    reprojectedRectangle = ct.transform( rectangle );
  }
  catch ( QgsCsException & )
  {
    return QgsRectangle();
  }

  return reprojectedRectangle;
}

QgsGeometry GeometryUtils::createGeometryFromWkt( const QString &wkt )
{
  return QgsGeometry::fromWkt( wkt );
}
