/***************************************************************************
                        test_expressionevaluator.h
                        --------------------
  begin                : Aug 2024
  copyright            : (C) 2024 by Mathieu Pellerin
  email                : mathieu at opengis dot ch
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
#include "expressionevaluator.h"
#include "positioningutils.h"

#include <QTemporaryFile>
#include <qgsvectorlayer.h>

TEST_CASE( "ExpressionEvaluator" )
{
  ExpressionEvaluator evaluator;

  QgsProject project;
  project.setTitle( QStringLiteral( "QField rocks!" ) );
  evaluator.setProject( &project );

  QgsVectorLayer layer( QStringLiteral( "Point?crs=EPSG:4326&field=test_field:string(255,0)" ), QStringLiteral( "test" ), QStringLiteral( "memory" ) );
  QgsFeature feature( layer.fields() );
  feature.setAttribute( QStringLiteral( "test_field" ), QStringLiteral( "success" ) );
  evaluator.setLayer( &layer );
  evaluator.setFeature( feature );

  GnssPositionInformation positionInformation = PositioningUtils::createGnssPositionInformation( 1.234, 1.234, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, QDateTime(), QStringLiteral( "test" ) );
  evaluator.setPositionInformation( positionInformation );

  CloudUserInformation cloudUserInformation( QStringLiteral( "nyuki" ), QStringLiteral( "nyuki@opengis.ch" ) );
  evaluator.setCloudUserInformation( cloudUserInformation );

  SECTION( "Expression mode" )
  {
    evaluator.setMode( ExpressionEvaluator::ExpressionMode );
    evaluator.setExpressionText( "10 + 10" );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "20" ) );

    evaluator.setExpressionText( QStringLiteral( "@project_title" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "QField rocks!" ) );

    evaluator.setExpressionText( QStringLiteral( "\"test_field\"" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "success" ) );

    evaluator.setExpressionText( QStringLiteral( "x(@gnss_coordinate)" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "1.234" ) );

    evaluator.setExpressionText( QStringLiteral( "@cloud_username || ' - ' || @cloud_useremail" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "nyuki - nyuki@opengis.ch" ) );
  }

  SECTION( "Expression template mode" )
  {
    evaluator.setMode( ExpressionEvaluator::ExpressionTemplateMode );
    evaluator.setExpressionText( QStringLiteral( "Result: [%10 + 10%]" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "Result: 20" ) );

    evaluator.setExpressionText( QStringLiteral( "[%@project_title%] What a title!" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "QField rocks! What a title!" ) );

    evaluator.setExpressionText( QStringLiteral( "This is [% \"test_field\" %]ful" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "This is successful" ) );

    evaluator.setExpressionText( QStringLiteral( "[%x(@gnss_coordinate)%]" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "1.234" ) );

    evaluator.setExpressionText( QStringLiteral( "User: [%@cloud_username%]\nEmail: [%@cloud_useremail%]" ) );
    REQUIRE( evaluator.evaluate() == QStringLiteral( "User: nyuki\nEmail: nyuki@opengis.ch" ) );
  }
}
