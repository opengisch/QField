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

#include "androidpicturesource.h"
#include "androidplatformutilities.h"
#include "androidprojectsource.h"
#include "androidviewstatus.h"
#include "appinterface.h"
#include "feedback.h"
#include "fileutils.h"
#include "qfield.h"
#include "qfield_android.h"

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMap>
#include <QMimeDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QString>
#include <QtAndroid>

#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>

const char *const applicationName = "QField";

#define GLUE_HELPER( u, v, w, x, y, z ) u##v##w##x##y##z
#define JNI_FUNCTION_NAME( package_name, class_name, function_name ) GLUE_HELPER( Java_ch_opengis_, package_name, _, class_name, _, function_name )

AndroidPlatformUtilities::AndroidPlatformUtilities()
  : mActivity( QtAndroid::androidActivity() )
{
}

PlatformUtilities::Capabilities AndroidPlatformUtilities::capabilities() const
{
  PlatformUtilities::Capabilities capabilities = Capabilities() | NativeCamera | AdjustBrightness;
#ifdef WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void AndroidPlatformUtilities::initSystem()
{
  // Copy data away from the virtual path `assets:/` to a path accessible also for non-qt-based libs
  const QString appDataLocation = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
  mSystemGenericDataLocation = appDataLocation + QStringLiteral( "/share" );

  QFile gitRevFile( appDataLocation + QStringLiteral( "/gitRev" ) );
  gitRevFile.open( QIODevice::ReadOnly );

  QByteArray localGitRev = gitRevFile.readAll();
  gitRevFile.close();
  QByteArray appGitRev = qfield::gitRev.toUtf8();
  if ( localGitRev != appGitRev )

  {
    if ( mActivity.isValid() )
    {
      QtAndroid::runOnAndroidThread( [] {
        QAndroidJniObject activity = QtAndroid::androidActivity();
        if ( activity.isValid() )
        {
          QAndroidJniObject messageJni = QAndroidJniObject::fromString( QObject::tr( "Please wait while QField installation finalizes." ) );
          activity.callMethod<void>( "showBlockingProgressDialog", "(Ljava/lang/String;)V", messageJni.object<jstring>() );
        }
      } );
    }

    qInfo() << QStringLiteral( "Different build git revision detected (previous: %1, current: %2)" )
                 .arg( localGitRev.size() > 0 ? localGitRev.mid( 0, 7 ) : QStringLiteral( "n/a" ) )
                 .arg( appGitRev.size() > 0 ? appGitRev.mid( 0, 7 ) : QStringLiteral( "n/a" ) );

    const bool success = FileUtils::copyRecursively( QStringLiteral( "assets:/share" ), mSystemGenericDataLocation );

    if ( mActivity.isValid() )
    {
      QtAndroid::runOnAndroidThread( [] {
        QAndroidJniObject activity = QtAndroid::androidActivity();
        if ( activity.isValid() )
        {
          activity.callMethod<void>( "dismissBlockingProgressDialog" );
        }
      } );
    }

    if ( success )
    {
      qDebug() << "Successfully copied share assets content";
      gitRevFile.open( QIODevice::WriteOnly | QIODevice::Truncate );
      gitRevFile.write( appGitRev );
      gitRevFile.close();
    }
  }
}

QString AndroidPlatformUtilities::systemGenericDataLocation() const
{
  return mSystemGenericDataLocation;
}

QString AndroidPlatformUtilities::qgsProject() const
{
  return getIntentExtra( "QGS_PROJECT" );
}

QString AndroidPlatformUtilities::qfieldDataDir() const
{
  return getIntentExtra( "QFIELD_DATA_DIR" );
}

QString AndroidPlatformUtilities::qfieldAppDataDir() const
{
  return getIntentExtra( "QFIELD_APP_DATA_DIR" );
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

PictureSource *AndroidPlatformUtilities::getCameraPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  Q_UNUSED( parent )
  if ( !checkCameraPermissions() )
    return nullptr;

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldCameraPictureActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );

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

  AndroidPictureSource *pictureSource = new AndroidPictureSource( prefix );

  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );

  return pictureSource;
}

PictureSource *AndroidPlatformUtilities::getGalleryPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath )
{
  Q_UNUSED( parent )

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldGalleryPictureActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );

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

  AndroidPictureSource *pictureSource = new AndroidPictureSource( prefix );

  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );

  return pictureSource;
}

ViewStatus *AndroidPlatformUtilities::open( const QString &uri, bool editing )
{
  checkWriteExternalStoragePermissions();

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldOpenExternallyActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  QMimeDatabase db;
  QAndroidJniObject filepath_label = QAndroidJniObject::fromString( "filepath" );
  QAndroidJniObject filepath = QAndroidJniObject::fromString( uri );
  QAndroidJniObject filetype_label = QAndroidJniObject::fromString( "filetype" );
  QAndroidJniObject filetype = QAndroidJniObject::fromString( db.mimeTypeForFile( uri ).name() );
  QAndroidJniObject fileediting_label = QAndroidJniObject::fromString( "fileediting" );
  QAndroidJniObject fileediting = QAndroidJniObject::fromString( editing ? "true" : "false" );

  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filepath_label.object<jstring>(), filepath.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filetype_label.object<jstring>(), filetype.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", fileediting_label.object<jstring>(), fileediting.object<jstring>() );

  AndroidViewStatus *viewStatus = new AndroidViewStatus();
  QtAndroid::startActivity( intent.object<jobject>(), 102, viewStatus );

  return viewStatus;
}

ProjectSource *AndroidPlatformUtilities::openProject()
{
  checkWriteExternalStoragePermissions();

  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldProjectActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );

  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QAndroidJniObject className = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldProjectActivity" ) );

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
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [allowLock] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QAndroidJniObject window = activity.callObjectMethod( "getWindow", "()Landroid/view/Window;" );

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

      QAndroidJniEnvironment env;
      if ( env->ExceptionCheck() )
      {
        env->ExceptionClear();
      }
    } );
  }
}

void AndroidPlatformUtilities::dimBrightness()
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "dimBrightness" );
      }
    } );
  }
}

void AndroidPlatformUtilities::restoreBrightness()
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "restoreBrightness" );
      }
    } );
  }
}

void AndroidPlatformUtilities::initiateSentry()
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "initiateSentry" );
      }
    } );
  }
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, openProject )( JNIEnv *env, jobject obj, jstring path )
{
  if ( AppInterface::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    AppInterface::instance()->loadFile( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

JNIEXPORT jobject JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldProjectListAdapter, createImageBitmap )( JNIEnv *env, jobject obj, jstring path )
{
  QImage image( QString( env->GetStringUTFChars( path, NULL ) ) );
  image = image.scaledToWidth( 255 );
  if ( image.format() != QImage::Format_RGBA8888 )
  {
    image = image.convertToFormat( QImage::Format_RGBA8888 );
  }

  qDebug() << "1111";
  QAndroidJniObject config = QAndroidJniObject::getStaticObjectField( "android/graphics/Bitmap$Config",
                                                                      "ARGB_8888",
                                                                      "Landroid/graphics/Bitmap$Config;" );
  QAndroidJniObject bitmap = QAndroidJniObject::callStaticObjectMethod( "android/graphics/Bitmap",
                                                                        "createBitmap",
                                                                        "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;",
                                                                        image.width(), image.height(), config.object() );
  AndroidBitmapInfo info;
  if ( AndroidBitmap_getInfo( env, bitmap.object(), &info ) != ANDROID_BITMAP_RESULT_SUCCESS )
    return QAndroidJniObject().object();

  if ( info.format != ANDROID_BITMAP_FORMAT_RGBA_8888 )
    return QAndroidJniObject().object();

  void *pixels;
  if ( AndroidBitmap_lockPixels( env, bitmap.object(), &pixels ) != ANDROID_BITMAP_RESULT_SUCCESS )
    return QAndroidJniObject().object();

  if ( info.stride == uint32_t( image.bytesPerLine() ) )
  {
    memcpy( pixels, image.constBits(), info.stride * info.height );
  }
  else
  {
    uchar *bmpPtr = static_cast<uchar *>( pixels );
    const unsigned width = std::min( info.width, ( uint ) image.width() );
    const unsigned height = std::min( info.height, ( uint ) image.height() );
    for ( unsigned y = 0; y < height; y++, bmpPtr += info.stride )
    {
      memcpy( bmpPtr, image.constScanLine( y ), width );
    }
  }

  if ( AndroidBitmap_unlockPixels( env, bitmap.object() ) != ANDROID_BITMAP_RESULT_SUCCESS )
    return QAndroidJniObject().object();

  qDebug() << "22222";
  return env->NewLocalRef( bitmap.object() );
}

#ifdef __cplusplus
}
#endif

#include "androidplatformutilities.moc"
