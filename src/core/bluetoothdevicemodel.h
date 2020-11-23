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
 * A model that manages the key/value pairs for a ValueMap widget.
 */
class BluetoothDeviceModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( bool scanning READ scanning WRITE setScanning NOTIFY scanningChanged )

    Q_ENUMS( BluetoothDeviceRoles )

  public:
    //! The roles provided by this model
    enum BluetoothDeviceRoles
    {
        DeviceAddressRole = Qt::UserRole + 1,
        DisplayStringRole,
    };

    /**
     * Create a new value map model
     */
    explicit BluetoothDeviceModel( QObject *parent = nullptr );

    bool scanning() const;
    void setScanning( bool scanning );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void startServiceDiscovery();
    Q_INVOKABLE int findAddessIndex( const QString &address ) const;
    Q_INVOKABLE QString findIndexAddess( int idx ) const;

  signals:
    void scanningChanged();

  private slots:
    void serviceDiscovered(const QBluetoothServiceInfo &service);

  private:
    QList<QPair<QString, QString>> mDiscoveredDevices;
    bool mScanning = false;
    QBluetoothServiceDiscoveryAgent mServiceDiscoveryAgent;
};

#endif // BLUETOOTHDEVICEMODEL_H
