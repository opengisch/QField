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
#include "androidprojectsource.h"

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

QString AndroidPlatformUtilities::packagePath() const
{
  return getIntentExtra( "PACKAGE_PATH" );
}

QString AndroidPlatformUtilities::qgsProject() const
{
  return getIntentExtra( "QGS_PROJECT" );
}

QString AndroidPlatformUtilities::getIntentExtra( const QString &extra, QAndroidJniObject extras ) const
{
  if ( extras == nullptr )
  {
    extras = getNativeExtras();
  }
  if ( extras.isValid() )
  {
    QAndroidJniObject extraJni = QAndroidJniObject::fromString( extra );
    extraJni = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;", extraJni.object<jstring>() );
    if ( extraJni.isValid() )
    {
      return extraJni.toString();
    }
  }
  return QString();
}

QAndroidJniObject AndroidPlatformUtilities::getNativeIntent() const
{
  QAndroidJniObject activity = QtAndroid::androidActivity();
  if ( activity.isValid() )
  {
    QAndroidJniObject intent = activity.callObjectMethod( "getIntent", "()Landroid/content/Intent;" );
    return intent;
  }
  return nullptr;
}

QAndroidJniObject AndroidPlatformUtilities::getNativeExtras() const
{
  QAndroidJniObject intent = getNativeIntent();
  if ( intent.isValid() )
  {
    QAndroidJniObject extras = intent.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );

    return extras;
  }
  return nullptr;
}

PictureSource *AndroidPlatformUtilities::getPicture( const QString &prefix )
{
  if ( !checkCameraPermissions() )
    return nullptr;

  QAndroidJniObject actionImageCapture = QAndroidJniObject::getStaticObjectField( "android/provider/MediaStore", "ACTION_IMAGE_CAPTURE", "Ljava/lang/String;" );

  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionImageCapture.object<jstring>() );

  AndroidPictureSource *pictureSource = nullptr;

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

void AndroidPlatformUtilities::open( const QString &uri, const QString &mimeType )
{
  checkWriteExternalStoragePermissions();
  QAndroidJniObject actionView = QAndroidJniObject::getStaticObjectField( "android/intent/action", "ACTION_VIEW", "Ljava/lang/String;" );

  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", actionView.object<jstring>() );

  QAndroidJniObject jDataString = QAndroidJniObject::fromString( uri );
  QAndroidJniObject jType = QAndroidJniObject::fromString( mimeType );

  QAndroidJniObject jData = QAndroidJniObject::callStaticObjectMethod( "android/net/Uri", "parse", "(Ljava/lang/String;)Landroid/net/Uri;", jDataString.object<jstring>() );

  intent.callObjectMethod( "setDataAndType", "(Landroid/net/Uri;Ljava/lang/String;)Landroid/content/Intent;", jData.object<jobject>(), jType.object<jstring>() );

  QtAndroid::startActivity( intent.object<jobject>(), 102 );
}

ProjectSource *AndroidPlatformUtilities::openProject()
{
  checkWriteExternalStoragePermissions();

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldProjectActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );

  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );
  QAndroidJniObject className = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldProjectActivity" ) );  

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), className.object<jstring>() );

  AndroidProjectSource *projectSource = nullptr;

  if ( intent.isValid() )
  {
    projectSource = new AndroidProjectSource();
    QtAndroid::startActivity( intent.object<jobject>(), 103, projectSource );
  }
  else
  {
    qDebug() << "Something went wrong creating the project intent";
  }

  return projectSource;
}

bool AndroidPlatformUtilities::checkPositioningPermissions() const
{
  // First check for coarse permissions. If the user configured QField to only get coarse permissions
  // it's his wish and we just let it be.
  QtAndroid::PermissionResult r = QtAndroid::checkPermission( "android.permission.ACCESS_COARSE_LOCATION" );
  if ( r == QtAndroid::PermissionResult::Denied )
  {
    // If there are no permissions available, ask for fine location permissions
    QtAndroid::requestPermissionsSync( QStringList() << "android.permission.ACCESS_FINE_LOCATION" );
    r = QtAndroid::checkPermission( "android.permission.ACCESS_FINE_LOCATION" );
    if ( r == QtAndroid::PermissionResult::Denied )
    {
      return false;
    }
  }
  return true;
}

bool AndroidPlatformUtilities::checkCameraPermissions() const
{
  return checkAndAcquirePermissions( "android.permission.CAMERA" );
}

bool AndroidPlatformUtilities::checkWriteExternalStoragePermissions() const
{
  return checkAndAcquirePermissions( "android.permission.WRITE_EXTERNAL_STORAGE" );
}

bool AndroidPlatformUtilities::checkAndAcquirePermissions( const QString &permissionString ) const
{
  QtAndroid::PermissionResult r = QtAndroid::checkPermission( permissionString );
  if ( r == QtAndroid::PermissionResult::Denied )
  {
    QtAndroid::requestPermissionsSync( QStringList() << permissionString );
    r = QtAndroid::checkPermission( permissionString );
    if ( r == QtAndroid::PermissionResult::Denied )
    {
      return false;
    }
  }
  return true;
}

void AndroidPlatformUtilities::showRateThisApp() const
{

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldAppRaterActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );

  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );
  QAndroidJniObject className = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldAppRaterActivity" ) );  

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), className.object<jstring>() );

    QtAndroid::startActivity( intent.object<jobject>(), 104);
}
