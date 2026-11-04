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

QString QfCloudAttachmentsWatcher::username() const
{
  return mUsername;
}

void QfCloudAttachmentsWatcher::setUsername( const QString &username )
{
  if ( mUsername == username )
  {
    return;
  }

  mUsername = username;
  attachWatcher();
}

void QfCloudAttachmentsWatcher::attachWatcher()
{
  mWatcher.reset();

  if ( !mActive || mUsername.isEmpty() )
  {
    return;
  }

  const QString directory = QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), mUsername );
  if ( !QFileInfo( directory ).isDir() )
  {
    return;
  }

  mWatcher = std::make_unique<QFileSystemWatcher>();
  connect( mWatcher.get(), &QFileSystemWatcher::fileChanged, this, &QfCloudAttachmentsWatcher::queueChanged );
  connect( mWatcher.get(), &QFileSystemWatcher::directoryChanged, this, &QfCloudAttachmentsWatcher::onDirectoryChanged );

  // The directory is only watched until the attachments file gets created
  const QString attachmentsFilePath = QStringLiteral( "%1/attachments.csv" ).arg( directory );
  mWatcher->addPath( QFileInfo::exists( attachmentsFilePath ) ? attachmentsFilePath : directory );
}

void QfCloudAttachmentsWatcher::onDirectoryChanged()
{
  const QString directory = QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), mUsername );
  const QString attachmentsFilePath = QStringLiteral( "%1/attachments.csv" ).arg( directory );
  if ( !mWatcher || mWatcher->files().contains( attachmentsFilePath ) || !QFileInfo::exists( attachmentsFilePath ) )
  {
    return;
  }

  if ( mWatcher->addPath( attachmentsFilePath ) )
  {
    mWatcher->removePath( directory );

    // The watcher is now connected to the attachments file
    emit queueChanged();
  }
}
