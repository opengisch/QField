/***************************************************************************
                            androidplatformutilities.cpp  -  utilities for qfield

                              -------------------
              begin                : February 2016
              copyright            : (C) 2016 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "androidplatformutilities.h"

#include <QMap>
#include <QString>
#include <QtAndroid>
#include <QDebug>
#include <QAndroidJniEnvironment>

AndroidPlatformUtilities::AndroidPlatformUtilities()
{

}

QString AndroidPlatformUtilities::configDir() const
{
  return getIntentExtra( "DOTQGIS2_DIR" );
}

QString AndroidPlatformUtilities::shareDir() const
{
  return getIntentExtra( "SHARE_DIR" );
}

QString AndroidPlatformUtilities::getIntentExtra( QString extra, QAndroidJniObject extras ) const
{
  if ( extras == 0 )
  {
    extras = getNativeExtras();
  }
  if( extras.isValid() )
  {
    QAndroidJniObject extraJni = QAndroidJniObject::fromString( extra );
    extraJni = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;", extraJni.object<jstring>() );
    if ( extraJni.isValid() )
    {
      extra = extraJni.toString();
      return extra;
    }
  }
  return "";
}


QMap<QString, QString> AndroidPlatformUtilities::getIntentExtras( QStringList intentExtras ) const
{
  QAndroidJniObject extras = getNativeExtras();
  QString extraValue, extraName;
  QMap<QString, QString> extraMap;

  for ( int i = 0; i < intentExtras.size(); ++i )
  {
    extraName = intentExtras.at( i ).toLocal8Bit().constData();
    extraValue = getIntentExtra( extraValue, extras );
    extraMap.insert( extraName, extraValue );
  }
  return extraMap;
}

QAndroidJniObject AndroidPlatformUtilities::getNativeIntent() const
{
  QAndroidJniObject activity = QtAndroid::androidActivity();
  if ( activity.isValid() )
  {
    QAndroidJniObject intent = activity.callObjectMethod( "getIntent", "()Landroid/content/Intent;" );
    if ( intent.isValid() )
    {
      return intent;
    }
  }
  return 0;
}

QAndroidJniObject AndroidPlatformUtilities::getNativeExtras() const
{
  QAndroidJniObject intent = getNativeIntent();
  if ( intent.isValid() )
  {
    QAndroidJniObject extras = intent.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );

    return extras;
  }
  return 0;
}

void AndroidPlatformUtilities::getPicture( const QString& prefix )
{
  QAndroidJniObject actionImageCapture = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore", "ACTION_IMAGE_CAPTURE", "Ljava/lang/String;" );

  QAndroidJniObject intent=QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionImageCapture.object<jstring>() );

  if ( actionImageCapture.isValid() && intent.isValid() )
  {
    QtAndroid::startActivity( intent.object<jobject>(), 101, this );
  }
  else
  {
    qDebug() << "Something went wrong creating the picture request";
  }
}

void AndroidPlatformUtilities::handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data )
{
  jint RESULT_OK = QAndroidJniObject::getStaticField<jint>( "android/app/Activity", "RESULT_OK" );
  if ( receiverRequestCode == 101 && resultCode == RESULT_OK )
  {
    QAndroidJniObject uri = data.callObjectMethod( "getData", "()Landroid/net/Uri;" );
    QAndroidJniObject data = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore$MediaColumns", "DATA", "Ljava/lang/String;" );
    QAndroidJniEnvironment env;
    jobjectArray projection = ( jobjectArray )env->NewObjectArray( 1, env->FindClass( "java/lang/String" ), NULL );
    jobject dataProjection = env->NewStringUTF( data.toString().toStdString().c_str() );
    env->SetObjectArrayElement( projection, 0, dataProjection );
    QAndroidJniObject contentResolver = QtAndroid::androidActivity().callObjectMethod( "getContentResolver", "()Landroid/content/ContentResolver;" );
    QAndroidJniObject cursor = contentResolver.callObjectMethod( "query", "(Landroid/net/Uri;[Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Landroid/database/Cursor;", uri.object<jobject>(), projection, NULL, NULL, NULL );
    jint columnIndex = cursor.callMethod<jint>( "getColumnIndex", "(Ljava/lang/String;)I", data.object<jstring>() );
    cursor.callMethod<jboolean>( "moveToFirst", "()Z" );
    QAndroidJniObject filePath = cursor.callObjectMethod( "getString", "(I)Ljava/lang/String;", columnIndex );
    emit pictureReceived( "file://" + filePath.toString() );
  }
  else
  {
    emit pictureReceived( QString::null );
  }
}
