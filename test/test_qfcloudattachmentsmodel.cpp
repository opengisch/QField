/***************************************************************************
  test_qfcloudattachmentsmodel.cpp - TestQfCloudAttachmentsModel

 ---------------------
 begin                : Oct 2026
 copyright            : (C) 2026 by Kaustuv Pokharel
 email                : kaustuv@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN

#include "catch2.h"
#include "qfieldcloud/qfcloudattachmentsmodel.h"
#include "qfieldcloud/qfcloudconnection.h"
#include "qfieldcloud/qfcloudproject.h"
#include "utils/qfcloudutils.h"

#include <QDir>
#include <QFile>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>


TEST_CASE( "QfCloudAttachmentsModel watches the attachments files" )
{
  QTemporaryDir cloudDir;
  REQUIRE( cloudDir.isValid() );
  QfCloudUtils::setLocalCloudDirectory( cloudDir.path() );

  const QString username = QStringLiteral( "alice" );
  const QString projectId = QStringLiteral( "11111111-1111-1111-1111-111111111111" );
  REQUIRE( QDir().mkpath( QStringLiteral( "%1/%2/%3/DCIM" ).arg( cloudDir.path(), username, projectId ) ) );

  const QString photoPath = QStringLiteral( "%1/%2/%3/DCIM/photo.jpg" ).arg( cloudDir.path(), username, projectId );
  QFile photoFile( photoPath );
  REQUIRE( photoFile.open( QIODevice::WriteOnly ) );
  photoFile.write( QByteArray( "photo" ) );
  photoFile.close();

  QfCloudConnection connection;
  connection.setUsername( username );
  QfCloudProject project( projectId, &connection );

  QfCloudAttachmentsModel model;
  model.setCloudConnection( &connection );
  model.setCloudProject( &project );
  REQUIRE( model.rowCount() == 0 );

  auto firstRowStatus = [&model]() {
    return model.data( model.index( 0 ), QfCloudAttachmentsModel::StatusRole ).toInt();
  };

  SECTION( "Attachments queued and uploaded outside of the model's connection are listed" )
  {
    QfCloudUtils::addPendingAttachments( username, projectId, QStringList() << photoPath );
    REQUIRE( QTest::qWaitFor( [&]() { return model.rowCount() == 1 && firstRowStatus() == QfCloudAttachmentsModel::PendingStatus; }, 5000 ) );

    QfCloudAttachmentHistoryEntry entry;
    entry.timestamp = QDateTime::currentDateTimeUtc();
    entry.httpCode = 201;
    entry.fileName = photoPath;
    QfCloudUtils::addAttachmentHistory( username, projectId, entry );
    QfCloudUtils::removePendingAttachment( username, projectId, photoPath );
    REQUIRE( QTest::qWaitFor( [&]() { return model.rowCount() == 1 && firstRowStatus() == QfCloudAttachmentsModel::UploadedStatus; }, 5000 ) );
    REQUIRE( model.data( model.index( 0 ), QfCloudAttachmentsModel::HttpCodeRole ).toInt() == 201 );
  }

  SECTION( "Attachments of a previous user are no longer watched" )
  {
    connection.setUsername( QStringLiteral( "bob" ) );
    QSignalSpy resetSpy( &model, &QAbstractItemModel::modelReset );
    QfCloudUtils::addPendingAttachments( username, projectId, QStringList() << photoPath );
    REQUIRE_FALSE( resetSpy.wait( 1000 ) );
    REQUIRE( model.rowCount() == 0 );
  }

  QfCloudUtils::setLocalCloudDirectory( QString() );
}
