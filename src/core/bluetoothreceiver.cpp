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
    mGpsConnection( new QgsNmeaConnection( mSocket.get() ) )
{
    //socket log connections
    connect(mSocket.get(), &QBluetoothSocket::connected,[=](){
        qDebug() << "SOCKET CONNECTED";
    });
    connect(mSocket.get(), &QBluetoothSocket::disconnected,[=](){
        qDebug() << "SOCKET DISCONNECTED";
    });
    connect(mSocket.get(), QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),[=](QBluetoothSocket::SocketError error){
        qDebug() << "SOCKET ERROR: " <<error;
    });
    connect(mSocket.get(), &QBluetoothSocket::stateChanged,
        [=](){
        qDebug() << "SOCKET STATE changed to " << mSocket->state();
    });

    //GpsConnection connections
    connect( mGpsConnection.get(), &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged);

    //connect on startup
    connectCurrentDevice();
}

void BluetoothReceiver::disconnectDevice()
{
    if( mSocket->state() != QBluetoothSocket::UnconnectedState )
        mSocket->disconnectFromService();
}

void BluetoothReceiver::connectCurrentDevice()
{
    QSettings settings;
    const QString deviceAddress = settings.value( QStringLiteral( "positioningDevice" ), QString() ).toString();

    qDebug() << "Device set: "<<deviceAddress;
    if( deviceAddress.isEmpty() || deviceAddress == QStringLiteral( "internal") )
    {
        qDebug() << "do not set up connection";
        return;
    }

    mSocket->connectToService( QBluetoothAddress(deviceAddress), QBluetoothUuid(QBluetoothUuid::SerialPort), QBluetoothSocket::ReadOnly );

    //kind of ugly workaround - if needed
    //repairDevice ( QBluetoothAddress(address) );
}

void BluetoothReceiver::stateChanged(const QgsGpsInformation &info)
{
    qDebug() << "state chagned " << info.longitude << "lon" << info.latitude << "ele" << info.elevation;

    mLastGpsInformation = info;

    emit lastGpsInformationChanged(mLastGpsInformation);
}

/* maybe repairDevice is not used...

void BluetoothReceiver::repairDevice( const QBluetoothAddress &address)
{

    qDebug() << "BT Debug: 8";
    connect(mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );
    connect(mLocalDevice.get(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
    connect(mLocalDevice.get(),  &QBluetoothLocalDevice::error, [=](QBluetoothLocalDevice::Error error){
        qDebug() << "RE-PAIR FAILED "<<error;
    });

    qDebug() << "BT Debug: 9";
    if( mLocalDevice->pairingStatus( address ) == QBluetoothLocalDevice::Paired )
    {
        qDebug() << "BT Debug: 10";
        mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Unpaired);
    }
    else{
        qDebug() << "BT Debug: 11";
        mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Paired);
    }
}

void BluetoothReceiver::confirmPairing(const QBluetoothAddress &address, QString pin)
{
    Q_UNUSED( address );
    Q_UNUSED( pin );
    mLocalDevice->pairingConfirmation(true);
}

void BluetoothReceiver::pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status)
{
    if( status == QBluetoothLocalDevice::Paired )
    {
        qDebug() << "paired device";
        disconnect(mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished);

        qDebug() << "BT Debug: 12";
        connectService(address);
    }else{
        qDebug() << "unpaired device";
        repairDevice(address);
    }
}

void BluetoothReceiver::connectService( const QBluetoothAddress &address )
{
    mSocket->connectToService( address, QBluetoothUuid(QBluetoothUuid::SerialPort), QBluetoothSocket::ReadOnly );
}
*/
