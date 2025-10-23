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
#include "qfieldcloudservice.h"

#include <QTimer>

QFieldCloudService::QFieldCloudService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
}

QFieldCloudService::~QFieldCloudService()
{
  QJniObject activity = QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context();
  activity.callMethod<void>( "stopSelf" );
}

void QFieldCloudService::initService()
{
  mCloudConnection.reset( new QFieldCloudConnection() );

  QObject::connect( mCloudConnection.get(), &QFieldCloudConnection::pendingAttachmentsUploadStatus, this, [=]( const QString &fileName, double fileProgress, int uploadPending ) {
    qInfo() << "about to trigger notification message" << fileName;
    const QJniObject message = QJniObject::fromString( tr( "Uploading %1 - %n file(s) remaining", "", uploadPending ).arg( fileName ) );
    const int progress = static_cast<int>( fileProgress * 100 )
      QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldCloudService",
                                          "triggerShowNotification",
                                          "(Ljava/lang/String;I)V",
                                          message.object<jstring>(),
                                          progress );
  } );
  QObject::connect( mCloudConnection.get(), &QFieldCloudConnection::pendingAttachmentsUploadFinished, this, [=]() {
    exit( 1 );
  } );

  QTimer::singleShot( 1000, [=] { uploadPendingAttachments(); } );
}

void QFieldCloudService::uploadPendingAttachments()
{
  mCloudConnection->uploadPendingAttachments();
}
