

# File qfgeometryutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfgeometryutils.cpp**](qfgeometryutils_8cpp.md)

[Go to the documentation of this file](qfgeometryutils_8cpp.md)


```C++
/***************************************************************************
  qfgeometryutils.cpp - QfGeometryUtils

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

#include "qfgeometryutils.h"
#include "qfrubberbandmodel.h"

#include <qgscoordinatetransform.h>
#include <qgsdistancearea.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsproject.h>
#include <qgsunittypes.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerutils.h>

QfGeometryUtils::QfGeometryUtils( QObject *parent )
  : QObject( parent )
{
}

QgsGeometry QfGeometryUtils::polygonFromRubberband( QfRubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType )
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

QgsGeometry QfGeometryUtils::lineFromRubberband( QfRubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs, Qgis::WkbType wkbType )
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

QgsGeometry QfGeometryUtils::variableWidthBufferByMFromRubberband( QfRubberbandModel *rubberBandModel, const QgsCoordinateReferenceSystem &crs )
{
  QgsPointSequence points = rubberBandModel->pointSequence( crs, Qgis::WkbType::PointM, false );
  std::unique_ptr<QgsLineString> line = std::make_unique<QgsLineString>( points );
  QgsGeometry g( std::move( line ) );
  return g.variableWidthBufferByM( 5 );
}

QfGeometryUtils::GeometryOperationResult QfGeometryUtils::reshapeFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, QfRubberbandModel *rubberBandModel )
{
  QgsFeature selectedFeature = layer->getFeature( fid );
  QgsGeometry selectedGeometry = selectedFeature.geometry();
  if ( selectedGeometry.isNull() || ( QgsWkbTypes::geometryType( selectedGeometry.wkbType() ) != Qgis::GeometryType::Line && QgsWkbTypes::geometryType( selectedGeometry.wkbType() ) != Qgis::GeometryType::Polygon ) )
  {
    return QfGeometryUtils::GeometryOperationResult::InvalidBaseGeometry;
  }

  const QgsPointSequence points = rubberBandModel->pointSequence( layer->crs(), Qgis::WkbType::Point, false );
  const QgsLineString reshapeLineString( points );
  const QgsGeometry reshapeLineStringGeom( reshapeLineString.clone() );

  QfGeometryUtils::GeometryOperationResult reshapeReturn = static_cast<QfGeometryUtils::GeometryOperationResult>( selectedGeometry.reshapeGeometry( reshapeLineString ) );

  /* Implementation logic:
   * - When both avoid intersection and topological editing is off, no other feature geometries are changed
   * - When avoid intersection is on and topological editing is off, the targeted feature will avoid
   *   overlapping other feature geometries
   * - When avoid intersection is off and topological editing is on, other features on the target layer
   *   will be reshaped when overlapping the reshape line
   * - When both avoid intersection and topological editing are on, the targeted feature will avoid overlapping
   *   other features geometries except those from the target layer which will be reshaped when overlapping
   *   the reshape line
   */
  if ( reshapeReturn == QfGeometryUtils::GeometryOperationResult::Success )
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

      if ( QgsProject::instance()->topologicalEditing() )
      {
        // Since we're allowing reshaping in our logic when topological editing is on, remove the layer as it'll never overlap
        avoidIntersectionsLayers.removeAll( layer );
      }

      if ( !avoidIntersectionsLayers.isEmpty() )
      {
        QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
        ignoredFeature.insert( layer, QSet<QgsFeatureId>() << fid );
        selectedGeometry.avoidIntersectionsV2( avoidIntersectionsLayers, ignoredFeature );
      }

      if ( selectedGeometry.isEmpty() ) // Intersection removal might have removed the whole geometry
      {
        return QfGeometryUtils::GeometryOperationResult::NothingHappened;
      }
    }

    if ( QgsProject::instance()->topologicalEditing() )
    {
      QgsFeatureIterator fit = layer->getFeatures( QgsFeatureRequest().setFilterRect( selectedGeometry.boundingBox() ) );
      QgsFeature otherFeature;
      while ( fit.nextFeature( otherFeature ) )
      {
        if ( otherFeature.id() == fid )
          continue;

        QgsGeometry otherGeometry = otherFeature.geometry();
        if ( otherGeometry.isNull() )
          continue;

        const bool isPolygon = layer->geometryType() == Qgis::GeometryType::Polygon;
        if ( selectedGeometry.intersects( reshapeLineStringGeom ) && otherGeometry.intersects( reshapeLineStringGeom ) )
        {
          QfGeometryUtils::GeometryOperationResult otherReturn = static_cast<QfGeometryUtils::GeometryOperationResult>( otherGeometry.reshapeGeometry( reshapeLineString ) );
          if ( otherReturn == QfGeometryUtils::GeometryOperationResult::Success )
          {
            if ( !isPolygon || ( !otherGeometry.intersects( selectedGeometry ) || otherGeometry.touches( selectedGeometry ) ) )
            {
              layer->changeGeometry( otherFeature.id(), otherGeometry );
              // Add topological points
              layer->addTopologicalPoints( otherGeometry );
              continue;
            }
          }
        }

        // Intersect fallback for polygon
        if ( isPolygon )
        {
          if ( !otherGeometry.intersects( selectedGeometry ) )
          {
            // No intersection, moving on
            continue;
          }

          otherGeometry = otherFeature.geometry();
          QgsGeometry otherGeometryDifference = otherGeometry.difference( selectedGeometry );
          if ( otherGeometryDifference.isExactlyEqual( otherGeometry ) )
          {
            // No change to geometry, moving on
            continue;
          }

          if ( QgsWkbTypes::isMultiType( otherGeometryDifference.wkbType() ) && !QgsWkbTypes::isMultiType( layer->wkbType() ) )
          {
            QVector<QgsGeometry> otherGeometryDifferenceCollection = otherGeometryDifference.asGeometryCollection();
            QVector<QgsGeometry>::iterator largestModifiedFeature = std::max_element( otherGeometryDifferenceCollection.begin(), otherGeometryDifferenceCollection.end(), []( const QgsGeometry &a, const QgsGeometry &b ) -> bool {
              return a.area() < b.area();
            } );
            otherGeometryDifference = *largestModifiedFeature;
            otherGeometryDifferenceCollection.erase( largestModifiedFeature );
            for ( QgsGeometry &otherGeometryDifferenceFromCollection : otherGeometryDifferenceCollection )
            {
              QgsFeature newFeature = QgsVectorLayerUtils::createFeature( layer, otherGeometryDifferenceFromCollection, otherFeature.attributes().toMap() );
              const QString sourcePrimaryKeys = layer->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
              if ( !sourcePrimaryKeys.isEmpty() && layer->fields().lookupField( sourcePrimaryKeys ) >= 0 )
              {
                const int sourcePrimaryKeysIndex = layer->fields().lookupField( sourcePrimaryKeys );
                if ( !layer->fields().at( sourcePrimaryKeysIndex ).defaultValueDefinition().isValid() )
                {
                  newFeature.setAttribute( layer->fields().lookupField( sourcePrimaryKeys ), QVariant() );
                }
              }
              layer->addFeature( newFeature );
              // Add topological points
              layer->addTopologicalPoints( otherGeometryDifference );
            }
          }
          layer->changeGeometry( otherFeature.id(), otherGeometryDifference );
          // Add topological points
          layer->addTopologicalPoints( otherGeometryDifference );
        }
      }
    }

    layer->changeGeometry( fid, selectedGeometry );

    // Add topological points
    if ( QgsProject::instance()->topologicalEditing() )
    {
      layer->addTopologicalPoints( selectedGeometry );
    }
  }

  return reshapeReturn;
}

QfGeometryUtils::GeometryOperationResult QfGeometryUtils::eraseFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, QfRubberbandModel *rubberBandModel )
{
  QgsFeature feature = layer->getFeature( fid );
  QgsGeometry geom = feature.geometry();
  if ( geom.isNull() || ( QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Line && QgsWkbTypes::geometryType( geom.wkbType() ) != Qgis::GeometryType::Polygon ) )
  {
    return QfGeometryUtils::GeometryOperationResult::InvalidBaseGeometry;
  }

  const QgsGeometry diffGeom = variableWidthBufferByMFromRubberband( rubberBandModel, layer->crs() );
  QgsGeometry resultGeom = geom.difference( diffGeom );
  if ( !resultGeom.isNull() )
  {
    if ( QgsWkbTypes::isMultiType( resultGeom.wkbType() ) && !QgsWkbTypes::isMultiType( layer->wkbType() ) )
    {
      return QfGeometryUtils::GeometryOperationResult::AddPartNotMultiGeometry;
    }

    layer->changeGeometry( fid, resultGeom );
    // Add topological points
    if ( QgsProject::instance()->topologicalEditing() )
    {
      layer->addTopologicalPoints( resultGeom );
    }
    return QfGeometryUtils::GeometryOperationResult::Success;
  }

  return QfGeometryUtils::GeometryOperationResult::InvalidInputGeometryType;
}

QfGeometryUtils::GeometryOperationResult QfGeometryUtils::addRingFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, QfRubberbandModel *rubberBandModel )
{
  if ( !layer || !rubberBandModel )
    return QfGeometryUtils::GeometryOperationResult::NothingHappened;

  QgsPointSequence ring = rubberBandModel->pointSequence( layer->crs(), layer->wkbType(), true );
  if ( ring.size() < 3 )
  {
    return QfGeometryUtils::GeometryOperationResult::AddRingNotValid;
  }

  // Try to fix invalid geometries, useful when being passed on a freehand digitized ring
  QgsGeometry geometry( new QgsLineString( ring ) );
  if ( !geometry.isNull() )
  {
    geometry = geometry.makeValid();
    const QgsLineString *line = qgsgeometry_cast<const QgsLineString *>( geometry.constGet() );
    if ( !line || line->isEmpty() )
    {
      return QfGeometryUtils::GeometryOperationResult::AddRingNotValid;
    }

    // Reset the ring to match the valid geometry
    line->points( ring );
  }

  const bool wasEditing = ( layer->editBuffer() );
  if ( !wasEditing )
  {
    layer->startEditing();
  }
  else
  {
    layer->commitChanges( false );
  }

  QfGeometryUtils::GeometryOperationResult result = static_cast<QfGeometryUtils::GeometryOperationResult>( layer->addRing( ring, &fid ) );
  if ( result != QfGeometryUtils::GeometryOperationResult::Success )
  {
    layer->rollBack();
  }
  else
  {
    layer->commitChanges( !wasEditing );
  }

  return result;
}

QfGeometryUtils::GeometryOperationResult QfGeometryUtils::splitFeatureFromRubberband( QgsVectorLayer *layer, QgsFeatureId fid, QfRubberbandModel *rubberBandModel )
{
  if ( !layer || !rubberBandModel )
    return QfGeometryUtils::GeometryOperationResult::NothingHappened;

  // The connection below will be triggered when the new feature is committed and will provide
  // the saved feature ID needed to fetch the saved feature back from the data provider
  QgsFeatureIds createdFeatureIds;
  QMetaObject::Connection connection = connect( layer, &QgsVectorLayer::featureAdded, [&createdFeatureIds]( QgsFeatureId fid ) { createdFeatureIds << fid; } );

  layer->selectByIds( QgsFeatureIds() << fid, Qgis::SelectBehavior::SetSelection );

  const bool wasEditing = ( layer->editBuffer() );
  if ( !wasEditing )
  {
    layer->startEditing();
  }
  else
  {
    layer->commitChanges( false );
  }

  QgsPointSequence line = rubberBandModel->pointSequence( layer->crs(), Qgis::WkbType::Point, false );
  QfGeometryUtils::GeometryOperationResult result = static_cast<QfGeometryUtils::GeometryOperationResult>( layer->splitFeatures( line, true ) );
  if ( result != QfGeometryUtils::GeometryOperationResult::Success )
  {
    layer->rollBack();
  }
  else
  {
    const QString sourcePrimaryKeys = layer->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
    if ( !sourcePrimaryKeys.isEmpty() && layer->fields().lookupField( sourcePrimaryKeys ) >= 0 )
    {
      const int sourcePrimaryKeysIndex = layer->fields().lookupField( sourcePrimaryKeys );
      if ( !layer->fields().at( sourcePrimaryKeysIndex ).defaultValueDefinition().isValid() )
      {
        for ( const QgsFeatureId &createdFeatureId : createdFeatureIds )
        {
          layer->changeAttributeValue( createdFeatureId, sourcePrimaryKeysIndex, QVariant() );
        }
      }
    }
    layer->commitChanges( !wasEditing );
  }

  layer->removeSelection();

  disconnect( connection );

  return result;
}

QgsPoint QfGeometryUtils::coordinateToPoint( const QGeoCoordinate &coor )
{
  return QgsPoint( coor.longitude(), coor.latitude(), coor.altitude() );
}

double QfGeometryUtils::distanceBetweenPoints( const QgsPoint &start, const QgsPoint &end )
{
  return start.distance( end );
}

QgsPoint QfGeometryUtils::reprojectPointToWgs84( const QgsPoint &point, const QgsCoordinateReferenceSystem &crs )
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

QgsPoint QfGeometryUtils::centroid( const QgsGeometry &geometry )
{
  const QgsGeometry centroid = geometry.centroid();
  if ( !centroid.isEmpty() )
  {
    return QgsPoint( *static_cast<const QgsPoint *>( centroid.constGet() ) );
  }
  return QgsPoint();
}

QgsPointXY QfGeometryUtils::reprojectPoint( const QgsPointXY &point, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
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
    return QgsPointXY();
  }

  return reprojectedPoint;
}

QgsPoint QfGeometryUtils::reprojectPoint( const QgsPoint &point, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
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

QgsRectangle QfGeometryUtils::reprojectRectangle( const QgsRectangle &rectangle, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( sourceCrs == destinationCrs )
  {
    return rectangle;
  }

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

QgsGeometry QfGeometryUtils::reprojectGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( sourceCrs == destinationCrs )
  {
    return geometry;
  }

  const QgsCoordinateTransform ct( sourceCrs, destinationCrs, QgsProject::instance() );
  QgsGeometry reprojectedGeometry = geometry;
  try
  {
    if ( reprojectedGeometry.transform( ct ) == Qgis::GeometryOperationResult::Success )
    {
      return reprojectedGeometry;
    }
  }
  catch ( QgsCsException & )
  {
    return QgsGeometry();
  }

  return QgsGeometry();
}

QgsGeometry QfGeometryUtils::createGeometryFromWkt( const QString &wkt )
{
  return QgsGeometry::fromWkt( wkt );
}

QgsRectangle QfGeometryUtils::createRectangleFromPoints( const QgsPoint &p1, const QgsPoint &p2 )
{
  return QgsRectangle( p1, p2 );
}

bool QfGeometryUtils::geometryWithin( const QgsGeometry &geometry, const QgsGeometry &referenceGeometry )
{
  return geometry.within( referenceGeometry );
}

bool QfGeometryUtils::geometryOverlaps( const QgsGeometry &geometry, const QgsGeometry &referenceGeometry )
{
  return geometry.overlaps( referenceGeometry );
}
```


