/***************************************************************************
 egeniousseeceiver.cpp - EgenioussReceiver

 ---------------------
 begin                : October 2024
 copyright            : (C) 2024 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "egenioussreceiver.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonValue>
#include <QNetworkReply>
#include <QTimeZone>
#include <qgsnetworkaccessmanager.h>

#include <cmath>

QLatin1String EgenioussReceiver::identifier = QLatin1String( "egeniouss" );

EgenioussReceiver::EgenioussReceiver( const QString &address, const int port, QObject *parent )
  : AbstractGnssReceiver( parent ), mTcpSocket( new QTcpSocket() ), mAddress( address ), mPort( port )
{
  connect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  connect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::handleError );
  connect( mTcpSocket, &QTcpSocket::stateChanged, this, &AbstractGnssReceiver::setSocketState );

  setValid( true );
}

EgenioussReceiver::~EgenioussReceiver()
{
  mTcpSocket->deleteLater();
  mTcpSocket = nullptr;
}

void EgenioussReceiver::handleConnectDevice()
{
  QNetworkRequest request( QString( "http://%1:%2/app/getStatus" ).arg( mAddress.toString() ).arg( mPort ) );
  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( request );
  connect( reply, &QNetworkReply::finished, this, [this, reply]() {
    if ( reply->error() != QNetworkReply::NoError )
    {
      const QString errorMessage = tr( "Is the app running? HTTP request failed: %1" ).arg( reply->errorString() );
      qInfo() << QStringLiteral( "EgenioussReceiver: %1" ).arg( errorMessage );
      handleErrorMessage( errorMessage );
      reply->deleteLater();
      return;
    }
    const QJsonObject jsonObject = QJsonDocument::fromJson( reply->readAll() ).object();
    const bool isRunning = jsonObject.value( "estimation_status" ).toBool();

    if ( !isRunning )
    {
      const QString errorMessage = tr( "Egeniouss service is not ready." );
      qInfo() << QStringLiteral( "EgenioussReceiver: %1" ).arg( errorMessage );
      handleErrorMessage( errorMessage );
      reply->deleteLater();
    }

    mTcpSocket->connectToHost( mAddress, mPort + 1, QTcpSocket::ReadOnly );
    reply->deleteLater();
  } );
}

void EgenioussReceiver::handleDisconnectDevice()
{
  mTcpSocket->disconnectFromHost();
}

void EgenioussReceiver::handleErrorMessage( const QString &errorMessage )
{
  mLastError = errorMessage;
  emit lastErrorChanged( mLastError );
}

GnssPositionDetails EgenioussReceiver::details() const
{
  GnssPositionDetails detailsList;

  if ( mPayload.isEmpty() )
  {
    return detailsList;
  }

  const qint64 timeMs = static_cast<qint64>( mPayload.value( "time" ).toDouble() * 1000.0 );
  const QDateTime timestamp = QDateTime::fromMSecsSinceEpoch( timeMs, QTimeZone( QTimeZone::Initialization::UTC ) );
  detailsList.append( tr( "Last fix" ), timestamp.toLocalTime().toString( QStringLiteral( "hh:mm:ss.zzz" ) ) );

  const double heading = mPayload.value( "kappa" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  if ( !std::isnan( heading ) )
  {
    detailsList.append( tr( "Heading" ), QStringLiteral( "%1°" ).arg( heading, 0, 'f', 1 ) );
  }

  const double roll = mPayload.value( "omega" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double pitch = mPayload.value( "phi" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  if ( !std::isnan( roll ) && !std::isnan( pitch ) )
  {
    detailsList.append( tr( "Roll" ), QStringLiteral( "%1°" ).arg( roll, 0, 'f', 1 ) );
    detailsList.append( tr( "Pitch" ), QStringLiteral( "%1°" ).arg( pitch, 0, 'f', 1 ) );
  }

  const double sigLat = mPayload.value( "sig_lat" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double sigLon = mPayload.value( "sig_lon" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double sigAlt = mPayload.value( "sig_alt" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double sigKappa = mPayload.value( "sig_kappa" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  if ( !std::isnan( sigLat ) && !std::isnan( sigLon ) )
  {
    const double hacc = std::sqrt( sigLat * sigLat + sigLon * sigLon );
    detailsList.append( tr( "H. sigma" ), QStringLiteral( "%1m" ).arg( hacc, 0, 'f', 3 ) );
  }
  if ( !std::isnan( sigAlt ) )
  {
    detailsList.append( tr( "V. sigma" ), QStringLiteral( "%1m" ).arg( sigAlt, 0, 'f', 3 ) );
  }
  if ( !std::isnan( sigKappa ) )
  {
    detailsList.append( tr( "Heading sigma" ), QStringLiteral( "%1°" ).arg( sigKappa, 0, 'f', 2 ) );
  }

  return detailsList;
}

void EgenioussReceiver::onReadyRead()
{
  const int minimumDataSize = 9;
  const uint8_t validStartByte = 0xFE;
  const int payloadHeaderSize = 12;

  QByteArray mReceivedData = mTcpSocket->readAll();

  if ( mReceivedData.size() < minimumDataSize )
  {
    handleErrorMessage( tr( "Received data is too short to process" ) );
    return;
  }
  if ( static_cast<uint8_t>( mReceivedData[0] ) != validStartByte )
  {
    handleErrorMessage( tr( "Invalid start byte" ) );
    return;
  }
  uint32_t payloadLength;
  QDataStream dataStream( mReceivedData.mid( 4, 4 ) );
  dataStream.setByteOrder( QDataStream::LittleEndian );
  dataStream >> payloadLength;
  if ( mReceivedData.size() < payloadHeaderSize + payloadLength )
  {
    handleErrorMessage( tr( "Received data is too short to contain the payload" ) );
    return;
  }
  QJsonDocument jsonDoc = QJsonDocument::fromJson( mReceivedData.mid( payloadHeaderSize, payloadLength ) );
  if ( jsonDoc.isNull() || !jsonDoc.isObject() )
  {
    handleErrorMessage( tr( "Failed to parse JSON" ) );
    return;
  }
  mPayload = jsonDoc.object();
  const double latitude = mPayload.value( "lat" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "lat" ).toDouble();
  const double longitude = mPayload.value( "lon" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "lon" ).toDouble();
  const double elevation = mPayload.value( "alt" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "alt" ).toDouble();
  // omega=roll, phi=pitch, kappa=yaw/heading — Egeniouss naming convention
  const double roll = mPayload.value( "omega" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double pitch = mPayload.value( "phi" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double heading = mPayload.value( "kappa" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  // 1-sigma position accuracy in metres
  const double sigLat = mPayload.value( "sig_lat" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double sigLon = mPayload.value( "sig_lon" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  const double sigAlt = mPayload.value( "sig_alt" ).toDouble( std::numeric_limits<double>::quiet_NaN() );
  // horizontal accuracy: RSS of sig_lat and sig_lon
  const double hacc = ( std::isnan( sigLat ) || std::isnan( sigLon ) ) ? std::numeric_limits<double>::quiet_NaN() : std::sqrt( sigLat * sigLat + sigLon * sigLon );
  mLastGnssPositionInformation = GnssPositionInformation(
    latitude,
    longitude,
    elevation,
    std::numeric_limits<double>::quiet_NaN(),
    std::numeric_limits<double>::quiet_NaN(),
    QList<QgsSatelliteInfo>(),
    0,
    0,
    0,
    hacc,
    sigAlt,
    QDateTime::fromMSecsSinceEpoch( static_cast<qint64>( mPayload.value( "time" ).toDouble() * 1000.0 ), QTimeZone( QTimeZone::Initialization::UTC ) ),
    QChar(),
    0,
    1,
    0,
    QChar(),
    QList<int>(),
    false,
    std::numeric_limits<double>::quiet_NaN(),
    std::numeric_limits<double>::quiet_NaN(),
    0,
    QString(),
    true,
    roll,
    pitch,
    heading );

  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
}

void EgenioussReceiver::handleError( QAbstractSocket::SocketError error )
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
  qInfo() << QStringLiteral( "EgenioussReceiver: Error: %1" ).arg( mLastError );

  emit lastErrorChanged( mLastError );
}
