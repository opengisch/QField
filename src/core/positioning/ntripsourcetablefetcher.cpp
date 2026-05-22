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

#include <qgspoint.h>

NtripSourceTableFetcher::NtripSourceTableFetcher( QObject *parent )
  : QObject( parent )
{
}

void NtripSourceTableFetcher::fetch( const NtripSettings &ntripSettings )
{
  cancel();

  mHost = ntripSettings.host();
  mPort = static_cast<quint16>( ntripSettings.port() );
  mUsername = ntripSettings.username();
  mPassword = ntripSettings.password();
  mProtocol = ntripSettings.protocol();

  mBuffer.clear();
  mHeadersParsed = false;

  mSocket = new QSslSocket( this );
  connect( mSocket, &QAbstractSocket::connected, this, &NtripSourceTableFetcher::onSocketConnected );
  connect( mSocket, &QAbstractSocket::readyRead, this, &NtripSourceTableFetcher::onSocketReadyRead );
  connect( mSocket, &QAbstractSocket::disconnected, this, &NtripSourceTableFetcher::onSocketDisconnected );
  connect( mSocket, &QAbstractSocket::errorOccurred, this, &NtripSourceTableFetcher::onSocketError );

  mIsFetching = true;
  emit isFetchingChanged();

  switch ( mProtocol )
  {
    case NtripSettings::NtripSsl:
    {
      mSocket->connectToHostEncrypted( mHost, mPort );
      break;
    }

    case NtripSettings::NtripVersion2:
    case NtripSettings::NtripVersion1:
    {
      mSocket->connectToHost( mHost, mPort );
      break;
    }
  }
}

void NtripSourceTableFetcher::cancel()
{
  if ( mSocket )
  {
    mSocket->abort();
  }
  cleanup();
}

void NtripSourceTableFetcher::cleanup()
{
  if ( mSocket )
  {
    mSocket->deleteLater();
    mSocket = nullptr;
  }
  if ( mIsFetching )
  {
    mIsFetching = false;
    emit isFetchingChanged();
  }
}

void NtripSourceTableFetcher::onSocketConnected()
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray request;
  switch ( mProtocol )
  {
    case NtripSettings::NtripSsl:
    case NtripSettings::NtripVersion2:
    {
      request.append( "GET / HTTP/1.1\r\n" );
      request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
      request.append( "Ntrip-Version: Ntrip/2.0\r\n" );
      request.append( "User-Agent: QField NTRIP Client/2.0\r\n" );
      request.append( "Accept: */*\r\n" );
      request.append( "Authorization: Basic " + base64 + "\r\n" );
      request.append( "Connection: close\r\n" );
      request.append( "\r\n" );
      break;
    }
    case NtripSettings::NtripVersion1:
    {
      request.append( "GET / HTTP/1.0\r\n" );
      request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
      request.append( "User-Agent: QField NTRIP Client/1.0\r\n" );
      request.append( "Accept: */*\r\n" );
      request.append( "Authorization: Basic " + base64 + "\r\n" );
      request.append( "Connection: close\r\n" );
      request.append( "\r\n" );
      break;
    }
  }

  mSocket->write( request );
  mSocket->flush();
}

void NtripSourceTableFetcher::onSocketReadyRead()
{
  if ( !mSocket )
  {
    return;
  }

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
      {
        mSocket->abort();
      }
      cleanup();
      return;
    }
  }
}

void NtripSourceTableFetcher::onSocketDisconnected()
{
  if ( !mIsFetching )
  {
    return;
  }

  if ( !mHeadersParsed )
  {
    emit fetchError( tr( "Disconnected before receiving source table" ) );
    cleanup();
    return;
  }

  const QList<NtripMountPoint> mountPoints = parseSourceTable( mBuffer );

  if ( !mountPoints.isEmpty() )
  {
    if ( mMountPoints != mountPoints )
    {
      mMountPoints = mountPoints;
      emit mountPointsChanged();
    }
  }
  else
  {
    emit fetchError( tr( "No mountpoints found in NTRIP source table" ) );
  }

  cleanup();
}

void NtripSourceTableFetcher::onSocketError( QAbstractSocket::SocketError error )
{
  if ( !mIsFetching )
  {
    return;
  }

  // Expected when server closes after sending sourcetable.
  if ( error == QAbstractSocket::RemoteHostClosedError )
  {
    return;
  }

  const QString message = mSocket ? mSocket->errorString() : QString::number( static_cast<int>( error ) );
  emit fetchError( tr( "NTRIP source table fetch error: " ) + message );
  cleanup();
}

QList<NtripMountPoint> NtripSourceTableFetcher::parseSourceTable( const QByteArray &data ) const
{
  QList<NtripMountPoint> mountPoints;

  const int strBegin = data.indexOf( "STR;" );
  if ( strBegin >= 0 )
  {
    const QByteArray body = data.mid( strBegin );
    const QList<QByteArray> lines = body.split( '\n' );
    for ( const QByteArray &line : lines )
    {
      const QByteArray trimmed = line.trimmed();
      if ( !trimmed.startsWith( "STR;" ) )
        continue;

      const QList<QByteArray> fields = trimmed.split( ';' );
      if ( fields.size() >= 11 )
      {
        // STR format details
        // https://software.rtcm-ntrip.org/wiki/STR
        const QString mountpoint = QString::fromUtf8( fields.at( 1 ) ).trimmed();
        const QString identifier = QString::fromUtf8( fields.at( 2 ) ).trimmed();
        const QString format = QString::fromUtf8( fields.at( 3 ) ).trimmed();
        const QgsPoint point = QgsPoint( QString::fromUtf8( fields.at( 10 ) ).toDouble(), QString::fromUtf8( fields.at( 9 ) ).toDouble() );
        if ( !mountpoint.isEmpty() )
        {
          mountPoints.append( NtripMountPoint( mountpoint, identifier, format, point ) );
        }
      }
    }
  }

  return mountPoints;
}
