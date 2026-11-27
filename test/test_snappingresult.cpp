/***************************************************************************
                        test_snappingresult
                        --------------------
  begin                : Nov 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "qfsnappingresult.h"

#include <qgspoint.h>
#include <qgspointlocator.h>
#include <qgsvectorlayer.h>

#include <memory>

TEST_CASE( "SnappingResult" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "LineString?crs=EPSG:3857" ), QStringLiteral( "lines" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  SECTION( "DefaultIsInvalid" )
  {
    const QfSnappingResult result;
    REQUIRE( result.type() == QfSnappingResult::Invalid );
    REQUIRE_FALSE( result.isValid() );
    REQUIRE_FALSE( result.hasVertex() );
    REQUIRE_FALSE( result.hasEdge() );
    REQUIRE_FALSE( result.hasArea() );
    REQUIRE( result.distance() == 0.0 );
    REQUIRE( result.point().isEmpty() );
    REQUIRE( result.vertexIndex() == 0 );
    REQUIRE( result.layer() == nullptr );
    REQUIRE( result.featureId() == 0 );
  }

  SECTION( "FromVertexMatch" )
  {
    const QgsPointLocator::Match match( QgsPointLocator::Vertex, layer.get(), 7, 2.5, QgsPointXY( 50, 10 ), 1 );
    const QfSnappingResult result( match );
    REQUIRE( result.type() == QfSnappingResult::Vertex );
    REQUIRE( result.isValid() );
    REQUIRE( result.hasVertex() );
    REQUIRE_FALSE( result.hasEdge() );
    REQUIRE_FALSE( result.hasArea() );
    REQUIRE( result.distance() == 2.5 );
    REQUIRE( result.point().asWkt() == QStringLiteral( "Point (50 10)" ) );
    REQUIRE( result.vertexIndex() == 1 );
    REQUIRE( result.layer() == layer.get() );
    REQUIRE( result.featureId() == 7 );
  }

  SECTION( "FromEdgeMatch" )
  {
    const QgsPointLocator::Match match( QgsPointLocator::Edge, layer.get(), 7, 3, QgsPointXY( 30, 10 ), 0 );
    const QfSnappingResult result( match );
    REQUIRE( result.type() == QfSnappingResult::Edge );
    REQUIRE( result.hasEdge() );
    REQUIRE_FALSE( result.hasVertex() );
    REQUIRE( result.point().asWkt() == QStringLiteral( "Point (30 10)" ) );
    REQUIRE( result.vertexIndex() == 0 );
  }

  SECTION( "FromAreaMatch" )
  {
    const QgsPointLocator::Match match( QgsPointLocator::Area, layer.get(), 7, 0, QgsPointXY( 30, 20 ) );
    const QfSnappingResult result( match );
    REQUIRE( result.type() == QfSnappingResult::Area );
    REQUIRE( result.hasArea() );
    REQUIRE_FALSE( result.hasVertex() );
    REQUIRE_FALSE( result.hasEdge() );
  }

  SECTION( "FromInvalidMatch" )
  {
    const QfSnappingResult result( ( QgsPointLocator::Match() ) );
    REQUIRE( result.type() == QfSnappingResult::Invalid );
    REQUIRE_FALSE( result.isValid() );
  }

  SECTION( "SetPointKeepsMatchDetails" )
  {
    QfSnappingResult result( QfSnappingResult::Vertex, layer.get(), 3, 1.5, QgsPoint( 50, 10 ), 1 );
    result.setPoint( QgsPoint( 50, 10, 2, 200 ) );
    REQUIRE( result.point().asWkt() == QStringLiteral( "Point ZM (50 10 2 200)" ) );
    REQUIRE( result.type() == QfSnappingResult::Vertex );
    REQUIRE( result.distance() == 1.5 );
    REQUIRE( result.vertexIndex() == 1 );
    REQUIRE( result.layer() == layer.get() );
    REQUIRE( result.featureId() == 3 );
  }

  SECTION( "EdgePoints" )
  {
    const QgsPoint edge[2] = { QgsPoint( 10, 10 ), QgsPoint( 50, 10 ) };
    const QfSnappingResult result( QfSnappingResult::Edge, layer.get(), 1, 3, QgsPoint( 30, 10 ), 0, edge );
    QgsPoint first;
    QgsPoint second;
    result.edgePoints( first, second );
    REQUIRE( first.asWkt() == QStringLiteral( "Point (10 10)" ) );
    REQUIRE( second.asWkt() == QStringLiteral( "Point (50 10)" ) );
  }

  SECTION( "LayerIsClearedWhenDeleted" )
  {
    const QfSnappingResult result( QfSnappingResult::Vertex, layer.get(), 1, 0, QgsPoint( 50, 10 ) );
    REQUIRE( result.layer() == layer.get() );
    layer.reset();
    REQUIRE( result.layer() == nullptr );
  }

  SECTION( "Equality" )
  {
    const QfSnappingResult result( QfSnappingResult::Vertex, layer.get(), 7, 2.5, QgsPoint( 50, 10 ), 1 );
    REQUIRE( result == QfSnappingResult( QfSnappingResult::Vertex, layer.get(), 7, 2.5, QgsPoint( 50, 10 ), 1 ) );
    REQUIRE( result == QfSnappingResult( QgsPointLocator::Match( QgsPointLocator::Vertex, layer.get(), 7, 2.5, QgsPointXY( 50, 10 ), 1 ) ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Edge, layer.get(), 7, 2.5, QgsPoint( 50, 10 ), 1 ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Vertex, nullptr, 7, 2.5, QgsPoint( 50, 10 ), 1 ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Vertex, layer.get(), 8, 2.5, QgsPoint( 50, 10 ), 1 ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Vertex, layer.get(), 7, 3, QgsPoint( 50, 10 ), 1 ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Vertex, layer.get(), 7, 2.5, QgsPoint( 50, 11 ), 1 ) );
    REQUIRE( result != QfSnappingResult( QfSnappingResult::Vertex, layer.get(), 7, 2.5, QgsPoint( 50, 10 ), 2 ) );
  }
}
