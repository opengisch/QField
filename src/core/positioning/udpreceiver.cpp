/***************************************************************************
 udpreceiver.cpp - UdpReceiver

 ---------------------
 begin                : December 2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "udpreceiver.h"

#include <QSettings>

UdpReceiver::UdpReceiver( const QString &address, const int port, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mPort( port )
  , mSocket( new QUdpSocket() )
  , mBuffer( new QBuffer() )
{
  connect( mSocket, &QAbstractSocket::stateChanged, this, &UdpReceiver::setSocketState );
  connect( mSocket, &QUdpSocket::readyRead, this, [=]() {
    QByteArray datagram;
    while ( mSocket->hasPendingDatagrams() )
    {
      datagram.resize( int( mSocket->pendingDatagramSize() ) );
      mSocket->readDatagram( datagram.data(), datagram.size() );
      mBuffer->buffer().clear();
      mBuffer->seek( 0 );
      mBuffer->write( datagram );
      mBuffer->seek( 0 );
    }
  } );

  mReconnectTimer.setSingleShot( true );
  connect( &mReconnectTimer, &QTimer::timeout, this, [this]() {
    mSocket->connectToHost( mAddress, mPort, QUdpSocket::ReadOnly );
  } );

  setValid( !mAddress.isEmpty() && mPort > 0 );
  initNmeaConnection( mBuffer );
}

UdpReceiver::~UdpReceiver()
{
  mSocket->deleteLater();
  mSocket = nullptr;
  mBuffer->deleteLater();
  mBuffer = nullptr;
}

void UdpReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() || mPort == 0 )
  {
    return;
  }
  qDebug() << QStringLiteral( "UdpReceiver: Initiating connection to address %1 (port %2)" ).arg( mAddress, QString::number( mPort ) );
  mBuffer->open( QIODevice::ReadWrite );
  mSocket->bind( QHostAddress( "localhost" ), mPort );
  mSocket->joinMulticastGroup( QHostAddress( "localhost" ) );
}

void UdpReceiver::handleDisconnectDevice()
{
  mReconnectOnDisconnect = false;
  mBuffer->close();
  mSocket->close();
}

void UdpReceiver::setSocketState( const QAbstractSocket::SocketState socketState )
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
