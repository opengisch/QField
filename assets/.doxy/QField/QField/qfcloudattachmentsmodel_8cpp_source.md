

# File qfcloudattachmentsmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudattachmentsmodel.cpp**](qfcloudattachmentsmodel_8cpp.md)

[Go to the documentation of this file](qfcloudattachmentsmodel_8cpp.md)


```C++
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
  {
    return QVariant();
  }

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
    case TimestampRole:
      return mAttachments.at( index.row() ).timestamp;
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
  roles[TimestampRole] = "Timestamp";
  return roles;
}

void QfCloudAttachmentsModel::refresh()
{
  beginResetModel();
  mAttachments.clear();

  if ( mCloudConnection && mCloudProject && !mCloudProject->id().isEmpty() )
  {
    const QString username = mCloudConnection->username();
    const QString projectId = mCloudProject->id();
    const QString projectFilePath = mCloudProject->localPath();
    const QDir projectDir( !projectFilePath.isEmpty() ? QFileInfo( projectFilePath ).absolutePath() : QString() );
    auto displayName = [&projectFilePath, &projectDir]( const QString &filePath ) {
      return !projectFilePath.isEmpty() ? projectDir.relativeFilePath( filePath ) : QFileInfo( filePath ).fileName();
    };

    QSet<QString> listedFilePaths;

    // Attachments of changes not yet uploaded only get queued once the changes are pushed
    if ( mDeltaFileWrapper )
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

    // A file queued for upload is pending, whatever the outcome of its previous uploads
    const QStringList pendingFilePaths = QfCloudUtils::getPendingAttachments( username ).values( projectId );
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

    // Only the most recent upload outcome of a file is listed, newest first
    const QList<QfCloudAttachmentHistoryEntry> history = QfCloudUtils::getAttachmentsHistory( username, projectId );
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
      attachment.timestamp = it->timestamp;
      mAttachments << attachment;
    }
  }

  endResetModel();
}

QfCloudConnection *QfCloudAttachmentsModel::cloudConnection() const
{
  return mCloudConnection;
}

void QfCloudAttachmentsModel::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudAttachmentsModel::refresh );
    disconnect( mCloudConnection, &QfCloudConnection::pendingAttachmentsAdded, this, &QfCloudAttachmentsModel::refresh );
    disconnect( mCloudConnection, &QfCloudConnection::pendingAttachmentsUploadFinished, this, &QfCloudAttachmentsModel::refresh );
    disconnect( mCloudConnection, &QfCloudConnection::attachmentsHistoryChanged, this, &QfCloudAttachmentsModel::refresh );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::pendingAttachmentsAdded, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::pendingAttachmentsUploadFinished, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudConnection, &QfCloudConnection::attachmentsHistoryChanged, this, &QfCloudAttachmentsModel::refresh );
  }

  emit cloudConnectionChanged();
  refresh();
}

QfCloudProject *QfCloudAttachmentsModel::cloudProject() const
{
  return mCloudProject;
}

void QfCloudAttachmentsModel::setCloudProject( QfCloudProject *cloudProject )
{
  if ( mCloudProject == cloudProject )
  {
    return;
  }

  if ( mCloudProject )
  {
    disconnect( mCloudProject, &QfCloudProject::idChanged, this, &QfCloudAttachmentsModel::refresh );
    disconnect( mCloudProject, &QfCloudProject::localPathChanged, this, &QfCloudAttachmentsModel::refresh );
    disconnect( mCloudProject, &QfCloudProject::deltaFileWrapperChanged, this, &QfCloudAttachmentsModel::onDeltaFileWrapperChanged );
  }

  mCloudProject = cloudProject;

  if ( mCloudProject )
  {
    connect( mCloudProject, &QfCloudProject::idChanged, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudProject, &QfCloudProject::localPathChanged, this, &QfCloudAttachmentsModel::refresh );
    connect( mCloudProject, &QfCloudProject::deltaFileWrapperChanged, this, &QfCloudAttachmentsModel::onDeltaFileWrapperChanged );
  }

  setDeltaFileWrapper( mCloudProject ? mCloudProject->deltaFileWrapper() : nullptr );

  emit cloudProjectChanged();
  refresh();
}

void QfCloudAttachmentsModel::onDeltaFileWrapperChanged()
{
  setDeltaFileWrapper( mCloudProject ? mCloudProject->deltaFileWrapper() : nullptr );
  refresh();
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
}
```


