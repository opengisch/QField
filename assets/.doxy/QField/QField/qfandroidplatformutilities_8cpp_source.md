

# File qfandroidplatformutilities.cpp

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**qfandroidplatformutilities.cpp**](qfandroidplatformutilities_8cpp.md)

[Go to the documentation of this file](qfandroidplatformutilities_8cpp.md)


```C++
/***************************************************************************
                            qfandroidplatformutilities.cpp  -  utilities for qfield

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

#include "qfandroidplatformutilities.h"
#include "qfandroidprojectsource.h"
#include "qfandroidresourcesource.h"
#include "qfandroidviewstatus.h"
#include "qfappinterface.h"
#include "qfcloudconnection.h"
#include "qffileutils.h"
#include "qfield.h"
#include "qfield_android.h"

#include <QJniEnvironment>
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>

inline QJniObject qtAndroidContext()
{
  return QJniObject( QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context() );
}

inline int qtAndroidSkdVersion()
{
  return QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->sdkVersion();
}

inline void runOnAndroidMainThread( const std::function<void()> &runnable )
{
  QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->runOnAndroidMainThread( [runnable]() {
    runnable();
    return QVariant();
  } );
}

#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QMimeDatabase>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QScreen>
#include <QSettings>
#include <QStandardPaths>
#include <QString>
#include <QTimer>
#include <qgsfileutils.h>

#include <android/bitmap.h>
#include <android/log.h>
#include <jni.h>

const char *const applicationName = "QField";

#define GLUE_HELPER( u, v, w, x, y, z ) u##v##w##x##y##z
#define JNI_FUNCTION_NAME( package_name, class_name, function_name ) GLUE_HELPER( Java_ch_opengis_, package_name, _, class_name, _, function_name )

QfAndroidPlatformUtilities::QfAndroidPlatformUtilities()
  : mActivity( qtAndroidContext() )
  , mSystemGenericDataLocation( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + QStringLiteral( "/share" ) )
{
}

QfPlatformUtilities::Capabilities QfAndroidPlatformUtilities::capabilities() const
{
  QfPlatformUtilities::Capabilities capabilities = Capabilities() | NativeCamera | AdjustBrightness | CustomImport | CustomExport | CustomSend | FilePicker | VolumeKeys | UpdateProjectFromArchive | PositioningService;
#if WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void QfAndroidPlatformUtilities::afterUpdate()
{
  // Copy data away from the virtual path `assets:/` to a path accessible also for non-qt-based libs

  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject messageJni = QJniObject::fromString( QObject::tr( "Please wait while QField installation finalizes." ) );
        activity.callMethod<void>( "showBlockingProgressDialog", "(Ljava/lang/String;)V", messageJni.object<jstring>() );
      }
    } );
  }

  QfFileUtils::copyRecursively( QStringLiteral( "assets:/" ), mSystemGenericDataLocation );

  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "dismissBlockingProgressDialog" );
      }
    } );
  }
}

QString QfAndroidPlatformUtilities::systemSharedDataLocation() const
{
  return mSystemGenericDataLocation;
}

bool QfAndroidPlatformUtilities::hasQgsProject() const
{
  return !getIntentExtra( "QGS_PROJECT" ).isEmpty();
}

void QfAndroidPlatformUtilities::loadQgsProject() const
{
  if ( mActivity.isValid() && hasQgsProject() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "processProjectIntent" );
      }
    } );
  }
}

bool QfAndroidPlatformUtilities::hasQfAction() const
{
  return !getIntentExtra( "QF_ACTION" ).isEmpty();
}

void QfAndroidPlatformUtilities::executeQfAction() const
{
  if ( mActivity.isValid() && hasQfAction() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "processQFieldIntent" );
      }
    } );
  }
}

QStringList QfAndroidPlatformUtilities::appDataDirs() const
{
  const QString dataDirs = getIntentExtra( "QFIELD_APP_DATA_DIRS" );
  return ( !dataDirs.isEmpty() ? dataDirs.split( "--;--" ) : QStringList() );
}

QString QfAndroidPlatformUtilities::applicationDirectory() const
{
  if ( mActivity.isValid() )
  {
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getApplicationDirectory" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString();
    }
  }

  return QString();
}

QStringList QfAndroidPlatformUtilities::additionalApplicationDirectories() const
{
  if ( mActivity.isValid() )
  {
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getAdditionalApplicationDirectories" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString().split( "--;--" );
    }
  }

  return QStringList();
}

QStringList QfAndroidPlatformUtilities::rootDirectories() const
{
  if ( mActivity.isValid() )
  {
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getRootDirectories" );
    if ( rootDirs.isValid() )
    {
      return rootDirs.toString().split( "--;--" );
    }
  }

  return QStringList();
}

void QfAndroidPlatformUtilities::importProjectFolder() const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportProjectFolder" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::importProjectArchive() const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportProjectArchive" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::importDatasets() const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "triggerImportDatasets" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::updateProjectFromArchive( const QString &projectPath ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [projectPath] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject projectPathJni = QJniObject::fromString( projectPath );
        activity.callMethod<void>( "triggerUpdateProjectFromArchive", "(Ljava/lang/String;)V", projectPathJni.object<jstring>() );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::sendDatasetTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QStringList paths = QStringList() << path;
        const QSet<QString> files = QgsFileUtils::sidecarFilesForPath( path );
        for ( const QString &file : files )
        {
          paths << file;
        }
        QJniObject pathsJni = QJniObject::fromString( paths.join( "--;--" ) );
        activity.callMethod<void>( "sendDatasetTo", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::exportDatasetTo( const QString &path ) const
{
  if ( path.trimmed().isEmpty() )
  {
    return;
  }

  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QStringList paths = QStringList() << path;
        const QSet<QString> files = QgsFileUtils::sidecarFilesForPath( path );
        for ( const QString &file : files )
        {
          paths << file;
        }
        QJniObject pathsJni = QJniObject::fromString( paths.join( "--;--" ) );
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::removeDataset( const QString &path ) const
{
  bool allowed = false;
  const QStringList allowedDirectories = QStringList() << applicationDirectory() << additionalApplicationDirectories();
  for ( const QString &directory : allowedDirectories )
  {
    if ( path.startsWith( directory ) )
    {
      allowed = true;
      break;
    }
  }
  if ( allowed )
  {
    if ( mActivity.isValid() )
    {
      runOnAndroidMainThread( [path] {
        auto activity = qtAndroidContext();
        if ( activity.isValid() )
        {
          QJniObject pathJni = QJniObject::fromString( path );
          activity.callMethod<void>( "removeDataset", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
        }
      } );
    }
  }
}

void QfAndroidPlatformUtilities::exportFolderTo( const QString &path ) const
{
  if ( path.trimmed().isEmpty() )
  {
    return;
  }

  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject pathJni = QJniObject::fromString( path );
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::sendCompressedFolderTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject pathJni = QJniObject::fromString( path );
        activity.callMethod<void>( "sendCompressedFolderTo", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::removeFolder( const QString &path ) const
{
  bool allowed = false;
  const QStringList allowedDirectories = QStringList() << applicationDirectory() << additionalApplicationDirectories();
  for ( const QString &directory : allowedDirectories )
  {
    if ( path.startsWith( directory ) )
    {
      allowed = true;
      break;
    }
  }
  if ( allowed )
  {
    if ( mActivity.isValid() )
    {
      runOnAndroidMainThread( [path] {
        auto activity = qtAndroidContext();
        if ( activity.isValid() )
        {
          QJniObject pathJni = QJniObject::fromString( path );
          activity.callMethod<void>( "removeProjectFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
        }
      } );
    }
  }
}

QString QfAndroidPlatformUtilities::getIntentExtra( const QString &extra, QJniObject extras ) const
{
  if ( extras == nullptr )
  {
    extras = getNativeExtras();
  }
  if ( extras.isValid() )
  {
    QJniObject extraJni = QJniObject::fromString( extra );
    extraJni = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;", extraJni.object<jstring>() );
    if ( extraJni.isValid() )
    {
      return extraJni.toString();
    }
  }
  return QString();
}

QJniObject QfAndroidPlatformUtilities::getNativeIntent() const
{
  if ( mActivity.isValid() )
  {
    QJniObject intent = mActivity.callObjectMethod( "getIntent", "()Landroid/content/Intent;" );
    return intent;
  }
  return nullptr;
}

QJniObject QfAndroidPlatformUtilities::getNativeExtras() const
{
  QJniObject intent = getNativeIntent();
  if ( intent.isValid() )
  {
    QJniObject extras = intent.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );

    return extras;
  }
  return nullptr;
}

QfResourceSource *QfAndroidPlatformUtilities::processCameraActivity( const QString &prefix, const QString &filePath, const QString &suffix, bool isVideo, QObject *parent )
{
  if ( !checkCameraPermissions() )
    return nullptr;

  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

  QfAndroidResourceSource *resourceSource = nullptr;
  if ( mActivity.isValid() )
  {
    resourceSource = new QfAndroidResourceSource( prefix, parent );

    runOnAndroidMainThread( [prefix, filePath, suffix, isVideo] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject prefixJni = QJniObject::fromString( prefix );
        QJniObject filePathJni = QJniObject::fromString( filePath );
        QJniObject suffixJni = QJniObject::fromString( suffix );

        QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), true );
        activity.callMethod<void>( "getCameraResource", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)V",
                                   prefixJni.object<jstring>(),
                                   filePathJni.object<jstring>(),
                                   suffixJni.object<jstring>(),
                                   isVideo );
      }
    } );
  }
  return resourceSource;
}

QfResourceSource *QfAndroidPlatformUtilities::getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix, QObject *parent )
{
  return processCameraActivity( prefix, pictureFilePath, suffix, false, parent );
}

QfResourceSource *QfAndroidPlatformUtilities::getCameraVideo( const QString &prefix, const QString &videoFilePath, const QString &suffix, QObject *parent )
{
  return processCameraActivity( prefix, videoFilePath, suffix, true, parent );
}

QfResourceSource *QfAndroidPlatformUtilities::processGalleryActivity( const QString &prefix, const QString &filePath, const QString &mimeType, QObject *parent )
{
  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

  QfAndroidResourceSource *resourceSource = nullptr;
  if ( mActivity.isValid() )
  {
    resourceSource = new QfAndroidResourceSource( prefix, parent );

    runOnAndroidMainThread( [prefix, filePath, mimeType] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject prefixJni = QJniObject::fromString( prefix );
        QJniObject filePathJni = QJniObject::fromString( filePath );
        QJniObject mimeTypeJni = QJniObject::fromString( mimeType );

        QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), true );
        activity.callMethod<void>( "getGalleryResource", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                                   prefixJni.object<jstring>(),
                                   filePathJni.object<jstring>(),
                                   mimeTypeJni.object<jstring>() );
      }
    } );
  }
  return resourceSource;
}

QfResourceSource *QfAndroidPlatformUtilities::getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent )
{
  return processGalleryActivity( prefix, pictureFilePath, QStringLiteral( "image/*" ), parent );
}

QfResourceSource *QfAndroidPlatformUtilities::getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent )
{
  return processGalleryActivity( prefix, videoFilePath, QStringLiteral( "video/*" ), parent );
}

QfResourceSource *QfAndroidPlatformUtilities::getFile( const QString &prefix, const QString &filePath, const QString &mimeType, QObject *parent )
{
  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

  QfAndroidResourceSource *resourceSource = nullptr;
  if ( mActivity.isValid() )
  {
    resourceSource = new QfAndroidResourceSource( prefix, parent );

    runOnAndroidMainThread( [prefix, filePath, mimeType] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject prefixJni = QJniObject::fromString( prefix );
        QJniObject filePathJni = QJniObject::fromString( filePath );
        QJniObject mimeTypeJni = QJniObject::fromString( mimeType );

        activity.callMethod<void>( "getFilePickerResource", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V",
                                   prefixJni.object<jstring>(),
                                   filePathJni.object<jstring>(),
                                   mimeTypeJni.object<jstring>() );
      }
    } );
  }
  return resourceSource;
}

QfViewStatus *QfAndroidPlatformUtilities::open( const QString &filePath, bool isEditing, QObject *parent )
{
  if ( QFileInfo( filePath ).isDir() )
    return nullptr;

  QMimeDatabase db;
  const QString mimeType = db.mimeTypeForFile( filePath ).name();

  QfAndroidViewStatus *viewStatus = nullptr;
  if ( mActivity.isValid() )
  {
    if ( parent )
    {
      viewStatus = new QfAndroidViewStatus( parent );
    }

    runOnAndroidMainThread( [filePath, mimeType, isEditing] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject filePathJni = QJniObject::fromString( filePath );
        QJniObject mimeTypeJni = QJniObject::fromString( mimeType );

        QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), true );
        activity.callMethod<void>( "openResource", "(Ljava/lang/String;Ljava/lang/String;Z)V",
                                   filePathJni.object<jstring>(),
                                   mimeTypeJni.object<jstring>(),
                                   isEditing );
      }
    } );
  }
  return viewStatus;
}

void QfAndroidPlatformUtilities::requestStoragePermission() const
{
  if ( !QSettings().value( QStringLiteral( "QField/storagePermissionChecked" ), false ).toBool() )
  {
    QStringList permissions;
    permissions << "android.permission.READ_EXTERNAL_STORAGE"
                << "android.permission.WRITE_EXTERNAL_STORAGE"
                << "android.permission.ACCESS_MEDIA_LOCATION";

    checkAndAcquirePermissions( permissions, true );
    QSettings().setValue( QStringLiteral( "QField/storagePermissionChecked" ), true );
  }
}

bool QfAndroidPlatformUtilities::checkPositioningPermissions() const
{
  // First check for coarse permissions. If the user configured QField to only get coarse permissions
  // it's his wish and we just let it be.
  auto r = QtAndroidPrivate::checkPermission( "android.permission.ACCESS_COARSE_LOCATION" ).result();
  if ( r == QtAndroidPrivate::Denied )
  {
    return checkAndAcquirePermissions( { QStringLiteral( "android.permission.ACCESS_FINE_LOCATION" ) } );
  }
  return true;
}

bool QfAndroidPlatformUtilities::checkCameraPermissions() const
{
  return checkAndAcquirePermissions( { QStringLiteral( "android.permission.CAMERA" ) } );
}

bool QfAndroidPlatformUtilities::checkMicrophonePermissions() const
{
  return checkAndAcquirePermissions( { QStringLiteral( "android.permission.RECORD_AUDIO" ) } );
}

bool QfAndroidPlatformUtilities::checkAndAcquirePermissions( QStringList permissions, bool forceAsk ) const
{
  if ( !forceAsk )
  {
    permissions.erase( std::remove_if( permissions.begin(), permissions.end(),
                                       []( const QString &permission ) {
                                         auto r = QtAndroidPrivate::checkPermission( permission ).result();
                                         return r != QtAndroidPrivate::Denied;
                                       } ),
                       permissions.end() );
  }

  if ( !permissions.isEmpty() )
  {
    for ( const QString &permission : permissions )
    {
      auto results = QtAndroidPrivate::requestPermission( permission ).results();
      if ( results.isEmpty() || results.at( 0 ) == QtAndroidPrivate::Denied )
      {
        if ( !forceAsk )
        {
          return false;
        }
      }
    }
  }

  return true;
}

void QfAndroidPlatformUtilities::setScreenLockPermission( const bool allowLock )
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [allowLock] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        QJniObject window = activity.callObjectMethod( "getWindow", "()Landroid/view/Window;" );

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

      QJniEnvironment env;
      if ( env->ExceptionCheck() )
      {
        env->ExceptionClear();
      }
    } );
  }
}

void QfAndroidPlatformUtilities::dimBrightness()
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "dimBrightness" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::restoreBrightness()
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "restoreBrightness" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::setHandleVolumeKeys( const bool handle )
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [handle] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( handle ? "takeVolumeKeys" : "releaseVolumeKeys" );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::uploadPendingAttachments( QfCloudConnection *connection ) const
{
  // Request notification permission
  checkAndAcquirePermissions( { QStringLiteral( "android.permission.POST_NOTIFICATIONS" ) } );

  QTimer::singleShot( 500, [connection]() {
    if ( connection )
    {
      qInfo() << "Launching QFieldCloud service...";
      QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldCloudService",
                                          "startQFieldCloudService",
                                          "(Landroid/content/Context;)V",
                                          qtAndroidContext().object() );
    }
  } );
}

bool QfAndroidPlatformUtilities::isSystemDarkTheme() const
{
  if ( mActivity.isValid() )
  {
    bool isDarkTheme = mActivity.callMethod<jboolean>( "isDarkTheme" ) == JNI_TRUE;
    return isDarkTheme;
  }
  return false;
}

void QfAndroidPlatformUtilities::vibrate( int milliseconds ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [milliseconds] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
        activity.callMethod<void>( "vibrate", "(I)V", milliseconds );
      }
    } );
  }
}

void QfAndroidPlatformUtilities::requestBackgroundPositioningPermissions()
{
  checkAndAcquirePermissions( { QStringLiteral( "android.permission.ACCESS_BACKGROUND_LOCATION" ) } );
}

QString QfAndroidPlatformUtilities::startPositioningService() const
{
  if ( qtAndroidSkdVersion() >= 33 )
  {
    // Request notification permission
    checkAndAcquirePermissions( { QStringLiteral( "android.permission.POST_NOTIFICATIONS" ) } );
  }

  qInfo() << "Launching QField positioning service...";
  QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                      "startQFieldPositioningService",
                                      "(Landroid/content/Context;)V",
                                      qtAndroidContext().object() );
  return QStringLiteral( "localabstract:" APP_PACKAGE_NAME "replica" );
}

void QfAndroidPlatformUtilities::stopPositioningService() const
{
  qInfo() << "Terminating QField positioning service...";
  QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldPositioningService",
                                      "stopQFieldPositioningService",
                                      "(Landroid/content/Context;)V",
                                      qtAndroidContext().object() );
}

#ifdef __cplusplus
extern "C" {
#endif

// QFieldActivity class functions
JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, openProject )( JNIEnv *env, jobject obj, jstring path )
{
  if ( QfAppInterface::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    QfAppInterface::instance()->loadFile( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, clearProject )( JNIEnv *env, jobject obj )
{
  if ( QfAppInterface::instance() )
  {
    QfAppInterface::instance()->clearProject();
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, executeAction )( JNIEnv *env, jobject obj, jstring action )
{
  if ( QfAppInterface::instance() )
  {
    const char *actionStr = env->GetStringUTFChars( action, NULL );
    QfAppInterface::instance()->executeAction( QString( actionStr ) );
    env->ReleaseStringUTFChars( action, actionStr );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, openPath )( JNIEnv *env, jobject obj, jstring path )
{
  if ( QfAppInterface::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    emit QfAppInterface::instance()->openPath( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

#define ANDROID_VOLUME_DOWN 25
#define ANDROID_VOLUME_UP 24

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, volumeKeyDown )( JNIEnv *env, jobject obj, int volumeKeyCode )
{
  if ( QfAppInterface::instance() )
  {
    emit QfAppInterface::instance()->volumeKeyDown( volumeKeyCode == ANDROID_VOLUME_DOWN ? Qt::Key_VolumeDown : Qt::Key_VolumeUp );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, volumeKeyUp )( JNIEnv *env, jobject obj, int volumeKeyCode )
{
  if ( QfAppInterface::instance() )
  {
    emit QfAppInterface::instance()->volumeKeyUp( volumeKeyCode == ANDROID_VOLUME_DOWN ? Qt::Key_VolumeDown : Qt::Key_VolumeUp );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, resourceReceived )( JNIEnv *env, jobject obj, jstring path )
{
  if ( QfPlatformUtilities::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    emit QfPlatformUtilities::instance()->resourceReceived( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, resourceOpened )( JNIEnv *env, jobject obj, jstring path )
{
  if ( QfPlatformUtilities::instance() )
  {
    const char *pathStr = env->GetStringUTFChars( path, NULL );
    emit QfPlatformUtilities::instance()->resourceOpened( QString( pathStr ) );
    env->ReleaseStringUTFChars( path, pathStr );
  }
  return;
}

JNIEXPORT void JNICALL JNI_FUNCTION_NAME( APP_PACKAGE_JNI_NAME, QFieldActivity, resourceCanceled )( JNIEnv *env, jobject obj, jstring message )
{
  if ( QfPlatformUtilities::instance() )
  {
    const char *messageStr = env->GetStringUTFChars( message, NULL );
    emit QfPlatformUtilities::instance()->resourceCanceled( QString( messageStr ) );
    env->ReleaseStringUTFChars( message, messageStr );
  }
  return;
}

#ifdef __cplusplus
}
#endif
```


