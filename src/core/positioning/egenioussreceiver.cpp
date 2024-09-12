#include "egenioussreceiver.h"

#include <QHostAddress>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

EgenioussReceiver::EgenioussReceiver( QObject *parent )
  : AbstractGnssReceiver( parent ), mTcpSocket( new QTcpSocket( this ) )
{
  connect( mTcpSocket, &QTcpSocket::readyRead, this, &EgenioussReceiver::onReadyRead );
  connect( mTcpSocket, &QTcpSocket::errorOccurred, this, &EgenioussReceiver::onErrorOccurred );
}

void EgenioussReceiver::handleConnectDevice()
{
  mTcpSocket->connectToHost( QHostAddress::LocalHost, 1235 );

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
  if ( mTcpSocket->state() == QAbstractSocket::ConnectedState )
  {
    mTcpSocket->disconnectFromHost();
    mTcpSocket->waitForDisconnected();
  }
}

QList<QPair<QString, QVariant>> EgenioussReceiver::details()
{
  QList<QPair<QString, QVariant>> dataList;
  dataList.append( qMakePair( QString( "startByte" ), QString::number( startByte ) ) );
  dataList.append( qMakePair( QString( "protocolVersion" ), QString::number( protocolVersion ) ) );
  dataList.append( qMakePair( QString( "counter" ), QString::number( counter ) ) );
  dataList.append( qMakePair( QString( "messageId" ), QString::number( messageId ) ) );
  dataList.append( qMakePair( QString( "N" ), QString::number( N ) ) );

  QJsonDocument jsonDoc = QJsonDocument::fromJson( payload );
  if ( jsonDoc.isNull() || !jsonDoc.isObject() )
  {
    qWarning() << "Failed to parse JSON:";
    return dataList;
  }

  QJsonObject jsonObject = jsonDoc.object();

  for ( auto it = jsonObject.begin(); it != jsonObject.end(); ++it )
  {
    QString key = it.key();
    QVariant value = it.value().toVariant();
    dataList.append( qMakePair( key, value ) );
  }
  return dataList;
}

void EgenioussReceiver::processReceivedData()
{
  if ( mReceivedData.size() < 9 )
  {
    qDebug() << "Received data is too short to process.";
    return;
  }

  startByte = static_cast<uint8_t>( mReceivedData[0] );
  if ( startByte != 0xFE )
  {
    qDebug() << "Invalid start byte:" << startByte;
    return;
  }

  protocolVersion = static_cast<uint8_t>( mReceivedData[1] );
  counter = static_cast<uint8_t>( mReceivedData[2] );
  messageId = static_cast<uint8_t>( mReceivedData[3] );

  QDataStream dataStream( mReceivedData.mid( 4, 4 ) );
  dataStream.setByteOrder( QDataStream::LittleEndian ); // Make sure the byte order matches protocol
  dataStream >> N;

  if ( mReceivedData.size() < 8 + N )
  {
    qDebug() << "Received data is too short to contain the payload.";
    return;
  }

  payload = mReceivedData.mid( 8, N );
}
void EgenioussReceiver::onReadyRead()
{
  mReceivedData = mTcpSocket->readAll();

  processReceivedData();

  if ( valid() )
  {
    emit detailsChanged();
  }
}

void EgenioussReceiver::onErrorOccurred( QAbstractSocket::SocketError socketError )
{
  mLastError = mTcpSocket->errorString();
  emit lastErrorChanged( mLastError );
}
