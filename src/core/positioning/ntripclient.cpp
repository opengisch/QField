/***************************************************************************
  ntripclient.cpp - NtripClient

 ---------------------
 begin                : 05.02.2026
 copyright            : (C) 2026 by Vincent LAMBERT
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "abstractgnssreceiver.h"
#include "nmeagnssreceiver.h"
#include "ntripclient.h"

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QRegularExpression>


NtripClient::NtripClient( QObject *parent )
  : QObject( parent )
{
}

NtripClient::~NtripClient() noexcept
{
  stop();
}

void NtripClient::start( const NtripSettings &ntripSettings, AbstractGnssReceiver *receiver )
{
  if ( mSocket )
  {
    stop();
  }

  if ( mReceiver )
  {
    disconnect( this, &NtripClient::correctionDataReceived, mReceiver, &AbstractGnssReceiver::onCorrectionDataReceived );

    if ( const NmeaGnssReceiver *nmeaReceiver = dynamic_cast<const NmeaGnssReceiver *>( mReceiver.get() ) )
    {
      disconnect( nmeaReceiver, &NmeaGnssReceiver::nmeaSentenceReceived, this, &NtripClient::nmeaSentenceReceived );
    }
  }

  mReceiver = receiver;

  if ( mReceiver )
  {
    connect( this, &NtripClient::correctionDataReceived, mReceiver, &AbstractGnssReceiver::onCorrectionDataReceived );

    if ( ntripSettings.forwardNmeaSentences() )
    {
      if ( const NmeaGnssReceiver *nmeaReceiver = dynamic_cast<const NmeaGnssReceiver *>( mReceiver.get() ) )
      {
        connect( nmeaReceiver, &NmeaGnssReceiver::nmeaSentenceReceived, this, &NtripClient::nmeaSentenceReceived );
      }
    }
  }

  mLastNtripGgaSent = 0;
  mBytesSent = 0;
  mBytesReceived = 0;

  qInfo() << QStringLiteral( "Starting NTRIP client: host %1, port %2, mount point %3" ).arg( ntripSettings.host(), QString::number( ntripSettings.port() ), ntripSettings.mountPoint() );
  mSocket = new NtripSocket( this );

  connect( mSocket, &NtripSocket::correctionDataReceived, this, [this]( const QByteArray &data ) {
    mBytesReceived += data.size();

    emit correctionDataReceived( data );
    emit bytesReceivedChanged();

    logData( data );
  } );

  connect( mSocket, &NtripSocket::errorOccurred, this, [this]( const QString &msg, bool isPermanent ) {
    qInfo() << msg;
    emit errorOccurred( msg, isPermanent );
  } );

  connect( mSocket, &NtripSocket::streamConnected, this, [this]() {
    emit streamConnected();
  } );

  connect( mSocket, &NtripSocket::streamDisconnected, this, [this]() {
    emit streamDisconnected();
  } );

  mBytesSent = mSocket->connectToHost( ntripSettings );
  emit bytesSentChanged();
}

void NtripClient::stop()
{
  stopLogging();

  if ( mSocket )
  {
    mSocket->abort();
    mSocket->deleteLater();
    mSocket = nullptr;
  }
}

void NtripClient::sendNmeaSentence( const QString &sentence )
{
  if ( !mSocket )
  {
    return;
  }

  const qint64 bytesWritten = mSocket->writeNmeaSentence( sentence.toUtf8() );
  if ( bytesWritten > 0 )
  {
    mBytesSent += bytesWritten;
    emit bytesSentChanged();
  }
}

void NtripClient::startLogging( const QString &path )
{
  if ( mLogFile.isOpen() )
  {
    return;
  }

  if ( !QFileInfo::exists( path ) )
  {
    return;
  }

  const QString timestamp = QDateTime::currentDateTime().toString( "yyyy-MM-ddThh-mm-ss" );

  mLogFile.setFileName( path + "/ntrip-" + timestamp + ".bin" );

  if ( !mLogFile.open( QIODevice::WriteOnly ) )
  {
    qInfo() << "NTRIP Client: Failed to open log file" << mLogFile.fileName() << mLogFile.errorString();
    return;
  }

  mLogStream.setDevice( &mLogFile );
  mLogStream.setByteOrder( QDataStream::LittleEndian );

  mLogBlockCount = 0;
}

void NtripClient::stopLogging()
{
  if ( mLogFile.isOpen() )
  {
    mLogFile.flush();
    mLogFile.close();
  }

  mLogBlockCount = 0;
}

void NtripClient::logData( const QByteArray &data )
{
  if ( !mLogFile.isOpen() )
  {
    return;
  }

  mLogStream << static_cast<qint64>( QDateTime::currentMSecsSinceEpoch() );
  mLogStream << static_cast<quint32>( data.size() );
  mLogStream.writeRawData( data.constData(), data.size() );

  mLogBlockCount++;
  if ( mLogBlockCount % 10 == 0 )
  {
    mLogFile.flush();
  }
}

void NtripClient::nmeaSentenceReceived( const QString &sentence )
{
  const qint64 epoch = QDateTime::currentMSecsSinceEpoch();
  if ( mLastNtripGgaSent != 0 && ( epoch - mLastNtripGgaSent ) < 10000 )
  {
    return;
  }

  const thread_local QRegularExpression rxSentence( u"^\\$([A-Z]{2})([A-Z]{3})"_s );
  const QRegularExpressionMatch sentenceMatch = rxSentence.match( sentence );
  const QString talkerId = sentenceMatch.captured( 1 );
  const QString sentenceId = sentenceMatch.captured( 2 );
  if ( sentenceId != QStringLiteral( "GGA" ) )
  {
    return;
  }

  QString sentenceToSend = sentence;
  if ( talkerId != QStringLiteral( "GP" ) )
  {
    // Some NTRIP servers will only consume GPGGA sentences, we must adjust accordingly
    const int asteriskIndex = sentence.lastIndexOf( '*' );
    if ( asteriskIndex > 0 )
    {
      QString payload = sentence.mid( 1, asteriskIndex - 1 );
      payload.replace( 0, 2, QStringLiteral( "GP" ) );

      // Calculate new checkum
      const QByteArray payloadBytes = payload.toLatin1();
      const quint8 checksum = std::accumulate( payloadBytes.begin(), payloadBytes.end(), 0, std::bit_xor<quint8>() );

      const QString trailing = sentence.mid( asteriskIndex + 3 );
      sentenceToSend = QStringLiteral( "$%1*%2%3" )
                         .arg( payload )
                         .arg( checksum, 2, 16, QLatin1Char( '0' ) )
                         .toUpper()
                         .arg( trailing );
    }
  }

  sendNmeaSentence( sentenceToSend );
  mLastNtripGgaSent = epoch;
}

// ---

NtripSocket::NtripSocket( QObject *parent )
  : QObject( parent )
  , mSocket( new QSslSocket( this ) )
{
  mSocket->setSocketOption( QAbstractSocket::LowDelayOption, true );

  connect( mSocket, &QAbstractSocket::connected, this, &NtripSocket::onConnected );
  connect( mSocket, &QAbstractSocket::readyRead, this, &NtripSocket::onReadyRead );
  connect( mSocket, &QAbstractSocket::disconnected, this, &NtripSocket::onDisconnected );
  connect( mSocket, &QAbstractSocket::errorOccurred, this, &NtripSocket::onSocketError );
}

NtripSocket::~NtripSocket() noexcept
{
  abort();
}

qint64 NtripSocket::connectToHost( const NtripSettings &ntripSettings )
{
  if ( mSocket->isOpen() )
  {
    abort();
  }

  mHost = ntripSettings.host();
  mPort = ntripSettings.port();
  mMountPoint = ntripSettings.mountPoint();
  mUsername = ntripSettings.username();
  mPassword = ntripSettings.password();
  mProtocol = ntripSettings.protocol();

  mHeadersSent = false;
  mPendingError = false;
  mChunkedEncoding = false;
  mChunkBuffer.clear();
  mChunkRemaining = -1;

  switch ( mProtocol )
  {
    case NtripSettings::NtripSsl:
    {
      mSocket->connectToHostEncrypted( mHost, mPort );
      break;
    }

    case NtripSettings::NtripVersion2:
    case NtripSettings::NtripVersion1:
    {
      mSocket->connectToHost( mHost, mPort );
      break;
    }
  }

  return estimateRequestSize();
}

qint64 NtripSocket::estimateRequestSize() const
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  return 200 + base64.size() + mMountPoint.size();
}

qint64 NtripSocket::writeNmeaSentence( const QByteArray &sentence )
{
  if ( !mSocket || !mSocket->isOpen() || mSocket->state() != QAbstractSocket::ConnectedState )
  {
    return -1;
  }

  QByteArray payload = sentence;
  if ( !payload.endsWith( "\r\n" ) )
  {
    payload.append( "\r\n" );
  }

  return mSocket->write( payload );
}

void NtripSocket::abort()
{
  if ( mSocket->isOpen() )
  {
    mSocket->abort();
  }

  mHeadersSent = false;
}

void NtripSocket::onConnected()
{
  qInfo() << QStringLiteral( "Connected to NTRIP caster:  host %1, port %2, mount point %3" ).arg( mHost, QString::number( mPort ), mMountPoint );

  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray mp = mMountPoint.toUtf8();
  if ( !mp.startsWith( '/' ) )
  {
    mp.prepend( '/' );
  }

  QByteArray request;
  switch ( mProtocol )
  {
    case NtripSettings::NtripSsl:
    case NtripSettings::NtripVersion2:
    {
      request.append( "GET " + mp + " HTTP/1.1\r\n" );
      request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
      request.append( "Ntrip-Version: Ntrip/2.0\r\n" );
      request.append( "User-Agent: QField NTRIP Client/2.0\r\n" );
      request.append( "Accept: */*\r\n" );
      request.append( "Authorization: Basic " + base64 + "\r\n" );
      request.append( "Connection: close\r\n" );
      request.append( "\r\n" );
      break;
    }

    case NtripSettings::NtripVersion1:
    {
      request.append( "GET " + mp + " HTTP/1.0\r\n" );
      request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
      request.append( "User-Agent: QField NTRIP Client/1.0\r\n" );
      request.append( "Accept: */*\r\n" );
      request.append( "Authorization: Basic " + base64 + "\r\n" );
      request.append( "Connection: close\r\n" );
      request.append( "\r\n" );
      break;
    }
  }

  mSocket->write( request );
  mSocket->flush();
}

void NtripSocket::onReadyRead()
{
  QByteArray data = mSocket->readAll();

  // If headers not processed yet, accumulate data until we find the header end marker
  if ( !mHeadersSent )
  {
    mHeaderBuffer.append( data );

    qsizetype headerEnd = mHeaderBuffer.indexOf( "\r\n\r\n" );
    int separatorSize = 4;
    if ( headerEnd == -1 )
    {
      // Check for ICY 200 header
      headerEnd = mHeaderBuffer.indexOf( "\r\n" );
      if ( headerEnd >= 0 )
      {
        if ( mHeaderBuffer.startsWith( "ICY 200 OK" ) )
        {
          separatorSize = 2;
        }
        else
        {
          headerEnd = -1;
        }
      }
    }

    if ( headerEnd >= 0 )
    {
      const QByteArray headerBlock = mHeaderBuffer.left( headerEnd );
      const QByteArray body = mHeaderBuffer.mid( headerEnd + separatorSize );
      mHeaderBuffer.clear();

      // Check for SOURCETABLE response (mountpoint not found)
      if ( headerBlock.startsWith( "SOURCETABLE" ) )
      {
        mPendingError = true;
        emit errorOccurred( QStringLiteral( "NTRIP mountpoint '" ) + mMountPoint
                              + QStringLiteral( "' not found on " ) + mHost
                              + QStringLiteral( ":" ) + QString::number( mPort )
                              + QStringLiteral( " (received SOURCETABLE)" ),
                            true );
        mSocket->abort();
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
        emit errorOccurred( QStringLiteral( "NTRIP caster " ) + mHost
                              + QStringLiteral( ":" ) + QString::number( mPort )
                              + QStringLiteral( " (" ) + mMountPoint
                              + QStringLiteral( ") returned HTTP " ) + QString::number( statusCode ),
                            true );
        mSocket->abort();
      }
      else if ( statusCode > 0 )
      {
        mPendingError = true;
        emit errorOccurred( QStringLiteral( "NTRIP caster " ) + mHost
                              + QStringLiteral( ":" ) + QString::number( mPort )
                              + QStringLiteral( " (" ) + mMountPoint
                              + QStringLiteral( ") returned HTTP " ) + QString::number( statusCode ),
                            false );
        mSocket->abort();
      }
      else
      {
        mPendingError = true;
        emit errorOccurred( QStringLiteral( "NTRIP caster " ) + mHost
                              + QStringLiteral( ":" ) + QString::number( mPort )
                              + QStringLiteral( " (" ) + mMountPoint
                              + QStringLiteral( ") returned unparsable response" ),
                            false );
        mSocket->abort();
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

void NtripSocket::processChunkedData( const QByteArray &data )
{
  mChunkBuffer.append( data );

  while ( !mChunkBuffer.isEmpty() )
  {
    if ( mChunkRemaining == -1 )
    {
      // Reading chunk size line
      const int lineEnd = mChunkBuffer.indexOf( "\r\n" );
      if ( lineEnd == -1 )
      {
        break; // Need more data
      }

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
      {
        // Need more data
        break;
      }

      mChunkBuffer.remove( 0, 2 );
      mChunkRemaining = -1;
    }
  }
}

void NtripSocket::onDisconnected()
{
  if ( mHeadersSent )
  {
    // Server-initiated disconnection after stream was connected
    emit streamDisconnected();
  }
  else if ( !mPendingError )
  {
    // Connection failed or disconnected before headers were processed
    emit errorOccurred( QStringLiteral( "Disconnected from NTRIP caster " ) + mHost
                          + QStringLiteral( ":" ) + QString::number( mPort )
                          + QStringLiteral( " (" ) + mMountPoint + QStringLiteral( ")" ),
                        false );
  }
}

void NtripSocket::onSocketError( QAbstractSocket::SocketError error )
{
  emit errorOccurred( QStringLiteral( "NTRIP socket error on " ) + mHost
                        + QStringLiteral( ":" ) + QString::number( mPort )
                        + QStringLiteral( " (" ) + mMountPoint
                        + QStringLiteral( "): " ) + QString::number( static_cast<int>( error ) )
                        + QStringLiteral( " (" ) + mSocket->errorString() + QStringLiteral( ")" ),
                      false );
}

int NtripSocket::parseHttpStatusCode( const QByteArray &headerBlock )
{
  // Match first line: "HTTP/1.0 200 OK", "ICY 200 OK", "SOURCETABLE 200 OK"
  const thread_local QRegularExpression re( QStringLiteral( "^(?:HTTP/\\d\\.\\d|ICY|SOURCETABLE)\\s+(\\d{3})" ) );
  const int firstNewLine = headerBlock.indexOf( "\r\n" );
  const QString firstLine = QString::fromLatin1( firstNewLine > 0 ? headerBlock.left( firstNewLine ) : headerBlock );
  const QRegularExpressionMatch match = re.match( firstLine );
  return match.hasMatch() ? match.captured( 1 ).toInt() : -1;
}

bool NtripSocket::isPermanentHttpError( int statusCode )
{
  return statusCode >= 400 && statusCode < 500;
}
