

# File fileutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**fileutils.cpp**](fileutils_8cpp.md)

[Go to the documentation of this file](fileutils_8cpp.md)


```C++
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
#include "platforms/platformutilities.h"
#include "qfieldcloudutils.h"
#include "qgsmessagelog.h"

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
#include <qgsapplication.h>
#include <qgsexiftools.h>
#include <qgsfileutils.h>
#include <qgsimagecache.h>
#include <qgsproject.h>
#include <qgsrendercontext.h>
#include <qgssvgcache.h>
#include <qgstextformat.h>
#include <qgstextrenderer.h>

#include <filesystem>
#include <zip.h>
#include <zlib.h>

namespace fs = std::filesystem;

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

QString FileUtils::sanitizeFilePath( const QString &filePath, const QString &replacement )
{
  QString sanitized = filePath;

  // replace multiple slashes
  sanitized.replace( QRegularExpression( "/+" ), QLatin1String( "/" ) );
  sanitized.replace( QRegularExpression( "[\\\\]+" ), QLatin1String( "\\" ) );
  // replace windows dives path with UNIX style path
  sanitized.replace( QRegularExpression( "^([a-zA-Z]):\\\\" ), QLatin1String( "/\\1/" ) );
  // replace windows slashes to UNIX slashes
  sanitized.replace( QLatin1String( "\\" ), QLatin1String( "/" ) );

  QStringList filePathParts;
  const QFileInfo fi( sanitized );
  const fs::path path = fi.filesystemFilePath();

  for ( const auto &partRaw : path )
  {
    const QString part = QString::fromUtf8( partRaw.string() );

    // we should not escape when the path starts with relative path single or double dot
    if ( part == QLatin1String( "." ) || part == QLatin1String( ".." ) )
    {
      filePathParts.append( part );
    }
    // we should not escape when the passed filePath is absolute path. However, `filesystemFilePath()` will return the first slash as a part.
    // In that case we add an empty string to the filePathParts, so we prevent double slash at the start of the string.
    else if ( part == QLatin1String( "/" ) )
    {
      filePathParts.append( QString() );
    }
    // in any other case we should escape all the characters in the string
    else
    {
      filePathParts.append( sanitizeFilePathPart( part, replacement ) );
    }
  }


  // we always convert to UNIX style paths
  sanitized = filePathParts.join( "/" );

  // if the resulting string is longer than max length, return empty string
  if ( sanitized.length() > FILENAME_MAX_CHAR_LENGTH )
  {
    return QString();
  }

  return sanitized;
}

QString FileUtils::sanitizeFilePathPart( const QString &filePathPart, const QString &replacement )
{
  QString sanitizedPart = filePathPart;

  sanitizedPart = sanitizedPart.trimmed();

  // Search for Windows invalid chars to be replaced
  const QRegularExpression invalidChars( QLatin1String( "([<>:\"/\\|\?*\\\\])" ) );
  sanitizedPart.replace( invalidChars, replacement );

  QFileInfo fi( sanitizedPart );

  // Avoid reserved filenames on Windows (CON, PRN, AUX, NUL, COM1-COM9, LPT1-LPT9)
  const QRegularExpression invalidWordsRegEx( QLatin1String( "^(COM[0-9]|CON|LPT[0-9]|NUL|PRN|AUX|com[0-9]|con|lpt[0-9]|nul|prn|aux)$" ) );
  const QRegularExpressionMatch invalidWordsMatch = invalidWordsRegEx.match( fi.completeBaseName() );

  if ( invalidWordsMatch.hasMatch() )
  {
    const QString invalidWordReplacement = QStringLiteral( "_" ).append( invalidWordsMatch.capturedTexts().first() );
    const QString suffix = fi.suffix();

    if ( suffix.isEmpty() )
    {
      sanitizedPart = invalidWordReplacement;
    }
    else
    {
      sanitizedPart = QStringLiteral( "%1.%2" ).arg( invalidWordReplacement, suffix );
    }
  }

  // Linux does not accept null char
  sanitizedPart.replace( QString( QChar( 0 ) ), replacement );

  // File paths shouldn't end with space or dot on Windows
  if ( sanitizedPart.endsWith( '.' ) )
  {
    sanitizedPart.chop( 1 );
    sanitizedPart.append( replacement );
  }

  return sanitizedPart;
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

  for ( const QString &key : metadata.keys() )
  {
    QgsExifTools::tagImage( imagePath, key, metadata[key] );
  }
}

void FileUtils::addImageStamp( const QString &imagePath, const QString &text, const QString &textFormat, Qgis::TextHorizontalAlignment horizontalAlignment, const QString &imageDecoration )
{
  if ( !QFileInfo::exists( imagePath ) || text.isEmpty() )
  {
    return;
  }

  QgsReadWriteContext readWriteContent;
  readWriteContent.setPathResolver( QgsProject::instance()->pathResolver() );
  QVariantMap metadata = QgsExifTools::readTags( imagePath );
  QImage img( imagePath );
  if ( !img.isNull() )
  {
    QPainter painter( &img );
    painter.setRenderHint( QPainter::Antialiasing );
    QgsRenderContext context = QgsRenderContext::fromQPainter( &painter );
    QgsTextFormat format;
    if ( !textFormat.isEmpty() )
    {
      QDomDocument document = QDomDocument();
      document.setContent( textFormat );
      format.readXml( document.documentElement(), readWriteContent );
    }
    else
    {
      QFont font = painter.font();
      font.setPixelSize( std::min( img.width(), img.height() ) / 40 );
      font.setBold( true );
      format.setFont( font );
      format.setSize( font.pixelSize() );
      format.setSizeUnit( Qgis::RenderUnit::Pixels );
      format.setColor( Qt::white );
      format.buffer().setColor( Qt::black );
      format.buffer().setSize( 2 );
      format.buffer().setSizeUnit( Qgis::RenderUnit::Pixels );
      format.buffer().setEnabled( true );
    }

    if ( format.sizeUnit() == Qgis::RenderUnit::Percentage )
    {
      format.setSize( std::min( img.width(), img.height() ) * format.size() / 100 );
      format.setSizeUnit( Qgis::RenderUnit::Pixels );
    }

    if ( !imageDecoration.isEmpty() )
    {
      const QFileInfo fi( QgsProject::instance()->pathResolver().readPath( imageDecoration ) );
      if ( fi.exists() )
      {
        const double lineHeight = context.convertFromPainterUnits( format.size(), format.sizeUnit() );
        const bool isRaster = fi.suffix().toLower() != QStringLiteral( "svg" );

        bool fitsInCache = false;
        if ( isRaster )
        {
          const QSizeF decorationOriginalSize = QgsApplication::instance()->imageCache()->originalSize( fi.absoluteFilePath(), true );
          const double decorationHeight = std::min( std::min( decorationOriginalSize.height(), img.height() / 6.0 ), lineHeight * 10.0 );
          const double decorationWidth = decorationOriginalSize.width() * decorationHeight / decorationOriginalSize.height();
          QImage decorationImage = QgsApplication::instance()->imageCache()->pathAsImage( fi.absoluteFilePath(), QSize( decorationWidth, decorationHeight ), true, 1, fitsInCache, true );
          switch ( horizontalAlignment )
          {
            case Qgis::TextHorizontalAlignment::Left:
            case Qgis::TextHorizontalAlignment::Justify:
              painter.drawImage( QRectF( 10, 10, decorationWidth, decorationHeight ), decorationImage );
              break;
            case Qgis::TextHorizontalAlignment::Center:
              painter.drawImage( QRectF( img.size().width() / 2 - decorationWidth / 2, 10, decorationWidth, decorationHeight ), decorationImage );
              break;
            case Qgis::TextHorizontalAlignment::Right:
              painter.drawImage( QRectF( img.size().width() - decorationWidth - 10, 10, decorationWidth, decorationHeight ), decorationImage );
              break;
          }
        }
        else
        {
          const QSizeF decorationOriginalSize = QgsApplication::instance()->svgCache()->svgViewboxSize( fi.absoluteFilePath(), 100, QColor( 0, 0, 0 ), QColor( 255, 0, 0 ), 10, 1, 0, true );
          const double decorationHeight = std::min( img.height() / 6.0, lineHeight * 10.0 );
          const double decorationWidth = decorationOriginalSize.width() * decorationHeight / decorationOriginalSize.height();
          QPicture decorationPicture = QgsApplication::instance()->svgCache()->svgAsPicture( fi.absoluteFilePath(), decorationWidth, QColor(), QColor(), 1.0, 1, fitsInCache, 0, true );
          switch ( horizontalAlignment )
          {
            case Qgis::TextHorizontalAlignment::Left:
            case Qgis::TextHorizontalAlignment::Justify:
              painter.drawPicture( 10 + decorationWidth / 2, 10 + decorationHeight / 2, decorationPicture );
              break;
            case Qgis::TextHorizontalAlignment::Center:
              painter.drawPicture( img.size().width() / 2, 10 + decorationHeight / 2, decorationPicture );
              break;
            case Qgis::TextHorizontalAlignment::Right:
              painter.drawPicture( img.size().width() - 10 - decorationWidth / 2, 10 + decorationHeight / 2, decorationPicture );
              break;
          }
        }
      }
    }

    const double textHeight = QgsTextRenderer::textHeight( context, format, text.split( QStringLiteral( "\n" ) ), Qgis::TextLayoutMode::Rectangle, nullptr, Qgis::TextRendererFlag::WrapLines, img.width() - 20 );
    QgsTextRenderer::drawText( QRectF( 10, img.height() - textHeight - 20, img.width() - 20, img.height() - 20 ), 0, horizontalAlignment, text.split( QStringLiteral( "\n" ) ), context, format, true, Qgis::TextVerticalAlignment::Top, Qgis::TextRendererFlag::WrapLines );

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
      //cppcheck-suppress ignoredReturnValue
      ( void ) rebuiltDir.cd( segment );
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

QVariantMap FileUtils::getFileInfo( const QString &filePath, bool fetchContent )
{
  QVariantMap info;

  // Initialize all possible keys with empty defaults
  info["exists"] = false;
  info["error"] = "";
  info["fileName"] = "";
  info["filePath"] = "";
  info["fileSize"] = 0;
  info["lastModified"] = QDateTime();
  info["suffix"] = "";
  info["mimeType"] = "";
  info["md5"] = "";
  info["md5Error"] = "";
  info["content"] = QByteArray();
  info["readable"] = false;
  info["readError"] = "";

  if ( !isWithinProjectDirectory( filePath ) )
  {
    qWarning() << QStringLiteral( "Security warning: Attempted to access file info outside project directory: %1" ).arg( filePath );
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
      info["readable"] = true;

      if ( fetchContent )
      {
        QByteArray content = readFileContent( filePath );

        if ( !content.isEmpty() || fileInfo.size() == 0 )
        {
          info["content"] = content;
        }
        else
        {
          info["readError"] = QStringLiteral( "Failed to read file content" );
        }
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

  return info;
}

bool FileUtils::unzip( const QString &zipFilename, const QString &dir, QStringList &files, bool checkConsistency )
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
    const long long count = zip_get_num_entries( z, ZIP_FL_UNCHANGED );
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
          if ( fileName.endsWith( "/" ) )
          {
            continue;
          }

          const QFileInfo newFile( QDir( dir ), fileName );

          if ( !QString( QDir::cleanPath( newFile.absolutePath() ) + QStringLiteral( "/" ) ).startsWith( QDir( dir ).absolutePath() + QStringLiteral( "/" ) ) )
          {
            QgsMessageLog::logMessage( QObject::tr( "Skipped file %1 outside of the directory %2" ).arg( newFile.absoluteFilePath(), QDir( dir ).absolutePath() ) );
            continue;
          }

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

bool FileUtils::isDeletable( const QString &filePath )
{
  const QFileInfo fileInfo( filePath );
  if ( !fileInfo.exists() || !fileInfo.isFile() )
  {
    return false;
  }

  const QString canonicalFilePath = fileInfo.canonicalFilePath();

  // Collect all allowed directories
  QStringList allowedDirectories;

  const QString appDataDir = QFileInfo( PlatformUtilities::instance()->applicationDirectory() ).canonicalFilePath();
  if ( !appDataDir.isEmpty() )
  {
    allowedDirectories << appDataDir;
  }

  const QString cloudDataDir = QFileInfo( QFieldCloudUtils::localCloudDirectory() ).canonicalFilePath();
  if ( !cloudDataDir.isEmpty() )
  {
    allowedDirectories << cloudDataDir;
  }

  const QStringList extraDirs = PlatformUtilities::instance()->additionalApplicationDirectories();
  for ( const QString &dir : extraDirs )
  {
    if ( !dir.isEmpty() )
    {
      const QString canonicalDir = QFileInfo( dir ).canonicalFilePath();
      if ( !canonicalDir.isEmpty() )
      {
        allowedDirectories << canonicalDir;
      }
    }
  }

  // Check if file is inside any of the allowed directories
  const bool isInsideAllowedDir = std::any_of( allowedDirectories.begin(), allowedDirectories.end(), [&canonicalFilePath]( const QString &dir ) {
    return canonicalFilePath.startsWith( dir );
  } );

  if ( !isInsideAllowedDir )
  {
    return false;
  }

  const QString suffix = fileInfo.suffix().toLower();

  static const QStringList allowedExtensions = { "pdf", "png", "jpg", "jpeg", "mp4", "mp4a", "mp3" };

  return allowedExtensions.contains( suffix );
}

QVariantMap FileUtils::deleteFiles( const QStringList &filePaths )
{
  QVariantMap results;

  for ( const QString &filePath : filePaths )
  {
    if ( !isDeletable( filePath ) )
    {
      qWarning() << QStringLiteral( "Cannot delete file (not allowed): %1" ).arg( filePath );
      results[filePath] = false;
      continue;
    }

    const QFileInfo fileInfo( filePath );
    const QString canonicalPath = fileInfo.canonicalFilePath();
    QFile file( canonicalPath );

    if ( !file.exists() )
    {
      qWarning() << QStringLiteral( "File does not exist: %1" ).arg( filePath );
      results[filePath] = false;
      continue;
    }

    const bool success = file.remove();
    if ( success )
    {
      qDebug() << QStringLiteral( "Successfully deleted file: %1" ).arg( filePath );
    }
    else
    {
      qWarning() << QStringLiteral( "Failed to delete file: %1 - %2" ).arg( filePath, file.errorString() );
    }

    results[filePath] = success;
  }

  return results;
}
```


