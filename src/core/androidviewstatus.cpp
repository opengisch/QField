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

#include <QtAndroid>

AndroidViewStatus::AndroidViewStatus()
  : ViewStatus( nullptr )
  , QAndroidActivityResultReceiver()
{
}

void AndroidViewStatus::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
{
  Q_UNUSED( resultCode )

  //on open file externally
  if ( receiverRequestCode == 102 )
  {
    QAndroidJniObject extras = data.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );
    QAndroidJniObject errorMessage = QAndroidJniObject::fromString( "ERROR_MESSAGE" );
    errorMessage = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;",
                                            errorMessage.object<jstring>() );
    if ( !errorMessage.toString().isEmpty() )
    {
      emit statusReceived( errorMessage.toString() );
    }
  }
}
