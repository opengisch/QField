/***************************************************************************
  androidprojectsource.cpp - AndroidProjectSource

 ---------------------
 begin                : 19.3.2018
 copyright            : (C) 2018 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgsmessagelog.h"
#include "androidprojectsource.h"

AndroidProjectSource::AndroidProjectSource()
  : ProjectSource( nullptr )
  , QAndroidActivityResultReceiver()
{
}

void AndroidProjectSource::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject& data )
{
  jint RESULT_OK = QAndroidJniObject::getStaticField<jint>( "android/app/Activity", "RESULT_OK" );
  if ( receiverRequestCode == 103 && resultCode == RESULT_OK )
  {
    QAndroidJniObject uri = data.callObjectMethod( "getData", "()Landroid/net/Uri;" );
    QAndroidJniObject path = uri.callObjectMethod( "getPath", "()Ljava/lang/String;" );
    QAndroidJniObject file = QAndroidJniObject( "java/io/File", "(Ljava/lang/String;)V", path.object<jstring>() );
    QString absolutePath = file.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" ).toString();

    QgsMessageLog::logMessage( QStringLiteral( "Uri - absolute %1" ).arg( absolutePath ) );
    if ( absolutePath.startsWith( QStringLiteral( "/document/primary:" ) ) )
    {
      QAndroidJniObject extStorage = QAndroidJniObject::callStaticObjectMethod( "android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;" );
      QString extStoragePath = extStorage.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" ).toString();

      extStoragePath += '/';

      absolutePath.replace( QStringLiteral( "/document/primary:" ), extStoragePath );
    }

    emit projectOpened( absolutePath );
  }
}
