/***************************************************************************
                        test_projectutils.cpp
                        --------------------
  begin                : Oct 2026
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
#include "utils/qfcloudutils.h"
#include "utils/qfprojectutils.h"

#include <QDir>
#include <QFile>
#include <QSet>
#include <QTemporaryDir>
#include <qgsproject.h>
#include <qgsrasterlayer.h>
#include <qgsvectorlayer.h>

static QString copyTestData( const QString &fileName, const QString &destinationDir )
{
  REQUIRE( QDir().mkpath( destinationDir ) );
  const QString destination = QDir( destinationDir ).filePath( fileName );
  REQUIRE( QFile::copy( QStringLiteral( TEST_DATA_DIR "/%1" ).arg( fileName ), destination ) );
  return destination;
}

static void writeFile( const QString &filePath, const QByteArray &content )
{
  REQUIRE( QDir().mkpath( QFileInfo( filePath ).absolutePath() ) );
  QFile file( filePath );
  REQUIRE( file.open( QIODevice::WriteOnly ) );
  file.write( content );
}

TEST_CASE( "QfProjectUtils::isLayerDataset" )
{
  QTemporaryDir dataDir;
  REQUIRE( dataDir.isValid() );

  const QString vectorPath = copyTestData( QStringLiteral( "polygons.gpkg" ), dataDir.path() );
  const QString rasterPath = copyTestData( QStringLiteral( "dem.tif" ), dataDir.path() );
  const QString unusedPath = copyTestData( QStringLiteral( "notes.gpkg" ), dataDir.path() );
  const QString photoPath = QDir( dataDir.path() ).filePath( QStringLiteral( "photo.jpg" ) );
  writeFile( photoPath, QByteArray( "photo" ) );
  const QString shapefilePath = QDir( dataDir.path() ).filePath( QStringLiteral( "roads.shp" ) );
  const QString shapefileDbfPath = QDir( dataDir.path() ).filePath( QStringLiteral( "roads.dbf" ) );
  writeFile( shapefilePath, QByteArray( "shp" ) );
  writeFile( shapefileDbfPath, QByteArray( "dbf" ) );
  const QString sitesPath = QDir( dataDir.path() ).filePath( QStringLiteral( "sites.gpkg" ) );
  const QString sitesWalPath = QStringLiteral( "%1-wal" ).arg( sitesPath );
  const QString sitesShmPath = QStringLiteral( "%1-shm" ).arg( sitesPath );
  writeFile( sitesPath, QByteArray( "gpkg" ) );
  writeFile( sitesWalPath, QByteArray( "wal" ) );
  writeFile( sitesShmPath, QByteArray( "shm" ) );

  QgsProject project;
  project.addMapLayer( new QgsVectorLayer( shapefilePath, QStringLiteral( "roads" ), QStringLiteral( "ogr" ) ) );
  project.addMapLayer( new QgsVectorLayer( QStringLiteral( "%1|layername=sites" ).arg( sitesPath ), QStringLiteral( "sites" ), QStringLiteral( "ogr" ) ) );
  project.addMapLayer( new QgsVectorLayer( QStringLiteral( "%1|layername=polygons" ).arg( vectorPath ), QStringLiteral( "polygons" ), QStringLiteral( "ogr" ) ) );
  project.addMapLayer( new QgsRasterLayer( rasterPath, QStringLiteral( "dem" ), QStringLiteral( "gdal" ) ) );
  project.addMapLayer( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:4326" ), QStringLiteral( "memory" ), QStringLiteral( "memory" ) ) );

  SECTION( "Layer datasets are detected" )
  {
    REQUIRE( QfProjectUtils::isLayerDataset( &project, vectorPath ) );
    REQUIRE( QfProjectUtils::isLayerDataset( &project, rasterPath ) );
    REQUIRE( QfProjectUtils::isLayerDataset( &project, QStringLiteral( "%1/./polygons.gpkg" ).arg( dataDir.path() ) ) );
    REQUIRE( QfProjectUtils::isLayerDataset( &project, shapefileDbfPath ) );
    REQUIRE( QfProjectUtils::isLayerDataset( &project, sitesWalPath ) );
    REQUIRE( QfProjectUtils::isLayerDataset( &project, sitesShmPath ) );
  }

  SECTION( "Other files are not layer datasets" )
  {
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( &project, unusedPath ) );
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( &project, photoPath ) );
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( &project, dataDir.path() ) );
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( &project, QDir( dataDir.path() ).filePath( QStringLiteral( "missing.gpkg" ) ) ) );
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( &project, QString() ) );
    REQUIRE_FALSE( QfProjectUtils::isLayerDataset( nullptr, vectorPath ) );
  }
}

TEST_CASE( "QfCloudUtils::addPendingAttachments skips layer datasets within folders" )
{
  QTemporaryDir cloudDir;
  REQUIRE( cloudDir.isValid() );
  QfCloudUtils::setLocalCloudDirectory( cloudDir.path() );

  const QString username = QStringLiteral( "alice" );
  const QString projectId = QStringLiteral( "11111111-1111-1111-1111-111111111111" );
  const QString projectDir = QStringLiteral( "%1/%2/%3" ).arg( cloudDir.path(), username, projectId );

  const QString vectorPath = QStringLiteral( "%1/data/points.geojson" ).arg( projectDir );
  writeFile( vectorPath, QByteArray( R"({"type":"FeatureCollection","features":[]})" ) );
  const QString unusedPath = copyTestData( QStringLiteral( "notes.gpkg" ), QStringLiteral( "%1/data" ).arg( projectDir ) );
  const QString photoPath = QStringLiteral( "%1/DCIM/photo.jpg" ).arg( projectDir );
  writeFile( photoPath, QByteArray( "photo" ) );
  const QString shapefilePath = QStringLiteral( "%1/data/roads.shp" ).arg( projectDir );
  writeFile( shapefilePath, QByteArray( "shp" ) );
  writeFile( QStringLiteral( "%1/data/roads.dbf" ).arg( projectDir ), QByteArray( "dbf" ) );

  QgsProject::instance()->addMapLayer( new QgsVectorLayer( vectorPath, QStringLiteral( "points" ), QStringLiteral( "ogr" ) ) );
  QgsProject::instance()->addMapLayer( new QgsVectorLayer( shapefilePath, QStringLiteral( "roads" ), QStringLiteral( "ogr" ) ) );

  QfCloudUtils::addPendingAttachments( username, projectId, QStringList() << projectDir );

  const QStringList pendingAttachments = QfCloudUtils::getPendingAttachments( username ).values( projectId );
  REQUIRE( QSet<QString>( pendingAttachments.begin(), pendingAttachments.end() ) == QSet<QString>( { unusedPath, photoPath } ) );

  QgsProject::instance()->clear();
  QfCloudUtils::setLocalCloudDirectory( QString() );
}
