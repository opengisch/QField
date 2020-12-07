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

BluetoothDeviceModel::BluetoothDeviceModel( QObject *parent )
  : QAbstractListModel( parent ),
    mLocalDevice( new QBluetoothLocalDevice )
{
    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,this, &BluetoothDeviceModel::serviceDiscovered);
    connect(&mServiceDiscoveryAgent, QOverload<QBluetoothServiceDiscoveryAgent::Error>::of(&QBluetoothServiceDiscoveryAgent::error),[=](){
        setLastError( mServiceDiscoveryAgent.errorString() );
        setScanningStatus(Failed);
        endResetModel();
    });
    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,[=](){
       setScanningStatus(Succeeded);
       endResetModel();
    });
    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled,[=](){
       setScanningStatus(Canceled);
       endResetModel();
    });
}

void BluetoothDeviceModel::startServiceDiscovery()
{
    beginResetModel();
    mDiscoveredDevices.clear();
    mDiscoveredDevices.append( qMakePair( tr("Internal device"), QString("internal") ) );

    if( mServiceDiscoveryAgent.isActive() )
        mServiceDiscoveryAgent.stop();

    mServiceDiscoveryAgent.setUuidFilter(QBluetoothUuid(QBluetoothUuid::SerialPort));
    mServiceDiscoveryAgent.start();

    if (!mServiceDiscoveryAgent.isActive() ||
            mServiceDiscoveryAgent.error() != QBluetoothServiceDiscoveryAgent::NoError) {
        qDebug() << "Cannot find remote services. "<< mServiceDiscoveryAgent.errorString();
    } else {
        qDebug() << "Scanning...";
        setScanningStatus(Scanning);
    }
}

void BluetoothDeviceModel::serviceDiscovered(const QBluetoothServiceInfo &service)
{
    qDebug() << "FOUND DEVICE: "<<service.device().name()<<'(' << service.device().address().toString() << ')' <<':' << service.serviceName() << " UUID:"<< service.serviceUuid();
#ifdef Q_OS_ANDROID
    if( mLocalDevice->pairingStatus(service.device().address()) != QBluetoothLocalDevice::Unpaired )
    {
        mDiscoveredDevices.append( qMakePair( service.device().name(), service.device().address().toString() ) );
    }
    else{
        qDebug() << "do not append it since it's unpaired: "<< service.device().name();
    }
#else
     mDiscoveredDevices.append( qMakePair( service.device().name(), service.device().address().toString() ) );
#endif
}

int BluetoothDeviceModel::findAddessIndex( const QString &address ) const
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
    return QStringLiteral( "%1 (%2)"  ).arg( mDiscoveredDevices.at( index.row() ).first, mDiscoveredDevices.at( index.row() ).second );
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

void BluetoothDeviceModel::setScanningStatus(BluetoothDeviceModel::ScanningStatus scanningStatus)
{
    if (mScanningStatus == scanningStatus)
        return;

    qDebug() << "scanning status "<<scanningStatus;
    mScanningStatus = scanningStatus;
    emit scanningStatusChanged(mScanningStatus);
}

void BluetoothDeviceModel::setLastError(QString lastError)
{
    if (mLastError == lastError)
        return;

    mLastError = lastError;
    emit lastErrorChanged(mLastError);
}
