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
#include "qgis.h"
#include <QSettings>
#include <QDebug>

BluetoothDeviceModel::BluetoothDeviceModel( QObject *parent )
  : QAbstractListModel( parent ),
    mLocalDevice( std::make_unique<QBluetoothLocalDevice>() )
{
  connect( &mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothDeviceModel::serviceDiscovered );
  connect( &mServiceDiscoveryAgent, qgis::overload<QBluetoothServiceDiscoveryAgent::Error>::of( &QBluetoothServiceDiscoveryAgent::error ), this, [ = ]()
  {
    setLastError( mServiceDiscoveryAgent.errorString() );
    setScanningStatus( Failed );
  } );
  connect( &mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished, [ = ]()
  {
    setScanningStatus( mServiceDiscoveryAgent.error() == QBluetoothServiceDiscoveryAgent::NoError ? Succeeded : Failed );
  } );
  connect( &mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled, [ = ]()
  {
    setScanningStatus( Canceled );
  } );

  beginResetModel();
  mDiscoveredDevices.clear();
  mDiscoveredDevices.append( qMakePair( tr( "Internal device" ), QString() ) );

  QSettings settings;
  const QString deviceAddress = settings.value( QStringLiteral( "positioningDevice" ), QString( "" ) ).toString();
  if ( !deviceAddress.isEmpty() )
  {
    const QString deviceName = settings.value( QStringLiteral( "positioningDeviceName" ), QStringLiteral( "Unknown device" ) ).toString();
    mDiscoveredDevices.append( qMakePair( deviceName, deviceAddress ) );
  }
  endResetModel();
}

void BluetoothDeviceModel::startServiceDiscovery( const bool fullDiscovery )
{
  if ( mServiceDiscoveryAgent.isActive() )
    mServiceDiscoveryAgent.stop();

  mServiceDiscoveryAgent.setUuidFilter( QBluetoothUuid( QBluetoothUuid::SerialPort ) );
  QBluetoothServiceDiscoveryAgent::DiscoveryMode discoveryMode = fullDiscovery ? QBluetoothServiceDiscoveryAgent::FullDiscovery : QBluetoothServiceDiscoveryAgent::MinimalDiscovery;

  // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
  setScanningStatus( Scanning );
  mServiceDiscoveryAgent.clear();
  mServiceDiscoveryAgent.start( discoveryMode );
}

void BluetoothDeviceModel::serviceDiscovered( const QBluetoothServiceInfo &service )
{
  //only list the paired devices so the user has control over it.
  //but in linux (not android) we list unpaired as well, since it needs to repair them later (or pair them at all).
  const QPair<QString, QString> serviceDiscovered = qMakePair( service.device().name(), service.device().address().toString() );
  if ( mDiscoveredDevices.contains( serviceDiscovered ) )
    return;

  beginInsertRows( QModelIndex(), mDiscoveredDevices.size(), mDiscoveredDevices.size() );
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

int BluetoothDeviceModel::findAddressIndex( const QString &address ) const
{
  int idx = 0;
  for ( const QPair<QString, QString> &device : mDiscoveredDevices )
  {
    if ( device.second == address )
    {
      return idx;
    }
    ++idx;
  }
  //if not found, then switch to the internal device (by index 0)
  return 0;
}

int BluetoothDeviceModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mDiscoveredDevices.size();
}

QVariant BluetoothDeviceModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case Qt::DisplayRole:
      return QStringLiteral( "%1%2" ).arg( mDiscoveredDevices.at( index.row() ).first,
                                            index.row() > 0 ? QStringLiteral( " (%2)" ).arg ( mDiscoveredDevices.at( index.row() ).second ) : QString() );
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
    return;

  qDebug() << "BluetoothDeviceModel: Status of service discovery changed to: " << scanningStatus;
  mScanningStatus = scanningStatus;
  emit scanningStatusChanged( mScanningStatus );
}

void BluetoothDeviceModel::setLastError( const QString &lastError )
{
  qDebug() << "BluetoothDeviceModel: Service discovery error received: " << lastError;
  if ( mLastError == lastError )
    return;

  mLastError = lastError;
  emit lastErrorChanged( mLastError );
}
