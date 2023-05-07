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
#include "androidprojectsource.h"
#include "androidresourcesource.h"
#include "androidviewstatus.h"
#include "appinterface.h"
#include "fileutils.h"
#include "qfield.h"
#include "qfield_android.h"
#include "qfieldcloudconnection.h"


#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>

inline QAndroidJniObject qtAndroidContext()
{
  auto result = QtAndroid::androidActivity();
  if ( result.isValid() )
    return result;
  return QtAndroid::androidService();
}

inline void runOnAndroidMainThread( const QtAndroid::Runnable &runnable )
{
  QtAndroid::runOnAndroidThread( runnable );
}

#else
#include <QJniEnvironment>
#include <QJniObject>
#include <QtCore/private/qandroidextras_p.h>

inline QJniObject qtAndroidContext()
{
  return QJniObject( QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->context() );
}

inline void runOnAndroidMainThread( const std::function<void()> &runnable )
{
  QCoreApplication::instance()->nativeInterface<QNativeInterface::QAndroidApplication>()->runOnAndroidMainThread( [runnable]() {
    runnable();
    return QVariant();
  } );
}
#endif

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

AndroidPlatformUtilities::AndroidPlatformUtilities()
  : mActivity( qtAndroidContext() )
  , mSystemGenericDataLocation( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + QStringLiteral( "/share" ) )
{
}

PlatformUtilities::Capabilities AndroidPlatformUtilities::capabilities() const
{
  PlatformUtilities::Capabilities capabilities = Capabilities() | NativeCamera | AdjustBrightness | CustomLocalDataPicker | CustomImport | CustomExport | CustomSend | FilePicker;
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject messageJni = QAndroidJniObject::fromString( QObject::tr( "Please wait while QField installation finalizes." ) );
#else
        QJniObject messageJni = QJniObject::fromString( QObject::tr( "Please wait while QField installation finalizes." ) );
#endif
        activity.callMethod<void>( "showBlockingProgressDialog", "(Ljava/lang/String;)V", messageJni.object<jstring>() );
      }
    } );
  }

  FileUtils::copyRecursively( QStringLiteral( "assets:/" ), mSystemGenericDataLocation );

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
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getApplicationDirectory" );
#else
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getApplicationDirectory" );
#endif
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
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getAdditionalApplicationDirectories" );
#else
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getAdditionalApplicationDirectories" );
#endif
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
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getRootDirectories" );
#else
    QJniObject rootDirs = mActivity.callObjectMethod<jstring>( "getRootDirectories" );
#endif
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
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
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathsJni = QAndroidJniObject::fromString( paths.join( "--;--" ) );
#else
        QJniObject pathsJni = QJniObject::fromString( paths.join( "--;--" ) );
#endif
        activity.callMethod<void>( "sendDatasetTo", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::exportDatasetTo( const QString &path ) const
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
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathsJni = QAndroidJniObject::fromString( paths.join( "--;--" ) );
#else
        QJniObject pathsJni = QJniObject::fromString( paths.join( "--;--" ) );
#endif
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathsJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::removeDataset( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
#else
        QJniObject pathJni = QJniObject::fromString( path );
#endif
        activity.callMethod<void>( "removeDataset", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::exportFolderTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
#else
        QJniObject pathJni = QJniObject::fromString( path );
#endif
        activity.callMethod<void>( "exportToFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::sendCompressedFolderTo( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
#else
        QJniObject pathJni = QJniObject::fromString( path );
#endif
        activity.callMethod<void>( "sendCompressedFolderTo", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

void AndroidPlatformUtilities::removeFolder( const QString &path ) const
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [path] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject pathJni = QAndroidJniObject::fromString( path );
#else
        QJniObject pathJni = QJniObject::fromString( path );
#endif
        activity.callMethod<void>( "removeProjectFolder", "(Ljava/lang/String;)V", pathJni.object<jstring>() );
      }
    } );
  }
}

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
QString AndroidPlatformUtilities::getIntentExtra( const QString &extra, QAndroidJniObject extras ) const
#else
QString AndroidPlatformUtilities::getIntentExtra( const QString &extra, QJniObject extras ) const
#endif
{
  if ( extras == nullptr )
  {
    extras = getNativeExtras();
  }
  if ( extras.isValid() )
  {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    QAndroidJniObject extraJni = QAndroidJniObject::fromString( extra );
#else
    QJniObject extraJni = QJniObject::fromString( extra );
#endif
    extraJni = extras.callObjectMethod( "getString", "(Ljava/lang/String;)Ljava/lang/String;", extraJni.object<jstring>() );
    if ( extraJni.isValid() )
    {
      return extraJni.toString();
    }
  }
  return QString();
}

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
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
#else
QJniObject AndroidPlatformUtilities::getNativeIntent() const
{
  if ( mActivity.isValid() )
  {
    QJniObject intent = mActivity.callObjectMethod( "getIntent", "()Landroid/content/Intent;" );
    return intent;
  }
  return nullptr;
}

QJniObject AndroidPlatformUtilities::getNativeExtras() const
{
  QJniObject intent = getNativeIntent();
  if ( intent.isValid() )
  {
    QJniObject extras = intent.callObjectMethod( "getExtras", "()Landroid/os/Bundle;" );

    return extras;
  }
  return nullptr;
}
#endif

ResourceSource *AndroidPlatformUtilities::processCameraActivity( const QString &prefix, const QString &filePath, const QString &suffix, bool isVideo )
{
  if ( !checkCameraPermissions() )
    return nullptr;

  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldCameraActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
#else
  QJniObject activity = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldCameraActivity" ) );
  QJniObject intent = QJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QJniObject packageName = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
#endif
  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject isVideo_label = QAndroidJniObject::fromString( "isVideo" );
  QAndroidJniObject isVideo_value = QAndroidJniObject::fromString( "yes" );
  QAndroidJniObject filePath_label = QAndroidJniObject::fromString( "filePath" );
  QAndroidJniObject filePath_value = QAndroidJniObject::fromString( filePath );
  QAndroidJniObject prefix_label = QAndroidJniObject::fromString( "prefix" );
  QAndroidJniObject prefix_value = QAndroidJniObject::fromString( prefix );
  QAndroidJniObject suffix_label = QAndroidJniObject::fromString( "suffix" );
  QAndroidJniObject suffix_value = QAndroidJniObject::fromString( suffix );
#else
  QJniObject isVideo_label = QJniObject::fromString( "isVideo" );
  QJniObject isVideo_value = QJniObject::fromString( "yes" );
  QJniObject filePath_label = QJniObject::fromString( "filePath" );
  QJniObject filePath_value = QJniObject::fromString( filePath );
  QJniObject prefix_label = QJniObject::fromString( "prefix" );
  QJniObject prefix_value = QJniObject::fromString( prefix );
  QJniObject suffix_label = QJniObject::fromString( "suffix" );
  QJniObject suffix_value = QJniObject::fromString( suffix );
#endif
  if ( isVideo )
  {
    intent.callObjectMethod( "putExtra",
                             "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                             isVideo_label.object<jstring>(),
                             isVideo_value.object<jstring>() );
  }

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           filePath_label.object<jstring>(),
                           filePath_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           prefix_label.object<jstring>(),
                           prefix_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           suffix_label.object<jstring>(),
                           suffix_value.object<jstring>() );

  QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), true );

  AndroidResourceSource *pictureSource = new AndroidResourceSource( prefix );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );
#else
  QtAndroidPrivate::startActivity( intent.object<jobject>(), 171, pictureSource );
#endif
  return pictureSource;
}

ResourceSource *AndroidPlatformUtilities::getCameraPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  Q_UNUSED( parent )

  return processCameraActivity( prefix, pictureFilePath, suffix, false );
}

ResourceSource *AndroidPlatformUtilities::getCameraVideo( QQuickItem *parent, const QString &prefix, const QString &videoFilePath, const QString &suffix )
{
  Q_UNUSED( parent )

  return processCameraActivity( prefix, videoFilePath, suffix, true );
}

ResourceSource *AndroidPlatformUtilities::processGalleryActivity( const QString &prefix, const QString &filePath, const QString &mimeType )
{
  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldGalleryActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QAndroidJniObject filePath_label = QAndroidJniObject::fromString( "filePath" );
  QAndroidJniObject filePath_value = QAndroidJniObject::fromString( filePath );
  QAndroidJniObject prefix_label = QAndroidJniObject::fromString( "prefix" );
  QAndroidJniObject prefix_value = QAndroidJniObject::fromString( prefix );
  QAndroidJniObject mimeType_label = QAndroidJniObject::fromString( "mimeType" );
  QAndroidJniObject mimeType_value = QAndroidJniObject::fromString( mimeType );
#else
  QJniObject activity = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldGalleryActivity" ) );
  QJniObject intent = QJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QJniObject packageName = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QJniObject filePath_label = QJniObject::fromString( "filePath" );
  QJniObject filePath_value = QJniObject::fromString( filePath );
  QJniObject prefix_label = QJniObject::fromString( "prefix" );
  QJniObject prefix_value = QJniObject::fromString( prefix );
  QJniObject mimeType_label = QJniObject::fromString( "mimeType" );
  QJniObject mimeType_value = QJniObject::fromString( mimeType );
#endif
  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           filePath_label.object<jstring>(),
                           filePath_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           prefix_label.object<jstring>(),
                           prefix_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           mimeType_label.object<jstring>(),
                           mimeType_value.object<jstring>() );

  AndroidResourceSource *pictureSource = new AndroidResourceSource( prefix );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QtAndroid::startActivity( intent.object<jobject>(), 171, pictureSource );
#else
  QtAndroidPrivate::startActivity( intent.object<jobject>(), 171, pictureSource );
#endif
  return pictureSource;
}

ResourceSource *AndroidPlatformUtilities::getGalleryPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath )
{
  Q_UNUSED( parent )

  return processGalleryActivity( prefix, pictureFilePath, QStringLiteral( "image/*" ) );
}

ResourceSource *AndroidPlatformUtilities::getGalleryVideo( QQuickItem *parent, const QString &prefix, const QString &videoFilePath )
{
  Q_UNUSED( parent )

  return processGalleryActivity( prefix, videoFilePath, QStringLiteral( "video/*" ) );
}

ResourceSource *AndroidPlatformUtilities::getFile( QQuickItem *parent, const QString &prefix, const QString &filePath, FileType fileType )
{
  Q_UNUSED( parent )

  const QFileInfo destinationInfo( prefix + filePath );
  const QDir prefixDir( prefix );
  prefixDir.mkpath( destinationInfo.absolutePath() );

  QString mimeType;
  switch ( fileType )
  {
    case AudioFiles:
      mimeType = "audio/*";
      break;
    case AllFiles:
    default:
      mimeType = "*/*";
      break;
  }

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldFilePickerActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QAndroidJniObject filePath_label = QAndroidJniObject::fromString( "filePath" );
  QAndroidJniObject filePath_value = QAndroidJniObject::fromString( filePath );
  QAndroidJniObject prefix_label = QAndroidJniObject::fromString( "prefix" );
  QAndroidJniObject prefix_value = QAndroidJniObject::fromString( prefix );
  QAndroidJniObject mimeType_label = QAndroidJniObject::fromString( "mimeType" );
  QAndroidJniObject mimeType_value = QAndroidJniObject::fromString( mimeType );
#else
  QJniObject activity = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldFilePickerActivity" ) );
  QJniObject intent = QJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QJniObject packageName = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QJniObject filePath_label = QJniObject::fromString( "filePath" );
  QJniObject filePath_value = QJniObject::fromString( filePath );
  QJniObject prefix_label = QJniObject::fromString( "prefix" );
  QJniObject prefix_value = QJniObject::fromString( prefix );
  QJniObject mimeType_label = QJniObject::fromString( "mimeType" );
  QJniObject mimeType_value = QJniObject::fromString( mimeType );
#endif

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           filePath_label.object<jstring>(),
                           filePath_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           prefix_label.object<jstring>(),
                           prefix_value.object<jstring>() );

  intent.callObjectMethod( "putExtra",
                           "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;",
                           mimeType_label.object<jstring>(),
                           mimeType_value.object<jstring>() );

  AndroidResourceSource *fileSource = new AndroidResourceSource( prefix );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QtAndroid::startActivity( intent.object<jobject>(), 171, fileSource );
#else
  QtAndroidPrivate::startActivity( intent.object<jobject>(), 171, fileSource );
#endif

  return fileSource;
}

ViewStatus *AndroidPlatformUtilities::open( const QString &uri, bool editing )
{
  if ( QFileInfo( uri ).isDir() )
    return nullptr;

  checkWriteExternalStoragePermissions();

  QMimeDatabase db;

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QAndroidJniObject activity = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldOpenExternallyActivity" ) );
  QAndroidJniObject intent = QAndroidJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QAndroidJniObject packageName = QAndroidJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QAndroidJniObject filepath_label = QAndroidJniObject::fromString( "filepath" );
  QAndroidJniObject filepath = QAndroidJniObject::fromString( uri );
  QAndroidJniObject filetype_label = QAndroidJniObject::fromString( "filetype" );
  QAndroidJniObject filetype = QAndroidJniObject::fromString( db.mimeTypeForFile( uri ).name() );
  QAndroidJniObject fileediting_label = QAndroidJniObject::fromString( "fileediting" );
  QAndroidJniObject fileediting = QAndroidJniObject::fromString( editing ? "true" : "false" );
#else
  QJniObject activity = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ".QFieldOpenExternallyActivity" ) );
  QJniObject intent = QJniObject( "android/content/Intent", "(Ljava/lang/String;)V", activity.object<jstring>() );
  QJniObject packageName = QJniObject::fromString( QStringLiteral( "ch.opengis." APP_PACKAGE_NAME ) );
  QJniObject filepath_label = QJniObject::fromString( "filepath" );
  QJniObject filepath = QJniObject::fromString( uri );
  QJniObject filetype_label = QJniObject::fromString( "filetype" );
  QJniObject filetype = QJniObject::fromString( db.mimeTypeForFile( uri ).name() );
  QJniObject fileediting_label = QJniObject::fromString( "fileediting" );
  QJniObject fileediting = QJniObject::fromString( editing ? "true" : "false" );
#endif

  intent.callObjectMethod( "setClassName", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", packageName.object<jstring>(), activity.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filepath_label.object<jstring>(), filepath.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", filetype_label.object<jstring>(), filetype.object<jstring>() );
  intent.callObjectMethod( "putExtra", "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/Intent;", fileediting_label.object<jstring>(), fileediting.object<jstring>() );

  AndroidViewStatus *viewStatus = new AndroidViewStatus();
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QtAndroid::startActivity( intent.object<jobject>(), 102, viewStatus );
#else
  QtAndroidPrivate::startActivity( intent.object<jobject>(), 102, viewStatus );
#endif

  return viewStatus;
}

bool AndroidPlatformUtilities::checkPositioningPermissions() const
{
  // First check for coarse permissions. If the user configured QField to only get coarse permissions
  // it's his wish and we just let it be.
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QtAndroid::PermissionResult r = QtAndroid::checkPermission( "android.permission.ACCESS_COARSE_LOCATION" );
  if ( r == QtAndroid::PermissionResult::Denied )
#else
  auto r = QtAndroidPrivate::checkPermission( "android.permission.ACCESS_COARSE_LOCATION" ).result();
  if ( r == QtAndroidPrivate::Denied )
#endif
  {
    return checkAndAcquirePermissions( "android.permission.ACCESS_FINE_LOCATION" );
  }
  return true;
}

bool AndroidPlatformUtilities::checkCameraPermissions() const
{
  return checkAndAcquirePermissions( "android.permission.CAMERA" );
}

bool AndroidPlatformUtilities::checkMicrophonePermissions() const
{
  return checkAndAcquirePermissions( "android.permission.RECORD_AUDIO" );
}

bool AndroidPlatformUtilities::checkWriteExternalStoragePermissions() const
{
  return checkAndAcquirePermissions( "android.permission.WRITE_EXTERNAL_STORAGE" );
}

bool AndroidPlatformUtilities::checkAndAcquirePermissions( const QString &permissions ) const
{
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  QStringList requestedPermissions = permissions.split( ';' );
  requestedPermissions.erase( std::remove_if( requestedPermissions.begin(), requestedPermissions.end(),
                                              []( const QString &permission ) {
                                                return QtAndroid::checkPermission( permission ) != QtAndroid::PermissionResult::Denied;
                                              } ),
                              requestedPermissions.end() );

  if ( !requestedPermissions.isEmpty() )
  {
    QtAndroid::requestPermissionsSync( requestedPermissions );
    for ( const QString &permission : requestedPermissions )
    {
      QtAndroid::PermissionResult r = QtAndroid::checkPermission( permission );
      if ( r == QtAndroid::PermissionResult::Denied )
      {
        return false;
      }
    }
  }

  return true;
#else
  QStringList requestedPermissions = permissions.split( ';' );
  requestedPermissions.erase( std::remove_if( requestedPermissions.begin(), requestedPermissions.end(),
                                              []( const QString &permission ) {
                                                auto r = QtAndroidPrivate::checkPermission( permission ).result();
                                                return r != QtAndroidPrivate::Denied;
                                              } ),
                              requestedPermissions.end() );

  if ( !requestedPermissions.isEmpty() )
  {
    for ( const QString &permission : requestedPermissions )
    {
      auto r = QtAndroidPrivate::requestPermission( permission ).result();
      if ( r == QtAndroidPrivate::Denied )
      {
        return false;
      }
    }
  }

  return true;
#endif
}

void AndroidPlatformUtilities::setScreenLockPermission( const bool allowLock )
{
  if ( mActivity.isValid() )
  {
    runOnAndroidMainThread( [allowLock] {
      auto activity = qtAndroidContext();
      if ( activity.isValid() )
      {
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
        QAndroidJniObject window = activity.callObjectMethod( "getWindow", "()Landroid/view/Window;" );
#else
        QJniObject window = activity.callObjectMethod( "getWindow", "()Landroid/view/Window;" );
#endif

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

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
      QAndroidJniEnvironment env;
#else
      QJniEnvironment env;
#endif
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
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
    runOnAndroidMainThread( [] {
      auto activity = qtAndroidContext();
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

  double statusBarMargin = std::abs( static_cast<double>( mActivity.callMethod<jdouble>( "statusBarMargin" ) ) );
  double navigationBarMargin = std::abs( static_cast<double>( mActivity.callMethod<jdouble>( "navigationBarMargin" ) ) );

  statusBarMargin /= QGuiApplication::primaryScreen()->devicePixelRatio();
  navigationBarMargin /= QGuiApplication::primaryScreen()->devicePixelRatio();

  QVariantMap margins;
  margins[QLatin1String( "top" )] = statusBarMargin;
  margins[QLatin1String( "right" )] = 0.0;
  margins[QLatin1String( "bottom" )] = navigationBarMargin;
  margins[QLatin1String( "left" )] = 0.0;
  return margins;
}

void AndroidPlatformUtilities::uploadPendingAttachments( QFieldCloudConnection *connection ) const
{
  QTimer::singleShot( 500, [connection]() {
    if ( connection )
    {
      qInfo() << "Launching service from main...";
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
      QAndroidJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldService",
                                                 "startQFieldService",
                                                 "(Landroid/content/Context;)V",
                                                 qtAndroidContext().object() );
#else
      QJniObject::callStaticMethod<void>( "ch/opengis/" APP_PACKAGE_NAME "/QFieldService",
                                          "startQFieldService",
                                          "(Landroid/content/Context;)V",
                                          qtAndroidContext().object() );
#endif
    }
  } );
}

bool AndroidPlatformUtilities::isSystemDarkTheme() const
{
  if ( mActivity.isValid() )
  {
    bool isDarkTheme = mActivity.callMethod<jboolean>( "isDarkTheme" ) == JNI_TRUE;
    return isDarkTheme;
  }
  return false;
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
