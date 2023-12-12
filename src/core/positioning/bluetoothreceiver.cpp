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

#if QT_VERSION >= QT_VERSION_CHECK( 6, 6, 0 )
#include <QPermissions>
#endif

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

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingFinished, this, &BluetoothReceiver::pairingFinished );

#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::errorOccurred, [=]( QBluetoothLocalDevice::Error error ) {
#else
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::error, [=]( QBluetoothLocalDevice::Error error ) {
#endif
    if ( error != QBluetoothLocalDevice::NoError )
    {
      mLastError = QStringLiteral( "Local device returned an error (%1) for %2" ).arg( QMetaEnum::fromType<QBluetoothLocalDevice::Error>().valueToKey( error ), mAddress );

      qInfo() << QStringLiteral( "BluetoothReceiver: %1" ).arg( mLastError );
      emit lastErrorChanged( mLastError );
    }
  } );

  connect( mLocalDevice.get(), &QBluetoothLocalDevice::hostModeStateChanged, this, [=]() {
    if ( mPoweringOn )
    {
      qInfo() << QStringLiteral( "BluetoothReceiver: Local device powered on" );
      mPoweringOn = false;
      doConnectDevice();
    }
  } );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
  // Pairing confirmation is gone in Qt 6
  connect( mLocalDevice.get(), &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothReceiver::confirmPairing );
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
    qInfo() << "BluetoothReceiver: Disconnecting from device: " << mAddress;
    mDisconnecting = true;
    mLastGnssPositionValid = false;
    mSocket->disconnectFromService();
  }
}

void BluetoothReceiver::handleConnectDevice()
{
#if QT_VERSION >= QT_VERSION_CHECK( 6, 6, 0 )
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
          handleConnectDevice();
        }
        else
        {
          setValid( false );
          mLastError = tr( "Bluetooth permission denied" );
          emit lastErrorChanged( mLastError );
        }
      } );
      return;
    }
    else if ( permissionStatus == Qt::PermissionStatus::Denied )
    {
      setValid( false );
      mLastError = tr( "Bluetooth permission denied" );
      emit lastErrorChanged( mLastError );
      return;
    }
  }
#endif
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qInfo() << "BluetoothReceiver: Initiating connection to device: " << mAddress;

  if ( mSocket->state() == QBluetoothSocket::SocketState::ConnectedState )
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
  mLastError = QStringLiteral( "%1 (%2)" ).arg( mSocket->errorString(), QMetaEnum::fromType<QBluetoothSocket::SocketError>().valueToKey( static_cast<int>( error ) ) );

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

  if ( mLocalDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )
  {
    qInfo() << QStringLiteral( "BluetoothReceiver: Powering on local device" );

    mPoweringOn = true;
    mLocalDevice->setHostMode( QBluetoothLocalDevice::HostConnectable );
    return;
  }

  repairDevice( QBluetoothAddress( mAddress ) );
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
