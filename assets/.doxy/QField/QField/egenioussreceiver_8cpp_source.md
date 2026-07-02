

# File egenioussreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**egenioussreceiver.cpp**](egenioussreceiver_8cpp.md)

[Go to the documentation of this file](egenioussreceiver_8cpp.md)


```C++
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
  QNetworkRequest request( QString( "http://%1:%2/app/start" ).arg( mAddress.toString() ).arg( mPort ) );
  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( request );
  connect( reply, &QNetworkReply::finished, this, [this, reply]() {
    if ( reply->error() != QNetworkReply::NoError )
    {
      handleErrorMessage( QString( "HTTP request failed: %1" ).arg( reply->errorString() ) );
      return;
    }
    const QJsonObject jsonObject = QJsonDocument::fromJson( reply->readAll() ).object();
    const bool success = jsonObject.value( "success" ).toBool();

    if ( success )
    {
      mTcpSocket->connectToHost( mAddress, mPort + 1, QTcpSocket::ReadOnly );
    }
    else
    {
      handleErrorMessage( tr( "Failed to start egeniouss server." ) );
    }
    reply->deleteLater();
  } );
}

void EgenioussReceiver::handleDisconnectDevice()
{
  QNetworkRequest request( QString( "http://%1:%2/app/stop" ).arg( mAddress.toString() ).arg( mPort ) );
  QNetworkReply *reply = QgsNetworkAccessManager::instance()->get( request );
  connect( reply, &QNetworkReply::finished, this, [this, reply]() {
    if ( reply->error() != QNetworkReply::NoError )
    {
      handleErrorMessage( QString( "HTTP request failed: %1" ).arg( reply->errorString() ) );
      return;
    }
    const QJsonObject jsonObject = QJsonDocument::fromJson( reply->readAll() ).object();
    const bool success = jsonObject.value( "success" ).toBool();

    if ( success )
    {
      mTcpSocket->disconnectFromHost();
    }
    else
    {
      handleErrorMessage( tr( "Failed to stop egeniouss server." ) );
    }
    reply->deleteLater();
  } );
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
```


