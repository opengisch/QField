/***************************************************************************
 qftcpreceiver.cpp - QfTcpReceiver

 ---------------------
 begin                : September 2022
 copyright            : (C) 2022 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qftcpreceiver.h"

QLatin1String QfTcpReceiver::identifier = QLatin1String( "tcp" );

QfTcpReceiver::QfTcpReceiver( const QString &address, const int port, QObject *parent )
  : QfNmeaGnssReceiver( parent )
  , mAddress( address )
  , mPort( port )
  , mSocket( new QTcpSocket() )
{
  connect( mSocket, qOverload<QAbstractSocket::SocketError>( &QAbstractSocket::errorOccurred ), this, &QfTcpReceiver::handleError );
  connect( mSocket, &QTcpSocket::stateChanged, this, &QfTcpReceiver::handleStateChanged );

  connect( mSocket, &QAbstractSocket::connected, this, [this] {
    // This line enables gpsd's NMEA through TCP
    mSocket->write( QStringLiteral( "?WATCH={\"enable\":true,\"nmea\":true,\"raw\":true};" ).toUtf8() );
  } );

  mReconnectTimer.setSingleShot( true );
  connect( &mReconnectTimer, &QTimer::timeout, this, [this]() {
    mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadWrite );
  } );

  setValid( !mAddress.isEmpty() && mPort > 0 );
  initNmeaConnection( mSocket );
}

QfTcpReceiver::~QfTcpReceiver()
{
  disconnectDevice();
  mSocket->deleteLater();
  mSocket = nullptr;
}

QfAbstractGnssReceiver::Capabilities QfTcpReceiver::capabilities() const
{
  return QfAbstractGnssReceiver::Capabilities() | QfAbstractGnssReceiver::OrthometricAltitude | QfAbstractGnssReceiver::Logging | QfAbstractGnssReceiver::NtripCorrection;
}

void QfTcpReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() || mPort == 0 )
  {
    return;
  }
  qInfo() << QStringLiteral( "TcpReceiver: Initiating connection to address %1 (port %2)" ).arg( mAddress, QString::number( mPort ) );
  mConnectionFailureCount = 0;
  mReconnectOnDisconnect = true;
  mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadWrite );
}

void QfTcpReceiver::handleDisconnectDevice()
{
  mReconnectOnDisconnect = false;
  mReconnectTimer.stop();
  if ( mSocket->state() != QAbstractSocket::UnconnectedState )
  {
    mSocket->disconnectFromHost();
  }
  else
  {
    // The socket state might not have been set to unconnected due to auto-reconnect, ensure it is updated
    setSocketState( QAbstractSocket::UnconnectedState );
  }
}

QString QfTcpReceiver::socketStateString()
{
  const QAbstractSocket::SocketState currentState = socketState();
  QString socketStateString = QfAbstractGnssReceiver::socketStateString();
  if ( currentState == QAbstractSocket::UnconnectedState && mReconnectOnDisconnect )
  {
    socketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );
  }
  return socketStateString;
}

void QfTcpReceiver::handleStateChanged( QAbstractSocket::SocketState state )
{
  switch ( state )
  {
    case QAbstractSocket::SocketState::UnconnectedState:
      if ( mReconnectOnDisconnect )
      {
        mReconnectTimer.start( 2000 );
        return;
      }
      break;

    case QAbstractSocket::BoundState:
    case QAbstractSocket::ClosingState:
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
    case QAbstractSocket::ConnectedState:
    case QAbstractSocket::ListeningState:
      break;
  }

  setSocketState( state );
}

void QfTcpReceiver::handleError( QAbstractSocket::SocketError error )
{
  switch ( error )
  {
    case QAbstractSocket::HostNotFoundError:
      mLastError = tr( "Could not find the remote host" );
      break;
    case QAbstractSocket::NetworkError:
      mLastError = tr( "Attempt to read or write from socket returned an error" );
      break;
    case QAbstractSocket::ConnectionRefusedError:
      mLastError = tr( "The connection was refused by the remote host" );
      break;
    default:
      mLastError = tr( "TCP receiver error (%1)" ).arg( QMetaEnum::fromType<QAbstractSocket::SocketError>().valueToKey( error ) );
      break;
  }
  qInfo() << QStringLiteral( "TcpReceiver: Error: %1" ).arg( mLastError );

  if ( mReconnectOnDisconnect )
  {
    mConnectionFailureCount++;
  }

  if ( mConnectionFailureCount > 10 )
  {
    mReconnectOnDisconnect = false;
  }

  if ( !mReconnectOnDisconnect || mSocket->state() != QAbstractSocket::SocketState::UnconnectedState )
  {
    emit lastErrorChanged( mLastError );
  }
}
