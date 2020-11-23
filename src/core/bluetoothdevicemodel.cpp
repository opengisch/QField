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
  : QAbstractListModel( parent )
{
    qDebug() << "here I am";

    qDebug() << "1 Model";
    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,this, &BluetoothDeviceModel::serviceDiscovered);

    connect(&mServiceDiscoveryAgent, QOverload<QBluetoothServiceDiscoveryAgent::Error>::of(&QBluetoothServiceDiscoveryAgent::error),[=](QBluetoothServiceDiscoveryAgent::Error error){
        qDebug() << "ServiceAgent ERROR:" << error;
        setScanning(false);
        endResetModel();
    });

    qDebug() << "2 Model";
    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished,[=](){
       qDebug() << "ServiceAgent finished";
       setScanning(false);
       endResetModel();
    });

    connect(&mServiceDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::canceled,[=](){
       qDebug() << "ServiceAgent canceled";
       setScanning(false);
       endResetModel();
    });

    qDebug() << "created Model";
}

void BluetoothDeviceModel::startServiceDiscovery()
{
    beginResetModel();
    mDiscoveredDevices.clear();
    mDiscoveredDevices.append( qMakePair( tr("Internal GPS"), QString("internal") ) );

    if( mServiceDiscoveryAgent.isActive() )
        mServiceDiscoveryAgent.stop();

    mServiceDiscoveryAgent.setUuidFilter(QBluetoothUuid(QBluetoothUuid::SerialPort));
    mServiceDiscoveryAgent.start();

    if (!mServiceDiscoveryAgent.isActive() ||
            mServiceDiscoveryAgent.error() != QBluetoothServiceDiscoveryAgent::NoError) {
        qDebug() << "Cannot find remote services. "<< mServiceDiscoveryAgent.errorString();
    } else {
        qDebug() << "Scanning...";
        setScanning(true);
    }
}

void BluetoothDeviceModel::serviceDiscovered(const QBluetoothServiceInfo &service)
{
    qDebug() << "FOUND DEVICE: "<<service.device().name()<<'(' << service.device().address().toString() << ')' <<':' << service.serviceName() << " UUID:"<< service.serviceUuid();
    mDiscoveredDevices.append( qMakePair( service.device().name(), service.device().address().toString() ) );
}

bool BluetoothDeviceModel::scanning() const
{
    return mScanning;
}

void BluetoothDeviceModel::setScanning( bool scanning )
{
    if ( scanning == mScanning )
        return;

    mScanning = scanning;
    emit scanningChanged();
}

int BluetoothDeviceModel::findAddessIndex( const QString &address ) const
{
  int idx = 0;
  for ( const QPair<QString, QString> &device : mDiscoveredDevices )
  {
    if ( device.second == address )
      return idx;

    ++idx;
  }

  return -1;
}

QString BluetoothDeviceModel::findIndexAddess( int idx ) const
{
  return mDiscoveredDevices.at(idx).second;
}

int BluetoothDeviceModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mDiscoveredDevices.size();
}

QVariant BluetoothDeviceModel::data( const QModelIndex &index, int role ) const
{
  if ( role == DisplayStringRole || role == Qt::DisplayRole )
    return mDiscoveredDevices.at( index.row() ).first;
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
