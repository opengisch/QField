

# File bluetoothreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**bluetoothreceiver.h**](bluetoothreceiver_8h.md)

[Go to the documentation of this file](bluetoothreceiver_8h.md)


```C++
/***************************************************************************
 bluetoothreceiver.h - BluetoothReceiver

 ---------------------
 begin                : 18.11.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef BLUETOOTHRECEIVER_H
#define BLUETOOTHRECEIVER_H

#include "nmeagnssreceiver.h"

#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothSocket>

class BluetoothReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit BluetoothReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~BluetoothReceiver() override;

  public slots:
    QString socketStateString() override;

  private slots:
    void pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QBluetoothSocket::SocketError error );

    void repairDevice( const QBluetoothAddress &address );

    void doConnectDevice();

    QString mAddress;

    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    QBluetoothSocket *mSocket = nullptr;

    bool mPoweringOn = false;
    bool mDisconnecting = false;
    bool mConnectOnDisconnect = false;
};

#endif // BLUETOOTHRECEIVER_H
```


