/***************************************************************************
                        test_3dterrainprovider
                        --------------------
  begin                : October 2026
  copyright            : (C) 2026 by Mohsen Dehghanzadeh
  email                : mohsen@opengis.ch
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
#include "qf3dterrainprovider.h"
#include "qgsquickmapsettings.h"

#include <QSignalSpy>
#include <qgscoordinatereferencesystem.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsrasterlayer.h>
#include <qgsrectangle.h>
#include <qgsterrainprovider.h>

TEST_CASE( "3D terrain provider heights" )
{
  QgsProject project;
  project.setCrs( QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:3857" ) ) );

  QgsRasterLayer *demLayer = new QgsRasterLayer( QStringLiteral( TEST_DATA_DIR "/dem.tif" ), QStringLiteral( "dem" ) );
  REQUIRE( demLayer->isValid() );
  project.addMapLayer( demLayer );

  QgsRasterDemTerrainProvider *demTerrainProvider = new QgsRasterDemTerrainProvider();
  demTerrainProvider->setLayer( demLayer );
  project.elevationProperties()->setTerrainProvider( demTerrainProvider );

  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( project.crs() );
  mapSettings.setOutputSize( QSize( 500, 500 ) );
  mapSettings.setExtent( QgsRectangle( 15433948.0, 4201986.0, 15453948.0, 4221986.0 ) );

  Qf3DTerrainProvider terrainProvider;
  terrainProvider.setProject( &project );

  // Nothing to read from before the terrain is loaded
  REQUIRE( terrainProvider.normalizedHeightAt( 15443948.0, 4211986.0 ) == 0.0 );

  QSignalSpy terrainDataReadySpy( &terrainProvider, &Qf3DTerrainProvider::terrainDataReady );
  terrainProvider.setMapSettings( &mapSettings );
  REQUIRE( ( terrainDataReadySpy.count() > 0 || terrainDataReadySpy.wait( 10000 ) ) );

  // The first two points of the top row of the terrain mesh
  const QgsRectangle extent = terrainProvider.normalizedDataExtent();
  const double cellWidth = extent.width() / ( terrainProvider.gridSize().width() - 1 );
  const double x = extent.xMinimum();
  const double y = extent.yMaximum();
  const double firstHeight = terrainProvider.normalizedData().at( 0 ).toDouble();
  const double secondHeight = terrainProvider.normalizedData().at( 1 ).toDouble();

  SECTION( "on mesh points" )
  {
    REQUIRE( terrainProvider.normalizedHeightAt( x, y ) == Catch::Approx( firstHeight ) );
    REQUIRE( terrainProvider.normalizedHeightAt( x + cellWidth, y ) == Catch::Approx( secondHeight ) );
  }

  SECTION( "between mesh points" )
  {
    REQUIRE( terrainProvider.normalizedHeightAt( x + cellWidth / 2, y ) == Catch::Approx( ( firstHeight + secondHeight ) / 2 ) );
  }

  SECTION( "outside the terrain" )
  {
    REQUIRE( terrainProvider.normalizedHeightAt( x - 1000, y + 1000 ) == Catch::Approx( firstHeight ) );
  }
}
