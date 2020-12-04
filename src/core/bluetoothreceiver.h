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

    public:
        explicit BluetoothReceiver( QObject *parent = nullptr );
        Q_INVOKABLE void disconnectDevice();
        Q_INVOKABLE void connectCurrentDevice();

        //void connectService( const QBluetoothAddress &address );
        //void repairDevice( const QBluetoothAddress &address );

        QgsGpsInformation lastGpsInformation() const { return mLastGpsInformation; }

    signals:
        void lastGpsInformationChanged(QgsGpsInformation lastGpsInformation);

    private slots:
        //void pairingFinished(const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status);
        //void confirmPairing(const QBluetoothAddress &address, QString pin);
        void stateChanged(const QgsGpsInformation &info );

    private:
        std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
        std::unique_ptr<QBluetoothSocket> mSocket;
        std::unique_ptr<QgsNmeaConnection> mGpsConnection;
        QgsGpsInformation mLastGpsInformation;
};

class GnssPositionConverter : public QObject
{
    Q_OBJECT
  public:
    static Q_INVOKABLE QgsGnssPositionInformation fromQgsGpsInformation( const QgsGpsInformation &info );
    static Q_INVOKABLE QgsGnssPositionInformation fromQGeoPositionInfo( const QGeoPositionInfo &info );
};



#endif // BLUETOOTHRECEIVER_H
