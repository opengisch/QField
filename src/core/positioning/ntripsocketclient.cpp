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
  const QString &password,
  int version )
{
  mHost = host;
  mPort = port;
  mMountpoint = mountpoint;
  mUsername = username;
  mPassword = password;
  mVersion = version;

  mHeadersSent = false;
  mPendingError = false;
  mChunkedEncoding = false;
  mChunkBuffer.clear();
  mChunkRemaining = -1;

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
  if ( mVersion == 2 )
  {
    request.append( "GET " + mp + " HTTP/1.1\r\n" );
    request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
    request.append( "Ntrip-Version: Ntrip/2.0\r\n" );
    request.append( "User-Agent: QField NTRIP QtSocketClient/2.0\r\n" );
    request.append( "Accept: */*\r\n" );
    request.append( "Authorization: Basic " + base64 + "\r\n" );
    request.append( "Connection: close\r\n" );
    request.append( "\r\n" );
  }
  else
  {
    request.append( "GET " + mp + " HTTP/1.0\r\n" );
    request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
    request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
    request.append( "Accept: */*\r\n" );
    request.append( "Authorization: Basic " + base64 + "\r\n" );
    request.append( "Connection: close\r\n" );
    request.append( "\r\n" );
  }

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

      // Check for chunked transfer encoding (case-insensitive)
      const QString headers = QString::fromLatin1( headerBlock );
      if ( headers.contains( QLatin1String( "Transfer-Encoding: chunked" ), Qt::CaseInsensitive ) )
      {
        mChunkedEncoding = true;
      }

      const int statusCode = parseHttpStatusCode( headerBlock );

      if ( statusCode == 200 )
      {
        mHeadersSent = true;
        emit streamConnected();

        if ( !body.isEmpty() )
        {
          if ( mChunkedEncoding )
          {
            processChunkedData( body );
          }
          else
          {
            emit correctionDataReceived( body );
          }
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
    if ( mChunkedEncoding )
    {
      processChunkedData( data );
    }
    else
    {
      emit correctionDataReceived( data );
    }
  }
}

void NtripSocketClient::processChunkedData( const QByteArray &data )
{
  mChunkBuffer.append( data );

  while ( !mChunkBuffer.isEmpty() )
  {
    if ( mChunkRemaining == -1 )
    {
      // Reading chunk size line
      const int lineEnd = mChunkBuffer.indexOf( "\r\n" );
      if ( lineEnd == -1 )
        break; // Need more data

      const QByteArray sizeLine = mChunkBuffer.left( lineEnd ).trimmed();
      bool ok = false;
      const int chunkSize = sizeLine.toInt( &ok, 16 );
      if ( !ok )
      {
        // Invalid chunk size, treat remaining buffer as raw data
        emit correctionDataReceived( mChunkBuffer );
        mChunkBuffer.clear();
        break;
      }

      mChunkBuffer.remove( 0, lineEnd + 2 );

      if ( chunkSize == 0 )
      {
        // Final chunk, stream done
        mChunkBuffer.clear();
        break;
      }

      mChunkRemaining = chunkSize;
    }
    else if ( mChunkRemaining > 0 )
    {
      // Reading chunk data
      const int available = std::min( static_cast<int>( mChunkBuffer.size() ), mChunkRemaining );
      const QByteArray chunk = mChunkBuffer.left( available );
      mChunkBuffer.remove( 0, available );
      mChunkRemaining -= available;
      emit correctionDataReceived( chunk );
    }
    else
    {
      // mChunkRemaining == 0: between chunks, skip trailing \r\n
      if ( mChunkBuffer.size() < 2 )
        break; // Need more data

      mChunkBuffer.remove( 0, 2 );
      mChunkRemaining = -1;
    }
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