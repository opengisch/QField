

# File egenioussreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**egenioussreceiver.h**](egenioussreceiver_8h.md)

[Go to the documentation of this file](egenioussreceiver_8h.md)


```C++
/***************************************************************************
 egeniousseeceiver.h - EgenioussReceiver

 ---------------------
 begin                : October 2024
 copyright            : (C) 2024 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EGENIOUSSRECEIVER_H
#define EGENIOUSSRECEIVER_H

#include "abstractgnssreceiver.h"

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QTcpSocket>

class EgenioussReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit EgenioussReceiver( const QString &address = QString(), const int port = 0, QObject *parent = nullptr );
    ~EgenioussReceiver() override;

    GnssPositionDetails details() const override;

    static QLatin1String identifier;

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

  private slots:
    void onReadyRead();
    void handleError( QAbstractSocket::SocketError error );

  private:
    void processReceivedData();
    void handleErrorMessage( const QString &errorMessage );

  private:
    QTcpSocket *mTcpSocket = nullptr;
    QJsonObject mPayload;
    const QHostAddress mAddress;
    const int mPort;
};

#endif // EGENIOUSSRECEIVER_H
```


