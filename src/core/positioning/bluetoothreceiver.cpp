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

BluetoothReceiver::BluetoothReceiver( const QString &address, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mLocalDevice( std::make_unique<QBluetoothLocalDevice>() )
  , mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) )
{
  connect( mSocket, &QBluetoothSocket::stateChanged, this, &BluetoothReceiver::setSocketState );
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  connect( mSocket, qOverload<QBluetoothSocket::SocketError>( &QBluetoothSocket::error ), this, &BluetoothReceiver::handleError );
#else
  connect( mSocket, qOverload<QBluetoothSocket::SocketError>( &QBluetoothSocket::errorOccurred ), this, &BluetoothReceiver::handleError );
#endif

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished, Qt::UniqueConnection );

#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::errorOccurred, [=]( QBluetoothLocalDevice::Error error ) {
#else
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::error, [=]( QBluetoothLocalDevice::Error error ) {
#endif
    mLastError = QStringLiteral( "BluetoothLocalDevice error: %1 (%2)" ).arg( mAddress, QMetaEnum::fromType<QBluetoothLocalDevice::Error>().valueToKey( error ) );
    emit lastErrorChanged( mLastError );
  } );

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::hostModeStateChanged, this, [=]() {
    if ( mPoweringOn )
    {
      mPoweringOn = false;
      doConnectDevice();
    }
  } );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  // Pairing confirmation is gone in Qt 6
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing, Qt::UniqueConnection );
#endif

  initNmeaConnection( mSocket );

  setValid( !mAddress.isEmpty() );
}

BluetoothReceiver::~BluetoothReceiver()
{
  disconnect( mSocket, &QBluetoothSocket::stateChanged, this, &BluetoothReceiver::setSocketState );

  mSocket->deleteLater();
  mSocket = nullptr;
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
  qInfo() << "BluetoothReceiver: Initiating connection to device: " << mAddress;

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

void BluetoothReceiver::handleError( QBluetoothSocket::SocketError error )
{
  mLastError = QStringLiteral( "%1 (%2)" ).arg( mSocket->errorString(), QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey( error ) );

  switch ( error )
  {
    case QBluetoothSocket::SocketError::HostNotFoundError:
    case QBluetoothSocket::SocketError::RemoteHostClosedError:
    case QBluetoothSocket::SocketError::ServiceNotFoundError:
    case QBluetoothSocket::SocketError::NetworkError:
    case QBluetoothSocket::SocketError::UnsupportedProtocolError:
    case QBluetoothSocket::SocketError::OperationError:
    case QBluetoothSocket::SocketError::UnknownSocketError:
    default:
      break;
  }

  qInfo() << QStringLiteral( "BluetoothReceiver: Error: %1" ).arg( mLastError );
  emit lastErrorChanged( mLastError );
}

void BluetoothReceiver::doConnectDevice()
{
  mConnectOnDisconnect = false;

  setSocketState( QBluetoothSocket::SocketState::ServiceLookupState );

  if ( mLocalDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )
  {
    emit mLastError = QStringLiteral( "POWERING ON!" );
    emit lastErrorChanged( mLastError );

    mPoweringOn = true;
    mLocalDevice->setHostMode( QBluetoothLocalDevice::HostConnectable );
    return;
  }

#ifdef Q_OS_LINUX
  // repairing only needed in the linux (not android) environment
  repairDevice( QBluetoothAddress( mAddress ) );
#else
  mSocket->connectToService( QBluetoothAddress( mAddress ), QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ), QBluetoothSocket::ReadOnly );
#endif
}

void BluetoothReceiver::setSocketState( const QBluetoothSocket::SocketState socketState )
{
  if ( mSocketState == static_cast<QAbstractSocket::SocketState>( socketState ) )
  {
    return;
  }

  switch ( socketState )
  {
    case QBluetoothSocket::SocketState::ServiceLookupState:
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

void BluetoothReceiver::repairDevice( const QBluetoothAddress &address )
{
  if ( mLocalDevice->pairingStatus( address ) == QBluetoothLocalDevice::Paired )
  {
    mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Unpaired );
  }
  else
  {
    mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Paired );
  }
}

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
void BluetoothReceiver::confirmPairing( const QBluetoothAddress &address, QString pin )
{
  Q_UNUSED( address );
  Q_UNUSED( pin );
  mLocalDevice->pairingConfirmation( true );
}
#endif

void BluetoothReceiver::pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status )
{
  if ( QBluetoothAddress( mAddress ) == address )
  {
    switch ( status )
    {
      case QBluetoothLocalDevice::Paired:
      case QBluetoothLocalDevice::AuthorizedPaired:
      {
        emit mLastError = QStringLiteral( "PAIRED!" );
        emit lastErrorChanged( mLastError );
        qDebug() << "BluetoothReceiver): Paired device " << address.toString();
        connectService( address );
        break;
      }

      case QBluetoothLocalDevice::Unpaired:
      {
        emit mLastError = QStringLiteral( "UNPAIRED!" );
        emit lastErrorChanged( mLastError );
        qDebug() << "BluetoothReceive: Unpaired device " << address.toString();
#ifdef Q_OS_LINUX
        repairDevice( address );
#endif
        break;
      }
    }
  }
}

void BluetoothReceiver::connectService( const QBluetoothAddress &address )
{
  mSocket->connectToService( address, QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ), QBluetoothSocket::ReadOnly );
}
