/***************************************************************************
                        test_maptoscreen
                        --------------------
  begin                : March 2026
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
#include "qfmaptoscreen.h"
#include "qgsquickmapsettings.h"

#include <QSignalSpy>
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>
#include <qgsrectangle.h>

#include <cmath>

static void setupMapSettings( QgsQuickMapSettings &ms )
{
  ms.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
  ms.setOutputSize( QSize( 1000, 500 ) );
  ms.setOutputDpi( 96.0 );
  ms.setExtent( QgsRectangle( 0.0, 0.0, 1000.0, 500.0 ) );
}

TEST_CASE( "MapToScreen" )
{
  SECTION( "default state: screenPoint is null and screenDistance is NaN" )
  {
    QfMapToScreen mts;
    REQUIRE( mts.screenPoint() == QPointF() );
    REQUIRE( std::isnan( mts.screenDistance() ) );
    REQUIRE( mts.mapSettings() == nullptr );
    REQUIRE( mts.mapDistance() == 0.0 );
  }

  SECTION( "setMapSettings emits mapSettingsChanged" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    QSignalSpy spy( &mts, &QfMapToScreen::mapSettingsChanged );
    mts.setMapSettings( &ms );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setMapSettings with same pointer does not emit signal" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    mts.setMapSettings( &ms );
    QSignalSpy spy( &mts, &QfMapToScreen::mapSettingsChanged );
    mts.setMapSettings( &ms );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "setMapPoint emits mapPointChanged" )
  {
    QfMapToScreen mts;
    QSignalSpy spy( &mts, &QfMapToScreen::mapPointChanged );
    mts.setMapPoint( QgsPoint( 100.0, 200.0 ) );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setMapPoint with same point does not emit signal" )
  {
    QfMapToScreen mts;
    mts.setMapPoint( QgsPoint( 100.0, 200.0 ) );
    QSignalSpy spy( &mts, &QfMapToScreen::mapPointChanged );
    mts.setMapPoint( QgsPoint( 100.0, 200.0 ) );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "setMapDistance emits mapDistanceChanged" )
  {
    QfMapToScreen mts;
    QSignalSpy spy( &mts, &QfMapToScreen::mapDistanceChanged );
    mts.setMapDistance( 50.0 );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setMapDistance with same value does not emit signal" )
  {
    QfMapToScreen mts;
    mts.setMapDistance( 50.0 );
    QSignalSpy spy( &mts, &QfMapToScreen::mapDistanceChanged );
    mts.setMapDistance( 50.0 );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "without mapSettings, screenPoint is null regardless of mapPoint" )
  {
    QfMapToScreen mts;
    mts.setMapPoint( QgsPoint( 500.0, 250.0 ) );
    REQUIRE( mts.screenPoint() == QPointF() );
  }

  SECTION( "without mapSettings, screenDistance is NaN" )
  {
    QfMapToScreen mts;
    mts.setMapDistance( 100.0 );
    REQUIRE( std::isnan( mts.screenDistance() ) );
  }

  SECTION( "center map point transforms to center screen point" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    // Extent is 0,0 -> 1000,500; center is 500,250
    mts.setMapSettings( &ms );
    mts.setMapPoint( QgsPoint( 500.0, 250.0 ) );
    // Center of a 1000x500 output should be at screen pixel (500, 250)
    REQUIRE( qgsDoubleNear( mts.screenPoint().x(), 500.0, 1.0 ) );
    REQUIRE( qgsDoubleNear( mts.screenPoint().y(), 250.0, 1.0 ) );
  }

  SECTION( "mapDistance converts correctly to screenDistance" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    // Extent width 1000 map units over 1000 pixels -> 1 map unit per pixel -> mapUnitsPerPoint = 1
    mts.setMapSettings( &ms );
    mts.setMapDistance( 100.0 );
    // 100 map units / 1 mapUnitsPerPoint = 100 screen units
    REQUIRE( qgsDoubleNear( mts.screenDistance(), 100.0, 1.0 ) );
  }

  SECTION( "with mapSettings and zero mapDistance, screenDistance is 0.0 not NaN" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    mts.setMapSettings( &ms );
    mts.setMapDistance( 0.0 );
    REQUIRE( mts.screenDistance() == 0.0 );
  }

  SECTION( "screenPoint updates when extent changes" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    mts.setMapSettings( &ms );
    mts.setMapPoint( QgsPoint( 500.0, 250.0 ) );
    const QPointF firstScreen = mts.screenPoint();
    // Shift extent so same map point is no longer at center
    ms.setExtent( QgsRectangle( 200.0, 100.0, 1200.0, 600.0 ) );
    REQUIRE( mts.screenPoint() != firstScreen );
  }

  SECTION( "screenPointChanged emitted when extent changes" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    mts.setMapSettings( &ms );
    mts.setMapPoint( QgsPoint( 500.0, 250.0 ) );
    QSignalSpy spy( &mts, &QfMapToScreen::screenPointChanged );
    ms.setExtent( QgsRectangle( 200.0, 100.0, 1200.0, 600.0 ) );
    REQUIRE( spy.count() >= 1 );
  }

  SECTION( "swapping mapSettings disconnects old instance signals" )
  {
    QfMapToScreen mts;
    QgsQuickMapSettings ms1;
    setupMapSettings( ms1 );
    mts.setMapSettings( &ms1 );
    mts.setMapPoint( QgsPoint( 500.0, 250.0 ) );
    QgsQuickMapSettings ms2;
    setupMapSettings( ms2 );
    mts.setMapSettings( &ms2 );

    const QPointF screenAfterSwap = mts.screenPoint();
    QSignalSpy spy( &mts, &QfMapToScreen::screenPointChanged );
    // Changing the old map settings should no longer affect mts
    ms1.setExtent( QgsRectangle( 999.0, 999.0, 9999.0, 9999.0 ) );
    REQUIRE( spy.count() == 0 );
    REQUIRE( mts.screenPoint() == screenAfterSwap );
  }
}
