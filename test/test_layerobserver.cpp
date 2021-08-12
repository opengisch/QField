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

#include "layerobserver.h"
#include "qfield_testbase.h"
#include "utils/qfieldcloudutils.h"

#include <gtest/gtest.h>


class TestLayerObserver : public ::testing::Test
{
  protected:

    void SetUp()
    {
      QTemporaryDir settingsDir;
      settingsDir.setAutoRemove( false );

      EXPECT_TRUE( settingsDir.isValid() ) << "Failed to create temp dir";
      EXPECT_TRUE( QDir( settingsDir.path() ).mkpath( QStringLiteral( "cloud_projects/TEST_PROJECT_ID" ) ) ) << "Failed to create project dir";

      QDir projectDir( QStringLiteral( "%1/cloud_projects/TEST_PROJECT_ID" ).arg( settingsDir.path() ) );
      QFieldCloudUtils::sQgisSettingsDirPath = settingsDir.path();
      QFile projectFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "project.qgs" ) ) );
      QFile attachmentFile( QStringLiteral( "%1/%2" ).arg( projectDir.path(), QStringLiteral( "attachment.jpg" ) ) );

      EXPECT_TRUE( projectFile.open( QIODevice::WriteOnly ) );
      EXPECT_TRUE( projectFile.flush() );

      QgsProject::instance()->setFileName( projectFile.fileName() );

      mLayer.reset( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=fid:integer&field=str:string" ), QStringLiteral( "Test Layer" ), QStringLiteral( "memory" ) ) );
      mLayer->setCustomProperty( QStringLiteral( "QFieldSync/action" ), QStringLiteral( "CLOUD" ) );
      mLayer->setCustomProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ), QStringLiteral( "fid" ) );

      EXPECT_TRUE( mLayer->isValid() );

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

      EXPECT_TRUE( mLayer->startEditing() );
      EXPECT_TRUE( mLayer->addFeature( f1 ) );
      EXPECT_TRUE( mLayer->addFeature( f2 ) );
      EXPECT_TRUE( mLayer->addFeature( f3 ) );
      EXPECT_TRUE( mLayer->commitChanges() );

      mLayerObserver.reset( new LayerObserver( QgsProject::instance() ) );

      EXPECT_TRUE( QgsProject::instance()->addMapLayer( mLayer.get(), false, false ) );
      EXPECT_TRUE( !mLayerObserver->deltaFileWrapper()->hasError() );
    }

    std::unique_ptr<QgsVectorLayer> mLayer;
    std::unique_ptr<LayerObserver> mLayerObserver;


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
};

#if 0
TEST_F( TestLayerObserver, HasError )
{
  // ? how I can test such thing?
  QSKIP( "decide how we test errors" );
  EXPECT_EQ( mLayerObserver->deltaFileWrapper()->hasError(), false );
  EXPECT_TRUE( QFile::exists( mLayerObserver->deltaFileWrapper()->fileName() ) );
}
#endif

TEST_F( TestLayerObserver, Clear )
{
  QgsFeature f1( mLayer->fields() );
  f1.setAttribute( QStringLiteral( "fid" ), 1000 );
  f1.setAttribute( QStringLiteral( "str" ), "new_string1" );
  f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->addFeature( f1 ) );
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size(), 1 );

  QgsFeature f2( mLayer->fields() );
  f2.setAttribute( QStringLiteral( "fid" ), 1001 );
  f2.setAttribute( QStringLiteral( "str" ), "new_string2" );
  f2.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->addFeature( f1 ) );
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size(), 2 );

  mLayerObserver->reset();
  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->commitChanges() );

  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ).size(), 0 );
}


TEST_F( TestLayerObserver, ObservesEditingStopped )
{
  QgsFeature f1( mLayer->fields() );
  f1.setAttribute( QStringLiteral( "fid" ), 1002 );
  f1.setAttribute( QStringLiteral( "str" ), "new_string" );
  f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->addFeature( f1 ) );
  // the changes are not written on the disk yet
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList() );
  // when we stop editing, all changes are written
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList( { "create" } ) );
}


TEST_F( TestLayerObserver, ObservesAdded )
{
  QgsFeature f1( mLayer->fields() );
  f1.setAttribute( QStringLiteral( "fid" ), 1003 );
  f1.setAttribute( QStringLiteral( "str" ), "new_string" );
  f1.setGeometry( QgsGeometry( new QgsPoint( 25.9657, 43.8356 ) ) );

  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->addFeature( f1 ) );
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList( { "create" } ) );
}


TEST_F( TestLayerObserver, ObservesRemoved )
{
  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->deleteFeature( 1 ) );
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList( { "delete" } ) );
}


TEST_F( TestLayerObserver, ObservesAttributeValueChanges )
{
  QgsFeature f1 = mLayer->getFeature( 2 );
  f1.setAttribute( QStringLiteral( "str" ), f1.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
  QgsFeature f2 = mLayer->getFeature( 3 );
  f2.setAttribute( QStringLiteral( "str" ), f2.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
  f2.setGeometry( QgsGeometry( new QgsPoint( 88.7695313, 51.0897229 ) ) );

  EXPECT_TRUE( mLayer->startEditing() );
  EXPECT_TRUE( mLayer->updateFeature( f1 ) );
  EXPECT_TRUE( mLayer->updateFeature( f2 ) );
  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList( { "patch", "patch" } ) );
}


TEST_F( TestLayerObserver, ObservesGeometryChanges )
{
  EXPECT_TRUE( mLayer->startEditing() );

  QgsFeature f1 = mLayer->getFeature( 2 );
  f1.setGeometry( QgsGeometry( new QgsPoint( 13.0545044, 47.8094654 ) ) );
  QgsFeature f2 = mLayer->getFeature( 3 );
  f2.setAttribute( QStringLiteral( "str" ), f2.attribute( QStringLiteral( "str" ) ).toString() + "_new" );
  f2.setGeometry( QgsGeometry( new QgsPoint( 13.0545044, 47.8094654 ) ) );

  EXPECT_TRUE( mLayer->updateFeature( f1 ) );
  EXPECT_TRUE( mLayer->updateFeature( f2 ) );

  EXPECT_TRUE( mLayer->commitChanges() );
  EXPECT_EQ( getDeltaOperations( mLayerObserver->deltaFileWrapper()->fileName() ), QStringList( { "patch", "patch" } ) );
}
