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

#include <QSettings>

TcpReceiver::TcpReceiver( const QString &address, const int port, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mPort( port )
  , mSocket( std::make_unique<QTcpSocket>() )
{
  connect( mSocket.get(), &QAbstractSocket::stateChanged, this, &TcpReceiver::setSocketState );

  mReconnectTimer.setSingleShot( true );
  connect( &mReconnectTimer, &QTimer::timeout, this, [this]() {
    mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadOnly );
  } );

  setValid( !mAddress.isEmpty() && mPort > 0 );
  initNmeaConnection( mSocket.get() );
}

void TcpReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() || mPort == 0 )
  {
    return;
  }
  qDebug() << QStringLiteral( "TcpReceiver: Initiating connection to address %1 (port %2)" ).arg( mAddress, QString::number( mPort ) );
  mSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadOnly );
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
