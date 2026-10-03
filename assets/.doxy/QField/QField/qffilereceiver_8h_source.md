

# File qffilereceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qffilereceiver.h**](qffilereceiver_8h.md)

[Go to the documentation of this file](qffilereceiver_8h.md)


```C++
/***************************************************************************
 qffilereceiver.h - QfFileReceiver

 ---------------------
 begin                : August 2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFFILERECEIVER_H
#define QFFILERECEIVER_H

#include "qfnmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>

class QfFileReceiver : public QfNmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfFileReceiver( const QString &filePath = QString(), const int interval = 0, QObject *parent = nullptr );
    ~QfFileReceiver();

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

    void readLogsLine();

    QFile mLogs;
    QTimer mTimer;
    QBuffer *mBuffer = nullptr;
};

#endif // QFFILERECEIVER_H
```


