/***************************************************************************
    qfcloudattachmentsmodel.h
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

#ifndef QFCLOUDATTACHMENTSMODEL_H
#define QFCLOUDATTACHMENTSMODEL_H

#include "qfcloudconnection.h"
#include "qfcloudproject.h"
#include "qfdeltafilewrapper.h"

#include <QAbstractListModel>
#include <QDateTime>
#include <QPointer>

/**
 * \ingroup core
 *
 * A model listing the attachments of a QFieldCloud project that are not yet uploaded
 * or pending upload, alongside the outcome of past attachment uploads.
 */
class QfCloudAttachmentsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QfCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( QfCloudProject *cloudProject READ cloudProject WRITE setCloudProject NOTIFY cloudProjectChanged )

  public:
    enum Status
    {
      LocalStatus,
      PendingStatus,
      UploadedStatus,
      FailedStatus,
    };
    Q_ENUM( Status )

    enum ColumnRole
    {
      FilePathRole = Qt::UserRole + 1,
      FileNameRole,
      StatusRole,
      HttpCodeRole,
      TimestampRole,
    };
    Q_ENUM( ColumnRole )

    explicit QfCloudAttachmentsModel( QObject *parent = nullptr );

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns the cloud connection used to read the pending attachments and the attachments upload history.
    QfCloudConnection *cloudConnection() const;

    //! Sets the \a cloudConnection used to read the pending attachments and the attachments upload history.
    void setCloudConnection( QfCloudConnection *cloudConnection );

    //! Returns the cloud project whose attachments are listed.
    QfCloudProject *cloudProject() const;

    //! Sets the \a cloudProject whose attachments are listed.
    void setCloudProject( QfCloudProject *cloudProject );

    //! Reloads the attachments not yet uploaded, the pending attachments and the attachments upload history.
    Q_INVOKABLE void refresh();

  signals:
    void cloudConnectionChanged();
    void cloudProjectChanged();

  private slots:
    void onDeltaFileWrapperChanged();

  private:
    void setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper );

    struct Attachment
    {
        QString filePath;
        QString fileName;
        Status status = PendingStatus;
        int httpCode = 0;
        QDateTime timestamp;
    };

    QList<Attachment> mAttachments;

    QfCloudConnection *mCloudConnection = nullptr;
    QPointer<QfCloudProject> mCloudProject;
    QPointer<QfDeltaFileWrapper> mDeltaFileWrapper;
};

#endif // QFCLOUDATTACHMENTSMODEL_H
