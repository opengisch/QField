/***************************************************************************
  bluetoothdevicemodel.h - BluetoothDeviceModel

 ---------------------
 begin                : 20.11.2020
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

#ifndef BLUETOOTHDEVICEMODEL_H
#define BLUETOOTHDEVICEMODEL_H

#include <QAbstractListModel>
#include <QtBluetooth/QBluetoothServiceDiscoveryAgent>
#include <QtBluetooth/QBluetoothServiceInfo>
#include <QtBluetooth/QBluetoothLocalDevice>

/**
 * A model that provides all paired bluetooth devices name/address that are accessible over the serial port
 */
class BluetoothDeviceModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( ScanningStatus scanningStatus READ scanningStatus NOTIFY scanningStatusChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

  public:
    //! The roles provided by this model
    enum BluetoothDeviceRoles
    {
      DeviceAddressRole = Qt::UserRole + 1,
      DeviceNameRole,
    };
    Q_ENUM( BluetoothDeviceRoles )

    //! The status telling the result of the scanning
    enum ScanningStatus
    {
      Scanning,
      Succeeded,
      Failed,
      Canceled,
      NoStatus
    };
    Q_ENUM( ScanningStatus )


    explicit BluetoothDeviceModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void startServiceDiscovery( const bool fullDiscovery );
    Q_INVOKABLE int findAddressIndex( const QString &address ) const;

    ScanningStatus scanningStatus() const { return mScanningStatus; };
    QString lastError() const { return mLastError; };

  signals:
    void scanningStatusChanged( ScanningStatus scanningStatus );
    void lastErrorChanged( QString lastError );

  private slots:
    void setScanningStatus( const ScanningStatus scanningStatus );
    void setLastError( const QString &lastError );
    void serviceDiscovered( const QBluetoothServiceInfo &service );

  private:
    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    QBluetoothServiceDiscoveryAgent mServiceDiscoveryAgent;
    QList<QPair<QString, QString>> mDiscoveredDevices;
    ScanningStatus mScanningStatus = NoStatus;
    QString mLastError;
};

#endif // BLUETOOTHDEVICEMODEL_H
