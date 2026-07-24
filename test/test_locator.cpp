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
#include "finlandlocatorfilter.h"
#include "gotolocatorfilter.h"
#include "helplocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "multifeaturelistmodel.h"
#include "qfieldlocatorfilter.h"
#include "qgsquickmapsettings.h"

#include <QCoreApplication>
#include <QSettings>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <qgsapplication.h>
#include <qgsbookmarkmanager.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsfeedback.h>
#include <qgsgeocoder.h>
#include <qgslocator.h>
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

class DummyGeocoder : public QgsGeocoderInterface
{
  public:
    Flags flags() const override { return Flags(); }
};

class ScopedIniSettings
{
  public:
    explicit ScopedIniSettings( const QString &root )
      : mPreviousDefaultFormat( QSettings::defaultFormat() )
      , mPreviousOrg( QCoreApplication::organizationName() )
      , mPreviousApp( QCoreApplication::applicationName() )
    {
      QSettings::setDefaultFormat( QSettings::IniFormat );
      QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, root );
      QCoreApplication::setOrganizationName( QStringLiteral( "QFieldUnitTests" ) );
      QCoreApplication::setApplicationName( QStringLiteral( "LocatorTests" ) );
    }

    ~ScopedIniSettings()
    {
      QCoreApplication::setOrganizationName( mPreviousOrg );
      QCoreApplication::setApplicationName( mPreviousApp );
      QSettings::setDefaultFormat( mPreviousDefaultFormat );
    }

  private:
    QSettings::Format mPreviousDefaultFormat;
    QString mPreviousOrg;
    QString mPreviousApp;
};

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

    REQUIRE( featureListModel.rowCount() == 1 );
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

    REQUIRE( featureListModel.rowCount() == 1 );
    REQUIRE( selection.focusedItem() == 0 );
    REQUIRE( formStateSpy.count() == 1 );
  }

  QgsProject::instance()->clear();
}

/*
 * FinlandLocatorFilter
 * The filter is currently not registered by the bridge and its geocoding path is
 * network backed, so only its construction time contract is covered
 */
TEST_CASE( "FinlandLocatorFilter" )
{
  LocatorModelSuperBridge bridge;
  DummyGeocoder geocoder;
  FinlandLocatorFilter filter( &geocoder, &bridge );

  SECTION( "Metadata" )
  {
    REQUIRE( filter.name() == QStringLiteral( "pelias-finland" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "fia" ) );
    REQUIRE( filter.geocoder() == &geocoder );
    REQUIRE( !filter.useWithoutPrefix() );
  }

  SECTION( "BoundingBoxCoversFinland" )
  {
    REQUIRE( filter.boundingBox().contains( QgsPointXY( 24.9384, 60.1699 ) ) );
    REQUIRE( !filter.boundingBox().contains( QgsPointXY( -79.3832, 43.6532 ) ) );
  }

  SECTION( "BoundingBoxRoundTrips" )
  {
    const QgsRectangle boundingBox( 1.0, 2.0, 3.0, 4.0 );
    filter.setBoundingBox( boundingBox );
    REQUIRE( filter.boundingBox() == boundingBox );
  }

  SECTION( "Clone" )
  {
    std::unique_ptr<FinlandLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == filter.name() );
    REQUIRE( cloned->geocoder() == &geocoder );
  }
}

/*
 * QFieldLocatorFilter
 */
TEST_CASE( "QFieldLocatorFilter" )
{
  QFieldLocatorFilter filter;

  SECTION( "Defaults" )
  {
    REQUIRE( filter.priority() == QgsLocatorFilter::Medium );
    REQUIRE( !filter.useWithoutPrefix() );
    REQUIRE( filter.name().isEmpty() );
    REQUIRE( filter.source().isEmpty() );
  }

  SECTION( "PropertiesRoundTripAndNotifyOnce" )
  {
    QSignalSpy nameSpy( &filter, &QFieldLocatorFilter::nameChanged );
    filter.setName( QStringLiteral( "plugin" ) );
    REQUIRE( filter.name() == QStringLiteral( "plugin" ) );
    REQUIRE( nameSpy.count() == 1 );

    filter.setName( QStringLiteral( "plugin" ) );
    REQUIRE( nameSpy.count() == 1 );

    QSignalSpy displayNameSpy( &filter, &QFieldLocatorFilter::displayNameChanged );
    filter.setDisplayName( QStringLiteral( "Plugin filter" ) );
    REQUIRE( filter.displayName() == QStringLiteral( "Plugin filter" ) );
    REQUIRE( displayNameSpy.count() == 1 );

    QSignalSpy prefixSpy( &filter, &QFieldLocatorFilter::prefixChanged );
    filter.setPrefix( QStringLiteral( "plg" ) );
    REQUIRE( filter.prefix() == QStringLiteral( "plg" ) );
    REQUIRE( prefixSpy.count() == 1 );

    QSignalSpy descriptionSpy( &filter, &QFieldLocatorFilter::descriptionChanged );
    filter.setDescription( QStringLiteral( "A plugin filter" ) );
    REQUIRE( filter.description() == QStringLiteral( "A plugin filter" ) );
    REQUIRE( descriptionSpy.count() == 1 );

    QSignalSpy delaySpy( &filter, &QFieldLocatorFilter::delayChanged );
    filter.setDelay( 500 );
    REQUIRE( filter.delay() == 500 );
    REQUIRE( delaySpy.count() == 1 );

    QSignalSpy parametersSpy( &filter, &QFieldLocatorFilter::parametersChanged );
    const QVariantMap parameters( { { QStringLiteral( "key" ), QStringLiteral( "value" ) } } );
    filter.setParameters( parameters );
    REQUIRE( filter.parameters() == parameters );
    REQUIRE( parametersSpy.count() == 1 );
  }

  SECTION( "SourceGetsCacheBustingQuery" )
  {
    QSignalSpy sourceSpy( &filter, &QFieldLocatorFilter::sourceChanged );
    filter.setSource( QUrl( QStringLiteral( "file:///tmp/filter.qml" ) ) );

    REQUIRE( sourceSpy.count() == 1 );
    REQUIRE( filter.source().path() == QStringLiteral( "/tmp/filter.qml" ) );
    REQUIRE( filter.source().hasQuery() );
  }

  SECTION( "CloneCopiesConfiguration" )
  {
    LocatorModelSuperBridge bridge;
    filter.setName( QStringLiteral( "plugin" ) );
    filter.setDisplayName( QStringLiteral( "Plugin filter" ) );
    filter.setPrefix( QStringLiteral( "plg" ) );
    filter.setDelay( 500 );
    filter.setParameters( QVariantMap( { { QStringLiteral( "key" ), QStringLiteral( "value" ) } } ) );
    filter.setSource( QUrl( QStringLiteral( "file:///tmp/filter.qml" ) ) );
    filter.setLocatorBridge( &bridge );

    std::unique_ptr<QFieldLocatorFilter> cloned( filter.clone() );
    REQUIRE( cloned );
    REQUIRE( cloned->name() == QStringLiteral( "plugin" ) );
    REQUIRE( cloned->displayName() == QStringLiteral( "Plugin filter" ) );
    REQUIRE( cloned->prefix() == QStringLiteral( "plg" ) );
    REQUIRE( cloned->delay() == 500 );
    REQUIRE( cloned->parameters() == filter.parameters() );
    REQUIRE( cloned->source().path() == filter.source().path() );
    REQUIRE( cloned->locatorBridge() == &bridge );
  }

  SECTION( "NoResultWithoutSource" )
  {
    REQUIRE( fetchResults( &filter, QStringLiteral( "anything" ) ).isEmpty() );
  }
}

/*
 * LocatorModelSuperBridge
 */
TEST_CASE( "LocatorModelSuperBridge" )
{
  LocatorModelSuperBridge bridge;

  SECTION( "RegistersQFieldFilters" )
  {
    REQUIRE( bridge.locator()->filters().count() == 6 );

    QStringList names;
    for ( QgsLocatorFilter *filter : bridge.locator()->filters() )
      names << filter->name();

    REQUIRE( names.contains( QStringLiteral( "features" ) ) );
    REQUIRE( names.contains( QStringLiteral( "allfeatures" ) ) );
    REQUIRE( names.contains( QStringLiteral( "goto" ) ) );
    REQUIRE( names.contains( QStringLiteral( "bookmarks" ) ) );
    REQUIRE( names.contains( QStringLiteral( "calculator" ) ) );
    REQUIRE( names.contains( QStringLiteral( "optionpages" ) ) );
  }

  SECTION( "RegistersAndDeregistersPluginFilters" )
  {
    const int initialCount = static_cast<int>( bridge.locator()->filters().count() );
    QSignalSpy spy( &bridge, &LocatorModelSuperBridge::locatorFiltersChanged );

    QFieldLocatorFilter *filter = new QFieldLocatorFilter();
    filter->setName( QStringLiteral( "plugin" ) );
    bridge.registerQFieldLocatorFilter( filter );
    REQUIRE( bridge.locator()->filters().count() == initialCount + 1 );
    REQUIRE( spy.count() == 1 );

    bridge.deregisterQFieldLocatorFilter( filter );
    REQUIRE( bridge.locator()->filters().count() == initialCount );
    REQUIRE( spy.count() == 2 );
  }

  SECTION( "PrefixIsExtractedFromSearchString" )
  {
    REQUIRE( bridge.getPrefixFromSearchString( QStringLiteral( "go 1.5 2.5" ) ) == QStringLiteral( "go" ) );
    REQUIRE( bridge.getPrefixFromSearchString( QStringLiteral( "  b Alpha" ) ) == QStringLiteral( "b" ) );
    REQUIRE( bridge.getPrefixFromSearchString( QStringLiteral( "zz Alpha" ) ).isEmpty() );
    REQUIRE( bridge.getPrefixFromSearchString( QString() ).isEmpty() );
  }

  SECTION( "PropertiesRoundTripAndNotifyOnce" )
  {
    QSignalSpy keepScaleSpy( &bridge, &LocatorModelSuperBridge::keepScaleChanged );
    REQUIRE( !bridge.keepScale() );
    bridge.setKeepScale( true );
    REQUIRE( bridge.keepScale() );
    REQUIRE( keepScaleSpy.count() == 1 );
    bridge.setKeepScale( true );
    REQUIRE( keepScaleSpy.count() == 1 );

    QgsVectorLayer layer( QStringLiteral( "Point?crs=epsg:4326" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
    QSignalSpy activeLayerSpy( &bridge, &LocatorModelSuperBridge::activeLayerChanged );
    bridge.setActiveLayer( &layer );
    REQUIRE( bridge.activeLayer() == &layer );
    REQUIRE( activeLayerSpy.count() == 1 );

    QgsBookmarkManager manager;
    BookmarkModel model( &manager, QgsApplication::bookmarkManager() );
    QSignalSpy bookmarksSpy( &bridge, &LocatorModelSuperBridge::bookmarksChanged );
    bridge.setBookmarks( &model );
    REQUIRE( bridge.bookmarks() == &model );
    REQUIRE( bookmarksSpy.count() == 1 );
  }

  SECTION( "EmitsSearchRequests" )
  {
    QSignalSpy searchSpy( &bridge, &LocatorModelSuperBridge::searchRequested );
    bridge.requestSearch( QStringLiteral( "Alpha" ) );
    REQUIRE( searchSpy.count() == 1 );
    REQUIRE( searchSpy.at( 0 ).at( 0 ).toString() == QStringLiteral( "Alpha" ) );

    QSignalSpy messageSpy( &bridge, &LocatorModelSuperBridge::messageEmitted );
    bridge.emitMessage( QStringLiteral( "Something happened" ) );
    REQUIRE( messageSpy.count() == 1 );
  }
}

/*
 * LocatorFiltersModel
 */
TEST_CASE( "LocatorFiltersModel" )
{
  LocatorFiltersModel model;

  SECTION( "EmptyWithoutBridge" )
  {
    /* data() dereferences the bridge before null checking it, unlike rowCount()
     * and setData(), so it is not called here without a bridge set */
    REQUIRE( model.rowCount() == 0 );
    REQUIRE( !model.index( 0, 0 ).isValid() );
  }

  SECTION( "RoleNames" )
  {
    const QHash<int, QByteArray> roles = model.roleNames();
    REQUIRE( roles.contains( LocatorFiltersModel::NameRole ) );
    REQUIRE( roles.contains( LocatorFiltersModel::DescriptionRole ) );
    REQUIRE( roles.contains( LocatorFiltersModel::PrefixRole ) );
    REQUIRE( roles.contains( LocatorFiltersModel::ActiveRole ) );
    REQUIRE( roles.contains( LocatorFiltersModel::DefaultRole ) );
  }

  SECTION( "ListsRegisteredFilters" )
  {
    LocatorModelSuperBridge bridge;
    LocatorFiltersModel boundModel;
    boundModel.setLocatorModelSuperBridge( &bridge );

    REQUIRE( boundModel.rowCount() == bridge.locator()->filters().count() );

    const QModelIndex index = boundModel.index( 0, 0 );
    QgsLocatorFilter *filter = boundModel.filterForIndex( index );
    REQUIRE( filter );
    REQUIRE( boundModel.data( index, LocatorFiltersModel::NameRole ).toString() == filter->displayName() );
    REQUIRE( boundModel.data( index, LocatorFiltersModel::DescriptionRole ).toString() == filter->description() );
    REQUIRE( boundModel.data( index, LocatorFiltersModel::ActiveRole ).toBool() == filter->enabled() );
  }

  SECTION( "TogglesFilterStateThroughSetData" )
  {
    QTemporaryDir settingsDir;
    REQUIRE( settingsDir.isValid() );
    const ScopedIniSettings scopedSettings( settingsDir.path() );

    LocatorModelSuperBridge bridge;
    LocatorFiltersModel boundModel;
    boundModel.setLocatorModelSuperBridge( &bridge );

    const QModelIndex index = boundModel.index( 0, 0 );
    QgsLocatorFilter *filter = boundModel.filterForIndex( index );
    const bool wasEnabled = filter->enabled();

    REQUIRE( boundModel.setData( index, !wasEnabled, LocatorFiltersModel::ActiveRole ) );
    REQUIRE( filter->enabled() == !wasEnabled );
    REQUIRE( !boundModel.setData( index, !wasEnabled, LocatorFiltersModel::ActiveRole ) );

    const bool wasDefault = filter->useWithoutPrefix();
    REQUIRE( boundModel.setData( index, !wasDefault, LocatorFiltersModel::DefaultRole ) );
    REQUIRE( filter->useWithoutPrefix() == !wasDefault );

    REQUIRE( !boundModel.setData( index, QStringLiteral( "nope" ), LocatorFiltersModel::NameRole ) );
    REQUIRE( !boundModel.setData( index, QStringLiteral( "nope" ), LocatorFiltersModel::PrefixRole ) );
  }
}
