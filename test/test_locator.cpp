/***************************************************************************
                        test_locator.cpp
                        --------------------
  begin                : July 2026
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
#include "expressioncalculatorlocatorfilter.h"
#include "gotolocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"

#include <qgscoordinatereferencesystem.h>
#include <qgsfeedback.h>
#include <qgslocatorcontext.h>
#include <qgspointxy.h>
#include <qgsproject.h>
#include <qgsrectangle.h>

using Catch::Approx;

static QList<QgsLocatorResult> fetchResults( QgsLocatorFilter *filter, const QString &string, const QgsLocatorContext &context = QgsLocatorContext() )
{
  QList<QgsLocatorResult> results;
  const QMetaObject::Connection connection = QObject::connect( filter, &QgsLocatorFilter::resultFetched, filter, [&results]( QgsLocatorResult result ) {
    results << result;
  } );
  QgsFeedback feedback;
  filter->fetchResults( string, context, &feedback );
  QObject::disconnect( connection );
  return results;
}

static bool hasAction( const QgsLocatorResult &result, int actionId )
{
  for ( const QgsLocatorResult::ResultAction &action : result.actions )
  {
    if ( action.id == actionId )
      return true;
  }
  return false;
}

/*
 * ExpressionCalculatorLocatorFilter
 * The trigger path copies to the clipboard through QGuiApplication, which is not
 * available in this gui less harness, so only fetching is exercised
 */
TEST_CASE( "ExpressionCalculatorLocatorFilter" )
{
  LocatorModelSuperBridge bridge;
  ExpressionCalculatorLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "calculator" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "=" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Highest );
    REQUIRE( filter.flags().testFlag( QgsLocatorFilter::FlagFast ) );
    REQUIRE( !filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<ExpressionCalculatorLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "EvaluatesExpression" )
  {
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "1 + 1" ) );
    REQUIRE( results.size() == 1 );
    REQUIRE( results.at( 0 ).userData().toString() == QStringLiteral( "2" ) );
    REQUIRE( results.at( 0 ).score == 1 );

    const QList<QgsLocatorResult> stringResults = fetchResults( &filter, QStringLiteral( "upper('abc')" ) );
    REQUIRE( stringResults.size() == 1 );
    REQUIRE( stringResults.at( 0 ).userData().toString() == QStringLiteral( "ABC" ) );
  }

  SECTION( "NoResultOnInvalidExpression" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "this is not an expression" ) ).isEmpty() );
  }
}

/*
 * GotoLocatorFilter
 */
TEST_CASE( "GotoLocatorFilter" )
{
  LocatorModelSuperBridge bridge;

  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
  mapSettings.setOutputSize( QSize( 1000, 500 ) );
  mapSettings.setExtent( QgsRectangle( -10.0, -10.0, 10.0, 10.0 ) );
  bridge.setMapSettings( &mapSettings );

  GotoLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "goto" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "go" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Medium );
    REQUIRE( filter.flags().testFlag( QgsLocatorFilter::FlagFast ) );
    REQUIRE( filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<GotoLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "ParsesCoordinatePairs" )
  {
    for ( const QString &string : { QStringLiteral( "1.5 2.5" ), QStringLiteral( "1.5,2.5" ), QStringLiteral( "geo:1.5,2.5" ) } )
    {
      const QList<QgsLocatorResult> results = fetchResults( &filter, string );
      REQUIRE( results.size() == 1 );

      const QgsPointXY point = results.at( 0 ).userData().toMap().value( QStringLiteral( "point" ) ).value<QgsPointXY>();
      REQUIRE( point.x() == Approx( 2.5 ) );
      REQUIRE( point.y() == Approx( 1.5 ) );
      REQUIRE( results.at( 0 ).score == Approx( 1.0 ) );
    }
  }

  SECTION( "ParsesCardinalSuffixes" )
  {
    const QList<QgsLocatorResult> northingFirst = fetchResults( &filter, QStringLiteral( "1.5N 2.5E" ) );
    REQUIRE( northingFirst.size() == 1 );
    QgsPointXY point = northingFirst.at( 0 ).userData().toMap().value( QStringLiteral( "point" ) ).value<QgsPointXY>();
    REQUIRE( point.x() == Approx( 2.5 ) );
    REQUIRE( point.y() == Approx( 1.5 ) );

    const QList<QgsLocatorResult> eastingFirst = fetchResults( &filter, QStringLiteral( "2.5E 1.5N" ) );
    REQUIRE( eastingFirst.size() == 1 );
    point = eastingFirst.at( 0 ).userData().toMap().value( QStringLiteral( "point" ) ).value<QgsPointXY>();
    REQUIRE( point.x() == Approx( 2.5 ) );
    REQUIRE( point.y() == Approx( 1.5 ) );
  }

  SECTION( "ParsesDegreeMinuteSecond" )
  {
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "1°30'0\"N 2°30'0\"E" ) );
    REQUIRE( results.size() == 1 );

    const QgsPointXY point = results.at( 0 ).userData().toMap().value( QStringLiteral( "point" ) ).value<QgsPointXY>();
    REQUIRE( point.x() == Approx( 2.5 ) );
    REQUIRE( point.y() == Approx( 1.5 ) );
  }

  SECTION( "ProvidesNavigationAction" )
  {
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "1.5 2.5" ) );
    REQUIRE( results.size() == 1 );
    REQUIRE( hasAction( results.at( 0 ), GotoLocatorFilter::Navigation ) );
  }

  SECTION( "NoResultOnInvalidInput" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "not a coordinate" ) ).isEmpty() );
    REQUIRE( fetchResults( &filter, QString() ).isEmpty() );
  }

  SECTION( "NoResultOutsideWgs84Bounds" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "200 200" ) ).isEmpty() );
  }

  SECTION( "EmitsMapCrsResultOnNonWgs84Map" )
  {
    QgsQuickMapSettings projectedSettings;
    projectedSettings.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    projectedSettings.setOutputSize( QSize( 1000, 500 ) );
    projectedSettings.setExtent( QgsRectangle( -1000.0, -1000.0, 1000.0, 1000.0 ) );

    LocatorModelSuperBridge projectedBridge;
    projectedBridge.setMapSettings( &projectedSettings );
    GotoLocatorFilter projectedFilter( &projectedBridge );

    const QList<QgsLocatorResult> results = fetchResults( &projectedFilter, QStringLiteral( "1.5 2.5" ) );
    REQUIRE( results.size() == 2 );
    REQUIRE( results.at( 0 ).score == Approx( 0.9 ) );
    REQUIRE( results.at( 1 ).score == Approx( 1.0 ) );

    const QgsPointXY mapCrsPoint = results.at( 0 ).userData().toMap().value( QStringLiteral( "point" ) ).value<QgsPointXY>();
    REQUIRE( mapCrsPoint.x() == Approx( 1.5 ) );
    REQUIRE( mapCrsPoint.y() == Approx( 2.5 ) );
  }
}
