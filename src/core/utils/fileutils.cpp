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

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QMimeDatabase>
#include <qgis.h>
#include <qgsfileutils.h>

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

QString FileUtils::representFileSize( qint64 bytes )
{
  return QgsFileUtils::representFileSize( bytes );
}

bool FileUtils::copyRecursively( const QString &sourceFolder, const QString &destFolder, QgsFeedback *feedback, bool wipeDestFolder )
{
  // Remove the destination folder and its content if it already exists
  if ( wipeDestFolder )
  {
    QDir destDir( destFolder );
    if ( destDir.exists() )
    {
      bool success = destDir.removeRecursively();
      if ( !success )
      {
        qDebug() << QStringLiteral( "Failed to recursively delete directory %1" ).arg( destFolder );
        return false;
      }
    }
  }

  QList<QPair<QString, QString>> mapping;
  int fileCount = copyRecursivelyPrepare( sourceFolder, destFolder, mapping );

  int current = 0;
  for ( QPair<QString, QString> srcDestFilePair : std::as_const( mapping ) )
  {
    QString srcName = srcDestFilePair.first;
    QString destName = srcDestFilePair.second;

    QFileInfo destInfo( destName );
    if ( QFileInfo( srcName ).isDir() )
      continue;

    QDir destDir( destInfo.absoluteDir() );
    if ( !destDir.exists() )
    {
      destDir.mkpath( destDir.path() );
    }
    if ( QFile::exists( destName ) )
      QFile::remove( destName );

    bool success = QFile::copy( srcName, destName );
    if ( !success )
    {
      qDebug() << QStringLiteral( "Failed to write file %1" ).arg( destName );
      return false;
    }

    QFile( destName ).setPermissions( QFileDevice::ReadOwner | QFileDevice::WriteOwner );

    if ( feedback )
      feedback->setProgress( 100 * current / fileCount );

    ++current;
  }

  return true;
}

int FileUtils::copyRecursivelyPrepare( const QString &sourceFolder, const QString &destFolder, QList<QPair<QString, QString>> &mapping )
{
  QDir sourceDir( sourceFolder );

  if ( !sourceDir.exists() )
    return 0;

  int count = 0;

  QDirIterator dirIt( sourceDir, QDirIterator::Subdirectories );
  int sfLentgh = sourceFolder.length();

  while ( dirIt.hasNext() )
  {
    QString filePath = dirIt.next();
    const QString relPath = filePath.mid( sfLentgh );
    if ( relPath.endsWith( QLatin1String( "/." ) ) || relPath.endsWith( QLatin1String( "/.." ) ) )
      continue;

    QString srcName = QDir::cleanPath( sourceFolder + QDir::separator() + relPath );
    QString destName = QDir::cleanPath( destFolder + QDir::separator() + relPath );

    mapping.append( qMakePair( srcName, destName ) );
    count += 1;
  }

  return count;
}


QByteArray FileUtils::fileChecksum( const QString &fileName, const QCryptographicHash::Algorithm hashAlgorithm )
{
  QFile f( fileName );

  if ( !f.open( QFile::ReadOnly ) )
    return QByteArray();

  QCryptographicHash hash( hashAlgorithm );

  if ( hash.addData( &f ) )
    return hash.result();

  return QByteArray();
}
