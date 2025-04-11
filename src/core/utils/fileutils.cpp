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
#include "gnsspositioninformation.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QMimeDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QStandardPaths>
#include <qgis.h>
#include <qgsexiftools.h>
#include <qgsfileutils.h>
#include <qgsproject.h>
#include <qgsrendercontext.h>
#include <qgstextformat.h>
#include <qgstextrenderer.h>


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

bool FileUtils::isImageMimeTypeSupported( const QString &mimeType )
{
  return QImageReader::supportedMimeTypes().contains( mimeType.toLatin1() );
}

QString FileUtils::absolutePath( const QString &filePath )
{
  QFileInfo fileInfo( filePath );
  return fileInfo.absolutePath();
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
  const qsizetype sfLength = sourceFolder.length();

  while ( dirIt.hasNext() )
  {
    QString filePath = dirIt.next();
    const QString relPath = filePath.mid( sfLength );
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

QString FileUtils::fileEtag( const QString &fileName, int partSize )
{
  QFile f( fileName );
  if ( !f.open( QFile::ReadOnly ) )
    return QString();

  const qint64 fileSize = f.size();
  QCryptographicHash hash( QCryptographicHash::Md5 );
  if ( fileSize <= partSize )
  {
    if ( hash.addData( &f ) )
    {
      return hash.result().toHex();
    }
  }
  else
  {
    QByteArray md5SumsData;
    qint64 readSize = 0;
    while ( readSize < fileSize )
    {
      hash.addData( f.read( partSize ) );
      md5SumsData += hash.result();
      hash.reset();
      readSize += partSize;
    }
    hash.addData( md5SumsData );
    return QStringLiteral( "%1-%2" ).arg( hash.result().toHex() ).arg( readSize / partSize );
  }
  return QString();
}

void FileUtils::restrictImageSize( const QString &imagePath, int maximumWidthHeight )
{
  if ( !QFileInfo::exists( imagePath ) )
  {
    return;
  }

  QVariantMap metadata = QgsExifTools::readTags( imagePath );
  QImage img( imagePath );
  if ( !img.isNull() && ( img.width() > maximumWidthHeight || img.height() > maximumWidthHeight ) )
  {
    QImage scaledImage = img.width() > img.height()
                           ? img.scaledToWidth( maximumWidthHeight, Qt::SmoothTransformation )
                           : img.scaledToHeight( maximumWidthHeight, Qt::SmoothTransformation );
    scaledImage.save( imagePath, nullptr, 90 );

    for ( const QString &key : metadata.keys() )
    {
      QgsExifTools::tagImage( imagePath, key, metadata[key] );
    }
  }
}

void FileUtils::addImageMetadata( const QString &imagePath, const GnssPositionInformation &positionInformation )
{
  if ( !QFileInfo::exists( imagePath ) )
  {
    return;
  }

  QVariantMap metadata;
  if ( positionInformation.latitudeValid() && positionInformation.longitudeValid() )
  {
    metadata["Exif.GPSInfo.GPSLatitude"] = std::abs( positionInformation.latitude() );
    metadata["Exif.GPSInfo.GPSLatitudeRef"] = positionInformation.latitude() >= 0 ? "N" : "S";
    metadata["Exif.GPSInfo.GPSLongitude"] = std::abs( positionInformation.longitude() );
    metadata["Exif.GPSInfo.GPSLongitudeRef"] = positionInformation.longitude() >= 0 ? "E" : "W";
    if ( positionInformation.elevationValid() )
    {
      metadata["Exif.GPSInfo.GPSAltitude"] = std::abs( positionInformation.elevation() );
      metadata["Exif.GPSInfo.GPSAltitudeRef"] = positionInformation.elevation() >= 0 ? "1" : "0";
    }
  }
  if ( positionInformation.orientationValid() )
  {
    metadata["Exif.GPSInfo.GPSImgDirection"] = positionInformation.orientation();
    metadata["Exif.GPSInfo.GPSImgDirectionRef"] = "M";
  }
  if ( positionInformation.speedValid() )
  {
    metadata["Exif.GPSInfo.GPSSpeed"] = positionInformation.speed();
    metadata["Exif.GPSInfo.GPSSpeedRef"] = "K";
  }

  metadata["Exif.GPSInfo.GPSDateStamp"] = positionInformation.utcDateTime().date();
  metadata["Exif.GPSInfo.GPSTimeStamp"] = positionInformation.utcDateTime().time();

  metadata["Exif.GPSInfo.GPSSatellites"] = QString::number( positionInformation.satellitesUsed() ).rightJustified( 2, '0' );

  metadata["Exif.Image.Make"] = QStringLiteral( "QField" );
  metadata["Xmp.tiff.Make"] = QStringLiteral( "QField" );

  for ( const QString key : metadata.keys() )
  {
    QgsExifTools::tagImage( imagePath, key, metadata[key] );
  }
}

void FileUtils::addImageStamp( const QString &imagePath, const QString &text )
{
  if ( !QFileInfo::exists( imagePath ) || text.isEmpty() )
  {
    return;
  }

  QVariantMap metadata = QgsExifTools::readTags( imagePath );
  QImage img( imagePath );
  if ( !img.isNull() )
  {
    QPainter painter( &img );
    painter.setRenderHint( QPainter::Antialiasing );

    QFont font = painter.font();
    font.setPixelSize( std::min( img.width(), img.height() ) / 40 );
    font.setBold( true );

    QgsRenderContext context = QgsRenderContext::fromQPainter( &painter );
    QgsTextFormat format;
    format.setFont( font );
    format.setSize( font.pixelSize() );
    format.setSizeUnit( Qgis::RenderUnit::Pixels );
    format.setColor( Qt::white );
    format.buffer().setColor( Qt::black );
    format.buffer().setSize( 2 );
    format.buffer().setSizeUnit( Qgis::RenderUnit::Pixels );
    format.buffer().setEnabled( true );
    QgsTextRenderer::drawText( QRectF( 10, 10, img.width() - 20, img.height() - 20 ), 0, Qgis::TextHorizontalAlignment::Left, text.split( QStringLiteral( "\n" ) ), context, format, true, Qgis::TextVerticalAlignment::Bottom, Qgis::TextRendererFlag::WrapLines );

    img.save( imagePath, nullptr, 90 );

    for ( const QString &key : metadata.keys() )
    {
      QgsExifTools::tagImage( imagePath, key, metadata[key] );
    }
  }
}

bool FileUtils::isWithinProjectDirectory( const QString &filePath )
{
  // Get the project instance
  QgsProject *project = QgsProject::instance();
  if ( !project || project->fileName().isEmpty() )
    return false;

  QFileInfo projectFileInfo( project->fileName() );
  if ( !projectFileInfo.exists() )
    return false;

  // Get the canonical path for the project directory
  QString projectDirCanonical = QFileInfo( projectFileInfo.dir().absolutePath() ).canonicalFilePath();
  if ( projectDirCanonical.isEmpty() )
  {
    // Fallback to absolutePath() if canonicalFilePath() is empty
    projectDirCanonical = QFileInfo( projectFileInfo.dir().absolutePath() ).absoluteFilePath();
    if ( projectDirCanonical.isEmpty() )
      return false;
  }

  // Get target file info and its canonical path
  QFileInfo targetInfo( filePath );
  QString targetCanonical;

  if ( targetInfo.exists() || targetInfo.isSymLink() )
  {
    targetCanonical = targetInfo.canonicalFilePath();
    if ( targetCanonical.isEmpty() )
    {
      targetCanonical = targetInfo.absoluteFilePath();
    }
  }
  else
  {
    // Walk up the directory tree until we find an existing parent
    QDir dir = targetInfo.dir();
    QStringList pendingSegments;

    while ( !dir.exists() && dir.cdUp() )
    {
      pendingSegments.prepend( QFileInfo( dir.path() ).fileName() );
    }

    QString existingCanonical = QFileInfo( dir.path() ).canonicalFilePath();
    if ( existingCanonical.isEmpty() )
    {
      existingCanonical = QFileInfo( dir.path() ).absoluteFilePath();
      if ( existingCanonical.isEmpty() )
        return false;
    }

    // Rebuild the target path from existing directories
    QDir rebuiltDir( existingCanonical );
    for ( const QString &segment : pendingSegments )
    {
      rebuiltDir.cd( segment );
    }

    targetCanonical = rebuiltDir.absoluteFilePath( targetInfo.fileName() );
  }

  // Normalize path separators
  projectDirCanonical = QDir::fromNativeSeparators( projectDirCanonical );
  targetCanonical = QDir::fromNativeSeparators( targetCanonical );

  // Normalize paths for Windows (case-insensitive check)
#ifdef Q_OS_WIN
  projectDirCanonical = projectDirCanonical.toLower();
  targetCanonical = targetCanonical.toLower();
#endif

  // Check if the target path is equal to or within the project directory
  bool result = targetCanonical == projectDirCanonical || targetCanonical.startsWith( projectDirCanonical + "/" );

  return result;
}

QByteArray FileUtils::readFileContent( const QString &filePath )
{
  QByteArray content;

  if ( !isWithinProjectDirectory( filePath ) )
  {
    qWarning() << QStringLiteral( "Security warning: Attempted to read file outside project directory: %1" ).arg( filePath );
    return QByteArray();
  }

  QFile file( filePath );

  if ( file.exists() )
  {
    if ( file.open( QIODevice::ReadOnly ) )
    {
      content = file.readAll();
      file.close();
    }
    else
    {
      qDebug() << QStringLiteral( "Failed to read file content: %1" ).arg( file.errorString() );
    }
  }
  else
  {
    qDebug() << QStringLiteral( "File does not exist: %1" ).arg( filePath );
  }

  return content;
}

bool FileUtils::writeFileContent( const QString &filePath, const QByteArray &content )
{
  if ( !isWithinProjectDirectory( filePath ) )
  {
    qWarning() << QStringLiteral( "Security warning: Attempted to write file outside project directory: %1" ).arg( filePath );
    return false;
  }

  QFile file( filePath );
  QFileInfo fileInfo( filePath );
  QDir directory = fileInfo.dir();

  bool isLikelyWritable = false;

#ifdef Q_OS_ANDROID
  QString appStorage = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
  isLikelyWritable = filePath.startsWith( appStorage );
#elif defined( Q_OS_IOS )
  QString appStorage = QStandardPaths::writableLocation( QStandardPaths::AppDataLocation );
  isLikelyWritable = filePath.startsWith( appStorage );
#else
  QFileInfo dirInfo( directory.absolutePath() );
  isLikelyWritable = dirInfo.isWritable();
#endif

  if ( !isLikelyWritable )
  {
    qWarning() << QStringLiteral( "Writing to %1 may fail due to platform restrictions. Use PlatformUtilities.applicationDirectory() for a safe location." ).arg( filePath );
  }

  if ( !directory.exists() )
  {
    if ( !directory.mkpath( "." ) )
    {
      qDebug() << QStringLiteral( "Failed to create directory for file: %1. This may be due to permission restrictions." ).arg( filePath );
      return false;
    }
  }

  if ( file.open( QIODevice::WriteOnly ) )
  {
    qint64 bytesWritten = file.write( content );
    file.close();

    if ( bytesWritten != content.size() )
    {
      qDebug() << QStringLiteral( "Failed to write all data to file: %1" ).arg( filePath );
      return false;
    }

    return true;
  }
  else
  {
    QString errorMsg = file.errorString();
    if ( file.error() == QFile::PermissionsError )
    {
      errorMsg += QStringLiteral( " - This may be due to platform security restrictions." );
    }
    qDebug() << QStringLiteral( "Failed to open file for writing: %1" ).arg( errorMsg );
    return false;
  }
}

QVariantMap FileUtils::getFileInfo( const QString &filePath )
{
  QVariantMap info;

  if ( !isWithinProjectDirectory( filePath ) )
  {
    qWarning() << QStringLiteral( "Security warning: Attempted to access file info outside project directory: %1" ).arg( filePath );
    info["exists"] = false;
    info["error"] = QStringLiteral( "Access denied: File is outside the current project directory" );
    return info;
  }

  QFileInfo fileInfo( filePath );
  QFile file( filePath );

  if ( fileInfo.exists() )
  {
    info["exists"] = true;
    info["fileName"] = fileInfo.fileName();
    info["filePath"] = fileInfo.absoluteFilePath();
    info["fileSize"] = fileInfo.size();
    info["lastModified"] = fileInfo.lastModified();
    info["suffix"] = fileInfo.suffix();

    QMimeDatabase db;
    info["mimeType"] = db.mimeTypeForFile( filePath ).name();

    QByteArray md5Hash = fileChecksum( filePath, QCryptographicHash::Md5 );
    if ( !md5Hash.isEmpty() )
    {
      info["md5"] = md5Hash.toHex();
    }
    else
    {
      info["md5Error"] = QStringLiteral( "Could not calculate MD5 hash - possibly due to permission restrictions" );
    }

    if ( file.open( QIODevice::ReadOnly ) )
    {
      file.close();

      QByteArray content = readFileContent( filePath );

      if ( !content.isEmpty() || fileInfo.size() == 0 )
      {
        info["content"] = content;
        info["readable"] = true;
      }
      else
      {
        info["readable"] = false;
        info["readError"] = QStringLiteral( "Failed to read file content" );
      }
    }
    else
    {
      info["readable"] = false;
      info["readError"] = file.errorString();

      if ( file.error() == QFile::PermissionsError )
      {
        info["readError"] = info["readError"].toString() + QStringLiteral( " - This may be due to platform security restrictions" );
      }
    }
  }
  else
  {
    info["exists"] = false;
  }

  return info;
}