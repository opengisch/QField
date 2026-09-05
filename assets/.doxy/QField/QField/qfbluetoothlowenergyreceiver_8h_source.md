

# File qfbluetoothlowenergyreceiver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfbluetoothlowenergyreceiver.h**](qfbluetoothlowenergyreceiver_8h.md)

[Go to the documentation of this file](qfbluetoothlowenergyreceiver_8h.md)


```C++
/***************************************************************************
 qfbluetoothreceiver.h - QfBluetoothReceiver

 ---------------------
 begin                : 2026/05/22
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFBLUETOOTHLOWENERGYRECEIVER_H
#define QFBLUETOOTHLOWENERGYRECEIVER_H

#include "qfnmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QtBluetooth/QBluetoothUuid>
#include <QtBluetooth/QLowEnergyController>
#include <QtBluetooth/QLowEnergyService>

class QfBluetoothLowEnergyReceiver : public QfNmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit QfBluetoothLowEnergyReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~QfBluetoothLowEnergyReceiver();

    QfAbstractGnssReceiver::Capabilities capabilities() const override;

    static QLatin1String identifier;

    static QMap<QBluetoothUuid, std::pair<QBluetoothUuid, QBluetoothUuid>> serviceChars;

  public slots:
    QString socketStateString() override;
    void onCorrectionDataReceived( const QByteArray &data ) override;

  private slots:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

    // BLE Controller slots
    void deviceConnected();
    void deviceDisconnected();
    void controllerErrorOccurred( QLowEnergyController::Error error );
    void serviceDiscovered( const QBluetoothUuid &newService );
    void serviceDiscoveryFinished();

    // BLE Service slots
    void serviceStateChanged( QLowEnergyService::ServiceState state );
    void serviceErrorOccurred( QLowEnergyService::ServiceError error );
    void characteristicChanged( const QLowEnergyCharacteristic &characteristic, const QByteArray &value );

    void forwardCorrectionDataChunk();

  private:
    void clearService();

    void doConnectDevice();
    void doDisconnectDevice();

    QString mAddress;

    QLowEnergyController *mController = nullptr;

    QLowEnergyService *mService = nullptr;
    QLowEnergyCharacteristic mRxCharacteristic;
    QLowEnergyCharacteristic mTxCharacteristic;

    QLowEnergyService *mBatteryService = nullptr;
    QLowEnergyCharacteristic mBatteryCharacteristic;

    QBuffer *mBuffer = nullptr;
    QByteArray mBufferData;

    QByteArray mCorrectionData;
    QTimer mCorrectionTimer;

    bool mDisconnecting = false;
    bool mConnectOnDisconnect = false;
    int mConnectionFailureCount = 0;
};

#endif //QFBLUETOOTHLOWENERGYRECEIVER_H
```


