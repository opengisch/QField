/***************************************************************************
                        test_distancearea.cpp
                        --------------------
  begin                : November 2026
  copyright            : (C) 2026 by Mohsen Dehghanzadeh
  email                : mohsen@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "catch2.h"
#include "qfdistancearea.h"
#include "qfrubberbandmodel.h"

#include <qgis.h>
#include <qgspoint.h>

#include <cmath>

TEST_CASE( "DistanceArea" )
{
  QfRubberbandModel model;
  QfDistanceArea distanceArea;
  distanceArea.setRubberbandModel( &model );

  SECTION( "Length" )
  {
    model.setGeometryType( Qgis::GeometryType::Line );
    model.addVertexFromPoint( QgsPoint( 0, 0, 0 ) );
    model.setCurrentCoordinate( QgsPoint( 10, 0, 10 ) );

    REQUIRE( distanceArea.length() == Catch::Approx( 10.0 ) );
    REQUIRE( distanceArea.segmentLength() == Catch::Approx( 10.0 ) );

    distanceArea.setUseZValues( true );

    REQUIRE( distanceArea.length() == Catch::Approx( std::sqrt( 200.0 ) ) );
    REQUIRE( distanceArea.segmentLength() == Catch::Approx( std::sqrt( 200.0 ) ) );
  }

  SECTION( "Area" )
  {
    model.setGeometryType( Qgis::GeometryType::Polygon );
    model.addVertexFromPoint( QgsPoint( 0, 0, 0 ) );
    model.addVertexFromPoint( QgsPoint( 10, 0, 0 ) );
    model.setCurrentCoordinate( QgsPoint( 0, 10, 10 ) );

    REQUIRE( distanceArea.area() == Catch::Approx( 50.0 ) );
    REQUIRE( distanceArea.perimeter() == Catch::Approx( 20.0 + std::sqrt( 200.0 ) ) );

    distanceArea.setUseZValues( true );

    REQUIRE( distanceArea.area() == Catch::Approx( 50.0 * std::sqrt( 2.0 ) ) );
    REQUIRE( distanceArea.perimeter() == Catch::Approx( 10.0 + std::sqrt( 300.0 ) + std::sqrt( 200.0 ) ) );
  }
}
