/***************************************************************************
                        test_layerobserver.h
                        --------------------
  begin                : Apr 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
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
#include "digitizinglogger.h"

#include <qgscoordinatereferencesystem.h>


TEST_CASE( "DigitizingLogger" )
{
  std::unique_ptr<QgsVectorLayer> logsLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=digitizing_action:string&field=digitizing_layer_name:string&field=digitizing_layer_id:string&field=digitizing_datetime:datetime" ), QStringLiteral( "Logs Layer" ), QStringLiteral( "memory" ) );
  REQUIRE( logsLayer->isValid() );

  logsLayer->setDefaultValueDefinition( 1, QgsDefaultValue( QStringLiteral( "@digitizing_type" ), false ) );
  logsLayer->setDefaultValueDefinition( 2, QgsDefaultValue( QStringLiteral( "@digitizing_layer_name" ), false ) );
  logsLayer->setDefaultValueDefinition( 3, QgsDefaultValue( QStringLiteral( "@digitizing_layer_id" ), false ) );
  logsLayer->setDefaultValueDefinition( 4, QgsDefaultValue( QStringLiteral( "@digitizing_datetime" ), false ) );

  QgsProject::instance()->addMapLayer( logsLayer.get() );
  QgsProject::instance()->setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
  QgsProject::instance()->writeEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "digitizingLogsLayer" ), logsLayer->id() );
  qDebug() << logsLayer->id();

  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=str:string" ), QStringLiteral( "Input Layer" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  std::unique_ptr<DigitizingLogger> digitizingLogger = std::make_unique<DigitizingLogger>();
  digitizingLogger->setType( QStringLiteral( "rock" ) );
  digitizingLogger->setProject( QgsProject::instance() );
  digitizingLogger->setDigitizingLayer( layer.get() );


  SECTION( "AddAndWritePoints" )
  {
    digitizingLogger->clearCoordinates();
    digitizingLogger->addCoordinate( QgsPoint( 1, 1 ) );
    digitizingLogger->addCoordinate( QgsPoint( 2, 2 ) );
    digitizingLogger->writeCoordinates();
    REQUIRE( logsLayer->featureCount() == 2 );
    digitizingLogger->addCoordinate( QgsPoint( 1, 1 ) );
    digitizingLogger->addCoordinate( QgsPoint( 2, 2 ) );
    digitizingLogger->removeLastCoordinate();
    digitizingLogger->writeCoordinates();
    REQUIRE( logsLayer->featureCount() == 3 );

    QgsFeature feature = logsLayer->getFeature( 1 );
    REQUIRE( feature.attributes().at( 1 ) == digitizingLogger->type() );
    REQUIRE( feature.attributes().at( 2 ) == layer->name() );
    REQUIRE( feature.attributes().at( 3 ) == layer->id() );
    REQUIRE( feature.attributes().at( 4 ).toDateTime().isValid() == true );
  }
}
