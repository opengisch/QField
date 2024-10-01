/***************************************************************************
 tcpreceiver.cpp - TcpReceiver

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

#include "tcpreceiver.h"

TcpReceiver::TcpReceiver( const QString &address, const int port, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mPort( port )
  , mSocket( new QTcpSocket() )
{
  connect( mSocket, &QAbstractSocket::stateChanged, this, &TcpReceiver::setSocketState );
  connect( mSocket, qOverload<QAbstractSocket::SocketError>( &QAbstractSocket::errorOccurred ), this, &TcpReceiver::handleError );

  connect( mSocket, &QAbstractSocket::connected, this, [=] {
    // This line enables gpsd's NMEA through TCP
    mSocket->write( QStringLiteral( "?WATCH={\"enable\":true,\"nmea\":true,\"raw\":true};" ).toUtf8() );
  } );

  mReconnectTimer.setSingleShot( true );
  connect( &mReconnectTimer, &QTimer::timeout, this, [this]() {
    mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadOnly );
  } );

  setValid( !mAddress.isEmpty() && mPort > 0 );
  initNmeaConnection( mSocket );
}

TcpReceiver::~TcpReceiver()
{
  disconnect( mSocket, &QAbstractSocket::stateChanged, this, &TcpReceiver::setSocketState );

  mSocket->deleteLater();
  mSocket = nullptr;
}

void TcpReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() || mPort == 0 )
  {
    return;
  }
  qInfo() << QStringLiteral( "TcpReceiver: Initiating connection to address %1 (port %2)" ).arg( mAddress, QString::number( mPort ) );
  mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadWrite );
}

void TcpReceiver::handleDisconnectDevice()
{
  mReconnectOnDisconnect = false;
  mSocket->disconnectFromHost();
}

void TcpReceiver::setSocketState( const QAbstractSocket::SocketState socketState )
{
  if ( mSocketState == socketState )
  {
    return;
  }

  switch ( socketState )
  {
    case QAbstractSocket::HostLookupState:
    case QAbstractSocket::ConnectingState:
    {
      mSocketStateString = tr( "Connecting…" );
      break;
    }
    case QAbstractSocket::ConnectedState:
    {
      mReconnectOnDisconnect = true;
      mSocketStateString = tr( "Successfully connected" );
      break;
    }
    case QAbstractSocket::UnconnectedState:
    {
      mSocketStateString = tr( "Disconnected" );
      if ( mReconnectOnDisconnect )
      {
        mSocketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );
        mReconnectTimer.start( 2000 );
      }
      break;
    }
    default:
    {
      mSocketStateString = tr( "Socket state %1" ).arg( static_cast<int>( socketState ) );
    }
  }

  mSocketState = socketState;
  emit socketStateChanged( mSocketState );
  emit socketStateStringChanged( mSocketStateString );
}

void TcpReceiver::handleError( QAbstractSocket::SocketError error )
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

  emit lastErrorChanged( mLastError );
}
