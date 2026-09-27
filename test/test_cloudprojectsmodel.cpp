/***************************************************************************
                        test_cloudprojectsmodel.cpp
                        ---------------------------
  begin                : Sep 2026
  copyright            : (C) 2026 by tw0b33rs
  email                : 28195707+tw0b33rs@users.noreply.github.com
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
#include "qfcloudconnection.h"
#include "qfcloudprojectsmodel.h"
#include "qfdeltafilewrapper.h"
#include "qflayerobserver.h"
#include "utils/qfcloudutils.h"

#include <QDir>
#include <QFile>
#include <QPointer>
#include <QSettings>
#include <QTemporaryDir>
#include <qgsproject.h>

TEST_CASE( "CloudProjectsModel" )
{
  static QTemporaryDir dataDir;

  REQUIRE( dataDir.isValid() );

  QSettings::setDefaultFormat( QSettings::IniFormat );
  QSettings::setPath( QSettings::IniFormat, QSettings::UserScope, dataDir.path() );

  const QString username( QStringLiteral( "surveyor" ) );
  const QString projectId( QStringLiteral( "00000000-0000-4000-8000-000000000001" ) );
  const QString cloudDirectory = QStringLiteral( "%1/cloud_projects" ).arg( dataDir.path() );
  const QString projectDirectory = QStringLiteral( "%1/%2/%3" ).arg( cloudDirectory, username, projectId );

  QfCloudUtils::setLocalCloudDirectory( cloudDirectory );

  REQUIRE( QDir().mkpath( projectDirectory ) );

  QFile projectFile( QStringLiteral( "%1/project.qgs" ).arg( projectDirectory ) );

  REQUIRE( projectFile.open( QIODevice::WriteOnly ) );

  projectFile.close();

  QfCloudUtils::setProjectSetting( projectId, QStringLiteral( "name" ), QStringLiteral( "Test project" ) );
  QfCloudUtils::setProjectSetting( projectId, QStringLiteral( "owner" ), username );
  QfCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastLocalExportId" ), QStringLiteral( "TEST_EXPORT_ID" ) );

  QfCloudConnection connection;
  QfLayerObserver layerObserver( QgsProject::instance() );
  QfCloudProjectsModel model;

  model.setCloudConnection( &connection );
  model.setLayerObserver( &layerObserver );

  connection.setUsername( username );
  model.setCurrentProjectId( projectId );

  REQUIRE( model.currentProject() );
  REQUIRE( layerObserver.deltaFileWrapper() );

  SECTION( "TheLayerObserverSurvivesAProjectListRebuild" )
  {
    QPointer<QfDeltaFileWrapper> wrapperBefore = layerObserver.deltaFileWrapper();

    connection.setUsername( QStringLiteral( "somebody-else" ) );
    connection.setUsername( username );

    REQUIRE( wrapperBefore.isNull() );
    REQUIRE( model.currentProject() );
    REQUIRE( layerObserver.deltaFileWrapper() );
    REQUIRE( layerObserver.deltaFileWrapper() == model.currentProject()->deltaFileWrapper() );
  }

  SECTION( "TheLayerObserverIsDetachedWhenTheProjectIsGone" )
  {
    connection.setUsername( QStringLiteral( "somebody-else" ) );

    REQUIRE( !model.currentProject() );
    REQUIRE( !layerObserver.deltaFileWrapper() );
  }

  QfCloudUtils::setLocalCloudDirectory( QString() );
}
