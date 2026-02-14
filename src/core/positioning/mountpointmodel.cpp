/***************************************************************************
  mountpointmodel.cpp - MountpointModel

 ---------------------
 begin                : 14.02.2026
 copyright            : (C) 2026 by Pascal
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mountpointmodel.h"

#include <algorithm>
#include <cmath>
#include <numeric>

MountpointModel::MountpointModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QHash<int, QByteArray> MountpointModel::roleNames() const
{
  return {
    { NameRole, "name" },
    { FormatRole, "format" },
    { IdentifierRole, "identifier" },
    { CountryRole, "country" },
    { NetworkRole, "network" },
    { NavSystemRole, "navSystem" },
    { LatitudeRole, "latitude" },
    { LongitudeRole, "longitude" },
    { DistanceRole, "distance" },
    { IsNearestRole, "isNearest" },
  };
}

int MountpointModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;

  return static_cast<int>( mMountpoints.size() );
}

QVariant MountpointModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() < 0 || index.row() >= mMountpoints.size() )
    return QVariant();

  const SourcetableParser::MountpointInfo &mp = mMountpoints.at( index.row() );

  switch ( role )
  {
    case NameRole:
      return mp.name;
    case FormatRole:
      return mp.format;
    case IdentifierRole:
      return mp.identifier;
    case CountryRole:
      return mp.country;
    case NetworkRole:
      return mp.network;
    case NavSystemRole:
      return mp.navSystem;
    case LatitudeRole:
      return mp.latitude;
    case LongitudeRole:
      return mp.longitude;
    case DistanceRole:
      if ( mHasPosition && index.row() < mDistances.size() )
        return mDistances.at( index.row() );
      return -1.0;
    case IsNearestRole:
      return mHasPosition && index.row() == mNearestIndex;
  }

  return QVariant();
}

void MountpointModel::fetchMountpoints(
  const QString &host, int port,
  const QString &username, const QString &password,
  int version )
{
  cancelFetch();

  mHost = host;
  mPort = port;
  mUsername = username;
  mPassword = password;
  mVersion = version;

  mBuffer.clear();
  mHeadersParsed = false;
  mDistances.clear();
  mNearestIndex = -1;
  mHasPosition = false;
  setLastError( QString() );
  setFetchStatus( Fetching );

  connect( &mSocket, &QTcpSocket::connected, this, &MountpointModel::onSocketConnected );
  connect( &mSocket, &QTcpSocket::readyRead, this, &MountpointModel::onSocketReadyRead );
  connect( &mSocket, &QTcpSocket::disconnected, this, &MountpointModel::onSocketDisconnected );
  connect( &mSocket, QOverload<QAbstractSocket::SocketError>::of( &QTcpSocket::errorOccurred ),
           this, &MountpointModel::onSocketError );

  mSocket.connectToHost( mHost, mPort );
}

void MountpointModel::cancelFetch()
{
  if ( mSocket.state() != QAbstractSocket::UnconnectedState )
  {
    mSocket.abort();
  }
  cleanup();
}

void MountpointModel::onSocketConnected()
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

void MountpointModel::onSocketReadyRead()
{
  mBuffer.append( mSocket.readAll() );

  if ( !mHeadersParsed )
  {
    qsizetype headerEnd = mBuffer.indexOf( "\r\n\r\n" );
    if ( headerEnd == -1 )
      return;

    const QByteArray headerBlock = mBuffer.left( headerEnd );
    mBuffer = mBuffer.mid( headerEnd + 4 );
    mHeadersParsed = true;

    // Verify we got a SOURCETABLE response (status 200)
    if ( !headerBlock.startsWith( "SOURCETABLE" ) && !headerBlock.contains( "200" ) )
    {
      setLastError( tr( "Unexpected response from NTRIP caster" ) );
      setFetchStatus( Error );
      mSocket.abort();
      cleanup();
      return;
    }
  }
}

void MountpointModel::onSocketDisconnected()
{
  if ( mHeadersParsed )
  {
    QList<SourcetableParser::MountpointInfo> mountpoints = SourcetableParser::parse( mBuffer );
    beginResetModel();
    mMountpoints = mountpoints;
    endResetModel();

    if ( mMountpoints.isEmpty() )
    {
      setLastError( tr( "No mountpoints found in sourcetable" ) );
      setFetchStatus( Error );
    }
    else
    {
      setFetchStatus( Success );
    }
  }
  else if ( mFetchStatus == Fetching )
  {
    setLastError( tr( "Disconnected before receiving sourcetable" ) );
    setFetchStatus( Error );
  }

  cleanup();
}

void MountpointModel::onSocketError( QAbstractSocket::SocketError error )
{
  Q_UNUSED( error )
  setLastError( tr( "Connection error: %1" ).arg( mSocket.errorString() ) );
  setFetchStatus( Error );
  cleanup();
}

void MountpointModel::updatePosition( double latitude, double longitude )
{
  if ( mMountpoints.isEmpty() )
    return;

  const int count = static_cast<int>( mMountpoints.size() );
  mDistances.resize( count );

  for ( int i = 0; i < count; ++i )
  {
    if ( mMountpoints[i].latitude == 0.0 && mMountpoints[i].longitude == 0.0 )
    {
      mDistances[i] = -1.0;
    }
    else
    {
      mDistances[i] = haversineDistance( latitude, longitude,
                                         mMountpoints[i].latitude,
                                         mMountpoints[i].longitude );
    }
  }

  // Build index array and sort by distance (stable sort preserves original order for equal distances)
  QVector<int> indices( count );
  std::iota( indices.begin(), indices.end(), 0 );
  std::stable_sort( indices.begin(), indices.end(), [this]( int a, int b ) {
    if ( mDistances[a] < 0 )
      return false;
    if ( mDistances[b] < 0 )
      return true;
    return mDistances[a] < mDistances[b];
  } );

  // Reorder mountpoints and distances
  QList<SourcetableParser::MountpointInfo> sortedMountpoints;
  QVector<double> sortedDistances;
  sortedMountpoints.reserve( count );
  sortedDistances.reserve( count );
  for ( int idx : indices )
  {
    sortedMountpoints.append( mMountpoints[idx] );
    sortedDistances.append( mDistances[idx] );
  }

  beginResetModel();
  mMountpoints = sortedMountpoints;
  mDistances = sortedDistances;
  mNearestIndex = ( !sortedDistances.isEmpty() && sortedDistances[0] >= 0 ) ? 0 : -1;
  mHasPosition = true;
  endResetModel();
}

double MountpointModel::haversineDistance( double lat1, double lon1, double lat2, double lon2 )
{
  constexpr double earthRadiusKm = 6371.0;
  constexpr double degToRad = M_PI / 180.0;

  const double dLat = ( lat2 - lat1 ) * degToRad;
  const double dLon = ( lon2 - lon1 ) * degToRad;

  const double a = std::sin( dLat / 2.0 ) * std::sin( dLat / 2.0 )
                   + std::cos( lat1 * degToRad ) * std::cos( lat2 * degToRad )
                       * std::sin( dLon / 2.0 ) * std::sin( dLon / 2.0 );

  const double c = 2.0 * std::atan2( std::sqrt( a ), std::sqrt( 1.0 - a ) );

  return earthRadiusKm * c;
}

MountpointModel::FetchStatus MountpointModel::fetchStatus() const
{
  return mFetchStatus;
}

QString MountpointModel::lastError() const
{
  return mLastError;
}

void MountpointModel::setFetchStatus( FetchStatus status )
{
  if ( mFetchStatus != status )
  {
    mFetchStatus = status;
    emit fetchStatusChanged();
  }
}

void MountpointModel::setLastError( const QString &error )
{
  if ( mLastError != error )
  {
    mLastError = error;
    emit lastErrorChanged();
  }
}

void MountpointModel::cleanup()
{
  disconnect( &mSocket, &QTcpSocket::connected, this, &MountpointModel::onSocketConnected );
  disconnect( &mSocket, &QTcpSocket::readyRead, this, &MountpointModel::onSocketReadyRead );
  disconnect( &mSocket, &QTcpSocket::disconnected, this, &MountpointModel::onSocketDisconnected );
  disconnect( &mSocket, QOverload<QAbstractSocket::SocketError>::of( &QTcpSocket::errorOccurred ),
              this, &MountpointModel::onSocketError );
}
