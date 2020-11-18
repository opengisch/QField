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

#include <QtBluetooth/QBluetoothServer>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothServiceDiscoveryAgent>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QObject>

class BluetoothReceiver : public QObject
{
    Q_OBJECT
    /*
    Q_PROPERTY( QString expressionText READ expressionText WRITE setExpressionText NOTIFY expressionTextChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsMapLayer *layer READ layer WRITE setLayer NOTIFY layerChanged )
    */
    public:
        explicit BluetoothReceiver( QObject *parent = nullptr );
        void startDeviceDiscovery();
        void connectService(const QBluetoothServiceInfo &service);
        void readSocket();

    signals:

    private slots:
        void serviceDiscovered(const QBluetoothServiceInfo &service);
        //void stringReceived(QString *stream );

    private:
        QBluetoothSocket *socket = nullptr;
        /*
        QBluetoothServer *rfcommServer = nullptr;
*/

};

#endif // BLUETOOTHRECEIVER_H
