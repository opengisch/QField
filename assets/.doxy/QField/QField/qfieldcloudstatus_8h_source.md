

# File qfieldcloudstatus.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfieldcloudstatus.h**](qfieldcloudstatus_8h.md)

[Go to the documentation of this file](qfieldcloudstatus_8h.md)


```C++
/***************************************************************************
    qfieldcloudstatus.h
    ---------------------
    begin                : February 2026
    copyright            : (C) 2026 by Mohsen Dehghanzadeh
    email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELDCLOUDSTATUS_H
#define QFIELDCLOUDSTATUS_H

#include <QObject>

class NetworkReply;

class QFieldCloudStatus : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY urlChanged )
    Q_PROPERTY( StatusType statusType READ statusType NOTIFY statusUpdated )
    Q_PROPERTY( bool hasProblem READ hasProblem NOTIFY statusUpdated )
    Q_PROPERTY( QString statusMessage READ statusMessage NOTIFY statusUpdated )
    Q_PROPERTY( QString detailsMessage READ detailsMessage NOTIFY statusUpdated )
    Q_PROPERTY( QString statusPageUrl READ statusPageUrl NOTIFY statusUpdated )

  public:
    enum class StatusType
    {
      Ok,
      Maintenance,
      Degraded,
      Incident,
    };
    Q_ENUM( StatusType )

    explicit QFieldCloudStatus( QObject *parent = nullptr );

    QString url() const;

    StatusType statusType() const;

    void setUrl( const QString &url );

    bool hasProblem() const;

    QString statusMessage() const;

    QString detailsMessage() const;

    QString statusPageUrl() const;

    Q_INVOKABLE void refresh();

  signals:
    void urlChanged();
    void statusUpdated();

  private:
    void fetchStatus();
    void parseStatusResponse( const QByteArray &data );

    QString mUrl;

    QString mStatusPageUrl;
    QString mIncidentMessage;
    QString mMaintenanceMessage;

    StatusType mStatusType = StatusType::Ok;
    bool mHasProblem = false;
    QString mStatusMessage;
    QString mDetailsMessage;

    NetworkReply *mPendingReply = nullptr;
};

#endif // QFIELDCLOUDSTATUS_H
```


