/***************************************************************************
  qfieldcloudservice.cpp - QFieldCloudService

 ---------------------
 begin                : 04.12.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfield_android.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudservice.h"

#include <QSettings>

QFieldCloudService::QFieldCloudService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
}

void QFieldCloudService::execute()
{
  QSettings settings;
  QEventLoop loop( this );
  QFieldCloudConnection connection;
  QObject::connect( &connection, &QFieldCloudConnection::pendingAttachmentsUploadStatus, this, [=]( const QString &fileName, double progress ) {
    QJniObject message = QJniObject::fromString( tr( "Uploading %1 - attachment synchronization %1% done)" ).arg( fileName, QStirng::number( progress, 2 ) );
    QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldCloudService",
                                        "triggerShowNotification",
                                        message.object<jstring>() );
  } );
  QObject::connect( &connection, &QFieldCloudConnection::pendingAttachmentsUploadFinished, &loop, &QEventLoop::quit );

  int pendingAttachments = connection.uploadPendingAttachments();
  if ( pendingAttachments > 0 )
  {
    loop.exec();
  }

  QJniObject activity = QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context();
  activity.callMethod<void>( "stopSelf" );
}

QFieldCloudService::~QFieldCloudService()
{
}
