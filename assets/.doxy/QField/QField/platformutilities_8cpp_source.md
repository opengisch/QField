

# File platformutilities.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**platformutilities.cpp**](platformutilities_8cpp.md)

[Go to the documentation of this file](platformutilities_8cpp.md)


```C++
/***************************************************************************
                            platformutilities.cpp  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "appinterface.h"
#include "fileutils.h"
#include "platformutilities.h"
#include "projectsource.h"
#include "qfield.h"
#include "qfieldcloudconnection.h"
#include "qgsmessagelog.h"
#include "resourcesource.h"
#include "stringutils.h"
#include "urlutils.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QMargins>
#include <QMessageBox>
#include <QQuickWindow>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QTimer>
#include <QUrl>
#include <QtGui/qpa/qplatformwindow.h>

#if defined( Q_OS_ANDROID )
#include "androidplatformutilities.h"
Q_GLOBAL_STATIC( AndroidPlatformUtilities, sPlatformUtils )
#elif defined( Q_OS_IOS )
#include "ios/iosplatformutilities.h"
Q_GLOBAL_STATIC( IosPlatformUtilities, sPlatformUtils )
#else
Q_GLOBAL_STATIC( PlatformUtilities, sPlatformUtils )
#endif

PlatformUtilities::~PlatformUtilities()
{
}

PlatformUtilities::Capabilities PlatformUtilities::capabilities() const
{
  PlatformUtilities::Capabilities capabilities = PlatformUtilities::Capabilities() | FilePicker | NativeLocalDataPicker;
#if WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void PlatformUtilities::copySampleProjects()
{
  if ( QFileInfo::exists( systemSharedDataLocation() + QLatin1String( "/qfield/sample_projects" ) ) )
  {
    FileUtils::copyRecursively( systemSharedDataLocation() + QLatin1String( "/qfield/sample_projects" ), systemLocalDataLocation( QLatin1String( "sample_projects" ) ) );
  }
}

void PlatformUtilities::initSystem()
{
  const QString appDataLocation = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
  QFile gitRevFile( appDataLocation + QStringLiteral( "/gitRev" ) );
  QByteArray localGitRev;
  if ( gitRevFile.open( QIODevice::ReadOnly ) )
  {
    localGitRev = gitRevFile.readAll();
  }
  gitRevFile.close();
  QByteArray appGitRev = qfield::gitRev.toUtf8();
  if ( localGitRev != appGitRev )
  {
    afterUpdate();
    copySampleProjects();

    gitRevFile.open( QIODevice::WriteOnly | QIODevice::Truncate );
    gitRevFile.write( appGitRev );
    gitRevFile.close();
  }
}

void PlatformUtilities::afterUpdate()
{
  const QStringList dirs = appDataDirs();
  for ( const QString &dir : dirs )
  {
    QDir appDataDir( dir );
    appDataDir.mkpath( QStringLiteral( "proj" ) );
    appDataDir.mkpath( QStringLiteral( "auth" ) );
    appDataDir.mkpath( QStringLiteral( "fonts" ) );
    appDataDir.mkpath( QStringLiteral( "basemaps" ) );
    appDataDir.mkpath( QStringLiteral( "logs" ) );
    appDataDir.mkpath( QStringLiteral( "plugins" ) );
  }

  QDir applicationDir( applicationDirectory() );
  applicationDir.mkpath( QStringLiteral( "Created Projects" ) );
  applicationDir.mkpath( QStringLiteral( "Imported Projects" ) );
  applicationDir.mkpath( QStringLiteral( "Imported Datasets" ) );
}

QString PlatformUtilities::systemSharedDataLocation() const
{
  const static QString sharePath = QDir( QFileInfo( !QCoreApplication::applicationFilePath().isEmpty() ? QCoreApplication::applicationFilePath() : QCoreApplication::arguments().value( 0 ) ).canonicalPath()
                                         + QLatin1String( "/../share" ) )
                                     .absolutePath();
  const static QString environmentSharePath = QString( qgetenv( "QFIELD_SYSTEM_SHARED_DATA_PATH" ) );
  return !environmentSharePath.isEmpty() ? QDir( environmentSharePath ).absolutePath() : sharePath;
}

QString PlatformUtilities::systemLocalDataLocation( const QString &subDir ) const
{
  return QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + ( !subDir.isEmpty() ? '/' + subDir : QString() );
}

bool PlatformUtilities::hasQgsProject() const
{
  return qApp->arguments().count() > 1 && !qApp->arguments().last().isEmpty();
}

void PlatformUtilities::loadQgsProject() const
{
  if ( hasQgsProject() )
  {
    AppInterface::instance()->loadFile( qApp->arguments().last() );
  }
}

bool PlatformUtilities::hasQfAction() const
{
  return false;
}

void PlatformUtilities::executeQfAction() const
{
  return;
}

QStringList PlatformUtilities::appDataDirs() const
{
  return QStringList() << QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField Documents/QField/" );
}

QStringList PlatformUtilities::availableGrids() const
{
  QStringList dataDirs = appDataDirs();
  QStringList grids;
  for ( const QString &dataDir : dataDirs )
  {
    QDir gridsDir( dataDir + "proj/" );
    if ( gridsDir.exists() )
    {
      grids << gridsDir.entryList( QStringList() << QStringLiteral( "*.tif" ) << QStringLiteral( "*.gtx" ) << QStringLiteral( "*.gsb" ) << QStringLiteral( "*.byn" ) );
    }
  }
  return grids;
}

bool PlatformUtilities::createDir( const QString &path, const QString &dirname ) const
{
  QDir parentDir( path );
  return parentDir.mkdir( dirname );
}

bool PlatformUtilities::rmFile( const QString &filename ) const
{
  QFile file( filename );
  return file.remove( filename );
}

bool PlatformUtilities::renameFile( const QString &oldFilePath, const QString &newFilePath, bool overwrite ) const
{
  QFileInfo oldFi( oldFilePath );
  QFileInfo newFi( newFilePath );
  if ( oldFi.absoluteFilePath() == newFi.absoluteFilePath() )
  {
    return true;
  }

  bool ok = false;

  // Insure the path exists
  QDir dir( newFi.absolutePath() );
  ok = dir.mkpath( newFi.absolutePath() );
  if ( !ok )
  {
    return false;
  }

  // If the renamed file exists, overwrite
  if ( newFi.exists() && overwrite )
  {
    QFile newfile( newFilePath );
    ok = newfile.remove();
    if ( !ok )
    {
      return false;
    }
  }

  ok = QFile::rename( oldFilePath, newFilePath );
  if ( !ok )
  {
    ok = QFile::copy( oldFilePath, newFilePath );
    QFile oldfile( oldFilePath );
    oldfile.remove();
  }
  return ok;
}

QString PlatformUtilities::applicationDirectory() const
{
  return QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField Documents/" );
}

QStringList PlatformUtilities::additionalApplicationDirectories() const
{
  return QStringList() << QString();
}

QStringList PlatformUtilities::rootDirectories() const
{
  QStringList rootDirectories;
  rootDirectories << QDir::homePath();
  for ( const QStorageInfo &volume : QStorageInfo::mountedVolumes() )
  {
    if ( volume.isReady() && !volume.isReadOnly() )
    {
      if ( volume.fileSystemType() != QLatin1String( "tmpfs" ) && !volume.rootPath().startsWith( QLatin1String( "/boot" ) ) )
      {
        rootDirectories << volume.rootPath();
      }
    }
  }
  return rootDirectories;
}

void PlatformUtilities::importProjectFolder() const
{}

void PlatformUtilities::importProjectArchive() const
{}

void PlatformUtilities::importDatasets() const
{}

void PlatformUtilities::updateProjectFromArchive( const QString &projectPath ) const
{
  Q_UNUSED( projectPath )
}

void PlatformUtilities::exportFolderTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void PlatformUtilities::exportDatasetTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void PlatformUtilities::sendDatasetTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void PlatformUtilities::sendCompressedFolderTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void PlatformUtilities::removeDataset( const QString &path ) const
{
  const QStringList allowedDirectories = QStringList() << applicationDirectory() << additionalApplicationDirectories();
  if ( std::any_of( allowedDirectories.begin(), allowedDirectories.end(), [&path]( const QString &directory ) { return path.startsWith( directory ); } ) )
  {
    if ( QMessageBox::warning( nullptr,
                               tr( "Removal Confirmation" ),
                               tr( "The dataset will be deleted, proceed with removal?" ),
                               QMessageBox::StandardButtons() | QMessageBox::Ok | QMessageBox::Abort )
         == QMessageBox::Ok )
    {
      QFile::moveToTrash( path );
    }
  }
}

void PlatformUtilities::removeFolder( const QString &path ) const
{
  const QStringList allowedDirectories = QStringList() << applicationDirectory() << additionalApplicationDirectories();
  if ( std::any_of( allowedDirectories.begin(), allowedDirectories.end(), [&path]( const QString &directory ) { return path.startsWith( directory ); } ) )
  {
    if ( QMessageBox::warning( nullptr,
                               tr( "Removal Confirmation" ),
                               tr( "The project folder will be deleted, proceed with removal?" ),
                               QMessageBox::StandardButtons() | QMessageBox::Ok | QMessageBox::Abort )
         == QMessageBox::Ok )
    {
      QFile::moveToTrash( path );
    }
  }
}

ResourceSource *PlatformUtilities::getCameraPicture( const QString &, const QString &, const QString &, QObject * )
{
  return nullptr;
}

ResourceSource *PlatformUtilities::getCameraVideo( const QString &, const QString &, const QString &, QObject * )
{
  return nullptr;
}

ResourceSource *PlatformUtilities::createResource( const QString &prefix, const QString &filePath, const QString &fileName, QObject *parent )
{
  QFileInfo fi( fileName );
  if ( fi.exists() )
  {
    // if the file is already in the prefixed path, no need to copy
    if ( fileName.startsWith( prefix ) )
    {
      return new ResourceSource( parent, prefix, fileName );
    }
    else
    {
      QString finalFilePath = StringUtils::replaceFilenameTags( filePath, fi.fileName() );
      QString destinationFile = prefix + finalFilePath;
      QFileInfo destinationInfo( destinationFile );
      QDir prefixDir( prefix );
      if ( prefixDir.mkpath( destinationInfo.absolutePath() ) && QFile::copy( fileName, destinationFile ) )
      {
        return new ResourceSource( parent, prefix, destinationFile );
      }
    }

    QgsMessageLog::logMessage( tr( "Failed to save file resource" ), "QField", Qgis::Critical );
  }

  return new ResourceSource( parent, prefix, QString() );
}

ResourceSource *PlatformUtilities::getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent )
{
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Image File" ), prefix,
                                                   tr( "All images (*.jpg *.jpeg *.png *.bmp);;JPEG images (*.jpg *.jpeg);;PNG images (*.jpg *.jpeg);;BMP images (*.bmp)" ) );
  return createResource( prefix, pictureFilePath, fileName, parent );
}

ResourceSource *PlatformUtilities::getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent )
{
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Video File" ), prefix,
                                                   tr( "All video (*.mp4 *.mkv *.mov);;MP4 video (*.mp4);;MKV video(*.mkv);;MOV video (*.mov)" ) );
  return createResource( prefix, videoFilePath, fileName, parent );
}

ResourceSource *PlatformUtilities::getFile( const QString &prefix, const QString &filePath, FileType fileType, QObject *parent )
{
  QString filter;
  switch ( fileType )
  {
    case AudioFiles:
      filter = tr( "Audio files (*.mp3 *.aac *.ogg *.m4a *.mp4 *.mov)" );
      break;

    case AllFiles:
    default:
      filter = tr( "All files (*.*)" );
      break;
  }

  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select File" ), prefix, filter );
  return createResource( prefix, filePath, fileName, parent );
}

ViewStatus *PlatformUtilities::open( const QString &uri, bool, QObject * )
{
  QDesktopServices::openUrl( UrlUtils::fromString( uri ) );
  return nullptr;
}

ProjectSource *PlatformUtilities::openProject( QObject * )
{
  QSettings settings;
  ProjectSource *source = new ProjectSource();
  QString fileName { QFileDialog::getOpenFileName( nullptr,
                                                   tr( "Open File" ),
                                                   settings.value( QStringLiteral( "QField/lastOpenDir" ), QString() ).toString(),
                                                   QStringLiteral( "%1 (*.%2);;%3 (*.%4);;%5 (*.%6);;%7 (*.%8)" ).arg( tr( "All Supported Files" ), ( SUPPORTED_PROJECT_EXTENSIONS + SUPPORTED_VECTOR_EXTENSIONS + SUPPORTED_RASTER_EXTENSIONS ).join( QStringLiteral( " *." ) ), tr( "QGIS Project Files" ), SUPPORTED_PROJECT_EXTENSIONS.join( QStringLiteral( " *." ) ), tr( "Vector Datasets" ), SUPPORTED_VECTOR_EXTENSIONS.join( QStringLiteral( " *." ) ), tr( "Raster Datasets" ), SUPPORTED_RASTER_EXTENSIONS.join( QStringLiteral( " *." ) ) ) ) };
  if ( !fileName.isEmpty() )
  {
    settings.setValue( QStringLiteral( "/QField/lastOpenDir" ), QFileInfo( fileName ).absolutePath() );
    QTimer::singleShot( 0, this, [source, fileName]() { emit source->projectOpened( fileName ); } );
  }
  return source;
}

bool PlatformUtilities::checkPositioningPermissions() const
{
  return true;
}

bool PlatformUtilities::checkCameraPermissions() const
{
  return true;
}

bool PlatformUtilities::checkMicrophonePermissions() const
{
  return true;
}

void PlatformUtilities::copyTextToClipboard( const QString &string ) const
{
  QGuiApplication::clipboard()->setText( string );
}

QString PlatformUtilities::getTextFromClipboard() const
{
  return QGuiApplication::clipboard()->text();
}

double PlatformUtilities::systemFontPointSize() const
{
  return QApplication::font().pointSizeF() + 2.0;
}

void PlatformUtilities::uploadPendingAttachments( QFieldCloudConnection *connection ) const
{
  QTimer::singleShot( 500, [connection]() {
    if ( connection )
    {
      connection->uploadPendingAttachments();
    }
  } );
}

bool PlatformUtilities::isSystemDarkTheme() const
{
  return false;
}

PlatformUtilities *PlatformUtilities::instance()
{
  return sPlatformUtils;
}

Qt::PermissionStatus PlatformUtilities::checkCameraPermission() const
{
  QCameraPermission cameraPermission;
  return qApp->checkPermission( cameraPermission );
}

void PlatformUtilities::requestCameraPermission( std::function<void( Qt::PermissionStatus )> func )
{
  QCameraPermission cameraPermission;
  qApp->requestPermission( cameraPermission, [this, func]( const QPermission &permission ) { func( permission.status() ); } );
}

Qt::PermissionStatus PlatformUtilities::checkMicrophonePermission() const
{
  QMicrophonePermission microphonePermission;
  return qApp->checkPermission( microphonePermission );
}

void PlatformUtilities::requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func )
{
  QMicrophonePermission microphonePermission;
  qApp->requestPermission( microphonePermission, [this, func]( const QPermission &permission ) { func( permission.status() ); } );
}
```


