/***************************************************************************
                        test_scalebarmeasurement
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

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "qgsquickmapsettings.h"
#include "scalebarmeasurement.h"

#include <QSignalSpy>
#include <qgscoordinatereferencesystem.h>
#include <qgsproject.h>
#include <qgsrectangle.h>


static void setupMapSettings( QgsQuickMapSettings &ms )
{
  ms.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
  ms.setOutputSize( QSize( 1000, 500 ) );
  ms.setOutputDpi( 96.0 );
  ms.setExtent( QgsRectangle( 0.0, 0.0, 100000.0, 50000.0 ) );
}

TEST_CASE( "ScaleBarMeasurement" )
{
  SECTION( "default state: screenLength is 0 and label is empty" )
  {
    ScaleBarMeasurement sbm;
    REQUIRE( sbm.screenLength() == 0.0 );
    REQUIRE( sbm.label().isEmpty() );
  }

  SECTION( "default referenceScreenLength is 300" )
  {
    ScaleBarMeasurement sbm;
    REQUIRE( sbm.referenceScreenLength() == 300.0 );
  }

  SECTION( "setReferenceScreenLength updates value and emits signal" )
  {
    ScaleBarMeasurement sbm;
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::referenceScreenLengthChanged );
    sbm.setReferenceScreenLength( 150.0 );
    REQUIRE( sbm.referenceScreenLength() == 150.0 );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setReferenceScreenLength with same value does not emit signal" )
  {
    ScaleBarMeasurement sbm;
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::referenceScreenLengthChanged );
    sbm.setReferenceScreenLength( 300.0 );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "setProject emits projectChanged" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::projectChanged );
    sbm.setProject( &project );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setProject with same pointer does not emit signal" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    sbm.setProject( &project );
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::projectChanged );
    sbm.setProject( &project );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "setMapSettings emits mapSettingsChanged" )
  {
    ScaleBarMeasurement sbm;
    QgsQuickMapSettings ms;
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::mapSettingsChanged );
    sbm.setMapSettings( &ms );
    REQUIRE( spy.count() == 1 );
  }

  SECTION( "setMapSettings with same pointer does not emit signal" )
  {
    ScaleBarMeasurement sbm;
    QgsQuickMapSettings ms;
    sbm.setMapSettings( &ms );
    QSignalSpy spy( &sbm, &ScaleBarMeasurement::mapSettingsChanged );
    sbm.setMapSettings( &ms );
    REQUIRE( spy.count() == 0 );
  }

  SECTION( "without project, screenLength stays 0 even with mapSettings" )
  {
    ScaleBarMeasurement sbm;
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setMapSettings( &ms );
    // measure() returns early when mProject is null
    REQUIRE( sbm.screenLength() == 0.0 );
    REQUIRE( sbm.label().isEmpty() );
  }

  SECTION( "with project and mapSettings, screenLength is positive and label is non-empty" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );
    REQUIRE( sbm.screenLength() > 0.0 );
    REQUIRE_FALSE( sbm.label().isEmpty() );
  }

  SECTION( "label contains correct numeric value and km unit for projected CRS" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );
    // label format is "%value %unit" like "50 km"
    const QString label = sbm.label();
    REQUIRE_FALSE( label.isEmpty() );
    const QStringList parts = label.split( QStringLiteral( " " ) );
    REQUIRE( parts.size() == 2 );
    bool ok = false;
    parts[0].toDouble( &ok );
    REQUIRE( ok );
    // for a metre-based CRS at 100km-wide extent the unit must be km
    REQUIRE( parts[1] == QStringLiteral( "km" ) );
  }

  SECTION( "changing extent triggers measure and updates screenLength" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );

    const double firstLength = sbm.screenLength();
    // double the extent width -> scale bar should change
    ms.setExtent( QgsRectangle( 0.0, 0.0, 200000.0, 100000.0 ) );
    REQUIRE( sbm.screenLength() != firstLength );
  }

  SECTION( "screenLengthChanged and labelChanged emitted after extent change" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );

    QSignalSpy screenSpy( &sbm, &ScaleBarMeasurement::screenLengthChanged );
    QSignalSpy labelSpy( &sbm, &ScaleBarMeasurement::labelChanged );
    ms.setExtent( QgsRectangle( 0.0, 0.0, 500000.0, 250000.0 ) );
    REQUIRE( screenSpy.count() >= 1 );
    REQUIRE( labelSpy.count() >= 1 );
  }

  SECTION( "degree-based CRS produces label prefixed with ~" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
    QgsQuickMapSettings ms;
    ms.setDestinationCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );
    ms.setOutputSize( QSize( 1000, 500 ) );
    ms.setOutputDpi( 96.0 );
    ms.setExtent( QgsRectangle( 0.0, 0.0, 10.0, 5.0 ) );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );
    REQUIRE( sbm.label().startsWith( QStringLiteral( "~" ) ) );
  }

  SECTION( "removing mapSettings by setting new one disconnects old signals" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms1;
    setupMapSettings( ms1 );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms1 );

    QgsQuickMapSettings ms2;
    setupMapSettings( ms2 );
    sbm.setMapSettings( &ms2 );

    const double lengthAfterSwitch = sbm.screenLength();
    // changing the old map settings should not affect sbm anymore
    ms1.setExtent( QgsRectangle( 0.0, 0.0, 999999.0, 500000.0 ) );
    REQUIRE( sbm.screenLength() == lengthAfterSwitch );
  }

  SECTION( "ellipsoid change on project triggers resetDistanceArea" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );

    const double lengthBefore = sbm.screenLength();
    // ellipsoidChanged -> resetDistanceArea only (measure is not called)
    // screenLength is unchanged; the next extent change will use the new ellipsoid
    project.setEllipsoid( QStringLiteral( "GRS80" ) );
    REQUIRE( sbm.screenLength() == lengthBefore );
    // a subsequent extent change now uses the updated distance area
    QSignalSpy screenSpy( &sbm, &ScaleBarMeasurement::screenLengthChanged );
    ms.setExtent( QgsRectangle( 0.0, 0.0, 200000.0, 100000.0 ) );
    REQUIRE( screenSpy.count() >= 1 );
  }

  SECTION( "setProject to nullptr after having a project resets label to empty" )
  {
    ScaleBarMeasurement sbm;
    QgsProject project;
    project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );
    QgsQuickMapSettings ms;
    setupMapSettings( ms );
    sbm.setProject( &project );
    sbm.setMapSettings( &ms );
    REQUIRE_FALSE( sbm.label().isEmpty() );

    sbm.setProject( nullptr );
    REQUIRE( sbm.label().isEmpty() );
    REQUIRE( sbm.screenLength() == 0.0 );
  }
}
