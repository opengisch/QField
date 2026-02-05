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
  mHost = host;
  mPort = port;
  mMountpoint = mountpoint;
  mUsername = username;
  mPassword = password;

  mHeadersSent = false;

  if ( mSocket.isOpen() )
    stop();

  mSocket.connectToHost( mHost, mPort );

  return estimateRequestSize();
}

qint64 NtripSocketClient::estimateRequestSize() const
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  return 200 + base64.size() + mMountpoint.size();
}

void NtripSocketClient::stop()
{
  if ( mSocket.isOpen() )
  {
    mSocket.abort();
  }
  mHeadersSent = false;
}

void NtripSocketClient::onConnected()
{
  qDebug() << "Connected to NTRIP caster:" << mHost << mPort << "mount:" << mMountpoint;

  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray mp = mMountpoint.toUtf8();
  if ( !mp.startsWith( '/' ) )
    mp.prepend( '/' );

  QByteArray request;
  request.append( "GET " + mp + " HTTP/1.0\r\n" );
  request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
  request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
  request.append( "Accept: */*\r\n" );
  request.append( "Authorization: Basic " + base64 + "\r\n" );
  request.append( "Connection: close\r\n" );
  request.append( "\r\n" );

  mSocket.write( request );
  mSocket.flush();
}

void NtripSocketClient::onReadyRead()
{
  QByteArray data = mSocket.readAll();

  // If headers not processed yet, accumulate data until we find the header end marker
  if ( !mHeadersSent )
  {
    mHeaderBuffer.append( data );

    qsizetype headerEnd = mHeaderBuffer.indexOf( "\r\n\r\n" );
    if ( headerEnd != -1 )
    {
      // Headers complete, extract any data that came after them
      mHeaderBuffer = mHeaderBuffer.mid( headerEnd + 4 );
      mHeadersSent = true;
      emit streamConnected();

      // If there was data after headers in the same chunk, emit it
      if ( !mHeaderBuffer.isEmpty() )
      {
        emit correctionDataReceived( mHeaderBuffer );
        mHeaderBuffer.clear();
      }
    }
    return;
  }

  if ( !data.isEmpty() )
  {
    emit correctionDataReceived( data );
  }
}

void NtripSocketClient::onDisconnected()
{
  if ( mHeadersSent )
  {
    // Server-initiated disconnection after stream was connected
    emit streamDisconnected();
  }
  else
  {
    // Connection failed or disconnected before headers were processed
    emit errorOccurred( QString( "Disconnected from NTRIP caster %1:%2 (%3)" )
                          .arg( mHost )
                          .arg( mPort )
                          .arg( mMountpoint ) );
  }
}

void NtripSocketClient::onSocketError( QAbstractSocket::SocketError error )
{
  emit errorOccurred( QString( "NTRIP socket error on %1:%2 (%3): %4 (%5)" )
                        .arg( mHost )
                        .arg( mPort )
                        .arg( mMountpoint )
                        .arg( error )
                        .arg( mSocket.errorString() ) );
}