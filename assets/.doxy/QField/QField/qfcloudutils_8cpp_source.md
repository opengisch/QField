

# File qfcloudutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfcloudutils.cpp**](qfcloudutils_8cpp.md)

[Go to the documentation of this file](qfcloudutils_8cpp.md)


```C++
/***************************************************************************
    qfcloudutils.cpp
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

#include "qfcloudconnection.h"
#include "qfcloudutils.h"
#include "qffileutils.h"
#include "qfplatformutilities.h"
#include "qfstringutils.h"

#include <QDir>
#include <QFile>
#include <QLockFile>
#include <QStandardPaths>
#include <QString>
#include <qgsapplication.h>
#include <qgsmessagelog.h>

static QString sLocalCloudDirectory;


void QfCloudUtils::setLocalCloudDirectory( const QString &path )
{
  sLocalCloudDirectory = path;
}

const QString QfCloudUtils::localCloudDirectory()
{
  QString cloudDirectoryPath = sLocalCloudDirectory.isNull()
                                 ? QfPlatformUtilities::instance()->systemLocalDataLocation( QStringLiteral( "cloud_projects" ) )
                                 : sLocalCloudDirectory;
  // Remove trailing '/' or '\' if present
  while ( !cloudDirectoryPath.isEmpty() && ( cloudDirectoryPath.endsWith( '/' ) || cloudDirectoryPath.endsWith( '\\' ) ) )
  {
    cloudDirectoryPath.chop( 1 );
  }
  return cloudDirectoryPath;
}

const QString QfCloudUtils::localProjectFilePath( const QString &username, const QString &projectId )
{
  QString project = QStringLiteral( "%1/%2/%3" ).arg( QfCloudUtils::localCloudDirectory(), username, projectId );
  QDir projectDir( project );
  QStringList projectFiles = projectDir.entryList( QStringList() << QStringLiteral( "*.qgz" ) << QStringLiteral( "*.qgs" ) );
  if ( projectFiles.count() > 0 )
  {
    return QStringLiteral( "%1/%2" ).arg( project, projectFiles.at( 0 ) );
  }
  return QString();
}

bool QfCloudUtils::isCloudAction( const QgsMapLayer *layer )
{
  Q_ASSERT( layer );

  const QString layerAction( layer->customProperty( QStringLiteral( "QFieldSync/cloud_action" ) ).toString().toUpper() );

  if ( layerAction == QStringLiteral( "NO_ACTION" ) || layerAction == QStringLiteral( "REMOVE" ) )
    return false;
  return true;
}

const QString QfCloudUtils::getProjectId( const QString &fileName )
{
  if ( fileName.isEmpty() )
    return QString();

  const QString path = QFileInfo( fileName ).canonicalFilePath();
  if ( path.isEmpty() )
    return QString();

  const QString cloudPath = QFileInfo( QfCloudUtils::localCloudDirectory() ).canonicalFilePath();
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

QString QfCloudUtils::userFriendlyErrorString( const QString &errorString )
{
  QString resultErrorString = errorString.startsWith( "[QF/" ) ? tr( "A server error has occured, please try again." ) : tr( "A network error has occured, please try again." );

  if ( errorString.contains( errorCodeOverQuota() ) )
  {
    resultErrorString = tr( "The project owner's available storage is full." );
  }
  else if ( errorString.contains( errorCodePlanInsufficient() ) )
  {
    resultErrorString = tr( "The project owner's subscription plan is insufficient." );
  }

  return resultErrorString;
}

QString QfCloudUtils::documentationFromErrorString( const QString &errorString )
{
  if ( errorString.contains( errorCodeOverQuota() ) )
  {
    return QStringLiteral( "https://docs.qfield.org/get-started/storage-qfc/#adding-qfieldcloud-storage" );
  }

  return QString();
}

void QfCloudUtils::setProjectSetting( const QString &projectId, const QString &setting, const QVariant &value )
{
  thread_local QgsSettings settings;
  const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId );
  settings.setValue( QStringLiteral( "%1/%2" ).arg( projectPrefix, setting ), value );
}

const QVariant QfCloudUtils::projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue )
{
  thread_local QgsSettings settings;
  const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId );
  return settings.value( QStringLiteral( "%1/%2" ).arg( projectPrefix, setting ), defaultValue );
}

bool QfCloudUtils::hasPendingAttachments( const QString &username )
{
  return !QfCloudUtils::getPendingAttachments( username ).isEmpty();
}

const QMultiMap<QString, QString> QfCloudUtils::getPendingAttachments( const QString &username )
{
  // Migration for QField < 3.6
  if ( QFileInfo::exists( QStringLiteral( "%1/attachments.csv" ).arg( QfCloudUtils::localCloudDirectory() ) ) )
  {
    // Step 1: Load the already existing legacy `attachments.csv` file contents in the memory.
    QMultiMap<QString, QString> migrationFiles;
    QFile migrationFile( QStringLiteral( "%1/attachments.csv" ).arg( QfCloudUtils::localCloudDirectory() ) );
    if ( migrationFile.open( QFile::ReadWrite | QFile::Text ) )
    {
      QTextStream migrationStream( &migrationFile );
      while ( !migrationStream.atEnd() )
      {
        const QString line = migrationStream.readLine().trimmed();
        const QStringList values = QfStringUtils::csvToStringList( line );
        if ( values.size() >= 2 )
        {
          migrationFiles.insert( values.at( 0 ), values.at( 1 ) );
        }
      }
    }

    // Step 2: Group the attachments list by username, which is extracted from the path of the queued files to upload.
    QMap<QString, QMultiMap<QString, QString>> migratedAttachmentDetails;
    // Extract the username by capturing the child folder name to the parent local cloud directory using / or \ as folder separators
    QRegularExpression re( QStringLiteral( "%1[\\/\\\\]([^\\/\\\\]+)[\\/\\\\]" ).arg( QRegularExpression::escape( QfCloudUtils::localCloudDirectory() ) ) );
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
        QDir().mkpath( QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), migratedUsername ) );
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

  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QfCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    QFileInfo fi( attachmentsFile );
    if ( !fi.exists() || fi.size() == 0 )
    {
      return files;
    }

    if ( attachmentsFile.open( QFile::ReadWrite | QFile::Text ) )
    {
      QTextStream attachmentsStream( &attachmentsFile );
      while ( !attachmentsStream.atEnd() )
      {
        const QString line = attachmentsStream.readLine().trimmed();
        const QStringList values = QfStringUtils::csvToStringList( line );

        // The expected CSV format must have two columns:
        // project_id,file_path
        if ( values.size() >= 2 )
        {
          files.insert( values.at( 0 ), values.at( 1 ) );
        }
      }
    }
  }

  return files;
}

void QfCloudUtils::addPendingAttachments( const QString &username, const QString &projectId, const QStringList &fileNames, QfCloudConnection *cloudConnection, const bool &checkSumCheck )
{
  if ( username.isEmpty() || projectId.isEmpty() )
  {
    Q_ASSERT( false );
    return;
  }

  if ( !QFileInfo::exists( QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), username ) ) )
  {
    Q_ASSERT( false );
    return;
  }

  if ( checkSumCheck && cloudConnection )
  {
    QVariantMap params;
    params.insert( "skip_metadata", 1 );
    QfNetworkReply *reply = cloudConnection->get( QStringLiteral( "/api/v1/files/%1/" ).arg( projectId ), params );

    connect( reply, &QfNetworkReply::finished, reply, [reply, username, projectId, fileNames, checkSumCheck, cloudConnection]() {
      QNetworkReply *rawReply = reply->currentRawReply();
      reply->deleteLater();

      if ( rawReply->error() != QNetworkReply::NoError )
      {
        QgsLogger::debug( QStringLiteral( "Project %1: failed to retrieve file information. %2" ).arg( projectId, QfCloudConnection::errorString( rawReply ) ) );
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

void QfCloudUtils::writeToAttachmentsFile( const QString &username, const QString &projectId, const QStringList &fileNames, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QfCloudConnection *cloudConnection )
{
  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QfCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    if ( attachmentsFile.open( QFile::Append | QFile::Text ) )
    {
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
    }

    if ( cloudConnection )
      emit cloudConnection->pendingAttachmentsAdded();
  }
}

void QfCloudUtils::writeFilesFromDirectory( const QString &dirPath, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream )
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

void QfCloudUtils::writeFileDetails( const QString &fileName, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream )
{
  const QString localEtag = QfFileUtils::fileEtag( fileName );
  QString cloudFileName = "";
  const QStringList fileNameParts = fileName.split( projectId + "/" );
  if ( fileNameParts.size() > 1 )
  {
    cloudFileName = fileNameParts[1];
  }

  if ( !checkSumCheck || localEtag != fileChecksumMap->value( cloudFileName ) )
  {
    QStringList values { projectId, fileName };
    attachmentsStream << QfStringUtils::stringListToCsv( values ) << Qt::endl;
  }
}

void QfCloudUtils::removePendingAttachment( const QString &username, const QString &projectId, const QString &fileName )
{
  const QString localCloudUSerDirectory = QLatin1String( "%1/%2/" ).arg( QfCloudUtils::localCloudDirectory(), username );
  QLockFile attachmentsLock( QStringLiteral( "%1/attachments.lock" ).arg( localCloudUSerDirectory ) );
  if ( attachmentsLock.tryLock( 10000 ) )
  {
    const QString lineToRemove = QfStringUtils::stringListToCsv( QStringList() << projectId << fileName );
    QString output;
    QFile attachmentsFile( QStringLiteral( "%1/attachments.csv" ).arg( localCloudUSerDirectory ) );
    if ( attachmentsFile.open( QFile::ReadWrite | QFile::Text ) )
    {
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
}

QString QfCloudUtils::subscriptionManagementUrl( const QString &serverUrl, const QString &plan, const QString &projectOwner, const QString &username )
{
  if ( serverUrl != QfCloudConnection::defaultUrl() )
  {
    return QString();
  }

  // TODO: change to plan_code once opengisch/QFieldCloud#1539 merged
  if ( plan.compare( QStringLiteral( "Community" ), Qt::CaseInsensitive ) == 0 )
  {
    return QStringLiteral( "https://app.qfield.cloud/plans" );
  }

  if ( projectOwner.isEmpty() || projectOwner == username )
  {
    return QStringLiteral( "https://app.qfield.cloud/settings/%1/billing" ).arg( username );
  }

  return QString();
}

QList<QfCloudDelta> QfCloudUtils::parseDeltaJsonDocument( const QJsonDocument &jsonDocument, QString &errorString, bool &isValid )
{
  QList<QfCloudDelta> deltas;

  if ( !jsonDocument.isArray() )
  {
    errorString = tr( "Expected the json document to be an array of delta status" );
    isValid = false;
    return deltas;
  }

  const QJsonArray deltasArray = jsonDocument.array();
  for ( const QJsonValue deltaValue : deltasArray )
  {
    if ( !deltaValue.isObject() )
    {
      errorString = tr( "Expected all array elements to be an object, but the element at #%1 is not" ).arg( deltas.size() );
      isValid = false;
      break;
    }

    const QJsonObject deltaObject = deltaValue.toObject();
    const QStringList requiredKeys( { "id", "deltafile_id", "created_at", "updated_at", "status" } );
    auto match = std::find_if( requiredKeys.begin(), requiredKeys.end(), [&deltaObject]( const QString &key ) {
      return deltaObject.value( key ).isNull() || deltaObject.value( key ).isUndefined();
    } );
    if ( match != requiredKeys.end() )
    {
      errorString = tr( "Expected all array elements to be an object containing a key \"%1\", but the element at #%2 is not" ).arg( *match ).arg( deltas.size() );
      isValid = false;
      break;
    }

    QfCloudDelta delta;
    delta.output = deltaObject.value( QStringLiteral( "output" ) ).toString();

    QList<QJsonObject> deltaObjects;
    deltaObjects << deltaObject.value( "content" ).toObject();
    delta.summary = QfCloudUtils::summarizeDeltaContent( deltaObjects );

    const QString statusString = deltaObject.value( QStringLiteral( "status" ) ).toString();
    if ( statusString == QStringLiteral( "STATUS_APPLIED" ) )
      delta.status = QfCloudDelta::AppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_CONFLICT" ) )
      delta.status = QfCloudDelta::ConflictStatus;
    else if ( statusString == QStringLiteral( "STATUS_NOT_APPLIED" ) )
      delta.status = QfCloudDelta::NotAppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_PENDING" ) )
      delta.status = QfCloudDelta::PendingStatus;
    else if ( statusString == QStringLiteral( "STATUS_BUSY" ) )
      delta.status = QfCloudDelta::BusyStatus;
    else if ( statusString == QStringLiteral( "STATUS_ERROR" ) )
      delta.status = QfCloudDelta::ErrorStatus;
    else if ( statusString == QStringLiteral( "STATUS_IGNORED" ) )
      delta.status = QfCloudDelta::IgnoredStatus;
    else if ( statusString == QStringLiteral( "STATUS_UNPERMITTED" ) )
      delta.status = QfCloudDelta::UnpermittedStatus;
    else
    {
      errorString = tr( "Unrecognized status \"%1\" for $%2" ).arg( statusString, QString::number( deltas.size() ) );
      isValid = false;
      break;
    }

    delta.id = QUuid( deltaObject.value( QStringLiteral( "id" ) ).toString() );
    delta.deltafileId = QUuid( deltaObject.value( QStringLiteral( "deltafile_id" ) ).toString() );
    delta.createdBy = deltaObject.value( QStringLiteral( "created_by" ) ).toString();
    delta.createdAt = QDateTime::fromString( deltaObject.value( QStringLiteral( "created_at" ) ).toString(), Qt::ISODate );
    delta.updatedAt = QDateTime::fromString( deltaObject.value( QStringLiteral( "updated_at" ) ).toString(), Qt::ISODate );

    isValid = true;
    deltas.append( delta );
  }

  std::sort( deltas.begin(), deltas.end(), []( const QfCloudDelta &a, const QfCloudDelta &b ) { return a.createdAt > b.createdAt; } );

  return deltas;
}

QString QfCloudUtils::summarizeDeltaContent( const QList<QJsonObject> &deltaObjects, const QString &modificationSeparator, const QString &layerSeparator )
{
  QMap<QString, int> createdFeatures;
  QMap<QString, int> patchedFeatures;
  QMap<QString, int> deletedFeatures;

  for ( const QJsonObject &deltaObject : deltaObjects )
  {
    const QString method = deltaObject.value( QStringLiteral( "method" ) ).toString();
    const QString layerName = deltaObject.value( QStringLiteral( "localLayerName" ) ).toString();

    if ( !layerName.isEmpty() )
    {
      if ( method == QStringLiteral( "create" ) )
      {
        if ( createdFeatures.contains( layerName ) )
        {
          createdFeatures[layerName]++;
        }
        else
        {
          createdFeatures[layerName] = 1;
        }
      }
      else if ( method == QStringLiteral( "patch" ) )
      {
        if ( patchedFeatures.contains( layerName ) )
        {
          patchedFeatures[layerName]++;
        }
        else
        {
          patchedFeatures[layerName] = 1;
        }
      }
      else if ( method == QStringLiteral( "delete" ) )
      {
        if ( deletedFeatures.contains( layerName ) )
        {
          deletedFeatures[layerName]++;
        }
        else
        {
          deletedFeatures[layerName] = 1;
        }
      }
    }
  }

  QStringList modifiedLayers = createdFeatures.keys() + patchedFeatures.keys() + deletedFeatures.keys();
  modifiedLayers.removeDuplicates();

  QStringList summary;
  for ( const QString &modifiedLayer : modifiedLayers )
  {
    QStringList modifications;
    if ( createdFeatures.contains( modifiedLayer ) )
    {
      modifications << tr( "%n feature(s) created", "", createdFeatures[modifiedLayer] );
    }
    if ( patchedFeatures.contains( modifiedLayer ) )
    {
      modifications << tr( "%n feature(s) edited", "", patchedFeatures[modifiedLayer] );
    }
    if ( deletedFeatures.contains( modifiedLayer ) )
    {
      modifications << tr( "%n feature(s) deleted", "", deletedFeatures[modifiedLayer] );
    }
    summary << tr( "%1 in layer %2" ).arg( modifications.join( modificationSeparator ), modifiedLayer );
  }

  return summary.join( layerSeparator );
}
```


