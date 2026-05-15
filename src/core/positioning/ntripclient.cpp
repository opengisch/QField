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
  qDebug() << "Starting NTRIP Client";
  if ( mSocketClient )
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

  mBytesSent = 0;
  mBytesReceived = 0;

  mSocketClient = new NtripSocketClient( this );

  connect( mSocketClient, &NtripSocketClient::correctionDataReceived, this, [this]( const QByteArray &data ) {
    mBytesReceived += data.size();

    emit correctionDataReceived( data );
    emit bytesCountersChanged();

    logRtcmData( data );
  } );

  connect( mSocketClient, &NtripSocketClient::errorOccurred, this, [this]( const QString &msg, bool isPermanent ) {
    qInfo() << msg;
    emit errorOccurred( msg, isPermanent );
  } );

  connect( mSocketClient, &NtripSocketClient::streamConnected, this, [this]() {
    emit streamConnected();
  } );

  connect( mSocketClient, &NtripSocketClient::streamDisconnected, this, [this]() {
    emit streamDisconnected();
  } );

  mBytesSent = mSocketClient->start( ntripSettings );
  emit bytesCountersChanged();
}

void NtripClient::stop()
{
  stopLogging();

  if ( mSocketClient )
  {
    mSocketClient->stop();
    mSocketClient->deleteLater();
    mSocketClient = nullptr;
  }
}

void NtripClient::sendNmeaSentence( const QString &sentence )
{
  if ( !mSocketClient )
  {
    return;
  }

  const qint64 bytesWritten = mSocketClient->sendNmeaSentence( sentence.toUtf8() );
  if ( bytesWritten > 0 )
  {
    mBytesSent += bytesWritten;
    emit bytesCountersChanged();
  }
}

void NtripClient::startLogging( const QString &path )
{
  if ( mLogFile.isOpen() )
    return;

  if ( !QFileInfo::exists( path ) )
    return;

  const QString timestamp = QDateTime::currentDateTime().toString( "yyyy-MM-ddThh-mm-ss" );

  mLogFile.setFileName( path + "/rtcm-" + timestamp + ".bin" );

  if ( !mLogFile.open( QIODevice::WriteOnly ) )
  {
    qWarning() << "NtripClient: Failed to open RTCM log file" << mLogFile.fileName() << mLogFile.errorString();
    return;
  }
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

void NtripClient::logRtcmData( const QByteArray &data )
{
  if ( !mLogFile.isOpen() )
    return;

  QDataStream stream( &mLogFile );
  stream.setByteOrder( QDataStream::LittleEndian );
  stream << static_cast<qint64>( QDateTime::currentMSecsSinceEpoch() );
  stream << static_cast<quint32>( data.size() );
  stream.writeRawData( data.constData(), data.size() );

  mLogBlockCount++;
  if ( mLogBlockCount % 10 == 0 )
  {
    mLogFile.flush();
  }
}
void NtripClient::nmeaSentenceReceived( const QString &sentence )
{
  const qint64 epoch = QDateTime::currentMSecsSinceEpoch();
  if ( mLastNtripGgaSent != 0 && ( epoch - mLastNtripGgaSent ) < 900 )
  {
    return;
  }
  mLastNtripGgaSent = epoch;

  if ( !( sentence.startsWith( "$GPGGA" ) || sentence.startsWith( "$GNGGA" ) || sentence.startsWith( "$GLGGA" ) || sentence.startsWith( "$GAGGA" ) || sentence.startsWith( "$GBGGA" ) ) )
  {
    return;
  }

  sendNmeaSentence( sentence );
}

// ---

NtripSocketClient::NtripSocketClient( QObject *parent )
  : QObject( parent ), mSocket( new QTcpSocket( this ) )
{
  mSocket->setSocketOption( QAbstractSocket::LowDelayOption, true );

  connect( mSocket, &QTcpSocket::connected, this, &NtripSocketClient::onConnected );
  connect( mSocket, &QTcpSocket::readyRead, this, &NtripSocketClient::onReadyRead );
  connect( mSocket, &QTcpSocket::disconnected, this, &NtripSocketClient::onDisconnected );
  connect( mSocket, &QAbstractSocket::errorOccurred, this, &NtripSocketClient::onSocketError );
}

NtripSocketClient::~NtripSocketClient() noexcept
{
  stop();
}

qint64 NtripSocketClient::start( const NtripSettings &ntripSettings )
{
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

  if ( mSocket->isOpen() )
    stop();

  mSocket->connectToHost( mHost, mPort );

  return estimateRequestSize();
}

qint64 NtripSocketClient::estimateRequestSize() const
{
  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  return 200 + base64.size() + mMountPoint.size();
}

qint64 NtripSocketClient::sendNmeaSentence( const QByteArray &sentence )
{
  if ( !mSocket->isOpen() || mSocket->state() != QAbstractSocket::ConnectedState )
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

void NtripSocketClient::stop()
{
  if ( mSocket->isOpen() )
  {
    mSocket->abort();
  }
  mHeadersSent = false;
}

void NtripSocketClient::onConnected()
{
  qDebug() << "Connected to NTRIP caster:" << mHost << mPort << "mount:" << mMountPoint;

  QString credentials = mUsername + ":" + mPassword;
  QByteArray base64 = credentials.toUtf8().toBase64();

  QByteArray mp = mMountPoint.toUtf8();
  if ( !mp.startsWith( '/' ) )
    mp.prepend( '/' );

  QByteArray request;
  switch ( mProtocol )
  {
    case NtripSettings::NtripVersion2:
    {
      request.append( "GET " + mp + " HTTP/1.1\r\n" );
      request.append( "Host: " + mHost.toUtf8() + ":" + QByteArray::number( mPort ) + "\r\n" );
      request.append( "Ntrip-Version: Ntrip/2.0\r\n" );
      request.append( "User-Agent: QField NTRIP QtSocketClient/2.0\r\n" );
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
      request.append( "User-Agent: QField NTRIP QtSocketClient/1.0\r\n" );
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

void NtripSocketClient::onReadyRead()
{
  QByteArray data = mSocket->readAll();

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
    emit errorOccurred( QStringLiteral( "Disconnected from NTRIP caster " ) + mHost
                          + QStringLiteral( ":" ) + QString::number( mPort )
                          + QStringLiteral( " (" ) + mMountPoint + QStringLiteral( ")" ),
                        false );
  }
}

void NtripSocketClient::onSocketError( QAbstractSocket::SocketError error )
{
  emit errorOccurred( QStringLiteral( "NTRIP socket error on " ) + mHost
                        + QStringLiteral( ":" ) + QString::number( mPort )
                        + QStringLiteral( " (" ) + mMountPoint
                        + QStringLiteral( "): " ) + QString::number( static_cast<int>( error ) )
                        + QStringLiteral( " (" ) + mSocket->errorString() + QStringLiteral( ")" ),
                      false );
}

int NtripSocketClient::parseHttpStatusCode( const QByteArray &headerBlock )
{
  // Match first line: "HTTP/1.0 200 OK", "ICY 200 OK", "SOURCETABLE 200 OK"
  static const QRegularExpression re( QStringLiteral( "^(?:HTTP/\\d\\.\\d|ICY|SOURCETABLE)\\s+(\\d{3})" ) );
  const QString firstLine = QString::fromLatin1( headerBlock.left( headerBlock.indexOf( "\r\n" ) ) );
  const QRegularExpressionMatch match = re.match( firstLine );
  if ( match.hasMatch() )
    return match.captured( 1 ).toInt();
  return -1;
}

bool NtripSocketClient::isPermanentHttpError( int statusCode )
{
  return statusCode >= 400 && statusCode < 500;
}
