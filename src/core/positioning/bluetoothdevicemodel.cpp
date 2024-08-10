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

void BluetoothDeviceModel::initiateDiscoveryAgent()
{
  mLocalDevice = std::make_unique<QBluetoothLocalDevice>();
  mServiceDiscoveryAgent = std::make_unique<QBluetoothServiceDiscoveryAgent>();

  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothDeviceModel::serviceDiscovered );
  connect( mServiceDiscoveryAgent.get(), qOverload<QBluetoothServiceDiscoveryAgent::Error>( &QBluetoothServiceDiscoveryAgent::errorOccurred ), this, [=]( QBluetoothServiceDiscoveryAgent::Error error ) {
    if ( error != QBluetoothServiceDiscoveryAgent::NoError )
    {
      setLastError( mServiceDiscoveryAgent->errorString() );
      setScanningStatus( Failed );
    }
  } );
  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::finished, [=]() {
    setScanningStatus( mServiceDiscoveryAgent->error() == QBluetoothServiceDiscoveryAgent::NoError ? Succeeded : Failed );
  } );
  connect( mServiceDiscoveryAgent.get(), &QBluetoothServiceDiscoveryAgent::canceled, [=]() {
    setScanningStatus( Canceled );
  } );
}

void BluetoothDeviceModel::startServiceDiscovery( const bool fullDiscovery )
{
  if ( !mPermissionChecked )
  {
    QBluetoothPermission bluetoothPermission;
    bluetoothPermission.setCommunicationModes( QBluetoothPermission::Access );
    Qt::PermissionStatus permissionStatus = qApp->checkPermission( bluetoothPermission );
    if ( permissionStatus == Qt::PermissionStatus::Undetermined )
    {
      qApp->requestPermission( bluetoothPermission, this, [=]( const QPermission &permission ) {
        if ( permission.status() == Qt::PermissionStatus::Granted )
        {
          mPermissionChecked = true;
          startServiceDiscovery( fullDiscovery );
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
      mLastError = tr( "Bluetooth permission denied" );
      emit lastErrorChanged( mLastError );
      return;
    }
  }

  if ( !mServiceDiscoveryAgent )
    initiateDiscoveryAgent();

  if ( mServiceDiscoveryAgent->isActive() )
    mServiceDiscoveryAgent->stop();

  mServiceDiscoveryAgent->setUuidFilter( QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ) );
  QBluetoothServiceDiscoveryAgent::DiscoveryMode discoveryMode = fullDiscovery ? QBluetoothServiceDiscoveryAgent::FullDiscovery : QBluetoothServiceDiscoveryAgent::MinimalDiscovery;

  // set scanning status _prior to_ start as start itself can error and then we get a broken status sequence
  setScanningStatus( Scanning );
  mServiceDiscoveryAgent->clear();
  mServiceDiscoveryAgent->start( discoveryMode );
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
  Q_UNUSED( parent )
  return mDiscoveredDevices.size();
}

QVariant BluetoothDeviceModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case Qt::DisplayRole:
      return QStringLiteral( "%1%2" ).arg( mDiscoveredDevices.at( index.row() ).first,
                                           index.row() > 0 ? QStringLiteral( " (%2)" ).arg( mDiscoveredDevices.at( index.row() ).second ) : QString() );
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

  int index = mDiscoveredDevices.size();
  beginInsertRows( QModelIndex(), index, index );
  mDiscoveredDevices << qMakePair( name, address );
  endInsertRows();

  return index;
}
