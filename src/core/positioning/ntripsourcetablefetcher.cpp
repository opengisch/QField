/***************************************************************************
  ntripclient.cpp - NtripSourceTableFetcher

 ---------------------
 begin                : 05.02.2026
 copyright            : (C) 2026 by Vincent LAMBERT
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "ntripsourcetablefetcher.h"

NtripSourceTableFetcher::NtripSourceTableFetcher( QObject *parent )
  : QObject( parent )
{
}

void NtripSourceTableFetcher::fetch( const QString &host, int port, const QString &username, const QString &password, int version)
{
  cancel();

  mHost = host;
  mPort = static_cast<quint16>( port );
  mUsername = username;
  mPassword = password;
  mVersion = version;
  mBuffer.clear();
  mHeadersParsed = false;

  mSocket = new QTcpSocket( this );
  QObject::connect( mSocket, SIGNAL( connected() ), this, SLOT( onConnected() ) );
  QObject::connect( mSocket, SIGNAL( readyRead() ), this, SLOT( onReadyRead() ) );
  QObject::connect( mSocket, SIGNAL( disconnected() ), this, SLOT( onDisconnected() ) );
  QObject::connect( mSocket, SIGNAL( error(QAbstractSocket::SocketError) ), this, SLOT( onSocketError(QAbstractSocket::SocketError) ) );

  mFetching = true;
  emit fetchingChanged();

  mSocket->connectToHost( mHost, mPort );
}

void NtripSourceTableFetcher::cancel()
{
  if ( mSocket )
  {
    mSocket->abort();
    mSocket->deleteLater();
    mSocket = nullptr;
  }
  if ( mFetching )
  {
    mFetching = false;
    emit fetchingChanged();
  }
}

void NtripSourceTableFetcher::cleanup()
{
  if ( mSocket )
  {
    mSocket->deleteLater();
    mSocket = nullptr;
  }
  if ( mFetching )
  {
    mFetching = false;
    emit fetchingChanged();
  }
}

void NtripSourceTableFetcher::onSocketConnected()
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray request;
  if ( mVersion == 2 )
  {
    request.append( "GET / HTTP/1.1\r\n" );
    request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
    request.append( "Ntrip-Version: Ntrip/2.0\r\n" );
    request.append( "User-Agent: QField NTRIP QtSocketClient/2.0\r\n" );
    request.append( "Accept: */*\r\n" );
    request.append( "Authorization: Basic " + base64 + "\r\n" );
    request.append( "Connection: close\r\n" );
    request.append( "\r\n" );
  }
  else
  {
    request.append( "GET / HTTP/1.0\r\n" );
    request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
    request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
    request.append( "Accept: */*\r\n" );
    request.append( "Authorization: Basic " + base64 + "\r\n" );
    request.append( "Connection: close\r\n" );
    request.append( "\r\n" );
  }

  mSocket.write( request );
  mSocket.flush();
}

void NtripSourceTableFetcher::onSocketReadyRead()
{
  if ( !mSocket )
    return;

  mBuffer.append( mSocket->readAll() );

  if ( !mHeadersParsed )
  {
    const qsizetype headerEnd = mBuffer.indexOf( "\r\n\r\n" );
    if ( headerEnd == -1 )
      return;

    const QByteArray headerBlock = mBuffer.left( headerEnd );
    mBuffer = mBuffer.mid( headerEnd + 4 );
    mHeadersParsed = true;

    // Accept standard NTRIP sourcetable answers and HTTP 200 responses.
    if ( !headerBlock.startsWith( "SOURCETABLE" ) && !headerBlock.contains( "200" ) )
    {
      emit fetchError( tr( "Unexpected response from NTRIP caster" ) );
      if ( mSocket )
        mSocket->abort();
      cleanup();
      return;
    }
  }
}

void NtripSourceTableFetcher::onSocketDisconnected()
{
  if ( !mFetching )
    return;

  if ( !mHeadersParsed )
  {
    emit fetchError( tr( "Disconnected before receiving sourcetable" ) );
    cleanup();
    return;
  }

  const QStringList mountpoints = parseSourceTable( mBuffer );
  mMountpoints = mountpoints;
  emit mountpointsChanged();

  if ( mountpoints.isEmpty() )
    emit fetchError( tr( "No mountpoints found in NTRIP source table" ) );

  cleanup();
}

void NtripSourceTableFetcher::onSocketError( QAbstractSocket::SocketError error )
{
  if ( !mFetching )
    return;

  // Expected when server closes after sending sourcetable.
  if ( error == QAbstractSocket::RemoteHostClosedError )
    return;

  const QString message = mSocket ? mSocket->errorString() : QString::number( static_cast<int>( error ) );
  emit fetchError( tr( "NTRIP source table fetch error: " ) + message );
  cleanup();
}

QStringList NtripSourceTableFetcher::parseSourceTable( const QByteArray &data ) const
{
  QStringList mountpoints;

  // Skip HTTP headers: find the blank line separator
  const int headerEnd = data.indexOf( "\r\n\r\n" );
  const QByteArray body = ( headerEnd >= 0 ) ? data.mid( headerEnd + 4 ) : data;

  const QList<QByteArray> lines = body.split( '\n' );
  for ( const QByteArray &line : lines )
  {
    const QByteArray trimmed = line.trimmed();
    if ( !trimmed.startsWith( "STR;" ) )
      continue;

    const QList<QByteArray> fields = trimmed.split( ';' );
    if ( fields.size() >= 2 )
    {
      const QString mountpoint = QString::fromUtf8( fields.at( 1 ) ).trimmed();
      if ( !mountpoint.isEmpty() )
        mountpoints.append( mountpoint );
    }
  }

  return mountpoints;
}
