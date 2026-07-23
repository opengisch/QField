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
#include "activelayerfeatureslocatorfilter.h"
#include "bookmarklocatorfilter.h"
#include "bookmarkmodel.h"
#include "catch2.h"
#include "expressioncalculatorlocatorfilter.h"
#include "featurelistextentcontroller.h"
#include "featurelistmodelselection.h"
#include "featureslocatorfilter.h"
#include "gotolocatorfilter.h"
#include "helplocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "multifeaturelistmodel.h"
#include "qgsquickmapsettings.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QSignalSpy>
#include <qgsapplication.h>
#include <qgsbookmarkmanager.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeedback.h>
#include <qgslocatorcontext.h>
#include <qgspointxy.h>
#include <qgsproject.h>
#include <qgsrectangle.h>
#include <qgsreferencedgeometry.h>
#include <qgsvectorlayer.h>

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

static bool waitForRowCount( const MultiFeatureListModel &model, int expectedCount )
{
  QElapsedTimer timer;
  timer.start();
  while ( model.rowCount() != expectedCount && timer.elapsed() < 5000 )
  {
    QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
  }
  return model.rowCount() == expectedCount;
}

static QgsBookmark makeBookmark( const QString &name )
{
  QgsBookmark bookmark;
  bookmark.setName( name );
  bookmark.setExtent( QgsReferencedRectangle( QgsRectangle( 0.0, 0.0, 10.0, 10.0 ), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) ) );
  return bookmark;
}

static QgsVectorLayer *addPointLayer( const QString &name )
{
  QgsVectorLayer *layer = new QgsVectorLayer( QStringLiteral( "Point?crs=epsg:4326&field=fid:integer&field=str:string" ), name, QStringLiteral( "memory" ) );
  layer->setDisplayExpression( QStringLiteral( "\"str\"" ) );

  QgsFeature alpha( layer->fields() );
  alpha.setAttribute( QStringLiteral( "fid" ), 1 );
  alpha.setAttribute( QStringLiteral( "str" ), QStringLiteral( "Alpha" ) );
  alpha.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 1.0, 1.0 ) ) );

  QgsFeature beta( layer->fields() );
  beta.setAttribute( QStringLiteral( "fid" ), 2 );
  beta.setAttribute( QStringLiteral( "str" ), QStringLiteral( "Beta" ) );
  beta.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 2.0, 2.0 ) ) );

  layer->dataProvider()->addFeature( alpha );
  layer->dataProvider()->addFeature( beta );

  QgsProject::instance()->addMapLayer( layer );
  return layer;
}

/*
 * ExpressionCalculatorLocatorFilter
 * The trigger path copies to the clipboard through QGuiApplication, which is not
 * available in this GUI-less harness, so only fetching is exercised
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

/*
 * BookmarkLocatorFilter
 * Triggering a result moves the map extent and writes onto the geometry highlighter,
 * neither of which has a lightweight headless equivalent, so only fetching is covered
 */
TEST_CASE( "BookmarkLocatorFilter" )
{
  LocatorModelSuperBridge bridge;
  BookmarkLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "bookmarks" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "b" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Highest );
    REQUIRE( filter.flags().testFlag( QgsLocatorFilter::FlagFast ) );
    REQUIRE( filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<BookmarkLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "NoResultWithoutBookmarkModel" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "anything" ) ).isEmpty() );
  }

  SECTION( "MatchesBookmarkByName" )
  {
    QgsBookmarkManager manager;
    manager.addBookmark( makeBookmark( QStringLiteral( "Alpha" ) ) );
    manager.addBookmark( makeBookmark( QStringLiteral( "Beta" ) ) );
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    bridge.setBookmarks( &model );

    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );
    REQUIRE( results.size() == 1 );
    REQUIRE( results.at( 0 ).displayString == QStringLiteral( "Alpha" ) );
    REQUIRE( results.at( 0 ).score > 0 );
    REQUIRE( results.at( 0 ).userData().toInt() == 0 );

    REQUIRE( fetchResults( &filter, QStringLiteral( "zzzzz" ) ).isEmpty() );
  }
}

/*
 * HelpLocatorFilter
 * Fetching documentation hits the live docs.qfield.org index and triggering opens a
 * browser, so coverage stops at the guard that runs before any request is made
 */
TEST_CASE( "HelpLocatorFilter" )
{
  LocatorModelSuperBridge bridge;
  HelpLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "optionpages" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "?" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Medium );
    REQUIRE( !filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<HelpLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "NoResultOnShortQuery" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "ab" ) ).isEmpty() );
    REQUIRE( fetchResults( &filter, QString() ).isEmpty() );
  }
}

/*
 * ActiveLayerFeaturesLocatorFilter
 * Triggering a result needs a feature list controller whose model and selection are
 * only populated from QML, so coverage stops at prepare() and fetchResults()
 */
TEST_CASE( "ActiveLayerFeaturesLocatorFilter" )
{
  QgsProject::instance()->clear();

  LocatorModelSuperBridge bridge;
  ActiveLayerFeaturesLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "features" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "f" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Medium );
    REQUIRE( !filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<ActiveLayerFeaturesLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "PrepareWithoutActiveLayer" )
  {
    REQUIRE( filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() ).isEmpty() );
  }

  SECTION( "PrepareSkipsShortSearchStrings" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    REQUIRE( filter.prepare( QStringLiteral( "Al" ), QgsLocatorContext() ).isEmpty() );

    QgsLocatorContext context;
    context.usingPrefix = true;
    REQUIRE( !filter.prepare( QStringLiteral( "Al" ), context ).isEmpty() );
  }

  SECTION( "PrepareReturnsFieldCompletions" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    const QStringList completions = filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    REQUIRE( completions.contains( QStringLiteral( "@fid " ) ) );
    REQUIRE( completions.contains( QStringLiteral( "@str " ) ) );
  }

  SECTION( "MatchesFeatureFromActiveLayer" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );

    REQUIRE( results.size() == 1 );
    REQUIRE( results.at( 0 ).displayString == QStringLiteral( "Alpha" ) );
    REQUIRE( results.at( 0 ).group == QStringLiteral( "points" ) );
    const QVariantList data = results.at( 0 ).userData().toList();
    REQUIRE( data.size() == 2 );
    REQUIRE( data.at( 1 ).toString() == QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 )->id() );
    REQUIRE( hasAction( results.at( 0 ), ActiveLayerFeaturesLocatorFilter::OpenForm ) );
    REQUIRE( hasAction( results.at( 0 ), ActiveLayerFeaturesLocatorFilter::Navigation ) );
  }

  SECTION( "ProposesFieldRestrictionsOnEmptySearch" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    QgsLocatorContext context;
    context.usingPrefix = true;
    filter.prepare( QString(), context );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QString(), context );

    REQUIRE( results.size() == 2 );
    REQUIRE( results.at( 0 ).displayString == QStringLiteral( "@fid" ) );
    REQUIRE( results.at( 1 ).displayString == QStringLiteral( "@str" ) );
    REQUIRE( results.at( 0 ).userData().toMap().value( QStringLiteral( "type" ) ).value<ActiveLayerFeaturesLocatorFilter::ResultType>() == ActiveLayerFeaturesLocatorFilter::ResultType::FieldRestriction );
  }

  SECTION( "RestrictsSearchToSingleField" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    filter.prepare( QStringLiteral( "@str Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "@str Alpha" ) );

    REQUIRE( results.size() == 1 );
    REQUIRE( results.at( 0 ).displayString == QStringLiteral( "Alpha" ) );
  }

  SECTION( "OpenFormActionFeedsTheFeatureList" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    bridge.setActiveLayer( QgsProject::instance()->mapLayersByName( QStringLiteral( "points" ) ).at( 0 ) );

    MultiFeatureListModel featureListModel;
    FeatureListModelSelection selection;
    FeatureListExtentController controller;
    controller.setProperty( "model", QVariant::fromValue( &featureListModel ) );
    controller.setProperty( "selection", QVariant::fromValue( &selection ) );
    bridge.setFeatureListController( &controller );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );
    REQUIRE( results.size() == 1 );

    QSignalSpy formStateSpy( &controller, &FeatureListExtentController::featureFormStateRequested );
    filter.triggerResultFromAction( results.at( 0 ), ActiveLayerFeaturesLocatorFilter::OpenForm );

    REQUIRE( waitForRowCount( featureListModel, 1 ) );
    REQUIRE( selection.focusedItem() == 0 );
    REQUIRE( formStateSpy.count() == 1 );
  }

  QgsProject::instance()->clear();
}

/*
 * FeaturesLocatorFilter
 * As with the active layer filter, triggering depends on QML-populated objects and is
 * left out; prepare() and fetchResults() carry the searchable behaviour
 */
TEST_CASE( "FeaturesLocatorFilter" )
{
  QgsProject::instance()->clear();

  LocatorModelSuperBridge bridge;
  FeaturesLocatorFilter filter( &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "allfeatures" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "af" ) );
    REQUIRE( filter.priority() == QgsLocatorFilter::Medium );
    REQUIRE( filter.useWithoutPrefix() );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<FeaturesLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->prefix() == filter.prefix() );
  }

  SECTION( "NoResultOnShortSearchString" )
  {
    addPointLayer( QStringLiteral( "points" ) );

    REQUIRE( filter.prepare( QStringLiteral( "Al" ), QgsLocatorContext() ).isEmpty() );
    REQUIRE( fetchResults( &filter, QStringLiteral( "Al" ) ).isEmpty() );
  }

  SECTION( "MatchesFeatureAcrossLayers" )
  {
    addPointLayer( QStringLiteral( "points" ) );
    addPointLayer( QStringLiteral( "more points" ) );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );

    REQUIRE( results.size() == 2 );
    for ( const QgsLocatorResult &result : results )
    {
      REQUIRE( result.displayString == QStringLiteral( "Alpha" ) );
      REQUIRE( result.userData().toList().size() == 2 );
      REQUIRE( hasAction( result, FeaturesLocatorFilter::OpenForm ) );
      REQUIRE( hasAction( result, FeaturesLocatorFilter::Navigation ) );
    }
  }

  SECTION( "SkipsNonSearchableLayers" )
  {
    QgsVectorLayer *layer = addPointLayer( QStringLiteral( "points" ) );
    layer->setFlags( layer->flags() & ~QgsMapLayer::Searchable );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    REQUIRE( fetchResults( &filter, QStringLiteral( "Alpha" ) ).isEmpty() );
  }

  SECTION( "OmitsNavigationActionForGeometrylessLayer" )
  {
    QgsVectorLayer *layer = new QgsVectorLayer( QStringLiteral( "None?field=str:string" ), QStringLiteral( "table" ), QStringLiteral( "memory" ) );
    layer->setDisplayExpression( QStringLiteral( "\"str\"" ) );
    QgsFeature feature( layer->fields() );
    feature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "Alpha" ) );
    layer->dataProvider()->addFeature( feature );
    QgsProject::instance()->addMapLayer( layer );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );

    REQUIRE( results.size() == 1 );
    REQUIRE( hasAction( results.at( 0 ), FeaturesLocatorFilter::OpenForm ) );
    REQUIRE( !hasAction( results.at( 0 ), FeaturesLocatorFilter::Navigation ) );
  }

  SECTION( "OpenFormActionFeedsTheFeatureList" )
  {
    addPointLayer( QStringLiteral( "points" ) );

    MultiFeatureListModel featureListModel;
    FeatureListModelSelection selection;
    FeatureListExtentController controller;
    controller.setProperty( "model", QVariant::fromValue( &featureListModel ) );
    controller.setProperty( "selection", QVariant::fromValue( &selection ) );
    bridge.setFeatureListController( &controller );

    filter.prepare( QStringLiteral( "Alpha" ), QgsLocatorContext() );
    const QList<QgsLocatorResult> results = fetchResults( &filter, QStringLiteral( "Alpha" ) );
    REQUIRE( results.size() == 1 );

    QSignalSpy formStateSpy( &controller, &FeatureListExtentController::featureFormStateRequested );
    filter.triggerResultFromAction( results.at( 0 ), FeaturesLocatorFilter::OpenForm );

    REQUIRE( waitForRowCount( featureListModel, 1 ) );
    REQUIRE( selection.focusedItem() == 0 );
    REQUIRE( formStateSpy.count() == 1 );
  }

  QgsProject::instance()->clear();
}
