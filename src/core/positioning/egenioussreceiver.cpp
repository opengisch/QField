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

QLatin1String EgenioussReceiver::identifier = QLatin1String( "egeniouss" );

EgenioussReceiver::EgenioussReceiver( QObject *parent )
  : AbstractGnssReceiver( parent ), mTcpSocket( new QTcpSocket() ), mNetworkManager( new QNetworkAccessManager( this ) )
{
  connect( mNetworkManager, &QNetworkAccessManager::finished, this, &EgenioussReceiver::onHttpFinished );
  connect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  connect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::handleError );
  connect( mTcpSocket, &QTcpSocket::stateChanged, this, &AbstractGnssReceiver::setSocketState );

  setValid( true );
}

EgenioussReceiver::~EgenioussReceiver()
{
  mTcpSocket->deleteLater();
  mNetworkManager->deleteLater();
  mTcpSocket = nullptr;
  mNetworkManager = nullptr;
}

void EgenioussReceiver::handleConnectDevice()
{
  QNetworkRequest request( QString( "http://%1:%2/app/start" ).arg( mAddress.toString() ).arg( mPort ) );
  mNetworkManager->get( request );
}

void EgenioussReceiver::handleDisconnectDevice()
{
  QNetworkRequest request( QString( "http://%1:%2/app/stop" ).arg( mAddress.toString() ).arg( mPort ) );
  mNetworkManager->get( request );
}

void EgenioussReceiver::onHttpFinished( QNetworkReply *reply )
{
  if ( reply->error() != QNetworkReply::NoError )
  {
    handleHttpError( QString( "HTTP request failed: %1" ).arg( reply->errorString() ) );
    return;
  }

  const QByteArray responseData = reply->readAll();
  const QJsonDocument jsonDoc = QJsonDocument::fromJson( responseData );

  if ( !jsonDoc.isObject() )
  {
    handleHttpError( "Failed to parse HTTP response." );
    return;
  }

  const QJsonObject jsonObject = jsonDoc.object();
  const QString message = jsonObject.value( "message" ).toString();
  const bool success = jsonObject.value( "success" ).toBool();

  if ( message.contains( "started egeniouss" ) )
  {
    if ( success )
    {
      mTcpSocket->connectToHost( mAddress, mPort + 1, QTcpSocket::ReadWrite );
    }
    else
    {
      handleHttpError( tr( "Failed to start egeniouss server." ) );
    }
  }
  else if ( message.contains( "stopped egeniouss" ) )
  {
    if ( success )
    {
      mTcpSocket->disconnectFromHost();
    }
    else
    {
      handleHttpError( tr( "Failed to stop egeniouss server." ) );
    }
  }

  reply->deleteLater();
}

void EgenioussReceiver::handleHttpError( const QString &errorMessage )
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

  detailsList.append( "q", mPayload.value( "q" ).toDouble() );

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
    mLastError = tr( "Received data is too short to process" );
    emit lastErrorChanged( mLastError );
    return;
  }
  if ( static_cast<uint8_t>( mReceivedData[0] ) != validStartByte )
  {
    mLastError = tr( "Invalid start byte" );
    emit lastErrorChanged( mLastError );
    return;
  }
  uint32_t payloadLength;
  QDataStream dataStream( mReceivedData.mid( 4, 4 ) );
  dataStream.setByteOrder( QDataStream::LittleEndian );
  dataStream >> payloadLength;
  if ( mReceivedData.size() < payloadHeaderSize + payloadLength )
  {
    mLastError = tr( "Received data is too short to contain the payload" );
    emit lastErrorChanged( mLastError );
    return;
  }
  QJsonDocument jsonDoc = QJsonDocument::fromJson( mReceivedData.mid( payloadHeaderSize, payloadLength ) );
  if ( jsonDoc.isNull() || !jsonDoc.isObject() )
  {
    mLastError = tr( "Failed to parse JSON" );
    emit lastErrorChanged( mLastError );
    return;
  }
  mPayload = jsonDoc.object();
  const double latitude = mPayload.value( "lat" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "lat" ).toDouble();
  const double longitude = mPayload.value( "lon" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "lon" ).toDouble();
  const double elevation = mPayload.value( "alt" ).toDouble() == 0 ? std::numeric_limits<double>::quiet_NaN() : mPayload.value( "alt" ).toDouble();
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
    std::numeric_limits<double>::quiet_NaN(),
    std::numeric_limits<double>::quiet_NaN(),
    QDateTime::fromMSecsSinceEpoch( mPayload.value( "time" ).toDouble() / 1e6, QTimeZone( QTimeZone::Initialization::UTC ) ),
    QChar(),
    0,
    1 );

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
