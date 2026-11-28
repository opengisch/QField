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
#include "platforms/qfplatformutilities.h"
#include "testutils.h"
#include "utils/qfcloudutils.h"
#include "utils/qfprojectutils.h"

#include <QColor>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QSet>
#include <QTemporaryDir>
#include <qgsattributeeditorcontainer.h>
#include <qgspolymorphicrelation.h>
#include <qgsproject.h>
#include <qgsprojectdisplaysettings.h>
#include <qgsrasterlayer.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>
#include <qgsxmlutils.h>

using Catch::Approx;

class ScopedCreatedProject
{
  public:
    explicit ScopedCreatedProject( const QVariantMap &options, const QfGnssPositionInformation &positionInformation = QfGnssPositionInformation() )
      : mFilePath( QfProjectUtils::createProject( options, positionInformation ) )
    {
    }

    ~ScopedCreatedProject()
    {
      if ( !mFilePath.isEmpty() )
      {
        QDir( directory() ).removeRecursively();
      }
    }

    QString filePath() const { return mFilePath; }
    QString directory() const { return QFileInfo( mFilePath ).absolutePath(); }

  private:
    QString mFilePath;
};

static QgsRectangle readCanvasExtent( const QString &projectFilePath )
{
  QgsRectangle extent;
  QgsProject project;
  QObject::connect( &project, &QgsProject::readProject, &project, [&extent]( const QDomDocument &document ) {
    extent = QgsXmlUtils::readRectangle( document.elementsByTagName( QStringLiteral( "mapcanvas" ) ).at( 0 ).firstChildElement( QStringLiteral( "extent" ) ) );
  } );
  REQUIRE( project.read( projectFilePath ) );
  return extent;
}

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

TEST_CASE( "QfProjectUtils::createProject" )
{
  QTemporaryDir settingsDir;
  REQUIRE( settingsDir.isValid() );
  const ScopedIniSettings scopedSettings( settingsDir.path(), QStringLiteral( "ProjectUtilsTests" ) );

  SECTION( "Blank project is written with title, crs and attachment directories" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Unit Test Project" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) } };
    const ScopedCreatedProject created( options );
    REQUIRE( created.filePath() == QStringLiteral( "%1/Created Projects/Unit_Test_Project/Unit_Test_Project.qgz" ).arg( QDir::cleanPath( QfPlatformUtilities::instance()->applicationDirectory() ) ) );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.title() == QStringLiteral( "Unit Test Project" ) );
    REQUIRE( project.crs().authid() == QStringLiteral( "EPSG:3857" ) );
    REQUIRE( project.displaySettings()->coordinateType() == Qgis::CoordinateDisplayType::CustomCrs );
    REQUIRE( project.displaySettings()->coordinateCustomCrs().authid() == QStringLiteral( "EPSG:4326" ) );
    REQUIRE( project.readListEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "attachmentDirs" ) ) == QStringList( { QStringLiteral( "DCIM" ), QStringLiteral( "audio" ), QStringLiteral( "video" ), QStringLiteral( "files" ) } ) );
    REQUIRE( project.readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ) ).isEmpty() );
    REQUIRE_FALSE( project.readBoolEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "forceAutoPush" ) ) );
    REQUIRE( project.mapLayers().isEmpty() );
  }

  SECTION( "Project file name is sanitized and kept unique" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Café Été 2026!" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) } };
    const ScopedCreatedProject first( options );
    const ScopedCreatedProject second( options );

    REQUIRE( QFileInfo( first.filePath() ).fileName() == QStringLiteral( "Cafe_Ete_2026_.qgz" ) );
    REQUIRE( QFileInfo( first.directory() ).fileName() == QStringLiteral( "Cafe_Ete_2026_" ) );
    REQUIRE( QFileInfo( second.filePath() ).fileName() == QStringLiteral( "Cafe_Ete_2026_.qgz" ) );
    REQUIRE( second.directory() == QStringLiteral( "%1_2" ).arg( first.directory() ) );

    QgsProject project;
    REQUIRE( project.read( second.filePath() ) );
    REQUIRE( project.title() == QStringLiteral( "Café Été 2026!" ) );
  }

  SECTION( "Notes layer is configured for digitizing" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Notes" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "notes" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ) ) == QStringLiteral( "digitize" ) );
    REQUIRE( project.mapLayers().size() == 1 );

    const QList<QgsMapLayer *> notesLayers = project.mapLayersByName( QStringLiteral( "Notes" ) );
    REQUIRE( notesLayers.size() == 1 );
    QgsVectorLayer *notesLayer = qobject_cast<QgsVectorLayer *>( notesLayers.at( 0 ) );
    REQUIRE( notesLayer );
    REQUIRE( notesLayer->isValid() );
    REQUIRE( notesLayer->source() == QStringLiteral( "%1/notes.gpkg|layername=notes_point" ).arg( created.directory() ) );
    REQUIRE( notesLayer->wkbType() == Qgis::WkbType::PointZ );
    REQUIRE( notesLayer->fields().names() == QStringList( { QStringLiteral( "fid" ), QStringLiteral( "uuid" ), QStringLiteral( "color" ), QStringLiteral( "title" ), QStringLiteral( "note" ), QStringLiteral( "timestamp" ) } ) );
    REQUIRE( notesLayer->displayExpression() == QStringLiteral( "\"title\"" ) );
    REQUIRE( notesLayer->customProperty( QStringLiteral( "QFieldSync/cloud_action" ) ).toString() == QStringLiteral( "offline" ) );
    REQUIRE( notesLayer->customProperty( QStringLiteral( "QFieldSync/action" ) ).toString() == QStringLiteral( "offline" ) );

    const int uuidIndex = notesLayer->fields().indexOf( QStringLiteral( "uuid" ) );
    REQUIRE( notesLayer->editorWidgetSetup( uuidIndex ).type() == QStringLiteral( "Hidden" ) );
    REQUIRE( notesLayer->defaultValueDefinition( uuidIndex ).expression() == QStringLiteral( "uuid()" ) );

    const int colorIndex = notesLayer->fields().indexOf( QStringLiteral( "color" ) );
    REQUIRE( notesLayer->editorWidgetSetup( colorIndex ).type() == QStringLiteral( "Color" ) );
    REQUIRE( notesLayer->defaultValueDefinition( colorIndex ).expression() == QStringLiteral( "'#377eb8'" ) );
    REQUIRE( notesLayer->attributeDisplayName( colorIndex ) == QStringLiteral( "Marker color" ) );

    const int noteIndex = notesLayer->fields().indexOf( QStringLiteral( "note" ) );
    REQUIRE( notesLayer->editorWidgetSetup( noteIndex ).type() == QStringLiteral( "TextEdit" ) );
    REQUIRE( notesLayer->editorWidgetSetup( noteIndex ).config().value( QStringLiteral( "IsMultiline" ) ).toBool() );

    const int timestampIndex = notesLayer->fields().indexOf( QStringLiteral( "timestamp" ) );
    REQUIRE( notesLayer->editorWidgetSetup( timestampIndex ).type() == QStringLiteral( "DateTime" ) );
    REQUIRE( notesLayer->defaultValueDefinition( timestampIndex ).expression() == QStringLiteral( "now()" ) );
    REQUIRE( notesLayer->attributeDisplayName( timestampIndex ) == QStringLiteral( "Time" ) );
  }

  SECTION( "Notes on lines and polygons add a layer per geometry type" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Notes" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "notes" ), true },
                                  { QStringLiteral( "notes_on_lines_polygons" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.mapLayers().size() == 3 );

    const QList<QPair<QString, Qgis::WkbType>> expectedLayers = {
      { QStringLiteral( "Notes — Point" ), Qgis::WkbType::PointZ },
      { QStringLiteral( "Notes — Line" ), Qgis::WkbType::LineStringZ },
      { QStringLiteral( "Notes — Polygon" ), Qgis::WkbType::PolygonZ },
    };
    for ( const auto &[name, wkbType] : expectedLayers )
    {
      const QList<QgsMapLayer *> layers = project.mapLayersByName( name );
      REQUIRE( layers.size() == 1 );
      QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( layers.at( 0 ) );
      REQUIRE( layer );
      REQUIRE( layer->isValid() );
      REQUIRE( layer->wkbType() == wkbType );
    }
  }

  SECTION( "Camera capture adds an attachments layer related to every notes layer" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Notes" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "notes" ), true },
                                  { QStringLiteral( "notes_on_lines_polygons" ), true },
                                  { QStringLiteral( "camera_capture" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.mapLayers().size() == 4 );

    QgsVectorLayer *attachmentsLayer = qobject_cast<QgsVectorLayer *>( project.mapLayersByName( QStringLiteral( "Note attachments" ) ).value( 0 ) );
    REQUIRE( attachmentsLayer );
    REQUIRE( attachmentsLayer->isValid() );
    REQUIRE( attachmentsLayer->source() == QStringLiteral( "%1/notes.gpkg|layername=notes_attachments" ).arg( created.directory() ) );
    REQUIRE( attachmentsLayer->wkbType() == Qgis::WkbType::NoGeometry );
    REQUIRE( attachmentsLayer->flags().testFlag( QgsMapLayer::Private ) );
    REQUIRE( attachmentsLayer->fields().names() == QStringList( { QStringLiteral( "fid" ), QStringLiteral( "note_layer" ), QStringLiteral( "note_uuid" ), QStringLiteral( "media" ), QStringLiteral( "description" ), QStringLiteral( "timestamp" ) } ) );
    REQUIRE( attachmentsLayer->editFormConfig().suppress() == Qgis::AttributeFormSuppression::On );

    const QgsEditorWidgetSetup mediaWidgetSetup = attachmentsLayer->editorWidgetSetup( attachmentsLayer->fields().indexOf( QStringLiteral( "media" ) ) );
    REQUIRE( mediaWidgetSetup.type() == QStringLiteral( "ExternalResource" ) );
    REQUIRE( mediaWidgetSetup.config().value( QStringLiteral( "RelativeStorage" ) ).toInt() == 1 );
    REQUIRE( mediaWidgetSetup.config().value( QStringLiteral( "DocumentViewer" ) ).toInt() == 1 );

    QStringList notesLayerIds;
    for ( const QString &name : { QStringLiteral( "Notes — Point" ), QStringLiteral( "Notes — Line" ), QStringLiteral( "Notes — Polygon" ) } )
    {
      QgsMapLayer *notesLayer = project.mapLayersByName( name ).value( 0 );
      REQUIRE( notesLayer );
      notesLayerIds << notesLayer->id();
    }

    const QgsPolymorphicRelation relation = project.relationManager()->polymorphicRelation( QStringLiteral( "notes_attachments_relation" ) );
    REQUIRE( relation.isValid() );
    REQUIRE( relation.referencingLayerId() == attachmentsLayer->id() );
    REQUIRE( relation.referencedLayerIds() == notesLayerIds );
    REQUIRE( relation.referencedLayerField() == QStringLiteral( "note_layer" ) );
    REQUIRE( relation.referencedLayerExpression() == QStringLiteral( "@layer_id" ) );
    REQUIRE( relation.fieldPairs().size() == 1 );
    REQUIRE( relation.fieldPairs().at( 0 ).referencingField() == QStringLiteral( "note_uuid" ) );
    REQUIRE( relation.fieldPairs().at( 0 ).referencedField() == QStringLiteral( "uuid" ) );

    QgsVectorLayer *notesPointLayer = qobject_cast<QgsVectorLayer *>( project.mapLayer( notesLayerIds.at( 0 ) ) );
    QgsEditFormConfig notesFormConfig = notesPointLayer->editFormConfig();
    REQUIRE( notesFormConfig.layout() == Qgis::AttributeFormLayout::DragAndDrop );

    const QList<QgsAttributeEditorElement *> formElements = notesFormConfig.invisibleRootContainer()->children();
    REQUIRE( formElements.size() == 5 );
    REQUIRE( formElements.at( 0 )->type() == Qgis::AttributeEditorType::Relation );
    REQUIRE( formElements.at( 0 )->name() == QStringLiteral( "notes_attachments_relation_%1" ).arg( notesPointLayer->id() ) );
    REQUIRE( formElements.at( 1 )->name() == QStringLiteral( "color" ) );
    REQUIRE( formElements.at( 2 )->name() == QStringLiteral( "title" ) );
    REQUIRE( formElements.at( 3 )->name() == QStringLiteral( "note" ) );
    REQUIRE( formElements.at( 4 )->name() == QStringLiteral( "timestamp" ) );
  }

  SECTION( "Tracks layer without tracking on launch opens in digitize mode" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Tracks" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "tracks" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ) ) == QStringLiteral( "digitize" ) );
    REQUIRE( project.mapLayers().size() == 1 );

    QgsVectorLayer *tracksLayer = qobject_cast<QgsVectorLayer *>( project.mapLayersByName( QStringLiteral( "Tracks" ) ).value( 0 ) );
    REQUIRE( tracksLayer );
    REQUIRE( tracksLayer->isValid() );
    REQUIRE( tracksLayer->wkbType() == Qgis::WkbType::LineStringZM );
    REQUIRE( tracksLayer->fields().names() == QStringList( { QStringLiteral( "fid" ), QStringLiteral( "color" ), QStringLiteral( "title" ), QStringLiteral( "timestamp" ) } ) );
    REQUIRE( tracksLayer->attributeDisplayName( tracksLayer->fields().indexOf( QStringLiteral( "color" ) ) ) == QStringLiteral( "Track color" ) );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/cloud_action" ) ).toString() == QStringLiteral( "offline" ) );
    REQUIRE_FALSE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_session_active" ) ).isValid() );
    REQUIRE( tracksLayer->editFormConfig().suppress() == Qgis::AttributeFormSuppression::Default );
  }

  SECTION( "Tracks layer with tracking on launch starts a tracking session" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Tracks" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "tracks" ), true },
                                  { QStringLiteral( "track_on_launch" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ) ).isEmpty() );

    QgsVectorLayer *tracksLayer = qobject_cast<QgsVectorLayer *>( project.mapLayersByName( QStringLiteral( "Tracks" ) ).value( 0 ) );
    REQUIRE( tracksLayer );
    REQUIRE( tracksLayer->editFormConfig().suppress() == Qgis::AttributeFormSuppression::On );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_session_active" ) ).toBool() );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_time_requirement_active" ) ).toBool() );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_time_requirement_interval_seconds" ) ).toInt() == 2 );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_erroneous_distance_safeguard_active" ) ).toBool() );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_erroneous_distance_safeguard_maximum_meters" ) ).toInt() == 50 );
    REQUIRE( tracksLayer->customProperty( QStringLiteral( "QFieldSync/tracking_measurement_type" ) ).toInt() == 1 );
  }

  SECTION( "Auto push to cloud is enabled with a 30 minutes interval" )
  {
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Cloud" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) },
                                  { QStringLiteral( "auto_push_to_cloud" ), true } };
    const ScopedCreatedProject created( options );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.readBoolEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "forceAutoPush" ) ) );
    REQUIRE( project.readNumEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "forceAutoPushIntervalMins" ) ) == 30 );
  }

  SECTION( "Built-in basemaps add an OpenStreetMap layer with a matching background color" )
  {
    const QList<QPair<QString, QColor>> basemaps = {
      { QStringLiteral( "colorful" ), QColor( 242, 239, 233 ) },
      { QStringLiteral( "lightgray" ), QColor( 240, 240, 240 ) },
      { QStringLiteral( "darkgray" ), QColor( 15, 15, 15 ) },
    };
    for ( const auto &[basemap, backgroundColor] : basemaps )
    {
      const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Basemap" ) },
                                    { QStringLiteral( "basemap" ), basemap } };
      const ScopedCreatedProject created( options );

      QgsProject project;
      REQUIRE( project.read( created.filePath() ) );
      REQUIRE( project.backgroundColor() == backgroundColor );
      REQUIRE( project.crs().authid() == QStringLiteral( "EPSG:3857" ) );

      QgsRasterLayer *basemapLayer = qobject_cast<QgsRasterLayer *>( project.mapLayersByName( QStringLiteral( "OpenStreetMap" ) ).value( 0 ) );
      REQUIRE( basemapLayer );
      REQUIRE( basemapLayer->providerType() == QStringLiteral( "wms" ) );
    }
  }

  SECTION( "Custom basemap sets the project crs and limits the initial extent" )
  {
    const QfGnssPositionInformation positionInformation( 36.29, -97.31, 0, 0, 0, QList<QgsSatelliteInfo>(), 0, 0, 0, 5, 5 );
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Custom Basemap" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "custom" ) },
                                  { QStringLiteral( "basemap_custom_source" ), QStringLiteral( TEST_DATA_DIR "/testgrid.tif" ) },
                                  { QStringLiteral( "basemap_custom_provider" ), QStringLiteral( "gdal" ) },
                                  { QStringLiteral( "basemap_custom_extent" ), QStringLiteral( "POLYGON((-97.33 36.28, -97.30 36.28, -97.30 36.30, -97.33 36.30, -97.33 36.28))" ) } };
    const ScopedCreatedProject created( options, positionInformation );

    QgsProject project;
    REQUIRE( project.read( created.filePath() ) );
    REQUIRE( project.crs().authid() == QStringLiteral( "EPSG:4326" ) );

    QgsRasterLayer *basemapLayer = qobject_cast<QgsRasterLayer *>( project.mapLayersByName( QStringLiteral( "Basemap" ) ).value( 0 ) );
    REQUIRE( basemapLayer );
    REQUIRE( basemapLayer->isValid() );
    REQUIRE( basemapLayer->providerType() == QStringLiteral( "gdal" ) );

    const QgsRectangle extent = readCanvasExtent( created.filePath() );
    REQUIRE( extent.xMinimum() == Approx( -97.33 ) );
    REQUIRE( extent.yMinimum() == Approx( 36.28 ) );
    REQUIRE( extent.xMaximum() == Approx( -97.30 ) );
    REQUIRE( extent.yMaximum() == Approx( 36.30 ) );
  }

  SECTION( "Initial extent surrounds the device position" )
  {
    const QfGnssPositionInformation positionInformation( 46, 7, 0, 0, 0, QList<QgsSatelliteInfo>(), 0, 0, 0, 5, 5 );
    const QVariantMap options = { { QStringLiteral( "title" ), QStringLiteral( "Position" ) },
                                  { QStringLiteral( "basemap" ), QStringLiteral( "blank" ) } };
    const ScopedCreatedProject created( options, positionInformation );

    const QgsRectangle extent = readCanvasExtent( created.filePath() );
    REQUIRE( extent.xMinimum() == Approx( 667916.9447596414 ) );
    REQUIRE( extent.yMinimum() == Approx( 5621521.486192066 ) );
    REQUIRE( extent.xMaximum() == Approx( 890555.9263461885 ) );
    REQUIRE( extent.yMaximum() == Approx( 5942074.072431108 ) );
  }
}
