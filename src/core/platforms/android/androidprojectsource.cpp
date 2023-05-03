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

#include "androidprojectsource.h"
#include "qgsmessagelog.h"

#include <QFile>

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QtAndroid>
#else
#include <QtCore/private/qandroidextras_p.h>
#endif

AndroidProjectSource::AndroidProjectSource( QObject *parent )
  : ProjectSource( parent )
  , QAndroidActivityResultReceiver()
{
}

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
void AndroidProjectSource::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
#else
void AndroidProjectSource::handleActivityResult( int receiverRequestCode, int resultCode, const QJniObject &data )
#endif
{
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  jint RESULT_OK = QAndroidJniObject::getStaticField<jint>( "android/app/Activity", "RESULT_OK" );
#else
  jint RESULT_OK = QJniObject::getStaticField<jint>( "android/app/Activity", "RESULT_OK" );
#endif
  if ( receiverRequestCode == 103 && resultCode == RESULT_OK )
  {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject uri = data.callObjectMethod( "getData", "()Landroid/net/Uri;" );
#else
    QJniObject uri = data.callObjectMethod( "getData", "()Landroid/net/Uri;" );
#endif

    QString path = uri.callObjectMethod( "getPath", "()Ljava/lang/String;" ).toString();

    if ( !QFile( path ).exists() )
    {
      QgsMessageLog::logMessage( tr( "File %1 does not exist" ).arg( path ), QStringLiteral( "QField" ), Qgis::Warning );
    }

    emit projectOpened( path );
  }
}
