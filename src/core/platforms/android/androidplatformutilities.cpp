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
#include "fileutils.h"
#include "qfield.h"
#include "qfield_android.h"
#include "qfieldcloudconnection.h"

#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QMimeDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QStandardPaths>
#include <QString>
#include <QTimer>
#include <QtAndroid>
#include <qgsfileutils.h>

#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>

const char *const applicationName = "QField";

#define GLUE_HELPER( u, v, w, x, y, z ) u##v##w##x##y##z
#define JNI_FUNCTION_NAME( package_name, class_name, function_name ) GLUE_HELPER( Java_ch_opengis_, package_name, _, class_name, _, function_name )

AndroidPlatformUtilities::AndroidPlatformUtilities()
  : mActivity( QtAndroid::androidActivity() )
  , mSystemGenericDataLocation( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + QStringLiteral( "/share" ) )
{
}

PlatformUtilities::Capabilities AndroidPlatformUtilities::capabilities() const
{
  PlatformUtilities::Capabilities capabilities = Capabilities() | NativeCamera | AdjustBrightness | CustomLocalDataPicker | CustomImport | CustomExport | CustomSend;
#ifdef WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void AndroidPlatformUtilities::afterUpdate()
{
  // Copy data away from the virtual path `assets:/` to a path accessible also for non-qt-based libs

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

  FileUtils::copyRecursively( QStringLiteral( "assets:/share" ), mSystemGenericDataLocation );

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
}

QString AndroidPlatformUtilities::systemSharedDataLocation() const
{
  return mSystemGenericDataLocation;
}

QString AndroidPlatformUtilities::qgsProject() const
{
  return getIntentExtra( "QGS_PROJECT" );
}

QStringList AndroidPlatformUtilities::appDataDirs() const
{
  const QString dataDirs = getIntentExtra( "QFIELD_APP_DATA_DIRS" );
  return ( !dataDirs.isEmpty() ? dataDirs.split( "--;--" ) : QStringList() );
}

QString AndroidPlatformUtilities::applicationDirectory() const
{
  if ( mActivity.isValid() )
  {
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getApplicationDirectory" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString();
    }
  }

  return QString();
}

QStringList AndroidPlatformUtilities::additionalApplicationDirectories() const
{
  if ( mActivity.isValid() )
  {
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getAdditionalApplicationDirectories" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString().split( "--;--" );
    }
  }

  return QStringList();
}

QStringList AndroidPlatformUtilities::rootDirectories() const
{
  if ( mActivity.isValid() )
  {
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getRootDirectories" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString().split( "--;--" );
    }
  }

  return QStringList();
}

void AndroidPlatformUtilities::importProjectFolder() const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportProjectFolder" );
      }
    } );
  }
}

void AndroidPlatformUtilities::importProjectArchive() const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportProjectArchive" );
      }
    } );
  }
}

void AndroidPlatformUtilities::importDatasets() const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportDatasets" );
      }
    } );
  }
}

void AndroidPlatformUtilities::sendDatasetTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QStringList paths = QStringList() << path;
        const QSet<QString> files = QgsFileUtils::sidecarFilesForPath( path );
        for ( const QString &file : files )
        {
          paths << file;
        }
        QAndroidJniObject pathsJni = QAndroidJniObject::fromString( paths.join( "--;--" ) );
        activity.callMethod<void>( "sendDatasetTo", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::exportDatasetTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QStringList paths = QStringList() << path;
        const QSet<QString> files = QgsFileUtils::sidecarFilesForPath( path );
        for ( const QString &file : files )
        {
          paths << file;
        }
        QAndroidJniObject pathsJni = QAndroidJniObject::fromString( paths.join( "--;--" ) );
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::removeDataset( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
        activity.callMethod<void>( "removeDataset", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::exportFolderTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::sendCompressedFolderTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
        activity.callMethod<void>( "sendCompressedFolderTo", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::removeFolder( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    QtAndroid::runOnAndroidThread( [path] {
      QAndroidJniObject activity = QtAndroid::androidActivity();
      if ( activity.isValid() )
      {
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
        activity.callMethod<void>( "removeProjectFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
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
  if ( QFileInfo( uri ).isDir() )
    return nullptr;

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

QVariantMap AndroidPlatformUtilities::sceneMargins( QQuickWindow *window ) const
{
  Q_UNUSED( window )

  const QAndroidJniObject activity = QtAndroid::androidActivity();
  double statusBarMargin = std::abs( static_cast<double>( activity.callMethod<jdouble>( "statusBarMargin" ) ) );

  statusBarMargin /= QGuiApplication::primaryScreen()->devicePixelRatio();

  QVariantMap margins;
  margins[QLatin1String( "top" )] = statusBarMargin;
  margins[QLatin1String( "right" )] = 0.0;
  margins[QLatin1String( "bottom" )] = 0.0;
  margins[QLatin1String( "left" )] = 0.0;
  return margins;
}

void AndroidPlatformUtilities::uploadPendingAttachments( QFieldCloudConnection *connection ) const
{
  QTimer::singleShot( 500, [connection]() {
    if ( connection )
    {
      qInfo() << "Launching service from main...";
      QAndroidJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldService",
                                                 "startQFieldService",
                                                 "(Landroid/content/Context;)V",
                                                 QtAndroid::androidActivity().object() );
    }
  } );
}

#ifdef __cplusplus
extern "C" {
#endif

// QFieldActivity class functions
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

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, openPath )( JNIEnv *env, jobject obj, jstring path )
{
  if ( AppInterface::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    emit AppInterface::instance()->openPath( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

#ifdef __cplusplus
}
#endif
