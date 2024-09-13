#include "egenioussreceiver.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

EgenioussReceiver::EgenioussReceiver( const QString &address, const int port, QObject *parent )
  : AbstractGnssReceiver( parent ), mAddress( address ), mPort( port ), mTcpSocket( new QTcpSocket( this ) )
{
}

void EgenioussReceiver::handleConnectDevice()
{
  connect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  connect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::onErrorOccurred );

  mTcpSocket->connectToHost( QHostAddress::LocalHost, mPort );

  if ( !mTcpSocket->waitForConnected( 3000 ) )
  {
    setValid( false );
    mLastError = mTcpSocket->errorString();
    emit lastErrorChanged( mLastError );
  }
  else
  {
    setValid( true );
    mSocketState = QAbstractSocket::ConnectedState;
    emit socketStateChanged( mSocketState );
  }
}

void EgenioussReceiver::handleDisconnectDevice()
{
  disconnect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  disconnect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::onErrorOccurred );

  if ( mTcpSocket->state() == QAbstractSocket::ConnectedState )
  {
    mTcpSocket->disconnectFromHost();
    mTcpSocket->waitForDisconnected();
  }
}

QList<QPair<QString, QVariant>> EgenioussReceiver::details()
{
  QList<QPair<QString, QVariant>> dataList;

  QJsonDocument jsonDoc = QJsonDocument::fromJson( payload );
  QJsonObject jsonObject = jsonDoc.object();

  if ( jsonDoc.isNull() || !jsonDoc.isObject() )
  {
    return dataList; // Failed to parse JSON
  }

  dataList.append( qMakePair( "q", jsonObject.value( "q" ).toString() ) );

  return dataList;
}

void EgenioussReceiver::onReadyRead()
{
  if ( valid() )
  {
    mReceivedData = mTcpSocket->readAll();
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

    payload = mReceivedData.mid( 8, payloadLength );

    QJsonDocument jsonDoc = QJsonDocument::fromJson( payload );

    if ( jsonDoc.isNull() || !jsonDoc.isObject() )
    {
      return; // Failed to parse JSON
    }

    QJsonObject jsonObject = jsonDoc.object();

    mLastGnssPositionInformation = GnssPositionInformation(
      jsonObject.value( "lat" ).toDouble(),
      jsonObject.value( "lon" ).toDouble(),
      jsonObject.value( "alt" ).toDouble(),
      std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::quiet_NaN(),
      QList<QgsSatelliteInfo>(),
      0,
      0,
      0,
      std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::quiet_NaN(),
      QDateTime::fromMSecsSinceEpoch( jsonObject.value( "utc" ).toDouble() / 1e6, Qt::UTC ),
      QChar(),
      0,
      1 );

    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }
}

void EgenioussReceiver::onErrorOccurred( QAbstractSocket::SocketError socketError )
{
  mLastError = mTcpSocket->errorString();
  emit lastErrorChanged( mLastError );
}
