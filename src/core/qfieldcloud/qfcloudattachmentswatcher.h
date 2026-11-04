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

#include <QFileSystemWatcher>
#include <QObject>

#include <memory>

/**
 * \ingroup core
 *
 * Watches the pending attachments file of a QFieldCloud user on disk, as attachments can be
 * uploaded by a separate service process which does not reach the rest of QField.
 *
 * The attachments upload history is written right before the pending attachments file, so
 * a change of the queue also means both files are up to date.
 */
class QfCloudAttachmentsWatcher : public QObject
{
    Q_OBJECT

  public:
    explicit QfCloudAttachmentsWatcher( QObject *parent = nullptr );
    ~QfCloudAttachmentsWatcher() override;

    //! Returns TRUE when the pending attachments file is being watched.
    bool active() const;

    //! Sets whether the pending attachments file is being watched.
    void setActive( bool active );

    //! Returns the cloud user whose pending attachments file is watched.
    QString username() const;

    //! Sets the cloud \a username whose pending attachments file is watched.
    void setUsername( const QString &username );

  signals:
    //! Emitted when the pending attachments queue has changed on disk.
    void queueChanged();

  private slots:
    void onDirectoryChanged();

  private:
    void attachWatcher();

    bool mActive = false;
    QString mUsername;
    std::unique_ptr<QFileSystemWatcher> mWatcher;
};

#endif // QFCLOUDATTACHMENTSWATCHER_H
