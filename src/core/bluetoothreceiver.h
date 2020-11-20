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

#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothSocket>

class BluetoothReceiver : public QObject
{
    Q_OBJECT

    public:
        explicit BluetoothReceiver( QObject *parent = nullptr );
        void connectService();
        void disconnectService();
        void repairDevice();
        void readSocket();

    private slots:
        void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status);
        void confirmPairing(const QBluetoothAddress &address, QString pin);
    private:
        QBluetoothLocalDevice *mLocalDevice = nullptr;
        QBluetoothSocket *mSocket = nullptr;

};

#endif // BLUETOOTHRECEIVER_H
