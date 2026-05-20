

# File bluetoothdevicemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**bluetoothdevicemodel.cpp**](bluetoothdevicemodel_8cpp.md)

[Go to the documentation of this file](bluetoothdevicemodel_8cpp.md)


```C++
/***************************************************************************
  bluetoothdevicemodel.cpp - BluetoothDeviceModel

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

#include "bluetoothdevicemodel.h"

#include <QDebug>
#include <QGuiApplication>
#include <QPermissions>
#include <QSettings>
#include <qgis.h>


BluetoothDeviceModel::BluetoothDeviceModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

BluetoothDeviceModel::~BluetoothDeviceModel()
{
  stopServiceDiscovery();
}

void BluetoothDeviceModel::initiateDiscoveryAgent()
{
  mLocalDevice = std::make_unique<QBluetoothLocalDevice>();
  mServiceDiscoveryAgent = std::make_unique<QBluetoothServiceDiscoveryAgent>();

  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothDeviceModel::serviceDiscovered );
  connect( mServiceDiscoveryAgent.get(), qOverload<QBluetoothServiceDiscoveryAgent::Error>( &QBluetoothServiceDiscoveryAgent::errorOccurred ), this, [this]( QBluetoothServiceDiscoveryAgent::Error error ) {
    if ( error != QBluetoothServiceDiscoveryAgent::NoError )
    {
      setLastError( mServiceDiscoveryAgent->errorString() );
      setScanningStatus( Failed );
    }
  } );
  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::finished, [this]() {
    if ( mServiceDiscoveryAgent->error() == QBluetoothServiceDiscoveryAgent::NoError )
    {
      if ( mScanningStatus == FastScanning )
      {
        startServiceDiscovery();
      }
      else
      {
        setScanningStatus( Succeeded );
      }
    }
    else
    {
      setLastError( mServiceDiscoveryAgent->errorString() );
      setScanningStatus( Failed );
    }
  } );
  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::canceled, [this]() {
    setScanningStatus( Canceled );
  } );
}

void BluetoothDeviceModel::startServiceDiscovery()
{
  // Handle Bluetooth permission
  if ( !mBluetoothPermissionChecked )
  {
    QBluetoothPermission bluetoothPermission;
    bluetoothPermission.setCommunicationModes( QBluetoothPermission::Access );
    Qt::PermissionStatus permissionStatus = qApp->checkPermission( bluetoothPermission );
    if ( permissionStatus == Qt::PermissionStatus::Undetermined )
    {
      qApp->requestPermission( bluetoothPermission, this, [this]( const QPermission &permission ) {
        if ( permission.status() == Qt::PermissionStatus::Granted )
        {
          mBluetoothPermissionChecked = true;
          startServiceDiscovery();
        }
        else
        {
          mLastError = tr( "Bluetooth permission denied" );
          emit lastErrorChanged( mLastError );
        }
      } );
      return;
    }
    else if ( permissionStatus == Qt::PermissionStatus::Denied )
    {
      mLastError = tr( "Bluetooth permission is required to scan for bluetooth devices" );
      emit lastErrorChanged( mLastError );
      return;
    }
  }

  // Handle location permission
  if ( !mLocationPermissionChecked )
  {
    QLocationPermission locationPermission;
    locationPermission.setAccuracy( QLocationPermission::Precise );
    Qt::PermissionStatus permissionStatus = qApp->checkPermission( locationPermission );

    if ( permissionStatus == Qt::PermissionStatus::Undetermined )
    {
      qApp->requestPermission( locationPermission, this, [this]( const QPermission &permission ) {
        if ( permission.status() == Qt::PermissionStatus::Granted )
        {
          mLocationPermissionChecked = true;
          startServiceDiscovery();
        }
        else
        {
          mLastError = tr( "Location permission denied" );
          emit lastErrorChanged( mLastError );
        }
      } );
      return;
    }
    else if ( permissionStatus == Qt::PermissionStatus::Denied )
    {
      mLastError = tr( "Location permission is required to scan for bluetooth devices" );
      emit lastErrorChanged( mLastError );
      return;
    }
    mLocationPermissionChecked = true;
  }

  if ( !mServiceDiscoveryAgent )
  {
    initiateDiscoveryAgent();
  }

  if ( mServiceDiscoveryAgent->isActive() )
  {
    stopServiceDiscovery();
  }

  mServiceDiscoveryAgent->setUuidFilter( QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ) );

  if ( mScanningStatus == FastScanning )
  {
    // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
    setScanningStatus( FullScanning );
    mServiceDiscoveryAgent->clear();
    mServiceDiscoveryAgent->start( QBluetoothServiceDiscoveryAgent::FullDiscovery );
  }
  else
  {
    // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
    setScanningStatus( FastScanning );
    mServiceDiscoveryAgent->clear();
    mServiceDiscoveryAgent->start( QBluetoothServiceDiscoveryAgent::MinimalDiscovery );
  }
}

void BluetoothDeviceModel::stopServiceDiscovery()
{
  if ( !mServiceDiscoveryAgent )
    return;

  if ( mServiceDiscoveryAgent->isActive() )
  {
    mServiceDiscoveryAgent->stop();
    setScanningStatus( Canceled );
  }
}

void BluetoothDeviceModel::serviceDiscovered( const QBluetoothServiceInfo &service )
{
  qInfo() << QStringLiteral( "Bluetooth service discovered: name %1, address %2, pairing status %3" )
               .arg( service.device().name() )
               .arg( service.device().address().toString() )
               .arg( mLocalDevice->pairingStatus( service.device().address() ) );
  //only list the paired devices so the user has control over it.
  //but in linux (not android) we list unpaired as well, since it needs to repair them later (or pair them at all).
  const QPair<QString, QString> serviceDiscovered = qMakePair( service.device().name(), service.device().address().toString() );
  if ( mDiscoveredDevices.contains( serviceDiscovered ) )
    return;

  const int index = static_cast<int>( mDiscoveredDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
#ifdef Q_OS_ANDROID
  if ( mLocalDevice->pairingStatus( service.device().address() ) != QBluetoothLocalDevice::Unpaired )
  {
    mDiscoveredDevices.append( serviceDiscovered );
  }
#else
  mDiscoveredDevices.append( serviceDiscovered );
#endif
  endInsertRows();
}

int BluetoothDeviceModel::findIndexFromAddress( const QString &address ) const
{
  for ( int i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( mDiscoveredDevices.at( i ).second == address )
    {
      return i;
    }
  }

  return -1;
}

int BluetoothDeviceModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mDiscoveredDevices.size() ) : 0;
}

QVariant BluetoothDeviceModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case Qt::DisplayRole:
      return QStringLiteral( "%1 (%2)" ).arg( mDiscoveredDevices.at( index.row() ).first, mDiscoveredDevices.at( index.row() ).second );
      break;

    case DeviceAddressRole:
      return mDiscoveredDevices.at( index.row() ).second;
      break;

    case DeviceNameRole:
      return mDiscoveredDevices.at( index.row() ).first;
      break;
  }
  return QVariant();
}

QHash<int, QByteArray> BluetoothDeviceModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DeviceAddressRole] = "deviceAddress";
  roles[DeviceNameRole] = "deviceName";

  return roles;
}

void BluetoothDeviceModel::setScanningStatus( const BluetoothDeviceModel::ScanningStatus scanningStatus )
{
  if ( mScanningStatus == scanningStatus )
  {
    return;
  }

  qDebug() << "BluetoothDeviceModel: Status of service discovery changed to: " << scanningStatus;
  mScanningStatus = scanningStatus;
  emit scanningStatusChanged( mScanningStatus );
}

void BluetoothDeviceModel::setLastError( const QString &lastError )
{
  if ( mLastError == lastError )
  {
    return;
  }

  qDebug() << "BluetoothDeviceModel: Service discovery error received: " << lastError;
  mLastError = lastError;
  emit lastErrorChanged( mLastError );
}

int BluetoothDeviceModel::addDevice( const QString &name, const QString &address )
{
  if ( name.isEmpty() || address.isEmpty() )
    return -1;

  for ( int i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( mDiscoveredDevices.at( i ).first == name && mDiscoveredDevices.at( i ).second == address )
    {
      return i;
    }
  }

  const int index = static_cast<int>( mDiscoveredDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
  mDiscoveredDevices << qMakePair( name, address );
  endInsertRows();

  return index;
}
```


