/***************************************************************************
                        test_deltafilewrapper.h
                        -----------------------
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

#define QFIELDTEST_MAIN

#include "catch2.h"
#include "deltafilewrapper.h"
#include "qfield.h"
#include "qgsapplication.h"
#include "qgssettings.h"
#include "qgsvectorlayerjoininfo.h"
#include "utils/fileutils.h"
#include "utils/qfieldcloudutils.h"

#include <QFileInfo>
#include <qgsproject.h>

QT_BEGIN_NAMESPACE
std::ostream &operator<<( std::ostream &os, const QJsonDocument &value )
{
  os << value.toJson().constData();
  return os;
}
QT_END_NAMESPACE

QJsonArray normalizeDeltasSchema( const QJsonArray &deltasJson )
{
  QStringList localLayerIds;
  QStringList sourceLayerIds;
  QJsonArray deltas;

  // normalize layerIds
  for ( const QJsonValue &v : deltasJson )
  {
    QJsonObject deltaItem = v.toObject();
    const QString localLayerId = deltaItem.value( QStringLiteral( "localLayerId" ) ).toString();
    const QString sourceLayerId = deltaItem.value( QStringLiteral( "sourceLayerId" ) ).toString();

    if ( !localLayerIds.contains( localLayerId ) )
      localLayerIds.append( localLayerId );

    if ( !sourceLayerIds.contains( sourceLayerId ) )
      sourceLayerIds.append( sourceLayerId );

    int localLayerIdx = localLayerIds.indexOf( localLayerId ) + 1;
    int sourceLayerIdx = sourceLayerIds.indexOf( sourceLayerId ) + 1;

    deltaItem.insert( QStringLiteral( "localLayerId" ), QStringLiteral( "dummyLayerIdL%1" ).arg( localLayerIdx ) );
    deltaItem.insert( QStringLiteral( "sourceLayerId" ), QStringLiteral( "dummyLayerIdS%1" ).arg( sourceLayerIdx ) );
    deltaItem.insert( QStringLiteral( "uuid" ), QStringLiteral( "11111111-1111-1111-1111-111111111111" ) );
    deltas.append( deltaItem );
  }

  return deltas;
}


QJsonArray normalizeFilesSchema( const QJsonArray &filesJson )
{
  QJsonArray files;

  // normalize file names
  int i = 0;
  while ( i < filesJson.size() )
    files.append( QStringLiteral( "file%1.jpg" ).arg( i++ ) );

  return files;
}


/**
 * Normalized the random part of the delta file JSON schema to static values.
 * "id"         - "11111111-1111-1111-1111-111111111111"
 * "project"  - "projectId"
 *
 * @param json - JSON string
 * @return QJsonDocument normalized JSON document. NULL document if the input is invalid.
 */
QJsonDocument normalizeSchema( const QString &json )
{
  QJsonDocument doc = QJsonDocument::fromJson( json.toUtf8() );

  if ( doc.isNull() )
    return doc;

  QJsonObject o = doc.object();
  QJsonArray deltas;

  if ( o.value( QStringLiteral( "version" ) ).toString() != DeltaFormatVersion )
    return QJsonDocument();
  if ( o.value( QStringLiteral( "project" ) ).toString().size() == 0 )
    return QJsonDocument();
  if ( QUuid::fromString( o.value( QStringLiteral( "id" ) ).toString() ).isNull() )
    return QJsonDocument();
  if ( !o.value( QStringLiteral( "deltas" ) ).isArray() )
    return QJsonDocument();
  if ( !o.value( QStringLiteral( "files" ) ).isArray() )
    return QJsonDocument();

  // normalize non-constant values
  o.insert( QStringLiteral( "id" ), QStringLiteral( "11111111-1111-1111-1111-111111111111" ) );
  o.insert( QStringLiteral( "project" ), QStringLiteral( "projectId" ) );
  o.insert( QStringLiteral( "deltas" ), normalizeDeltasSchema( o.value( QStringLiteral( "deltas" ) ).toArray() ) );
  o.insert( QStringLiteral( "files" ), normalizeFilesSchema( o.value( QStringLiteral( "files" ) ).toArray() ) );

  return QJsonDocument( o );
}

QJsonArray getDeltasArray( const QString &json )
{
  return normalizeSchema( json.toUtf8() )
    .object()
    .value( QStringLiteral( "deltas" ) )
    .toArray();
}


TEST_CASE( "DeltaFileWrapper" )
{
  QgsProject *project = QgsProject::instance();
  QTemporaryDir settingsDir;
  QTemporaryFile tmpDeltaFile;
  QTemporaryDir workDir;

  REQUIRE( settingsDir.isValid() );
  REQUIRE( tmpDeltaFile.open() );
  REQUIRE( QDir( settingsDir.path() ).mkpath( QStringLiteral( "cloud_projects/TEST_PROJECT_ID" ) ) );

  QDir projectDir( QStringLiteral( "%1/cloud_projects/TEST_PROJECT_ID" ).arg( settingsDir.path() ) );
  QFieldCloudUtils::setLocalCloudDirectory( settingsDir.path() );
  QFile projectFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "project.qgs" ) ) );
  QFile attachmentFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "attachment.jpg" ) ) );

  REQUIRE( projectFile.open( QIODevice::WriteOnly ) );
  REQUIRE( projectFile.flush() );

  project->setFileName( projectFile.fileName() );

  const char *fileContents = "кирилица"; // SHA 256 71055d022f50027387eae32426a1857d6e2fa2d416d64753b63470db7f00f239
  REQUIRE( attachmentFile.open( QIODevice::ReadWrite ) );
  REQUIRE( attachmentFile.write( fileContents ) );
  REQUIRE( attachmentFile.flush() );

  QString attachmentFileName = attachmentFile.fileName();
  QString attachmentFileChecksum = FileUtils::fileChecksum( attachmentFileName ).toHex();

  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=int:integer&field=dbl:double&field=str:string&field=attachment:string" ), QStringLiteral( "layer_name" ), QStringLiteral( "memory" ) );
  layer->setEditorWidgetSetup( layer->fields().indexFromName( QStringLiteral( "attachment" ) ), QgsEditorWidgetSetup( QStringLiteral( "ExternalResource" ), QVariantMap() ) );

  std::unique_ptr<QgsVectorLayer> joinedLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "NoGeometry?field=fid:integer&field=number_field:integer" ), QStringLiteral( "joined_layer" ), QStringLiteral( "memory" ) );

  REQUIRE( layer->isValid() );
  REQUIRE( joinedLayer->isValid() );
  REQUIRE( project->addMapLayer( layer.get(), false, false ) );
  REQUIRE( project->addMapLayer( joinedLayer.get(), false, false ) );

  QFieldCloudUtils::setProjectSetting( QStringLiteral( "TEST_PROJECT_ID" ), QStringLiteral( "lastLocalExportId" ), QStringLiteral( "22222222-2222-2222-2222-222222222222" ) );
  QFieldCloudUtils::setProjectSetting( QStringLiteral( "TEST_PROJECT_ID" ), QStringLiteral( "lastExportId" ), QStringLiteral( "33333333-3333-3333-3333-333333333333" ) );

  QgsFeature f( layer->fields(), 1 );

  f.setAttribute( QStringLiteral( "fid" ), 1 );
  f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
  f.setAttribute( QStringLiteral( "int" ), 42 );
  f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
  f.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );

  REQUIRE( layer->startEditing() );
  REQUIRE( layer->addFeature( f ) );
  REQUIRE( layer->commitChanges() );

  QgsFeature jf1( joinedLayer->fields() );
  jf1.setAttribute( QStringLiteral( "fid" ), 1 );
  jf1.setAttribute( QStringLiteral( "number_field" ), 100 );

  REQUIRE( joinedLayer->startEditing() );
  REQUIRE( joinedLayer->addFeature( jf1 ) );
  REQUIRE( joinedLayer->commitChanges() );

#if 0
//  TODO enable this code once we have a single delta pointer stored per project and passed to the layer observer.
//  Now both the qfieldcloudprojects model (Read only) and the layer observer (Read/Write) create their pointers to the deltafilewrapper
  SECTION( "NoMoreThanOneInstance" )
  {
    QString fileName( wrappedDeltaFilePath );
    DeltaFileWrapper dfw1( project, fileName );

    REQUIRE( dfw1.errorType() == DeltaFileWrapper::ErrorTypes::NoError );

    DeltaFileWrapper dfw2( project, fileName );

    REQUIRE( dfw2.errorType() == DeltaFileWrapper::ErrorTypes::LockError );
  }
#endif

  SECTION( "No error with existing file" )
  {
    QString correctExistingContents = QStringLiteral( R""""(
          {
            "deltas":[],
            "files":[],
            "id":"11111111-1111-1111-1111-111111111111",
            "project":"projectId",
            "version":"1.0"
          }
        )"""" );
    REQUIRE( tmpDeltaFile.write( correctExistingContents.toUtf8() ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper correctExistingDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( correctExistingDfw.errorType() == DeltaFileWrapper::ErrorTypes::NoError );
    QJsonDocument correctExistingDoc = normalizeSchema( correctExistingDfw.toString() );
    REQUIRE( !correctExistingDoc.isNull() );
    REQUIRE( correctExistingDoc == QJsonDocument::fromJson( correctExistingContents.toUtf8() ) );
  }


  SECTION( "NoErrorNonExistingFile" )
  {
    QString fileName( workDir.filePath( QUuid::createUuid().toString() ) );
    DeltaFileWrapper dfw( project, fileName );
    REQUIRE( dfw.errorType() == DeltaFileWrapper::ErrorTypes::NoError );
    REQUIRE( QFileInfo::exists( fileName ) );
    DeltaFileWrapper validNonexistingFileCheckDfw( project, fileName );
    QFile deltaFile( fileName );
    REQUIRE( deltaFile.open( QIODevice::ReadOnly ) );
    QJsonDocument fileContents = normalizeSchema( deltaFile.readAll() );
    REQUIRE( !fileContents.isNull() );
    QJsonDocument expectedDoc = QJsonDocument::fromJson( R""""(
        {
          "deltas": [],
          "files":[],
          "id":"11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" );
    REQUIRE( fileContents == expectedDoc );
  }


  SECTION( "ErrorInvalidName" )
  {
    DeltaFileWrapper dfw( project, "" );
    REQUIRE( dfw.errorType() == DeltaFileWrapper::ErrorTypes::IOError );
  }


  SECTION( "ErrorInvalidJsonParse" )
  {
    REQUIRE( tmpDeltaFile.write( R""""( asd )"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper dfw( project, tmpDeltaFile.fileName() );
    REQUIRE( dfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonParseError );
  }


  SECTION( "ErrorJsonFormatVersionType" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":5,"files":[],"id":"11111111-1111-1111-1111-111111111111","project":"projectId","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper dfw( project, tmpDeltaFile.fileName() );
    REQUIRE( dfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatVersionError );
  }


  SECTION( "ErrorJsonFormatVersionEmpty" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"","files":[],"id":"11111111-1111-1111-1111-111111111111","project":"projectId","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper emptyVersionDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( emptyVersionDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatVersionError );
  }


  SECTION( "ErrorJsonFormatVersionValue" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id":"11111111-1111-1111-1111-111111111111","project":"projectId","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper wrongVersionNumberDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( wrongVersionNumberDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonIncompatibleVersionError );
  }


  SECTION( "ErrorJsonFormatIdType" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id": 5,"project":"projectId","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper wrongIdTypeDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( wrongIdTypeDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatIdError );
  }


  SECTION( "ErrorJsonFormatIdEmpty" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id": "","project":"projectId","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper emptyIdDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( emptyIdDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatIdError );
  }


  SECTION( "ErrorJsonFormatProjectIdType" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id": "11111111-1111-1111-1111-111111111111","project":5,"deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper wrongProjectIdTypeDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( wrongProjectIdTypeDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatProjectIdError );
  }


  SECTION( "ErrorJsonFormatProjectIdEmpty" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id": "11111111-1111-1111-1111-111111111111","project":"","deltas":[]})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper emptyProjectIdDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( emptyProjectIdDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatProjectIdError );
  }


  SECTION( "ErrorJsonFormatDeltasType" )
  {
    REQUIRE( tmpDeltaFile.write( R""""({"version":"2.0","files":[],"id": "11111111-1111-1111-1111-111111111111","project":"projectId","deltas":{}})"""" ) );
    tmpDeltaFile.flush();
    DeltaFileWrapper wrongDeltasTypeDfw( project, tmpDeltaFile.fileName() );
    REQUIRE( wrongDeltasTypeDfw.errorType() == DeltaFileWrapper::ErrorTypes::JsonFormatDeltasError );
  }


  SECTION( "FileName" )
  {
    QString fileName( QFileInfo( workDir.filePath( QUuid::createUuid().toString() ) ).absoluteFilePath() );
    DeltaFileWrapper dfw( project, fileName );
    REQUIRE( dfw.fileName() == fileName );
  }


  SECTION( "Id" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( !QUuid::fromString( dfw.id() ).isNull() );
  }


  SECTION( "Reset" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( getDeltasArray( dfw.toString() ).size() == 1 );

    dfw.reset();

    REQUIRE( getDeltasArray( dfw.toString() ).size() == 0 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( getDeltasArray( dfw.toString() ).size() == 1 );
  }


  SECTION( "ResetId" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( getDeltasArray( dfw.toString() ).size() == 0 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    const QString dfwId = dfw.id();
    dfw.resetId();

    REQUIRE( getDeltasArray( dfw.toString() ).size() == 1 );
    REQUIRE( dfwId != dfw.id() );
  }


  SECTION( "ToString" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields;
    fields.append( QgsField( "fid", QVariant::Int, "integer" ) );

    QgsFeature f1( fields, 100 );
    f1.setAttribute( "fid", 100 );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );

    QgsFeature f2( fields, 101 );
    f2.setAttribute( "fid", 101 );
    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );

    QJsonDocument doc = normalizeSchema( dfw.toString() );

    REQUIRE( !doc.isNull() );
    QJsonDocument expectedDoc = QJsonDocument::fromJson( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "EPSG:3857",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "layer_name",
              "localPk": "100",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "fid": 100
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "EPSG:3857",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "layer_name",
              "localPk": "101",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "101",
              "method": "delete",
              "old": {
                "attributes": {
                  "fid": 101
                },
                "geometry": null
              }
            }
          ],
          "files":[],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" );
    REQUIRE( doc == expectedDoc );
  }


  SECTION( "ToJson" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields;
    fields.append( QgsField( "fid", QVariant::Int, "integer" ) );

    QgsFeature f1( fields, 100 );
    f1.setAttribute( "fid", 100 );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );

    QgsFeature f2( fields, 101 );
    f2.setAttribute( "fid", 101 );
    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );

    QJsonDocument doc = normalizeSchema( QString( dfw.toJson() ) );

    REQUIRE( !doc.isNull() );
    QJsonDocument expectedDoc = QJsonDocument::fromJson( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "EPSG:3857",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "layer_name",
              "localPk": "100",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "fid": 100
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "EPSG:3857",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "layer_name",
              "localPk": "101",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "101",
              "method": "delete",
              "old": {
                "attributes": {
                  "fid": 101
                },
                "geometry": null
              }
            }
          ],
          "files":[],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" );
    REQUIRE( doc == expectedDoc );
  }


  SECTION( "ProjectId" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( dfw.projectId() == QStringLiteral( "TEST_PROJECT_ID" ) );
  }


  SECTION( "IsDirty" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( dfw.isDirty() == false );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( dfw.isDirty() == true );
    REQUIRE( dfw.toFile() );
    REQUIRE( dfw.isDirty() == false );

    dfw.reset();

    REQUIRE( dfw.isDirty() == true );
  }


  SECTION( "Count" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( dfw.count() == 0 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( dfw.count() == 1 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( dfw.count() == 2 );
  }


  SECTION( "Deltas" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    REQUIRE( QJsonDocument( dfw.deltas() ) == QJsonDocument::fromJson( "[]" ) );

    QgsFields fields;
    fields.append( QgsField( "fid", QVariant::Int, "integer" ) );
    QgsFeature f1( fields, 100 );
    f1.setAttribute( "fid", 100 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );

    QJsonDocument expectedDoc = QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "create",
            "new": {
              "attributes": {
                "fid": 100
              },
              "geometry": null
            }
          }
        ]
      )"""" );
    REQUIRE( QJsonDocument( normalizeDeltasSchema( dfw.deltas() ) ) == expectedDoc );

    QgsFeature f2( fields, 101 );
    f2.setAttribute( "fid", 101 );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );

    expectedDoc = QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "create",
            "new": {
              "attributes": {
                "fid": 100
              },
              "geometry": null
            }
          },
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "101",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "101",
            "method": "create",
            "new": {
              "attributes": {
                "fid": 101
              },
              "geometry": null
            }
          }
        ]
      )"""" );
    REQUIRE( QJsonDocument( normalizeDeltasSchema( dfw.deltas() ) ) == expectedDoc );
  }


  SECTION( "ToFile" )
  {
    QString fileName = workDir.filePath( QUuid::createUuid().toString() );
    DeltaFileWrapper dfw1( project, fileName );
    dfw1.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature() );

    REQUIRE( !dfw1.hasError() );
    REQUIRE( getDeltasArray( dfw1.toString() ).size() == 1 );
    REQUIRE( dfw1.toFile() );
    REQUIRE( getDeltasArray( dfw1.toString() ).size() == 1 );

    QFile deltaFile( fileName );
    REQUIRE( deltaFile.open( QIODevice::ReadOnly ) );
    REQUIRE( getDeltasArray( deltaFile.readAll() ).size() == 1 );
  }


  SECTION( "Append" )
  {
    DeltaFileWrapper dfw1( project, workDir.filePath( QUuid::createUuid().toString() ) );
    DeltaFileWrapper dfw2( project, workDir.filePath( QUuid::createUuid().toString() ) );
    dfw1.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), QgsFeature( QgsFields(), 100 ) );
    dfw2.append( &dfw1 );

    REQUIRE( dfw2.count() == 1 );
  }


  SECTION( "AttachmentFieldNames" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );

    QStringList attachmentFields = dfw.attachmentFieldNames( project, layer->id() );

    REQUIRE( attachmentFields == QStringList( { QStringLiteral( "attachment" ) } ) );
  }


  SECTION( "AttachmentFileNames" )
  {
    QTemporaryFile deltaFile;

    REQUIRE( deltaFile.open() );
    REQUIRE( deltaFile.write( QStringLiteral( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "100",
              "sourceLayerId": "%1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE1.jpg",
                  "dbl": 3.14,
                  "int": 42,
                  "str": "stringy"
                },
                "files_sha256": {
                  "FILE1.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE2.jpg",
                  "dbl": null,
                  "int": null,
                  "str": null
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "patch",
              "new": {
                "attributes": {
                  "attachment": "FILE3.jpg"
                },
                "files_sha256": {
                  "FILE3.jpg": null
                },
                "geometry": null
              },
              "old": {
                "attributes": {
                  "attachment": "FILE2.jpg"
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            }
          ],
          "files": [],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" )
                                .arg( layer->id() )
                                .toUtf8() ) );
    REQUIRE( deltaFile.flush() );

    DeltaFileWrapper dfw( project, deltaFile.fileName() );

    REQUIRE( !dfw.hasError() );

    QMap<QString, QString> attachmentFileNames = dfw.attachmentFileNames();
    QMap<QString, QString> expectedAttachmentFileNames(
      { { "FILE1.jpg", "" },
        { "FILE3.jpg", "" } } );

    REQUIRE( attachmentFileNames == expectedAttachmentFileNames );
  }


  SECTION( "AddCreate" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFeature f( layer->fields(), 100 );
    f.setAttribute( QStringLiteral( "fid" ), 100 );
    f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f.setAttribute( QStringLiteral( "int" ), 42 );
    f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    f.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );

    // Check if creates delta of a feature with a geometry and existing attachment
    f.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f );

    QJsonDocument expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "create",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": "%2"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)"
            }
          }
        ]
      )"""" )
                                                           .arg( attachmentFileName, attachmentFileChecksum )
                                                           .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Check if creates delta of a feature with a NULL geometry and non existant attachment.
    // NOTE this is the same as calling f clearGeometry()
    dfw.reset();
    f.setGeometry( QgsGeometry() );
    f.setAttribute( QStringLiteral( "attachment" ), workDir.filePath( QUuid::createUuid().toString() ) );
    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f );

    expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "create",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": null
              },
              "geometry": null
            }
          }
        ]
      )"""" )
                                             .arg( f.attribute( QStringLiteral( "attachment" ) ).toString() )
                                             .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Check if creates delta of a feature without attributes
    dfw.reset();

    QgsFields fields;
    fields.append( QgsField( QStringLiteral( "fid" ), QVariant::Int ) );

    QgsFeature f1( fields, 101 );
    f1.setAttribute( QStringLiteral( "fid" ), 101 );
    f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );

    expectedDoc = QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "101",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "101",
            "method": "create",
            "new": {
              "attributes":{
                "fid": 101
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)"
            }
          }
        ]
      )"""" );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );
  }


  SECTION( "AddPatch" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFeature oldFeature( layer->fields(), 100 );
    oldFeature.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    oldFeature.setAttribute( QStringLiteral( "int" ), 42 );
    oldFeature.setAttribute( QStringLiteral( "fid" ), 100 );
    oldFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    oldFeature.setAttribute( QStringLiteral( "attachment" ), QString() );
    QgsFeature newFeature( layer->fields(), 100 );
    newFeature.setAttribute( QStringLiteral( "dbl" ), 9.81 );
    newFeature.setAttribute( QStringLiteral( "int" ), 680 );
    newFeature.setAttribute( QStringLiteral( "fid" ), 100 );
    newFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "pingy" ) );
    newFeature.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );


    // Patch both the attributes with existing attachment and the geometry
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    QJsonDocument expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "patch",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 9.81,
                "int": 680,
                "str": "pingy"
              },
              "files_sha256": {
                "%1": "%2"
              },
              "geometry": "Point (23.39881899999999959 41.7672146999999967)",
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "attachment": null,
                "dbl": 3.14,
                "int": 42,
                "str": "stringy"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)",
              "is_snapshot": false
            }
          }
        ]
      )"""" )
                                                           .arg( attachmentFileName, attachmentFileChecksum )
                                                           .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Patch attributes only with non existing attachnment
    dfw.reset();
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setAttribute( QStringLiteral( "attachment" ), workDir.filePath( QUuid::createUuid().toString() ) );
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "patch",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 9.81,
                "int": 680,
                "str": "pingy"
              },
              "files_sha256": {
                "%1": null
              },
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "attachment": null,
                "dbl": 3.14,
                "int": 42,
                "str": "stringy"
              },
              "is_snapshot": false
            }
          }
        ]
      )"""" )
                                             .arg( newFeature.attribute( "attachment" ).toString() )
                                             .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Patch feature without geometry on attributes only with non existant attachment
    dfw.reset();
    newFeature.setGeometry( QgsGeometry() );
    oldFeature.setGeometry( QgsGeometry() );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature );

    // Patch geometry only, switch store snapshot on
    dfw.reset();
    newFeature.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    newFeature.setAttribute( QStringLiteral( "int" ), 42 );
    newFeature.setAttribute( QStringLiteral( "fid" ), 100 );
    newFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    newFeature.setAttribute( QStringLiteral( "attachment" ), QVariant() );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );
    oldFeature.setAttribute( QStringLiteral( "attachment" ), QVariant() );
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, true );

    expectedDoc = QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "patch",
            "new": {
              "attributes": {
              },
              "geometry": "Point (23.39881899999999959 41.7672146999999967)",
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "attachment": null,
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)",
              "is_snapshot": true
            }
          }
        ]
      )"""" );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Do not patch equal features
    dfw.reset();
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( "[]" ) );

    // Patch null values
    dfw.reset();
    newFeature.setAttribute( QStringLiteral( "dbl" ), QVariant() );
    newFeature.setAttribute( QStringLiteral( "int" ), QVariant() );
    newFeature.setAttribute( QStringLiteral( "fid" ), QVariant() );
    newFeature.setAttribute( QStringLiteral( "str" ), QVariant() );
    newFeature.setAttribute( QStringLiteral( "attachment" ), QVariant() );
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, true );

    expectedDoc = QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "patch",
            "new": {
              "attributes": {
                "dbl": null,
                "fid": null,
                "int": null,
                "str": null
              },
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "attachment": null,
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)",
              "is_snapshot": true
            }
          }
        ]
      )"""" );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );


    // Do not patch equal features
    dfw.reset();
    newFeature.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    newFeature.setAttribute( QStringLiteral( "int" ), 42 );
    newFeature.setAttribute( QStringLiteral( "fid" ), 100 );
    newFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    newFeature.setAttribute( QStringLiteral( "attachment" ), QVariant() );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( "[]" ) );
  }

  SECTION( "AddDeleteWithStringPk" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFeature f( layer->fields(), 100 );
    f.setAttribute( QStringLiteral( "fid" ), 100 );
    f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f.setAttribute( QStringLiteral( "int" ), 42 );
    f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    f.setAttribute( QStringLiteral( "attachment" ), workDir.filePath( QUuid::createUuid().toString() ) );
    f.setGeometry( QgsGeometry() );

    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "str" ), f );

    QJsonDocument expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
          [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "EPSG:3857",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "layer_name",
              "localPk": "100",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "stringy",
              "method": "delete",
              "old": {
                "attributes": {
                  "attachment": "%1",
                  "dbl": 3.14,
                  "fid": 100,
                  "int": 42,
                  "str": "stringy"
                },
                "files_sha256": {
                  "%1": null
                },
                "geometry": null
              }
            }
          ]
        )"""" )
                                                           .arg( f.attribute( QStringLiteral( "attachment" ) ).toString() )
                                                           .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );
  }

  SECTION( "AddDelete" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFeature f( layer->fields(), 100 );
    f.setAttribute( QStringLiteral( "fid" ), 100 );
    f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f.setAttribute( QStringLiteral( "int" ), 42 );
    f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    f.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );

    // Check if creates delta of a feature with a geometry and existant attachment.
    f.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    // ? why this is not working, as QgsPoint is QgsAbstractGeometry and there is example in the docs? https://qgis.org/api/classQgsFeature.html#a14dcfc99b476b613c21b8c35840ff388
    // f.setGeometry( QgsPoint( 25.9657, 43.8356 ) );
    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f );

    QJsonDocument expectedDoc = QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "delete",
            "old": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": "%2"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)"
            }
          }
        ]
      )"""" )
                                                           .arg( attachmentFileName, attachmentFileChecksum )
                                                           .toUtf8() );
    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == expectedDoc );

    // Check if creates delta of a feature with a NULL geometry and non existant attachment.
    // NOTE this is the same as calling f clearGeometry()
    dfw.reset();
    f.setGeometry( QgsGeometry() );
    f.setAttribute( QStringLiteral( "attachment" ), workDir.filePath( QUuid::createUuid().toString() ) );
    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "100",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "100",
            "method": "delete",
            "old": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 100,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": null
              },
              "geometry": null
            }
          }
        ]
      )"""" )
                                                                                             .arg( f.attribute( QStringLiteral( "attachment" ) ).toString() )
                                                                                             .toUtf8() ) );


    // Check if creates delta of a feature without attributes
    dfw.reset();

    QgsFields fields;
    fields.append( QgsField( QStringLiteral( "fid" ), QVariant::Int ) );

    QgsFeature f1( fields, 101 );

    f1.setAttribute( QStringLiteral( "fid" ), 101 );
    f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    dfw.addDelete( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "101",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "101",
            "method": "delete",
            "old": {
              "attributes": {
                "fid": 101
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)"
            }
          }
        ]
      )"""" ) );
  }

  SECTION( "MultipleDeltaAdd" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields;
    fields.append( QgsField( "dbl", QVariant::Double, "double" ) );
    fields.append( QgsField( "int", QVariant::Int, "integer" ) );
    fields.append( QgsField( "fid", QVariant::Int, "integer" ) );
    fields.append( QgsField( "str", QVariant::String, "text" ) );
    QgsFeature f1( fields, 100 );
    f1.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f1.setAttribute( QStringLiteral( "int" ), 42 );
    f1.setAttribute( QStringLiteral( "fid" ), 100 );
    f1.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );

    QgsFields fields2;
    fields2.append( QgsField( "fid", QVariant::Int, "integer" ) );
    QgsFeature f2( fields2, 101 );
    f2.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    f2.setAttribute( QStringLiteral( "fid" ), 101 );

    QgsFeature f3( fields, 102 );
    f3.setAttribute( QStringLiteral( "fid" ), 102 );

    dfw.addCreate( QStringLiteral( "dummyLayerId1" ), QStringLiteral( "dummyLayerId1" ), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f1 );
    dfw.addDelete( QStringLiteral( "dummyLayerId2" ), QStringLiteral( "dummyLayerId2" ), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f2 );
    dfw.addDelete( QStringLiteral( "dummyLayerId1" ), QStringLiteral( "dummyLayerId1" ), QStringLiteral( "fid" ), QStringLiteral( "fid" ), f3 );

    QJsonDocument doc = normalizeSchema( dfw.toString() );

    REQUIRE( !doc.isNull() );
    REQUIRE( doc == QJsonDocument::fromJson( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "",
              "localPk": "100",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "dbl": 3.14,
                  "fid": 100,
                  "int": 42,
                  "str": "stringy"
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "",
              "localLayerId": "dummyLayerIdL2",
              "localLayerName": "",
              "localPk": "101",
              "sourceLayerId": "dummyLayerIdS2",
              "sourcePk": "101",
              "method": "delete",
              "old": {
                "attributes": {
                  "fid": 101
                },
                "geometry": "Point (25.96569999999999823 43.83559999999999945)"
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerCrs": "",
              "localLayerId": "dummyLayerIdL1",
              "localLayerName": "",
              "localPk": "102",
              "sourceLayerId": "dummyLayerIdS1",
              "sourcePk": "102",
              "method": "delete",
              "old": {
                "attributes": {
                  "dbl": null,
                  "fid": 102,
                  "int": null,
                  "str": null
                },
                "geometry": null
              }
            }
          ],
          "files":[],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" ) );
  }


  SECTION( "Apply" )
  {
    QTemporaryFile deltaFile;

    REQUIRE( deltaFile.open() );
    REQUIRE( deltaFile.write( QStringLiteral( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "100",
              "sourceLayerId": "%1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE1.jpg",
                  "dbl": 3.14,
                  "fid": 100,
                  "int": 42,
                  "str": "stringy"
                },
                "files_sha256": {
                  "FILE1.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE2.jpg",
                  "dbl": null,
                  "fid": 102,
                  "int": null,
                  "str": null
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "patch",
              "new": {
                "attributes": {
                  "attachment": "FILE3.jpg"
                },
                "files_sha256": {
                  "FILE3.jpg": null
                },
                "geometry": null
              },
              "old": {
                "attributes": {
                  "attachment": "FILE2.jpg"
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "1",
              "sourceLayerId": "%1",
              "sourcePk": "1",
              "method": "delete",
              "old": {
                "attachment": "%2",
                "dbl": 3.14,
                "fid": 1,
                "int": 42,
                "str": "stringy"
              }
            }
          ],
          "files": [],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" )
                                .arg( layer->id(), attachmentFileName )
                                .toUtf8() ) );
    REQUIRE( deltaFile.flush() );

    DeltaFileWrapper dfw( project, deltaFile.fileName() );

    // make sure there is a single feature with id 1
    QgsFeature f0;
    qApp->processEvents();
    QgsFeatureIterator it0 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 1 " ) ) );
    REQUIRE( it0.nextFeature( f0 ) );
    REQUIRE( layer->featureCount() == 1 );

    REQUIRE( dfw.apply() );

    REQUIRE( layer->featureCount() == 2 );

    QgsFeature f1;
    QgsFeatureIterator it1 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 100 " ) ) );

    REQUIRE( it1.nextFeature( f1 ) );
    REQUIRE( f1.isValid() );
    REQUIRE( f1.attribute( QStringLiteral( "int" ) ) == 42 );
    REQUIRE( f1.attribute( QStringLiteral( "dbl" ) ) == 3.14 );
    REQUIRE( f1.attribute( QStringLiteral( "str" ) ) == QStringLiteral( "stringy" ) );
    REQUIRE( f1.attribute( QStringLiteral( "attachment" ) ) == QStringLiteral( "FILE1.jpg" ) );
    REQUIRE( !it1.nextFeature( f1 ) );

    QgsFeature f2;
    QgsFeatureIterator it2 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 102 " ) ) );

    REQUIRE( it2.nextFeature( f2 ) );
    REQUIRE( f2.isValid() );
    REQUIRE( f2.attribute( QStringLiteral( "attachment" ) ).toString() == QStringLiteral( "FILE3.jpg" ) );
    REQUIRE( !it2.nextFeature( f2 ) );

    QgsFeature f3;
    QgsFeatureIterator it3 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 1 " ) ) );
    REQUIRE( !it3.nextFeature( f3 ) );
  }


  SECTION( "ApplyReversed" )
  {
    QTemporaryFile deltaFile;

    REQUIRE( deltaFile.open() );
    REQUIRE( deltaFile.write( QStringLiteral( R""""(
        {
          "deltas": [
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "100",
              "sourceLayerId": "%1",
              "sourcePk": "100",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE1.jpg",
                  "dbl": 3.14,
                  "fid": 100,
                  "int": 42,
                  "str": "stringy"
                },
                "files_sha256": {
                  "FILE1.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "create",
              "new": {
                "attributes": {
                  "attachment": "FILE2.jpg",
                  "dbl": null,
                  "fid": 102,
                  "int": null,
                  "str": null
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "102",
              "sourceLayerId": "%1",
              "sourcePk": "102",
              "method": "patch",
              "new": {
                "attributes": {
                  "attachment": "FILE3.jpg"
                },
                "files_sha256": {
                  "FILE3.jpg": null
                },
                "geometry": null
              },
              "old": {
                "attributes": {
                  "attachment": "FILE2.jpg"
                },
                "files_sha256": {
                  "FILE2.jpg": null
                },
                "geometry": null
              }
            },
            {
              "uuid": "11111111-1111-1111-1111-111111111111",
              "clientId": "22222222-2222-2222-2222-222222222222",
              "exportId": "33333333-3333-3333-3333-333333333333",
              "localLayerId": "%1",
              "localPk": "1",
              "sourceLayerId": "%1",
              "sourcePk": "1",
              "method": "delete",
              "old": {
                "attributes": {
                  "attachment": "%2",
                  "dbl": 3.14,
                  "fid": 1,
                  "int": 42,
                  "str": "stringy"
                },
                "files_sha256": {
                  "%2": "%3"
                }
              }
            }
          ],
          "files": [],
          "id": "11111111-1111-1111-1111-111111111111",
          "project": "projectId",
          "version": "1.0"
        }
      )"""" )
                                .arg( layer->id(), attachmentFileName, attachmentFileChecksum )
                                .toUtf8() ) );
    REQUIRE( deltaFile.flush() );

    DeltaFileWrapper dfw( project, deltaFile.fileName() );

    // make sure there is a single feature with id 1
    QgsFeature f0;
    QgsFeatureIterator it0 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 1 " ) ) );
    REQUIRE( it0.nextFeature( f0 ) );
    REQUIRE( layer->featureCount() == 1 );

    REQUIRE( dfw.apply() );

    REQUIRE( layer->featureCount() == 2 );

    QgsFeature f1;
    QgsFeatureIterator it1 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 100 " ) ) );

    REQUIRE( it1.nextFeature( f1 ) );
    REQUIRE( f1.isValid() );
    REQUIRE( f1.attribute( QStringLiteral( "int" ) ) == 42 );
    REQUIRE( f1.attribute( QStringLiteral( "dbl" ) ) == 3.14 );
    REQUIRE( f1.attribute( QStringLiteral( "str" ) ) == QStringLiteral( "stringy" ) );
    REQUIRE( f1.attribute( QStringLiteral( "attachment" ) ) == QStringLiteral( "FILE1.jpg" ) );
    REQUIRE( !it1.nextFeature( f1 ) );

    QgsFeature f2;
    QgsFeatureIterator it2 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 102 " ) ) );

    REQUIRE( it2.nextFeature( f2 ) );
    REQUIRE( f2.isValid() );
    REQUIRE( f2.attribute( QStringLiteral( "attachment" ) ).toString() == QStringLiteral( "FILE3.jpg" ) );
    REQUIRE( !it2.nextFeature( f2 ) );

    QgsFeature f3;
    QgsFeatureIterator it3 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 1 " ) ) );
    REQUIRE( !it3.nextFeature( f3 ) );

    // ^^^ the same as apply above

    dfw.applyReversed();

    QgsFeature f4;
    QgsFeatureIterator it4 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 100 OR fid = 102 " ) ) );
    REQUIRE( !it4.nextFeature( f4 ) );

    QgsFeature f5;
    QgsFeatureIterator it5 = layer->getFeatures( QgsFeatureRequest( QgsExpression( " fid = 1 " ) ) );
    REQUIRE( it5.nextFeature( f5 ) );
    REQUIRE( f5.isValid() );
    REQUIRE( f5.attribute( QStringLiteral( "fid" ) ) == 1 );
    REQUIRE( f5.attribute( QStringLiteral( "int" ) ) == 42 );
    REQUIRE( f5.attribute( QStringLiteral( "dbl" ) ) == 3.14 );
    REQUIRE( f5.attribute( QStringLiteral( "str" ) ) == QStringLiteral( "stringy" ) );
    REQUIRE( f5.attribute( QStringLiteral( "attachment" ) ) == attachmentFileName );
    REQUIRE( !it5.nextFeature( f5 ) );
  }


  SECTION( "AddCreateWithJoinedLayer" )
  {
    QgsVectorLayerJoinInfo ji;
    ji.setTargetFieldName( QStringLiteral( "fid" ) );
    ji.setJoinLayer( joinedLayer.get() );
    ji.setJoinFieldName( QStringLiteral( "fid" ) );
    ji.setPrefix( QString( "" ) );
    ji.setEditable( false );

    REQUIRE( layer->addJoin( ji ) );

    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFeature f( layer->fields(), 2 );
    f.setAttribute( QStringLiteral( "fid" ), 2 );
    f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f.setAttribute( QStringLiteral( "int" ), 42 );
    f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    f.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );

    REQUIRE( f.isValid() );
    REQUIRE( layer->startEditing() );
    REQUIRE( layer->addFeature( f ) );
    REQUIRE( layer->commitChanges() );

    QgsFeature savedFeat = layer->getFeature( 2 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), savedFeat );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "2",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "2",
            "method": "create",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 2,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": "%2"
              },
              "geometry": null
            }
          }
        ]
      )"""" )
                                                                                             .arg( attachmentFileName, attachmentFileChecksum )
                                                                                             .toUtf8() ) );
  }


  SECTION( "AddCreateWithExpressionField" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields = layer->fields();
    QgsField expressionField( QStringLiteral( "expression" ), QVariant::String, QStringLiteral( "text" ) );

    QgsFeature f( fields, 2 );
    f.setAttribute( QStringLiteral( "fid" ), 2 );
    f.setAttribute( QStringLiteral( "dbl" ), 3.14 );
    f.setAttribute( QStringLiteral( "int" ), 42 );
    f.setAttribute( QStringLiteral( "str" ), QStringLiteral( "stringy" ) );
    f.setAttribute( QStringLiteral( "attachment" ), attachmentFileName );

    REQUIRE( f.isValid() );
    REQUIRE( layer->startEditing() );
    REQUIRE( layer->addFeature( f ) );
    REQUIRE( layer->commitChanges() );
    REQUIRE( layer->addExpressionField( QStringLiteral( " UPPER( str ) " ), expressionField ) >= 0 );

    QgsFeature savedFeat = layer->getFeature( 2 );

    dfw.addCreate( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), savedFeat );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "2",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "2",
            "method": "create",
            "new": {
              "attributes": {
                "attachment": "%1",
                "dbl": 3.14,
                "fid": 2,
                "int": 42,
                "str": "stringy"
              },
              "files_sha256": {
                "%1": "%2"
              },
              "geometry": null
            }
          }
        ]
      )"""" )
                                                                                             .arg( attachmentFileName, attachmentFileChecksum )
                                                                                             .toUtf8() ) );
  }


  SECTION( "AddPatchWithJoinedLayer" )
  {
    QgsVectorLayerJoinInfo ji;
    ji.setTargetFieldName( QStringLiteral( "fid" ) );
    ji.setJoinLayer( joinedLayer.get() );
    ji.setJoinFieldName( QStringLiteral( "fid" ) );
    ji.setPrefix( QString( "" ) );
    ji.setEditable( false );

    REQUIRE( layer->addJoin( ji ) );

    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields = layer->fields();
    QgsField expressionField( QStringLiteral( "expression" ), QVariant::String, QStringLiteral( "text" ) );

    QgsFeature oldFeature = layer->getFeature( 1 );
    QgsFeature newFeature = layer->getFeature( 1 );

    REQUIRE( newFeature.setAttribute( QStringLiteral( "dbl" ), 9.81 ) );
    REQUIRE( newFeature.setAttribute( QStringLiteral( "int" ), 680 ) );
    REQUIRE( newFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "pingy" ) ) );

    // Patch both the attributes with existing attachment and the geometry
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "1",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "1",
            "method": "patch",
            "new": {
              "attributes": {
                "dbl": 9.81,
                "int": 680,
                "str": "pingy"
              },
              "geometry": "Point (23.39881899999999959 41.7672146999999967)",
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "dbl": 3.14,
                "int": 42,
                "str": "stringy"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)",
              "is_snapshot": false
            }
          }
        ]
      )"""" )
                                                                                             .toUtf8() ) );
  }


  SECTION( "AddPatchWithExpressionField" )
  {
    DeltaFileWrapper dfw( project, workDir.filePath( QUuid::createUuid().toString() ) );
    QgsFields fields = layer->fields();
    QgsField expressionField( QStringLiteral( "expression" ), QVariant::String, QStringLiteral( "text" ) );

    // this is the easiest way to create an expression field
    REQUIRE( layer->addExpressionField( QStringLiteral( " UPPER( str ) " ), expressionField ) >= 0 );

    QgsFeature oldFeature = layer->getFeature( 1 );
    QgsFeature newFeature = layer->getFeature( 1 );

    REQUIRE( newFeature.setAttribute( QStringLiteral( "dbl" ), 9.81 ) );
    REQUIRE( newFeature.setAttribute( QStringLiteral( "int" ), 680 ) );
    REQUIRE( newFeature.setAttribute( QStringLiteral( "str" ), QStringLiteral( "pingy" ) ) );

    // Patch both the attributes with existing attachment and the geometry
    oldFeature.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
    newFeature.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );

    dfw.addPatch( layer->id(), layer->id(), QStringLiteral( "fid" ), QStringLiteral( "fid" ), oldFeature, newFeature, false );

    REQUIRE( QJsonDocument( getDeltasArray( dfw.toString() ) ) == QJsonDocument::fromJson( QStringLiteral( R""""(
        [
          {
            "uuid": "11111111-1111-1111-1111-111111111111",
            "clientId": "22222222-2222-2222-2222-222222222222",
            "exportId": "33333333-3333-3333-3333-333333333333",
            "localLayerCrs": "EPSG:3857",
            "localLayerId": "dummyLayerIdL1",
            "localLayerName": "layer_name",
            "localPk": "1",
            "sourceLayerId": "dummyLayerIdS1",
            "sourcePk": "1",
            "method": "patch",
            "new": {
              "attributes": {
                "dbl": 9.81,
                "int": 680,
                "str": "pingy"
              },
              "geometry": "Point (23.39881899999999959 41.7672146999999967)",
              "is_snapshot": false
            },
            "old": {
              "attributes": {
                "dbl": 3.14,
                "int": 42,
                "str": "stringy"
              },
              "geometry": "Point (25.96569999999999823 43.83559999999999945)",
              "is_snapshot": false
            }
          }
        ]
      )"""" )
                                                                                             .toUtf8() ) );
  }

  project->removeMapLayer( layer.get() );
  project->removeMapLayer( joinedLayer.get() );
}
