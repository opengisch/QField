/***************************************************************************
                        test_cogo.cpp
                        --------------------
  begin                : August 2026
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
#include "cogoexecutor.h"
#include "cogooperation.h"
#include "cogooperationsmodel.h"
#include "cogoregistry.h"
#include "qgsquickmapsettings.h"
#include "rubberbandmodel.h"

#include <QSignalSpy>
#include <qgscoordinatereferencesystem.h>
#include <qgsgeometryutils.h>
#include <qgspoint.h>
#include <qgsproject.h>
#include <qgsrectangle.h>
#include <qgsvectorlayer.h>

#include <cmath>

using Catch::Approx;

/* The registry is a process-wide singleton the executor and operations model
 * both reach through CogoRegistry::instance(). The app sets it up once at
 * startup; a single shared instance mirrors that for the whole test run. */
static CogoRegistry *cogoRegistry()
{
  static CogoRegistry sRegistry;
  if ( CogoRegistry::instance() != &sRegistry )
  {
    CogoRegistry::setInstance( &sRegistry );
  }
  return &sRegistry;
}

static QVariant pointValue( double x, double y, double z = std::numeric_limits<double>::quiet_NaN() )
{
  return std::isnan( z ) ? QVariant::fromValue( QgsPoint( x, y ) ) : QVariant::fromValue( QgsPoint( x, y, z ) );
}

/*
 * CogoRegistry
 */
TEST_CASE( "CogoRegistry" )
{
  CogoRegistry *registry = cogoRegistry();

  SECTION( "registers the built-in operations" )
  {
    const QStringList operations = registry->availableOperations();
    REQUIRE( operations.contains( QStringLiteral( "point_at_xyz" ) ) );
    REQUIRE( operations.contains( QStringLiteral( "point_at_distance_angle" ) ) );
    REQUIRE( operations.contains( QStringLiteral( "point_at_intersection_circles" ) ) );
  }

  SECTION( "returns an operation by name and null for the unknown" )
  {
    REQUIRE( registry->operation( QStringLiteral( "point_at_xyz" ) ) != nullptr );
    REQUIRE( registry->operation( QStringLiteral( "does_not_exist" ) ) == nullptr );
  }

  SECTION( "rejects a duplicate registration" )
  {
    const int before = registry->availableOperations().count();
    REQUIRE( !registry->registerOperation( new CogoOperationPointAtXYZ() ) );
    REQUIRE( registry->availableOperations().count() == before );
  }
}

/*
 * CogoOperationPointAtXYZ
 */
TEST_CASE( "CogoOperationPointAtXYZ" )
{
  cogoRegistry();
  CogoOperationPointAtXYZ operation;

  SECTION( "exposes metadata" )
  {
    REQUIRE( operation.name() == QStringLiteral( "point_at_xyz" ) );
    REQUIRE( !operation.displayName().isEmpty() );
    REQUIRE( !operation.icon().isEmpty() );
  }

  SECTION( "parameters reflect the geometry's Z ability" )
  {
    const QList<CogoParameter> flat = operation.parameters( Qgis::WkbType::Point );
    REQUIRE( flat.size() == 1 );
    REQUIRE( flat.at( 0 ).name == QStringLiteral( "point" ) );
    REQUIRE( flat.at( 0 ).configuration.value( QStringLiteral( "hasZ" ) ).toBool() == false );

    const QList<CogoParameter> withZ = operation.parameters( Qgis::WkbType::PointZ );
    REQUIRE( withZ.at( 0 ).configuration.value( QStringLiteral( "hasZ" ) ).toBool() == true );
  }

  SECTION( "readiness requires a valid point" )
  {
    REQUIRE( !operation.checkReadiness( QVariantMap() ) );
    REQUIRE( !operation.checkReadiness( { { QStringLiteral( "point" ), QStringLiteral( "not a point" ) } } ) );
    REQUIRE( !operation.checkReadiness( { { QStringLiteral( "point" ), QVariant::fromValue( QgsPoint() ) } } ) );
    REQUIRE( operation.checkReadiness( { { QStringLiteral( "point" ), pointValue( 3.0, 4.0 ) } } ) );
  }

  SECTION( "execute adds a vertex at the point" )
  {
    RubberbandModel rubberbandModel;
    const int before = rubberbandModel.vertexCount();

    REQUIRE( operation.execute( &rubberbandModel, { { QStringLiteral( "point" ), pointValue( 3.0, 4.0 ) } }, Qgis::WkbType::Point ) );
    REQUIRE( rubberbandModel.vertexCount() == before + 1 );

    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 3.0 ) );
    REQUIRE( added.y() == Approx( 4.0 ) );
  }

  SECTION( "execute fails without a rubberband or when not ready" )
  {
    RubberbandModel rubberbandModel;
    REQUIRE( !operation.execute( nullptr, { { QStringLiteral( "point" ), pointValue( 1.0, 1.0 ) } }, Qgis::WkbType::Point ) );
    REQUIRE( !operation.execute( &rubberbandModel, QVariantMap(), Qgis::WkbType::Point ) );
  }

  SECTION( "executing the same point twice does not duplicate the vertex" )
  {
    RubberbandModel rubberbandModel;
    const QVariantMap parameters = { { QStringLiteral( "point" ), pointValue( 3.0, 4.0 ) } };

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    const int afterFirst = rubberbandModel.vertexCount();
    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    REQUIRE( rubberbandModel.vertexCount() == afterFirst );
  }
}
