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
  stopDeviceDiscovery();
}

void BluetoothDeviceModel::initiateDiscoveryAgent()
{
  mLocalDevice = std::make_unique<QBluetoothLocalDevice>();
  mDeviceDiscoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();

  connect( mDeviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothDeviceModel::deviceDiscovered );
  connect( mDeviceDiscoveryAgent.get(), qOverload<QBluetoothDeviceDiscoveryAgent::Error>( &QBluetoothDeviceDiscoveryAgent::errorOccurred ), this, [this]( QBluetoothDeviceDiscoveryAgent::Error error ) {
    if ( error != QBluetoothDeviceDiscoveryAgent::NoError )
    {
      setLastError( mDeviceDiscoveryAgent->errorString() );
      setScanningStatus( Failed );
    }
  } );
  connect( mDeviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::finished, this, [this]() {
    if ( mDeviceDiscoveryAgent->error() == QBluetoothDeviceDiscoveryAgent::NoError )
    {
      setScanningStatus( Succeeded );
    }
    else
    {
      setLastError( mDeviceDiscoveryAgent->errorString() );
      setScanningStatus( Failed );
    }
  } );
  connect( mDeviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::canceled, this, [this]() {
    setScanningStatus( Canceled );
  } );
}

void BluetoothDeviceModel::startDeviceDiscovery()
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
          startDeviceDiscovery();
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
          startDeviceDiscovery();
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

  if ( !mDeviceDiscoveryAgent )
  {
    initiateDiscoveryAgent();
  }

  if ( mDeviceDiscoveryAgent->isActive() )
  {
    stopDeviceDiscovery();
  }

  // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
  setScanningStatus( Discovering );
  mDeviceDiscoveryAgent->start();
}

void BluetoothDeviceModel::stopDeviceDiscovery()
{
  if ( !mDeviceDiscoveryAgent )
    return;

  if ( mDeviceDiscoveryAgent->isActive() )
  {
    mDeviceDiscoveryAgent->stop();
    setScanningStatus( Canceled );
  }
}

void BluetoothDeviceModel::deviceDiscovered( const QBluetoothDeviceInfo &info )
{
  qInfo() << QStringLiteral( "Bluetooth device discovered: name %1, address %2, pairing status %3" )
               .arg( info.name(), info.address().toString() )
               .arg( mLocalDevice->pairingStatus( info.address() ) );

  if ( mDiscoveredDevices.contains( info ) )
  {
    return;
  }

  const int index = static_cast<int>( mDiscoveredDevices.size() );
#ifdef Q_OS_ANDROID
  //only list the paired devices so the user has control over it.
  //but in linux (not android) we list unpaired as well, since it needs to repair them later (or pair them at all).
  if ( mLocalDevice->pairingStatus( info.address() ) != QBluetoothLocalDevice::Unpaired )
  {
    beginInsertRows( QModelIndex(), index, index );
    mDiscoveredDevices.append( info );
    endInsertRows();
  }
#else
  beginInsertRows( QModelIndex(), index, index );
  mDiscoveredDevices.append( info );
  endInsertRows();
#endif
}

int BluetoothDeviceModel::findIndexFromAddress( const QString &address ) const
{
  for ( int i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( mDiscoveredDevices.at( i ).address().toString() == address )
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
  if ( index.row() == -1 || index.row() >= mDiscoveredDevices.size() )
  {
    return QVariant();
  }

  const QBluetoothDeviceInfo info = mDiscoveredDevices.at( index.row() );
  switch ( role )
  {
    case Qt::DisplayRole:
      return QStringLiteral( "%1 (%2)" ).arg( info.name(), info.address().toString() );

    case DeviceAddressRole:
      return info.address().toString();

    case DeviceNameRole:
      return info.name();

    case DeviceClassicSupportRole:
      return ( info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateCoreConfiguration || info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration ? true : false );

    case DeviceLowEnergySupportRole:
      return ( info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration || info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration ? true : false );
  }
  return QVariant();
}

QHash<int, QByteArray> BluetoothDeviceModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DeviceAddressRole] = "DeviceAddress";
  roles[DeviceNameRole] = "DeviceName";
  roles[DeviceClassicSupportRole] = "DeviceClassicSupport";
  roles[DeviceLowEnergySupportRole] = "DeviceLowEnergySupport";

  return roles;
}

void BluetoothDeviceModel::setScanningStatus( const BluetoothDeviceModel::ScanningStatus scanningStatus )
{
  if ( mScanningStatus == scanningStatus )
  {
    return;
  }

  qDebug() << "BluetoothDeviceModel: Status of device discovery changed to: " << scanningStatus;
  mScanningStatus = scanningStatus;
  emit scanningStatusChanged( mScanningStatus );
}

void BluetoothDeviceModel::setLastError( const QString &lastError )
{
  if ( mLastError == lastError )
  {
    return;
  }

  qDebug() << "BluetoothDeviceModel: device discovery error received: " << lastError;
  mLastError = lastError;
  emit lastErrorChanged( mLastError );
}

int BluetoothDeviceModel::addDevice( const QString &name, const QString &address )
{
  if ( name.isEmpty() || address.isEmpty() )
    return -1;

  for ( int i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( mDiscoveredDevices.at( i ).name() == name && mDiscoveredDevices.at( i ).address().toString() == address )
    {
      return i;
    }
  }

  const int index = static_cast<int>( mDiscoveredDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
  mDiscoveredDevices << QBluetoothDeviceInfo( QBluetoothAddress( address ), name, 0 );
  endInsertRows();

  return index;
}
