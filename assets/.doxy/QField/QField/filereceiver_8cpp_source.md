

# File filereceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**filereceiver.cpp**](filereceiver_8cpp.md)

[Go to the documentation of this file](filereceiver_8cpp.md)


```C++
/***************************************************************************
 tcpreceiver.cpp - TcpReceiver

 ---------------------
 begin                : September 2022
 copyright            : (C) 2022 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "filereceiver.h"

#include <QFileInfo>

QLatin1String FileReceiver::identifier = QLatin1String( "file" );

FileReceiver::FileReceiver( const QString &filePath, const int interval, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mBuffer( new QBuffer() )
{
  QFileInfo fi( filePath );
  if ( fi.exists() && fi.size() > 0 )
  {
    mLogs.setFileName( filePath );
    setValid( true );
  }
  else
  {
    setValid( false );
  }

  mTimer.setSingleShot( false );
  mTimer.setInterval( std::max( 50, interval ) );
  connect( &mTimer, &QTimer::timeout, this, &FileReceiver::readLogsLine );

  initNmeaConnection( mBuffer );
}

FileReceiver::~FileReceiver()
{
  mBuffer->deleteLater();
  mBuffer = nullptr;
}

void FileReceiver::handleConnectDevice()
{
  if ( !valid() )
  {
    return;
  }

  qInfo() << QStringLiteral( "FileReceiver: Initiating replay of logs %1" ).arg( mLogs.fileName() );
  mLogs.open( QIODevice::ReadOnly );
  mBuffer->open( QIODevice::ReadWrite );

  setSocketState( QAbstractSocket::ConnectedState );

  mTimer.start();
}

void FileReceiver::handleDisconnectDevice()
{
  mTimer.stop();

  setSocketState( QAbstractSocket::UnconnectedState );

  mLogs.close();
  mBuffer->close();
}

void FileReceiver::readLogsLine()
{
  if ( mLogs.atEnd() )
  {
    mLogs.seek( 0 );
  }

  const QByteArray line = mLogs.readLine().trimmed();

  mBuffer->buffer().clear();
  mBuffer->seek( 0 );
  mBuffer->write( line + QByteArray( "\r\n" ) );
  mBuffer->seek( 0 );
}
```


