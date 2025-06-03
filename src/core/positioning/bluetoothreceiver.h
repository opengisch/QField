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

#include "nmeagnssreceiver.h"
#include "vendorcommandhelper.h"

#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothSocket>

/**
 * The bluetoothreceiver connects to a device and feeds the QgsNmeaConnection over QBluetoothSocket.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 */
class BluetoothReceiver : public NmeaGnssReceiver
{
    Q_OBJECT

  public:
    explicit BluetoothReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~BluetoothReceiver() override;

    Q_INVOKABLE bool sendVendorCommand( const QString &command ) { return mVendorHelper.sendCommand( command ); }
    Q_INVOKABLE bool configImuToAntOffset( double x, double y, double z,
                                           double stdx, double stdy, double stdz ) { return mVendorHelper.configImuToAntOffset( x, y, z, stdx, stdy, stdz ); }
    Q_INVOKABLE bool configInsSlantMeas() { return mVendorHelper.configInsSlantMeas(); }
    Q_INVOKABLE bool configInsDisable() { return mVendorHelper.configInsDisable(); }
    Q_INVOKABLE bool configInsReset() { return mVendorHelper.configInsReset(); }
    Q_INVOKABLE bool configAntennaDeltaHen( double len ) { return mVendorHelper.configAntennaDeltaHen( len ); }
    Q_INVOKABLE bool eraseImuParam() { return mVendorHelper.eraseImuParam(); }
    Q_INVOKABLE bool configInsReliability( int level ) { return mVendorHelper.configInsReliability( level ); }
    Q_INVOKABLE bool saveConfig() { return mVendorHelper.saveConfig(); }

  private slots:
    /**
     * these functions used for repairing are only needed in the linux (not android) environment
     * since there are troubles by connect to a paired device. Maybe it can be resolved in future
     * but meanwhile we keep them as the developer setup.
     */
    void pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status );
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    void confirmPairing( const QBluetoothAddress &address, QString pin );
#endif
    void setSocketState( const QBluetoothSocket::SocketState socketState );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;
    void handleError( QBluetoothSocket::SocketError error );

    void repairDevice( const QBluetoothAddress &address );

    //! Used to wait for previous connection to finish disconnecting
    void doConnectDevice();

    QString mAddress;

    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    QBluetoothSocket *mSocket = nullptr;
    VendorCommandHelper mVendorHelper;

    bool mPoweringOn = false;
    bool mDisconnecting = false;
    bool mConnectOnDisconnect = false;
};

#endif // BLUETOOTHRECEIVER_H
