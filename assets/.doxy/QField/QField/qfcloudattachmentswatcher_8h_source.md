

# File qfcloudattachmentswatcher.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudattachmentswatcher.h**](qfcloudattachmentswatcher_8h.md)

[Go to the documentation of this file](qfcloudattachmentswatcher_8h.md)


```C++
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

class QfCloudAttachmentsWatcher : public QObject
{
    Q_OBJECT

  public:
    explicit QfCloudAttachmentsWatcher( QObject *parent = nullptr );
    ~QfCloudAttachmentsWatcher() override;

    bool active() const;

    void setActive( bool active );

    QfCloudConnection *cloudConnection() const;

    void setCloudConnection( QfCloudConnection *cloudConnection );

  signals:
    void queueChanged();

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
```


