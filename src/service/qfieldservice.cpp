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

#include <QtAndroid>

QFieldService::QFieldService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
  QSettings settings;
  QEventLoop loop( this );
  QFieldCloudConnection connection;
  QObject::connect( &connection, &QFieldCloudConnection::pendingAttachmentsUploadFinished, &loop, &QEventLoop::quit );
  int pendingAttachments = connection.uploadPendingAttachments();
  if ( pendingAttachments > 0 )
  {
    loop.exec();
  }

  QtAndroid::androidService().callMethod<void>( "stopSelf" );

  exit( 0 );
}

QFieldService::~QFieldService()
{
}
