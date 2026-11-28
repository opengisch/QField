

# File qfudpreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfudpreceiver.h**](qfudpreceiver_8h.md)

[Go to the documentation of this file](qfudpreceiver_8h.md)


```C++
/***************************************************************************
 qfudpreceiver.h - QfUdpReceiver

 ---------------------
 begin                : December 2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFUDPRECEIVER_H
#define QFUDPRECEIVER_H

#include "qfnmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class QfUdpReceiver : public QfNmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfUdpReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );
    ~QfUdpReceiver();

    static QLatin1String identifier;

  public slots:
    QString socketStateString() override;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QAbstractSocket::SocketError error );
    void handleStateChanged( QAbstractSocket::SocketState state );

    QString mAddress;
    int mPort = 0;
    QUdpSocket *mSocket = nullptr;
    QBuffer *mBuffer = nullptr;

    bool mReconnectOnDisconnect = false;
    int mConnectionFailureCount = 0;
    QTimer mReconnectTimer;
};

#endif // QFUDPRECEIVER_H
```


