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

#include <QAbstractListModel>
#include <QPointer>

class QfDeltaFileWrapper;

/**
 * \ingroup core
 *
 * A model listing the attachments of a QFieldCloud project that are pending upload,
 * alongside the outcome of past attachment uploads.
 */
class QfCloudAttachmentsModel : public QAbstractListModel
{
    Q_OBJECT

    // moc needs the full definition to build the metaobject.
    Q_MOC_INCLUDE( "qfdeltafilewrapper.h" )

    Q_PROPERTY( QfCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( QString cloudProjectId READ cloudProjectId WRITE setCloudProjectId NOTIFY cloudProjectIdChanged )
    Q_PROPERTY( QfDeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper WRITE setDeltaFileWrapper NOTIFY deltaFileWrapperChanged )

  public:
    enum Status
    {
      PendingStatus,
      UploadedStatus,
      FailedStatus,
      LocalStatus,
    };
    Q_ENUM( Status )

    enum ColumnRole
    {
      FilePathRole = Qt::UserRole + 1,
      FileNameRole,
      StatusRole,
      HttpCodeRole,
    };
    Q_ENUM( ColumnRole )

    explicit QfCloudAttachmentsModel( QObject *parent = nullptr );

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    QfCloudConnection *cloudConnection() const { return mCloudConnection; }

    void setCloudConnection( QfCloudConnection *cloudConnection );

    QString cloudProjectId() const { return mCloudProjectId; }

    void setCloudProjectId( const QString &cloudProjectId );

    QfDeltaFileWrapper *deltaFileWrapper() const;

    /**
     * Sets the \a deltaFileWrapper of the opened cloud project, its attachments not yet
     * uploaded are listed with a local status when it matches the cloud project ID.
     */
    void setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper );

    //! Reloads the pending attachments and the attachments upload history.
    Q_INVOKABLE void refresh();

  signals:
    void cloudConnectionChanged();
    void cloudProjectIdChanged();
    void deltaFileWrapperChanged();

  private:
    struct Attachment
    {
        QString filePath;
        QString fileName;
        Status status = PendingStatus;
        int httpCode = 0;
    };

    QList<Attachment> mAttachments;

    QfCloudConnection *mCloudConnection = nullptr;
    QString mCloudProjectId;
    QPointer<QfDeltaFileWrapper> mDeltaFileWrapper;
};

#endif // QFCLOUDATTACHMENTSMODEL_H
