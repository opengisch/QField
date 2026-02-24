#include "ntripsourcetablefetcher.h"

#include <QDebug>

NtripSourceTableFetcher::NtripSourceTableFetcher( QObject *parent )
  : QObject( parent )
{
}

NtripSourceTableFetcher::~NtripSourceTableFetcher()
{
  cancel();
}

void NtripSourceTableFetcher::fetch( const QString &host, int port, const QString &username, const QString &password )
{
  if ( mFetching )
    return;

  cancel();

  mHost = host;
  mPort = static_cast<quint16>( port );
  mUsername = username;
  mPassword = password;
  mBuffer.clear();

  mSocket = new QTcpSocket( this );
  connect( mSocket, &QTcpSocket::connected, this, &NtripSourceTableFetcher::onConnected );
  connect( mSocket, &QTcpSocket::readyRead, this, &NtripSourceTableFetcher::onReadyRead );
  connect( mSocket, &QTcpSocket::disconnected, this, &NtripSourceTableFetcher::onDisconnected );
  connect( mSocket, QOverload<QAbstractSocket::SocketError>::of( &QTcpSocket::errorOccurred ),
           this, &NtripSourceTableFetcher::onSocketError );

  mFetching = true;
  emit fetchingChanged();

  mSocket->connectToHost( mHost, mPort );
}

void NtripSourceTableFetcher::cancel()
{
  if ( mSocket )
  {
    mSocket->abort();
    mSocket->deleteLater();
    mSocket = nullptr;
  }
  if ( mFetching )
  {
    mFetching = false;
    emit fetchingChanged();
  }
}

void NtripSourceTableFetcher::cleanup()
{
  if ( mSocket )
  {
    mSocket->deleteLater();
    mSocket = nullptr;
  }
  mFetching = false;
  emit fetchingChanged();
}

void NtripSourceTableFetcher::onConnected()
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray request;
  request.append( "GET / HTTP/1.0\r\n" );
  request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
  request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
  request.append( "Accept: */*\r\n" );
  request.append( "Authorization: Basic " + base64 + "\r\n" );
  request.append( "Connection: close\r\n" );
  request.append( "\r\n" );

  mSocket->write( request );
  mSocket->flush();
}

void NtripSourceTableFetcher::onReadyRead()
{
  mBuffer.append( mSocket->readAll() );
}

void NtripSourceTableFetcher::onDisconnected()
{
  const QStringList mountpoints = parseSourceTable( mBuffer );
  mMountpoints = mountpoints;
  emit mountpointsChanged();

  if ( mountpoints.isEmpty() )
  {
    emit fetchError( tr( "No mountpoints found in NTRIP source table" ) );
  }

  cleanup();
}

void NtripSourceTableFetcher::onSocketError( QAbstractSocket::SocketError error )
{
  const QString message = mSocket ? mSocket->errorString() : QString::number( error );
  emit fetchError( tr( "NTRIP source table fetch error: %1" ).arg( message ) );
  cleanup();
}

QStringList NtripSourceTableFetcher::parseSourceTable( const QByteArray &data ) const
{
  QStringList mountpoints;

  // Skip HTTP headers: find the blank line separator
  const int headerEnd = data.indexOf( "\r\n\r\n" );
  const QByteArray body = ( headerEnd >= 0 ) ? data.mid( headerEnd + 4 ) : data;

  const QList<QByteArray> lines = body.split( '\n' );
  for ( const QByteArray &line : lines )
  {
    const QByteArray trimmed = line.trimmed();
    if ( !trimmed.startsWith( "STR;" ) )
      continue;

    const QList<QByteArray> fields = trimmed.split( ';' );
    if ( fields.size() >= 2 )
    {
      const QString mountpoint = QString::fromUtf8( fields.at( 1 ) ).trimmed();
      if ( !mountpoint.isEmpty() )
        mountpoints.append( mountpoint );
    }
  }

  return mountpoints;
}
