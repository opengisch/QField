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
#include "bluetoothlowenergyreceiver.h"

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
      emit lastDiscoveredCountChanged();
      setScanningStatus( Failed );
    }
  } );
  connect( mDeviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::finished, this, [this]() {
    if ( mDeviceDiscoveryAgent->error() == QBluetoothDeviceDiscoveryAgent::NoError )
    {
      emit lastDiscoveredCountChanged();
      setScanningStatus( Succeeded );
    }
    else
    {
      setLastError( mDeviceDiscoveryAgent->errorString() );
      emit lastDiscoveredCountChanged();
      setScanningStatus( Failed );
    }
  } );
  connect( mDeviceDiscoveryAgent.get(), &QBluetoothDeviceDiscoveryAgent::canceled, this, [this]() {
    emit lastDiscoveredCountChanged();
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

  mLastDiscoveredCount = 0;

  // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
  setScanningStatus( Discovering );

#if defined( Q_OS_IOS )
  mDeviceDiscoveryAgent->start( QBluetoothDeviceDiscoveryAgent::LowEnergyMethod );
#else
  mDeviceDiscoveryAgent->start();
#endif
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
  for ( qsizetype i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( deviceAddress( mDiscoveredDevices[i] ) == deviceAddress( info ) )
    {
      qInfo() << QStringLiteral( "Bluetooth device information updated: name %1, address %2, pairing status %3" )
                   .arg( info.name(), info.address().toString(), deviceAddress( info ) )
                   .arg( mLocalDevice->pairingStatus( info.address() ) );

      mDiscoveredDevices.replace( i, info );

      QModelIndex idx = index( i, 0 );
      emit dataChanged( idx, idx );
      return;
    }
  }

  qInfo() << QStringLiteral( "Bluetooth device information discovered: name %1, address %2, pairing status %3" )
               .arg( info.name(), info.address().toString(), deviceAddress( info ) )
               .arg( mLocalDevice->pairingStatus( info.address() ) );

#if defined( Q_OS_ANDROID )
  // Only list paired devices users have control over it.
  const bool paired = mLocalDevice->pairingStatus( info.address() ) != QBluetoothLocalDevice::Unpaired;
  if ( !paired )
  {
    return;
  }
#endif

  const int index = static_cast<int>( mDiscoveredDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
  mDiscoveredDevices.append( info );
  endInsertRows();

  mLastDiscoveredCount++;
}

int BluetoothDeviceModel::findIndexFromAddress( const QString &address ) const
{
  for ( int i = 0; i < mDiscoveredDevices.size(); i++ )
  {
    if ( deviceAddress( mDiscoveredDevices.at( i ) ) == address )
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
      return QStringLiteral( "%1" ).arg( !info.name().isEmpty() ? info.name().trimmed() : deviceAddress( info ) );

    case DeviceAddressRole:
      return deviceAddress( info );

    case DeviceNameRole:
      return info.name();

    case DeviceClassicSupportRole:
      return ( info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateCoreConfiguration || info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration ? true : false );

    case DeviceLowEnergySupportRole:
      return ( info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration || info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration ? true : false );

    case DeviceLowEnergyByDefaultRole:
#if defined( Q_OS_IOS )
      return true;
#endif
      if ( info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration || info.coreConfigurations() & QBluetoothDeviceInfo::BaseRateAndLowEnergyCoreConfiguration )
      {
        const QList<QBluetoothUuid> supportedServices = BluetoothLowEnergyReceiver::serviceChars.keys();
        return std::any_of( supportedServices.begin(), supportedServices.end(), [&info]( const QBluetoothUuid &service ) { return info.serviceUuids().contains( service ); } );
      }
      return false;
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
  roles[DeviceLowEnergyByDefaultRole] = "DeviceLowEnergyByDefault";

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
    if ( mDiscoveredDevices.at( i ).name() == name && deviceAddress( mDiscoveredDevices.at( i ) ) == address )
    {
      return i;
    }
  }

  const int index = static_cast<int>( mDiscoveredDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
#if defined( Q_OS_IOS ) || defined( Q_OS_MACOS )
  mDiscoveredDevices << QBluetoothDeviceInfo( QBluetoothUuid( address ), name, 0 );
#else
  mDiscoveredDevices << QBluetoothDeviceInfo( QBluetoothAddress( address ), name, 0 );
#endif
  endInsertRows();

  return index;
}

QString BluetoothDeviceModel::deviceAddress( const QBluetoothDeviceInfo &info ) const
{
#if defined( Q_OS_IOS ) || defined( Q_OS_MACOS )
  return info.deviceUuid().toString();
#else
  return info.address().toString();
#endif
}
