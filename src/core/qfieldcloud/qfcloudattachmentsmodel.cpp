/***************************************************************************
    qfcloudattachmentsmodel.cpp
    ---------------------
    begin                : October 2026
    copyright            : (C) 2026 by Kaustuv Pokharel
    email                : kaustuv@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfcloudattachmentsmodel.h"
#include "qfcloudutils.h"
#include "qfdeltafilewrapper.h"

#include <QDir>
#include <QFileInfo>
#include <QSet>


QfCloudAttachmentsModel::QfCloudAttachmentsModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

int QfCloudAttachmentsModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mAttachments.size() );
}

QVariant QfCloudAttachmentsModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mAttachments.size() || index.row() < 0 )
    return QVariant();

  switch ( static_cast<ColumnRole>( role ) )
  {
    case FilePathRole:
      return mAttachments.at( index.row() ).filePath;
    case FileNameRole:
      return mAttachments.at( index.row() ).fileName;
    case StatusRole:
      return mAttachments.at( index.row() ).status;
    case HttpCodeRole:
      return mAttachments.at( index.row() ).httpCode;
  }

  return QVariant();
}

QHash<int, QByteArray> QfCloudAttachmentsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[FilePathRole] = "FilePath";
  roles[FileNameRole] = "FileName";
  roles[StatusRole] = "Status";
  roles[HttpCodeRole] = "HttpCode";
  return roles;
}

void QfCloudAttachmentsModel::refresh()
{
  beginResetModel();
  mAttachments.clear();

  if ( mCloudConnection && !mCloudProjectId.isEmpty() )
  {
    const QString username = mCloudConnection->username();
    const QString projectFilePath = QfCloudUtils::localProjectFilePath( username, mCloudProjectId );
    const QDir projectDir( !projectFilePath.isEmpty() ? QFileInfo( projectFilePath ).absolutePath() : QString() );
    auto displayName = [&projectFilePath, &projectDir]( const QString &filePath ) {
      return !projectFilePath.isEmpty() ? projectDir.relativeFilePath( filePath ) : QFileInfo( filePath ).fileName();
    };

    // A file queued for upload is pending, whatever the outcome of its previous uploads
    QSet<QString> listedFilePaths;
    const QStringList pendingFilePaths = QfCloudUtils::getPendingAttachments( username ).values( mCloudProjectId );
    for ( const QString &filePath : pendingFilePaths )
    {
      if ( listedFilePaths.contains( filePath ) )
      {
        continue;
      }
      listedFilePaths.insert( filePath );

      Attachment attachment;
      attachment.filePath = filePath;
      attachment.fileName = displayName( filePath );
      attachment.status = PendingStatus;
      mAttachments << attachment;
    }

    // Attachments of changes not yet uploaded only get queued once the changes are pushed
    if ( mDeltaFileWrapper && mDeltaFileWrapper->projectId() == mCloudProjectId )
    {
      const QStringList localFileNames = mDeltaFileWrapper->attachmentFileNames().keys();
      for ( const QString &localFileName : localFileNames )
      {
        if ( localFileName.isEmpty() )
        {
          continue;
        }

        const QString filePath = QFileInfo( localFileName ).isRelative() && !projectFilePath.isEmpty() ? projectDir.absoluteFilePath( localFileName ) : localFileName;
        if ( listedFilePaths.contains( filePath ) || !QFileInfo( filePath ).isFile() )
        {
          continue;
        }
        listedFilePaths.insert( filePath );

        Attachment attachment;
        attachment.filePath = filePath;
        attachment.fileName = displayName( filePath );
        attachment.status = LocalStatus;
        mAttachments << attachment;
      }
    }

    // Only the most recent upload outcome of a file is listed, newest first
    const QList<QfCloudAttachmentHistoryEntry> history = QfCloudUtils::getAttachmentsHistory( username, mCloudProjectId );
    for ( auto it = history.crbegin(); it != history.crend(); ++it )
    {
      if ( listedFilePaths.contains( it->fileName ) )
      {
        continue;
      }
      listedFilePaths.insert( it->fileName );

      Attachment attachment;
      attachment.filePath = it->fileName;
      attachment.fileName = displayName( it->fileName );
      attachment.status = it->httpCode >= 200 && it->httpCode < 300 ? UploadedStatus : FailedStatus;
      attachment.httpCode = it->httpCode;
      mAttachments << attachment;
    }
  }

  endResetModel();
}

void QfCloudAttachmentsModel::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, nullptr, this, nullptr );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::pendingAttachmentsAdded, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::pendingAttachmentsUploadFinished, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::attachmentsHistoryChanged, this, [this]( const QString &projectId ) {
      if ( projectId == mCloudProjectId )
      {
        refresh();
      }
    } );
  }

  emit cloudConnectionChanged();
  refresh();
}

QfDeltaFileWrapper *QfCloudAttachmentsModel::deltaFileWrapper() const
{
  return mDeltaFileWrapper;
}

void QfCloudAttachmentsModel::setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper )
{
  if ( mDeltaFileWrapper == deltaFileWrapper )
  {
    return;
  }

  if ( mDeltaFileWrapper )
  {
    disconnect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfCloudAttachmentsModel::refresh );
  }

  mDeltaFileWrapper = deltaFileWrapper;

  if ( mDeltaFileWrapper )
  {
    connect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfCloudAttachmentsModel::refresh );
  }

  emit deltaFileWrapperChanged();
  refresh();
}

void QfCloudAttachmentsModel::setCloudProjectId( const QString &cloudProjectId )
{
  if ( mCloudProjectId == cloudProjectId )
  {
    return;
  }

  mCloudProjectId = cloudProjectId;
  emit cloudProjectIdChanged();
  refresh();
}
