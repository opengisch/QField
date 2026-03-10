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
#include "ntripclient.h"
#include "ntripsocketclient.h"

#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>

NtripClient::NtripClient( QObject *parent )
  : QObject( parent )
{
}

NtripClient::~NtripClient()
{
  stop();
}

void NtripClient::start( const QString &ntripHost, const quint16 &port, const QString &mountpoint, const QString &username, const QString &password, int version )
{
  if ( mSocketClient )
  {
    return;
  }

  mBytesSent = 0;
  mBytesReceived = 0;

  mSocketClient = new NtripSocketClient( this );

  connect( mSocketClient, &NtripSocketClient::correctionDataReceived, [this]( const QByteArray &data ) {
    mBytesReceived += data.size();

    emit correctionDataReceived( data );
    emit bytesCountersChanged();

    logRtcmData( data );
  } );

  connect( mSocketClient, &NtripSocketClient::errorOccurred, [this]( const QString &msg, bool isPermanent ) {
    qWarning() << msg;
    emit errorOccurred( msg, isPermanent );
  } );

  connect( mSocketClient, &NtripSocketClient::streamConnected, [this]() {
    emit streamConnected();
  } );

  connect( mSocketClient, &NtripSocketClient::streamDisconnected, [this]() {
    emit streamDisconnected();
  } );

  mBytesSent = mSocketClient->start(
    ntripHost,
    port,
    "/" + mountpoint,
    username,
    password,
    version );

  // Emit immediately to show sent bytes
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

  const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-ddThh-mm-ss");

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