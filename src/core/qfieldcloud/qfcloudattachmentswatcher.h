/***************************************************************************
    qfcloudattachmentswatcher.h
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

#ifndef QFCLOUDATTACHMENTSWATCHER_H
#define QFCLOUDATTACHMENTSWATCHER_H

#include "qfcloudconnection.h"

#include <QFileSystemWatcher>
#include <QObject>
#include <QPointer>

#include <memory>

/**
 * \ingroup core
 *
 * Watches the QFieldCloud pending attachments queue and signals when it has changed.
 */
class QfCloudAttachmentsWatcher : public QObject
{
    Q_OBJECT

  public:
    explicit QfCloudAttachmentsWatcher( QObject *parent = nullptr );
    ~QfCloudAttachmentsWatcher() override;

    //! Returns TRUE when the pending attachments queue is being watched.
    bool active() const;

    //! Sets whether the pending attachments queue is being watched.
    void setActive( bool active );

    //! Returns the cloud connection whose user's pending attachments queue is watched.
    QfCloudConnection *cloudConnection() const;

    //! Sets the \a cloudConnection whose user's pending attachments queue is watched.
    void setCloudConnection( QfCloudConnection *cloudConnection );

  signals:
    //! Emitted when the pending attachments queue has changed.
    void queueChanged();

    //! Emitted when an outstanding pending attachments queue has been emptied.
    void queueFinished();

  private slots:
    void attachWatcher();
    void onFileChanged();
    void onDirectoryChanged();

  private:
    bool mActive = false;
    bool mQueueOutstanding = false;
    QPointer<QfCloudConnection> mCloudConnection;
    QString mAttachmentsFilePath;
    std::unique_ptr<QFileSystemWatcher> mWatcher;
};

#endif // QFCLOUDATTACHMENTSWATCHER_H
