/***************************************************************************
                        ziputils.h
                        ---------------
  begin                : March 2023
  copyright            : (C) 2023 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   Original QgsZipUtils code by Paul Blottiere                           *
 *                                                                         *
 ***************************************************************************/

#include "ziputils.h"

#include <QDir>
#include <QFileInfo>
#include <qgsmessagelog.h>

#include <fstream>
#include <iostream>
#include <zip.h>
#include <zlib.h>

const QStringList ZipUtils::files( const QString &zip )
{
  if ( zip.isEmpty() && !QFileInfo::exists( zip ) )
  {
    return QStringList();
  }
  QStringList files;

  int rc = 0;
  const QByteArray fileNamePtr = zip.toUtf8();
  struct zip *z = zip_open( fileNamePtr.constData(), 0, &rc );

  if ( rc == ZIP_ER_OK && z )
  {
    const int count = zip_get_num_files( z );
    if ( count != -1 )
    {
      struct zip_stat stat;

      for ( int i = 0; i < count; i++ )
      {
        zip_stat_index( z, i, 0, &stat );
        files << QString( stat.name );
      }
    }

    zip_close( z );
  }

  return files;
}

bool ZipUtils::unzip( const QString &zipFilename, const QString &dir, QStringList &files, bool checkConsistency )
{
  files.clear();

  if ( !QFileInfo::exists( zipFilename ) )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error zip file does not exist: '%1'" ).arg( zipFilename ) );
    return false;
  }
  else if ( zipFilename.isEmpty() )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error zip filename is empty" ) );
    return false;
  }
  else if ( !QDir( dir ).exists( dir ) )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error output dir does not exist: '%1'" ).arg( dir ) );
    return false;
  }
  else if ( !QFileInfo( dir ).isDir() )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error output dir is not a directory: '%1'" ).arg( dir ) );
    return false;
  }
  else if ( !QFileInfo( dir ).isWritable() )
  {
    QgsMessageLog::logMessage( QObject::tr( "Error output dir is not writable: '%1'" ).arg( dir ) );
    return false;
  }

  int rc = 0;
  const QByteArray fileNamePtr = zipFilename.toUtf8();
  struct zip *z = zip_open( fileNamePtr.constData(), checkConsistency ? ZIP_CHECKCONS : 0, &rc );

  if ( rc == ZIP_ER_OK && z )
  {
    const int count = zip_get_num_files( z );
    if ( count != -1 )
    {
      struct zip_stat stat;

      for ( int i = 0; i < count; i++ )
      {
        zip_stat_index( z, i, 0, &stat );
        const size_t len = stat.size;

        struct zip_file *file = zip_fopen_index( z, i, 0 );
        const std::unique_ptr<char[]> buf( new char[len] );
        if ( zip_fread( file, buf.get(), len ) != -1 )
        {
          const QString fileName( stat.name );
          const QFileInfo newFile( QDir( dir ), fileName );

          // Create path for a new file if it does not exist.
          if ( !newFile.absoluteDir().exists() )
          {
            if ( !QDir( dir ).mkpath( newFile.absolutePath() ) )
              QgsMessageLog::logMessage( QObject::tr( "Failed to create a subdirectory %1/%2" ).arg( dir ).arg( fileName ) );
          }

          QFile outFile( newFile.absoluteFilePath() );
          if ( !outFile.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
          {
            QgsMessageLog::logMessage( QObject::tr( "Could not write to %1" ).arg( newFile.absoluteFilePath() ) );
          }
          else
          {
            outFile.write( buf.get(), len );
          }
          zip_fclose( file );
          files.append( newFile.absoluteFilePath() );
        }
        else
        {
          zip_fclose( file );
          QgsMessageLog::logMessage( QObject::tr( "Error reading file: '%1'" ).arg( zip_strerror( z ) ) );
          return false;
        }
      }
    }
    else
    {
      zip_close( z );
      QgsMessageLog::logMessage( QObject::tr( "Error getting files: '%1'" ).arg( zip_strerror( z ) ) );
      return false;
    }

    zip_close( z );
  }
  else
  {
    QgsMessageLog::logMessage( QObject::tr( "Error opening zip archive: '%1' (Error code: %2)" ).arg( z ? zip_strerror( z ) : zipFilename ).arg( rc ) );
    return false;
  }

  return true;
}
