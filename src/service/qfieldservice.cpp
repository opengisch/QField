/***************************************************************************
  qfieldservice.cpp - QFieldService

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
#include "qfieldservice.h"

#include <QSettings>

QFieldService::QFieldService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
  qInfo() << "Service starting";
  QSettings settings;
  QEventLoop loop( this );
  QFieldCloudConnection connection;
  QObject::connect( &connection, &QFieldCloudConnection::pendingAttachmentsUploadFinished, &loop, &QEventLoop::quit );
  int pendingAttachments = connection.uploadPendingAttachments();
  qInfo() << "Service pending attachments: " << pendingAttachments;
  if ( pendingAttachments > 0 )
  {
    loop.exec();
  }
  qInfo() << "Service ending";

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject activity = QtAndroid::androidService();
#else
  QJniObject activity = QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context();
#endif
  activity.callMethod<void>( "stopSelf" );

  exit( 0 );
}

QFieldService::~QFieldService()
{
}
