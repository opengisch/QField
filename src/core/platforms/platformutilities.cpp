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

#include "fileutils.h"
#include "picturesource.h"
#include "platformutilities.h"
#include "projectsource.h"
#include "qfield.h"
#include "qfieldcloudconnection.h"
#include "qgismobileapp.h"
#include "qgsmessagelog.h"

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QMargins>
#include <QQuickWindow>
#include <QStandardPaths>
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
  PlatformUtilities::Capabilities capabilities = NoCapabilities;
#if WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void PlatformUtilities::copySampleProjects()
{
  const bool success = FileUtils::copyRecursively( systemSharedDataLocation() + QLatin1String( "/qfield/sample_projects" ), systemLocalDataLocation( QLatin1String( "sample_projects" ) ) );
  Q_ASSERT( success );
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
{}

QString PlatformUtilities::systemSharedDataLocation() const
{
  /**
   * By default, assume that we have a layout like this:
   *
   * [prefix_path]
   * |-- bin
   * |   |-- qfield.exe
   * |-- share
   * |   |-- qfield
   * |   |   |-- sample_projects
   * |   |-- proj
   * |   |   |-- data
   * |   |   |   |--  proj.db
   *
   * systemSharedDataLocation()'s return value will therefore be - relative to qfield.exe - '../share'.
   * However it is possible to override this default logic through a environment variable named
   * QFIELD_SYSTEM_SHARED_DATA_PATH. If present, its value will be used as the return value instead.
  */
  const static QString sharePath = QDir( QFileInfo( !QCoreApplication::applicationFilePath().isEmpty() ? QCoreApplication::applicationFilePath() : QCoreApplication::arguments().value( 0 ) ).canonicalPath()
                                         + QLatin1String( "/../share" ) )
                                     .absolutePath();
  const static QString environmentSharePath = QString( qgetenv( "QFIELD_SYSTEM_SHARED_DATA_PATH" ) );
  return !environmentSharePath.isEmpty() ? QDir( environmentSharePath ).absolutePath() : sharePath;
}

QString PlatformUtilities::systemLocalDataLocation( const QString &subDir ) const
{
  return QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) + '/' + subDir;
}

QString PlatformUtilities::qgsProject() const
{
  return QString();
}

QStringList PlatformUtilities::appDataDirs() const
{
  return QStringList() << QStandardPaths::standardLocations( QStandardPaths::DocumentsLocation ).first() + QStringLiteral( "/QField/" );
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

bool PlatformUtilities::renameFile( const QString &filename, const QString &newname ) const
{
  QFile file( filename );
  return file.rename( newname );
}

QString PlatformUtilities::applicationDirectory() const
{
  return QString();
}

QStringList PlatformUtilities::additionalApplicationDirectories() const
{
  return QStringList() << QString();
}

QStringList PlatformUtilities::rootDirectories() const
{
  return QStringList() << QString();
}

void PlatformUtilities::importProjectFolder() const
{}

void PlatformUtilities::importProjectArchive() const
{}

void PlatformUtilities::importDatasets() const
{}

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
  Q_UNUSED( path )
}

void PlatformUtilities::removeFolder( const QString &path ) const
{
  Q_UNUSED( path )
}

PictureSource *PlatformUtilities::getCameraPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  Q_UNUSED( parent )
  Q_UNUSED( prefix )
  Q_UNUSED( pictureFilePath )
  Q_UNUSED( suffix )
  return nullptr;
}

PictureSource *PlatformUtilities::getGalleryPicture( QQuickItem *parent, const QString &prefix, const QString &pictureFilePath )
{
  Q_UNUSED( parent )
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Media File" ), prefix, tr( "JPEG images (*.jpg *.jpeg)" ) );

  if ( QFileInfo::exists( fileName ) )
  {
    // if the file is already in the prefixed path, no need to copy
    if ( fileName.startsWith( prefix ) )
    {
      return new PictureSource( nullptr, prefix, fileName );
    }
    else
    {
      QString destinationFile = prefix + pictureFilePath;
      QFileInfo destinationInfo( destinationFile );
      QDir prefixDir( prefix );
      if ( prefixDir.mkpath( destinationInfo.absolutePath() ) && QFile::copy( fileName, destinationFile ) )
      {
        return new PictureSource( nullptr, prefix, destinationFile );
      }
    }

    QgsMessageLog::logMessage( tr( "Failed to save gallery picture" ), "QField", Qgis::Critical );
  }

  return new PictureSource( nullptr, prefix, QString() );
}

ViewStatus *PlatformUtilities::open( const QString &uri, bool )
{
  QDesktopServices::openUrl( QStringLiteral( "file://%1" ).arg( uri ) );
  return nullptr;
}

ProjectSource *PlatformUtilities::openProject( QObject *parent )
{
  Q_UNUSED( parent );
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

bool PlatformUtilities::checkWriteExternalStoragePermissions() const
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

QVariantMap PlatformUtilities::sceneMargins( QQuickWindow *window ) const
{
  QVariantMap margins;
  margins[QLatin1String( "top" )] = 0.0;
  margins[QLatin1String( "right" )] = 0.0;
  margins[QLatin1String( "bottom" )] = 0.0;
  margins[QLatin1String( "left" )] = 0.0;

  QPlatformWindow *platformWindow = static_cast<QPlatformWindow *>( window->handle() );
  if ( platformWindow )
  {
    margins[QLatin1String( "top" )] = platformWindow->safeAreaMargins().top();
    margins[QLatin1String( "bottom" )] = platformWindow->safeAreaMargins().bottom();
  }

  return margins;
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

PlatformUtilities *PlatformUtilities::instance()
{
  return sPlatformUtils;
}
