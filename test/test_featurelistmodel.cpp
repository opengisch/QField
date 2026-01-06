/***************************************************************************
                        test_featurelistmodel
                        --------------------
  begin                : January 2026
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


#include "catch2.h"
#include "featurelistmodel.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include <qgsvectorlayer.h>

#include <memory>


TEST_CASE( "FeatureListModel validation checks" )
{
  FeatureListModel featureListModel;

  REQUIRE( featureListModel.rowCount() == 0 );
  REQUIRE_FALSE( featureListModel.getFeatureFromKeyValue( QVariant( 1 ) ).isValid() );
  REQUIRE_FALSE( featureListModel.getFeatureById( 1 ).isValid() );
  REQUIRE( featureListModel.findKey( QVariant( 1 ) ) == -1 );
  REQUIRE( featureListModel.findDisplayValueMatches( QStringLiteral( "t" ) ).isEmpty() );
}

TEST_CASE( "FeatureListModel behaviours" )
{
  if ( !QCoreApplication::instance() )
  {
    static int applicationArgumentCount = 1;
    static char applicationArgumentZero[] = "qfield-tests";
    static char *applicationArgumentValues[] = { applicationArgumentZero, nullptr };
    static QCoreApplication coreApplication( applicationArgumentCount, applicationArgumentValues );
    Q_UNUSED( coreApplication );
  }

  static const int maximumWaitTimeMilliseconds = 5000;

  std::unique_ptr<QgsVectorLayer> vectorLayerDummy(
    new QgsVectorLayer(
      "Point?crs=epsg:4326&field=key:integer&field=name:string&field=grp:string",
      "test",
      "memory" ) );

  REQUIRE( vectorLayerDummy->isValid() );

  QgsFeature featureOne( vectorLayerDummy->fields() );
  featureOne.setAttribute( "key", 2 );
  featureOne.setAttribute( "name", "One" );
  featureOne.setAttribute( "grp", "B" );
  featureOne.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 0, 0 ) ) );

  QgsFeature featureTwo( vectorLayerDummy->fields() );
  featureTwo.setAttribute( "key", 1 );
  featureTwo.setAttribute( "name", "Two" );
  featureTwo.setAttribute( "grp", "A" );
  featureTwo.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 1, 1 ) ) );

  QgsFeature featureThree( vectorLayerDummy->fields() );
  featureThree.setAttribute( "key", 3 );
  featureThree.setAttribute( "name", "Three" );
  featureThree.setAttribute( "grp", "A" );
  featureThree.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 2, 2 ) ) );

  vectorLayerDummy->startEditing();
  vectorLayerDummy->addFeature( featureOne );
  vectorLayerDummy->addFeature( featureTwo );
  vectorLayerDummy->addFeature( featureThree );
  vectorLayerDummy->commitChanges();

  FeatureListModel featureListModel;
  featureListModel.setKeyField( QStringLiteral( "key" ) );
  featureListModel.setDisplayValueField( QStringLiteral( "name" ) );
  featureListModel.setGroupField( QStringLiteral( "grp" ) );
  featureListModel.setCurrentLayer( vectorLayerDummy.get() );

  // Wait for initial population (timer and gatherer thread)
  {
    QElapsedTimer waitForPopulationTimer;
    waitForPopulationTimer.start();
    while ( featureListModel.rowCount() != 3 && waitForPopulationTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( featureListModel.rowCount() == 3 );
  }

  // Roles and lookups
  {
    const int rowIndexForKeyOne = featureListModel.findKey( QVariant( 1 ) );
    REQUIRE( rowIndexForKeyOne >= 0 );

    REQUIRE( featureListModel.dataFromRowIndex( rowIndexForKeyOne, FeatureListModel::KeyFieldRole ).toInt() == 1 );
    REQUIRE( featureListModel.dataFromRowIndex( rowIndexForKeyOne, FeatureListModel::DisplayStringRole ).toString() == QStringLiteral( "Two" ) );
    REQUIRE( featureListModel.dataFromRowIndex( rowIndexForKeyOne, FeatureListModel::GroupFieldRole ).toString() == QStringLiteral( "A" ) );

    const QgsFeature featureFetchedByKeyValue = featureListModel.getFeatureFromKeyValue( QVariant( 3 ) );
    REQUIRE( featureFetchedByKeyValue.isValid() );
    REQUIRE( featureFetchedByKeyValue.attribute( "name" ).toString() == QStringLiteral( "Three" ) );

    const QgsFeatureId featureIdForKeyOne = featureListModel.dataFromRowIndex( rowIndexForKeyOne, FeatureListModel::FeatureIdRole ).value<QgsFeatureId>();

    const QgsFeature featureFetchedById = featureListModel.getFeatureById( featureIdForKeyOne );
    REQUIRE( featureFetchedById.isValid() );
    REQUIRE( featureFetchedById.attribute( "key" ).toInt() == 1 );
  }

  // Prefix matching behaviour
  {
    const QList<int> matchedRowIndices = featureListModel.findDisplayValueMatches( QStringLiteral( " t " ) );
    REQUIRE( matchedRowIndices.size() == 2 );

    const QString firstMatchLower = featureListModel.dataFromRowIndex( matchedRowIndices.at( 0 ), FeatureListModel::DisplayStringRole ).toString().trimmed().toLower();
    const QString secondMatchLower = featureListModel.dataFromRowIndex( matchedRowIndices.at( 1 ), FeatureListModel::DisplayStringRole ).toString().trimmed().toLower();

    REQUIRE( firstMatchLower.startsWith( QStringLiteral( "t" ) ) );
    REQUIRE( secondMatchLower.startsWith( QStringLiteral( "t" ) ) );
  }

  // addNull behaviour
  {
    featureListModel.setAddNull( true );

    QElapsedTimer waitForNullTimer;
    waitForNullTimer.start();
    while ( featureListModel.rowCount() != 4 && waitForNullTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( featureListModel.rowCount() == 4 );
    REQUIRE( featureListModel.dataFromRowIndex( 0, FeatureListModel::DisplayStringRole ).toString() == QStringLiteral( "<i>NULL</i>" ) );

    // Current implementation, if key not found and addNull enabled -> returns 0
    REQUIRE( featureListModel.findKey( QVariant( 999 ) ) == 0 );

    featureListModel.setAddNull( false );
  }

  // filterExpression behaviour
  {
    featureListModel.setFilterExpression( QStringLiteral( "\"grp\" = 'A'" ) );

    QElapsedTimer waitForFilterTimer;
    waitForFilterTimer.start();
    while ( featureListModel.rowCount() != 2 && waitForFilterTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( featureListModel.rowCount() == 2 );
    REQUIRE( featureListModel.findKey( QVariant( 1 ) ) >= 0 );
    REQUIRE( featureListModel.findKey( QVariant( 3 ) ) >= 0 );
    REQUIRE( featureListModel.findKey( QVariant( 2 ) ) == -1 );

    featureListModel.setFilterExpression( QString() );

    QElapsedTimer waitForUnfilterTimer;
    waitForUnfilterTimer.start();
    while ( featureListModel.rowCount() != 3 && waitForUnfilterTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( featureListModel.rowCount() == 3 );
  }

  // orderByValue behaviour (wait for sorted first item, not rowCount)
  {
    featureListModel.setOrderByValue( true );

    QElapsedTimer waitForSortTimer;
    waitForSortTimer.start();
    while ( waitForSortTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      const QString firstRowDisplayLower = featureListModel.dataFromRowIndex( 0, FeatureListModel::DisplayStringRole ).toString().toLower();
      if ( firstRowDisplayLower == QStringLiteral( "three" ) )
      {
        break;
      }

      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( featureListModel.dataFromRowIndex( 0, FeatureListModel::DisplayStringRole ).toString().toLower() == QStringLiteral( "three" ) );
  }

  // Layer change triggers reload (featureAdded gives reload)
  {
    QgsFeature featureFour( vectorLayerDummy->fields() );
    featureFour.setAttribute( "key", 4 );
    featureFour.setAttribute( "name", "Four" );
    featureFour.setAttribute( "grp", "C" );
    featureFour.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 3, 3 ) ) );

    vectorLayerDummy->startEditing();
    vectorLayerDummy->addFeature( featureFour );
    vectorLayerDummy->commitChanges();

    QElapsedTimer waitForAddTimer;
    waitForAddTimer.start();
    while ( featureListModel.rowCount() != 4 && waitForAddTimer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( featureListModel.rowCount() == 4 );
    REQUIRE( featureListModel.findKey( QVariant( 4 ) ) >= 0 );
  }
}
