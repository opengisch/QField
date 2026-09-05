

# File qfbluetoothdevicemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfbluetoothdevicemodel.h**](qfbluetoothdevicemodel_8h.md)

[Go to the documentation of this file](qfbluetoothdevicemodel_8h.md)


```C++
/***************************************************************************
  qfbluetoothdevicemodel.h - QfBluetoothDeviceModel

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

#ifndef QFBLUETOOTHDEVICEMODEL_H
#define QFBLUETOOTHDEVICEMODEL_H

#include <QAbstractListModel>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QtBluetooth/QBluetoothDeviceInfo>
#include <QtBluetooth/QBluetoothLocalDevice>
#include <QtBluetooth/QBluetoothUuid>


class QfBluetoothDeviceModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( ScanningStatus scanningStatus READ scanningStatus NOTIFY scanningStatusChanged )
    Q_PROPERTY( qsizetype lastDiscoveredCount READ lastDiscoveredCount NOTIFY lastDiscoveredCountChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

  public:
    enum BluetoothDeviceRoles
    {
      DeviceAddressRole = Qt::UserRole + 1,
      DeviceNameRole,
      DeviceClassicSupportRole,
      DeviceLowEnergySupportRole,
      DeviceLowEnergyByDefaultRole,
    };
    Q_ENUM( BluetoothDeviceRoles )

    
    enum ScanningStatus
    {
      Discovering,
      Succeeded,
      Failed,
      Canceled,
      NoStatus
    };
    Q_ENUM( ScanningStatus )


    explicit QfBluetoothDeviceModel( QObject *parent = nullptr );
    ~QfBluetoothDeviceModel();

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE int addDevice( const QString &name, const QString &address );

    Q_INVOKABLE void startDeviceDiscovery();

    Q_INVOKABLE void stopDeviceDiscovery();

    Q_INVOKABLE int findIndexFromAddress( const QString &address ) const;

    ScanningStatus scanningStatus() const { return mScanningStatus; };

    qsizetype lastDiscoveredCount() const { return mLastDiscoveredCount; }

    QString lastError() const { return mLastError; };

    QString deviceAddress( const QBluetoothDeviceInfo &info ) const;

  signals:

    void scanningStatusChanged( QfBluetoothDeviceModel::ScanningStatus scanningStatus );
    void lastDiscoveredCountChanged();
    void lastErrorChanged( QString lastError );

  private slots:
    void setScanningStatus( const QfBluetoothDeviceModel::ScanningStatus scanningStatus );
    void setLastError( const QString &lastError );
    void deviceDiscovered( const QBluetoothDeviceInfo &info );

  private:
    void initiateDiscoveryAgent();

    bool mBluetoothPermissionChecked = false;
    bool mLocationPermissionChecked = false;

    std::unique_ptr<QBluetoothLocalDevice> mLocalDevice;
    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> mDeviceDiscoveryAgent;
    QList<QBluetoothDeviceInfo> mDiscoveredDevices;
    ScanningStatus mScanningStatus = NoStatus;

    qsizetype mLastDiscoveredCount;
    QString mLastError;
};

#endif // QFBLUETOOTHDEVICEMODEL_H
```


