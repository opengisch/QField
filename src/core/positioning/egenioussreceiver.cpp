#include "egenioussreceiver.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonValue>

EgenioussReceiver::EgenioussReceiver( QObject *parent )
  : AbstractGnssReceiver( parent ), mTcpSocket( new QTcpSocket( this ) )
{
  connect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  connect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::onErrorOccurred );
  connect( mTcpSocket, &QTcpSocket::connected, this, &EgenioussReceiver::connected );
  connect( mTcpSocket, &QTcpSocket::disconnected, this, &EgenioussReceiver::disconnected );
}

void EgenioussReceiver::handleConnectDevice()
{
  mTcpSocket->connectToHost( mAddress, mPort, QTcpSocket::ReadWrite );
}

void EgenioussReceiver::connected()
{
  mSocketState = QAbstractSocket::ConnectedState;
  emit socketStateChanged( mSocketState );
}

void EgenioussReceiver::handleDisconnectDevice()
{
  mTcpSocket->disconnectFromHost();
}

void EgenioussReceiver::disconnected()
{
  if ( mTcpSocket->state() == QAbstractSocket::ConnectedState )
  {
    mSocketState = QAbstractSocket::UnconnectedState;
    emit socketStateChanged( mSocketState );
  }
}

QList<QPair<QString, QVariant>> EgenioussReceiver::details()
{
  QList<QPair<QString, QVariant>> detailsList;

  if ( mPayload.isEmpty() )
  {
    return detailsList;
  }

  detailsList.append( qMakePair( "q", mPayload.value( "q" ).toString() ) );

  return detailsList;
}

void EgenioussReceiver::onReadyRead()
{
  QByteArray mReceivedData = mTcpSocket->readAll();
  if ( mReceivedData.size() < 9 )
  {
    return; // Received data is too short to process.
  }
  if ( static_cast<uint8_t>( mReceivedData[0] ) != 0xFE )
  {
    return; // Invalid start byte
  }
  uint32_t payloadLength;
  QDataStream dataStream( mReceivedData.mid( 4, 4 ) );
  dataStream.setByteOrder( QDataStream::LittleEndian );
  dataStream >> payloadLength;
  if ( mReceivedData.size() < 8 + payloadLength )
  {
    return; // Received data is too short to contain the payload.
  }
  QJsonDocument jsonDoc = QJsonDocument::fromJson( mReceivedData.mid( 8, payloadLength ) );
  if ( jsonDoc.isNull() || !jsonDoc.isObject() )
  {
    return; // Failed to parse JSON
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
    QDateTime::fromMSecsSinceEpoch( mPayload.value( "utc" ).toDouble() / 1e6, Qt::UTC ),
    QChar(),
    0,
    1 );

  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
}

void EgenioussReceiver::onErrorOccurred( QAbstractSocket::SocketError socketError )
{
  mLastError = mTcpSocket->errorString();
  emit lastErrorChanged( mLastError );
}
