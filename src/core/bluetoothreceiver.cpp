/***************************************************************************
 bluetoothreceiver.cpp - BluetoothReceiver

 ---------------------
 begin                : 18.11.2020
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

#include "bluetoothreceiver.h"
#include <QSettings>
#include <QDebug>

BluetoothReceiver::BluetoothReceiver( QObject *parent ) : QObject( parent ),
  mLocalDevice( std::make_unique<QBluetoothLocalDevice>() ),
  mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) ),
  mGpsConnection( std::make_unique<QgsNmeaConnection>( mSocket ) )
{
  //socket state changed
  connect( mSocket, &QBluetoothSocket::stateChanged, this, &BluetoothReceiver::setSocketState );

  //QgsGpsConnection state changed (received location string)
  connect( mGpsConnection.get(), &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged );
}

void BluetoothReceiver::disconnectDevice()
{
  if ( mSocket->state() != QBluetoothSocket::UnconnectedState )
  {
    mDisconnecting = true;
    mLastGnssPositionValid = false;
    mSocket->disconnectFromService();
  }
}

void BluetoothReceiver::connectDevice( const QString &address )
{
  if ( address.isEmpty() )
  {
    disconnectDevice();
    return;
  }

  qDebug() << "BluetoothReceiver: Connect device: " << address;

  if ( mSocket->state() != QBluetoothSocket::UnconnectedState )
  {
    mAddressToConnect = address;
    disconnectDevice();
  }
  else
  {
    doConnectDevice( address );
  }
}

void BluetoothReceiver::doConnectDevice( const QString &address )
{
  mAddressToConnect.clear();

  //repairing only needed in the linux (not android) environment
#ifndef Q_OS_ANDROID
  repairDevice( QBluetoothAddress( address ) );
#else
  mSocket->connectToService( QBluetoothAddress( address ), QBluetoothUuid( QBluetoothUuid::SerialPort ), QBluetoothSocket::ReadOnly );
#endif
}

void BluetoothReceiver::stateChanged( const QgsGpsInformation &info )
{
  if ( mLastGnssPositionValid && std::isnan( info.latitude ) )
  {
    //we already sent a valid position, stick to last valid position
    return;
  }
  mLastGnssPositionValid = !std::isnan( info.latitude );

  // QgsGpsInformation's speed is served in km/h, translate to m/s
  mLastGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude, mEllipsoidalElevation ? info.elevation + info.elevation_diff : info.elevation, info.speed * 1000 / 60 / 60, info.direction, info.satellitesInView, info.pdop,
                                 info.hdop, info.vdop, info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality,
                                 info.satellitesUsed, info.status, info.satPrn, info.satInfoComplete );
  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
}

void BluetoothReceiver::setSocketState( const QBluetoothSocket::SocketState socketState )
{
  if ( mSocketState == socketState )
    return;

  switch ( socketState )
  {
    case QBluetoothSocket::ConnectingState:
    {
      mSocketStateString = tr( "Connecting…" );
      break;
    }
    case QBluetoothSocket::ConnectedState:
    {
      mSocketStateString = tr( "Successfully connected" );
      break;
    }
    case QBluetoothSocket::UnconnectedState:
    {
      mSocketStateString = tr( "Disconnected" );
      if ( !mDisconnecting && mSocket->error() != QBluetoothSocket::NoSocketError )
        mSocketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );

      if ( !mAddressToConnect.isEmpty() )
        doConnectDevice( mAddressToConnect );
      break;
    }
    default:
    {
      mSocketStateString = tr( "Socket state %1" ).arg( QMetaEnum::fromType<QBluetoothSocket::SocketState>().valueToKey( socketState ) );
    }
  }

  mSocketState = socketState;
  emit socketStateChanged( mSocketState );
  emit socketStateStringChanged( mSocketStateString );
}

GnssPositionInformation BluetoothReceiver::createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName )
{
  return GnssPositionInformation( latitude, longitude, altitude, speed, direction, QList<QgsSatelliteInfo>(), 0, 0, 0, horizontalAccuracy, verticalAcurracy, timestamp, QChar(), 0, -1, 0, QChar( 'A' ), QList<int>(), false, verticalSpeed, magneticVariation, sourceName );
}

void BluetoothReceiver::setEllipsoidalElevation( const bool ellipsoidalElevation )
{
  if ( mEllipsoidalElevation == ellipsoidalElevation )
    return;

  mEllipsoidalElevation = ellipsoidalElevation;
  emit ellipsoidalElevationChanged();
}

#ifndef Q_OS_ANDROID
void BluetoothReceiver::repairDevice( const QBluetoothAddress &address )
{
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
  connect( mLocalDevice.get(),  &QBluetoothLocalDevice::error, [ = ]( QBluetoothLocalDevice::Error error )
  {
    qDebug() << "BluetoothReceiver (not android): Re-pairing device " << address.toString() << " failed:" << error;
  } );

  if ( mLocalDevice->pairingStatus( address ) == QBluetoothLocalDevice::Paired )
  {
    mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Unpaired );
  }
  else
  {
    mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Paired );
  }
}

void BluetoothReceiver::confirmPairing( const QBluetoothAddress &address, QString pin )
{
  Q_UNUSED( address );
  Q_UNUSED( pin );
  mLocalDevice->pairingConfirmation( true );
}

void BluetoothReceiver::pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status )
{
  if ( status == QBluetoothLocalDevice::Paired )
  {
    qDebug() << "BluetoothReceiver (not android): Paired device " << address.toString();
    disconnect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished );
    connectService( address );
  }
  else
  {
    qDebug() << "BluetoothReceiver (not android): Unpaired device " << address.toString();
    repairDevice( address );
  }
}

void BluetoothReceiver::connectService( const QBluetoothAddress &address )
{
  mSocket->connectToService( address, QBluetoothUuid( QBluetoothUuid::SerialPort ), QBluetoothSocket::ReadOnly );
}
#endif
