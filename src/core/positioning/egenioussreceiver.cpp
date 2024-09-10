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
  dataList.append( qMakePair( QString( "Last Received Data" ), mReceivedData ) );
  return dataList;
}

void EgenioussReceiver::onReadyRead()
{
  mReceivedData = mTcpSocket->readAll();
  if ( valid() )
  {
    qDebug() << "Received Data:" << mReceivedData;
  }
}

void EgenioussReceiver::onErrorOccurred( QAbstractSocket::SocketError socketError )
{
  mLastError = mTcpSocket->errorString();
  emit lastErrorChanged( mLastError );
  qDebug() << "Socket Error:" << mLastError;
}
