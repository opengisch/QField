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

#include "digitizinglogger.h"
#include "qfield_testbase.h"

#include <qgscoordinatereferencesystem.h>

#include <QtTest>


class TestDigitizingLogger : public QObject
{
    Q_OBJECT

  private slots:

    void initTestCase()
    {
      mLogsLayer.reset( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=digitizing_action:string&field=digitizing_layer_name:string&field=digitizing_layer_id:string&field=digitizing_datetime:datetime" ), QStringLiteral( "Logs Layer" ), QStringLiteral( "memory" ) ) );
      QVERIFY( mLogsLayer->isValid() );

      mLogsLayer->setDefaultValueDefinition( 1, QgsDefaultValue( QStringLiteral( "@digitizing_type" ), false ) );
      mLogsLayer->setDefaultValueDefinition( 2, QgsDefaultValue( QStringLiteral( "@digitizing_layer_name" ), false ) );
      mLogsLayer->setDefaultValueDefinition( 3, QgsDefaultValue( QStringLiteral( "@digitizing_layer_id" ), false ) );
      mLogsLayer->setDefaultValueDefinition( 4, QgsDefaultValue( QStringLiteral( "@digitizing_datetime" ), false ) );

      QgsProject::instance()->addMapLayer( mLogsLayer.get() );
      QgsProject::instance()->setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
      QgsProject::instance()->writeEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "digitizingLogsLayer" ), mLogsLayer->id() );
      qDebug() << mLogsLayer->id();

      mLayer.reset( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=str:string" ), QStringLiteral( "Input Layer" ), QStringLiteral( "memory" ) ) );
      QVERIFY( mLayer->isValid() );

      mDigitizingLogger.reset( new DigitizingLogger() );
      mDigitizingLogger->setType( QStringLiteral( "rock" ) );
      mDigitizingLogger->setProject( QgsProject::instance() );
      mDigitizingLogger->setDigitizingLayer( mLayer.get() );
    }

    void testAddAndWritePoints()
    {
      mDigitizingLogger->clearCoordinates();
      mDigitizingLogger->addCoordinate( QgsPoint( 1, 1 ) );
      mDigitizingLogger->addCoordinate( QgsPoint( 2, 2 ) );
      mDigitizingLogger->writeCoordinates();
      QCOMPARE( mLogsLayer->featureCount(), 2 );
      mDigitizingLogger->addCoordinate( QgsPoint( 1, 1 ) );
      mDigitizingLogger->addCoordinate( QgsPoint( 2, 2 ) );
      mDigitizingLogger->removeLastCoordinate();
      mDigitizingLogger->writeCoordinates();
      QCOMPARE( mLogsLayer->featureCount(), 3 );

      QgsFeature feature = mLogsLayer->getFeature( 1 );
      QCOMPARE( feature.attributes().at( 1 ), mDigitizingLogger->type() );
      QCOMPARE( feature.attributes().at( 2 ), mLayer->name() );
      QCOMPARE( feature.attributes().at( 3 ), mLayer->id() );
      QCOMPARE( feature.attributes().at( 4 ).toDateTime().isValid(), true );
    }

  private:

    std::unique_ptr<DigitizingLogger> mDigitizingLogger;

    std::unique_ptr<QgsVectorLayer> mLogsLayer;
    std::unique_ptr<QgsVectorLayer> mLayer;
};

QFIELDTEST_MAIN( TestDigitizingLogger )
#include "test_digitizinglogger.moc"
