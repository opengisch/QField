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

#include <QDebug>
#include <QSettings>

BluetoothReceiver::BluetoothReceiver( const QString &address, QObject *parent )
  : AbstractGnssReceiver( parent )
  , mAddress( address )
  , mLocalDevice( std::make_unique<QBluetoothLocalDevice>() )
  , mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) )
  , mGpsConnection( std::make_unique<QgsNmeaConnection>( mSocket ) )
{
  connect( mSocket, &QBluetoothSocket::stateChanged, this, &BluetoothReceiver::setSocketState );

  //QgsGpsConnection state changed (received location string)
  connect( mGpsConnection.get(), &QgsGpsConnection::stateChanged, this, &BluetoothReceiver::stateChanged );

  setValid( !mAddress.isEmpty() );
}

void BluetoothReceiver::handleDisconnectDevice()
{
  if ( mSocket->state() != QBluetoothSocket::SocketState::UnconnectedState )
  {
    mDisconnecting = true;
    mLastGnssPositionValid = false;
    mSocket->disconnectFromService();
  }
}

void BluetoothReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qDebug() << "BluetoothReceiver: Initiating connection to device: " << mAddress;

  if ( mSocket->state() != QBluetoothSocket::SocketState::UnconnectedState )
  {
    mConnectOnDisconnect = true;
    disconnectDevice();
  }
  else
  {
    doConnectDevice();
  }
}

void BluetoothReceiver::doConnectDevice()
{
  mConnectOnDisconnect = false;

  //repairing only needed in the linux (not android) environment
#ifdef Q_OS_LINUX
  repairDevice( QBluetoothAddress( mAddress ) );
#else
  mSocket->connectToService( QBluetoothAddress( mAddress ), QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ), QBluetoothSocket::ReadOnly );
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
  mLastGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude, mEllipsoidalElevation ? info.elevation + info.elevation_diff : info.elevation,
                                                          info.speed * 1000 / 60 / 60, info.direction, info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                          info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality, info.satellitesUsed, info.status,
                                                          info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                          0, QStringLiteral( "nmea" ) );
  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
}

void BluetoothReceiver::setSocketState( const QBluetoothSocket::SocketState socketState )
{
  if ( mSocketState == static_cast<QAbstractSocket::SocketState>( socketState ) )
  {
    return;
  }

  switch ( socketState )
  {
    case QBluetoothSocket::SocketState::ConnectingState:
    {
      mSocketStateString = tr( "Connecting…" );
      break;
    }
    case QBluetoothSocket::SocketState::ConnectedState:
    {
      mSocketStateString = tr( "Successfully connected" );
      break;
    }
    case QBluetoothSocket::SocketState::UnconnectedState:
    {
      mSocketStateString = tr( "Disconnected" );
      if ( !mDisconnecting && mSocket->error() != QBluetoothSocket::SocketError::NoSocketError )
        mSocketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );

      if ( mConnectOnDisconnect )
        doConnectDevice();
      break;
    }
    default:
    {
      mSocketStateString = tr( "Socket state %1" ).arg( static_cast<int>( socketState ) );
    }
  }

  mSocketState = static_cast<QAbstractSocket::SocketState>( socketState );
  emit socketStateChanged( mSocketState );
  emit socketStateStringChanged( mSocketStateString );
}

void BluetoothReceiver::setEllipsoidalElevation( const bool ellipsoidalElevation )
{
  if ( mEllipsoidalElevation == ellipsoidalElevation )
    return;

  mEllipsoidalElevation = ellipsoidalElevation;
}

#ifdef Q_OS_LINUX
void BluetoothReceiver::repairDevice( const QBluetoothAddress &address )
{
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::error, [=]( QBluetoothLocalDevice::Error error ) {
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
