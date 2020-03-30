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

#include "platformutilities.h"
#include "projectsource.h"

#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QFileDialog>
#include <QTimer>
#include <QUrl>

PlatformUtilities::~PlatformUtilities()
{
}

QString PlatformUtilities::configDir() const
{
  return QString();
}

QString PlatformUtilities::shareDir() const
{
  return QString();
}

QString PlatformUtilities::packagePath() const
{
  return QString();
}

QString PlatformUtilities::qgsProject() const
{
  return QString();
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
  Q_UNUSED( prefix )
  Q_UNUSED( pictureFilePath )
  return nullptr;
}

ViewStatus *PlatformUtilities::open( const QString &uri )
{
  QDesktopServices::openUrl( QUrl( uri ) );
  return nullptr;
}

QString PlatformUtilities::fieldType( const QgsField &field ) const
{
  return QVariant( field.type() ).typeName();
}

ProjectSource *PlatformUtilities::openProject()
{
  ProjectSource *source = new ProjectSource();
  QString path {QFileDialog::getOpenFileName( nullptr, tr( "Open QGIS Project File" ), QString(), tr( "QGIS Project Files (*.qgs *.qgz)" ) )};
  if ( !path.isEmpty() )
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
