/***************************************************************************
    qfcloudattachmentswatcher.cpp
    ---------------------
    begin                : November 2026
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

#include "qfcloudattachmentswatcher.h"
#include "qfcloudutils.h"

#include <QFileInfo>


QfCloudAttachmentsWatcher::QfCloudAttachmentsWatcher( QObject *parent )
  : QObject( parent )
{
}

QfCloudAttachmentsWatcher::~QfCloudAttachmentsWatcher() = default;

bool QfCloudAttachmentsWatcher::active() const
{
  return mActive;
}

void QfCloudAttachmentsWatcher::setActive( bool active )
{
  if ( mActive == active )
  {
    return;
  }

  mActive = active;
  attachWatcher();
}

QfCloudConnection *QfCloudAttachmentsWatcher::cloudConnection() const
{
  return mCloudConnection;
}

void QfCloudAttachmentsWatcher::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudAttachmentsWatcher::attachWatcher );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudAttachmentsWatcher::attachWatcher );
  }

  attachWatcher();
}

void QfCloudAttachmentsWatcher::attachWatcher()
{
  mWatcher.reset();
  mAttachmentsFilePath.clear();
  mQueueOutstanding = false;

  if ( !mActive || !mCloudConnection || mCloudConnection->username().isEmpty() )
  {
    return;
  }

  const QString directory = QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), mCloudConnection->username() );
  if ( !QFileInfo( directory ).isDir() )
  {
    return;
  }

  mAttachmentsFilePath = QStringLiteral( "%1/attachments.csv" ).arg( directory );

  mWatcher = std::make_unique<QFileSystemWatcher>();
  connect( mWatcher.get(), &QFileSystemWatcher::fileChanged, this, &QfCloudAttachmentsWatcher::onFileChanged );
  connect( mWatcher.get(), &QFileSystemWatcher::directoryChanged, this, &QfCloudAttachmentsWatcher::onDirectoryChanged );

  // The directory is only watched until the attachments file gets created
  const QFileInfo attachmentsFileInfo( mAttachmentsFilePath );
  if ( attachmentsFileInfo.exists() )
  {
    mWatcher->addPath( mAttachmentsFilePath );
    mQueueOutstanding = attachmentsFileInfo.size() > 0;
  }
  else
  {
    mWatcher->addPath( directory );
  }
}

void QfCloudAttachmentsWatcher::onFileChanged()
{
  emit queueChanged();

  const bool queueOutstanding = QFileInfo( mAttachmentsFilePath ).size() > 0;
  if ( mQueueOutstanding && !queueOutstanding )
  {
    emit queueFinished();
  }
  mQueueOutstanding = queueOutstanding;
}

void QfCloudAttachmentsWatcher::onDirectoryChanged()
{
  // The directory also changes whenever the attachments lock file is taken, only switch once the attachments file exists
  if ( !mWatcher || mWatcher->files().contains( mAttachmentsFilePath ) || !QFileInfo::exists( mAttachmentsFilePath ) )
  {
    return;
  }

  if ( mWatcher->addPath( mAttachmentsFilePath ) )
  {
    mWatcher->removePath( QFileInfo( mAttachmentsFilePath ).absolutePath() );

    // An empty attachments file does not hold a queue yet, its first write is reported as a file change
    mQueueOutstanding = QFileInfo( mAttachmentsFilePath ).size() > 0;
    if ( mQueueOutstanding )
    {
      emit queueChanged();
    }
  }
}
