

# File tcpreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**tcpreceiver.cpp**](tcpreceiver_8cpp.md)

[Go to the documentation of this file](tcpreceiver_8cpp.md)


```C++
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

QLatin1String TcpReceiver::identifier = QLatin1String( "tcp" );

TcpReceiver::TcpReceiver( const QString &address, const int port, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mPort( port )
  , mSocket( new QTcpSocket() )
{
  connect( mSocket, qOverload<QAbstractSocket::SocketError>( &QAbstractSocket::errorOccurred ), this, &TcpReceiver::handleError );
  connect( mSocket, &QTcpSocket::stateChanged, this, [this]( QAbstractSocket::SocketState state ) {
    setSocketState( state );
    if ( state == QAbstractSocket::SocketState::UnconnectedState && mReconnectOnDisconnect )
    {
      mReconnectTimer.start( 2000 );
    }
  } );

  connect( mSocket, &QAbstractSocket::connected, this, [this] {
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

QString TcpReceiver::socketStateString()
{
  const QAbstractSocket::SocketState currentState = socketState();
  QString socketStateString = AbstractGnssReceiver::socketStateString();
  if ( currentState == QAbstractSocket::UnconnectedState && mReconnectOnDisconnect )
  {
    socketStateString.append( QStringLiteral( ": %1" ).arg( mSocket->errorString() ) );
  }
  return socketStateString;
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
```


