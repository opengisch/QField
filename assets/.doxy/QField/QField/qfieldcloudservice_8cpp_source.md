

# File qfieldcloudservice.cpp

[**File List**](files.md) **>** [**service**](dir_4f691f982165d1317925cee58b4b4f56.md) **>** [**qfieldcloudservice.cpp**](qfieldcloudservice_8cpp.md)

[Go to the documentation of this file](qfieldcloudservice_8cpp.md)


```C++
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
    QString messageString = tr( "Uploading %1" ).arg( fileName );
    if ( uploadPending > 0 )
    {
      messageString += QStringLiteral( " — " ) + tr( "%n file(s) remaining", "", uploadPending );
    }
    const QJniObject message = QJniObject::fromString( messageString );
    const int progress = static_cast<int>( fileProgress * 100 );
    QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldCloudService",
                                        "triggerShowNotification",
                                        "(Ljava/lang/String;I)V",
                                        message.object<jstring>(),
                                        progress );
  } );
  QObject::connect( mCloudConnection.get(), &QFieldCloudConnection::pendingAttachmentsUploadFinished, this, [=]() {
    exit();
  } );

  QTimer::singleShot( 1000, [=] { uploadPendingAttachments(); } );
}

void QFieldCloudService::uploadPendingAttachments()
{
  mCloudConnection->uploadPendingAttachments();
}
```


