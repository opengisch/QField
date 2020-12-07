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
#include <QDebug>

BluetoothReceiver::BluetoothReceiver(QObject *parent) : QObject( parent ),
    mLocalDevice( new QBluetoothLocalDevice ),
    mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) ),
    mGpsConnection( new QgsNmeaConnection( mSocket.get() ) )
{
    //nmea-todo: the following 3 connection could be removed in the end, since we handle the connection info over the stateChanged
    //socket log
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
        mSocketState = mSocket->state();
        emit socketStateChanged( mSocketState );
    });

    //QgsGpsConnection
    connect( mGpsConnection.get(), &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged);

    //connect on startup
    QSettings settings;
    const QString deviceAddress = settings.value( QStringLiteral( "positioningDevice" ), QString() ).toString();
    connectDevice(deviceAddress);
}

void BluetoothReceiver::disconnectDevice()
{
    if( mSocket->state() != QBluetoothSocket::UnconnectedState )
        mSocket->disconnectFromService();
}

void BluetoothReceiver::connectDevice(const QString &address)
{
    disconnectDevice();
    qDebug() << "Device set: "<<address;
    if( address.isEmpty() || address == QStringLiteral( "internal") )
    {
        qDebug() << "do not set up connection";
        return;
    }

#ifndef ANDROID
    //ugly workaround for linux tests
    repairDevice ( QBluetoothAddress(address) );
#else
    mSocket->connectToService( QBluetoothAddress(address), QBluetoothUuid(QBluetoothUuid::SerialPort), QBluetoothSocket::ReadOnly );
#endif
}

void BluetoothReceiver::stateChanged(const QgsGpsInformation &info)
{
    qDebug() << "state chagned " << info.longitude << "lon" << info.latitude << "ele" << info.elevation;

    mLastGnssPositionInformation = QgsGnssPositionInformation( info.latitude, info.longitude, info.elevation, info.speed, info.direction, info.satellitesInView, info.pdop,
                                                                   info.hdop, info.vdop, info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality,
                                                                   info.satellitesUsed, info.status, info.satPrn, info.satInfoComplete );
    qDebug() << "QGS Gps DATA " << mLastGnssPositionInformation.longitude() << "lon" << mLastGnssPositionInformation.latitude() << "ele" << mLastGnssPositionInformation.elevation();

    emit lastGnssPositionInformationChanged(mLastGnssPositionInformation);
}

QgsGnssPositionInformation BluetoothReceiver::fromQGeoPositionInfo(const QString &name)
{

    QGeoPositionInfoSource* positionSource = QGeoPositionInfoSource::createSource( name, nullptr );

    QGeoPositionInfo info = positionSource->lastKnownPosition();
    //not handled: magneticVariation = positionInfo.attribute( QGeoPositionInfo::Attribute::MagneticVariation );
    //not handled: info.attribute( QGeoPositionInfo::Attribute::VerticalSpeed );
    QgsGnssPositionInformation gnssPositionInformation = QgsGnssPositionInformation( info.coordinate().latitude(), info.coordinate().longitude(), info.coordinate().altitude(), info.attribute( QGeoPositionInfo::Attribute::GroundSpeed ),
                                                                                     info.attribute( QGeoPositionInfo::Attribute::Direction ), QList<QgsSatelliteInfo>(), 0, 0, 0,
                                                                                     info.attribute( QGeoPositionInfo::Attribute::HorizontalAccuracy ),info.attribute( QGeoPositionInfo::Attribute::VerticalAccuracy ),
                                                                                     info.timestamp(), QChar(), 0, -1,0, QChar(), QList<int>(),true );
    qDebug() << "Qt Position DATA " << gnssPositionInformation.latitude() << "lon" << gnssPositionInformation.longitude() << "ele" << gnssPositionInformation.elevation();
    return gnssPositionInformation;
}

#ifndef ANDROID
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
#endif
