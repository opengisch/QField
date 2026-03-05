

# File qfieldcloudproject.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfieldcloudproject.cpp**](qfieldcloudproject_8cpp.md)

[Go to the documentation of this file](qfieldcloudproject_8cpp.md)


```C++
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
#include "qfieldcloudconnection.h"
#include "qfieldcloudproject.h"
#include "qfieldcloudutils.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QQmlEngine>
#include <qgsmessagelog.h>

#define MAX_REDIRECTS_ALLOWED 10
#define MAX_PARALLEL_REQUESTS 6
#define CACHE_PROJECT_DATA_SECS 1
#define QFIELDCLOUD_MINIMUM_RANGE_HEADER_LENGTH 1000000

QFieldCloudProject::QFieldCloudProject( const QString &id, QFieldCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher )
  : mId( id ), mCloudConnection( connection ), mGpkgFlusher( gpkgFlusher )
{
  QQmlEngine::setObjectOwnership( this, QQmlEngine::CppOwnership );

  if ( mCloudConnection )
  {
    mUsername = mCloudConnection->username();
  }
}

void QFieldCloudProject::setSharedDatasetsProjectId( const QString &id )
{
  if ( mSharedDatasetsProjectId == id )
    return;

  mSharedDatasetsProjectId = id;
  emit sharedDatasetsProjectIdChanged();
}

void QFieldCloudProject::setIsSharedDatasetsProject( bool isSharedDatasetsProject )
{
  if ( mIsSharedDatasetsProject == isSharedDatasetsProject )
    return;

  mIsSharedDatasetsProject = isSharedDatasetsProject;
  emit isSharedDatasetsProjectChanged();
}

void QFieldCloudProject::setIsPublic( bool isPublic )
{
  if ( mIsPublic == isPublic )
    return;

  mIsPublic = isPublic;
  emit isPublicChanged();
}

void QFieldCloudProject::setIsFeatured( bool isFeatured )
{
  if ( mIsFeatured == isFeatured )
    return;

  mIsFeatured = isFeatured;
  emit isFeaturedChanged();
}

void QFieldCloudProject::setOwner( const QString &owner )
{
  if ( mOwner == owner )
    return;

  mOwner = owner;
  emit ownerChanged();
}

void QFieldCloudProject::setName( const QString &name )
{
  if ( mName == name )
    return;

  mName = name;
  emit nameChanged();
}

void QFieldCloudProject::setDescription( const QString &description )
{
  if ( mDescription == description )
    return;

  mDescription = description;
  emit descriptionChanged();
}

void QFieldCloudProject::setUserRole( const QString &userRole )
{
  if ( mUserRole == userRole )
    return;

  mUserRole = userRole;
  emit userRoleChanged();
}

void QFieldCloudProject::setUserRoleOrigin( const QString &userRoleOrigin )
{
  if ( mUserRoleOrigin == userRoleOrigin )
    return;

  mUserRoleOrigin = userRoleOrigin;
  emit userRoleOriginChanged();
}

void QFieldCloudProject::setCanRepackage( bool canRepackage )
{
  if ( mCanRepackage == canRepackage )
    return;

  mCanRepackage = canRepackage;
  emit canRepackageChanged();
}

void QFieldCloudProject::setNeedsRepackaging( bool needsRepackaging )
{
  if ( mNeedsRepackaging == needsRepackaging )
    return;

  mNeedsRepackaging = needsRepackaging;
  emit needsRepackagingChanged();
}

void QFieldCloudProject::setIsOutdated( bool isOutdated )
{
  if ( mIsOutdated == isOutdated )
    return;

  mIsOutdated = isOutdated;
  emit isOutdatedChanged();
}

void QFieldCloudProject::setIsProjectOutdated( bool isProjectOutdated )
{
  if ( mIsProjectOutdated == isProjectOutdated )
    return;

  mIsProjectOutdated = isProjectOutdated;
  emit isProjectOutdatedChanged();
}

void QFieldCloudProject::setLastRefreshedAt( const QDateTime &lastRefreshedAt )
{
  if ( mLastRefreshedAt == lastRefreshedAt )
    return;

  mLastRefreshedAt = lastRefreshedAt;
  emit lastRefreshedAtChanged();
}

void QFieldCloudProject::setCreatedAt( const QDateTime &createdAt )
{
  if ( mCreatedAt == createdAt )
    return;

  mCreatedAt = createdAt;
  emit createdAtChanged();
}

void QFieldCloudProject::setUpdatedAt( const QDateTime &updatedAt )
{
  if ( mUpdatedAt == updatedAt )
    return;

  mUpdatedAt = updatedAt;
  emit updatedAtChanged();
}

void QFieldCloudProject::setDataLastUpdatedAt( const QDateTime &dataLastUpdatedAt )
{
  if ( mDataLastUpdatedAt == dataLastUpdatedAt )
    return;

  mDataLastUpdatedAt = dataLastUpdatedAt;

  if ( mDataLastUpdatedAt.isValid() && mLastLocalDataLastUpdatedAt.isValid() )
  {
    setIsOutdated( mDataLastUpdatedAt > mLastLocalDataLastUpdatedAt );
  }

  emit dataLastUpdatedAtChanged();
}

void QFieldCloudProject::setRestrictedDataLastUpdatedAt( const QDateTime &restrictedDataLastUpdatedAt )
{
  if ( mRestrictedDataLastUpdatedAt == restrictedDataLastUpdatedAt )
    return;

  mRestrictedDataLastUpdatedAt = restrictedDataLastUpdatedAt;

  if ( mRestrictedDataLastUpdatedAt.isValid() && mLastLocalDataLastUpdatedAt.isValid() )
  {
    setIsProjectOutdated( mRestrictedDataLastUpdatedAt > mLastLocalDataLastUpdatedAt );
  }

  emit restrictedDataLastUpdatedAtChanged();
}

void QFieldCloudProject::setErrorStatus( ProjectErrorStatus errorStatus )
{
  if ( mErrorStatus == errorStatus )
    return;

  mErrorStatus = errorStatus;
  emit errorStatusChanged();
}

void QFieldCloudProject::setCheckout( ProjectCheckouts checkout )
{
  if ( mCheckout == checkout )
    return;

  mCheckout = checkout;
  emit checkoutChanged();
}

void QFieldCloudProject::setStatus( ProjectStatus status )
{
  if ( mStatus == status )
    return;

  mStatus = status;
  emit statusChanged();
}

void QFieldCloudProject::setModification( ProjectModification modification )
{
  if ( mModification == modification )
    return;

  mModification = modification;
  emit modificationChanged();
}

void QFieldCloudProject::setLocalPath( const QString &localPath )
{
  if ( mLocalPath == localPath )
    return;

  mLocalPath = localPath;
  emit localPathChanged();
}

void QFieldCloudProject::setDeltaFileId( const QString &deltaFileId )
{
  if ( mDeltaFileId == deltaFileId )
    return;

  mDeltaFileId = deltaFileId;
  emit deltaFileIdChanged();
}

void QFieldCloudProject::setDeltaFilePushStatus( DeltaFileStatus deltaFilePushStatus )
{
  if ( mDeltaFilePushStatus == deltaFilePushStatus )
    return;

  mDeltaFilePushStatus = deltaFilePushStatus;
  emit deltaFilePushStatusChanged();
}

void QFieldCloudProject::setDeltaFilePushStatusString( const QString &deltaFilePushStatusString )
{
  if ( mDeltaFilePushStatusString == deltaFilePushStatusString )
    return;

  mDeltaFilePushStatusString = deltaFilePushStatusString;
  emit deltaFilePushStatusStringChanged();
}

void QFieldCloudProject::setDeltaLayersToDownload( const QStringList &deltaLayersToDownload )
{
  if ( mDeltaLayersToDownload == deltaLayersToDownload )
    return;

  mDeltaLayersToDownload = deltaLayersToDownload;
  emit deltaLayersToDownloadChanged();
}

void QFieldCloudProject::setIsPackagingActive( bool isPackagingActive )
{
  if ( mIsPackagingActive == isPackagingActive )
    return;

  mIsPackagingActive = isPackagingActive;
  emit isPackagingActiveChanged();
}

void QFieldCloudProject::setIsPackagingFailed( bool isPackagingFailed )
{
  if ( mIsPackagingFailed == isPackagingFailed )
    return;

  mIsPackagingFailed = isPackagingFailed;
  emit isPackagingFailedChanged();
}

void QFieldCloudProject::setPackagingStatus( PackagingStatus packagingStatus )
{
  if ( mPackagingStatus == packagingStatus )
    return;

  mPackagingStatus = packagingStatus;
  emit packagingStatusChanged();
}

void QFieldCloudProject::setPackagingStatusString( const QString &packagingStatusString )
{
  if ( mPackagingStatusString == packagingStatusString )
    return;

  mPackagingStatusString = packagingStatusString;
  emit packagingStatusStringChanged();
}

void QFieldCloudProject::setPackagedLayerErrors( const QStringList &packagedLayerErrors )
{
  if ( mPackagedLayerErrors == packagedLayerErrors )
    return;

  mPackagedLayerErrors = packagedLayerErrors;
  emit packagedLayerErrorsChanged();
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

void QFieldCloudProject::setAttachmentsOnDemandEnabled( bool enabled )
{
  if ( mAttachmentsOnDemandEnabled == enabled )
    return;

  mAttachmentsOnDemandEnabled = enabled;
  emit attachmentsOnDemandEnabledChanged();
}

void QFieldCloudProject::setLastLocalPushDeltas( const QString &lastLocalPushDeltas )
{
  if ( mLastLocalPushDeltas == lastLocalPushDeltas )
    return;

  mLastLocalPushDeltas = lastLocalPushDeltas;
  emit lastLocalPushDeltasChanged();
}

void QFieldCloudProject::setLastLocalExportedAt( const QString &lastLocalExportedAt )
{
  if ( mLastLocalExportedAt == lastLocalExportedAt )
    return;

  mLastLocalExportedAt = lastLocalExportedAt;
  emit lastLocalExportedAtChanged();
}

void QFieldCloudProject::setLastLocalExportId( const QString &lastLocalExportId )
{
  if ( mLastLocalExportId == lastLocalExportId )
    return;

  mLastLocalExportId = lastLocalExportId;
  emit lastLocalExportIdChanged();
}

void QFieldCloudProject::setLastLocalDataLastUpdatedAt( const QDateTime &lastLocalDataLastUpdatedAt )
{
  if ( mLastLocalDataLastUpdatedAt == lastLocalDataLastUpdatedAt )
    return;

  mLastLocalDataLastUpdatedAt = lastLocalDataLastUpdatedAt;

  if ( mDataLastUpdatedAt.isValid() && mLastLocalDataLastUpdatedAt.isValid() )
  {
    setIsOutdated( mDataLastUpdatedAt > mLastLocalDataLastUpdatedAt );
  }

  emit lastLocalDataLastUpdatedAtChanged();
}

void QFieldCloudProject::setLastLocalRestrictedDataLastUpdatedAt( const QDateTime &lastLocalRestrictedDataLastUpdatedAt )
{
  if ( mLastLocalRestrictedDataLastUpdatedAt == lastLocalRestrictedDataLastUpdatedAt )
    return;

  mLastLocalRestrictedDataLastUpdatedAt = lastLocalRestrictedDataLastUpdatedAt;

  if ( mRestrictedDataLastUpdatedAt.isValid() && mLastLocalRestrictedDataLastUpdatedAt.isValid() )
  {
    setIsOutdated( mRestrictedDataLastUpdatedAt > mLastLocalRestrictedDataLastUpdatedAt );
  }

  emit lastLocalRestrictedDataLastUpdatedAtChanged();
}

void QFieldCloudProject::setThumbnailPath( const QString &thumbnailPath )
{
  if ( mThumbnailPath == thumbnailPath )
    return;

  mThumbnailPath = thumbnailPath;
  emit thumbnailPathChanged();
}

void QFieldCloudProject::downloadThumbnail()
{
  QgsLogger::debug( QStringLiteral( "Project %1: thumbnail download initiated." ).arg( mId ) );

  if ( !mCloudConnection )
    return;

  QNetworkRequest request;
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  mCloudConnection->setAuthenticationDetails( request );

  NetworkReply *reply = mCloudConnection->get( request, QStringLiteral( "/api/v1/files/thumbnails/%1/" ).arg( mId ) );
  connect( reply, &NetworkReply::finished, reply, [this, reply]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() == QNetworkReply::NoError )
    {
      QString imageExtension( "PNG" );
      const QString contentType = rawReply->header( QNetworkRequest::ContentTypeHeader ).toString();
      if ( !contentType.isEmpty() )
      {
        if ( contentType.contains( QStringLiteral( "image/png" ) ) )
        {
          imageExtension = QStringLiteral( "PNG" );
        }
        else if ( contentType.contains( QStringLiteral( "image/jpg" ) ) || contentType.contains( QStringLiteral( "image/jpeg" ) ) )
        {
          imageExtension = QStringLiteral( "JPG" );
        }
        else if ( contentType.contains( QStringLiteral( "image/webp" ) ) )
        {
          imageExtension = QStringLiteral( "WEBP" );
        }
      }
      QTemporaryFile file( QString( "%1/XXXXXX.%2" ).arg( QDir::tempPath(), imageExtension ) );
      file.setAutoRemove( false );
      file.open();
      file.write( rawReply->readAll() );
      file.close();
      setThumbnailPath( file.fileName() );
    };
  } );
}

void QFieldCloudProject::downloadAttachment( const QString &fileName )
{
  if ( !mAttachmentsFileTransfers.contains( fileName ) )
  {
    mAttachmentsFileTransfers.insert( fileName, FileTransfer( fileName, 0, mId, FileUtils::fileEtag( fileName ) ) );

    NetworkReply *reply = downloadFile( mId, fileName, true, true );
    QTemporaryFile *file = new QTemporaryFile( reply );
    file->setAutoRemove( false );
    if ( !file->open() )
    {
      emit downloadAttachmentFinished( fileName, tr( "Failed to open temporary file for `%1`, reason:\n%2" ).arg( fileName ).arg( file->errorString() ) );
      mAttachmentsFileTransfers.remove( fileName );
      return;
    }

    mAttachmentsFileTransfers[fileName].tmpFile = file->fileName();
    mAttachmentsFileTransfers[fileName].networkReply = reply;
    downloadAttachmentConnections( fileName );
  }
}

void QFieldCloudProject::downloadAttachmentConnections( const QString &fileKey )
{
  if ( !mAttachmentsFileTransfers.contains( fileKey ) )
  {
    Q_ASSERT( false );
    return;
  }

  NetworkReply *reply = mAttachmentsFileTransfers[fileKey].networkReply;
  if ( !reply )
  {
    Q_ASSERT( false );
    return;
  }

  connect( reply, &NetworkReply::downloadProgress, reply, [this, reply, fileKey]( qint64 bytesReceived, qint64 bytesTotal ) {
    QNetworkReply *rawReply = reply->currentRawReply();
    if ( !rawReply )
    {
      return;
    }

    QString errorMessage;
    bool hasError = false;

    QFile file( mAttachmentsFileTransfers[fileKey].tmpFile );
    if ( file.open( QIODevice::WriteOnly | QIODevice::Append ) )
    {
      file.write( rawReply->readAll() );
      if ( file.error() != QFile::NoError )
      {
        hasError = true;
        errorMessage = tr( "File system error. Failed to write attachment to temporary location `%1`." ).arg( mAttachmentsFileTransfers[fileKey].tmpFile );
      }

      file.close();
    }
    else
    {
      hasError = true;
      errorMessage = tr( "File system error. Failed to open attachment for writing on temporary `%1`." ).arg( mAttachmentsFileTransfers[fileKey].tmpFile );
    }

    if ( hasError )
    {
      rawReply->abort();
      emit downloadAttachmentFinished( fileKey, errorMessage );
      mAttachmentsFileTransfers.remove( fileKey );
      return;
    }
  } );

  connect( reply, &NetworkReply::finished, reply, [this, reply, fileKey]() {
    QNetworkReply *rawReply = reply->currentRawReply();
    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    QString errorMessage;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      errorMessage = tr( "Network error. Failed to download attachment `%1`." ).arg( mAttachmentsFileTransfers[fileKey].fileName );
      rawReply->abort();
      emit downloadAttachmentFinished( fileKey, errorMessage );
      mAttachmentsFileTransfers.remove( fileKey );
      return;
    }

    QFileInfo fileInfo( mAttachmentsFileTransfers[fileKey].fileName );
    QDir dir( QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mAttachmentsFileTransfers[fileKey].projectId, fileInfo.path() ) );

    if ( !dir.exists() && !dir.mkpath( QStringLiteral( "." ) ) )
    {
      errorMessage = QStringLiteral( "Failed to create attachment directory at `%1`" ).arg( dir.path() );
      rawReply->abort();
      emit downloadAttachmentFinished( mAttachmentsFileTransfers[fileKey].fileName, errorMessage );
      mAttachmentsFileTransfers.remove( fileKey );
      return;
    }

    QFile file( mAttachmentsFileTransfers[fileKey].tmpFile );
    const QString destinationFileName( QDir::cleanPath( dir.filePath( fileInfo.fileName() ) ) );

    // if the file already exists, we need to delete it first, as QT does not support overwriting
    // NOTE: it is possible that someone creates the file in the meantime between this and the next if statement
    if ( QFile::exists( destinationFileName ) && !file.remove( destinationFileName ) )
    {
      errorMessage = QStringLiteral( "Failed to remove pre-existing attachment before overwriting stored at `%1`, reason:\n%2" ).arg( destinationFileName ).arg( file.errorString() );
      rawReply->abort();
      emit downloadAttachmentFinished( mAttachmentsFileTransfers[fileKey].fileName, errorMessage );
      mAttachmentsFileTransfers.remove( fileKey );
      return;
    }

    if ( !file.copy( destinationFileName ) )
    {
      errorMessage = QStringLiteral( "Failed to write downloaded attachment stored at `%1`, reason:\n%2" ).arg( destinationFileName ).arg( file.errorString() );
      rawReply->abort();
      emit downloadAttachmentFinished( mAttachmentsFileTransfers[fileKey].fileName, errorMessage );
      mAttachmentsFileTransfers.remove( fileKey );
      return;
    }

    if ( !file.remove() )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove temporary attachment `%1`" ).arg( destinationFileName ) );
    }

    emit downloadAttachmentFinished( mAttachmentsFileTransfers[fileKey].fileName );
    mAttachmentsFileTransfers.remove( fileKey );
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

  mDownloadFileTransfers.clear();
  mDownloadFilesFinished = 0;
  mDownloadFilesFailed = 0;
  mDownloadBytesTotal = 0;
  mDownloadBytesReceived = 0;
  mDownloadProgress = 0;
  emit downloadBytesTotalChanged();
  emit downloadBytesReceivedChanged();
  emit downloadProgressChanged();

  setPackagingStatus( PackagingUnstartedStatus );
  setPackagingStatusString( QString() );
  setPackagedLayerErrors( QStringList() );
  setStatus( ProjectStatus::Downloading );
  setErrorStatus( NoErrorStatus );
  setModification( NoModification );

  auto repackageIfNeededAndThenDownload = [this]() {
    if ( mNeedsRepackaging )
    {
      QgsLogger::debug( QStringLiteral( "Project %1: repackaging triggered." ).arg( mId ) );

      setPackagingStatus( PackagingBusyStatus );
      startJob( JobType::Package );

      QObject *tempProjectJobFinishedParent = new QObject( this ); // we need this to unsubscribe
      connect( this, &QFieldCloudProject::jobFinished, tempProjectJobFinishedParent, [this, tempProjectJobFinishedParent]( const JobType type, const QString &errorString ) {
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

          setPackagingStatus( PackagingErrorStatus );
          setPackagingStatusString( errorString );

          emit downloadFinished( tr( "Packaging job finished unsuccessfully for `%1`. %2" ).arg( mName ).arg( errorString ) );
          return;
        }

        setPackagingStatus( PackagingFinishedStatus );
        setPackagingStatusString( QString() );

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
    connect( this, &QFieldCloudProject::dataRefreshed, tempProjectRefreshParent, [this, tempProjectRefreshParent, repackageIfNeededAndThenDownload]( const ProjectRefreshReason reason, const QString &error ) {
      if ( reason != ProjectRefreshReason::Package )
      {
        QgsLogger::critical( QStringLiteral( "Project %1: unexpected job type, expected %2 but %3 received." ).arg( mId ).arg( static_cast<int>( ProjectRefreshReason::Package ) ).arg( static_cast<int>( reason ) ) );
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
  connect( this, &QFieldCloudProject::downloadFinished, tempProjectDownloadFinishedParent, [this, tempProjectDownloadFinishedParent]( const QString &error ) {
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
      setErrorStatus( DownloadErrorStatus );
      QgsMessageLog::logMessage( QStringLiteral( "Downloading project `%1` finished with an error: %2" ).arg( mId ).arg( error ) );
    }
    else
    {
      setErrorStatus( NoErrorStatus );
    }

    setStatus( ProjectStatus::Idle );

    emit downloaded( mName, error );
  } );
}

void QFieldCloudProject::download()
{
  QVariantMap params;
  params.insert( "skip_metadata", "1" );
  NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/packages/%1/latest/" ).arg( mId ), params );

  connect( reply, &NetworkReply::finished, reply, [this, reply]() {
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

      const qint64 fileSize = fileObject.value( QStringLiteral( "size" ) ).toInteger();
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


      if ( mAttachmentsOnDemandEnabled && fileObject.value( QStringLiteral( "is_attachment" ) ).toBool() )
      {
        if ( !localEtag.isEmpty() && cloudEtag != localEtag )
        {
          // The cloud attachment has changed, remove locally to trigger a new download locally
          QFile::remove( projectFileName );
        }
        continue;
      }

      if ( cloudEtag == localEtag )
      {
        continue;
      }

      prepareDownloadTransfer( mId, fileName, fileSize, cloudEtag );
    }

    emit downloadBytesTotalChanged();

    const QJsonObject layers = payload.value( QStringLiteral( "layers" ) ).toObject();
    QStringList localizedDatasetsFileNames;
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
          const QString localizedDatasetFileName = layer.value( "filename" ).toString();
          if ( !localizedDatasetFileName.isEmpty() )
          {
            localizedDatasetsFileNames << localizedDatasetFileName;
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

    if ( !localizedDatasetsFileNames.isEmpty() && !mSharedDatasetsProjectId.isEmpty() )
    {
      NetworkReply *localizedDatasetsReply = mCloudConnection->get( QStringLiteral( "/api/v1/files/%1/" ).arg( mSharedDatasetsProjectId ) );
      connect( localizedDatasetsReply, &NetworkReply::finished, localizedDatasetsReply, [this, localizedDatasetsReply, localizedDatasetsFileNames]() {
        QNetworkReply *localizedDatasetsRawReply = localizedDatasetsReply->currentRawReply();
        localizedDatasetsReply->deleteLater();

        if ( localizedDatasetsRawReply->error() == QNetworkReply::NoError )
        {
          const QJsonArray files = QJsonDocument::fromJson( localizedDatasetsRawReply->readAll() ).array();
          for ( const QJsonValue fileValue : files )
          {
            const QJsonObject fileObject = fileValue.toObject();
            const QString fileName = fileObject.value( QStringLiteral( "name" ) ).toString();
            if ( localizedDatasetsFileNames.contains( fileName ) )
            {
              const qint64 fileSize = fileObject.value( QStringLiteral( "size" ) ).toInteger();
              const QString absoluteFileName = QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mSharedDatasetsProjectId, fileName );
              // NOTE the cloud API is giving the false impression that the file keys `md5sum` is having a MD5 or another checksum.
              // This actually is an Object Storage (S3) implementation specific ETag.
              const QString cloudEtag = fileObject.value( QStringLiteral( "md5sum" ) ).toString();
              const QString localEtag = FileUtils::fileEtag( absoluteFileName );

              if (
                !fileObject.value( QStringLiteral( "size" ) ).isDouble()
                || fileName.isEmpty()
                || cloudEtag.isEmpty() )
              {
                QgsLogger::debug( QStringLiteral( "Project %1: package in \"files\" list does not contain the expected fields: size(int), name(string), md5sum(string)" ).arg( mSharedDatasetsProjectId ) );
                emit downloadFinished( tr( "Latest package data structure error." ) );
                return;
              }

              if ( cloudEtag == localEtag )
              {
                continue;
              }

              prepareDownloadTransfer( mSharedDatasetsProjectId, fileName, fileSize, cloudEtag );
            }
          }
          emit downloadBytesTotalChanged();

          QgsLogger::debug( QStringLiteral( "Project %1: packaged files to download - %2 files, namely: %3" ).arg( mId ).arg( mDownloadFileTransfers.count() ).arg( mDownloadFileTransfers.keys().join( ", " ) ) );
        }
        else if ( localizedDatasetsRawReply->error() == QNetworkReply::ContentAccessDenied )
        {
          QgsLogger::debug( QStringLiteral( "Project %1: access denied to shared datasets" ).arg( mId ) );
        }
        else
        {
          QgsLogger::debug( QStringLiteral( "Project %1: failed to get latest shared datasets data. %2" ).arg( mId, QFieldCloudConnection::errorString( localizedDatasetsRawReply ) ) );
          emit downloadFinished( tr( "Failed to get latest package data." ) );
          return;
        }

        updateActiveFilesToDownload();
        downloadFiles();
      } );
    }
    else
    {
      QgsLogger::debug( QStringLiteral( "Project %1: packaged files to download - %2 files, namely: %3" ).arg( mId ).arg( mDownloadFileTransfers.count() ).arg( mDownloadFileTransfers.keys().join( ", " ) ) );

      updateActiveFilesToDownload();
      downloadFiles();
    }
  } );
}

void QFieldCloudProject::prepareDownloadTransfer( const QString &projectId, const QString &fileName, qint64 fileSize, const QString &cloudEtag )
{
  const QString fileKey = QStringLiteral( "%1/%2" ).arg( projectId, fileName );
  const QString projectDir = QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, projectId );

  FileTransfer transfer( fileName, fileSize, projectId, cloudEtag );
  transfer.partialFilePath = QDir( projectDir ).filePath( QStringLiteral( "%1.%2.part" ).arg( fileName, cloudEtag ) );

  mDownloadFileTransfers.insert( fileKey, transfer );

  // Remove old .part files with different etag
  QDir dir( projectDir );
  QStringList partFiles = dir.entryList( QStringList() << QStringLiteral( "%1.*.part" ).arg( fileName ), QDir::Files );
  for ( const QString &partFile : partFiles )
  {
    if ( partFile.startsWith( fileName + "." ) && !partFile.endsWith( QStringLiteral( "%1.part" ).arg( cloudEtag ) ) )
    {
      QgsLogger::debug( QStringLiteral( "Deleting outdated partial file: %1" ).arg( partFile ) );
      QFile::remove( dir.filePath( partFile ) );
    }
  }
  mDownloadBytesTotal += std::max( fileSize, static_cast<qint64>( 0 ) );
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
    setStatus( ProjectStatus::Idle );
    mDownloadProgress = 1;

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

    FileTransfer &fileTransfer = mDownloadFileTransfers[fileKey];

    NetworkReply *reply = downloadFile( fileTransfer.projectId, fileTransfer.fileName, fileTransfer.projectId == mId );

    const QDir partialDir = QFileInfo( fileTransfer.partialFilePath ).dir();
    if ( !partialDir.exists() )
      partialDir.mkpath( "." );

    if ( reply )
    {
      fileTransfer.networkReply = reply;
      downloadFileConnections( fileKey );
    }
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

  connect( reply, &NetworkReply::redirected, reply, [this, reply, fileKey]( const QUrl &url ) {
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

  connect( reply, &NetworkReply::downloadProgress, reply, [this, reply, fileKey]( qint64 bytesReceived, qint64 bytesTotal ) {
    QNetworkReply *rawReply = reply->currentRawReply();
    if ( !rawReply )
    {
      return;
    }

    const QString partialFileName = mDownloadFileTransfers[fileKey].partialFilePath;
    QFile file( partialFileName );
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
        errorMessage = tr( "File system error. Failed to write to partial file `%1`." ).arg( partialFileName );
      }

      file.close();
    }
    else
    {
      hasError = true;
      errorMessageDetail = file.errorString();
      errorMessage = tr( "File system error. Failed to open partial file `%1`." ).arg( partialFileName );
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

    mDownloadProgress = std::clamp( ( static_cast<double>( mDownloadBytesReceived ) / std::max( mDownloadBytesTotal, static_cast<qint64>( 1 ) ) ), 0., 1. );

    emit downloadBytesReceivedChanged();
    emit downloadProgressChanged();
  } );

  connect( reply, &NetworkReply::finished, reply, [this, reply, fileKey, fileKeys]() {
    if ( mPackagingStatus == PackagingAbortStatus )
    {
      return;
    }

    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

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
      const int httpStatus = rawReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
      if ( httpStatus == 416 && mDownloadFileTransfers[fileKey].retryCount < 3 )
      {
        mDownloadFileTransfers[fileKey].resumableDownload = false;
        mDownloadFileTransfers[fileKey].retryCount++;

        NetworkReply *newReply = downloadFile(
          mDownloadFileTransfers[fileKey].projectId,
          mDownloadFileTransfers[fileKey].fileName,
          true,
          true );

        if ( newReply )
        {
          mDownloadFileTransfers[fileKey].networkReply = newReply;
          newReply->setParent( reply );
          downloadFileConnections( fileKey );
        }

        reply->abort();
        return;
      }

      hasError = true;
      errorMessageDetail = QFieldCloudConnection::errorString( rawReply );
      errorMessage = tr( "Network error. Failed to download file `%1`." ).arg( fileKey );
    }

    if ( !hasError )
    {
      mDownloadBytesReceived -= mDownloadFileTransfers[fileKey].bytesTransferred;
      mDownloadBytesReceived += mDownloadFileTransfers[fileKey].bytesTotal;
      mDownloadProgress = std::clamp( ( static_cast<double>( mDownloadBytesReceived ) / std::max( mDownloadBytesTotal, static_cast<qint64>( 1 ) ) ), 0., 1. );

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
      downloadFilesCompleted();
    }
    else
    {
      downloadFiles();
    }
  } );
}

void QFieldCloudProject::downloadFilesCompleted()
{
  QgsLogger::debug( QStringLiteral( "Project %1: All files downloaded." ).arg( mId ) );
  Q_ASSERT( mActiveFilesToDownload.size() == 0 );

  if ( !mDeltaFileWrapper )
  {
    setupDeltaFileWrapper();
  }

  const QDir projectPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId ) );
  const bool currentProjectReloadNeeded = QgsProject::instance()->homePath().startsWith( projectPath.absolutePath() );
  QStringList gpkgFileNames;
  if ( currentProjectReloadNeeded )
  {
    // we need to close the project to safely flush the gpkg files and avoid file lock on Windows
    QDirIterator it( projectPath.absolutePath(), { QStringLiteral( "*.gpkg" ), QStringLiteral( "*.sqlite" ) }, QDir::Filter::Files, QDirIterator::Subdirectories );
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

  setStatus( ProjectStatus::Idle );
  setErrorStatus( NoErrorStatus );
  setCheckout( ProjectCheckout::LocalAndRemoteCheckout );
  setLocalPath( QFieldCloudUtils::localProjectFilePath( mUsername, mId ) );
  setLastLocalExportedAt( QDateTime::currentDateTimeUtc().toString( Qt::ISODate ) );
  setLastLocalExportId( QUuid::createUuid().toString( QUuid::WithoutBraces ) );
  setLastLocalDataLastUpdatedAt( mDataLastUpdatedAt );
  setLastLocalRestrictedDataLastUpdatedAt( mRestrictedDataLastUpdatedAt );
  setIsOutdated( false );
  setIsProjectOutdated( false );

  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastExportedAt" ), mLastExportedAt );
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastExportId" ), mLastExportId );
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalExportedAt" ), mLastLocalExportedAt );
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalExportId" ), mLastLocalExportId );
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalDataLastUpdatedAt" ), mLastLocalDataLastUpdatedAt );
  QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalRestrictedDataLastUpdatedAt" ), mLastLocalRestrictedDataLastUpdatedAt );

  emit downloadFinished();
}

NetworkReply *QFieldCloudProject::downloadFile( const QString &projectId, const QString &fileName, bool fromLatestPackage, bool autoRedirect )
{
  QNetworkRequest request;
  if ( autoRedirect )
  {
    request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  }
  else
  {
    request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::UserVerifiedRedirectPolicy );
  }

  // Never rely on cache to insure latest package files are properly downloaded
  request.setAttribute( QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork );

  mCloudConnection->setAuthenticationDetails( request );

  const QString fileKey = QStringLiteral( "%1/%2" ).arg( projectId, fileName );
  const FileTransfer &fileTransfer = mDownloadFileTransfers[fileKey];
  QFile partialFile( fileTransfer.partialFilePath );

  if ( partialFile.exists() )
  {
    qint64 partialSize = partialFile.size();
    if ( !fileTransfer.resumableDownload || partialSize < QFIELDCLOUD_MINIMUM_RANGE_HEADER_LENGTH || partialSize > fileTransfer.bytesTotal || ( partialSize == fileTransfer.bytesTotal && fileTransfer.etag != FileUtils::fileEtag( fileTransfer.partialFilePath ) ) )
    {
      // Invalid or dirty file; delete and re-download
      partialFile.remove();
    }
    else if ( partialSize < fileTransfer.bytesTotal )
    {
      // Partial file found; resume download using Range header
      request.setRawHeader( "Range", "bytes=" + QByteArray::number( partialSize ) + "-" );
      mDownloadBytesReceived += partialSize;
    }
    else if ( partialSize == fileTransfer.bytesTotal )
    {
      // File already fully downloaded and valid; skip download
      mDownloadBytesReceived += partialSize;
      mDownloadFilesFinished++;
      mActiveFilesToDownload.removeOne( fileKey );

      if ( mActiveFilesToDownload.size() == 0 )
      {
        downloadFilesCompleted();
      }
      return nullptr;
    }
  }

  const QString urlPath = fromLatestPackage ? QStringLiteral( "/api/v1/packages/%1/latest/files/%2/" ).arg( projectId, fileName ) : QStringLiteral( "/api/v1/files/%1/%2/" ).arg( projectId, fileName );
  return mCloudConnection->get( request, urlPath );
}

bool QFieldCloudProject::moveDownloadedFilesToPermanentStorage()
{
  bool hasError = false;
  const QStringList fileKeys = mDownloadFileTransfers.keys();

  for ( const QString &fileKey : fileKeys )
  {
    const FileTransfer &fileTransfer = mDownloadFileTransfers[fileKey];
    const QFileInfo origInfo( fileTransfer.fileName );

    const QDir targetDir( QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, fileTransfer.projectId, origInfo.path() ) );

    if ( !targetDir.exists() && !targetDir.mkpath( "." ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to create directory `%1`" ).arg( targetDir.absolutePath() ) );
      continue;
    }

    const QString finalFilePath = targetDir.filePath( origInfo.fileName() );

    // If the file already exists, we need to delete it first, as QT does not support overwriting
    // NOTE: it is possible that someone creates the file in the meantime between this and the next if statement
    if ( QFile::exists( finalFilePath ) && !QFile::remove( finalFilePath ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove existing file `%1`" ).arg( finalFilePath ) );
      continue;
    }

    // Rename the .part file to the final file name
    if ( !QFile::rename( fileTransfer.partialFilePath, finalFilePath ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to rename `%1` to `%2`." ).arg( fileTransfer.partialFilePath, finalFilePath ) );
    }
    else
    {
      QgsLogger::debug( QStringLiteral( "Moved downloaded file `%1` to `%2`" ).arg( fileTransfer.partialFilePath, finalFilePath ) );
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

void QFieldCloudProject::push( bool shouldDownloadUpdates )
{
  if ( mStatus != ProjectStatus::Idle )
  {
    return;
  }

  if ( shouldDownloadUpdates && mDeltaFileWrapper->count() == 0 )
  {
    setStatus( ProjectStatus::Idle );
    packageAndDownload();
    return;
  }

  if ( !( mModification & LocalModification ) )
  {
    return;
  }

  if ( !mDeltaFileWrapper->toFile() )
  {
    return;
  }

  if ( mDeltaFileWrapper->hasError() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "The delta file has an error: %1" ).arg( mDeltaFileWrapper->errorString() ) );
    return;
  }

  mDeltaFileWrapper->setIsPushing( true );

  setStatus( ProjectStatus::Pushing );
  setDeltaFileId( mDeltaFileWrapper->id() );
  setDeltaFilePushStatus( DeltaLocalStatus );
  setDeltaFilePushStatusString( QString() );
  mPushDeltaProgress = 0.0;

  emit pushDeltaProgressChanged();

  refreshModification();

  // //////////
  // prepare attachment files to be uploaded
  // //////////

  const QFileInfo projectInfo( QFieldCloudUtils::localProjectFilePath( mUsername, mId ) );
  const QDir projectDir( projectInfo.absolutePath() );
  const QStringList attachmentFileNames = mDeltaFileWrapper->attachmentFileNames().keys();

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

    // ? should we also check the checksums of the files being uploaded? they are available at deltaFile->attachmentFileNames()->values()
    QFieldCloudUtils::addPendingAttachments( mUsername, mId, { absoluteFilePath } );
  }

  QString deltaFileToUpload = mDeltaFileWrapper->toFileForPush();

  if ( deltaFileToUpload.isEmpty() )
  {
    mDeltaFileWrapper->setIsPushing( false );
    setStatus( ProjectStatus::Idle );
    return;
  }

  // //////////
  // 1) upload the deltas
  // //////////
  NetworkReply *deltasCloudReply = mCloudConnection->post( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mId ), QVariantMap(), QStringList( { deltaFileToUpload } ) );

  Q_ASSERT( deltasCloudReply );

  connect( deltasCloudReply, &NetworkReply::uploadProgress, this, [this]( qint64 bytesSent, qint64 bytesTotal ) {
    mPushDeltaProgress = std::clamp( ( static_cast<double>( bytesSent ) / bytesTotal ), 0., 1. );
    emit pushDeltaProgressChanged();
  } );

  connect( deltasCloudReply, &NetworkReply::finished, this, [this, deltasCloudReply]() {
    QNetworkReply *deltasReply = deltasCloudReply->currentRawReply();
    deltasCloudReply->deleteLater();

    Q_ASSERT( deltasCloudReply->isFinished() );
    Q_ASSERT( deltasReply );

    // if there is an error, cannot continue sync
    if ( deltasReply->error() != QNetworkReply::NoError )
    {
      setDeltaFilePushStatusString( QFieldCloudConnection::errorString( deltasReply ) );
      // TODO check why exactly we failed
      // maybe the project does not exist, then create it?
      QgsMessageLog::logMessage( QStringLiteral( "Failed to upload delta file, reason:\n%1\n%2" ).arg( deltasReply->errorString(), mDeltaFilePushStatusString ) );

      mDeltaFileWrapper->setIsPushing( false );

      cancelPush();
      return;
    }

    mPushDeltaProgress = 1.0;
    setDeltaFilePushStatus( DeltaPendingStatus );
    setDeltaLayersToDownload( mDeltaFileWrapper->deltaLayerIds() );

    emit pushDeltaProgressChanged();

    emit networkDeltaPushed();
  } );


  // //////////
  // 2) delta successfully uploaded
  // //////////
  QObject *networkDeltaPushedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProject::networkDeltaPushed, networkDeltaPushedParent, [this, networkDeltaPushedParent, shouldDownloadUpdates]() {
    delete networkDeltaPushedParent;

    if ( shouldDownloadUpdates )
    {
      getDeltaStatus();
    }
    else
    {
      mModification ^= LocalModification;
      emit modificationChanged();

      setStatus( ProjectStatus::Idle );
      setLastLocalPushDeltas( QDateTime::currentDateTimeUtc().toString( Qt::ISODate ) );

      if ( !isOutdated() )
      {
        // If we are not in an outdated state, avoid falling into outdated state due to our own data change
        setLastLocalDataLastUpdatedAt( QDateTime::currentDateTimeUtc().addSecs( 60 * 2 ) );
        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalDataLastUpdatedAt" ), mLastLocalDataLastUpdatedAt );
      }

      QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalPushDeltas" ), mLastLocalPushDeltas );

      mDeltaFileWrapper->reset();
      mDeltaFileWrapper->resetId();
      mDeltaFileWrapper->setIsPushing( false );

      if ( !mDeltaFileWrapper->toFile() )
      {
        QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file after delta push. %1" ).arg( mDeltaFileWrapper->errorString() ) );
      }

      emit pushFinished( false );

      refreshData( ProjectRefreshReason::DeltaPushed );
    }
  } );

  // //////////
  // 3) new delta status received. Never give up to get a successful status.
  // //////////
  QObject *networkDeltaStatusCheckedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProject::networkDeltaStatusChecked, networkDeltaStatusCheckedParent, [this, networkDeltaStatusCheckedParent, shouldDownloadUpdates]() {
    switch ( mDeltaFilePushStatus )
    {
      case DeltaLocalStatus:
        // delta file should be already sent!!!
        Q_ASSERT( 0 );
        [[fallthrough]];

      case DeltaPendingStatus:
      case DeltaBusyStatus:
        // infinite retry, there should be one day, when we can get the status!
        QTimer::singleShot( sDelayBeforeStatusRetry, [this]() { getDeltaStatus(); } );
        break;

      case DeltaErrorStatus:
        delete networkDeltaStatusCheckedParent;
        mDeltaFileWrapper->resetId();
        mDeltaFileWrapper->setIsPushing( false );

        if ( !mDeltaFileWrapper->toFile() )
        {
          QgsMessageLog::logMessage( QStringLiteral( "Failed update committed delta file." ) );
        }

        cancelPush();
        return;

      case DeltaConflictStatus:
      case DeltaNotAppliedStatus:
      case DeltaAppliedStatus:
        delete networkDeltaStatusCheckedParent;

        mDeltaFileWrapper->reset();
        mDeltaFileWrapper->resetId();
        mDeltaFileWrapper->setIsPushing( false );

        if ( !mDeltaFileWrapper->toFile() )
        {
          QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file. %1" ).arg( mDeltaFileWrapper->errorString() ) );
        }

        mModification ^= LocalModification;
        mModification |= RemoteModification;
        emit modificationChanged();
        setStatus( ProjectStatus::Idle );
        setLastLocalPushDeltas( QDateTime::currentDateTimeUtc().toString( Qt::ISODate ) );

        QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "lastLocalPushDeltas" ), mLastLocalPushDeltas );

        // download the updated files, so the files are for sure the same on the client and on the server
        if ( shouldDownloadUpdates )
        {
          emit pushFinished( true, QString() );
          packageAndDownload();
        }
        else
        {
          emit statusChanged();
          emit pushFinished( false, QString() );

          refreshData( ProjectRefreshReason::DeltaPushed );
        }
    }
  } );
}

void QFieldCloudProject::cancelPush()
{
  setStatus( ProjectStatus::Idle );
  setErrorStatus( PushErrorStatus );

  emit pushFinished( false, mDeltaFilePushStatusString );

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

  connect( reply, &NetworkReply::finished, reply, [this, reply, type]() {
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
  setDeltaFilePushStatusString( QString() );

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/%2/" ).arg( mId, mDeltaFileId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [this, deltaStatusReply]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      setDeltaFilePushStatus( DeltaErrorStatus );
      // TODO this is oversimplification. e.g. 404 error is when the requested delta file id is not existant
      setDeltaFilePushStatusString( QFieldCloudConnection::errorString( rawReply ) );

      emit networkDeltaStatusChecked();
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    DeltaListModel deltaListModel( doc );
    if ( !deltaListModel.isValid() )
    {
      setDeltaFilePushStatus( DeltaErrorStatus );
      setDeltaFilePushStatusString( deltaListModel.errorString() );

      emit networkDeltaStatusChecked();
      return;
    }

    setDeltaFilePushStatusString( QString() );

    if ( !deltaListModel.allHaveFinalStatus() )
    {
      setDeltaFilePushStatus( DeltaPendingStatus );

      emit networkDeltaStatusChecked();
      return;
    }

    setDeltaFilePushStatus( DeltaAppliedStatus );

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

  connect( reply, &NetworkReply::finished, this, [this, reply, type, jobId]() {
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
        QTimer::singleShot( sDelayBeforeStatusRetry, [this, type]() {
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

void QFieldCloudProject::refreshModification()
{
  ProjectModifications oldModifications = mModification;

  if ( mDeltaFileWrapper && mDeltaFileWrapper->count() > 0 )
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
  connect( reply, &NetworkReply::finished, reply, [this, reply, reason]() {
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

    setName( projectData.value( "name" ).toString() );
    setOwner( projectData.value( "owner" ).toString() );
    setDescription( projectData.value( "description" ).toString() );
    setUserRole( projectData.value( "user_role" ).toString() );
    setUserRoleOrigin( mUserRoleOrigin = projectData.value( "user_role_origin" ).toString() );
    setCreatedAt( QDateTime::fromString( projectData.value( "created_at" ).toString(), Qt::ISODate ) );
    setUpdatedAt( QDateTime::fromString( projectData.value( "updated_at" ).toString(), Qt::ISODate ) );
    setIsPublic( projectData.value( "is_public" ).toBool() );
    setIsFeatured( projectData.value( "is_featured" ).toBool() );
    setCanRepackage( projectData.value( "can_repackage" ).toBool() );
    setNeedsRepackaging( projectData.value( "needs_repackaging" ).toBool() );
    setLastRefreshedAt( QDateTime::currentDateTimeUtc() );
    setDataLastUpdatedAt( QDateTime::fromString( projectData.value( "data_last_updated_at" ).toString(), Qt::ISODate ) );

    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "name" ), mName );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "owner" ), mOwner );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "description" ), mDescription );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "userRole" ), mUserRole );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "userRoleOrigin" ), mUserRoleOrigin );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "createdAt" ), mCreatedAt.toString( Qt::DateFormat::ISODate ) );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "updatedAt" ), mUpdatedAt.toString( Qt::DateFormat::ISODate ) );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "isPublic" ), mIsPublic );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "isFeatured" ), mIsFeatured );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "canRepackage" ), mCanRepackage );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "needsRepackaging" ), mNeedsRepackaging );
    QFieldCloudUtils::setProjectSetting( mId, QStringLiteral( "dataLastUpdatedAt" ), mDataLastUpdatedAt.toString( Qt::DateFormat::ISODate ) );

    emit dataRefreshed( reason );
  } );
}

void QFieldCloudProject::refreshDeltaList()
{
  if ( mDeltaListModel )
  {
    mDeltaListModel.reset();
    emit deltaListModelChanged();
  }

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [this, deltaStatusReply]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    mDeltaListModel.reset( new DeltaListModel( doc ) );
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

  setPackagingStatus( PackagingAbortStatus );
  setPackagingStatusString( tr( "aborted" ) );
  setErrorStatus( NoErrorStatus );
  setIsPackagingActive( false );
  setIsPackagingFailed( true );
  setStatus( ProjectStatus::Idle );
}

void QFieldCloudProject::removeLocally()
{
  QDir dir( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId ) );
  if ( dir.exists() )
  {
    dir.removeRecursively();

    if ( mDeltaFileWrapper )
    {
      mDeltaFileWrapper.reset();
      emit deltasCountChanged();
      emit deltaFileWrapperChanged();
    }

    setLocalPath( QString() );
    setModification( NoModification );
    mCheckout = mCheckout & ~LocalCheckout;
    emit checkoutChanged();
  }

  QSettings().remove( QStringLiteral( "QFieldCloud/projects/%1" ).arg( mId ) );
}

QFieldCloudProject *QFieldCloudProject::fromDetails( const QVariantHash &details, QFieldCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher )
{
  QFieldCloudProject *project = new QFieldCloudProject( details.value( "id" ).toString(), connection, gpkgFlusher );
  project->mIsPublic = details.value( "is_public" ).toBool();
  project->mIsFeatured = details.value( "is_featured" ).toBool();
  project->mOwner = details.value( "owner" ).toString();
  project->mName = details.value( "name" ).toString();
  project->mDescription = details.value( "description" ).toString();
  project->mUserRole = details.value( "user_role" ).toString();
  project->mUserRoleOrigin = details.value( "user_role_origin" ).toString();
  project->mCheckout = RemoteCheckout;
  project->mStatus = details.value( "status" ).toString() == "failed" ? ProjectStatus::Failing : ProjectStatus::Idle;
  project->mCreatedAt = QDateTime::fromString( details.value( "created_at" ).toString(), Qt::ISODate );
  project->mUpdatedAt = QDateTime::fromString( details.value( "updated_at" ).toString(), Qt::ISODate );
  project->mDataLastUpdatedAt = QDateTime::fromString( details.value( "data_last_updated_at" ).toString(), Qt::ISODate );
  project->mRestrictedDataLastUpdatedAt = QDateTime::fromString( details.value( "restricted_data_last_updated_at" ).toString(), Qt::ISODate );
  project->mCanRepackage = details.value( "can_repackage" ).toBool();
  project->mNeedsRepackaging = details.value( "needs_repackaging" ).toBool();
  project->mSharedDatasetsProjectId = details.value( "shared_datasets_project_id" ).toString();
  project->mIsSharedDatasetsProject = details.value( "is_shared_datasets_project" ).toBool();
  project->mAttachmentsOnDemandEnabled = details.value( "is_attachment_download_on_demand" ).toBool();

  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "owner" ), project->owner() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "name" ), project->name() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "description" ), project->description() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "userRole" ), project->userRole() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "userRoleOrigin" ), project->userRoleOrigin() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "createdAt" ), project->createdAt().toString( Qt::DateFormat::ISODate ) );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "updatedAt" ), project->updatedAt().toString( Qt::DateFormat::ISODate ) );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "canRepackage" ), project->canRepackage() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "needsRepackaging" ), project->needsRepackaging() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "sharedDatasetsProjectId" ), project->sharedDatasetsProjectId() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "isSharedDatasetsProject" ), project->isSharedDatasetsProject() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "isPublic" ), project->isPublic() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "isFeatured" ), project->isFeatured() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "isAttachmentDownloadOnDemand" ), project->attachmentsOnDemandEnabled() );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "dataLastUpdatedAt" ), project->mDataLastUpdatedAt.toString( Qt::DateFormat::ISODate ) );
  QFieldCloudUtils::setProjectSetting( project->id(), QStringLiteral( "restrictedDataLastUpdatedAt" ), project->mRestrictedDataLastUpdatedAt.toString( Qt::DateFormat::ISODate ) );

  QString username = connection ? connection->username() : QString();
  if ( !username.isEmpty() )
  {
    project->mLocalPath = QFieldCloudUtils::localProjectFilePath( username, project->id() );
    QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, project->id() ) );
    if ( localPath.exists() )
    {
      restoreLocalSettings( project, localPath );
      project->mCheckout = !project->mLocalPath.isEmpty() ? LocalAndRemoteCheckout : RemoteCheckout;
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

  const bool isPublic = QFieldCloudUtils::projectSetting( id, QStringLiteral( "isPublic" ) ).toBool();
  const bool isFeatured = QFieldCloudUtils::projectSetting( id, QStringLiteral( "isFeatured" ) ).toBool();
  const QString owner = QFieldCloudUtils::projectSetting( id, QStringLiteral( "owner" ) ).toString();
  const QString name = QFieldCloudUtils::projectSetting( id, QStringLiteral( "name" ) ).toString();
  const QString description = QFieldCloudUtils::projectSetting( id, QStringLiteral( "description" ) ).toString();
  const QString status = QFieldCloudUtils::projectSetting( id, QStringLiteral( "status" ) ).toString();
  const QString userRole = QFieldCloudUtils::projectSetting( id, QStringLiteral( "userRole" ) ).toString();
  const QString userRoleOrigin = QFieldCloudUtils::projectSetting( id, QStringLiteral( "userRoleOrigin" ) ).toString();
  const QDateTime createdAt = QDateTime::fromString( QFieldCloudUtils::projectSetting( id, QStringLiteral( "createdAt" ) ).toString(), Qt::DateFormat::ISODate );
  const QDateTime updatedAt = QDateTime::fromString( QFieldCloudUtils::projectSetting( id, QStringLiteral( "updatedAt" ) ).toString(), Qt::DateFormat::ISODate );
  const QString sharedDatasetsProjectId = QFieldCloudUtils::projectSetting( id, QStringLiteral( "sharedDatasetsProjectId" ) ).toString();
  const bool isSharedDatasetsProject = QFieldCloudUtils::projectSetting( id, QStringLiteral( "isSharedDatasetsProject" ) ).toBool();
  const bool isAttachmentDownloadOnDemand = QFieldCloudUtils::projectSetting( id, QStringLiteral( "isAttachmentDownloadOnDemand" ) ).toBool();
  const QDateTime dataLastUpdatedAt = QDateTime::fromString( QFieldCloudUtils::projectSetting( id, QStringLiteral( "dataLastUpdatedAt" ) ).toString(), Qt::DateFormat::ISODate );
  const QDateTime restrictedDataLastUpdatedAt = QDateTime::fromString( QFieldCloudUtils::projectSetting( id, QStringLiteral( "restrictedDataLastUpdatedAt" ) ).toString(), Qt::DateFormat::ISODate );

  QFieldCloudProject *project = new QFieldCloudProject( id, connection, gpkgFlusher );
  project->mIsPublic = isPublic;
  project->mIsFeatured = isFeatured;
  project->mOwner = owner;
  project->mName = name;
  project->mDescription = description;
  project->mUserRole = userRole;
  project->mUserRoleOrigin = userRoleOrigin;
  project->mCheckout = LocalCheckout;
  project->mStatus = status == "failed" ? ProjectStatus::Failing : ProjectStatus::Idle;
  project->mCreatedAt = createdAt;
  project->mUpdatedAt = updatedAt;
  project->mDataLastUpdatedAt = dataLastUpdatedAt;
  project->mRestrictedDataLastUpdatedAt = restrictedDataLastUpdatedAt;
  project->mCanRepackage = false;
  project->mNeedsRepackaging = false;
  project->mSharedDatasetsProjectId = sharedDatasetsProjectId;
  project->mIsSharedDatasetsProject = isSharedDatasetsProject;
  project->mAttachmentsOnDemandEnabled = isAttachmentDownloadOnDemand;

  QString username = connection ? connection->username() : QString();
  if ( !username.isEmpty() )
  {
    project->mLocalPath = QFieldCloudUtils::localProjectFilePath( username, project->mId );
  }

  const QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, project->mId ) );
  restoreLocalSettings( project, localPath );

  return project;
}

void QFieldCloudProject::restoreLocalSettings( QFieldCloudProject *project, const QDir &localPath )
{
  project->mLastExportId = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastExportId" ) ).toString();
  project->mLastExportedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastExportedAt" ) ).toString();
  project->mLastLocalExportId = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalExportId" ) ).toString();
  project->mLastLocalExportedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalExportedAt" ) ).toString();
  project->mLastLocalPushDeltas = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalPushDeltas" ) ).toString();
  project->mLastLocalDataLastUpdatedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalDataLastUpdatedAt" ) ).toDateTime();
  project->mLastLocalRestrictedDataLastUpdatedAt = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "lastLocalRestrictedDataLastUpdatedAt" ) ).toDateTime();
  project->mIsOutdated = project->mDataLastUpdatedAt > project->mLastLocalDataLastUpdatedAt;
  project->mIsProjectOutdated = project->mRestrictedDataLastUpdatedAt.isValid() && project->mLastLocalRestrictedDataLastUpdatedAt.isValid() && project->mRestrictedDataLastUpdatedAt > project->mLastLocalRestrictedDataLastUpdatedAt;
  project->mAutoPushEnabled = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "autoPushEnabled" ), false ).toBool();
  project->mAutoPushIntervalMins = QFieldCloudUtils::projectSetting( project->id(), QStringLiteral( "autoPushIntervalMins" ), 30 ).toInt();

  // generate local export id if not present. Possible reasons for missing localExportId are:
  // - the cloud project download aborted halfway
  // - the local settings were somehow deleted, but not the project itself (unlikely)
  if ( !project->lastLocalExportId().isEmpty() )
  {
    project->setupDeltaFileWrapper();
  }
  else
  {
    project->mLocalPath.clear();
  }
};

void QFieldCloudProject::setupDeltaFileWrapper()
{
  const QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, mId ) );
  mDeltaFileWrapper.reset( new DeltaFileWrapper( mId, QStringLiteral( "%1/deltafile.json" ).arg( localPath.absolutePath() ) ) );

  connect( mDeltaFileWrapper.get(), &DeltaFileWrapper::countChanged, this, [this]() {
    refreshModification();
    emit deltasCountChanged();
  } );

  emit deltaFileWrapperChanged();
  refreshModification();
  emit deltasCountChanged();
}

void QFieldCloudProject::uploadLocalPath( QString localPath, bool deleteAfterSuccessfulUpload )
{
  QFileInfo localInfo( localPath );
  if ( !localInfo.exists() )
  {
    emit uploadFinished( tr( "Local path doesn't exist" ) );
    return;
  }

  if ( localInfo.isFile() )
  {
    localPath = localInfo.absolutePath();
  }

  QFileInfo projectFileInfo;
  QDirIterator projectDirIterator( localPath, { "*.qgs", "*.qgz" }, QDir::Files, QDirIterator::Subdirectories );
  while ( projectDirIterator.hasNext() )
  {
    projectDirIterator.next();
    if ( projectFileInfo.exists() )
    {
      emit uploadFinished( tr( "Local path to upload cannot be used as it has multiple project files" ) );
      return;
    }
    projectFileInfo = projectDirIterator.fileInfo();
  }

  if ( !projectFileInfo.exists() || projectFileInfo.size() == 0 )
  {
    emit uploadFinished( tr( "Local path to upload is missing a valid project file" ) );
    return;
  }

  const QString currentProjectLocalPath = FileUtils::absolutePath( QgsProject::instance()->fileName() );
  if ( projectFileInfo.absoluteFilePath() == currentProjectLocalPath )
  {
    // we need to close the project to safely flush the gpkg files and avoid file lock on Windows
    QDirIterator it( localPath, { QStringLiteral( "*.gpkg" ), QStringLiteral( "*.sqlite" ) }, QDir::Filter::Files, QDirIterator::Subdirectories );
    QStringList gpkgFileNames;
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

  mUploadFilesFailed = 0;
  mUploadBytesTotal = 0;
  mUploadBytesSent = 0;
  mUploadProgress = 0.0;

  QDir localDir( localPath );
  QDirIterator localDirIterator( localPath, QDir::Files, QDirIterator::Subdirectories );
  while ( localDirIterator.hasNext() )
  {
    localDirIterator.next();
    QFileInfo localFileInfo = localDirIterator.fileInfo();
    const QString localFileSuffix = localFileInfo.suffix().toLower();
    if ( localFileSuffix == QStringLiteral( "gpkg-shm" ) || localFileSuffix == QStringLiteral( "gpkg-wal" ) )
    {
      continue;
    }

    mUploadFileTransfers.insert( localFileInfo.absoluteFilePath(), QFieldCloudProject::FileTransfer( localDir.relativeFilePath( localFileInfo.absoluteFilePath() ), localFileInfo.size(), mId, QString() ) );
    mUploadBytesTotal += localFileInfo.size();
  }

  if ( !mUploadFileTransfers.isEmpty() )
  {
    emit uploadBytesTotalChanged();
    emit uploadBytesSentChanged();
    emit uploadProgressChanged();

    setStatus( ProjectStatus::Uploading );

    mUploadLocalPath = localPath;
    mUploadDeleteAfterSuccessfulUpload = deleteAfterSuccessfulUpload;

    uploadFiles();
  }
}

void QFieldCloudProject::uploadFiles()
{
  if ( mUploadFileTransfers.isEmpty() )
  {
    if ( mStatus == ProjectStatus::Uploading )
    {
      setStatus( ProjectStatus::Idle );
    }
    return;
  }

  QString filePath = mUploadFileTransfers.lastKey();
  QFieldCloudProject::FileTransfer &fileTransfer = mUploadFileTransfers.last();

  NetworkReply *reply = mCloudConnection->post( QStringLiteral( "/api/v1/files/%1/%2/" ).arg( fileTransfer.projectId, fileTransfer.fileName ), QVariantMap(), QStringList( { filePath } ) );
  fileTransfer.networkReply = reply;
  connect( reply, &NetworkReply::finished, this, [this, reply, filePath]() {
    QNetworkReply *rawReply = reply->currentRawReply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    // this is most probably the redirected request, nothing to do with this reply anymore, just ignore it
    if ( mUploadFileTransfers[filePath].networkReply != reply )
    {
      return;
    }

    rawReply->deleteLater();

    const QString projectId = mUploadFileTransfers[filePath].projectId;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      const QString errorMessage = tr( "Network error. Failed to upload file `%1`." ).arg( filePath );
      QgsLogger::debug( errorMessage );
      rawReply->abort();

      if ( mUploadFileTransfers[filePath].retryCount++ <= 3 )
      {
        uploadFiles();
        return;
      }
      else
      {
        mUploadFilesFailed++;
      }
    }
    else
    {
      QgsLogger::debug( QStringLiteral( "Project %1, file `%2`: uploaded" ).arg( projectId, filePath ) );
    }

    mUploadBytesSent += mUploadFileTransfers[filePath].bytesTotal;
    mUploadProgress = std::clamp( ( static_cast<double>( mUploadBytesSent ) / std::max( mUploadBytesTotal, static_cast<qint64>( 1 ) ) ), 0., 1. );
    emit uploadBytesSentChanged();
    emit uploadProgressChanged();

    mUploadFileTransfers.remove( filePath );
    if ( mUploadFileTransfers.isEmpty() )
    {
      setStatus( ProjectStatus::Idle );
      if ( mUploadDeleteAfterSuccessfulUpload && mUploadFilesFailed == 0 )
      {
        const QString currentProjectLocalPath = FileUtils::absolutePath( QgsProject::instance()->fileName() );
        if ( mUploadLocalPath == currentProjectLocalPath )
        {
          // Remove local path in favor of cloud version
          QDir uploadLocalDir( mUploadLocalPath );
          uploadLocalDir.removeRecursively();
        }
      }

      emit uploadFinished( mUploadFilesFailed > 0 ? tr( "One or more files could not be uploaded" ) : QString() );
    }
    else
    {
      uploadFiles();
    }
  } );
}
```


