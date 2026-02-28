

# File serialportreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**serialportreceiver.cpp**](serialportreceiver_8cpp.md)

[Go to the documentation of this file](serialportreceiver_8cpp.md)


```C++
/***************************************************************************
 serialportreceiver.h - SerialPortReceiver

 ---------------------
 begin                : 15.01.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "serialportreceiver.h"

QLatin1String SerialPortReceiver::identifier = QLatin1String( "serial" );

SerialPortReceiver::SerialPortReceiver( const QString &address, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mSerialPort( new QSerialPort() )
{
  connect( mSerialPort, qOverload<QSerialPort::SerialPortError>( &QSerialPort::errorOccurred ), this, &SerialPortReceiver::handleError );

  initNmeaConnection( mSerialPort );

  setValid( !mAddress.isEmpty() );
}

SerialPortReceiver::~SerialPortReceiver()
{
  mSerialPort->deleteLater();
  mSerialPort = nullptr;
}

void SerialPortReceiver::handleDisconnectDevice()
{
  if ( socketState() == QAbstractSocket::ConnectedState )
  {
    mSerialPort->close();
    setSocketState( QAbstractSocket::UnconnectedState );
  }
}

void SerialPortReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qInfo() << "SerialPortReceiver: Initiating connection to port name: " << mAddress;

  mSerialPort->setPortName( mAddress );
  mSerialPort->setBaudRate( QSerialPort::Baud9600 );
  if ( mSerialPort->open( QIODevice::ReadOnly ) )
  {
    setSocketState( QAbstractSocket::ConnectedState );
  }
}

void SerialPortReceiver::handleError( QSerialPort::SerialPortError error )
{
  if ( error == QSerialPort::NoError )
  {
    return;
  }

  switch ( error )
  {
    case QSerialPort::DeviceNotFoundError:
      mLastError = tr( "Could not find the serial port device" );
      break;
    case QSerialPort::ReadError:
      mLastError = tr( "Attempt to read from the serial port returned an error" );
      break;
    case QSerialPort::PermissionError:
      mLastError = tr( "The connection was refused due to not having enough permission" );
      break;
    default:
      mLastError = tr( "Serial port receiver error (%1)" ).arg( QMetaEnum::fromType<QSerialPort::SerialPortError>().valueToKey( error ) );
      break;
  }
  qInfo() << QStringLiteral( "SerialPortReceiver: Error: %1" ).arg( mLastError );

  emit lastErrorChanged( mLastError );
}
```


