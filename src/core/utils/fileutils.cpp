/***************************************************************************
 fileutils.cpp

 ---------------------
 begin                : 29.02.2020
 copyright            : (C) 2020 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "fileutils.h"
#include <QMimeDatabase>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

FileUtils::FileUtils( QObject *parent )
  : QObject( parent )
{

}

QString FileUtils::mimeTypeName( const QString &filePath )
{
  QMimeDatabase db;
  QMimeType mimeType = db.mimeTypeForFile( filePath );
  return mimeType.name();
}

QString FileUtils::fileName( const QString &filePath )
{
  QFileInfo fileInfo( filePath );
  return fileInfo.fileName();
}

QString FileUtils::fileSuffix( const QString &filePath )
{
  QFileInfo fileInfo( filePath );
  return fileInfo.suffix();
}

bool FileUtils::fileExists( const QString &filePath )
{
  QFileInfo fileInfo( filePath );
  return ( fileInfo.exists() && fileInfo.isFile() );
}

bool FileUtils::copyRecursively( const QString &sourceFolder, const QString &destFolder )
{
  QDir sourceDir( sourceFolder );

  if ( !sourceDir.exists() )
    return false;

  QDir destDir( destFolder );
  if ( !destDir.exists() )
    destDir.mkdir( destFolder );

  const QStringList files = sourceDir.entryList( QDir::Files );
  for ( const QString &file : files )
  {
    QString srcName = sourceFolder + QDir::separator() + file;
    QString destName = destFolder + QDir::separator() + file;
    bool success = QFile::copy( srcName, destName );
    if ( !success )
      return false;
  }

  const QStringList dirs = sourceDir.entryList( QDir::AllDirs | QDir::NoDotAndDotDot );
  for ( const QString &dir : dirs )
  {
    QString srcName = sourceFolder + QDir::separator() + dir;
    QString destName = destFolder + QDir::separator() + dir;
    bool success = copyRecursively( srcName, destName );
    if ( !success )
      return false;
  }

  return true;
}
