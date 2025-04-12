/***************************************************************************
    qfieldcloudconnection.cpp
    ---------------------
    begin                : April 2025
    copyright            : (C) 2025 by Mathieu Pellerin
    email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "appinterface.h"
#include "deltafilewrapper.h"
#include "deltalistmodel.h"
#include "fileutils.h"
#include "layerobserver.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudproject.h"
#include "qfieldcloudutils.h"

#include <QDirIterator>
#include <QFileInfo>
#include <qgsmessagelog.h>

#define MAX_REDIRECTS_ALLOWED 10
#define MAX_PARALLEL_REQUESTS 6
#define CACHE_PROJECT_DATA_SECS 1

QFieldCloudProject::QFieldCloudProject( const QString &id, QFieldCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher )
  : mId( id ), mCloudConnection( connection ), mGpkgFlusher( gpkgFlusher )
{
  if ( mCloudConnection )
  {
    mUsername = mCloudConnection->username();
  }
}

QFieldCloudProject::~QFieldCloudProject()
{
  delete mDeltaListModel;
}

void QFieldCloudProject::setForceAutoPush( bool force )
{
  if ( mForceAutoPush == force )
    return;

  mForceAutoPush = force;
  emit forceAutoPushChanged();
}

void QFieldCloudProject::setAutoPushEnabled( bool enabled )
{
  if ( mAutoPushEnabled == enabled )
    return;

  mAutoPushEnabled = enabled;
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "autoPushEnabled" ), mAutoPushEnabled );
  emit autoPushEnabledChanged();
}

void QFieldCloudProject::setAutoPushIntervalMins( int minutes )
{
  if ( mAutoPushIntervalMins == minutes )
    return;

  mAutoPushIntervalMins = minutes;
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "autoPushIntervalMins" ), mAutoPushIntervalMins );
  emit autoPushIntervalMinsChanged();
}

void QFieldCloudProject::refreshFileOutdatedStatus()
{
  NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/files/%1/" ).arg( mId ) );

  connect( reply, &NetworkReply::finished, reply, [=]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    reply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: failed to refresh the project file outdated satus. %2" ).arg( mId, QFieldCloudConnection::errorString( rawReply ) ) );
      return;
    }

    const QJsonArray files = QJsonDocument::fromJson( rawReply->readAll() ).array();
    const QString lastProjectFileMd5 = QFieldCloudUtils::projectSetting( mId, QStringLiteral( "lastProjectFileMd5" ), QString() ).toString();
    for ( const auto file : files )
    {
      QVariantHash fileDetails = file.toObject().toVariantHash();
      const QString fileName = fileDetails.value( "name" ).toString().toLower();
      if ( fileName.endsWith( QStringLiteral( ".qgs" ) ) || fileName.endsWith( QStringLiteral( ".qgz" ) ) )
      {
        if ( lastProjectFileMd5.isEmpty() )
        {
          // First check, store for future comparison
          QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastProjectFileMd5" ), fileDetails.value( "md5sum" ).toString() );
        }
        else if ( lastProjectFileMd5 != fileDetails.value( "md5sum" ).toString() )
        {
          mProjectFileIsOutdated = true;
          QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "projectFileOudated" ), true );
          emit projectFileIsOutdatedChanged();
        }
      }
    }
  } );
}

void QFieldCloudProject::packageAndDownload()
{
  QgsLogger::debug( QStringLiteral( "Project %1: package and download initiated." ).arg( mId ) );

  if ( !mCloudConnection )
    return;

  if ( mStatus != ProjectStatus::Idle )
  {
    return;
  }

  mPackagingStatus = PackagingUnstartedStatus;
  mPackagingStatusString = QString();
  mPackagedLayerErrors.clear();
  mDownloadFileTransfers.clear();
  mDownloadFilesFinished = 0;
  mDownloadFilesFailed = 0;
  mDownloadBytesTotal = 0;
  mDownloadBytesReceived = 0;
  mDownloadProgress = 0;
  mStatus = ProjectStatus::Downloading;
  mErrorStatus = NoErrorStatus;
  mModification = NoModification;

  emit packagingStatusChanged();
  emit packagingStatusStringChanged();
  emit packagedLayerErrorsChanged();
  emit downloadFilesFinishedChanged();
  emit downloadFilesFailedChanged();
  emit downloadBytesTotalChanged();
  emit downloadBytesReceivedChanged();
  emit downloadProgressChanged();
  emit statusChanged();
  emit errorStatusChanged();
  emit modificationChanged();

  auto repackageIfNeededAndThenDownload = [=]() {
    if ( mNeedsRepackaging )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: repackaging triggered." ).arg( mId ) );

      mPackagingStatus = PackagingBusyStatus;
      emit packagingStatusChanged();

      startJob( JobType::Package );

      QObject *tempProjectJobFinishedParent = new QObject( this ); // we need this to unsubscribe
      connect( this, &QFieldCloudProject::jobFinished, tempProjectJobFinishedParent, [=]( const JobType type, const QString &errorString ) {
        if ( type != JobType::Package )
        {
          QMetaEnum me = QMetaEnum::fromType<JobType>();
          QgsLogger::debug( QStringLiteral( "Project %1: unexpected job type, expected %2 but %3 received." ).arg( mId, me.valueToKey( static_cast<int>( JobType::Package ) ), me.valueToKey( static_cast<int>( type ) ) ) );
          Q_ASSERT( 0 );
          return;
        }

        tempProjectJobFinishedParent->deleteLater();

        if ( mPackagingStatus == PackagingAbortStatus )
        {
          // no need to emit why we aborted packaging, whoever sets the packagingStatus is responsible to inform the user
          QgsLogger::debug( QStringLiteral( "Project %1: packaging finished, but project operations are aborted." ).arg( mId ) );
          return;
        }

        if ( mJobs[type].status != JobFinishedStatus )
        {
          QgsLogger::warning( QStringLiteral( "Project %1: packaging has an error: %2" ).arg( mId, errorString ) );

          mJobs.take( type );

          mPackagingStatus = PackagingErrorStatus;
          mPackagingStatusString = errorString;
          emit packagingStatusChanged();

          emit downloadFinished( tr( "Packaging job finished unsuccessfully for `%1`. %2" ).arg( mName ).arg( errorString ) );
          return;
        }

        mPackagingStatus = PackagingFinishedStatus;
        mPackagingStatusString = QString();
        emit packagingStatusChanged();

        download();
      } );
    }
    else
    {
      download();
    }
  };

  // Check and refresh project data if needed, because it might be outdated
  if ( !mLastRefreshedAt.isValid() || mLastRefreshedAt.secsTo( QDateTime::currentDateTimeUtc() ) > CACHE_PROJECT_DATA_SECS )
  {
    QgsLogger::debug( QStringLiteral( "Project %1: refreshing data..." ).arg( mId ) );

    refreshData( ProjectRefreshReason::Package );

    QObject *tempProjectRefreshParent = new QObject( this ); // we need this to unsubscribe
    connect( this, &QFieldCloudProject::dataRefreshed, tempProjectRefreshParent, [=]( const ProjectRefreshReason reason, const QString &error ) {
      if ( reason != ProjectRefreshReason::Package )
      {
        QgsLogger::critical( QStringLiteral( "Project %1: unexpected job type, expected %2 but %3 received." ).arg( mId ).arg( static_cast<int>( ProjectRefreshReason::Package ), static_cast<int>( reason ) ) );
        Q_ASSERT( 0 );
        return;
      }

      tempProjectRefreshParent->deleteLater();

      if ( mPackagingStatus == PackagingAbortStatus )
      {
        // no need to emit why we aborted packaging, whoever sets the packagingStatus is responsible to inform the user
        QgsLogger::debug( QStringLiteral( "Project %1: refreshing data finished, but project operations are aborted." ).arg( mId ) );
        return;
      }

      if ( !error.isNull() )
      {
        QgsLogger::debug( QStringLiteral( "Project %1: refreshing data finished with an error: %2." ).arg( mId, error ) );
        emit downloadFinished( tr( "Failed to refresh the latest info for `%1`: %2" ).arg( mName, error ) );
        return;
      }

      repackageIfNeededAndThenDownload();
    } );
  }
  else
  {
    repackageIfNeededAndThenDownload();
  }

  QObject *tempProjectDownloadFinishedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProject::downloadFinished, tempProjectDownloadFinishedParent, [=]( const QString &error ) {
    tempProjectDownloadFinishedParent->deleteLater();

    if ( mPackagingStatus == PackagingAbortStatus )
    {
      // no need to emit why we aborted packaging, it is callers responsibility to inform the user
      QgsLogger::debug( QStringLiteral( "Project %1: downloading project finished, but project operations are aborted." ).arg( mId ) );
      return;
    }

    const QStringList fileKeys = mDownloadFileTransfers.keys();
    for ( const QString &fileKey : fileKeys )
    {
      if ( mDownloadFileTransfers[fileKey].networkReply )
      {
        if ( !mDownloadFileTransfers[fileKey].networkReply->isFinished() )
        {
          mDownloadFileTransfers[fileKey].networkReply->abort();
        }
        mDownloadFileTransfers[fileKey].networkReply->deleteLater();
      }
    }

    mActiveFilesToDownload.clear();

    const bool hasError = !error.isNull();
    if ( hasError )
    {
      mErrorStatus = DownloadErrorStatus;
      QgsMessageLog::logMessage( QStringLiteral( "Downloading project `%1` finished with an error: %2" ).arg( mId ).arg( error ) );
    }
    else
    {
      mErrorStatus = NoErrorStatus;
    }

    mStatus = ProjectStatus::Idle;

    emit downloaded( mName, error );
  } );
}

void QFieldCloudProject::download()
{
  QVariantMap params;
  params.insert( "skip_metadata", "1" );
  NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/packages/%1/latest/" ).arg( mId ), params );

  connect( reply, &NetworkReply::finished, reply, [=]() {
    reply->deleteLater();

    if ( mPackagingStatus == PackagingAbortStatus )
    {
      // no need to emit why we aborted packaging, it is callers responsibility to inform the user
      QgsLogger::debug( QStringLiteral( "Project %1: adding download file connections, but the project is aborted." ).arg( mId ) );
      return;
    }

    QNetworkReply *rawReply = reply->currentRawReply();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: failed to get latest package data. %2" ).arg( mId, QFieldCloudConnection::errorString( rawReply ) ) );
      emit downloadFinished( tr( "Failed to get latest package data." ) );
      return;
    }

    const QJsonObject payload = QJsonDocument::fromJson( rawReply->readAll() ).object();
    const QString packageId = payload.value( QStringLiteral( "package_id" ) ).toString();
    const QString packagedAt = payload.value( QStringLiteral( "packaged_at" ) ).toString();

    if ( packageId.isNull() || packagedAt.isNull() || !payload.value( QStringLiteral( "files" ) ).isArray() )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: JSON for package does not contain the expected fields: package_id(string), packaged_at(string), files(array), layers(object)" ).arg( mId ) );
      emit downloadFinished( tr( "Latest package data response error." ) );
      return;
    }

    const QJsonArray files = payload.value( QStringLiteral( "files" ) ).toArray();
    for ( const QJsonValue fileValue : files )
    {
      const QJsonObject fileObject = fileValue.toObject();
      const int fileSize = fileObject.value( QStringLiteral( "size" ) ).toInt();
      const QString fileName = fileObject.value( QStringLiteral( "name" ) ).toString();
      const QString projectFileName = QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId, fileName );
      // NOTE the cloud API is giving the false impression that the file keys `md5sum` is having a MD5 or another checksum.
      // This actually is an Object Storage (S3) implementation specific ETag.
      const QString cloudEtag = fileObject.value( QStringLiteral( "md5sum" ) ).toString();
      const QString localEtag = FileUtils::fileEtag( projectFileName );

      if ( !fileObject.value( QStringLiteral( "size" ) ).isDouble() || fileName.isEmpty() || cloudEtag.isEmpty() )
      {
        QgsLogger::debug( QStringLiteral( "Project %1: package in \"files\" list does not contain the expected fields: size(int), name(string), md5sum(string)" ).arg( mId ) );
        emit downloadFinished( tr( "Latest package data structure error." ) );
        return;
      }

      if ( cloudEtag == localEtag )
        continue;

      mDownloadFileTransfers.insert( QStringLiteral( "%1/%2" ).arg( mId, fileName ), FileTransfer( fileName, fileSize, mId ) );
      mDownloadBytesTotal += std::max( fileSize, 0 );
    }

    emit downloadBytesTotalChanged();

    const QJsonObject layers = payload.value( QStringLiteral( "layers" ) ).toObject();
    QStringList localizedDatasets;
    bool hasLayerExportErrror = false;
    for ( const QString &layerKey : layers.keys() )
    {
      QJsonObject layer = layers.value( layerKey ).toObject();
      QString layerName = layer.value( QStringLiteral( "name" ) ).toString();
      QString layerStatus = layer.value( QStringLiteral( "error_code" ) ).toString();

      if ( layerKey.isEmpty() || layerName.isEmpty() || layerStatus.isEmpty() || !layer.value( QStringLiteral( "is_valid" ) ).isBool() )
      {
        QgsLogger::debug( QStringLiteral( "Project %1: JSON structure package in \"layers\" list does not contain the expected fields: name(string), error_code(string), is_valid(bool)" ).arg( mId ) );
      }
      else
      {
        if ( layer.value( QStringLiteral( "is_localized" ) ).toBool() )
        {
          const QString localizedDataset = layer.value( "datasource" ).toString().mid( 10 );
          if ( !localizedDataset.isEmpty() )
          {
            localizedDatasets << localizedDataset;
          }
        }
        else if ( !layer.value( QStringLiteral( "is_valid" ) ).toBool() )
        {
          QString errorSummary = layer.value( QStringLiteral( "error_summary" ) ).toString() + layer.value( QStringLiteral( "provider_error_summary" ) ).toString();

          mPackagedLayerErrors.append( tr( "Project %1: Packaged layer `%2` is not valid. Error code %3, error message: %4" ).arg( mId, layerName, layerStatus, errorSummary ) );
          QgsMessageLog::logMessage( mPackagedLayerErrors.last() );

          hasLayerExportErrror = true;
        }
      }
    }

    if ( hasLayerExportErrror )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: packaged files list request finished with some failed layers:\n%2" ).arg( mId, mPackagedLayerErrors.join( QStringLiteral( "\n" ) ) ) );
      emit packagedLayerErrorsChanged();
    }

    mLastExportId = packageId;
    mLastExportedAt = packagedAt;

    //TODO
    if ( false ) //( !localizedDatasets.isEmpty() && mLocalizedDatasetsProjects.contains( mOwner ) )
    {
      //  NetworkReply *localizedDatasetsReply = mCloudConnection->get( QStringLiteral( "/api/v1/files/%1/" ).arg( mLocalizedDatasetsProjects[mOwner] ) );
      //  connect( localizedDatasetsReply, &NetworkReply::finished, localizedDatasetsReply, [=]() {
      //    QNetworkReply *localizedDatasetsRawReply = localizedDatasetsReply->currentRawReply();
      //    localizedDatasetsReply->deleteLater();

      //    if ( localizedDatasetsRawReply->error() != QNetworkReply::NoError )
      //    {
      //      QgsLogger::debug( QStringLiteral( "Project %1: failed to get latest package data. %2" ).arg( mId, QFieldCloudConnection::errorString( rawReply ) ) );
      //      emit downloadFinished( tr( "Failed to get latest package data." ) );
      //      return;
      //    }

      //    const QJsonArray files = QJsonDocument::fromJson( localizedDatasetsRawReply->readAll() ).array();
      //    for ( const QJsonValue fileValue : files )
      //    {
      //      const QJsonObject fileObject = fileValue.toObject();
      //      const QString fileName = fileObject.value( QStringLiteral( "name" ) ).toString();
      //      if ( localizedDatasets.contains( fileName ) )
      //      {
      //        const int fileSize = fileObject.value( QStringLiteral( "size" ) ).toInt();
      //        const QString absoluteFileName = QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mLocalizedDatasetsProjects[mOwner], fileName );
      //        // NOTE the cloud API is giving the false impression that the file keys `md5sum` is having a MD5 or another checksum.
      //        // This actually is an Object Storage (S3) implementation specific ETag.
      //        const QString cloudEtag = fileObject.value( QStringLiteral( "md5sum" ) ).toString();
      //        const QString localEtag = FileUtils::fileEtag( absoluteFileName );

      //        if (
      //          !fileObject.value( QStringLiteral( "size" ) ).isDouble()
      //          || fileName.isEmpty()
      //          || cloudEtag.isEmpty() )
      //        {
      //          QgsLogger::debug( QStringLiteral( "Project %1: package in \"files\" list does not contain the expected fields: size(int), name(string), md5sum(string)" ).arg( mLocalizedDatasetsProjects[mOwner] ) );
      //          emit downloadFinished( tr( "Latest package data structure error." ) );
      //          return;
      //        }

      //        if ( cloudEtag == localEtag )
      //          continue;

      //        mDownloadFileTransfers.insert( QStringLiteral( "%1/%2" ).arg( mLocalizedDatasetsProjects[mOwner], fileName ), FileTransfer( fileName, fileSize, mLocalizedDatasetsProjects[mOwner] ) );
      //        mDownloadBytesTotal += std::max( fileSize, 0 );
      //      }
      //    }
      //
      //    emit downloadBytesTotalChanged();

      //    QgsLogger::debug( QStringLiteral( "Project %1: packaged files to download - %2 files, namely: %3" ).arg( mId ).arg( mDownloadFileTransfers.count() ).arg( mDownloadFileTransfers.keys().join( ", " ) ) );

      //    updateActiveFilesToDownload();
      //    downloadFiles();
      //  } );
    }
    else
    {
      QgsLogger::debug( QStringLiteral( "Project %1: packaged files to download - %2 files, namely: %3" ).arg( mId ).arg( mDownloadFileTransfers.count() ).arg( mDownloadFileTransfers.keys().join( ", " ) ) );

      updateActiveFilesToDownload();
      downloadFiles();
    }
  } );
}

void QFieldCloudProject::updateActiveFilesToDownload()
{
  if ( !mCloudConnection )
    return;

  const QStringList fileKeys = mDownloadFileTransfers.keys();

  if ( fileKeys.isEmpty() )
  {
    mActiveFilesToDownload.clear();
    return;
  }

  for ( const QString &fileKey : fileKeys )
  {
    if ( mDownloadFileTransfers[fileKey].networkReply )
    {
      if ( mDownloadFileTransfers[fileKey].networkReply->isFinished() )
      {
        if ( mActiveFilesToDownload.removeOne( fileKey ) )
        {
          // QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: removed from the list of active download files" ).arg( projectId, fileName ) );
        }
        continue;
      }
      else
      {
        // the request is still active
        continue;
      }
    }

    if ( mActiveFilesToDownload.size() >= MAX_PARALLEL_REQUESTS )
    {
      return;
    }

    // QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: appended to the active download files list" ).arg( projectId, fileName ) );

    mActiveFilesToDownload.append( fileKey );
  }

  if ( mActiveFilesToDownload.count() > 0 )
  {
    QgsLogger::debug( QStringLiteral( "Project %1: active download files list contains %2 files, namely: %3" ).arg( mId ).arg( mActiveFilesToDownload.count() ).arg( mActiveFilesToDownload.join( ", " ) ) );
  }
  else
  {
    QgsLogger::debug( QStringLiteral( "Project %1: active download files list is empty" ).arg( mId ) );
  }
}

void QFieldCloudProject::downloadFiles()
{
  // calling updateActiveProjectFilesToDownload() before calling this function is mandatory

  if ( !mCloudConnection )
    return;

  // Don't call download project files, if there are no project files
  if ( mActiveFilesToDownload.isEmpty() )
  {
    mStatus = ProjectStatus::Idle;
    mDownloadProgress = 1;

    emit statusChanged();
    emit downloadProgressChanged();

    return;
  }

  QgsLogger::debug( QStringLiteral( "Project %1: active download files list before actual download: %2" ).arg( mId, mActiveFilesToDownload.join( ", " ) ) );

  for ( const QString &fileKey : std::as_const( mActiveFilesToDownload ) )
  {
    if ( mDownloadFileTransfers[fileKey].networkReply )
    {
      // Download is already in progress
      continue;
    }

    NetworkReply *reply = downloadFile( mDownloadFileTransfers[fileKey].projectId, mDownloadFileTransfers[fileKey].fileName, mDownloadFileTransfers[fileKey].projectId == mId );

    QTemporaryFile *file = new QTemporaryFile( reply );
    file->setAutoRemove( false );

    if ( !file->open() )
    {
      mDownloadFilesFailed++;
      emit downloadFinished( tr( "Failed to open temporary file for `%1`, reason:\n%2" ).arg( mDownloadFileTransfers[fileKey].fileName ).arg( file->errorString() ) );
      return;
    }

    mDownloadFileTransfers[fileKey].tmpFile = file->fileName();
    mDownloadFileTransfers[fileKey].networkReply = reply;

    downloadFileConnections( fileKey );
  }
}

void QFieldCloudProject::downloadFileConnections( const QString &fileKey )
{
  if ( !mDownloadFileTransfers.contains( fileKey ) )
  {
    Q_ASSERT( false );
    return;
  }

  NetworkReply *reply = mDownloadFileTransfers[fileKey].networkReply;
  if ( !reply )
  {
    Q_ASSERT( false );
    return;
  }

  const QStringList fileKeys = mDownloadFileTransfers.keys();

  QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: requested." ).arg( mDownloadFileTransfers[fileKey].projectId, mDownloadFileTransfers[fileKey].fileName ) );

  connect( reply, &NetworkReply::redirected, reply, [=]( const QUrl &url ) {
    QUrl oldUrl = mDownloadFileTransfers[fileKey].lastRedirectUrl;

    mDownloadFileTransfers[fileKey].redirectsCount++;
    mDownloadFileTransfers[fileKey].lastRedirectUrl = url;

    if ( mDownloadFileTransfers[fileKey].redirectsCount >= MAX_REDIRECTS_ALLOWED )
    {
      QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: too many redirects, last two urls are `%3` and `%4`" ).arg( mId, fileKey, oldUrl.toString(), url.toString() ) );
      reply->abort();
      return;
    }

    if ( oldUrl == url )
    {
      QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: has redirects to the same URL `%3`" ).arg( mId, fileKey, url.toString() ) );
      reply->abort();
      return;
    }

    QgsLogger::debug( QStringLiteral( "Package %1, file `%2`: redirected to `%3`" ).arg( mId, fileKey, url.toString() ) );

    QNetworkRequest request;
    mDownloadFileTransfers[fileKey].networkReply = mCloudConnection->get( request, url );
    mDownloadFileTransfers[fileKey].networkReply->setParent( reply );

    // we need to somehow finish the request, otherwise it will remain unfinished for the QFieldCloudConnection
    reply->abort();

    downloadFileConnections( fileKey );
  } );

  connect( reply, &NetworkReply::downloadProgress, reply, [=]( int bytesReceived, int bytesTotal ) {
    QNetworkReply *rawReply = reply->currentRawReply();
    if ( !rawReply )
    {
      return;
    }

    const QString temporaryFileName = mDownloadFileTransfers[fileKey].tmpFile;
    QFile file( temporaryFileName );
    QString errorMessageDetail;
    QString errorMessage;
    bool hasError = false;

    if ( file.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      file.write( rawReply->readAll() );

      if ( file.error() != QFile::NoError )
      {
        hasError = true;
        errorMessageDetail = file.errorString();
        errorMessage = tr( "File system error. Failed to write file to temporary location `%1`." ).arg( temporaryFileName );
      }

      file.close();
    }
    else
    {
      hasError = true;
      errorMessageDetail = file.errorString();
      errorMessage = tr( "File system error. Failed to open file for writing on temporary `%1`." ).arg( temporaryFileName );
    }

    // check if the code above failed with error
    if ( hasError )
    {
      logFailedDownload( fileKey, errorMessage, errorMessageDetail );
      rawReply->abort();
      return;
    }

    Q_UNUSED( bytesTotal )

    mDownloadBytesReceived -= mDownloadFileTransfers[fileKey].bytesTransferred;
    mDownloadBytesReceived += bytesReceived;
    mDownloadFileTransfers[fileKey].bytesTransferred = bytesReceived;
    mDownloadProgress = std::clamp( ( static_cast<double>( mDownloadBytesReceived ) / std::max( mDownloadBytesTotal, 1 ) ), 0., 1. );

    emit downloadBytesReceivedChanged();
    emit downloadProgressChanged();
  } );

  connect( reply, &NetworkReply::finished, reply, [=]() {
    if ( mPackagingStatus == PackagingAbortStatus )
    {
      return;
    }

    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( reply );

    // this is most probably the redirected request, nothing to do with this reply anymore, just ignore it
    if ( mDownloadFileTransfers[fileKey].networkReply != reply )
    {
      return;
    }

    mDownloadFilesFinished++;

    bool hasError = false;
    QString errorMessageDetail;
    QString errorMessage;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      hasError = true;
      errorMessageDetail = QFieldCloudConnection::errorString( rawReply );
      errorMessage = tr( "Network error. Failed to download file `%1`." ).arg( fileKey );
    }

    if ( !hasError )
    {
      mDownloadBytesReceived -= mDownloadFileTransfers[fileKey].bytesTransferred;
      mDownloadBytesReceived += mDownloadFileTransfers[fileKey].bytesTotal;
      mDownloadProgress = std::clamp( ( static_cast<double>( mDownloadBytesReceived ) / std::max( mDownloadBytesTotal, 1 ) ), 0., 1. );

      emit downloadBytesReceivedChanged();
      emit downloadProgressChanged();
    }

    // check if the code above failed with error
    if ( hasError )
    {
      logFailedDownload( fileKey, errorMessage, errorMessageDetail );
      rawReply->abort();
      return;
    }

    QgsLogger::debug( QStringLiteral( "Package %1, file `%2`: downloaded" ).arg( mId, fileKey ) );

    updateActiveFilesToDownload();

    if ( mDownloadFilesFinished == fileKeys.count() )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: All files downloaded." ).arg( mId ) );
      Q_ASSERT( mActiveFilesToDownload.size() == 0 );

      if ( !hasError )
      {
        QDir projectPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId ) );
        const bool currentProjectReloadNeeded = QgsProject::instance()->homePath().startsWith( projectPath.absolutePath() );
        QStringList gpkgFileNames;
        if ( currentProjectReloadNeeded )
        {
          // we need to close the project to safely flush the gpkg files and avoid file lock on Windows
          QDirIterator it( projectPath.absolutePath(), { QStringLiteral( "*.gpkg" ) }, QDir::Filter::Files, QDirIterator::Subdirectories );
          while ( it.hasNext() )
          {
            gpkgFileNames << it.nextFileInfo().absoluteFilePath();
          }

          QgsProject::instance()->clear();
          if ( mGpkgFlusher )
          {
            for ( const QString &fileName : gpkgFileNames )
            {
              mGpkgFlusher->stop( fileName );
            }
          }
        }

        // move the files from their temporary location to their permanent one
        if ( !moveDownloadedFilesToPermanentStorage() )
        {
          emit downloadFinished( tr( "Failed to copy some of the downloaded files on your device. Check your device storage." ) );
          return;
        }

        if ( currentProjectReloadNeeded )
        {
          // Clear up Geopackage's shm and wal files
          for ( const QString &fileName : gpkgFileNames )
          {
            QFile shmFile( QStringLiteral( "%1-shm" ).arg( fileName ) );
            if ( shmFile.exists() )
            {
              if ( !shmFile.remove() )
              {
                QgsMessageLog::logMessage( QStringLiteral( "Failed to remove -shm file '%1' " ).arg( shmFile.fileName() ) );
              }
            }

            QFile walFile( QStringLiteral( "%1-wal" ).arg( fileName ) );
            if ( walFile.exists() )
            {
              if ( !walFile.remove() )
              {
                QgsMessageLog::logMessage( QStringLiteral( "Failed to remove -wal file '%1' " ).arg( walFile.fileName() ) );
              }
            }
          }

          AppInterface::instance()->reloadProject();
        }

        mStatus = ProjectStatus::Idle;
        mErrorStatus = NoErrorStatus;
        mCheckout = ProjectCheckout::LocalAndRemoteCheckout;
        mLocalPath = QFieldCloudUtils::localProjectFilePath( mUsername, mId );
        mLastLocalExportedAt = QDateTime::currentDateTimeUtc().toString( Qt::ISODate );
        mLastLocalExportId = QUuid::createUuid().toString( QUuid::WithoutBraces );
        mLastLocalDataLastUpdatedAt = mDataLastUpdatedAt;
        mIsOutdated = false;
        mProjectFileIsOutdated = false;

        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastExportedAt" ), mLastExportedAt );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastExportId" ), mLastExportId );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalExportedAt" ), mLastLocalExportedAt );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalExportId" ), mLastLocalExportId );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalDataLastUpdatedAt" ), mLastLocalDataLastUpdatedAt );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastProjectFileMd5" ), QString() );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "projectFileOudated" ), false );

        emit statusChanged();
        emit errorStatusChanged();
        emit checkoutChanged();
        emit localPathChanged();
        emit lastLocalExportedAtChanged();
        emit lastLocalExportedIdChanged();
        emit lastDataLastUpdatedAtChanged();
        emit isOutdatedChanged();
        emit projectFileIsOutdatedChanged();

        emit downloadFinished();
      }
    }
    else
    {
      downloadFiles();
    }
  } );
}

NetworkReply *QFieldCloudProject::downloadFile( const QString &projectId, const QString &fileName, bool fromLatestPackage )
{
  QNetworkRequest request;
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::UserVerifiedRedirectPolicy );
  mCloudConnection->setAuthenticationDetails( request );

  return mCloudConnection->get( request, fromLatestPackage ? QStringLiteral( "/api/v1/packages/%1/latest/files/%2/" ).arg( projectId, fileName ) : QStringLiteral( "/api/v1/files/%1/%2/" ).arg( projectId, fileName ) );
}

bool QFieldCloudProject::moveDownloadedFilesToPermanentStorage()
{
  bool hasError = false;
  const QStringList fileKeys = mDownloadFileTransfers.keys();

  for ( const QString &fileKey : fileKeys )
  {
    QFileInfo fileInfo( mDownloadFileTransfers[fileKey].fileName );
    QFile file( mDownloadFileTransfers[fileKey].tmpFile );
    QDir dir( QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mDownloadFileTransfers[fileKey].projectId, fileInfo.path() ) );

    if ( !hasError && !dir.exists() && !dir.mkpath( QStringLiteral( "." ) ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to create directory at `%1`" ).arg( dir.path() ) );
    }

    const QString destinationFileName( QDir::cleanPath( dir.filePath( fileInfo.fileName() ) ) );

    // if the file already exists, we need to delete it first, as QT does not support overwriting
    // NOTE: it is possible that someone creates the file in the meantime between this and the next if statement
    if ( !hasError && QFile::exists( destinationFileName ) && !file.remove( destinationFileName ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove file before overwriting stored at `%1`, reason:\n%2" ).arg( destinationFileName ).arg( file.errorString() ) );
    }

    if ( !hasError && !file.copy( destinationFileName ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to write downloaded file stored at `%1`, reason:\n%2" ).arg( destinationFileName ).arg( file.errorString() ) );

      if ( !QFile::remove( dir.filePath( mDownloadFileTransfers[fileKey].fileName ) ) )
        QgsMessageLog::logMessage( QStringLiteral( "Failed to remove partly overwritten file stored at `%1`" ).arg( destinationFileName ) );
    }

    if ( !file.remove() )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove temporary file `%1`" ).arg( destinationFileName ) );
    }
  }

  return !hasError;
}

void QFieldCloudProject::logFailedDownload( const QString &fileKey, const QString &errorMessage, const QString &errorMessageDetail )
{
  mDownloadFilesFailed++;

  QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: %3 %4" ).arg( mId, fileKey, errorMessage, errorMessageDetail ) );

  // translate the user messages
  const QString baseMessage = tr( "Project `%1`, file `%2`: %3" ).arg( mName, fileKey, errorMessage );
  const QString trimmedMessage = baseMessage + QStringLiteral( " " ) + tr( "System message: " )
                                 + ( ( errorMessageDetail.size() > 100 )
                                       ? ( errorMessageDetail.left( 100 ) + tr( " (see more in the QField error log)…" ) )
                                       : errorMessageDetail );

  QgsMessageLog::logMessage( QStringLiteral( "%1\n%2" ).arg( baseMessage, errorMessageDetail ) );

  emit downloadFinished( trimmedMessage );
}


void QFieldCloudProject::upload( LayerObserver *layerObserver, bool shouldDownloadUpdates )
{
  if ( mStatus != ProjectStatus::Idle )
  {
    return;
  }

  DeltaFileWrapper *deltaFileWrapper = layerObserver->deltaFileWrapper();

  if ( shouldDownloadUpdates && deltaFileWrapper->count() == 0 )
  {
    mStatus = ProjectStatus::Idle;
    packageAndDownload();
    return;
  }

  if ( !( mModification & LocalModification ) )
  {
    return;
  }

  if ( !layerObserver->deltaFileWrapper()->toFile() )
  {
    return;
  }

  if ( deltaFileWrapper->hasError() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "The delta file has an error: %1" ).arg( deltaFileWrapper->errorString() ) );
    return;
  }

  deltaFileWrapper->setIsPushing( true );

  mStatus = ProjectStatus::Uploading;
  mDeltaFileId = deltaFileWrapper->id();
  mDeltaFileUploadStatus = DeltaLocalStatus;
  mDeltaFileUploadStatusString = QString();
  mUploadDeltaProgress = 0.0;

  emit statusChanged();
  emit deltaFileIdChanged();
  emit deltaFileUploadStatusChanged();
  emit deltaFileUploadStatusStringChanged();
  emit uploadDeltaProgressChanged();

  refreshModification( layerObserver );

  // //////////
  // prepare attachment files to be uploaded
  // //////////

  const QFileInfo projectInfo( QFieldCloudUtils::localProjectFilePath( mUsername, mId ) );
  const QDir projectDir( projectInfo.absolutePath() );
  const QStringList attachmentFileNames = deltaFileWrapper->attachmentFileNames().keys();

  for ( const QString &fileName : attachmentFileNames )
  {
    if ( fileName.isEmpty() )
      continue;

    QString absoluteFilePath = fileName;
    QFileInfo fileInfo( fileName );
    if ( fileInfo.isRelative() )
    {
      absoluteFilePath = projectDir.absoluteFilePath( fileName );
      fileInfo = QFileInfo( absoluteFilePath );
    }

    if ( !fileInfo.exists() || !fileInfo.isFile() )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Attachment file '%1' does not exist" ).arg( absoluteFilePath ) );
      continue;
    }

    const long long fileSize = fileInfo.size();

    // ? should we also check the checksums of the files being uploaded? they are available at deltaFile->attachmentFileNames()->values()
    QFieldCloudUtils::addPendingAttachments( mId, { absoluteFilePath } );
  }

  QString deltaFileToUpload = deltaFileWrapper->toFileForUpload();

  if ( deltaFileToUpload.isEmpty() )
  {
    deltaFileWrapper->setIsPushing( false );
    mStatus = ProjectStatus::Idle;
    emit statusChanged();
    return;
  }

  // //////////
  // 1) upload the deltas
  // //////////
  NetworkReply *deltasCloudReply = mCloudConnection->post( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mId ), QVariantMap(), QStringList( { deltaFileToUpload } ) );

  Q_ASSERT( deltasCloudReply );

  connect( deltasCloudReply, &NetworkReply::uploadProgress, this, [=]( int bytesSent, int bytesTotal ) {
    mUploadDeltaProgress = std::clamp( ( static_cast<double>( bytesSent ) / bytesTotal ), 0., 1. );
    emit uploadDeltaProgressChanged();
  } );

  connect( deltasCloudReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *deltasReply = deltasCloudReply->currentRawReply();
    deltasCloudReply->deleteLater();

    Q_ASSERT( deltasCloudReply->isFinished() );
    Q_ASSERT( deltasReply );

    // if there is an error, cannot continue sync
    if ( deltasReply->error() != QNetworkReply::NoError )
    {
      mDeltaFileUploadStatusString = QFieldCloudConnection::errorString( deltasReply );
      // TODO check why exactly we failed
      // maybe the project does not exist, then create it?
      QgsMessageLog::logMessage( QStringLiteral( "Failed to upload delta file, reason:\n%1\n%2" ).arg( deltasReply->errorString(), mDeltaFileUploadStatusString ) );

      layerObserver->deltaFileWrapper()->setIsPushing( false );

      cancelUpload();
      return;
    }

    mUploadDeltaProgress = 1.0;
    mDeltaFileUploadStatus = DeltaPendingStatus;
    mDeltaLayersToDownload = layerObserver->deltaFileWrapper()->deltaLayerIds();

    emit uploadDeltaProgressChanged();
    emit deltaFileUploadStatusChanged();

    emit networkDeltaUploaded();
  } );


  // //////////
  // 2) delta successfully uploaded
  // //////////
  QObject *networkDeltaUploadedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProject::networkDeltaUploaded, networkDeltaUploadedParent, [=]() {
    delete networkDeltaUploadedParent;

    if ( shouldDownloadUpdates )
    {
      getDeltaStatus();
    }
    else
    {
      mStatus = ProjectStatus::Idle;
      mModification ^= LocalModification;
      mLastLocalPushDeltas = QDateTime::currentDateTimeUtc().toString( Qt::ISODate );

      QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalPushDeltas" ), mLastLocalPushDeltas );

      DeltaFileWrapper *deltaFileWrapper = layerObserver->deltaFileWrapper();
      deltaFileWrapper->reset();
      deltaFileWrapper->resetId();
      deltaFileWrapper->setIsPushing( false );

      if ( !deltaFileWrapper->toFile() )
      {
        QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file after delta push. %1" ).arg( deltaFileWrapper->errorString() ) );
      }

      emit statusChanged();
      emit modificationChanged();
      emit lastLocalPushDeltasChanged();

      emit uploadFinished( false );

      refreshData( ProjectRefreshReason::DeltaUploaded );
    }
  } );

  // //////////
  // 3) new delta status received. Never give up to get a successful status.
  // //////////
  QObject *networkDeltaStatusCheckedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProject::networkDeltaStatusChecked, networkDeltaStatusCheckedParent, [=]() {
    DeltaFileWrapper *deltaFileWrapper = layerObserver->deltaFileWrapper();

    switch ( mDeltaFileUploadStatus )
    {
      case DeltaLocalStatus:
        // delta file should be already sent!!!
        Q_ASSERT( 0 );
        [[fallthrough]];

      case DeltaPendingStatus:
      case DeltaBusyStatus:
        // infinite retry, there should be one day, when we can get the status!
        QTimer::singleShot( sDelayBeforeStatusRetry, [=]() { getDeltaStatus(); } );
        break;

      case DeltaErrorStatus:
        delete networkDeltaStatusCheckedParent;
        deltaFileWrapper->resetId();
        deltaFileWrapper->setIsPushing( false );

        if ( !deltaFileWrapper->toFile() )
          QgsMessageLog::logMessage( QStringLiteral( "Failed update committed delta file." ) );

        cancelUpload();
        return;

      case DeltaConflictStatus:
      case DeltaNotAppliedStatus:
      case DeltaAppliedStatus:
        delete networkDeltaStatusCheckedParent;

        deltaFileWrapper->reset();
        deltaFileWrapper->resetId();
        deltaFileWrapper->setIsPushing( false );

        if ( !deltaFileWrapper->toFile() )
          QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file. %1" ).arg( deltaFileWrapper->errorString() ) );

        mStatus = ProjectStatus::Idle;
        mModification ^= LocalModification;
        mModification |= RemoteModification;
        mLastLocalPushDeltas = QDateTime::currentDateTimeUtc().toString( Qt::ISODate );

        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalPushDeltas" ), mLastLocalPushDeltas );

        emit modificationChanged();
        emit lastLocalPushDeltasChanged();

        // download the updated files, so the files are for sure the same on the client and on the server
        if ( shouldDownloadUpdates )
        {
          emit uploadFinished( true, QString() );
          packageAndDownload();
        }
        else
        {
          emit statusChanged();
          emit uploadFinished( false, QString() );
          refreshData( ProjectRefreshReason::DeltaUploaded );
        }
    }
  } );
}

void QFieldCloudProject::cancelUpload()
{
  mStatus = ProjectStatus::Idle;
  mErrorStatus = UploadErrorStatus;

  emit statusChanged();
  emit errorStatusChanged();

  emit uploadFinished( false, mDeltaFileUploadStatusString );

  return;
}


void QFieldCloudProject::startJob( JobType type )
{
  mJobs[type] = Job( QString(), mId, type );

  QString jobTypeName = getJobTypeAsString( type );
  QgsLogger::debug( QStringLiteral( "Project %1: creating a new `%2` job..." ).arg( mId, jobTypeName ) );
  NetworkReply *reply = mCloudConnection->post( QStringLiteral( "/api/v1/jobs/" ),
                                                QVariantMap(
                                                  {
                                                    { "project_id", mId },
                                                    { "type", jobTypeName },
                                                  } ) );

  connect( reply, &NetworkReply::finished, reply, [=]() {
    reply->deleteLater();

    if ( mPackagingStatus == PackagingAbortStatus )
    {
      // no need to emit why we aborted packaging, it is callers responsibility to inform the user
      QgsLogger::debug( QStringLiteral( "Project %1: job creation finished, but project operations are aborted." ).arg( mId ) );
      return;
    }

    QNetworkReply *rawReply = reply->currentRawReply();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QString errorString = QFieldCloudConnection::errorString( rawReply );
      QgsLogger::debug( QStringLiteral( "Project %1: job creation failed with an error: %2" ).arg( mId, errorString ) );
      emit jobFinished( type, errorString );
      return;
    }

    const QJsonObject payload = QJsonDocument::fromJson( rawReply->readAll() ).object();
    const QString jobId = payload.value( QStringLiteral( "id" ) ).toString();

    if ( jobId.isEmpty() )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: job creation finished, but missing id key from the response payload" ).arg( mId ) );
      emit jobFinished( type, tr( "Job creation finished, but the server response is missing required fields: id(string)" ).arg( jobId ) );
      return;
    }

    QgsLogger::debug( QStringLiteral( "Project %1: created job with id `%2`" ).arg( mId ).arg( jobId ) );

    mJobs[type].id = jobId;

    getJobStatus( type );
  } );
}

void QFieldCloudProject::getDeltaStatus()
{
  mDeltaFileUploadStatusString = QString();

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/%2/" ).arg( mId, mDeltaFileId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      mDeltaFileUploadStatus = DeltaErrorStatus;
      // TODO this is oversimplification. e.g. 404 error is when the requested delta file id is not existant
      mDeltaFileUploadStatusString = QFieldCloudConnection::errorString( rawReply );

      emit deltaFileUploadStatusChanged();
      emit deltaFileUploadStatusStringChanged();
      emit networkDeltaStatusChecked();

      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    DeltaListModel deltaListModel( doc );
    if ( !deltaListModel.isValid() )
    {
      mDeltaFileUploadStatus = DeltaErrorStatus;
      mDeltaFileUploadStatusString = deltaListModel.errorString();

      emit deltaFileUploadStatusChanged();
      emit deltaFileUploadStatusStringChanged();
      emit networkDeltaStatusChecked();

      return;
    }

    mDeltaFileUploadStatusString = deltaListModel.errorString();

    if ( !deltaListModel.allHaveFinalStatus() )
    {
      mDeltaFileUploadStatus = DeltaPendingStatus;

      emit deltaFileUploadStatusChanged();
      emit deltaFileUploadStatusStringChanged();
      emit networkDeltaStatusChecked();

      return;
    }

    mDeltaFileUploadStatus = DeltaAppliedStatus;

    emit deltaFileUploadStatusChanged();
    emit deltaFileUploadStatusStringChanged();
    emit networkDeltaStatusChecked();
  } );
}

void QFieldCloudProject::getJobStatus( const JobType type )
{
  if ( mPackagingStatus == PackagingAbortStatus )
  {
    // no need to emit why we aborted packaging, it is callers responsibility to inform the user
    QgsLogger::debug( QStringLiteral( "Project %1: getting job status, but project operations are aborted." ).arg( mId ) );
    return;
  }

  if ( !mJobs.contains( type ) )
  {
    const QString jobTypeName = getJobTypeAsString( type );
    QgsLogger::debug( QStringLiteral( "Project %1: getting job status, but no `%2` job triggered yet." ).arg( mId, jobTypeName ) );
    emit jobFinished( type, tr( "Getting job status, but no `%2` job triggered yet." ).arg( jobTypeName ) );
    return;
  }

  const QString jobId = mJobs[type].id;
  QgsLogger::debug( QStringLiteral( "Project %1, job %2: getting job status..." ).arg( mId, jobId ) );
  NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/jobs/%1/" ).arg( jobId ) );

  connect( reply, &NetworkReply::finished, this, [=]() {
    reply->deleteLater();

    if ( mPackagingStatus == PackagingAbortStatus )
    {
      // no need to emit why we aborted packaging, it is callers responsibility to inform the user
      QgsLogger::debug( QStringLiteral( "Project %1, job %2: getting job status finished, but project operations are aborted." ).arg( mId, jobId ) );
      return;
    }

    QNetworkReply *rawReply = reply->currentRawReply();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QString errorString = QFieldCloudConnection::errorString( rawReply );
      QgsLogger::debug( QStringLiteral( "Project %1, job %2: getting job status finished with an error: %3." ).arg( mId, jobId, errorString ) );
      mJobs[type].status = JobFailedStatus;
      emit jobFinished( type, errorString );
      return;
    }

    const QJsonObject payload = QJsonDocument::fromJson( rawReply->readAll() ).object();
    const QString jobStatusString = payload.value( QStringLiteral( "status" ) ).toString();

    if ( jobStatusString.isEmpty() )
    {
      QgsLogger::debug( QStringLiteral( "Project %1, job %2: getting job status finished, but missing status key from the response payload" ).arg( mId, jobId ) );

      mJobs[type].status = JobFailedStatus;
      emit jobFinished( type, tr( "job(%1) status response does not contain all the expected keys: status(string)" ).arg( jobId ) );
      return;
    }

    mJobs[type].status = getJobStatusFromString( jobStatusString );

    QgsLogger::debug( QStringLiteral( "Project %1, job %2: getting job status finished with `%3`" ).arg( mId, jobId, jobStatusString ) );

    switch ( mJobs[type].status )
    {
      case JobPendingStatus:
      case JobQueuedStatus:
      case JobStartedStatus:
      case JobStoppedStatus:
        // infinite retry, there should be one day, when we can get the status!
        QTimer::singleShot( sDelayBeforeStatusRetry, [=]() {
          getJobStatus( type );
        } );
        break;

      case JobFailedStatus:
        emit jobFinished( type, tr( "Job(%1) finished with a failed status." ).arg( jobId ) );
        return;
      case JobFinishedStatus:
        emit jobFinished( type );
        return;
    }
  } );
}

QFieldCloudProject::JobStatus QFieldCloudProject::getJobStatusFromString( const QString &status )
{
  const QString statusLower = status.toLower();
  if ( statusLower == QStringLiteral( "pending" ) )
    return QFieldCloudProject::JobPendingStatus;
  else if ( statusLower == QStringLiteral( "queued" ) )
    return QFieldCloudProject::JobQueuedStatus;
  else if ( statusLower == QStringLiteral( "started" ) )
    return QFieldCloudProject::JobStartedStatus;
  else if ( statusLower == QStringLiteral( "stopped" ) )
    return QFieldCloudProject::JobStoppedStatus;
  else if ( statusLower == QStringLiteral( "finished" ) )
    return QFieldCloudProject::JobFinishedStatus;
  //cppcheck-suppress duplicateBranch
  else if ( statusLower == QStringLiteral( "failed" ) )
    return QFieldCloudProject::JobFailedStatus;
  else
    // "STATUS_ERROR" or any unknown status is considered an error
    return QFieldCloudProject::JobFailedStatus;
}

QString QFieldCloudProject::getJobTypeAsString( JobType jobType )
{
  switch ( jobType )
  {
    case QFieldCloudProject::JobType::Package:
      return QStringLiteral( "package" );
  }

  return QString();
}

void QFieldCloudProject::refreshModification( LayerObserver *layerObserver )
{
  ProjectModifications oldModifications = mModification;

  if ( layerObserver->deltaFileWrapper()->count() > 0 )
  {
    mModification |= LocalModification;
  }
  else if ( mModification & QFieldCloudProject::LocalModification )
  {
    mModification ^= LocalModification;
  }

  if ( oldModifications != mModification )
  {
    emit modificationChanged();
  }
}

void QFieldCloudProject::refreshData( ProjectRefreshReason reason )
{
  NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/projects/%1/" ).arg( mId ) );
  connect( reply, &NetworkReply::finished, reply, [=]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    reply->deleteLater();

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      emit dataRefreshed( reason, QFieldCloudConnection::errorString( rawReply ) );
      return;
    }

    const QJsonObject projectData = QJsonDocument::fromJson( rawReply->readAll() ).object();
    const QString projectId = projectData.value( "id" ).toString();
    if ( mId != projectId )
      return;

    QgsLogger::debug( QStringLiteral( "Project %1: data refreshed." ).arg( mId ) );

    if (
      !projectData.value( "name" ).isString()
      || !projectData.value( "owner" ).isString()
      || !projectData.value( "description" ).isString()
      || !projectData.value( "user_role" ).isString()
      || !projectData.value( "is_public" ).isBool()
      || !projectData.value( "can_repackage" ).isBool()
      || !projectData.value( "needs_repackaging" ).isBool() )
    {
      emit dataRefreshed( reason, tr( "project(%1) trigger response refresh not contain all the expected keys: name(string), owner(string), description(string), user_role(string), is_public(bool), can_repackage(bool), needs_repackaging(bool)" ).arg( projectId ) );
      return;
    }

    mName = projectData.value( "name" ).toString();
    mOwner = projectData.value( "owner" ).toString();
    mDescription = projectData.value( "description" ).toString();
    mUserRole = projectData.value( "user_role" ).toString();
    mUserRoleOrigin = projectData.value( "user_role_origin" ).toString();
    mIsPrivate = projectData.value( "is_public" ).isUndefined() ? projectData.value( "private" ).toBool() : !projectData.value( "is_public" ).toBool( false );
    mCanRepackage = projectData.value( "can_repackage" ).toBool();
    mNeedsRepackaging = projectData.value( "needs_repackaging" ).toBool();
    mLastRefreshedAt = QDateTime::currentDateTimeUtc();
    mDataLastUpdatedAt = QDateTime::fromString( projectData.value( "data_last_updated_at" ).toString(), Qt::ISODate );
    mIsOutdated = mLastLocalDataLastUpdatedAt.isValid() ? mDataLastUpdatedAt > mLastLocalDataLastUpdatedAt : false;

    emit nameChanged();
    emit ownerChanged();
    emit descriptionChanged();
    emit userRoleChanged();
    emit userRoleOriginChanged();
    emit isPrivateChanged();
    emit canRepackageChanged();
    emit needsRepackagingChanged();
    emit lastRefreshedAtChanged();
    emit dataLastUpdatedAtChanged();
    emit isOutdatedChanged();

    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "name" ), mName );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "owner" ), mOwner );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "description" ), mDescription );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "userRole" ), mUserRole );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "userRoleOrigin" ), mUserRoleOrigin );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "isPrivate" ), mIsPrivate );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "canRepackage" ), mCanRepackage );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "needsRepackaging" ), mNeedsRepackaging );

    emit dataRefreshed( reason );
  } );
}

void QFieldCloudProject::refreshDeltaList()
{
  if ( mDeltaListModel )
  {
    delete mDeltaListModel;
    mDeltaListModel = nullptr;
    emit deltaListModelChanged();
  }

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    mDeltaListModel = new DeltaListModel( doc );
    emit deltaListModelChanged();
  } );
}
void QFieldCloudProject::cancelDownload()
{
  const QStringList fileKeys = mDownloadFileTransfers.keys();
  for ( const QString &fileKey : fileKeys )
  {
    NetworkReply *reply = mDownloadFileTransfers[fileKey].networkReply;

    if ( reply )
      reply->abort();

    mDownloadFileTransfers.remove( fileKey );
  }

  QgsMessageLog::logMessage( QStringLiteral( "Download of project id `%1` aborted" ).arg( mId ) );

  mPackagingStatus = PackagingAbortStatus;
  mPackagingStatusString = tr( "aborted" );
  mErrorStatus = NoErrorStatus;
  mIsPackagingActive = false;
  mIsPackagingFailed = true;
  mStatus = ProjectStatus::Idle;

  emit packagingStatusChanged();
  emit packagingStatusStringChanged();
  emit errorStatusChanged();
  emit isPackagingActiveChanged();
  emit isPackagingFailedChanged();
  emit statusChanged();
}

void QFieldCloudProject::removeLocally()
{
  QDir dir( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId ) );
  if ( dir.exists() )
  {
    dir.removeRecursively();

    mLocalPath = QString();
    mCheckout = RemoteCheckout;
    mModification = NoModification;

    emit localPathChanged();
    emit checkoutChanged();
    emit modificationChanged();
  }

  QSettings().remove( QStringLiteral( "QFieldCloud/projects/%1" ).arg( mId ) );
}

QFieldCloudProject *QFieldCloudProject::fromDetails( const QVariantHash &details, QFieldCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher )
{
  QFieldCloudProject *project = new QFieldCloudProject( details.value( "id" ).toString(), connection, gpkgFlusher );
  project->mIsPrivate = details.value( "private" ).toBool();
  project->mOwner = details.value( "owner" ).toString();
  project->mName = details.value( "name" ).toString();
  project->mDescription = details.value( "description" ).toString();
  project->mUserRole = details.value( "user_role" ).toString();
  project->mUserRoleOrigin = details.value( "user_role_origin" ).toString();
  project->mCheckout = RemoteCheckout;
  project->mStatus = ProjectStatus::Idle;
  project->mDataLastUpdatedAt = QDateTime::fromString( details.value( "data_last_updated_at" ).toString(), Qt::ISODate );
  project->mCanRepackage = details.value( "can_repackage" ).toBool();
  project->mNeedsRepackaging = details.value( "needs_repackaging" ).toBool();

  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "owner" ), project->owner() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "name" ), project->name() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "description" ), project->description() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "userRole" ), project->userRole() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "userRoleOrigin" ), project->userRoleOrigin() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "canRepackage" ), project->canRepackage() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "needsRepackaging" ), project->needsRepackaging() );

  QString username = connection ? connection->username() : QString();
  if ( !username.isEmpty() )
  {
    project->mLocalPath = QFieldCloudUtils::localProjectFilePath( username, project->id() );
    QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, project->id() ) );
    if ( localPath.exists() )
    {
      project->mCheckout = LocalAndRemoteCheckout;
      restoreLocalSettings( project, localPath );
    }
  }

  project->mLastRefreshedAt = QDateTime::currentDateTimeUtc();
  return project;
}

QFieldCloudProject *QFieldCloudProject::fromLocalSettings( const QString &id, QFieldCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher )
{
  const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( id );
  if ( !QSettings().contains( QStringLiteral( "%1/name" ).arg( projectPrefix ) ) )
  {
    return nullptr;
  }

  const QString owner = QFieldCloudUtils::projectSetting( id, QStringLiteral( "owner" ) ).toString();
  const QString name = QFieldCloudUtils::projectSetting( id, QStringLiteral( "name" ) ).toString();
  const QString description = QFieldCloudUtils::projectSetting( id, QStringLiteral( "description" ) ).toString();
  const QString updatedAt = QFieldCloudUtils::projectSetting( id, QStringLiteral( "updatedAt" ) ).toString();
  const QString userRole = QFieldCloudUtils::projectSetting( id, QStringLiteral( "userRole" ) ).toString();
  const QString userRoleOrigin = QFieldCloudUtils::projectSetting( id, QStringLiteral( "userRoleOrigin" ) ).toString();

  QFieldCloudProject *project = new QFieldCloudProject( id, connection, gpkgFlusher );
  project->mIsPrivate = true;
  project->mOwner = owner;
  project->mName = name;
  project->mDescription = description;
  project->mUserRole = userRole;
  project->mUserRoleOrigin = userRoleOrigin;
  project->mCheckout = LocalCheckout;
  project->mStatus = ProjectStatus::Idle;
  project->mDataLastUpdatedAt = QDateTime();
  project->mCanRepackage = false;
  project->mNeedsRepackaging = false;

  QString username = connection ? connection->username() : QString();
  if ( !username.isEmpty() )
  {
    project->mLocalPath = QFieldCloudUtils::localProjectFilePath( username, project->mId );
  }

  QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, project->mId ) );
  restoreLocalSettings( project, localPath );

  return project;
}

void QFieldCloudProject::restoreLocalSettings( QFieldCloudProject *project, const QDir &localPath )
{
  project->mDeltasCount = DeltaFileWrapper( QgsProject::instance(), QStringLiteral( "%1/deltafile.json" ).arg( localPath.absolutePath() ) ).count();
  project->mLastExportId = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastExportId" ) ).toString();
  project->mLastExportedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastExportedAt" ) ).toString();
  project->mLastLocalExportId = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalExportId" ) ).toString();
  project->mLastLocalExportedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalExportedAt" ) ).toString();
  project->mLastLocalPushDeltas = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalPushDeltas" ) ).toString();
  project->mLastLocalDataLastUpdatedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalDataLastUpdatedAt" ) ).toDateTime();
  project->mIsOutdated = project->mDataLastUpdatedAt > project->mLastLocalDataLastUpdatedAt;
  project->mProjectFileIsOutdated = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "projectFileOudated" ), false ).toBool();
  project->mAutoPushEnabled = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "autoPushEnabled" ), false ).toBool();
  project->mAutoPushIntervalMins = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "autoPushIntervalMins" ), 30 ).toInt();

  // generate local export id if not present. Possible reasons for missing localExportId are:
  // - just upgraded QField that introduced the field
  // - the local settings were somehow deleted, but not the project itself
  if ( project->lastLocalExportId().isEmpty() )
  {
    project->mLastLocalExportId = QUuid::createUuid().toString( QUuid::WithoutBraces );
    QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "lastLocalExportId" ), project->lastLocalExportId() );
  }
};
