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

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"
#include "qgsnmeaconnection.h"

#include <QObject>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothSocket>

/**
 * The bluetoothreceiver connects to a device and feeds the QgsNmeaConnection over QBluetoothSocket.
 * It receives QgsGpsInformation and converts it to GnssPositionInformation
 */
class BluetoothReceiver : public AbstractGnssReceiver
{
    Q_OBJECT

  public:
    explicit BluetoothReceiver( const QString &address = QString(), QObject *parent = nullptr );
    ~BluetoothReceiver() override {}

    /**
     * Sets whether the elevation value provided will be ellipsoidal or, if not, orthometric
     */
    void setEllipsoidalElevation( const bool ellipsoidalElevation );

    /**
     * Returns whether the elevation value provided will be ellipsoidal or orthometric
     */
    bool ellipsoidalElevation() const { return mEllipsoidalElevation; }

  private slots:
    /**
     * these functions used for repairing are only needed in the linux (not android) environment
     * since there are troubles by connect to a paired device. Maybe it can be resolved in future
     * but meanwhile we keep them as the developer setup.
     */
#ifdef Q_OS_LINUX
    void pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status );
    void confirmPairing( const QBluetoothAddress &address, QString pin );
#endif
    void stateChanged( const QgsGpsInformation &info );
    void setSocketState( const QBluetoothSocket::SocketState socketState );

  private:
    void handleConnectDevice() override;
    void handleDisconnectDevice() override;

#ifdef Q_OS_LINUX
    void connectService( const QBluetoothAddress &address );
    void repairDevice( const QBluetoothAddress &address );
#endif

    //! Used to wait for previous connection to finish disconnecting
    void doConnectDevice();

    QString mAddress;

    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    QBluetoothSocket *mSocket = nullptr;
    std::unique_ptr<QgsNmeaConnection> mGpsConnection;
    bool mLastGnssPositionValid = false;

    bool mDisconnecting = false;
    bool mConnectOnDisconnect;

    bool mEllipsoidalElevation = true;
};

#endif // BLUETOOTHRECEIVER_H
