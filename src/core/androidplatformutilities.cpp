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
#include "androidviewstatus.h"

#include <QMap>
#include <QString>
#include <QtAndroid>
#include <QDebug>
#include <QAndroidJniEnvironment>
#include <QMimeDatabase>

AndroidPlatformUtilities::AndroidPlatformUtilities()
{
  mActivity = QtAndroid::androidActivity();
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

  if ( mActivity.isValid() )
  {
    QAndroidJniObject intent = mActivity.callObjectMethod( "getIntent", "()Landroid/content/Intent;" );
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

PictureSource *AndroidPlatformUtilities::getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  if ( !checkCameraPermissions() )
    return nullptr;

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldCameraPictureActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  QAndroidJniObject pictureFilePath_label = QAndroidJniObject::fromString( "pictureFilePath" );
  QAndroidJniObject pictureFilePath_value = QAndroidJniObject::fromString( pictureFilePath );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           pictureFilePath_label.object<jstring>(),
                           pictureFilePath_value.object<jstring>() );

  QAndroidJniObject prefix_label = QAndroidJniObject::fromString( "prefix" );
  QAndroidJniObject prefix_value = QAndroidJniObject::fromString( prefix );
  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           prefix_label.object<jstring>(),
                           prefix_value.object<jstring>() );

  QAndroidJniObject suffix_label = QAndroidJniObject::fromString( "suffix" );
  QAndroidJniObject suffix_value = QAndroidJniObject::fromString( suffix );
  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           suffix_label.object<jstring>(),
                           suffix_value.object<jstring>() );

  AndroidPictureSource *pictureSource = nullptr;
  pictureSource = new AndroidPictureSource( prefix );

  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );

  return pictureSource;
}

PictureSource *AndroidPlatformUtilities::getGalleryPicture( const QString &prefix, const QString &pictureFilePath )
{
  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldGalleryPictureActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  QAndroidJniObject pictureFilePath_label = QAndroidJniObject::fromString( "pictureFilePath" );
  QAndroidJniObject pictureFilePath_value = QAndroidJniObject::fromString( pictureFilePath );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           pictureFilePath_label.object<jstring>(),
                           pictureFilePath_value.object<jstring>() );

  QAndroidJniObject prefix_label = QAndroidJniObject::fromString( "prefix" );
  QAndroidJniObject prefix_value = QAndroidJniObject::fromString( prefix );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           prefix_label.object<jstring>(),
                           prefix_value.object<jstring>() );

  AndroidPictureSource *pictureSource = nullptr;
  pictureSource = new AndroidPictureSource( prefix );

  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );

  return pictureSource;
}

ViewStatus *AndroidPlatformUtilities::open( const QString &uri )
{
  checkWriteExternalStoragePermissions();

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldOpenExternallyActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  QMimeDatabase db;
  QAndroidJniObject filepath_label = QAndroidJniObject::fromString( "filepath" );
  QAndroidJniObject filepath = QAndroidJniObject::fromString( uri );
  QAndroidJniObject filetype_label = QAndroidJniObject::fromString( "filetype" );
  QAndroidJniObject filetype = QAndroidJniObject::fromString( db.mimeTypeForFile( uri ).name() );

  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filepath_label.object<jstring>(), filepath.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filetype_label.object<jstring>(), filetype.object<jstring>() );

  AndroidViewStatus *viewStatus = nullptr;
  viewStatus = new AndroidViewStatus();
  QtAndroid::startActivity( intent.object<jobject>(), 102, viewStatus );

  return viewStatus;
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
    return checkAndAcquirePermissions( "android.permission.ACCESS_FINE_LOCATION" );
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

void AndroidPlatformUtilities::setScreenLockPermission( const bool allowLock )
{
#if 0
  if ( mActivity.isValid() )
  {
    QAndroidJniObject window = mActivity.callObjectMethod( "getWindow", "()Landroid/view/Window;" );

    if ( window.isValid() )
    {
      const int FLAG_KEEP_SCREEN_ON = 128;
      if ( !allowLock )
      {
        window.callMethod<void>( "addFlags", "(I)V", FLAG_KEEP_SCREEN_ON );
      }
      else
      {
        window.callMethod<void>( "clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON );
      }
    }
  }
#endif
}

void AndroidPlatformUtilities::showRateThisApp() const
{

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldAppRaterActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );

  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield" ) );
  QAndroidJniObject className = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis.qfield.QFieldAppRaterActivity" ) );

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), className.object<jstring>() );

  QtAndroid::startActivity( intent.object<jobject>(), 104 );
}
