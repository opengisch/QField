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
#include "qfcogoexecutor.h"
#include "qfcogooperation.h"
#include "qfcogooperationsmodel.h"
#include "qfcogoregistry.h"
#include "qfrubberbandmodel.h"
#include "qgsquickmapsettings.h"

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

static CogoParameter parameterByName( const QList<CogoParameter> &parameters, const QString &name )
{
  for ( const CogoParameter &parameter : parameters )
  {
    if ( parameter.name == name )
      return parameter;
  }
  return CogoParameter();
}

class CogoOperationDummy : public CogoOperation
{
  public:
    CogoOperationDummy() {}
    ~CogoOperationDummy() = default;

    QString name() const override { return QStringLiteral( "dummy" ); }
};

/*
 * CogoRegistry
 */
TEST_CASE( "CogoRegistry" )
{
  CogoRegistry registry;

  SECTION( "registers the built-in operations" )
  {
    const QStringList operations = registry.availableOperations();
    REQUIRE( operations.contains( QStringLiteral( "point_at_xyz" ) ) );
    REQUIRE( operations.contains( QStringLiteral( "point_at_distance_angle" ) ) );
    REQUIRE( operations.contains( QStringLiteral( "point_at_intersection_circles" ) ) );
  }

  SECTION( "returns the requested operation and null for the unknown" )
  {
    CogoOperation *operation = registry.operation( QStringLiteral( "point_at_xyz" ) );
    REQUIRE( operation != nullptr );
    REQUIRE( operation->name() == QStringLiteral( "point_at_xyz" ) );
    REQUIRE( registry.operation( QStringLiteral( "does_not_exist" ) ) == nullptr );
  }

  SECTION( "registers a new operation" )
  {
    const int before = registry.availableOperations().count();
    REQUIRE( registry.registerOperation( new CogoOperationDummy() ) );
    REQUIRE( registry.availableOperations().count() == before + 1 );
    REQUIRE( registry.availableOperations().contains( QStringLiteral( "dummy" ) ) );
    REQUIRE( registry.operation( QStringLiteral( "dummy" ) ) != nullptr );
  }

  SECTION( "rejects a duplicate registration" )
  {
    const int before = registry.availableOperations().count();
    REQUIRE( !registry.registerOperation( new CogoOperationPointAtXYZ() ) );
    REQUIRE( registry.availableOperations().count() == before );
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
    const CogoParameter flatPoint = parameterByName( operation.parameters( Qgis::WkbType::Point ), QStringLiteral( "point" ) );
    REQUIRE( flatPoint.name == QStringLiteral( "point" ) );
    REQUIRE( flatPoint.configuration.value( QStringLiteral( "hasZ" ) ).toBool() == false );

    const CogoParameter zPoint = parameterByName( operation.parameters( Qgis::WkbType::PointZ ), QStringLiteral( "point" ) );
    REQUIRE( zPoint.configuration.value( QStringLiteral( "hasZ" ) ).toBool() == true );
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

/*
 * CogoOperationPointAtDistanceAngle
 * Angle is measured clockwise from north: 0 deg points +Y, 90 deg points +X.
 */
TEST_CASE( "CogoOperationPointAtDistanceAngle" )
{
  cogoRegistry();
  CogoOperationPointAtDistanceAngle operation;

  SECTION( "parameters include elevation only with Z" )
  {
    const QList<CogoParameter> flat = operation.parameters( Qgis::WkbType::Point );
    REQUIRE( parameterByName( flat, QStringLiteral( "point" ) ).name == QStringLiteral( "point" ) );
    REQUIRE( parameterByName( flat, QStringLiteral( "distance" ) ).name == QStringLiteral( "distance" ) );
    REQUIRE( parameterByName( flat, QStringLiteral( "angle" ) ).name == QStringLiteral( "angle" ) );
    REQUIRE( parameterByName( flat, QStringLiteral( "elevation" ) ).name.isEmpty() );

    const QList<CogoParameter> withZ = operation.parameters( Qgis::WkbType::PointZ );
    REQUIRE( parameterByName( withZ, QStringLiteral( "elevation" ) ).name == QStringLiteral( "elevation" ) );
  }

  SECTION( "readiness requires a numeric distance and angle" )
  {
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0 );
    REQUIRE( !operation.checkReadiness( parameters ) );

    parameters[QStringLiteral( "distance" )] = QStringLiteral( "ten" );
    parameters[QStringLiteral( "angle" )] = 90.0;
    REQUIRE( !operation.checkReadiness( parameters ) );

    parameters[QStringLiteral( "distance" )] = 10.0;
    REQUIRE( operation.checkReadiness( parameters ) );
  }

  SECTION( "readiness requires an elevation with Z" )
  {
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0, 5.0 );
    parameters[QStringLiteral( "distance" )] = 10.0;
    parameters[QStringLiteral( "angle" )] = 90.0;
    REQUIRE( !operation.checkReadiness( parameters, Qgis::WkbType::PointZ ) );

    parameters[QStringLiteral( "elevation" )] = 2.0;
    REQUIRE( operation.checkReadiness( parameters, Qgis::WkbType::PointZ ) );
  }

  SECTION( "execute places a vertex east at 90 degrees" )
  {
    RubberbandModel rubberbandModel;
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0 );
    parameters[QStringLiteral( "distance" )] = 10.0;
    parameters[QStringLiteral( "angle" )] = 90.0;

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 10.0 ) );
    REQUIRE( added.y() == Approx( 0.0 ).margin( 1e-9 ) );
  }

  SECTION( "execute places a vertex north at 0 degrees" )
  {
    RubberbandModel rubberbandModel;
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0 );
    parameters[QStringLiteral( "distance" )] = 10.0;
    parameters[QStringLiteral( "angle" )] = 0.0;

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 0.0 ).margin( 1e-9 ) );
    REQUIRE( added.y() == Approx( 10.0 ) );
  }

  SECTION( "an angle past a full turn wraps around" )
  {
    RubberbandModel rubberbandModel;
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0 );
    parameters[QStringLiteral( "distance" )] = 10.0;
    parameters[QStringLiteral( "angle" )] = 450.0;

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 10.0 ) );
    REQUIRE( added.y() == Approx( 0.0 ).margin( 1e-9 ) );
  }

  SECTION( "a zero distance keeps the vertex on the source point" )
  {
    RubberbandModel rubberbandModel;
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 5.0, 5.0 );
    parameters[QStringLiteral( "distance" )] = 0.0;
    parameters[QStringLiteral( "angle" )] = 90.0;

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::Point ) );
    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 5.0 ) );
    REQUIRE( added.y() == Approx( 5.0 ) );
  }

  SECTION( "execute offsets the elevation" )
  {
    RubberbandModel rubberbandModel;
    QVariantMap parameters;
    parameters[QStringLiteral( "point" )] = pointValue( 0.0, 0.0, 100.0 );
    parameters[QStringLiteral( "distance" )] = 10.0;
    parameters[QStringLiteral( "angle" )] = 90.0;
    parameters[QStringLiteral( "elevation" )] = 2.5;

    REQUIRE( operation.execute( &rubberbandModel, parameters, Qgis::WkbType::PointZ ) );
    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.z() == Approx( 102.5 ) );
  }
}

/*
 * CogoOperationPointAtIntersectionCircles
 * Circles centred at (0,0) r=5 and (8,0) r=5 intersect at (4, +/-3).
 */
TEST_CASE( "CogoOperationPointAtIntersectionCircles" )
{
  cogoRegistry();
  CogoOperationPointAtIntersectionCircles operation;

  auto intersectingParameters = []() {
    QVariantMap parameters;
    parameters[QStringLiteral( "point1" )] = pointValue( 0.0, 0.0 );
    parameters[QStringLiteral( "distance1" )] = 5.0;
    parameters[QStringLiteral( "point2" )] = pointValue( 8.0, 0.0 );
    parameters[QStringLiteral( "distance2" )] = 5.0;
    return parameters;
  };

  SECTION( "parameters expose the candidate toggle" )
  {
    const CogoParameter candidate = parameterByName( operation.parameters( Qgis::WkbType::Point ), QStringLiteral( "candidate" ) );
    REQUIRE( candidate.name == QStringLiteral( "candidate" ) );
    REQUIRE( candidate.type == QStringLiteral( "enum" ) );
    REQUIRE( candidate.configuration.value( QStringLiteral( "options" ) ).toStringList() == QStringList( { QStringLiteral( "A" ), QStringLiteral( "B" ) } ) );
  }

  SECTION( "not ready while a circle is missing" )
  {
    QVariantMap parameters;
    parameters[QStringLiteral( "point1" )] = pointValue( 0.0, 0.0 );
    parameters[QStringLiteral( "distance1" )] = 5.0;
    REQUIRE( !operation.checkReadiness( parameters ) );
  }

  SECTION( "not ready when the circles do not intersect" )
  {
    QVariantMap parameters = intersectingParameters();
    parameters[QStringLiteral( "distance1" )] = 1.0;
    parameters[QStringLiteral( "distance2" )] = 1.0;
    REQUIRE( !operation.checkReadiness( parameters ) );
  }

  SECTION( "ready when the circles intersect" )
  {
    REQUIRE( operation.checkReadiness( intersectingParameters() ) );
  }

  SECTION( "execute defaults to candidate A" )
  {
    RubberbandModel rubberbandModel;
    REQUIRE( operation.execute( &rubberbandModel, intersectingParameters(), Qgis::WkbType::Point ) );

    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 4.0 ) );
    REQUIRE( std::abs( added.y() ) == Approx( 3.0 ) );
  }

  SECTION( "candidate selection picks the mirrored intersection" )
  {
    RubberbandModel rubberbandModelA;
    QVariantMap parametersA = intersectingParameters();
    parametersA[QStringLiteral( "candidate" )] = QStringLiteral( "A" );
    REQUIRE( operation.execute( &rubberbandModelA, parametersA, Qgis::WkbType::Point ) );
    const QgsPoint pointA = rubberbandModelA.vertexAt( 0 );

    RubberbandModel rubberbandModelB;
    QVariantMap parametersB = intersectingParameters();
    parametersB[QStringLiteral( "candidate" )] = QStringLiteral( "B" );
    REQUIRE( operation.execute( &rubberbandModelB, parametersB, Qgis::WkbType::Point ) );
    const QgsPoint pointB = rubberbandModelB.vertexAt( 0 );

    REQUIRE( pointA.x() == Approx( pointB.x() ) );
    REQUIRE( pointA.y() == Approx( -pointB.y() ) );
    REQUIRE( pointA.y() != Approx( pointB.y() ) );
  }
}

/*
 * CogoExecutor
 */
TEST_CASE( "CogoExecutor" )
{
  cogoRegistry();

  QgsVectorLayer *layer = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  RubberbandModel rubberbandModel;
  rubberbandModel.setVectorLayer( layer );

  CogoExecutor executor;
  executor.setRubberbandModel( &rubberbandModel );

  SECTION( "setting the name loads parameters and clears values" )
  {
    QSignalSpy parametersSpy( &executor, &CogoExecutor::parametersChanged );
    executor.setName( QStringLiteral( "point_at_distance_angle" ) );

    REQUIRE( executor.name() == QStringLiteral( "point_at_distance_angle" ) );
    REQUIRE( parametersSpy.count() >= 1 );
    REQUIRE( !executor.parameters().isEmpty() );
    REQUIRE( executor.parameterValues().isEmpty() );
  }

  SECTION( "readiness tracks the parameter values" )
  {
    executor.setName( QStringLiteral( "point_at_xyz" ) );
    REQUIRE( !executor.isReady() );

    QSignalSpy readySpy( &executor, &CogoExecutor::isReadyChanged );
    executor.setParameterValues( { { QStringLiteral( "point" ), pointValue( 1.0, 2.0 ) } } );
    REQUIRE( executor.isReady() );
    REQUIRE( readySpy.count() == 1 );
  }

  SECTION( "execute adds a vertex through the rubberband" )
  {
    executor.setName( QStringLiteral( "point_at_xyz" ) );
    executor.setParameterValues( { { QStringLiteral( "point" ), pointValue( 7.0, 8.0 ) } } );

    const int before = rubberbandModel.vertexCount();
    REQUIRE( executor.execute() );
    REQUIRE( rubberbandModel.vertexCount() == before + 1 );

    const QgsPoint added = rubberbandModel.vertexAt( 0 );
    REQUIRE( added.x() == Approx( 7.0 ) );
    REQUIRE( added.y() == Approx( 8.0 ) );
  }

  SECTION( "execute fails for an unknown operation" )
  {
    executor.setName( QStringLiteral( "no_such_operation" ) );
    executor.setParameterValues( { { QStringLiteral( "point" ), pointValue( 1.0, 1.0 ) } } );
    REQUIRE( !executor.execute() );
  }

  SECTION( "visual guides require map settings" )
  {
    executor.setName( QStringLiteral( "point_at_xyz" ) );
    executor.setParameterValues( { { QStringLiteral( "point" ), pointValue( 1.0, 1.0 ) } } );
    REQUIRE( executor.visualGuides().isEmpty() );

    QgsQuickMapSettings mapSettings;
    mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
    mapSettings.setOutputSize( QSize( 1000, 500 ) );
    mapSettings.setExtent( QgsRectangle( -10.0, -10.0, 10.0, 10.0 ) );

    QSignalSpy guidesSpy( &executor, &CogoExecutor::visualGuidesChanged );
    executor.setMapSettings( &mapSettings );
    REQUIRE( guidesSpy.count() >= 1 );
    REQUIRE( executor.visualGuides().size() == 1 );
    REQUIRE( executor.visualGuides().at( 0 ).type == CogoVisualGuide::Point );
  }

  SECTION( "visual guides regenerate when the map extent moves" )
  {
    QgsQuickMapSettings mapSettings;
    mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
    mapSettings.setOutputSize( QSize( 1000, 500 ) );
    mapSettings.setExtent( QgsRectangle( -10.0, -10.0, 10.0, 10.0 ) );

    executor.setName( QStringLiteral( "point_at_xyz" ) );
    executor.setMapSettings( &mapSettings );
    executor.setParameterValues( { { QStringLiteral( "point" ), pointValue( 1.0, 1.0 ) } } );

    const QPointF before = executor.visualGuides().at( 0 ).details.value( QStringLiteral( "point" ) ).toPointF();

    QSignalSpy guidesSpy( &executor, &CogoExecutor::visualGuidesChanged );
    mapSettings.setExtent( QgsRectangle( 0.0, 0.0, 20.0, 10.0 ) );
    REQUIRE( guidesSpy.count() >= 1 );

    const QPointF after = executor.visualGuides().at( 0 ).details.value( QStringLiteral( "point" ) ).toPointF();
    REQUIRE( before != after );
  }

  delete layer;
}

/*
 * CogoOperationsModel
 */
TEST_CASE( "CogoOperationsModel" )
{
  cogoRegistry();
  CogoOperationsModel model;

  SECTION( "rows match the registry" )
  {
    REQUIRE( model.rowCount() == cogoRegistry()->availableOperations().count() );
  }

  SECTION( "role names" )
  {
    const QHash<int, QByteArray> roles = model.roleNames();
    REQUIRE( roles.value( CogoOperationsModel::NameRole ) == QByteArray( "Name" ) );
    REQUIRE( roles.value( CogoOperationsModel::DisplayNameRole ) == QByteArray( "DisplayName" ) );
    REQUIRE( roles.value( CogoOperationsModel::IconRole ) == QByteArray( "Icon" ) );
  }

  SECTION( "get by name returns the operation data" )
  {
    const QVariantMap data = model.get( QStringLiteral( "point_at_xyz" ) );
    REQUIRE( data.value( QStringLiteral( "Name" ) ).toString() == QStringLiteral( "point_at_xyz" ) );
    REQUIRE( !data.value( QStringLiteral( "DisplayName" ) ).toString().isEmpty() );
    REQUIRE( !data.value( QStringLiteral( "Icon" ) ).toString().isEmpty() );
  }

  SECTION( "get by an invalid name returns empty" )
  {
    REQUIRE( model.get( QStringLiteral( "no_such_operation" ) ).isEmpty() );
  }
}
