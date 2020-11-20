/***************************************************************************
 bluetoothreceiver.cpp - BluetoothReceiver

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

#include "bluetoothreceiver.h"

#include <QSettings>
#include <qgsmessagelog.h>
#include <QDebug>

BluetoothReceiver::BluetoothReceiver(QObject *parent) : QObject( parent ),
    mLocalDevice( new QBluetoothLocalDevice ),
    mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) ),
    mGpsConnection( new QgsNmeaConnection( mSocket ) )
{
    //socket log connections
    connect(mSocket, &QBluetoothSocket::connected,[=](){
        qDebug() << "SOCKET CONNECTED";
    });
    connect(mSocket, &QBluetoothSocket::disconnected,[=](){
        qDebug() << "SOCKET DISCONNECTED";
    });
    connect(mSocket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),[=](QBluetoothSocket::SocketError error){
        qDebug() << "SOCKET ERROR: " <<error;
    });
    connect(mSocket, &QBluetoothSocket::stateChanged,
        [=](){
        qDebug() << "SOCKET STATE changed to " << mSocket->state();
    });

    //GpsConnection connections
    connect( mGpsConnection, &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged);


    QSettings settings;
    const QString deviceAddress = settings.value( QStringLiteral( "positioningDevice" ), QString() ).toString();

    //connect on startup
    reconnectDevice(deviceAddress);
}

void BluetoothReceiver::reconnectDevice( const QString &address )
{
    if( mSocket->state() != QBluetoothSocket::UnconnectedState )
        mSocket->disconnectFromService();

    qDebug() << "Device set: "<<address;
    if( address.isEmpty() || address == QStringLiteral( "internal") )
    {
        qDebug() << "do not set up connection";
        return;
    }

    //kind of ugly workaround - if not needed could call directly connectService(QBluetoothAddress(deviceAddress)
    repairDevice( QBluetoothAddress(address) );
}

void BluetoothReceiver::connectService( const QBluetoothAddress &address )
{
    qDebug() << "Connect service";
    mSocket->connectToService( address, QBluetoothUuid(QBluetoothUuid::SerialPort), QBluetoothSocket::ReadOnly );
}

void BluetoothReceiver::repairDevice( const QBluetoothAddress &address)
{
    connect(mLocalDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );
    connect(mLocalDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
    connect(mLocalDevice,  &QBluetoothLocalDevice::error, [=](QBluetoothLocalDevice::Error error){
        qDebug() << "RE-PAIR FAILED "<<error;
    });

    if( mLocalDevice->pairingStatus( address ) == QBluetoothLocalDevice::Paired )
    {
        mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Unpaired);
    }
    else{
        mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Paired);
    }
}

void BluetoothReceiver::confirmPairing(const QBluetoothAddress &address, QString pin)
{
    Q_UNUSED( address );
    Q_UNUSED( pin );
    mLocalDevice->pairingConfirmation(true);
}

void BluetoothReceiver::stateChanged(const QgsGpsInformation &info)
{
    qDebug() << "state chagned";
    qDebug() << "lat" << info.latitude;
    qDebug() << "lon" << info.latitude;
    qDebug() << "ele" << info.elevation;

}

void BluetoothReceiver::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status)
{
    if( status == QBluetoothLocalDevice::Paired )
    {
        qDebug() << "paired device";
        disconnect(mLocalDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished);
        connectService(address);
    }else{
        qDebug() << "unpaired device";
        repairDevice(address);
    }
}
