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
#include <QGuiApplication>
#include <QTimer>

BluetoothReceiver::BluetoothReceiver( const QString &address, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mLocalDevice( std::make_unique<QBluetoothLocalDevice>() )
  , mSocket( new QBluetoothSocket( QBluetoothServiceInfo::RfcommProtocol ) )
{
  connect( mSocket, qOverload<QBluetoothSocket::SocketError>( &QBluetoothSocket::errorOccurred ), this, &BluetoothReceiver::handleErrorOccurred );
  connect( mSocket, &QBluetoothSocket::stateChanged, this, &BluetoothReceiver::handleStateChanged );

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished );
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::errorOccurred, this, [this]( QBluetoothLocalDevice::Error error ) {
    if ( error != QBluetoothLocalDevice::NoError )
    {
      mLastError = QStringLiteral( "Local device returned an error (%1) for %2" ).arg( QMetaEnum::fromType<QBluetoothLocalDevice::Error>().valueToKey( error ), mAddress );

      qInfo() << QStringLiteral( "BluetoothReceiver: %1" ).arg( mLastError );
      emit lastErrorChanged( mLastError );
    }
  } );

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::hostModeStateChanged, this, [this]() {
    if ( mPoweringOn )
    {
      qInfo() << QStringLiteral( "BluetoothReceiver: Local device powered on" );
      mPoweringOn = false;
      doConnectDevice();
    }
  } );

  initNmeaConnection( mSocket );

  setValid( !mAddress.isEmpty() );
}

BluetoothReceiver::~BluetoothReceiver()
{
  disconnectDevice();
  mSocket->deleteLater();
  mSocket = nullptr;
}

void BluetoothReceiver::handleDisconnectDevice()
{
  if ( mSocket->state() != QBluetoothSocket::SocketState::UnconnectedState )
  {
    qInfo() << "BluetoothReceiver: Disconnecting from device: " << mAddress;
    mConnectOnDisconnect = false;
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

  mConnectionFailureCount = 0;
  mConnectOnDisconnect = true;
  if ( mSocket->state() == QBluetoothSocket::SocketState::ConnectedState )
  {
    disconnectDevice();
  }
  else
  {
    doConnectDevice();
  }
}

void BluetoothReceiver::handleStateChanged( QBluetoothSocket::SocketState state )
{
  qDebug() << "bbb handleStateChanged" << state;
  QAbstractSocket::SocketState currentState;
  switch ( state )
  {
    case QBluetoothSocket::SocketState::UnconnectedState:
      currentState = QAbstractSocket::UnconnectedState;
      break;
    case QBluetoothSocket::SocketState::ConnectingState:
      currentState = QAbstractSocket::ConnectingState;
      break;
    case QBluetoothSocket::SocketState::ConnectedState:
      currentState = QAbstractSocket::ConnectedState;
      break;
    case QBluetoothSocket::SocketState::ClosingState:
      currentState = QAbstractSocket::ClosingState;
      break;
    case QBluetoothSocket::SocketState::ListeningState:
      currentState = QAbstractSocket::ListeningState;
      break;
    default:
      currentState = QAbstractSocket::UnconnectedState;
      break;
  }

  if ( currentState == QAbstractSocket::UnconnectedState && mConnectOnDisconnect )
  {
    qDebug() << "bbb singleShot";
    QTimer::singleShot( 1000, this, &BluetoothReceiver::doConnectDevice );
  }
  else
  {
    setSocketState( currentState );
  }
}

void BluetoothReceiver::handleErrorOccurred( QBluetoothSocket::SocketError error )
{
  switch ( error )
  {
    case QBluetoothSocket::SocketError::HostNotFoundError:
    case QBluetoothSocket::SocketError::ServiceNotFoundError:
    case QBluetoothSocket::SocketError::RemoteHostClosedError:
    case QBluetoothSocket::SocketError::NetworkError:
    case QBluetoothSocket::SocketError::UnsupportedProtocolError:
    case QBluetoothSocket::SocketError::OperationError:
    case QBluetoothSocket::SocketError::UnknownSocketError:
    default:
      break;
  }

  mLastError = QStringLiteral( "%1 (%2)" ).arg( mSocket->errorString(), QMetaEnum::fromType<QBluetoothSocket::SocketError>().valueToKey( static_cast<int>( error ) ) );
  qInfo() << QStringLiteral( "BluetoothReceiver: Error: %1" ).arg( mLastError );

  if ( mConnectOnDisconnect )
  {
    mConnectionFailureCount++;
  }

  bool emitMessage = false;
  if ( mSocket->isOpen() )
  {
    emitMessage = true;
    mSocket->close();
  }

  if ( mConnectionFailureCount > 10 )
  {
    emitMessage = true;
    mConnectOnDisconnect = false;
  }

  if ( emitMessage )
  {
    emit lastErrorChanged( mLastError );
  }
}

void BluetoothReceiver::doConnectDevice()
{
  if ( mLocalDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )
  {
    qInfo() << QStringLiteral( "BluetoothReceiver: Powering on local device" );

    mPoweringOn = true;
    mLocalDevice->setHostMode( QBluetoothLocalDevice::HostConnectable );
    return;
  }

  repairDevice( QBluetoothAddress( mAddress ) );
}

QString BluetoothReceiver::socketStateString()
{
  const QAbstractSocket::SocketState currentState = socketState();
  QString socketStateString = AbstractGnssReceiver::socketStateString();
  if ( currentState == QAbstractSocket::UnconnectedState )
  {
    if ( !mDisconnecting && mSocket->error() != QBluetoothSocket::SocketError::NoSocketError )
    {
      socketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );
    }
  }
  return socketStateString;
}

void BluetoothReceiver::repairDevice( const QBluetoothAddress &address )
{
  const QBluetoothLocalDevice::Pairing pairingStatus = mLocalDevice->pairingStatus( address );
  qInfo() << QStringLiteral( "BluetoothReceiver: Pre-connection pairing status for %1: %2" ).arg( mAddress, QMetaEnum::fromType<QBluetoothLocalDevice::Pairing>().valueToKey( pairingStatus ) );
  switch ( pairingStatus )
  {
    case QBluetoothLocalDevice::Paired:
    case QBluetoothLocalDevice::AuthorizedPaired:
    {
      mSocket->connectToService( address, QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ), QBluetoothSocket::ReadOnly );
      break;
    }

    case QBluetoothLocalDevice::Unpaired:
    {
      mLocalDevice->requestPairing( address, QBluetoothLocalDevice::Paired );
      break;
    }
  }
}

void BluetoothReceiver::pairingFinished( const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing status )
{
  if ( QBluetoothAddress( mAddress ) == address )
  {
    qInfo() << QStringLiteral( "BluetoothReceiver: Finished pairing status for %1: %2" ).arg( mAddress, QMetaEnum::fromType<QBluetoothLocalDevice::Pairing>().valueToKey( status ) );
    switch ( status )
    {
      case QBluetoothLocalDevice::Paired:
      case QBluetoothLocalDevice::AuthorizedPaired:
      {
        mSocket->connectToService( address, QBluetoothUuid( QBluetoothUuid::ServiceClassUuid::SerialPort ), QBluetoothSocket::ReadOnly );
        break;
      }

      case QBluetoothLocalDevice::Unpaired:
      {
#ifdef Q_OS_LINUX
        repairDevice( address );
#endif
        break;
      }
    }
  }
}
