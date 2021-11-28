/***************************************************************************
    qfieldcloudprojectsmodel.cpp
    ---------------------
    begin                : January 2020
    copyright            : (C) 2020 by Matthias Kuhn
    email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "deltafilewrapper.h"
#include "fileutils.h"
#include "layerobserver.h"
#include "qfield.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudprojectsmodel.h"
#include "qfieldcloudutils.h"

#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>
#include <QTemporaryFile>
#include <qgis.h>
#include <qgsapplication.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>

#define MAX_REDIRECTS_ALLOWED 10
#define MAX_PARALLEL_REQUESTS 6

QFieldCloudProjectsModel::QFieldCloudProjectsModel()
  : mProject( QgsProject::instance() )
{
  QJsonArray projects;
  reload( projects );

  // TODO all of these connects are a bit too much, and I guess not very precise, should be refactored!
  connect( this, &QFieldCloudProjectsModel::currentProjectIdChanged, this, [=]() {
    const int index = findProject( mCurrentProjectId );

    if ( index < 0 || index >= mCloudProjects.size() )
      return;

    refreshProjectModification( mCurrentProjectId );
  } );

  connect( this, &QFieldCloudProjectsModel::modelReset, this, [=]() {
    const int index = findProject( mCurrentProjectId );

    if ( index < 0 || index >= mCloudProjects.size() )
      return;

    emit currentProjectDataChanged();

    refreshProjectModification( mCurrentProjectId );
  } );

  connect( this, &QFieldCloudProjectsModel::dataChanged, this, [=]( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles ) {
    Q_UNUSED( bottomRight )
    Q_UNUSED( roles )

    const int index = findProject( mCurrentProjectId );

    if ( index < 0 || index >= mCloudProjects.size() )
      return;

    // current project
    if ( topLeft.row() == index )
    {
      emit currentProjectDataChanged();
    }
  } );

  connect( this, &QFieldCloudProjectsModel::cloudConnectionChanged, this, [=]() {
    if ( !mCloudConnection )
      return;

    mUsername = mCloudConnection->username();
    if ( mCloudConnection->status() != QFieldCloudConnection::ConnectionStatus::LoggedIn )
    {
      QJsonArray projects;
      reload( projects );
    }
    connect( mCloudConnection, &QFieldCloudConnection::usernameChanged, this, [=]() {
      mUsername = mCloudConnection->username();
    } );
  } );
}

QFieldCloudConnection *QFieldCloudProjectsModel::cloudConnection() const
{
  return mCloudConnection;
}

void QFieldCloudProjectsModel::setCloudConnection( QFieldCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
    return;

  if ( cloudConnection )
    connect( cloudConnection, &QFieldCloudConnection::statusChanged, this, &QFieldCloudProjectsModel::connectionStatusChanged );

  mCloudConnection = cloudConnection;
  emit cloudConnectionChanged();
}

LayerObserver *QFieldCloudProjectsModel::layerObserver() const
{
  return mLayerObserver;
}

void QFieldCloudProjectsModel::setLayerObserver( LayerObserver *layerObserver )
{
  if ( mLayerObserver == layerObserver )
    return;

  mLayerObserver = layerObserver;

  if ( !layerObserver )
    return;

  connect( layerObserver, &LayerObserver::layerEdited, this, &QFieldCloudProjectsModel::layerObserverLayerEdited );
  connect( layerObserver->deltaFileWrapper(), &DeltaFileWrapper::countChanged, this, [=]() {
    refreshProjectModification( mCurrentProjectId );
  } );

  emit layerObserverChanged();
}

QString QFieldCloudProjectsModel::currentProjectId() const
{
  return mCurrentProjectId;
}

void QFieldCloudProjectsModel::setCurrentProjectId( const QString &currentProjectId )
{
  if ( mCurrentProjectId == currentProjectId )
    return;

  mCurrentProjectId = currentProjectId;

  emit currentProjectIdChanged();
  emit currentProjectDataChanged();
}

QVariantMap QFieldCloudProjectsModel::currentProjectData() const
{
  return getProjectData( mCurrentProjectId );
}

QVariantMap QFieldCloudProjectsModel::getProjectData( const QString &projectId ) const
{
  QVariantMap data;

  const int index = findProject( projectId );
  const QModelIndex idx = this->index( index, 0 );

  if ( !idx.isValid() )
    return data;

  const QHash<int, QByteArray> rn = this->roleNames();

  for ( auto [key, value] : qfield::asKeyValueRange( rn ) )
  {
    data[value] = idx.data( key );
  }

  return data;
}

void QFieldCloudProjectsModel::refreshProjectsList()
{
  switch ( mCloudConnection->status() )
  {
    case QFieldCloudConnection::ConnectionStatus::LoggedIn:
    {
      NetworkReply *reply = mCloudConnection->get( QStringLiteral( "/api/v1/projects/?include-public=true" ) );
      connect( reply, &NetworkReply::finished, this, &QFieldCloudProjectsModel::projectListReceived );
      break;
    }
    case QFieldCloudConnection::ConnectionStatus::Disconnected:
    {
      // Nothing done at the moment; since the connection can be disconnected after discovering
      // a token has been invalidated, be careful with what is added here.
      break;
    }
    case QFieldCloudConnection::ConnectionStatus::Connecting:
      // Nothing done for this intermediary status.
      break;
  }
}

int QFieldCloudProjectsModel::findProject( const QString &projectId ) const
{
  const QList<CloudProject> cloudProjects = mCloudProjects;
  int index = -1;
  for ( int i = 0; i < cloudProjects.count(); i++ )
  {
    if ( cloudProjects.at( i ).id == projectId )
    {
      index = i;
      break;
    }
  }
  return index;
}

void QFieldCloudProjectsModel::removeLocalProject( const QString &projectId )
{
  QDir dir( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, projectId ) );

  if ( dir.exists() )
  {
    int index = findProject( projectId );
    if ( index > -1 )
    {
      if ( mCloudProjects.at( index ).status == ProjectStatus::Idle && mCloudProjects.at( index ).checkout & RemoteCheckout )
      {
        mCloudProjects[index].localPath = QString();
        mCloudProjects[index].checkout = RemoteCheckout;
        mCloudProjects[index].modification = NoModification;
        QModelIndex idx = createIndex( index, 0 );
        emit dataChanged( idx, idx, QVector<int>() << StatusRole << LocalPathRole << CheckoutRole );
      }
      else
      {
        beginRemoveRows( QModelIndex(), index, index );
        mCloudProjects.removeAt( index );
        endRemoveRows();
      }
    }

    dir.removeRecursively();
  }

  QSettings().remove( QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId ) );
}

QFieldCloudProjectsModel::ProjectStatus QFieldCloudProjectsModel::projectStatus( const QString &projectId ) const
{
  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return QFieldCloudProjectsModel::ProjectStatus::Idle;

  return mCloudProjects[index].status;
}

bool QFieldCloudProjectsModel::canSyncProject( const QString &projectId ) const
{
  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return false;

  if ( mCurrentProjectId.isEmpty() )
    return false;
  else if ( projectStatus( projectId ) == ProjectStatus::Idle )
    return true;

  return false;
}

QFieldCloudProjectsModel::ProjectModifications QFieldCloudProjectsModel::projectModification( const QString &projectId ) const
{
  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return NoModification;

  return mCloudProjects[index].modification;
}

void QFieldCloudProjectsModel::refreshProjectModification( const QString &projectId )
{
  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  ProjectModifications oldModifications = mCloudProjects[index].modification;

  if ( mLayerObserver->deltaFileWrapper()->count() > 0 )
    mCloudProjects[index].modification |= LocalModification;
  else if ( mCloudProjects[index].modification & LocalModification )
    mCloudProjects[index].modification ^= LocalModification;

  if ( oldModifications != mCloudProjects[index].modification )
  {
    QModelIndex idx = createIndex( index, 0 );
    emit dataChanged( idx, idx, QVector<int>() << ModificationRole << CanSyncRole );
  }
}

QString QFieldCloudProjectsModel::layerFileName( const QgsMapLayer *layer ) const
{
  return layer->dataProvider()->dataSourceUri().split( '|' )[0];
}

QFieldCloudProjectsModel::PackagingStatus QFieldCloudProjectsModel::packagingStatus( const QString &status ) const
{
  const QString statusUpper = status.toUpper();
  if ( statusUpper == QStringLiteral( "STATUS_PENDING" ) )
    return PackagingPendingStatus;
  else if ( statusUpper == QStringLiteral( "STATUS_BUSY" ) )
    return PackagingBusyStatus;
  else if ( statusUpper == QStringLiteral( "STATUS_EXPORTED" ) )
    return PackagingFinishedStatus;
  else
    // "STATUS_ERROR" or any unknown status is considered an error
    return PackagingErrorStatus;
}

void QFieldCloudProjectsModel::cancelDownloadProject( const QString &projectId )
{
  if ( !mCloudConnection )
    return;

  int index = findProject( projectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  // before canceling, the project should be downloading
  if ( mCloudProjects[index].status != ProjectStatus::Downloading )
    return;

  mCloudProjects[index].packagingStatus = PackagingAbortStatus;

  const QStringList fileNames = mCloudProjects[index].downloadFileTransfers.keys();
  for ( const QString &fileName : fileNames )
  {
    NetworkReply *reply = mCloudProjects[index].downloadFileTransfers[fileName].networkReply;

    if ( reply )
      reply->abort();

    mCloudProjects[index].downloadFileTransfers.remove( fileName );
  }

  if ( mCloudProjects[index].apiNetworkReply )
  {
    mCloudProjects[index].apiNetworkReply->abort();
  }

  mCloudProjects[index].status = ProjectStatus::Idle;

  QModelIndex idx = createIndex( index, 0 );
  emit dataChanged( idx, idx, QVector<int>() << StatusRole << PackagingStatusRole );
}

void QFieldCloudProjectsModel::downloadProject( const QString &projectId, bool overwriteProject )
{
  if ( !mCloudConnection )
    return;

  int index = findProject( projectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  // before downloading, the project should be idle
  if ( mCloudProjects[index].status != ProjectStatus::Idle )
    return;

  // before downloading, there should be no local modification, otherwise it will be overwritten
  if ( mCloudProjects[index].modification & LocalModification && !overwriteProject )
    return;

  mCloudProjects[index].packagingStatus = PackagingUnstartedStatus;
  mCloudProjects[index].packagingStatusString = QString();
  mCloudProjects[index].packagedLayerErrors.clear();
  mCloudProjects[index].downloadFileTransfers.clear();
  mCloudProjects[index].downloadFilesFinished = 0;
  mCloudProjects[index].downloadFilesFailed = 0;
  mCloudProjects[index].downloadBytesTotal = 0;
  mCloudProjects[index].downloadBytesReceived = 0;
  mCloudProjects[index].downloadProgress = 0;

  mCloudProjects[index].status = ProjectStatus::Downloading;
  mCloudProjects[index].errorStatus = NoErrorStatus;
  mCloudProjects[index].modification = NoModification;
  QModelIndex idx = createIndex( index, 0 );

  emit dataChanged( idx, idx, QVector<int>::fromList( roleNames().keys() ) );

  QgsLogger::debug( QStringLiteral( "Packaging requested for \"%1\"" ).arg( projectId ) );

  // //////////
  // 1) request a new project packaging
  // //////////
  /// If you want to skip packaging as being the slowest action, change /qfield-files/export/<PROJECT_ID> from POST to GET request
  NetworkReply *reply = mCloudConnection->post( QStringLiteral( "/api/v1/qfield-files/export/%1/" ).arg( projectId ) );
  mCloudProjects[index].apiNetworkReply = reply;

  connect( reply, &NetworkReply::finished, reply, [=]() {
    QNetworkReply *rawReply = reply->reply();

    reply->deleteLater();
    mCloudProjects[index].apiNetworkReply = nullptr;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsLogger::debug( QStringLiteral( "Packaging request for \"%1\" finished with error: %2" ).arg( projectId, rawReply->error() ) );
      projectDownloadFinishedWithError( projectId, QFieldCloudConnection::errorString( rawReply ) );
      return;
    }

    QgsLogger::debug( QStringLiteral( "Packaging request for \"%1\" finished with no error" ).arg( projectId ) );

    const QJsonObject payload = QJsonDocument::fromJson( rawReply->readAll() ).object();
    Q_ASSERT( !payload.isEmpty() );
    mCloudProjects[index].packagingStatus = packagingStatus( payload.value( QStringLiteral( "status" ) ).toString() );

    emit dataChanged( idx, idx, QVector<int>() << PackagingStatusRole );

    projectGetPackagingStatus( projectId );
  } );
}

void QFieldCloudProjectsModel::projectGetPackagingStatus( const QString &projectId )
{
  const int index = findProject( projectId );

  Q_ASSERT( index >= 0 && index < mCloudProjects.size() );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  if ( mCloudProjects[index].packagingStatus == PackagingAbortStatus )
    return;

  Q_ASSERT( mCloudProjects[index].packagingStatus != PackagingUnstartedStatus );

  QModelIndex idx = createIndex( index, 0 );
  NetworkReply *downloadStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/qfield-files/export/%1/" ).arg( projectId ) );

  mCloudProjects[index].packagingStatusString = QString();
  mCloudProjects[index].apiNetworkReply = downloadStatusReply;

  QgsLogger::debug( QStringLiteral( "Packaging status requested for \"%1\"" ).arg( projectId ) );

  connect( downloadStatusReply, &NetworkReply::finished, this, [=]() {
    if ( mCloudProjects[index].packagingStatus == PackagingAbortStatus )
      return;

    QNetworkReply *rawReply = downloadStatusReply->reply();

    Q_ASSERT( downloadStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    downloadStatusReply->deleteLater();
    mCloudProjects[index].apiNetworkReply = nullptr;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      QgsLogger::debug( QStringLiteral( "Packaging status request for \"%1\" finished with error: %2" ).arg( projectId, rawReply->error() ) );
      projectDownloadFinishedWithError( projectId, QFieldCloudConnection::errorString( rawReply ) );
      return;
    }

    const QJsonObject payload = QJsonDocument::fromJson( rawReply->readAll() ).object();
    Q_ASSERT( !payload.isEmpty() );

    mCloudProjects[index].packagingStatus = packagingStatus( payload.value( QStringLiteral( "status" ) ).toString() );

    QgsLogger::debug( QStringLiteral( "Packaging request for \"%1\" finished with no error and status \"%2\"" )
                      .arg( projectId )
                      .arg( QVariant::fromValue( mCloudProjects[index].packagingStatus ).toString() ) );

    switch ( mCloudProjects[index].packagingStatus )
    {
      case PackagingUnstartedStatus:
        // download export job should be already started!!!
        Q_ASSERT( 0 );
        FALLTHROUGH

      case PackagingAbortStatus:
        return;

      case PackagingPendingStatus:
      case PackagingBusyStatus:
        // infinite retry, there should be one day, when we can get the status!
        QTimer::singleShot( sDelayBeforeStatusRetry, [=]() {
          projectGetPackagingStatus( projectId );
        } );
        break;

      case PackagingErrorStatus:
      {
        QString output = payload.value( QStringLiteral( "output" ) ).toString().split( '\n' ).last();
        if ( output.size() > 0 )
          projectDownloadFinishedWithError( projectId, output );
        else
          projectDownloadFinishedWithError( projectId, tr( "Export failed" ) );
        return;
      }

      case PackagingFinishedStatus:
        QgsLogger::debug( QStringLiteral( "Export files list requested for \"%1\"" ).arg( projectId ) );

        NetworkReply *exportedFilesReply = mCloudConnection->get( QStringLiteral( "/api/v1/qfield-files/%1/" ).arg( projectId ) );

        mCloudProjects[index].packagingStatusString = QString();
        mCloudProjects[index].apiNetworkReply = exportedFilesReply;

        emit dataChanged( idx, idx, QVector<int>() << PackagingStatusRole );

        connect( exportedFilesReply, &NetworkReply::finished, exportedFilesReply, [=]() {
          if ( mCloudProjects[index].packagingStatus == PackagingAbortStatus )
            return;

          QNetworkReply *exportedFilesRawReply = exportedFilesReply->reply();

          exportedFilesReply->deleteLater();
          mCloudProjects[index].apiNetworkReply = nullptr;

          if ( exportedFilesRawReply->error() != QNetworkReply::NoError )
          {
            QgsLogger::debug( QStringLiteral( "Packaging status request for \"%1\" finished with error: %2" ).arg( projectId, rawReply->error() ) );
            projectDownloadFinishedWithError( projectId, QFieldCloudConnection::errorString( rawReply ) );
            return;
          }

          const QJsonObject exportedFilesPayload = QJsonDocument::fromJson( exportedFilesRawReply->readAll() ).object();

          mCloudProjects[index].lastExportedAt = exportedFilesPayload.value( QStringLiteral( "exported_at" ) ).toString();
          mCloudProjects[index].lastExportId = exportedFilesPayload.value( QStringLiteral( "export_id" ) ).toString();

          QgsLogger::debug( QStringLiteral( "Packaging files list request finished for \"%1\" with no error and response: %2" ).arg( projectId, QString::fromUtf8( QJsonDocument( exportedFilesPayload ).toJson( QJsonDocument::Indented ) ) ) );

          const QJsonArray files = exportedFilesPayload.value( QStringLiteral( "files" ) ).toArray();
          for ( const QJsonValue &file : files )
          {
            QJsonObject fileObject = file.toObject();
            QString fileName = fileObject.value( QStringLiteral( "name" ) ).toString();
            QString projectFileName = QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, projectId, fileName );
            QString cloudChecksum = fileObject.value( QStringLiteral( "sha256" ) ).toString();
            QString localChecksum = FileUtils::fileChecksum( projectFileName, QCryptographicHash::Sha256 ).toHex();

            if ( cloudChecksum == localChecksum )
              continue;

            int fileSize = fileObject.value( QStringLiteral( "size" ) ).toInt();

            mCloudProjects[index].downloadFileTransfers.insert( fileName, FileTransfer( fileName, fileSize ) );
            mCloudProjects[index].downloadBytesTotal += std::max( fileSize, 0 );
          }

          const QJsonObject layers = exportedFilesPayload.value( QStringLiteral( "layers" ) ).toObject();
          bool hasLayerExportErrror = false;
          for ( const QString &layerKey : layers.keys() )
          {
            QJsonObject layer = layers.value( layerKey ).toObject();

            if ( !layer.value( QStringLiteral( "valid" ) ).toBool() )
            {
              QString layerName = layer.value( QStringLiteral( "name" ) ).toString();
              QString layerStatus = layer.value( QStringLiteral( "status" ) ).toString();

              mCloudProjects[index].packagedLayerErrors.append( tr( "Packaged layer '%1' is not valid: '%2'" )
                                                                .arg( layerName )
                                                                .arg( layerStatus ) );
              QgsMessageLog::logMessage( mCloudProjects[index].packagedLayerErrors.last() );

              hasLayerExportErrror = true;
            }
          }

          if ( hasLayerExportErrror )
          {
            QgsLogger::debug( QStringLiteral( "Packaged files list request finished for \"%1\" with some failed layers:\n%2" ).arg( projectId, mCloudProjects[index].packagedLayerErrors.join( QStringLiteral( "\n" ) ) ) );
            emit dataChanged( idx, idx, QVector<int>() << PackagedLayerErrorsRole );
          }

          QgsLogger::debug( QStringLiteral( "Packaged files to download - %1 files, namely: %2" )
                            .arg( mCloudProjects[index].downloadFileTransfers.count() )
                            .arg( mCloudProjects[index].downloadFileTransfers.keys().join( ", " ) ) );

          updateActiveProjectFilesToDownload( projectId );
          projectDownloadFiles( projectId );
        } );
        break;
    }
  } );
}

void QFieldCloudProjectsModel::updateActiveProjectFilesToDownload( const QString &projectId )
{
  if ( !mCloudConnection )
    return;

  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  const QStringList fileNames = mCloudProjects[index].downloadFileTransfers.keys();

  QgsLogger::debug( QStringLiteral( "Active download files list before update: %1" ).arg( mActiveProjectFilesToDownload.join( ", " ) ) );

  if ( fileNames.isEmpty() )
  {
    mActiveProjectFilesToDownload.clear();
    return;
  }

  for ( const QString &fileName : fileNames )
  {
    if ( mCloudProjects[index].downloadFileTransfers[fileName].networkReply )
    {
      if ( mCloudProjects[index].downloadFileTransfers[fileName].networkReply->isFinished() )
      {
        QgsLogger::debug( QStringLiteral( "Remove file \"%1\" from the active download files list if still present" ).arg( fileName ) );

        mActiveProjectFilesToDownload.removeOne( fileName );
        continue;
      }
      else
      {
        // the request is still active
        QgsLogger::debug( QStringLiteral( "File \"%1\" is still downloading, keep it in the active download files list" ).arg( fileName ) );
        continue;
      }
    }

    if ( mActiveProjectFilesToDownload.size() >= MAX_PARALLEL_REQUESTS )
    {
      QgsLogger::debug( QStringLiteral( "Active download files list is full, wait before continue with \"%1\"" ).arg( fileName ) );
      return;
    }

    QgsLogger::debug( QStringLiteral( "File \"%1\" appended to the active download files list" ).arg( fileName ) );

    mActiveProjectFilesToDownload.append( fileName );
  }

  QgsLogger::debug( QStringLiteral( "Active download files list after update: %1" ).arg( mActiveProjectFilesToDownload.join( ", " ) ) );
}

void QFieldCloudProjectsModel::projectDownloadFiles( const QString &projectId )
{
  // calling updateActiveProjectFilesToDownload() before calling this function is mandatory

  if ( !mCloudConnection )
    return;

  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  // Don't call download project files, if there are no project files
  if ( mActiveProjectFilesToDownload.isEmpty() )
  {
    QModelIndex idx = createIndex( index, 0 );

    mCloudProjects[index].status = ProjectStatus::Idle;
    mCloudProjects[index].downloadProgress = 1;

    emit dataChanged( idx, idx, QVector<int>() << StatusRole << DownloadProgressRole );

    return;
  }

  QgsLogger::debug( QStringLiteral( "Active download files list before actual download: %1" ).arg( mActiveProjectFilesToDownload.join( ", " ) ) );

  for ( const QString &fileName : std::as_const( mActiveProjectFilesToDownload ) )
  {
    if ( mCloudProjects[index].downloadFileTransfers[fileName].networkReply )
    {
      // Download is already in progress
      continue;
    }

    NetworkReply *reply = downloadFile( projectId, fileName );
    QTemporaryFile *file = new QTemporaryFile( reply );

    file->setAutoRemove( false );

    if ( !file->open() )
    {
      mCloudProjects[index].downloadFilesFailed++;
      projectDownloadFinishedWithError( projectId, tr( "Failed to open temporary file for \"%1\", reason:\n%2" )
                                                   .arg( fileName )
                                                   .arg( file->errorString() ) );
      return;
    }

    mCloudProjects[index].downloadFileTransfers[fileName].tmpFile = file->fileName();
    mCloudProjects[index].downloadFileTransfers[fileName].networkReply = reply;

    QgsLogger::debug( QStringLiteral( "Requested file \"%1/%2\"" ).arg( projectId, fileName ) );

    downloadFileConnections( projectId, fileName );
  }
}

void QFieldCloudProjectsModel::projectDownloadFinishedWithError( const QString &projectId, const QString &errorString )
{
  const int index = findProject( projectId );

  mActiveProjectFilesToDownload.clear();

  if ( index < 0 || index >= mCloudProjects.size() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "Project id \"%1\" not found" ).arg( projectId ) );
    return;
  }
  QModelIndex idx = createIndex( index, 0 );

  mCloudProjects[index].status = ProjectStatus::Idle;
  mCloudProjects[index].errorStatus = DownloadErrorStatus;
  mCloudProjects[index].packagingStatus = PackagingErrorStatus;
  mCloudProjects[index].packagingStatusString = errorString;
  QgsMessageLog::logMessage( QStringLiteral( "Download of project id \"%1\" finished with error: %2" ).arg( projectId ).arg( mCloudProjects[index].packagingStatusString ) );
  emit dataChanged( idx, idx, QVector<int>() << StatusRole << PackagingStatusRole << ErrorStatusRole << ErrorStringRole );
  emit projectDownloaded( projectId, mCloudProjects[index].name, true, errorString );
}

bool QFieldCloudProjectsModel::projectMoveDownloadedFilesToPermanentStorage( const QString &projectId )
{
  if ( !mCloudConnection )
    return false;

  const int index = findProject( projectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return false;

  bool hasError = false;
  const QStringList fileNames = mCloudProjects[index].downloadFileTransfers.keys();

  for ( const QString &fileName : fileNames )
  {
    QFileInfo fileInfo( fileName );
    QFile file( mCloudProjects[index].downloadFileTransfers[fileName].tmpFile );
    QDir dir( QStringLiteral( "%1/%2/%3/%4" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, projectId, fileInfo.path() ) );

    if ( !hasError && !dir.exists() && !dir.mkpath( QStringLiteral( "." ) ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to create directory at \"%1\"" ).arg( dir.path() ) );
    }

    const QString destinationFileName( dir.filePath( fileInfo.fileName() ) );

    // if the file already exists, we need to delete it first, as QT does not support overwriting
    // NOTE: it is possible that someone creates the file in the meantime between this and the next if statement
    if ( !hasError && QFile::exists( destinationFileName ) && !file.remove( destinationFileName ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove file before overwriting stored at \"%1\", reason:\n%2" ).arg( fileName ).arg( file.errorString() ) );
    }

    if ( !hasError && !file.copy( destinationFileName ) )
    {
      hasError = true;
      QgsMessageLog::logMessage( QStringLiteral( "Failed to write downloaded file stored at \"%1\", reason:\n%2" ).arg( fileName ).arg( file.errorString() ) );

      if ( !QFile::remove( dir.filePath( fileName ) ) )
        QgsMessageLog::logMessage( QStringLiteral( "Failed to remove partly overwritten file stored at \"%1\"" ).arg( fileName ) );
    }

    if ( !file.remove() )
      QgsMessageLog::logMessage( QStringLiteral( "Failed to remove temporary file \"%1\"" ).arg( fileName ) );
  }

  return !hasError;
}


void QFieldCloudProjectsModel::uploadProject( const QString &projectId, const bool shouldDownloadUpdates )
{
  if ( !mCloudConnection )
    return;

  int index = findProject( projectId );

  if ( index < 0 )
    return;

  if ( !( mCloudProjects[index].status == ProjectStatus::Idle ) )
    return;

  DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  if ( shouldDownloadUpdates && deltaFileWrapper->count() == 0 )
  {
    downloadProject( projectId );
    return;
  }

  if ( !( mCloudProjects[index].modification & LocalModification ) )
    return;

  if ( !mLayerObserver->deltaFileWrapper()->toFile() )
    return;

  QModelIndex idx = createIndex( index, 0 );

  if ( deltaFileWrapper->hasError() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "The delta file has an error: %1" ).arg( deltaFileWrapper->errorString() ) );
    return;
  }

  mCloudProjects[index].status = ProjectStatus::Uploading;
  mCloudProjects[index].deltaFileId = deltaFileWrapper->id();
  mCloudProjects[index].deltaFileUploadStatus = DeltaLocalStatus;
  mCloudProjects[index].deltaFileUploadStatusString = QString();

  refreshProjectModification( projectId );

  emit dataChanged( idx, idx, QVector<int>() << StatusRole << UploadAttachmentsCountRole << UploadDeltaProgressRole << UploadDeltaStatusRole << UploadDeltaStatusStringRole );

  // //////////
  // prepare attachment files to be uploaded
  // //////////
  if ( mCloudProjects[index].uploadAttachmentsStatus != UploadAttachmentsStatus::UploadAttachmentsDone )
  {
    projectCancelUploadAttachments( projectId );
  }
  mCloudProjects[index].uploadAttachmentsFailed = 0;

  const QFileInfo projectInfo( QFieldCloudUtils::localProjectFilePath( mUsername, projectId ) );
  const QDir projectDir( projectInfo.absolutePath() );
  const QStringList attachmentFileNames = deltaFileWrapper->attachmentFileNames().keys();
  for ( const QString &fileName : attachmentFileNames )
  {
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
    mCloudProjects[index].uploadAttachments.insert( absoluteFilePath, FileTransfer( absoluteFilePath, fileSize ) );
  }

  QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "uploadAttachments" ), QStringList( mCloudProjects[index].uploadAttachments.keys() ) );

  QString deltaFileToUpload = deltaFileWrapper->toFileForUpload();

  if ( deltaFileToUpload.isEmpty() )
  {
    mCloudProjects[index].status = ProjectStatus::Idle;
    emit dataChanged( idx, idx, QVector<int>() << StatusRole );
    return;
  }

  // //////////
  // 1) upload the deltas
  // //////////
  NetworkReply *deltasCloudReply = mCloudConnection->post(
  QStringLiteral( "/api/v1/deltas/%1/" ).arg( projectId ),
  QVariantMap(),
  QStringList( { deltaFileToUpload } ) );

  Q_ASSERT( deltasCloudReply );

  connect( deltasCloudReply, &NetworkReply::uploadProgress, this, [=]( int bytesSent, int bytesTotal ) {
    mCloudProjects[index].uploadDeltaProgress = std::clamp( ( static_cast<double>( bytesSent ) / bytesTotal ), 0., 1. );

    emit dataChanged( idx, idx, QVector<int>() << UploadDeltaProgressRole );
  } );
  connect( deltasCloudReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *deltasReply = deltasCloudReply->reply();
    deltasCloudReply->deleteLater();

    Q_ASSERT( deltasCloudReply->isFinished() );
    Q_ASSERT( deltasReply );

    // if there is an error, cannot continue sync
    if ( deltasReply->error() != QNetworkReply::NoError )
    {
      mCloudProjects[index].deltaFileUploadStatusString = QFieldCloudConnection::errorString( deltasReply );
      // TODO check why exactly we failed
      // maybe the project does not exist, then create it?
      QgsMessageLog::logMessage( QStringLiteral( "Failed to upload delta file, reason:\n%1\n%2" ).arg( deltasReply->errorString(), mCloudProjects[index].deltaFileUploadStatusString ) );
      projectCancelUpload( projectId );
      return;
    }

    mCloudProjects[index].uploadDeltaProgress = 1;
    mCloudProjects[index].deltaFileUploadStatus = DeltaPendingStatus;
    mCloudProjects[index].deltaLayersToDownload = deltaFileWrapper->deltaLayerIds();

    emit dataChanged( idx, idx, QVector<int>() << UploadDeltaProgressRole << UploadDeltaStatusRole );
    emit networkDeltaUploaded( projectId );
  } );


  // //////////
  // 2) delta successfully uploaded
  // //////////
  QObject *networkDeltaUploadedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProjectsModel::networkDeltaUploaded, networkDeltaUploadedParent, [=]( const QString &uploadedProjectId ) {
    if ( projectId != uploadedProjectId )
      return;

    delete networkDeltaUploadedParent;

    // send attachment in a non-blocking fashion
    projectUploadAttachments( projectId );

    if ( shouldDownloadUpdates )
    {
      projectApplyDeltas( projectId );
    }
    else
    {
      mCloudProjects[index].status = ProjectStatus::Idle;
      mCloudProjects[index].modification ^= LocalModification;

      deltaFileWrapper->reset();
      deltaFileWrapper->resetId();

      if ( !deltaFileWrapper->toFile() )
        QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file after delta push. %1" ).arg( deltaFileWrapper->errorString() ) );

      QModelIndex idx = createIndex( index, 0 );

      emit dataChanged( idx, idx, QVector<int>() << StatusRole );
      emit pushFinished( projectId, false );
    }
  } );

  // //////////
  // 3) new delta status received. Never give up to get a successful status.
  // //////////
  QObject *networkDeltaStatusCheckedParent = new QObject( this ); // we need this to unsubscribe
  connect( this, &QFieldCloudProjectsModel::networkDeltaStatusChecked, networkDeltaStatusCheckedParent, [=]( const QString &uploadedProjectId ) {
    if ( projectId != uploadedProjectId )
      return;

    switch ( mCloudProjects[index].deltaFileUploadStatus )
    {
      case DeltaLocalStatus:
        // delta file should be already sent!!!
        Q_ASSERT( 0 );
        FALLTHROUGH
      case DeltaPendingStatus:
      case DeltaBusyStatus:
        // infinite retry, there should be one day, when we can get the status!
        QTimer::singleShot( sDelayBeforeStatusRetry, [=]() {
          projectGetDeltaStatus( projectId );
        } );
        break;
      case DeltaErrorStatus:
        delete networkDeltaStatusCheckedParent;
        deltaFileWrapper->resetId();

        if ( !deltaFileWrapper->toFile() )
          QgsMessageLog::logMessage( QStringLiteral( "Failed update committed delta file." ) );

        projectCancelUpload( projectId );
        return;
      case DeltaConflictStatus:
      case DeltaNotAppliedStatus:
      case DeltaAppliedStatus:
        delete networkDeltaStatusCheckedParent;

        deltaFileWrapper->reset();
        deltaFileWrapper->resetId();

        if ( !deltaFileWrapper->toFile() )
          QgsMessageLog::logMessage( QStringLiteral( "Failed to reset delta file. %1" ).arg( deltaFileWrapper->errorString() ) );

        mCloudProjects[index].status = ProjectStatus::Idle;
        mCloudProjects[index].modification ^= LocalModification;
        mCloudProjects[index].modification |= RemoteModification;

        mCloudProjects[index].lastLocalPushDeltas = QDateTime::currentDateTimeUtc().toString( Qt::ISODate );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastLocalPushDeltas" ), mCloudProjects[index].lastLocalPushDeltas );

        emit dataChanged( idx, idx, QVector<int>() << ModificationRole << LastLocalPushDeltasRole );

        // download the updated files, so the files are for sure the same on the client and on the server
        if ( shouldDownloadUpdates )
        {
          downloadProject( projectId );
        }
        else
        {
          QModelIndex idx = createIndex( index, 0 );
          emit dataChanged( idx, idx, QVector<int>() << StatusRole );

          emit pushFinished( projectId, false );
        }
    }
  } );


  // this code is no longer needed, as we do not upload or download files selectively
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  //  // //////////
  //  // 4) project downloaded, if all done, then reload the project and sync done!
  //  // //////////
  //  connect( this, &QFieldCloudProjectsModel::networkAllLayersDownloaded, this, [ = ]( const QString & callerProjectId )
  //  {
  //    if ( projectId != callerProjectId )
  //      return;

  //    // wait until all layers are downloaded
  //    if ( mCloudProjects[index].downloadLayersFinished < mCloudProjects[index].deltaLayersToDownload.size() )
  //      return;

  //    // there are some files that failed to download
  //    if ( mCloudProjects[index].downloadLayersFailed > 0 )
  //    {
  //      // TODO translate this message
  //      mCloudProjects[index].deltaFileUploadStatusString = QStringLiteral( "Failed to retrieve some of the updated layers, but changes are committed on the server. "
  //                            "Try to reload the project from the cloud." );
  //      projectCancelUpload( projectId );

  //      return;
  //    }

  //    QgsProject::instance()->reloadAllLayers();

  //    emit dataChanged( idx, idx, QVector<int>() << StatusRole );
  //    emit syncFinished( projectId, false );
  //  } );
}


void QFieldCloudProjectsModel::projectApplyDeltas( const QString &projectId )
{
  const int index = findProject( projectId );

  Q_ASSERT( index >= 0 && index < mCloudProjects.size() );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  QModelIndex idx = createIndex( index, 0 );
  NetworkReply *reply = mCloudConnection->post( QStringLiteral( "/api/v1/deltas/apply/%1/" ).arg( mCloudProjects[index].id ) );

  connect( reply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = reply->reply();
    reply->deleteLater();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      mCloudProjects[index].status = ProjectStatus::Idle;
      mCloudProjects[index].deltaFileUploadStatus = DeltaErrorStatus;
      mCloudProjects[index].deltaFileUploadStatusString = QFieldCloudConnection::errorString( rawReply );

      emit dataChanged( idx, idx, QVector<int>() << UploadDeltaStatusRole << UploadDeltaStatusStringRole );
      emit networkDeltaStatusChecked( projectId );
      return;
    }

    projectGetDeltaStatus( projectId );
  } );
}

void QFieldCloudProjectsModel::refreshProjectDeltaList( const QString &projectId )
{
  const int index = findProject( projectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mCloudProjects[index].id ) );

  connect( deltaStatusReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = deltaStatusReply->reply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    const int index = findProject( projectId );
    QModelIndex idx = createIndex( index, 0 );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );

    if ( mCloudProjects[index].deltaListModel )
      delete mCloudProjects[index].deltaListModel;

    mCloudProjects[index].deltaListModel = new DeltaListModel( doc );

    emit dataChanged( idx, idx, QVector<int>() << DeltaListRole );
    emit networkDeltaStatusChecked( projectId );
  } );
}

void QFieldCloudProjectsModel::projectGetDeltaStatus( const QString &projectId )
{
  const int index = findProject( projectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/%2/" ).arg( mCloudProjects[index].id, mCloudProjects[index].deltaFileId ) );

  mCloudProjects[index].deltaFileUploadStatusString = QString();
  connect( deltaStatusReply, &NetworkReply::finished, this, [=]() {
    QNetworkReply *rawReply = deltaStatusReply->reply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    const int index = findProject( projectId );
    QModelIndex idx = createIndex( index, 0 );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      mCloudProjects[index].deltaFileUploadStatus = DeltaErrorStatus;
      // TODO this is oversimplification. e.g. 404 error is when the requested delta file id is not existant
      mCloudProjects[index].deltaFileUploadStatusString = QFieldCloudConnection::errorString( rawReply );

      emit dataChanged( idx, idx, QVector<int>() << UploadDeltaStatusRole << UploadDeltaStatusStringRole );
      emit networkDeltaStatusChecked( projectId );

      return;
    }

    const QJsonDocument doc = QJsonDocument::fromJson( rawReply->readAll() );
    DeltaListModel deltaListModel( doc );
    if ( !deltaListModel.isValid() )
    {
      mCloudProjects[index].deltaFileUploadStatus = DeltaErrorStatus;
      mCloudProjects[index].deltaFileUploadStatusString = deltaListModel.errorString();
      emit dataChanged( idx, idx, QVector<int>() << UploadDeltaStatusRole << UploadDeltaStatusStringRole );
      emit networkDeltaStatusChecked( projectId );
      return;
    }

    mCloudProjects[index].deltaFileUploadStatusString = QString();

    if ( !deltaListModel.allHaveFinalStatus() )
    {
      mCloudProjects[index].deltaFileUploadStatus = DeltaPendingStatus;
      emit dataChanged( idx, idx, QVector<int>() << UploadDeltaStatusRole << UploadDeltaStatusStringRole );
      emit networkDeltaStatusChecked( projectId );
      return;
    }

    mCloudProjects[index].deltaFileUploadStatus = DeltaAppliedStatus;

    emit dataChanged( idx, idx, QVector<int>() << UploadDeltaStatusRole << UploadDeltaStatusStringRole );
    emit networkDeltaStatusChecked( projectId );
  } );
}

void QFieldCloudProjectsModel::projectUploadAttachments( const QString &projectId )
{
  const int index = findProject( projectId );

  Q_ASSERT( index >= 0 && index < mCloudProjects.size() );

  if ( !mCloudConnection || index < 0 || index >= mCloudProjects.size() || mCloudProjects[index].uploadAttachments.size() == 0 )
    return;

  QModelIndex idx = createIndex( index, 0 );

  // start uploading the attachments
  const QStringList attachmentFileNames = mCloudProjects[index].uploadAttachments.keys();
  for ( const QString &fileName : attachmentFileNames )
  {
    NetworkReply *attachmentCloudReply = uploadAttachment( projectId, fileName );
    mCloudProjects[index].uploadAttachments[fileName].networkReply = attachmentCloudReply;

    connect( attachmentCloudReply, &NetworkReply::uploadProgress, this, [=]( int bytesSent, int bytesTotal ) {
      Q_UNUSED( bytesTotal )
      mCloudProjects[index].uploadAttachments[fileName].bytesTransferred = bytesSent;
      emit dataChanged( idx, idx, QVector<int>() << UploadAttachmentsCountRole );
    } );

    connect( attachmentCloudReply, &NetworkReply::finished, this, [=]() {
      QNetworkReply *attachmentReply = attachmentCloudReply->reply();
      attachmentCloudReply->deleteLater();

      Q_ASSERT( attachmentCloudReply->isFinished() );
      Q_ASSERT( attachmentReply );

      // if there is an error, don't panic, we continue uploading. The files may be later manually synced.
      if ( attachmentReply->error() != QNetworkReply::NoError )
      {
        mCloudProjects[index].uploadAttachmentsFailed++;
        QgsMessageLog::logMessage( tr( "Failed to upload attachment stored at \"%1\", reason:\n%2" )
                                   .arg( fileName )
                                   .arg( QFieldCloudConnection::errorString( attachmentReply ) ) );
      }
      else
      {
        mCloudProjects[index].uploadAttachments.remove( fileName );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "uploadAttachments" ), QStringList( mCloudProjects[index].uploadAttachments.keys() ) );
      }

      if ( mCloudProjects[index].uploadAttachments.size() - mCloudProjects[index].uploadAttachmentsFailed == 0 )
        mCloudProjects[index].uploadAttachmentsStatus = UploadAttachmentsStatus::UploadAttachmentsDone;

      emit dataChanged( idx, idx, QVector<int>() << UploadAttachmentsCountRole );
    } );
  }
  if ( attachmentFileNames.size() > 0 )
  {
    mCloudProjects[index].uploadAttachmentsStatus = UploadAttachmentsStatus::UploadAttachmentsInProgress;
    emit dataChanged( idx, idx, QVector<int>() << UploadAttachmentsStatusRole );
  }
}

void QFieldCloudProjectsModel::projectCancelUpload( const QString &projectId )
{
  int index = findProject( projectId );
  if ( !mCloudConnection || index < 0 || index >= mCloudProjects.size() )
    return;

  projectCancelUploadAttachments( projectId );

  mCloudProjects[index].status = ProjectStatus::Idle;
  mCloudProjects[index].errorStatus = UploadErrorStatus;

  QModelIndex idx = createIndex( index, 0 );
  emit dataChanged( idx, idx, QVector<int>() << StatusRole << ErrorStatusRole );
  emit pushFinished( projectId, true, mCloudProjects[index].deltaFileUploadStatusString );

  return;
}

void QFieldCloudProjectsModel::projectCancelUploadAttachments( const QString &projectId )
{
  int index = findProject( projectId );
  if ( !mCloudConnection || index < 0 || index >= mCloudProjects.size() )
    return;

  const QStringList attachmentFileNames = mCloudProjects[index].uploadAttachments.keys();
  for ( const QString &attachmentFileName : attachmentFileNames )
  {
    NetworkReply *attachmentReply = mCloudProjects[index].uploadAttachments[attachmentFileName].networkReply;

    // the replies might be already disposed
    if ( !attachmentReply )
      continue;

    // the replies might be already finished
    if ( attachmentReply->isFinished() )
      continue;

    attachmentReply->abort();
  }
  mCloudProjects[index].uploadAttachmentsStatus = UploadAttachmentsStatus::UploadAttachmentsDone;
  QModelIndex idx = createIndex( index, 0 );
  emit dataChanged( idx, idx, QVector<int>() << UploadAttachmentsStatusRole );
}

void QFieldCloudProjectsModel::connectionStatusChanged()
{
  refreshProjectsList();
}

void QFieldCloudProjectsModel::layerObserverLayerEdited( const QString &layerId )
{
  Q_UNUSED( layerId )

  const int index = findProject( mCurrentProjectId );

  if ( index < 0 || index >= mCloudProjects.size() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "Layer observer triggered `isDirtyChanged` signal incorrectly" ) );
    return;
  }

  beginResetModel();

  const DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  Q_ASSERT( deltaFileWrapper );

  if ( deltaFileWrapper->count() > 0 )
    mCloudProjects[index].modification |= LocalModification;
  else if ( mCloudProjects[index].modification & LocalModification )
    mCloudProjects[index].modification ^= LocalModification;

  endResetModel();
}

void QFieldCloudProjectsModel::projectListReceived()
{
  NetworkReply *reply = qobject_cast<NetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->reply();

  Q_ASSERT( rawReply );

  if ( rawReply->error() != QNetworkReply::NoError )
  {
    emit warning( QFieldCloudConnection::errorString( rawReply ) );
    return;
  }

  QByteArray response = rawReply->readAll();

  QJsonDocument doc = QJsonDocument::fromJson( response );
  QJsonArray projects = doc.array();
  reload( projects );
}

NetworkReply *QFieldCloudProjectsModel::downloadFile( const QString &projectId, const QString &fileName )
{
  QNetworkRequest request;
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::UserVerifiedRedirectPolicy );
  mCloudConnection->setAuthenticationToken( request );

  return mCloudConnection->get( request, QStringLiteral( "/api/v1/qfield-files/%1/%2/" ).arg( projectId, fileName ) );
}


void QFieldCloudProjectsModel::downloadFileConnections( const QString &projectId, const QString &fileName )
{
  const int index = findProject( projectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return;

  if ( !mCloudProjects[index].downloadFileTransfers.contains( fileName ) )
  {
    Q_ASSERT( false );
    return;
  }

  NetworkReply *reply = mCloudProjects[index].downloadFileTransfers[fileName].networkReply;

  if ( !reply )
  {
    Q_ASSERT( false );
    return;
  }

  QStringList fileNames = mCloudProjects[index].downloadFileTransfers.keys();

  QgsLogger::debug( QStringLiteral( "File \"%1\" requested." ).arg( fileName ) );

  connect( reply, &NetworkReply::redirected, reply, [=]( const QUrl &url ) {
    QUrl oldUrl = mCloudProjects[index].downloadFileTransfers[fileName].lastRedirectUrl;

    mCloudProjects[index].downloadFileTransfers[fileName].redirectsCount++;
    mCloudProjects[index].downloadFileTransfers[fileName].lastRedirectUrl = url;

    if ( mCloudProjects[index].downloadFileTransfers[fileName].redirectsCount >= MAX_REDIRECTS_ALLOWED )
    {
      QgsLogger::debug( QStringLiteral( "Export file \"%1/%2\" has too many redirects, last two urls are %3 and %4" ).arg( projectId, fileName, oldUrl.toString(), url.toString() ) );
      reply->abort();
      return;
    }

    if ( oldUrl == url )
    {
      QgsLogger::debug( QStringLiteral( "Export file \"%1/%2\" has redirects to the same URL %3" ).arg( projectId, fileName, url.toString() ) );
      reply->abort();
      return;
    }

    QgsLogger::debug( QStringLiteral( "Export file \"%1/%2\" redirected to %3" ).arg( projectId, fileName, url.toString() ) );

    QNetworkRequest request;
    mCloudProjects[index].downloadFileTransfers[fileName].networkReply = mCloudConnection->get( request, url );

    // we need to somehow finish the request, otherwise it will remain unfinished for the QFieldCloudConnection
    reply->abort();

    downloadFileConnections( projectId, fileName );
  } );

  connect( reply, &NetworkReply::downloadProgress, reply, [=]( int bytesReceived, int bytesTotal ) {
    Q_UNUSED( bytesTotal )

    // it means the NetworkReply has failed and retried
    mCloudProjects[index].downloadBytesReceived -= mCloudProjects[index].downloadFileTransfers[fileName].bytesTransferred;
    mCloudProjects[index].downloadBytesReceived += bytesReceived;
    mCloudProjects[index].downloadFileTransfers[fileName].bytesTransferred = bytesReceived;
    mCloudProjects[index].downloadProgress = std::clamp( ( static_cast<double>( mCloudProjects[index].downloadBytesReceived ) / std::max( mCloudProjects[index].downloadBytesTotal, 1 ) ), 0., 1. );

    QModelIndex idx = createIndex( index, 0 );

    emit dataChanged( idx, idx, QVector<int>() << DownloadProgressRole );
  } );

  connect( reply, &NetworkReply::finished, reply, [=]() {
    if ( mCloudProjects[index].packagingStatus == PackagingAbortStatus )
      return;

    QVector<int> rolesChanged;
    QNetworkReply *rawReply = reply->reply();

    Q_ASSERT( reply->isFinished() );
    Q_ASSERT( reply );

    // this is most probably the redirected request, nothing to do with this reply anymore, just ignore it
    if ( mCloudProjects[index].downloadFileTransfers[fileName].networkReply != reply )
      return;

    mCloudProjects[index].downloadFilesFinished++;

    bool hasError = false;
    QString errorMessageDetail;
    QString errorMessageTemplate;

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      hasError = true;
      errorMessageDetail = QFieldCloudConnection::errorString( rawReply );
      errorMessageTemplate = tr( "Failed to download file \"%1\", reason:" ).arg( fileName );
    }

    QFile file( mCloudProjects[index].downloadFileTransfers[fileName].tmpFile );

    file.open( QIODevice::ReadWrite );

    if ( !hasError && !file.write( rawReply->readAll() ) )
    {
      hasError = true;
      errorMessageDetail = file.errorString();
      errorMessageTemplate = tr( "Failed to write downloaded file stored at \"%1\", fs reason:" ).arg( fileName );
    }

    if ( hasError )
    {
      QString trimmedErrorMessage = errorMessageDetail.size() > 100
                                    ? ( errorMessageDetail.left( 100 ) + tr( " (see more in the QField error log)…" ) )
                                    : errorMessageDetail;
      mCloudProjects[index].downloadFilesFailed++;
      mCloudProjects[index].errorStatus = DownloadErrorStatus;
      mCloudProjects[index].packagingStatusString = QStringLiteral( "%1: \n%2" ).arg( errorMessageTemplate, fileName ).arg( trimmedErrorMessage );

      QgsLogger::debug( QStringLiteral( "%1: \n%2" ).arg( errorMessageTemplate, fileName ).arg( errorMessageDetail ) );
      QgsMessageLog::logMessage( QStringLiteral( "%1: \n%2" ).arg( errorMessageTemplate, fileName ).arg( errorMessageDetail ) );

      rolesChanged << StatusRole << LocalPathRole << CheckoutRole << ErrorStringRole;

      emit projectDownloaded( projectId, mCloudProjects[index].name, true, trimmedErrorMessage );
    }

    QgsLogger::debug( QStringLiteral( "File \"%1\" downloaded." ).arg( fileName ) );

    updateActiveProjectFilesToDownload( projectId );

    if ( mCloudProjects[index].downloadFilesFinished == fileNames.count() )
    {
      QgsLogger::debug( QStringLiteral( "All files downloaded." ) );

      Q_ASSERT( mActiveProjectFilesToDownload.size() == 0 );

      if ( !hasError )
      {
        const QStringList unprefixedGpkgFileNames = filterGpkgFileNames( fileNames );
        const QStringList gpkgFileNames = projectFileNames( mProject->homePath(), unprefixedGpkgFileNames );
        // we need to close the project to safely flush the gpkg files
        mProject->setFileName( QString() );

        for ( const QString &fileName : gpkgFileNames )
          mGpkgFlusher->stop( fileName );

        // move the files from their temporary location to their permanent one
        if ( !projectMoveDownloadedFilesToPermanentStorage( projectId ) )
        {
          mCloudProjects[index].errorStatus = DownloadErrorStatus;
          mCloudProjects[index].packagingStatus = PackagingErrorStatus;
          mCloudProjects[index].packagingStatusString = tr( "Failed to copy some of the downloaded files on your device. Check your device storage." );
          emit projectDownloaded( projectId, mCloudProjects[index].name, true, mCloudProjects[index].packagingStatusString );
          return;
        }

        deleteGpkgShmAndWal( gpkgFileNames );

        for ( const QString &fileName : gpkgFileNames )
          mGpkgFlusher->start( fileName );

        mCloudProjects[index].errorStatus = NoErrorStatus;
        mCloudProjects[index].packagingStatus = PackagingFinishedStatus;
        mCloudProjects[index].packagingStatusString = QString();
        mCloudProjects[index].checkout = ProjectCheckout::LocalAndRemoteCheckout;
        mCloudProjects[index].localPath = QFieldCloudUtils::localProjectFilePath( mUsername, projectId );
        mCloudProjects[index].lastLocalExportedAt = QDateTime::currentDateTimeUtc().toString( Qt::ISODate );
        mCloudProjects[index].lastLocalExportId = QUuid::createUuid().toString( QUuid::WithoutBraces );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastExportedAt" ), mCloudProjects[index].lastExportedAt );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastExportId" ), mCloudProjects[index].lastExportId );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastLocalExportedAt" ), mCloudProjects[index].lastLocalExportedAt );
        QFieldCloudUtils::setProjectSetting( projectId, QStringLiteral( "lastLocalExportId" ), mCloudProjects[index].lastLocalExportId );

        emit projectDownloaded( projectId, mCloudProjects[index].name, false );
      }

      for ( const QString &fileNameKey : fileNames )
      {
        mCloudProjects[index].downloadFileTransfers[fileNameKey].networkReply->deleteLater();
      }

      mCloudProjects[index].status = ProjectStatus::Idle;
    }
    else
    {
      projectDownloadFiles( projectId );
    }

    QModelIndex idx = createIndex( index, 0 );
    rolesChanged << StatusRole << LocalPathRole << CheckoutRole << LastLocalExportedAtRole;

    emit dataChanged( idx, idx, rolesChanged );
  } );
}

NetworkReply *QFieldCloudProjectsModel::uploadAttachment( const QString &projectId, const QString &fileName )
{
  QFileInfo projectInfo( QFieldCloudUtils::localProjectFilePath( mUsername, projectId ) );
  QDir projectDir( projectInfo.absolutePath() );

  const QString apiPath = projectDir.relativeFilePath( fileName );

  return mCloudConnection->post( QStringLiteral( "/api/v1/files/%1/%2/" ).arg( projectId, apiPath ), QVariantMap(), QStringList( { fileName } ) );
}

QHash<int, QByteArray> QFieldCloudProjectsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IdRole] = "Id";
  roles[PrivateRole] = "Private";
  roles[OwnerRole] = "Owner";
  roles[NameRole] = "Name";
  roles[DescriptionRole] = "Description";
  roles[ModificationRole] = "Modification";
  roles[CheckoutRole] = "Checkout";
  roles[StatusRole] = "Status";
  roles[ErrorStatusRole] = "ErrorStatus";
  roles[ErrorStringRole] = "ErrorString";
  roles[DownloadProgressRole] = "DownloadProgress";
  roles[PackagingStatusRole] = "PackagingStatus";
  roles[PackagedLayerErrorsRole] = "PackagedLayerErrors";
  roles[UploadAttachmentsStatusRole] = "UploadAttachmentsStatus";
  roles[UploadAttachmentsCountRole] = "UploadAttachmentsCount";
  roles[UploadDeltaProgressRole] = "UploadDeltaProgress";
  roles[UploadDeltaStatusRole] = "UploadDeltaStatus";
  roles[UploadDeltaStatusStringRole] = "UploadDeltaStatusString";
  roles[LocalDeltasCountRole] = "LocalDeltasCount";
  roles[LocalPathRole] = "LocalPath";
  roles[CanSyncRole] = "CanSync";
  roles[LastLocalExportedAtRole] = "LastLocalExportedAt";
  roles[LastLocalPushDeltasRole] = "LastLocalPushDeltas";
  roles[UserRoleRole] = "UserRole";
  roles[DeltaListRole] = "DeltaList";

  return roles;
}

void QFieldCloudProjectsModel::reload( const QJsonArray &remoteProjects )
{
  beginResetModel();
  mCloudProjects.clear();

  QgsProject *qgisProject = QgsProject::instance();

  auto restoreLocalSettings = [=]( CloudProject &cloudProject, const QDir &localPath ) {
    cloudProject.deltasCount = DeltaFileWrapper( qgisProject, QStringLiteral( "%1/deltafile.json" ).arg( localPath.absolutePath() ) ).count();
    cloudProject.lastExportId = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "lastExportId" ) ).toString();
    cloudProject.lastExportedAt = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "lastExportedAt" ) ).toString();
    cloudProject.lastLocalExportId = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "lastLocalExportId" ) ).toString();
    cloudProject.lastLocalExportedAt = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "lastLocalExportedAt" ) ).toString();
    cloudProject.lastLocalPushDeltas = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "lastLocalPushDeltas" ) ).toString();

    // generate local export id if not present. Possible reasons for missing localExportId are:
    // - just upgraded QField that introduced the field
    // - the local settings were somehow deleted, but not the project itself
    if ( cloudProject.lastLocalExportId.isEmpty() )
    {
      cloudProject.lastLocalExportId = QUuid::createUuid().toString( QUuid::WithoutBraces );
      QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "lastLocalExportId" ), cloudProject.lastLocalExportId );
    }

    const QStringList fileNames = QFieldCloudUtils::projectSetting( cloudProject.id, QStringLiteral( "uploadAttachments" ) ).toStringList();
    for ( const QString &fileName : fileNames )
    {
      QFileInfo fileInfo( fileName );
      if ( fileInfo.exists() && !fileInfo.isDir() )
      {
        cloudProject.uploadAttachments.insert( fileName, FileTransfer( fileName, fileInfo.size() ) );
      }
    }
  };

  for ( const auto project : remoteProjects )
  {
    QVariantHash projectDetails = project.toObject().toVariantHash();
    CloudProject cloudProject( projectDetails.value( "id" ).toString(),
                               projectDetails.value( "private" ).toBool(),
                               projectDetails.value( "owner" ).toString(),
                               projectDetails.value( "name" ).toString(),
                               projectDetails.value( "description" ).toString(),
                               projectDetails.value( "user_role" ).toString(),
                               QString(),
                               RemoteCheckout,
                               ProjectStatus::Idle );

    const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( cloudProject.id );
    QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "owner" ), cloudProject.owner );
    QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "name" ), cloudProject.name );
    QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "description" ), cloudProject.description );
    QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "updatedAt" ), cloudProject.updatedAt );
    QFieldCloudUtils::setProjectSetting( cloudProject.id, QStringLiteral( "userRole" ), cloudProject.userRole );

    if ( !mUsername.isEmpty() )
    {
      cloudProject.localPath = QFieldCloudUtils::localProjectFilePath( mUsername, cloudProject.id );
      QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, cloudProject.id ) );
      if ( localPath.exists() )
      {
        cloudProject.checkout = LocalAndRemoteCheckout;
        restoreLocalSettings( cloudProject, localPath );
      }
    }

    mCloudProjects << cloudProject;
  }

  QDirIterator userDirs( QFieldCloudUtils::localCloudDirectory(), QDir::Dirs | QDir::NoDotAndDotDot );
  while ( userDirs.hasNext() )
  {
    userDirs.next();
    const QString username = userDirs.fileName();

    // We skip cloud projects that are not linked to the last successul logged in account
    if ( username != mUsername )
      continue;

    QDirIterator projectDirs( QStringLiteral( "%1/%2" ).arg( QFieldCloudUtils::localCloudDirectory(), username ), QDir::Dirs | QDir::NoDotAndDotDot );
    while ( projectDirs.hasNext() )
    {
      projectDirs.next();

      const QString projectId = projectDirs.fileName();
      int index = findProject( projectId );
      if ( index != -1 )
        continue;

      const QString projectPrefix = QStringLiteral( "QFieldCloud/projects/%1" ).arg( projectId );
      if ( !QSettings().contains( QStringLiteral( "%1/name" ).arg( projectPrefix ) ) )
        continue;

      const QString owner = QFieldCloudUtils::projectSetting( projectId, QStringLiteral( "owner" ) ).toString();
      const QString name = QFieldCloudUtils::projectSetting( projectId, QStringLiteral( "name" ) ).toString();
      const QString description = QFieldCloudUtils::projectSetting( projectId, QStringLiteral( "description" ) ).toString();
      const QString updatedAt = QFieldCloudUtils::projectSetting( projectId, QStringLiteral( "updatedAt" ) ).toString();
      const QString userRole = QFieldCloudUtils::projectSetting( projectId, QStringLiteral( "userRole" ) ).toString();

      CloudProject cloudProject( projectId, true, owner, name, description, userRole, QString(), LocalCheckout, ProjectStatus::Idle );

      cloudProject.localPath = QFieldCloudUtils::localProjectFilePath( username, cloudProject.id );
      QDir localPath( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), username, cloudProject.id ) );
      restoreLocalSettings( cloudProject, localPath );

      mCloudProjects << cloudProject;

      Q_ASSERT( projectId == cloudProject.id );
    }
  }

  endResetModel();
}

int QFieldCloudProjectsModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mCloudProjects.size();
  else
    return 0;
}

QVariant QFieldCloudProjectsModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mCloudProjects.size() || index.row() < 0 )
    return QVariant();

  switch ( static_cast<ColumnRole>( role ) )
  {
    case IdRole:
      return mCloudProjects.at( index.row() ).id;
    case PrivateRole:
      return mCloudProjects.at( index.row() ).isPrivate;
    case OwnerRole:
      return mCloudProjects.at( index.row() ).owner;
    case NameRole:
      return mCloudProjects.at( index.row() ).name;
    case DescriptionRole:
      return mCloudProjects.at( index.row() ).description;
    case ModificationRole:
      return static_cast<int>( mCloudProjects.at( index.row() ).modification );
    case CheckoutRole:
      return static_cast<int>( mCloudProjects.at( index.row() ).checkout );
    case StatusRole:
      return static_cast<int>( mCloudProjects.at( index.row() ).status );
    case ErrorStatusRole:
      return static_cast<int>( mCloudProjects.at( index.row() ).errorStatus );
    case ErrorStringRole:
      return mCloudProjects.at( index.row() ).errorStatus == DownloadErrorStatus
             ? mCloudProjects.at( index.row() ).packagingStatusString
             : mCloudProjects.at( index.row() ).errorStatus == UploadErrorStatus
               ? mCloudProjects.at( index.row() ).deltaFileUploadStatusString
               : QString();
    case PackagingStatusRole:
      return mCloudProjects.at( index.row() ).packagingStatus;
    case PackagedLayerErrorsRole:
      return QVariant( mCloudProjects.at( index.row() ).packagedLayerErrors );
    case DownloadProgressRole:
      return mCloudProjects.at( index.row() ).downloadProgress;
    case UploadAttachmentsStatusRole:
      return mCloudProjects.at( index.row() ).uploadAttachmentsStatus;
    case UploadAttachmentsCountRole:
      return mCloudProjects.at( index.row() ).uploadAttachments.size() - mCloudProjects[index.row()].uploadAttachmentsFailed;
    case UploadDeltaProgressRole:
      return mCloudProjects.at( index.row() ).uploadDeltaProgress;
    case UploadDeltaStatusRole:
      return mCloudProjects.at( index.row() ).deltaFileUploadStatus;
    case UploadDeltaStatusStringRole:
      return mCloudProjects.at( index.row() ).deltaFileUploadStatusString;
    case LocalDeltasCountRole:
      return mCloudProjects.at( index.row() ).deltasCount;
    case LocalPathRole:
      return mCloudProjects.at( index.row() ).localPath;
    case CanSyncRole:
      return canSyncProject( mCloudProjects.at( index.row() ).id );
    case LastLocalExportedAtRole:
      return mCloudProjects.at( index.row() ).lastLocalExportedAt;
    case LastLocalPushDeltasRole:
      return mCloudProjects.at( index.row() ).lastLocalPushDeltas;
    case UserRoleRole:
      return mCloudProjects.at( index.row() ).userRole;
    case DeltaListRole:
      return QVariant::fromValue<DeltaListModel *>( mCloudProjects.at( index.row() ).deltaListModel );
  }

  return QVariant();
}

bool QFieldCloudProjectsModel::revertLocalChangesFromCurrentProject()
{
  const int index = findProject( mCurrentProjectId );

  if ( index < 0 || index >= mCloudProjects.size() )
    return false;

  DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  if ( !deltaFileWrapper->toFile() )
    return false;

  if ( !deltaFileWrapper->applyReversed() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "Failed to apply reversed" ) );
    return false;
  }

  mCloudProjects[index].modification ^= LocalModification;

  deltaFileWrapper->reset();
  deltaFileWrapper->resetId();

  if ( !deltaFileWrapper->toFile() )
    return false;

  return true;
}

bool QFieldCloudProjectsModel::discardLocalChangesFromCurrentProject()
{
  const int index = findProject( mCurrentProjectId );
  if ( index < 0 || index >= mCloudProjects.size() )
    return false;

  DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  if ( !deltaFileWrapper->toFile() )
    QgsMessageLog::logMessage( QStringLiteral( "Failed to write deltas." ) );

  mCloudProjects[index].modification ^= LocalModification;

  deltaFileWrapper->reset();
  deltaFileWrapper->resetId();

  if ( !deltaFileWrapper->toFile() )
    return false;

  return true;
}

void QFieldCloudProjectsModel::setGpkgFlusher( QgsGpkgFlusher *flusher )
{
  if ( mGpkgFlusher == flusher )
    return;

  mGpkgFlusher = flusher;

  emit gpkgFlusherChanged();
}

bool QFieldCloudProjectsModel::deleteGpkgShmAndWal( const QStringList &gpkgFileNames )
{
  bool isSuccess = true;

  for ( const QString &fileName : gpkgFileNames )
  {
    QFile shmFile( QStringLiteral( "%1-shm" ).arg( fileName ) );
    if ( shmFile.exists() )
    {
      if ( !shmFile.remove() )
      {
        QgsMessageLog::logMessage( QStringLiteral( "Failed to remove -shm file '%1' " ).arg( shmFile.fileName() ) );
        isSuccess = false;
      }
    }

    QFile walFile( QStringLiteral( "%1-wal" ).arg( fileName ) );

    if ( walFile.exists() )
    {
      if ( !walFile.remove() )
      {
        QgsMessageLog::logMessage( QStringLiteral( "Failed to remove -wal file '%1' " ).arg( walFile.fileName() ) );
        isSuccess = false;
      }
    }
  }

  return isSuccess;
}

QStringList QFieldCloudProjectsModel::filterGpkgFileNames( const QStringList &fileNames ) const
{
  QStringList gpkgFileNames;

  for ( const QString &fileName : fileNames )
  {
    if ( fileName.endsWith( QStringLiteral( ".gpkg" ) ) )
    {
      gpkgFileNames.append( fileName );
    }
  }

  return gpkgFileNames;
}

QStringList QFieldCloudProjectsModel::projectFileNames( const QString &projectPath, const QStringList &fileNames ) const
{
  QStringList prefixedFileNames;

  for ( const QString &fileName : fileNames )
  {
    prefixedFileNames.append( QStringLiteral( "%1/%2" ).arg( projectPath, fileName ) );
  }

  return prefixedFileNames;
}

// --

QFieldCloudProjectsFilterModel::QFieldCloudProjectsFilterModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
}

void QFieldCloudProjectsFilterModel::setProjectsModel( QFieldCloudProjectsModel *projectsModel )
{
  if ( mSourceModel == projectsModel )
    return;

  mSourceModel = projectsModel;
  setSourceModel( mSourceModel );

  emit projectsModelChanged();
}

QFieldCloudProjectsModel *QFieldCloudProjectsFilterModel::projectsModel() const
{
  return mSourceModel;
}

void QFieldCloudProjectsFilterModel::setFilter( ProjectsFilter filter )
{
  if ( mFilter == filter )
    return;

  mFilter = filter;
  invalidateFilter();

  emit filterChanged();
}

QFieldCloudProjectsFilterModel::ProjectsFilter QFieldCloudProjectsFilterModel::filter() const
{
  return mFilter;
}

void QFieldCloudProjectsFilterModel::setShowLocalOnly( bool showLocalOnly )
{
  if ( mShowLocalOnly == showLocalOnly )
    return;

  mShowLocalOnly = showLocalOnly;
  invalidateFilter();

  emit showLocalOnlyChanged();
}

bool QFieldCloudProjectsFilterModel::showLocalOnly() const
{
  return mShowLocalOnly;
}

bool QFieldCloudProjectsFilterModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( mShowLocalOnly && mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QFieldCloudProjectsModel::LocalPathRole ).toString().isEmpty() )
  {
    return false;
  }

  bool ok = false;
  switch ( mFilter )
  {
    case PrivateProjects:
      // the list will include public "community" projects that are present locally so they can appear in the "My projects" list
      ok = mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QFieldCloudProjectsModel::PrivateRole ).toBool() || !mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QFieldCloudProjectsModel::LocalPathRole ).toString().isEmpty();
      break;
    case PublicProjects:
      ok = !mSourceModel->data( mSourceModel->index( source_row, 0, source_parent ), QFieldCloudProjectsModel::PrivateRole ).toBool();
      break;
  }
  return ok;
}
