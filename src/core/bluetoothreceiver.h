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
#include "qgsnmeaconnection.h"
#include "qgsgnsspositioninformation.h"

class BluetoothReceiver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsGpsInformation lastGpsInformation READ lastGpsInformation NOTIFY lastGpsInformationChanged )
    Q_PROPERTY( QBluetoothSocket::SocketState socketState READ socketState NOTIFY socketStateChanged )

    public:
        explicit BluetoothReceiver( QObject *parent = nullptr );
        Q_INVOKABLE void disconnectDevice();
        Q_INVOKABLE void connectDevice(const QString &address);

        void connectService( const QBluetoothAddress &address );
        void repairDevice( const QBluetoothAddress &address );

        QgsGpsInformation lastGpsInformation() const { return mLastGpsInformation; }

        QBluetoothSocket::SocketState socketState() const { return mSocketState;}

signals:
        void lastGpsInformationChanged(QgsGpsInformation lastGpsInformation);
        void socketStateChanged(QBluetoothSocket::SocketState socketState);

private slots:
        void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status);
        void confirmPairing(const QBluetoothAddress &address, QString pin);
        void stateChanged(const QgsGpsInformation &info );

    private:
        std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
        std::unique_ptr<QBluetoothSocket> mSocket;
        std::unique_ptr<QgsNmeaConnection> mGpsConnection;
        QgsGpsInformation mLastGpsInformation;
        QBluetoothSocket::SocketState mSocketState;
};

class GnssPositionConverter : public QObject
{
    Q_OBJECT
  public:
    static Q_INVOKABLE QgsGnssPositionInformation fromQgsGpsInformation( const QgsGpsInformation &info );
    static Q_INVOKABLE QgsGnssPositionInformation fromQGeoPositionInfo( const QString &name);
};



#endif // BLUETOOTHRECEIVER_H
