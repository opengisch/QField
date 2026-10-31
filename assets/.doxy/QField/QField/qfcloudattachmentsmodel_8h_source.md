

# File qfcloudattachmentsmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudattachmentsmodel.h**](qfcloudattachmentsmodel_8h.md)

[Go to the documentation of this file](qfcloudattachmentsmodel_8h.md)


```C++
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

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

    QfCloudConnection *cloudConnection() const;

    void setCloudConnection( QfCloudConnection *cloudConnection );

    QfCloudProject *cloudProject() const;

    void setCloudProject( QfCloudProject *cloudProject );

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
```


