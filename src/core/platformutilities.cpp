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

#include "qgsmessagelog.h"

#include "platformutilities.h"
#include "projectsource.h"

#include <QDebug>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QTimer>

#if defined (Q_OS_ANDROID)
#include "androidplatformutilities.h"
Q_GLOBAL_STATIC(AndroidPlatformUtilities, sPlatformUtils)
#else
Q_GLOBAL_STATIC(PlatformUtilities, sPlatformUtils)
#endif

PlatformUtilities::~PlatformUtilities()
{
}

void PlatformUtilities::initSystem()
{
}

QString PlatformUtilities::systemGenericDataLocation() const
{
  return QStandardPaths::standardLocations( QStandardPaths::GenericDataLocation ).first();
}

QString PlatformUtilities::qgsProject() const
{
  return QString();
}

QString PlatformUtilities::qfieldDataDir() const
{
  return QString();
}

QStringList PlatformUtilities::availableGrids() const
{
  if ( !qfieldDataDir().isEmpty() )
  {
    QDir gridsDir( qfieldDataDir() + "proj/" );
    if ( gridsDir.exists() )
    {
      return gridsDir.entryList( QStringList() << QStringLiteral( "*.tif" ) << QStringLiteral( "*.gtx" ) << QStringLiteral( "*.gsb" ) << QStringLiteral( "*.byn" ) );
    }
  }
  return QStringList();
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

PictureSource *PlatformUtilities::getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  Q_UNUSED( prefix )
  Q_UNUSED( pictureFilePath )
  Q_UNUSED( suffix )
  return nullptr;
}

PictureSource *PlatformUtilities::getGalleryPicture( const QString &prefix, const QString &pictureFilePath )
{
  QString destinationFile = QStringLiteral( "%1/%2" ).arg( prefix, pictureFilePath );
  QString fileName = QFileDialog::getOpenFileName( nullptr, tr( "Select Media File" ), prefix, tr( "JPEG images (*.jpg *.jpeg)" ) );

  if ( QFileInfo::exists( fileName ) )
  {
    // if the file is already in the prefixed path, no need to copy
    if ( fileName.startsWith( prefix ) )
      return new PictureSource( nullptr, prefix, fileName );
    else if ( QFile::copy( fileName, destinationFile ) )
      return new PictureSource( nullptr, prefix, destinationFile );

    QgsMessageLog::logMessage( tr( "Failed to save gallery picture" ), "QField", Qgis::Critical );
  }

  return new PictureSource( nullptr, prefix, QString() );
}

ViewStatus *PlatformUtilities::open( const QString &uri )
{
  QDesktopServices::openUrl( QStringLiteral( "file://%1" ).arg( uri ) );
  return nullptr;
}

QString PlatformUtilities::fieldType( const QgsField &field ) const
{
  return QVariant( field.type() ).typeName();
}

ProjectSource *PlatformUtilities::openProject()
{
  ProjectSource *source = new ProjectSource( );
  QString path { QFileDialog::getOpenFileName( nullptr, tr( "Open QGIS Project File" ), QString(), tr( "QGIS Project Files (*.qgs *.qgz)" ) ) };
  if ( ! path.isEmpty() )
  {
    QTimer::singleShot( 0, this, [source, path]() { emit source->projectOpened( path ); } );
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

PlatformUtilities *PlatformUtilities::instance()
{
  return sPlatformUtils;
}
