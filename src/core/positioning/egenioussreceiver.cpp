#include "egenioussreceiver.h"

#include <QHostAddress>

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
  dataList.append( qMakePair( QString( "Payload" ), QVariant::fromValue( payload.toHex() ) ) );
  return dataList;
}

void EgenioussReceiver::processReceivedData()
{
  // Check if the received data is long enough
  if ( mReceivedData.size() < 9 )
  {
    qDebug() << "Received data is too short to process.";
    return;
  }

  // Read the start byte (uint8)
  startByte = static_cast<uint8_t>( mReceivedData[0] );
  if ( startByte != 0xFE )
  {
    qDebug() << "Invalid start byte:" << startByte;
    return;
  }

  // Read the next bytes (uint8s)
  protocolVersion = static_cast<uint8_t>( mReceivedData[1] );
  counter = static_cast<uint8_t>( mReceivedData[2] );
  messageId = static_cast<uint8_t>( mReceivedData[3] );

  // Read the N value (uint32)
  N = 0;
  if ( mReceivedData.size() < 9 + sizeof( uint32_t ) )
  {
    qDebug() << "Received data is too short to read N.";
    return;
  }

  // Convert bytes 4 to 8 into a uint32
  QDataStream dataStream( mReceivedData.mid( 4, 4 ) );
  dataStream.setByteOrder( QDataStream::LittleEndian ); // Make sure the byte order matches protocol
  dataStream >> N;

  // Check if we have enough payload data
  if ( mReceivedData.size() < 9 + N )
  {
    qDebug() << "Received data is too short to contain the payload.";
    return;
  }

  // Extract the payload
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
