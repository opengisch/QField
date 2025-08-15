#include "ntripsocketclient.h"

#include <QDebug>

NtripSocketClient::NtripSocketClient( QObject *parent )
  : QObject( parent )
{
  connect( &mSocket, &QTcpSocket::connected, this, &NtripSocketClient::onConnected );
  connect( &mSocket, &QTcpSocket::readyRead, this, &NtripSocketClient::onReadyRead );
  connect( &mSocket, &QTcpSocket::disconnected, this, &NtripSocketClient::onDisconnected );
  connect( &mSocket, QOverload<QAbstractSocket::SocketError>::of( &QTcpSocket::errorOccurred ),
           this, &NtripSocketClient::onSocketError );
}

NtripSocketClient::~NtripSocketClient()
{
  stop();
}

qint64 NtripSocketClient::start(
  const QString &host,
  quint16 port,
  const QString &mountpoint,
  const QString &username,
  const QString &password )
{
  mHeadersSent = false;
  mSocket.connectToHost( host, port );

  QString credentials = username + ":" + password;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray request;
  request.append( "GET " + mountpoint.toUtf8() + " HTTP/1.0\r\n" );
  request.append( "Host: " + host.toUtf8() + ":" + QByteArray::number( port ) + "\r\n" );
  request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
  request.append( "Accept: */*\r\n" );
  request.append( "Authorization: Basic " + base64 + "\r\n" );
  request.append( "Connection: close\r\n" );
  //request.append("Ntrip-Version: Ntrip/2.0\r\n");
  request.append( "\r\n" );

  connect( &mSocket, &QTcpSocket::connected, [this, request]() {
    mSocket.write( request );
    mSocket.flush();
  } );

  return request.size();
}

void NtripSocketClient::stop()
{
  if ( mSocket.isOpen() )
  {
    mSocket.disconnectFromHost();
    mSocket.close();
  }
}

void NtripSocketClient::onConnected()
{
  qDebug() << "Connected to NTRIP caster.";
}

void NtripSocketClient::onReadyRead()
{
  QByteArray data = mSocket.readAll();

  // If headers not processed yet, discard them
  if ( !mHeadersSent )
  {
    int headerEnd = data.indexOf( "\r\n\r\n" );
    if ( headerEnd != -1 )
    {
      QByteArray headerData = data.left( headerEnd );
      qDebug() << "Received HTTP headers:\n"
               << headerData;
      data = data.mid( headerEnd + 4 );
      mHeadersSent = true;
      emit streamConnected();
    }
    else
    {
      // Wait for more data
      return;
    }
  }

  if ( !data.isEmpty() )
  {
    emit correctionDataReceived( data );
  }
}

void NtripSocketClient::onDisconnected()
{
  emit errorOccurred( "Disconnected from NTRIP caster." );
}

void NtripSocketClient::onSocketError( QAbstractSocket::SocketError error )
{
  emit errorOccurred( "Socket error: " + QString::number( error ) + " (" + mSocket.errorString() + ")" );
}
