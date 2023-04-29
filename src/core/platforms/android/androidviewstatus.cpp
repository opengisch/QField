/***************************************************************************
 androidviewstatus.cpp

 ---------------------
 begin                : 29.02.2020
 copyright            : (C) 2020 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "androidviewstatus.h"
#include "qgsmessagelog.h"

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QtAndroid>
#else
#include <QtCore/private/qandroidextras_p.h>
#endif

AndroidViewStatus::AndroidViewStatus()
  : ViewStatus( nullptr )
  , QAndroidActivityResultReceiver()
{
}

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
void AndroidViewStatus::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
#else
void AndroidViewStatus::handleActivityResult( int receiverRequestCode, int resultCode, const QJniObject &data )
#endif
{
  Q_UNUSED( resultCode )

  //on open file externally
  if ( receiverRequestCode == 102 )
  {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject extras = data.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );
    QAndroidJniObject errorMessage = QAndroidJniObject::fromString( "ERROR_MESSAGE" );
#else
    QJniObject extras = data.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );
    QJniObject errorMessage = QJniObject::fromString( "ERROR_MESSAGE" );
#endif
    errorMessage = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;",
                                            errorMessage.object<jstring>() );
    if ( !errorMessage.toString().isEmpty() )
    {
      emit statusReceived( errorMessage.toString() );
    }
    emit finished();
  }
}
