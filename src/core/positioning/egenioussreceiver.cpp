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
  qDebug() << __FUNCTION__;
  mTcpSocket->connectToHost( QHostAddress::LocalHost, 1235 );

  if ( !mTcpSocket->waitForConnected( 3000 ) )
  {
    qDebug() << __FUNCTION__ << __LINE__;
    setValid( false );
    mLastError = mTcpSocket->errorString();
    emit lastErrorChanged( mLastError );
  }
  else
  {
    qDebug() << __FUNCTION__ << __LINE__;
    setValid( true );
    mSocketState = QAbstractSocket::ConnectedState;
    emit socketStateChanged( mSocketState );
  }
}

void EgenioussReceiver::handleDisconnectDevice()
{
  qDebug() << __FUNCTION__;
  if ( mTcpSocket->state() == QAbstractSocket::ConnectedState )
  {
    mTcpSocket->disconnectFromHost();
    mTcpSocket->waitForDisconnected();
  }
}

QList<QPair<QString, QVariant>> EgenioussReceiver::details()
{
  qDebug() << __FUNCTION__;
  QList<QPair<QString, QVariant>> dataList;
  dataList.append( qMakePair( QString( "Last Received Data" ), mReceivedData ) );
  return dataList;
}

void EgenioussReceiver::onReadyRead()
{
  qDebug() << __FUNCTION__;
  mReceivedData = mTcpSocket->readAll();
  if ( valid() )
  {
    qDebug() << __FUNCTION__ << "Received Data:" << mReceivedData;
  }
}

void EgenioussReceiver::onErrorOccurred( QAbstractSocket::SocketError socketError )
{
  mLastError = mTcpSocket->errorString();
  emit lastErrorChanged( mLastError );
  qDebug() << __FUNCTION__ << "Socket Error:" << mLastError;
}
