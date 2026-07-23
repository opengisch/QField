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
#include "locatormodelsuperbridge.h"

#include <qgsfeedback.h>
#include <qgslocatorcontext.h>
#include <qgsproject.h>

//! Collects the results emitted by a filter for a given search string.
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
