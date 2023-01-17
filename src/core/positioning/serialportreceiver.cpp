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

SerialPortReceiver::SerialPortReceiver( const QString &address, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mSocket( new QSerialPort() )
{
#if QT_VERSION < QT_VERSION_CHECK( 5, 15, 0 )
  connect( mSocket, qOverload<QSerialPort::SerialPortError>( &QSerialPort::error ), this, &SerialPortReceiver::handleError );
#else
  connect( mSocket, qOverload<QSerialPort::SerialPortError>( &QSerialPort::errorOccurred ), this, &SerialPortReceiver::handleError );
#endif

  initNmeaConnection( mSocket );

  setValid( !mAddress.isEmpty() );
}

SerialPortReceiver::~SerialPortReceiver()
{
  mSocket->deleteLater();
  mSocket = nullptr;
}

void SerialPortReceiver::handleDisconnectDevice()
{
  if ( mSocketState == QAbstractSocket::ConnectedState )
  {
    mSocket->close();

    mSocketState = QAbstractSocket::UnconnectedState;
    emit socketStateChanged( mSocketState );
  }
}

void SerialPortReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qInfo() << "SerialPortReceiver: Initiating connection to port name: " << mAddress;

  mSocket->setPortName( mAddress );
  mSocket->setBaudRate( QSerialPort::Baud9600 );
  if ( mSocket->open( QIODevice::ReadOnly ) )
  {
    mSocketState = QAbstractSocket::ConnectedState;
    emit socketStateChanged( mSocketState );
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
