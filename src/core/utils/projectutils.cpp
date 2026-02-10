/***************************************************************************
  projectutils.cpp - ProjectUtils

 ---------------------
 begin                : 19.04.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "layerutils.h"
#include "platformutilities.h"
#include "positioningutils.h"
#include "projectutils.h"

#include <qgsmaplayer.h>
#include <qgsprojectdisplaysettings.h>
#include <qgsrasterlayer.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayer.h>
#include <qgsvectortilelayer.h>
#include <qgsvectortileutils.h>

ProjectUtils::ProjectUtils( QObject *parent )
  : QObject( parent )
{
}

QVariantMap ProjectUtils::mapLayers( QgsProject *project )
{
  if ( !project )
    return QVariantMap();

  QVariantMap mapLayers;
  const QMap<QString, QgsMapLayer *> projectMapLayers = project->mapLayers();
  for ( const QString &layerId : projectMapLayers.keys() )
  {
    mapLayers.insert( layerId, QVariant::fromValue<QgsMapLayer *>( projectMapLayers[layerId] ) );
  }

  return mapLayers;
}

bool ProjectUtils::addMapLayer( QgsProject *project, QgsMapLayer *layer )
{
  if ( !project )
    return false;

  return ( project->addMapLayer( layer ) );
}

void ProjectUtils::removeMapLayer( QgsProject *project, QgsMapLayer *layer )
{
  if ( !project || !layer )
    return;

  project->removeMapLayer( layer );
}

void ProjectUtils::removeMapLayer( QgsProject *project, const QString &layerId )
{
  if ( !project || layerId.isEmpty() )
    return;

  project->removeMapLayer( layerId );
}

Qgis::TransactionMode ProjectUtils::transactionMode( QgsProject *project )
{
  if ( !project )
    return Qgis::TransactionMode::Disabled;

  return project->transactionMode();
}

QString ProjectUtils::title( QgsProject *project )
{
  if ( !project )
    return QString();

  const QString title = project->title();
  return !title.isEmpty() ? title : QFileInfo( project->fileName() ).completeBaseName();
}

QString ProjectUtils::createProject( const QVariantMap &options, const GnssPositionInformation &positionInformation )
{
  QString projectTitle = options.value( QStringLiteral( "title" ), tr( "Created Project" ) ).toString();
  QString projectFilename = projectTitle;
  projectFilename.replace( QRegularExpression( "[^A-Za-z0-9_]" ), QStringLiteral( "_" ) );

  QDir createdProjectsDir( QStringLiteral( "%1/Created Projects/" ).arg( PlatformUtilities::instance()->applicationDirectory() ) );
  QString createdProjectDir = createdProjectsDir.filePath( projectFilename );
  int uniqueSuffix = 2;
  while ( QFileInfo::exists( createdProjectDir ) )
  {
    createdProjectDir = QStringLiteral( "%1_%2" ).arg( createdProjectsDir.filePath( projectFilename ), QString::number( uniqueSuffix++ ) );
  }
  createdProjectDir = QDir::cleanPath( createdProjectDir );
  createdProjectsDir.mkpath( createdProjectDir );
  const QString projectFilepath = QStringLiteral( "%1/%2.qgz" ).arg( createdProjectDir, projectFilename );

  QList<QgsMapLayer *> createdProjectLayers;
  QgsProject *createdProject = new QgsProject();

  // Basic project settings
  createdProject->setCrs( QgsCoordinateReferenceSystem( "EPSG:3857" ) );
  createdProject->displaySettings()->setCoordinateType( Qgis::CoordinateDisplayType::CustomCrs );
  createdProject->displaySettings()->setCoordinateCustomCrs( QgsCoordinateReferenceSystem( "EPSG:4326" ) );

  // Notes layer
  QgsVectorLayer *notesLayer = nullptr;
  if ( options.value( QStringLiteral( "notes" ) ).toBool() )
  {
    createdProject->writeEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ), QStringLiteral( "digitize" ) );

    const QString notesFilepath = QStringLiteral( "%1/notes.gpkg" ).arg( createdProjectDir );

    QgsFields fields;
    if ( options.value( QStringLiteral( "camera_capture" ) ).toBool() )
    {
      fields.append( QgsField( QStringLiteral( "media" ), QMetaType::QString ) );
    }
    fields.append( QgsField( QStringLiteral( "color" ), QMetaType::QString ) );
    fields.append( QgsField( QStringLiteral( "title" ), QMetaType::QString ) );
    fields.append( QgsField( QStringLiteral( "note" ), QMetaType::QString ) );
    fields.append( QgsField( QStringLiteral( "timestamp" ), QMetaType::QDateTime ) );

    QgsVectorFileWriter::SaveVectorOptions writerOptions;
    QgsVectorFileWriter *writer = QgsVectorFileWriter::create( notesFilepath, fields, Qgis::WkbType::PointZ, QgsCoordinateReferenceSystem( "EPSG:4326" ), createdProject->transformContext(), writerOptions );
    delete writer;

    notesLayer = new QgsVectorLayer( notesFilepath, tr( "Notes" ) );
    fields = notesLayer->fields();
    LayerUtils::setDefaultRenderer( notesLayer, nullptr, options.value( QStringLiteral( "camera_capture" ) ).toBool() ? QStringLiteral( "media" ) : QString(), QStringLiteral( "color" ) );
    LayerUtils::setDefaultLabeling( notesLayer );

    // Set a nice display expression for the feature list
    notesLayer->setDisplayExpression( "COALESCE( title , 'Note #' || fid || ' from ' || format_date( timestamp, 'yyyy-MM-dd HH:mm' ) )" );

    int fieldIndex;
    QVariantMap widgetOptions;
    QgsEditorWidgetSetup widgetSetup;

    // Configure fid field
    fieldIndex = fields.indexOf( QStringLiteral( "fid" ) );
    if ( fieldIndex >= 0 )
    {
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "Hidden" ), widgetOptions );
      notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
    }

    // Configure time field
    fieldIndex = fields.indexOf( QStringLiteral( "timestamp" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetOptions[QStringLiteral( "display_format" )] = QStringLiteral( "yyyy-MM-dd HH:mm" );
      widgetOptions[QStringLiteral( "field_format" )] = QStringLiteral( "yyyy-MM-dd HH:mm" );
      widgetOptions[QStringLiteral( "field_format_overwrite" )] = true;
      widgetOptions[QStringLiteral( "allow_null" )] = true;
      widgetOptions[QStringLiteral( "calendar_popup" )] = true;
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "DateTime" ), widgetOptions );
      notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      notesLayer->setDefaultValueDefinition( fieldIndex, QgsDefaultValue( QStringLiteral( "now()" ), false ) );
      //notesLayer->setFieldAlias( fieldIndex, tr( "Time" ) );
      notesLayer->setFieldAlias( fieldIndex, tr( "시간" ) );
    }

    // Configure color field
    fieldIndex = fields.indexOf( QStringLiteral( "color" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "Color" ), widgetOptions );
      notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      notesLayer->setDefaultValueDefinition( fieldIndex, QgsDefaultValue( QStringLiteral( "'#377eb8'" ), false ) );
      //notesLayer->setFieldAlias( fieldIndex, tr( "Marker color" ) );
      notesLayer->setFieldAlias( fieldIndex, tr( "마커 색상" ) );
    }

    // Configure note field
    fieldIndex = fields.indexOf( QStringLiteral( "제목" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), widgetOptions );
      notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      //notesLayer->setFieldAlias( fieldIndex, tr( "Title" ) );
      notesLayer->setFieldAlias( fieldIndex, tr( "제목" ) );
    }

    // Configure note field
    //fieldIndex = fields.indexOf( QStringLiteral( "note" ) );
    fieldIndex = fields.indexOf( QStringLiteral( "메모" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetOptions[QStringLiteral( "IsMultiline" )] = true;
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), widgetOptions );
      notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      //notesLayer->setFieldAlias( fieldIndex, tr( "Note" ) );
      notesLayer->setFieldAlias( fieldIndex, tr( "메모" ) );
    }

    if ( options.value( QStringLiteral( "camera_capture" ) ).toBool() )
    {
      // Configure camera field
      //fieldIndex = fields.indexOf( QStringLiteral( "media" ) );
      fieldIndex = fields.indexOf( QStringLiteral( "사진 / 영상" ) );
      if ( fieldIndex >= 0 )
      {
        widgetOptions.clear();
        widgetOptions[QStringLiteral( "DocumentViewer" )] = 1;
        widgetOptions[QStringLiteral( "RelativeStorage" )] = 1;
        widgetOptions[QStringLiteral( "FileWidget" )] = true;
        widgetOptions[QStringLiteral( "FileWidgetButton" )] = true;
        widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "ExternalResource" ), widgetOptions );
        notesLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
        notesLayer->setFieldAlias( fieldIndex, tr( "Media" ) );
      }
    }

    // Insure the layer is ready cloud-friendly
    notesLayer->setCustomProperty( QStringLiteral( "QFieldSync/cloud_action" ), QStringLiteral( "offline" ) );
    notesLayer->setCustomProperty( QStringLiteral( "QFieldSync/action" ), QStringLiteral( "offline" ) );

    notesLayer->setDisplayExpression( QStringLiteral( "\"title\"" ) );

    createdProjectLayers << notesLayer;
  }

  // Tracks layer
  QgsVectorLayer *tracksLayer = nullptr;
  if ( options.value( QStringLiteral( "tracks" ) ).toBool() )
  {
    const QString tracksFilepath = QStringLiteral( "%1/tracks.gpkg" ).arg( createdProjectDir );

    QgsFields fields;
    fields.append( QgsField( QStringLiteral( "color" ), QMetaType::QString ) );
    fields.append( QgsField( QStringLiteral( "title" ), QMetaType::QString ) );
    fields.append( QgsField( QStringLiteral( "timestamp" ), QMetaType::QDateTime ) );
    QgsVectorFileWriter::SaveVectorOptions writerOptions;
    QgsVectorFileWriter *writer = QgsVectorFileWriter::create( tracksFilepath, fields, Qgis::WkbType::LineStringZM, QgsCoordinateReferenceSystem( "EPSG:4326" ), createdProject->transformContext(), writerOptions );
    delete writer;

    tracksLayer = new QgsVectorLayer( tracksFilepath, tr( "Tracks" ) );
    fields = tracksLayer->fields();
    LayerUtils::setDefaultRenderer( tracksLayer, nullptr, QString(), QStringLiteral( "color" ) );

    // Set a nice display expression for the feature list
    tracksLayer->setDisplayExpression( "'Track #' || fid || ' from ' || format_date( timestamp, 'yyyy-MM-dd HH:mm' )" );

    int fieldIndex;
    QVariantMap widgetOptions;
    QgsEditorWidgetSetup widgetSetup;

    // Configure fid field
    fieldIndex = fields.indexOf( QStringLiteral( "fid" ) );
    if ( fieldIndex >= 0 )
    {
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "Hidden" ), widgetOptions );
      tracksLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
    }

    // Configure color field
    fieldIndex = fields.indexOf( QStringLiteral( "color" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "Color" ), widgetOptions );
      tracksLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      tracksLayer->setDefaultValueDefinition( fieldIndex, QgsDefaultValue( QStringLiteral( "'#377eb8'" ), false ) );
      tracksLayer->setFieldAlias( fieldIndex, tr( "Track color" ) );
    }

    // Configure note field
    fieldIndex = fields.indexOf( QStringLiteral( "title" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "TextEdit" ), widgetOptions );
      tracksLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      tracksLayer->setFieldAlias( fieldIndex, tr( "Title" ) );
    }

    // Configure time field
    fieldIndex = fields.indexOf( QStringLiteral( "timestamp" ) );
    if ( fieldIndex >= 0 )
    {
      widgetOptions.clear();
      widgetOptions[QStringLiteral( "display_format" )] = QStringLiteral( "yyyy-MM-dd HH:mm" );
      widgetOptions[QStringLiteral( "field_format" )] = QStringLiteral( "yyyy-MM-dd HH:mm" );
      widgetOptions[QStringLiteral( "field_format_overwrite" )] = true;
      widgetOptions[QStringLiteral( "allow_null" )] = true;
      widgetOptions[QStringLiteral( "calendar_popup" )] = true;
      widgetSetup = QgsEditorWidgetSetup( QStringLiteral( "DateTime" ), widgetOptions );
      tracksLayer->setEditorWidgetSetup( fieldIndex, widgetSetup );
      tracksLayer->setDefaultValueDefinition( fieldIndex, QgsDefaultValue( QStringLiteral( "now()" ), false ) );
      tracksLayer->setFieldAlias( fieldIndex, tr( "Time" ) );
    }

    if ( options.value( QStringLiteral( "track_on_launch" ) ).toBool() )
    {
      // Skip feature form when launching tracks
      QgsEditFormConfig formConfig = tracksLayer->editFormConfig();
      formConfig.setSuppress( Qgis::AttributeFormSuppression::On );
      tracksLayer->setEditFormConfig( formConfig );

      // Launch tracks on project opening
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_session_active" ), true );
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_time_requirement_active" ), true );
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_time_requirement_interval_seconds" ), 2 );
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_erroneous_distance_safeguard_active" ), true );
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_erroneous_distance_safeguard_maximum_meters" ), 50 );
      tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/tracking_measurement_type" ), 1 ); // Attach epoch value to the M value
    }
    else
    {
      createdProject->writeEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "initialMapMode" ), QStringLiteral( "digitize" ) );
    }

    // Insure the layer is ready cloud-friendly
    tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/cloud_action" ), QStringLiteral( "offline" ) );
    tracksLayer->setCustomProperty( QStringLiteral( "QFieldSync/action" ), QStringLiteral( "offline" ) );

    createdProjectLayers << tracksLayer;
  }

  // Basemap
  QgsMapLayer *basemapLayer = nullptr;
  const QString basemap = options.value( QStringLiteral( "basemap" ), QStringLiteral( "color" ) ).toString();
  if ( basemap.compare( QStringLiteral( "custom" ) ) == 0 )
  {
    QString basemapUrl = options.value( QStringLiteral( "basemap_url" ) ).toString();
    if ( !basemapUrl.isEmpty() )
    {
      if ( basemapUrl.endsWith( QStringLiteral( ".json" ), Qt::CaseInsensitive ) )
      {
        // Vector tile layer style URL
        QgsDataSourceUri uri;
        uri.setParam( QStringLiteral( "type" ), QStringLiteral( "xyz" ) );
        uri.setParam( QStringLiteral( "styleUrl" ), basemapUrl );
        uri.setParam( QStringLiteral( "zmin" ), QString::number( 0 ) );
        uri.setParam( QStringLiteral( "zmax" ), QString::number( 14 ) );
        basemapUrl = uri.encodedUri();

        QgsVectorTileUtils::updateUriSources( basemapUrl );
        QgsVectorTileLayer *layer = new QgsVectorTileLayer( basemapUrl, tr( "Basemap" ) );
        QString error;
        QStringList warnings;
        QList<QgsMapLayer *> subLayers;
        layer->loadDefaultStyleAndSubLayers( error, warnings, subLayers );
        basemapLayer = layer;
      }
      else if ( basemapUrl.contains( QStringLiteral( "{z}" ), Qt::CaseInsensitive ) || basemapUrl.contains( QStringLiteral( "{q}" ), Qt::CaseInsensitive ) )
      {
        // XYZ raster layer URL
        QgsDataSourceUri uri;
        uri.setParam( QStringLiteral( "type" ), QStringLiteral( "xyz" ) );
        uri.setParam( QStringLiteral( "url" ), basemapUrl );
        uri.setParam( QStringLiteral( "zmin" ), QString::number( 0 ) );
        uri.setParam( QStringLiteral( "zmax" ), QString::number( 19 ) );
        basemapUrl = uri.encodedUri();

        basemapLayer = new QgsRasterLayer( basemapUrl, tr( "Basemap" ), QStringLiteral( "wms" ) );
      }
    }
  }
  else if ( basemap.compare( QStringLiteral( "blank" ) ) != 0 )
  {
    basemapLayer = LayerUtils::createBasemap( basemap );
    if ( basemap.compare( QStringLiteral( "darkgray" ) ) == 0 )
    {
      createdProject->setBackgroundColor( QColor( 15, 15, 15 ) );
    }
    else if ( basemap.compare( QStringLiteral( "lightgray" ) ) == 0 )
    {
      createdProject->setBackgroundColor( QColor( 240, 240, 240 ) );
    }
    else
    {
      createdProject->setBackgroundColor( QColor( 242, 239, 233 ) );
    }
  }

  if ( basemapLayer && basemapLayer->isValid() )
  {
    createdProjectLayers << basemapLayer;
    createdProject->setCrs( basemapLayer->crs() );
  }

  // Insure attachment directories are populated in preparation for cloud project
  createdProject->writeEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "attachmentDirs" ), QStringList() << "DCIM"
                                                                                                                << "audio"
                                                                                                                << "video"
                                                                                                                << "files" );

  createdProject->addMapLayers( createdProjectLayers );

  connect( createdProject, &QgsProject::writeProject, [createdProject, basemapLayer, positionInformation]( QDomDocument &document ) {
    QDomNodeList nodes = document.elementsByTagName( "qgis" );
    if ( !nodes.isEmpty() )
    {
      QDomNode node = nodes.item( 0 );
      QDomElement element = node.toElement();

      QDomElement canvasElement = document.createElement( QStringLiteral( "mapcanvas" ) );
      canvasElement.setAttribute( QStringLiteral( "name" ), QStringLiteral( "theMapCanvas" ) );

      node.appendChild( canvasElement );

      QgsRectangle extent = PositioningUtils::createExtentForDevice( positionInformation, createdProject->crs() );
      if ( !extent.isEmpty() )
      {
        QgsMapSettings mapSettings;
        mapSettings.setDestinationCrs( createdProject->crs() );
        mapSettings.setOutputSize( QSize( 500, 500 ) );
        mapSettings.setExtent( extent );
        mapSettings.writeXml( canvasElement, document );
      }
    }
  } );

  const bool written = createdProject->write( projectFilepath );
  createdProject->clear();
  createdProject->deleteLater();

  return written ? projectFilepath : QString();
}
