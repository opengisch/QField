/***************************************************************************
                        test_layerobserver.h
                        --------------------
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
#include "layerobserver.h"
#include "utils/qfieldcloudutils.h"

QStringList getDeltaOperations( QString fileName )
{
  QStringList operations;
  QFile deltaFile( fileName );

  if ( !deltaFile.open( QIODevice::ReadOnly ) )
    return operations;

  QJsonDocument doc = QJsonDocument::fromJson( deltaFile.readAll() );

  if ( doc.isNull() )
    return operations;

  QJsonArray deltasJsonArray = doc.object().value( QStringLiteral( "deltas" ) ).toArray();

  for ( const QJsonValue &v : std::as_const( deltasJsonArray ) )
    operations.append( v.toObject().value( QStringLiteral( "method" ) ).toString() );

  return operations;
}


QString getId( QString fileName )
{
  QFile deltaFile( fileName );

  if ( !deltaFile.open( QIODevice::ReadOnly ) )
    return QString();

  QJsonDocument doc = QJsonDocument::fromJson( deltaFile.readAll() );

  if ( doc.isNull() )
    return QString();

  return doc.object().value( QStringLiteral( "id" ) ).toString();
}

TEST_CASE( "LayerObserver" )
{
  QTemporaryDir settingsDir;

  REQUIRE( settingsDir.isValid() );
  REQUIRE( QDir( settingsDir.path() ).mkpath( QStringLiteral( "cloud_projects/TEST_PROJECT_ID" ) ) );

  QDir projectDir( QStringLiteral( "%1/cloud_projects/TEST_PROJECT_ID" ).arg( settingsDir.path() ) );
  QFieldCloudUtils::setLocalCloudDirectory( settingsDir.path() );
  QFile projectFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "project.qgs" ) ) );
  QFile attachmentFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "attachment.jpg" ) ) );

  REQUIRE( projectFile.open( QIODevice::WriteOnly ) );
  REQUIRE( projectFile.flush() );

  QgsProject::instance()->setFileName( projectFile.fileName() );

  std::unique_ptr<QgsVectorLayer> mLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=str:string" ), QStringLiteral( "Test Layer" ), QStringLiteral( "memory" ) );
  mLayer->setCustomProperty( QStringLiteral( "QFieldSync/action" ), QStringLiteral( "CLOUD" ) );
  mLayer->setCustomProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ), QStringLiteral( "fid" ) );

  REQUIRE( mLayer->isValid() );

  QgsFeature f1( mLayer->fields() );
  f1.setAttribute( QStringLiteral( "fid" ), 1 );
  f1.setAttribute( QStringLiteral( "str" ), "string1" );
  f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );
  QgsFeature f2( mLayer->fields() );
  f2.setAttribute( QStringLiteral( "fid" ), 2 );
  f2.setAttribute( QStringLiteral( "str" ), "string2" );
  f2.setGeometry( QgsGeometry( new QgsPoint( 23.398819, 41.7672147 ) ) );
  QgsFeature f3( mLayer->fields() );
  f3.setAttribute( QStringLiteral( "fid" ), 3 );
  f3.setAttribute( QStringLiteral( "str" ), "string3" );

  REQUIRE( mLayer->startEditing() );
  REQUIRE( mLayer->addFeature( f1 ) );
  REQUIRE( mLayer->addFeature( f2 ) );
  REQUIRE( mLayer->addFeature( f3 ) );
  REQUIRE( mLayer->commitChanges() );

  std::unique_ptr<LayerObserver> mLayerObserver = std::make_unique<LayerObserver>( QgsProject::instance() );

  REQUIRE( QgsProject::instance()->addMapLayer( mLayer.get(), false, false ) );
  REQUIRE( !mLayerObserver->deltaFileWrapper()->hasError() );

#if 0
  SECTION( "HasError" )
  {
    // ? how I can test such thing?
    QSKIP( "decide how we test errors" );
    REQUIRE( mLayerObserver->deltaFileWrapper()->hasError() == false );
    REQUIRE( QFile::exists( mLayerObserver->deltaFileWrapper()->fileName() ) );
  }
#endif

  SECTION( "Clear" )
  {
    QgsFeature f1( mLayer->fields() );
    f1.setAttribute( QStringLiteral( "fid" ), 1000 );
    f1.setAttribute( QStringLiteral( "str" ), "new_string1" );
    f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->addFeature( f1 ) );
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size() == 1 );

    QgsFeature f2( mLayer->fields() );
    f2.setAttribute( QStringLiteral( "fid" ), 1001 );
    f2.setAttribute( QStringLiteral( "str" ), "new_string2" );
    f2.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->addFeature( f1 ) );
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size() == 2 );

    mLayerObserver->reset();
    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->commitChanges() );

    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size() == 0 );
  }


  SECTION( "ObservesEditingStopped" )
  {
    QgsFeature f1( mLayer->fields() );
    f1.setAttribute( QStringLiteral( "fid" ), 1002 );
    f1.setAttribute( QStringLiteral( "str" ), "new_string" );
    f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->addFeature( f1 ) );
    // the changes are not written on the disk yet
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList() );
    // when we stop editing, all changes are written
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList( { "create" } ) );
  }


  SECTION( "ObservesAdded" )
  {
    QgsFeature f1( mLayer->fields() );
    f1.setAttribute( QStringLiteral( "fid" ), 1003 );
    f1.setAttribute( QStringLiteral( "str" ), "new_string" );
    f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->addFeature( f1 ) );
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList( { "create" } ) );
  }


  SECTION( "ObservesRemoved" )
  {
    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->deleteFeature( 1 ) );
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList( { "delete" } ) );
  }


  SECTION( "ObservesAttributeValueChanges" )
  {
    QgsFeature f1 = mLayer->getFeature( 2 );
    f1.setAttribute( QStringLiteral( "str" ), f1.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
    QgsFeature f2 = mLayer->getFeature( 3 );
    f2.setAttribute( QStringLiteral( "str" ), f2.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
    f2.setGeometry( QgsGeometry( new QgsPoint( 88.7695313, 51.0897229 ) ) );

    REQUIRE( mLayer->startEditing() );
    REQUIRE( mLayer->updateFeature( f1 ) );
    REQUIRE( mLayer->updateFeature( f2 ) );
    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList( { "patch", "patch" } ) );
  }


  SECTION( "ObservesGeometryChanges" )
  {
    REQUIRE( mLayer->startEditing() );

    QgsFeature f1 = mLayer->getFeature( 2 );
    f1.setGeometry( QgsGeometry( new QgsPoint( 13.0545044, 47.8094654 ) ) );
    QgsFeature f2 = mLayer->getFeature( 3 );
    f2.setAttribute( QStringLiteral( "str" ), f2.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
    f2.setGeometry( QgsGeometry( new QgsPoint( 13.0545044, 47.8094654 ) ) );

    REQUIRE( mLayer->updateFeature( f1 ) );
    REQUIRE( mLayer->updateFeature( f2 ) );

    REQUIRE( mLayer->commitChanges() );
    REQUIRE( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ) == QStringList( { "patch", "patch" } ) );
  }
}
