

# File qfieldcloudutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfieldcloudutils.cpp**](qfieldcloudutils_8cpp.md)

[Go to the documentation of this file](qfieldcloudutils_8cpp.md)


```C++
/***************************************************************************
    qfieldcloudutils.cpp
    ---------------------
    begin                : February 2020
    copyright            : (C) 2020 by Mathieu Pellerin
    email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "fileutils.h"
#include "platformutilities.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudutils.h"
#include "stringutils.h"

#include <QDir>
#include <QFile>
#include <QLockFile>
#include <QStandardPaths>
#include <QString>
#include <qgsapplication.h>
#include <qgsmessagelog.h>

static QString sLocalCloudDirectory;


void QFieldCloudUtils::setLocalCloudDirectory( const QString &path )
{
  sLocalCloudDirectory = path;
}

const QString QFieldCloudUtils::localCloudDirectory()
{
  QString cloudDirectoryPath = sLocalCloudDirectory.isNull()
                                 ? PlatformUtilities::instance()->systemLocalDataLocation( QStringLiteral( "cloud_projects" ) )
                                 : sLocalCloudDirectory;
  // Remove trailing '/' or '\' if present
  while ( !cloudDirectoryPath.isEmpty() && ( cloudDirectoryPath.endsWith( '/' ) || cloudDirectoryPath.endsWith( '\\' ) ) )
  {
    cloudDirectoryPath.chop( 1 );
  }
  return cloudDirectoryPath;
}

const QString QFieldCloudUtils::localProjectFilePath( const QString &username, const QString &projectId )
{
  QString project = QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, projectId );
  QDir projectDir( project );
  QStringList projectFiles = projectDir.entryList( QStringList() << QStringLiteral( "*.qgz" ) << QStringLiteral( "*.qgs" ) );
  if ( projectFiles.count() > 0 )
  {
    return QStringLiteral( "%1/%2" ).arg( project, projectFiles.at( 0 ) );
  }
  return QString();
}

bool QFieldCloudUtils::isCloudAction( const QgsMapLayer *layer )
{
  Q_ASSERT( layer );

  const QString layerAction( layer->customProperty( QStringLiteral( "QFieldSync/cloud_action" ) ).toString().toUpper() );

  if ( layerAction == QStringLiteral( "NO_ACTION" ) || layerAction == QStringLiteral( "REMOVE" ) )
    return false;
  return true;
}

const QString QFieldCloudUtils::getProjectId( const QString &fileName )
{
  if ( fileName.isEmpty() )
    return QString();

  const QString path = QFileInfo( fileName ).canonicalFilePath();
  if ( path.isEmpty() )
    return QString();

  const QString cloudPath = QFileInfo( QFieldCloudUtils::localCloudDirectory() ).canonicalFilePath();
  if ( cloudPath.isEmpty() || !path.startsWith( cloudPath ) )
    return QString();

  const QRegularExpression re(
    QStringLiteral( "^%1[/\\\\][^/\\\\]+[/\\\\]([^/\\\\]+)" )
      .arg( QRegularExpression::escape( cloudPath ) ) );
  const QRegularExpressionMatch match = re.match( path );

  if ( match.hasMatch() )
  {
    return match.captured( 1 );
  }

  return QString();
}

QString QFieldCloudUtils::userFriendlyErrorString( const QString &errorString )
{
  QString resultErrorString = errorString.startsWith( "[QF/" ) ? tr( "A server error has occured, please try again." ) : tr( "A network error has occured, please try again." );

  if ( errorString.contains( errorCodeOverQuota ) )
  {
    resultErrorString = tr( "Your account's available storage is full." );
  }

  return resultErrorString;
}

QString QFieldCloudUtils::documentationFromErrorString( const QString &errorString )
{
  QString linkToDocumentation;

  if ( errorString.contains( errorCodeOverQuota ) )
  {
    linkToDocumentation = "https://docs.qfield.org/get-started/storage-qfc/#adding-qfieldcloud-storage";
  }

  return linkToDocumentation;
}

void QFieldCloudUtils::setProjectSetting( const QString &projectId, const QString &setting, const QVariant &value )
{
  thread_local QgsSettings settings;
  const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId );
  settings.setValue( QStringLiteral( "%1/%2" ).arg( projectPrefix, setting ), value );
}

const QVariant QFieldCloudUtils::projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue )
{
  thread_local QgsSettings settings;
  const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId );
  return settings.value( QStringLiteral( "%1/%2" ).arg( projectPrefix, setting ), defaultValue );
}

bool QFieldCloudUtils::hasPendingAttachments( const QString &username )
{
  return !QFieldCloudUtils::getPendingAttachments( username ).isEmpty();
}

const QMultiMap<QString, QString> QFieldCloudUtils::getPendingAttachments( const QString &username )
{
  // Migration for QField < 3.6
  if ( QFileInfo::exists( QStringLiteral( "%1/attachments.csv" ).arg( QFieldCloudUtils::localCloudDirectory() ) ) )
  {
    // Step 1: Load the already existing legacy `attachments.csv` file contents in the memory.
    QMultiMap<QString, QString> migrationFiles;
    QFile migrationFile( QStringLiteral( "%1/attachments.csv" ).arg( QFieldCloudUtils::localCloudDirectory() ) );
    migrationFile.open( QFile::ReadWrite | QFile::Text );
    QTextStream migrationStream( &migrationFile );
    while ( !migrationStream.atEnd() )
    {
      const QString line = migrationStream.readLine().trimmed();
      const QStringList values = StringUtils::csvToStringList( line );
      if ( values.size() >= 2 )
      {
        migrationFiles.insert( values.at( 0 ), values.at( 1 ) );
      }
    }

    // Step 2: Group the attachments list by username, which is extracted from the path of the queued files to upload.
    QMap<QString, QMultiMap<QString, QString>> migratedAttachmentDetails;
    // Extract the username by capturing the child folder name to the parent local cloud directory using / or \ as folder separators
    QRegularExpression re( QStringLiteral( "%1[\\/\\\\]([^\\/\\\\]+)[\\/\\\\]" ).arg( QRegularExpression::escape( QFieldCloudUtils::localCloudDirectory() ) ) );
    const QStringList projectIds = migrationFiles.uniqueKeys();
    for ( const QString &projectId : projectIds )
    {
      const QStringList files = migrationFiles.values( projectId );
      for ( const QString &file : files )
      {
        QRegularExpressionMatch match = re.match( file );
        if ( match.hasMatch() )
        {
          migratedAttachmentDetails[match.captured( 1 )].insert( projectId, file );
        }
      }
    }

    // Step 3: Create an `attachments.csv` file for each username.
    const QStringList migratedUsernames = migratedAttachmentDetails.keys();
    for ( const QString &migratedUsername : migratedUsernames )
    {
      const QStringList migratedProjectIds = migratedAttachmentDetails[migratedUsername].uniqueKeys();
      for ( const QString &migratedProjectId : migratedProjectIds )
      {
        // Play safe, create the user folder
        QDir().mkpath( QStringLiteral( "%1/%2" ).arg( QFieldCloudUtils::localCloudDirectory(), migratedUsername ) );
        addPendingAttachments( migratedUsername, migratedProjectId, migratedAttachmentDetails[migratedUsername].values( migratedProjectId ) );
      }
    }

    migrationFile.close();
    migrationFile.remove();
  }

  QMultiMap<QString, QString> files;
  if ( username.isEmpty() )
  {
    return files;
  }

  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QFieldCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    QFileInfo fi( attachmentsFile );
    if ( !fi.exists() || fi.size() == 0 )
    {
      return files;
    }

    attachmentsFile.open( QFile::ReadWrite | QFile::Text );
    QTextStream attachmentsStream( &attachmentsFile );
    while ( !attachmentsStream.atEnd() )
    {
      const QString line = attachmentsStream.readLine().trimmed();
      const QStringList values = StringUtils::csvToStringList( line );

      // The expected CSV format must have two columns:
      // project_id,file_path
      if ( values.size() >= 2 )
      {
        files.insert( values.at( 0 ), values.at( 1 ) );
      }
    }
  }

  return files;
}

void QFieldCloudUtils::addPendingAttachments( const QString &username, const QString &projectId, const QStringList &fileNames, QFieldCloudConnection *cloudConnection, const bool &checkSumCheck )
{
  if ( username.isEmpty() || projectId.isEmpty() )
  {
    Q_ASSERT( false );
    return;
  }

  if ( !QFileInfo::exists( QStringLiteral( "%1/%2" ).arg( QFieldCloudUtils::localCloudDirectory(), username ) ) )
  {
    Q_ASSERT( false );
    return;
  }

  if ( checkSumCheck && cloudConnection )
  {
    QVariantMap params;
    params.insert( "skip_metadata", 1 );
    NetworkReply *reply = cloudConnection->get( QStringLiteral( "/api/v1/files/%1/" ).arg( projectId ), params );

    connect( reply, &NetworkReply::finished, reply, [reply, username, projectId, fileNames, checkSumCheck, cloudConnection]() {
      QNetworkReply *rawReply = reply->currentRawReply();
      reply->deleteLater();

      if ( rawReply->error() != QNetworkReply::NoError )
      {
        QgsLogger::debug( QStringLiteral( "Project %1: failed to retrieve file information. %2" ).arg( projectId, QFieldCloudConnection::errorString( rawReply ) ) );
        return;
      }

      const QJsonArray files = QJsonDocument::fromJson( rawReply->readAll() ).array();
      QHash<QString, QString> fileChecksumMap;

      for ( const QJsonValueConstRef &fileValue : files )
      {
        const QJsonObject fileObject = fileValue.toObject();
        const QString cloudEtag = fileObject.value( QStringLiteral( "md5sum" ) ).toString();
        const QString fileName = fileObject.value( QStringLiteral( "name" ) ).toString();
        fileChecksumMap.insert( fileName, cloudEtag );
      }

      writeToAttachmentsFile( username, projectId, fileNames, &fileChecksumMap, checkSumCheck, cloudConnection );
    } );
  }
  else
  {
    writeToAttachmentsFile( username, projectId, fileNames, nullptr, false, cloudConnection );
  }
}

void QFieldCloudUtils::writeToAttachmentsFile( const QString &username, const QString &projectId, const QStringList &fileNames, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QFieldCloudConnection *cloudConnection )
{
  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QFieldCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    attachmentsFile.open( QFile::Append | QFile::Text );
    QTextStream attachmentsStream( &attachmentsFile );

    for ( const QString &fileName : fileNames )
    {
      QFileInfo fi( QDir::cleanPath( fileName ) );
      if ( fi.isDir() )
      {
        writeFilesFromDirectory( fileName, projectId, fileChecksumMap, checkSumCheck, attachmentsStream );
      }
      else if ( fi.isFile() )
      {
        writeFileDetails( fileName, projectId, fileChecksumMap, checkSumCheck, attachmentsStream );
      }
    }
    attachmentsFile.close();

    if ( cloudConnection )
      emit cloudConnection->pendingAttachmentsAdded();
  }
}

void QFieldCloudUtils::writeFilesFromDirectory( const QString &dirPath, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream )
{
  QDir dir( dirPath );
  if ( !dir.exists() )
  {
    return;
  }

  QFileInfoList entries = dir.entryInfoList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );

  for ( const QFileInfo &entry : entries )
  {
    if ( entry.isDir() )
    {
      writeFilesFromDirectory( entry.absoluteFilePath(), projectId, fileChecksumMap, checkSumCheck, attachmentsStream );
    }
    else if ( entry.isFile() )
    {
      writeFileDetails( entry.absoluteFilePath(), projectId, fileChecksumMap, checkSumCheck, attachmentsStream );
    }
  }
}

void QFieldCloudUtils::writeFileDetails( const QString &fileName, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream )
{
  const QString localEtag = FileUtils::fileEtag( fileName );
  QString cloudFileName = "";
  const QStringList fileNameParts = fileName.split( projectId + "/" );
  if ( fileNameParts.size() > 1 )
  {
    cloudFileName = fileNameParts[1];
  }

  if ( !checkSumCheck || localEtag != fileChecksumMap->value( cloudFileName ) )
  {
    QStringList values { projectId, fileName };
    attachmentsStream << StringUtils::stringListToCsv( values ) << Qt::endl;
  }
}

void QFieldCloudUtils::removePendingAttachment( const QString &username, const QString &projectId, const QString &fileName )
{
  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QFieldCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    const QString lineToRemove = StringUtils::stringListToCsv( QStringList() << projectId << fileName );
    QString output;
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    attachmentsFile.open( QFile::ReadWrite | QFile::Text );
    QTextStream attachmentsStream( &attachmentsFile );
    while ( !attachmentsStream.atEnd() )
    {
      const QString line = attachmentsStream.readLine();
      if ( !line.isEmpty() && !line.startsWith( lineToRemove ) )
      {
        output += line + QChar( '\n' );
      }
    }
    attachmentsFile.resize( 0 );
    attachmentsStream.reset();
    attachmentsStream << output;
    attachmentsFile.close();
  }
}
```


