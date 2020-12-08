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
#include "gnsspositioninformation.h"

class BluetoothReceiver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( GnssPositionInformation lastGnssPositionInformation READ lastGnssPositionInformation NOTIFY lastGnssPositionInformationChanged )
    Q_PROPERTY( QBluetoothSocket::SocketState socketState READ socketState NOTIFY socketStateChanged )
    Q_PROPERTY( bool connected READ connected NOTIFY connectedChanged ) //todo-nmea: can be replaced by socketState when QBluetoothSocket can be accessed by the QML

  public:
    explicit BluetoothReceiver( QObject *parent = nullptr );

    Q_INVOKABLE void disconnectDevice();
    Q_INVOKABLE void connectDevice( const QString &address );

    GnssPositionInformation lastGnssPositionInformation() const { return mLastGnssPositionInformation; }
    QBluetoothSocket::SocketState socketState() const { return mSocketState;}

    static Q_INVOKABLE GnssPositionInformation fromQGeoPositionInfo( const QString &name );
    bool connected() const  { return mConnected; }

  signals:
    void lastGnssPositionInformationChanged( GnssPositionInformation lastGnssPositionInformation );
    void socketStateChanged( QBluetoothSocket::SocketState socketState );
    void connectedChanged( bool connected );

  private slots:
#ifndef Q_OS_ANDROID
    void pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status );
    void confirmPairing( const QBluetoothAddress &address, QString pin );
#endif
    void stateChanged( const QgsGpsInformation &info );

  private:
#ifndef Q_OS_ANDROID
    void connectService( const QBluetoothAddress &address );
    void repairDevice( const QBluetoothAddress &address );
#endif

    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    std::unique_ptr<QBluetoothSocket> mSocket;
    std::unique_ptr<QgsNmeaConnection> mGpsConnection;
    GnssPositionInformation mLastGnssPositionInformation;
    QBluetoothSocket::SocketState mSocketState;
    bool mConnected = false;
};

#endif // BLUETOOTHRECEIVER_H
