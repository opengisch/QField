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
#include "androidpicturesource.h"

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

PictureSource* AndroidPlatformUtilities::getPicture( const QString& prefix )
{
  QAndroidJniObject actionImageCapture = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore", "ACTION_IMAGE_CAPTURE", "Ljava/lang/String;" );

  QAndroidJniObject intent=QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionImageCapture.object<jstring>() );

  AndroidPictureSource* pictureSource = nullptr;

  if ( actionImageCapture.isValid() && intent.isValid() )
  {
    pictureSource = new AndroidPictureSource( prefix );
    QtAndroid::startActivity( intent.object<jobject>(), 101, pictureSource );
  }
  else
  {
    qDebug() << "Something went wrong creating the picture request";
  }

  return pictureSource;
}
