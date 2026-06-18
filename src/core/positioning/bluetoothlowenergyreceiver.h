/***************************************************************************
 bluetoothreceiver.h - BluetoothReceiver

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

#ifndef BLUETOOTHLOWENERGYRECEIVER_H
#define BLUETOOTHLOWENERGYRECEIVER_H

#include "nmeagnssreceiver.h"

#include <QBuffer>
#include <QObject>
#include <QTimer>
#include <QtBluetooth/QBluetoothUuid>
#include <QtBluetooth/QLowEnergyController>
#include <QtBluetooth/QLowEnergyService>

/**
 * The BluetoothLowEnergyReceiver connects to a BLE device and feeds the
 * QgsNmeaConnection over a proxy QIODevice using QLowEnergyController.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 * \ingroup core
 */
class BluetoothLowEnergyReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit BluetoothLowEnergyReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~BluetoothLowEnergyReceiver();

    AbstractGnssReceiver::Capabilities capabilities() const override;

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

    //! Used to wait for previous connection to finish disconnecting
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

#endif //BLUETOOTHLOWENERGYRECEIVER_H
