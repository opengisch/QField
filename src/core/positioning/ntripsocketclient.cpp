#include "ntripsocketclient.h"

#include <QDebug>
#include <QRegularExpression>

namespace
{
  int parseHttpStatusCode( const QByteArray &headerBlock )
  {
    // Match first line: "HTTP/1.0 200 OK", "ICY 200 OK", "SOURCETABLE 200 OK"
    static const QRegularExpression re( QStringLiteral( "^(?:HTTP/\\d\\.\\d|ICY|SOURCETABLE)\\s+(\\d{3})" ) );
    const QString firstLine = QString::fromLatin1( headerBlock.left( headerBlock.indexOf( "\r\n" ) ) );
    const QRegularExpressionMatch match = re.match( firstLine );
    if ( match.hasMatch() )
      return match.captured( 1 ).toInt();
    return -1;
  }

  bool isPermanentHttpError( int statusCode )
  {
    return statusCode >= 400 && statusCode < 500;
  }
} // namespace

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
  mPendingError = false;

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

qint64 NtripSocketClient::sendNmeaSentence( const QByteArray &sentence )
{
  if ( !mSocket.isOpen() || mSocket.state() != QAbstractSocket::ConnectedState )
  {
    return -1;
  }

  QByteArray payload = sentence;
  if ( !payload.endsWith( "\r\n" ) )
  {
    payload.append( "\r\n" );
  }

  return mSocket.write( payload );
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
      const QByteArray headerBlock = mHeaderBuffer.left( headerEnd );
      const QByteArray body = mHeaderBuffer.mid( headerEnd + 4 );
      mHeaderBuffer.clear();

      // Check for SOURCETABLE response (mountpoint not found)
      if ( headerBlock.startsWith( "SOURCETABLE" ) )
      {
        mPendingError = true;
        emit errorOccurred( QString( "NTRIP mountpoint '%1' not found on %2:%3 (received SOURCETABLE)" )
                              .arg( mMountpoint, mHost )
                              .arg( mPort ),
                            true );
        mSocket.abort();
        return;
      }

      const int statusCode = parseHttpStatusCode( headerBlock );

      if ( statusCode == 200 )
      {
        mHeadersSent = true;
        emit streamConnected();

        if ( !body.isEmpty() )
        {
          emit correctionDataReceived( body );
        }
      }
      else if ( statusCode > 0 && isPermanentHttpError( statusCode ) )
      {
        mPendingError = true;
        emit errorOccurred( QString( "NTRIP caster %1:%2 (%3) returned HTTP %4" )
                              .arg( mHost )
                              .arg( mPort )
                              .arg( mMountpoint )
                              .arg( statusCode ),
                            true );
        mSocket.abort();
      }
      else if ( statusCode > 0 )
      {
        mPendingError = true;
        emit errorOccurred( QString( "NTRIP caster %1:%2 (%3) returned HTTP %4" )
                              .arg( mHost )
                              .arg( mPort )
                              .arg( mMountpoint )
                              .arg( statusCode ),
                            false );
        mSocket.abort();
      }
      else
      {
        mPendingError = true;
        emit errorOccurred( QString( "NTRIP caster %1:%2 (%3) returned unparsable response" )
                              .arg( mHost )
                              .arg( mPort )
                              .arg( mMountpoint ),
                            false );
        mSocket.abort();
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
  else if ( !mPendingError )
  {
    // Connection failed or disconnected before headers were processed
    emit errorOccurred( QString( "Disconnected from NTRIP caster %1:%2 (%3)" )
                          .arg( mHost )
                          .arg( mPort )
                          .arg( mMountpoint ),
                        false );
  }
}

void NtripSocketClient::onSocketError( QAbstractSocket::SocketError error )
{
  emit errorOccurred( QString( "NTRIP socket error on %1:%2 (%3): %4 (%5)" )
                        .arg( mHost )
                        .arg( mPort )
                        .arg( mMountpoint )
                        .arg( error )
                        .arg( mSocket.errorString() ),
                      false );
}