

# File qfplatformutilities.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**qfplatformutilities.cpp**](qfplatformutilities_8cpp.md)

[Go to the documentation of this file](qfplatformutilities_8cpp.md)


```C++
/***************************************************************************
                            qfplatformutilities.cpp  -  utilities for qfield

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

#include "qfappinterface.h"
#include "qfcloudconnection.h"
#include "qffileutils.h"
#include "qfield.h"
#include "qfplatformutilities.h"
#include "qfprojectsource.h"
#include "qfresourcesource.h"
#include "qfstringutils.h"
#include "qfurlutils.h"
#include "qgsmessagelog.h"

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QMargins>
#include <QMessageBox>
#include <QQuickWindow>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QTimer>
#include <QUrl>
#include <QtGui/qpa/qplatformwindow.h>
#include <qgsfileutils.h>
#include <qgsziputils.h>

#if defined( Q_OS_ANDROID )
#include "qfandroidplatformutilities.h"
Q_GLOBAL_STATIC( QfAndroidPlatformUtilities, sPlatformUtils )
#elif defined( Q_OS_IOS )
#include "ios/qfiosplatformutilities.h"
Q_GLOBAL_STATIC( QfIosPlatformUtilities, sPlatformUtils )
#else
Q_GLOBAL_STATIC( QfPlatformUtilities, sPlatformUtils )
#endif

QfPlatformUtilities::~QfPlatformUtilities()
{
}

QfPlatformUtilities::Capabilities QfPlatformUtilities::capabilities() const
{
  QfPlatformUtilities::Capabilities capabilities = QfPlatformUtilities::Capabilities() | CustomSend | FilePicker | NativeLocalDataPicker | UpdateProjectFromArchive;
#if WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void QfPlatformUtilities::copySampleProjects()
{
  if ( QFileInfo::exists( systemSharedDataLocation() + QLatin1String( "/qfield/sample_projects" ) ) )
  {
    QfFileUtils::copyRecursively( systemSharedDataLocation() + QLatin1String( "/qfield/sample_projects" ), systemLocalDataLocation( QLatin1String( "sample_projects" ) ) );
  }
}

void QfPlatformUtilities::initSystem()
{
  const QString appDataLocation = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
  QFile gitRevFile( appDataLocation + QStringLiteral( "/gitRev" ) );
  QByteArray localGitRev;
  if ( gitRevFile.open( QIODevice::ReadOnly ) )
  {
    localGitRev = gitRevFile.readAll();
  }
  gitRevFile.close();
  QByteArray appGitRev = Qfield::gitRev.toUtf8();
  if ( localGitRev != appGitRev )
  {
    afterUpdate();
    copySampleProjects();

    if ( gitRevFile.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
    {
      gitRevFile.write( appGitRev );
      gitRevFile.close();
    }
  }
}

void QfPlatformUtilities::afterUpdate()
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

QString QfPlatformUtilities::systemSharedDataLocation() const
{
  const static QString sharePath = QDir( QFileInfo( !QCoreApplication::applicationFilePath().isEmpty() ? QCoreApplication::applicationFilePath() : QCoreApplication::arguments().value( 0 ) ).canonicalPath()
                                         + QLatin1String( "/../share" ) )
                                     .absolutePath();
  const static QString environmentSharePath = QString( qgetenv( "QFIELD_SYSTEM_SHARED_DATA_PATH" ) );
  return !environmentSharePath.isEmpty() ? QDir( environmentSharePath ).absolutePath() : sharePath;
}

QString QfPlatformUtilities::systemLocalDataLocation( const QString &subDir ) const
{
  return QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + ( !subDir.isEmpty() ? '/' + subDir : QString() );
}

bool QfPlatformUtilities::hasQgsProject() const
{
  return qApp->arguments().count() > 1 && !qApp->arguments().last().isEmpty();
}

void QfPlatformUtilities::loadQgsProject() const
{
  if ( hasQgsProject() )
  {
    QfAppInterface::instance()->loadFile( qApp->arguments().last() );
  }
}

bool QfPlatformUtilities::hasQfAction() const
{
  return false;
}

void QfPlatformUtilities::executeQfAction() const
{
  return;
}

QStringList QfPlatformUtilities::appDataDirs() const
{
  return QStringList() << QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField Documents/QField/" );
}

QStringList QfPlatformUtilities::availableGrids() const
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

bool QfPlatformUtilities::createDir( const QString &path, const QString &dirname ) const
{
  QDir parentDir( path );
  return parentDir.mkdir( dirname );
}

bool QfPlatformUtilities::rmFile( const QString &filename ) const
{
  QFile file( filename );
  return file.remove( filename );
}

bool QfPlatformUtilities::renameFile( const QString &oldFilePath, const QString &newFilePath, bool overwrite ) const
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

QString QfPlatformUtilities::applicationDirectory() const
{
  return QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField Documents/" );
}

QStringList QfPlatformUtilities::additionalApplicationDirectories() const
{
  return QStringList() << QString();
}

QStringList QfPlatformUtilities::rootDirectories() const
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

void QfPlatformUtilities::importProjectFolder() const
{}

void QfPlatformUtilities::importProjectArchive() const
{}

void QfPlatformUtilities::importDatasets() const
{}

void QfPlatformUtilities::updateProjectFromArchive( const QString &projectPath ) const
{
  const QString zipFilePath = QFileDialog::getOpenFileName( nullptr,
                                                            tr( "Select ZIP Archive" ),
                                                            QFileInfo( projectPath ).absolutePath(),
                                                            tr( "ZIP Archives (*.zip)" ) );
  if ( zipFilePath.isEmpty() )
  {
    return;
  }

  QfAppInterface::instance()->clearProject();

  QStringList extractedFiles;
  const QString projectFolder = QFileInfo( projectPath ).absolutePath();
  ( void ) QfFileUtils::unzip( zipFilePath, projectFolder, extractedFiles, false );

  QfAppInterface::instance()->loadFile( projectPath );
}

void QfPlatformUtilities::exportFolderTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void QfPlatformUtilities::exportDatasetTo( const QString &path ) const
{
  Q_UNUSED( path )
}

void QfPlatformUtilities::sendDatasetTo( const QString &path ) const
{
  const QString directory = QFileDialog::getExistingDirectory( nullptr, tr( "Select Destination Folder" ) );
  if ( directory.isEmpty() )
  {
    return;
  }

  QStringList paths = QStringList() << path;
  const QSet<QString> files = QgsFileUtils::sidecarFilesForPath( path );
  for ( const QString &file : files )
  {
    paths << file;
  }

  for ( const QString &file : std::as_const( paths ) )
  {
    const QString destination = QStringLiteral( "%1/%2" ).arg( directory, QFileInfo( file ).fileName() );
    QFile::copy( file, destination );
  }
}

void QfPlatformUtilities::sendCompressedFolderTo( const QString &path ) const
{
  const QString tempZipPath = QStringLiteral( "%1/%2.zip" ).arg( QDir::tempPath(), QFileInfo( path ).fileName() );
  QFile::remove( tempZipPath );

  QStringList files;
  QDirIterator it( path, QDir::Files, QDirIterator::Subdirectories );
  while ( it.hasNext() )
  {
    files << it.next();
  }

  if ( files.isEmpty() || !QgsZipUtils::zip( tempZipPath, files ) )
  {
    return;
  }

  sendDatasetTo( tempZipPath );
}

void QfPlatformUtilities::sendCompressedFilesTo( const QStringList &paths ) const
{
  const QString tempZipPath = QStringLiteral( "%1/qfield_files_%2.zip" ).arg( QDir::tempPath(), QDateTime::currentDateTime().toString( QStringLiteral( "yyyyMMddHHmmss" ) ) );
  QFile::remove( tempZipPath );

  QStringList files;
  for ( const QString &path : paths )
  {
    QFileInfo fi( path );
    if ( fi.isFile() && fi.exists() )
    {
      files << path;
    }
  }

  if ( files.isEmpty() || !QgsZipUtils::zip( tempZipPath, files ) )
  {
    return;
  }

  sendDatasetTo( tempZipPath );
}

void QfPlatformUtilities::removeDataset( const QString &path ) const
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

void QfPlatformUtilities::removeFolder( const QString &path ) const
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

QfResourceSource *QfPlatformUtilities::getCameraPicture( const QString &, const QString &, const QString &, QObject * )
{
  return nullptr;
}

QfResourceSource *QfPlatformUtilities::getCameraVideo( const QString &, const QString &, const QString &, QObject * )
{
  return nullptr;
}

QfResourceSource *QfPlatformUtilities::createResource( const QString &prefix, const QString &filePath, const QString &fileName, QObject *parent )
{
  QFileInfo fi( fileName );
  if ( fi.exists() )
  {
    // if the file is already in the prefixed path, no need to copy
    if ( fileName.startsWith( prefix ) )
    {
      return new QfResourceSource( parent, prefix, fileName );
    }
    else
    {
      QString finalFilePath = QfStringUtils::replaceFilenameTags( filePath, fi.fileName() );
      QString destinationFile = prefix + finalFilePath;
      QFileInfo destinationInfo( destinationFile );
      QDir prefixDir( prefix );
      if ( prefixDir.mkpath( destinationInfo.absolutePath() ) && QFile::copy( fileName, destinationFile ) )
      {
        return new QfResourceSource( parent, prefix, destinationFile );
      }
    }

    QgsMessageLog::logMessage( tr( "Failed to save file resource" ), "QField", Qgis::Critical );
  }

  return new QfResourceSource( parent, prefix, QString() );
}

QfResourceSource *QfPlatformUtilities::getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent )
{
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Image File" ), prefix,
                                                   tr( "All images (*.jpg *.jpeg *.png *.bmp);;JPEG images (*.jpg *.jpeg);;PNG images (*.jpg *.jpeg);;BMP images (*.bmp)" ) );
  return createResource( prefix, pictureFilePath, fileName, parent );
}

QfResourceSource *QfPlatformUtilities::getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent )
{
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Video File" ), prefix,
                                                   tr( "All video (*.mp4 *.mkv *.mov);;MP4 video (*.mp4);;MKV video(*.mkv);;MOV video (*.mov)" ) );
  return createResource( prefix, videoFilePath, fileName, parent );
}

QfResourceSource *QfPlatformUtilities::getFile( const QString &prefix, const QString &filePath, const QString &mimeType, QObject *parent )
{
  QFileDialog fileDialog( nullptr, tr( "Select File" ), prefix );
  fileDialog.setMimeTypeFilters( { mimeType } );
  if ( fileDialog.exec() )
  {
    return createResource( prefix, filePath, fileDialog.selectedFiles().at( 0 ), parent );
  }
  return nullptr;
}

QfViewStatus *QfPlatformUtilities::open( const QString &uri, bool, QObject * )
{
  QDesktopServices::openUrl( QfUrlUtils::fromString( uri ) );
  return nullptr;
}

QfProjectSource *QfPlatformUtilities::openProject( QObject * )
{
  QSettings settings;
  QfProjectSource *source = new QfProjectSource();
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

bool QfPlatformUtilities::checkPositioningPermissions() const
{
  return true;
}

bool QfPlatformUtilities::checkCameraPermissions() const
{
  return true;
}

bool QfPlatformUtilities::checkMicrophonePermissions() const
{
  return true;
}

void QfPlatformUtilities::copyTextToClipboard( const QString &string ) const
{
  QGuiApplication::clipboard()->setText( string );
}

QString QfPlatformUtilities::getTextFromClipboard() const
{
  return QGuiApplication::clipboard()->text();
}

double QfPlatformUtilities::systemFontPointSize() const
{
  return QApplication::font().pointSizeF() + 2.0;
}

void QfPlatformUtilities::uploadPendingAttachments( QfCloudConnection *connection ) const
{
  QTimer::singleShot( 500, [connection]() {
    if ( connection )
    {
      connection->uploadPendingAttachments();
    }
  } );
}

bool QfPlatformUtilities::isSystemDarkTheme() const
{
  return false;
}

QfPlatformUtilities *QfPlatformUtilities::instance()
{
  return sPlatformUtils;
}

Qt::PermissionStatus QfPlatformUtilities::checkCameraPermission() const
{
  QCameraPermission cameraPermission;
  return qApp->checkPermission( cameraPermission );
}

void QfPlatformUtilities::requestCameraPermission( std::function<void( Qt::PermissionStatus )> func )
{
  QCameraPermission cameraPermission;
  qApp->requestPermission( cameraPermission, [this, func]( const QPermission &permission ) { func( permission.status() ); } );
}

Qt::PermissionStatus QfPlatformUtilities::checkMicrophonePermission() const
{
  QMicrophonePermission microphonePermission;
  return qApp->checkPermission( microphonePermission );
}

void QfPlatformUtilities::requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func )
{
  QMicrophonePermission microphonePermission;
  qApp->requestPermission( microphonePermission, [this, func]( const QPermission &permission ) { func( permission.status() ); } );
}
```


