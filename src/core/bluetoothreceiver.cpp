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
    connect( mGpsConnection, &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged);

    //kind of ugly workaround - could be improved
    repairDevice();
}

void BluetoothReceiver::disconnectService()
{
    mSocket->disconnectFromService();
}

void BluetoothReceiver::connectService()
{
    QSettings settings;
    const QString deviceAddress = settings.value( QStringLiteral( "positioningDevice" ), QString() ).toString();

    qDebug() << "configured positioningDevice is "<< deviceAddress << "but we connect to statically to D4:12:43:53:B1:8D";
    /*
    if( deviceAddress.isEmpty() || deviceAddress == QStringLiteral( "internal ") )
        return;
    */
    qDebug() << "Disconnect service";

    disconnectService();
    qDebug() << "SOCKET state is " << mSocket->state();
     connect(mSocket, &QBluetoothSocket::connected,
         [=](){
        qDebug() << " : SOCKET CONNECTED";
        //QgsMessageLog::logMessage( QString("CONNECTED"), "QField", Qgis::Warning );

     });
     connect(mSocket, &QBluetoothSocket::disconnected,
         [=](){
        qDebug() << " : SOCKET DISCONNECTED";
        //QgsMessageLog::logMessage( QString("DISCONNECTED"), "QField", Qgis::Warning );
     });
     connect(mSocket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
         [=](QBluetoothSocket::SocketError error){
         //QgsMessageLog::logMessage( QString("Error %1").arg( error), "QField", Qgis::Warning );
        qDebug() << " : SOCKET ERROR: " <<error;
     });
     connect(mSocket, &QBluetoothSocket::readyRead,
             [=](){
            qDebug() << " : READY READ";
            //QgsMessageLog::logMessage( QString("READY READ"), "QField", Qgis::Warning );
            //readSocket();
         });
     connect(mSocket, &QBluetoothSocket::stateChanged,
         [=](){
        qDebug() << " : SOCKET state changed to " << mSocket->state();
     });

    mSocket->connectToService( QBluetoothAddress("D4:12:43:53:B1:8D"), QBluetoothUuid(QBluetoothUuid::SerialPort), QBluetoothSocket::ReadOnly );
}

void BluetoothReceiver::repairDevice()
{
    connect(mLocalDevice, &QBluetoothLocalDevice::pairingFinished,
             this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );
    connect(mLocalDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation,
            this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
    connect(mLocalDevice,  &QBluetoothLocalDevice::error,
            [=](){
        qDebug() << "RE-PAIR FAILED";
     });

    if( mLocalDevice->pairingStatus(QBluetoothAddress("D4:12:43:53:B1:8D") ) == QBluetoothLocalDevice::Paired )
    {
        mLocalDevice->requestPairing( QBluetoothAddress("D4:12:43:53:B1:8D"), QBluetoothLocalDevice::Unpaired);
    }
    else{
        mLocalDevice->requestPairing( QBluetoothAddress("D4:12:43:53:B1:8D"), QBluetoothLocalDevice::Paired);
    }
}

void BluetoothReceiver::confirmPairing(const QBluetoothAddress &address, QString pin)
{
    qDebug() << "needds conf";
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
        disconnect(mLocalDevice, &QBluetoothLocalDevice::pairingFinished,
                 this, &BluetoothReceiver::pairingFinished);
        connectService();
    }else{
        qDebug() << "unpaired device";
        repairDevice();
    }
}

void BluetoothReceiver::readSocket()
{
    while (mSocket->canReadLine()) {
        QByteArray line = mSocket->readLine().trimmed();
        qDebug() << mSocket->peerName() << "sais ["<<line.length()<<"]: "<< QString::fromUtf8(line.constData());
        //QgsMessageLog::logMessage( QString("received %1").arg( QString::fromUtf8(line.constData()) ), "QField", Qgis::Warning );
    }
}
