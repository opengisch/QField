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
#include <QDebug>

BluetoothDeviceModel::BluetoothDeviceModel( QObject *parent ) : QAbstractListModel( parent )
{
#ifdef ENABLE_BLUETOOTH
  mLocalDevice = std::make_unique<QBluetoothLocalDevice>();
  mServiceDiscoveryAgent = std::make_unique<QBluetoothServiceDiscoveryAgent>;

  connect( mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothDeviceModel::serviceDiscovered );
  connect( mServiceDiscoveryAgent, qgis::overload<QBluetoothServiceDiscoveryAgent::Error>::of( &QBluetoothServiceDiscoveryAgent::error ), [ = ]()
  {
    setLastError( mServiceDiscoveryAgent->errorString() );
    setScanningStatus( Failed );
    endResetModel();
  } );
  connect( mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished, [ = ]()
  {
    setScanningStatus( Succeeded );
    endResetModel();
  } );
  connect( mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled, [ = ]()
  {
    setScanningStatus( Canceled );
    endResetModel();
  } );
#else
  beginResetModel();
  mDiscoveredDevices.clear();
  mDiscoveredDevices.append( qMakePair( tr( "Internal device" ), QString( "internal" ) ) );
  endResetModel();
#endif
}

void BluetoothDeviceModel::startServiceDiscovery( const bool fullDiscovery )
{
  beginResetModel();
  mDiscoveredDevices.clear();
  mDiscoveredDevices.append( qMakePair( tr( "Internal device" ), QString( "internal" ) ) );

  if ( mServiceDiscoveryAgent )
  {
    if ( mServiceDiscoveryAgent->isActive() )
      mServiceDiscoveryAgent->stop();

    mServiceDiscoveryAgent->setUuidFilter( QBluetoothUuid( QBluetoothUuid::SerialPort ) );
    QBluetoothServiceDiscoveryAgent::DiscoveryMode discoveryMode = fullDiscovery ? QBluetoothServiceDiscoveryAgent::FullDiscovery : QBluetoothServiceDiscoveryAgent::MinimalDiscovery;
    mServiceDiscoveryAgent->start( discoveryMode );
    setScanningStatus( Scanning );
  }
  else
  {
    endResetModel();
  }
}

void BluetoothDeviceModel::serviceDiscovered( const QBluetoothServiceInfo &service )
{
  //only list the paired devices so the user has control over it.
  //but in linux (not android) we list unpaired as well, since it needs to repair them later (or pair them at all).
#ifdef Q_OS_ANDROID
  if ( mLocalDevice->pairingStatus( service.device().address() ) != QBluetoothLocalDevice::Unpaired )
  {
    mDiscoveredDevices.append( qMakePair( service.device().name(), service.device().address().toString() ) );
  }
#else
  mDiscoveredDevices.append( qMakePair( service.device().name(), service.device().address().toString() ) );
#endif
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
  if ( role == DisplayStringRole || role == Qt::DisplayRole )
    return QStringLiteral( "%1 (%2)" ).arg( mDiscoveredDevices.at( index.row() ).first, mDiscoveredDevices.at( index.row() ).second );
  else
    return mDiscoveredDevices.at( index.row() ).second;
}

QHash<int, QByteArray> BluetoothDeviceModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayStringRole] = "displayString";
  roles[DeviceAddressRole] = "deviceAddress";

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
