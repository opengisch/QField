/***************************************************************************
 bluetoothlowenergyreceiver.cpp - BluetoothLowEnergyReceiver

 ---------------------
 begin                : 2026/05/22
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "bluetoothlowenergyreceiver.h"

#include <QDebug>
#include <QGuiApplication>
#include <QMetaEnum>
#include <QTimer>

QMap<QBluetoothUuid, std::pair<QBluetoothUuid, QBluetoothUuid>> BluetoothLowEnergyReceiver::serviceChars = {
  // Standard Nordic UART Service (NUS) UUIDs
  { QBluetoothUuid( "6e400001-b5a3-f393-e0a9-e50e24dcca9e" ), std::make_pair( QBluetoothUuid( "6e400003-b5a3-f393-e0a9-e50e24dcca9e" ), QBluetoothUuid( "6e400002-b5a3-f393-e0a9-e50e24dcca9e" ) ) },
  // Beken Corp. UUIDs
  { QBluetoothUuid( "0000ffe0-0000-1000-8000-00805f9b34fb" ), std::make_pair( QBluetoothUuid( "0000ffe1-0000-1000-8000-00805f9b34fb" ), QBluetoothUuid( "0000ffe2-0000-1000-8000-00805f9b34fb" ) ) },
};


QLatin1String BluetoothLowEnergyReceiver::identifier = QLatin1String( "ble" );

BluetoothLowEnergyReceiver::BluetoothLowEnergyReceiver( const QString &address, QObject *parent )
  : NmeaGnssReceiver( parent )
  , mAddress( address )
  , mBuffer( new QBuffer( this ) )
{
  qInfo() << "BluetoothLowEnergyReceiver: Creating the receiver";

  connect( mBuffer, &QIODevice::readyRead, this, [this] {
    if ( mService && mTxCharacteristic.isValid() )
    {
      mService->writeCharacteristic( mTxCharacteristic, mBuffer->readAll(), QLowEnergyService::WriteWithoutResponse );
    }
  } );

  initNmeaConnection( mBuffer );
  setValid( !mAddress.isEmpty() );
}

BluetoothLowEnergyReceiver::~BluetoothLowEnergyReceiver()
{
  disconnectDevice();

  mBuffer->deleteLater();
  mBuffer = nullptr;

  if ( mController )
  {
    mController->deleteLater();
    mController = nullptr;
  }
}

AbstractGnssReceiver::Capabilities BluetoothLowEnergyReceiver::capabilities() const
{
  return AbstractGnssReceiver::Capabilities() | AbstractGnssReceiver::OrthometricAltitude | AbstractGnssReceiver::Logging | AbstractGnssReceiver::NtripCorrection;
}

void BluetoothLowEnergyReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qInfo() << "BluetoothLowEnergyReceiver: Initiating connection to device: " << mAddress;

  mConnectionFailureCount = 0;
  mConnectOnDisconnect = true;

  if ( mController && mController->state() != QLowEnergyController::UnconnectedState )
  {
    disconnectDevice();
  }
  else
  {
    doConnectDevice();
  }
}

void BluetoothLowEnergyReceiver::handleDisconnectDevice()
{
  if ( mController && mController->state() != QLowEnergyController::UnconnectedState )
  {
    qInfo() << "BluetoothLowEnergyReceiver: Disconnecting from device: " << mAddress;

    clearService();

    mConnectOnDisconnect = false;
    mDisconnecting = true;
    mLastGnssPositionValid = false;
    mController->disconnectFromDevice();
  }
}

void BluetoothLowEnergyReceiver::doConnectDevice()
{
  if ( !mController )
  {
    QBluetoothDeviceInfo deviceInfo( QBluetoothAddress( mAddress ), QString(), 0 );
    mController = QLowEnergyController::createCentral( deviceInfo, this );

    connect( mController, &QLowEnergyController::connected, this, &BluetoothLowEnergyReceiver::deviceConnected );
    connect( mController, &QLowEnergyController::disconnected, this, &BluetoothLowEnergyReceiver::deviceDisconnected );
    connect( mController, qOverload<QLowEnergyController::Error>( &QLowEnergyController::errorOccurred ), this, &BluetoothLowEnergyReceiver::controllerErrorOccurred );
    connect( mController, &QLowEnergyController::serviceDiscovered, this, &BluetoothLowEnergyReceiver::serviceDiscovered );
    connect( mController, &QLowEnergyController::discoveryFinished, this, &BluetoothLowEnergyReceiver::serviceDiscoveryFinished );
  }

  setSocketState( QAbstractSocket::ConnectingState );
  mController->connectToDevice();
}

void BluetoothLowEnergyReceiver::deviceConnected()
{
  qInfo() << "BluetoothLowEnergyReceiver: Connected, discovering services";
  mController->discoverServices();
}

void BluetoothLowEnergyReceiver::deviceDisconnected()
{
  qInfo() << "BluetoothLowEnergyReceiver: Disconnected.";
  setSocketState( QAbstractSocket::UnconnectedState );

  clearService();
}

void BluetoothLowEnergyReceiver::controllerErrorOccurred( QLowEnergyController::Error error )
{
  mLastError = QStringLiteral( "Controller Error: %1, %2" ).arg( QMetaEnum::fromType<QLowEnergyController::Error>().valueToKey( static_cast<int>( error ) ), mController->errorString() );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: %1" ).arg( mLastError );

  if ( mController->state() == QLowEnergyController::UnconnectedState || mController->state() == QLowEnergyController::ConnectingState || mController->state() == QLowEnergyController::ClosingState )
  {
    if ( mConnectOnDisconnect )
    {
      mConnectionFailureCount++;
    }

    if ( mConnectionFailureCount > 10 )
    {
      mConnectOnDisconnect = false;
    }
    return;
  }

  emit lastErrorChanged( mLastError );
}

void BluetoothLowEnergyReceiver::serviceDiscovered( const QBluetoothUuid &newService )
{
  if ( serviceChars.contains( newService ) )
  {
    qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Target service found (%1)" ).arg( newService.toString() );
  }
  else
  {
    qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Other service found (%1)" ).arg( newService.toString() );
  }
}

void BluetoothLowEnergyReceiver::serviceDiscoveryFinished()
{
  if ( !mController )
  {
    return;
  }

  clearService();
  mService = nullptr;

  const QList<QBluetoothUuid> validServices = serviceChars.keys();
  const QList<QBluetoothUuid> controllerServices = mController->services();
  for ( const QBluetoothUuid &service : controllerServices )
  {
    if ( validServices.contains( service ) )
    {
      qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Connecting to target service (%1)" ).arg( service.toString() );
      mService = mController->createServiceObject( service, this );
    }
  }

  if ( !mService )
  {
    mLastError = "BluetoothLowEnergyReceiver: Required target service not found on device.";
    qWarning() << mLastError;
    emit lastErrorChanged( mLastError );
    handleDisconnectDevice();
    return;
  }

  connect( mService, &QLowEnergyService::stateChanged, this, &BluetoothLowEnergyReceiver::serviceStateChanged );
  connect( mService, qOverload<QLowEnergyService::ServiceError>( &QLowEnergyService::errorOccurred ), this, &BluetoothLowEnergyReceiver::serviceErrorOccurred );
  connect( mService, &QLowEnergyService::characteristicChanged, this, &BluetoothLowEnergyReceiver::characteristicChanged );

  mService->discoverDetails();
}

void BluetoothLowEnergyReceiver::serviceStateChanged( QLowEnergyService::ServiceState s )
{
  if ( s == QLowEnergyService::RemoteServiceDiscovered )
  {
    mRxCharacteristic = mService->characteristic( serviceChars[mService->serviceUuid()].first );
    mTxCharacteristic = mService->characteristic( serviceChars[mService->serviceUuid()].second );

    if ( mRxCharacteristic.isValid() )
    {
      QLowEnergyDescriptor notificationDesc = mRxCharacteristic.descriptor( QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration );
      if ( notificationDesc.isValid() )
      {
        mService->writeDescriptor( notificationDesc, QByteArray::fromHex( "0100" ) );
        qInfo() << "BluetoothLowEnergyReceiver: Subscribed to RX characteristic notifications.";
      }

      mBuffer->open( QIODevice::ReadWrite );

      setSocketState( QAbstractSocket::ConnectedState );
    }
    else
    {
      qWarning() << "BluetoothLowEnergyReceiver: RX Characteristic not found!";
      handleDisconnectDevice();
    }
  }
}

void BluetoothLowEnergyReceiver::serviceErrorOccurred( QLowEnergyService::ServiceError e )
{
  mLastError = QStringLiteral( "Service Error: %1" ).arg( QMetaEnum::fromType<QLowEnergyService::ServiceError>().valueToKey( static_cast<int>( e ) ) );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: %1" ).arg( mLastError );
  emit lastErrorChanged( mLastError );
}

void BluetoothLowEnergyReceiver::characteristicChanged( const QLowEnergyCharacteristic &c, const QByteArray &value )
{
  qDebug() << c.uuid();
  qDebug() << value;
  if ( c.uuid() == mRxCharacteristic.uuid() )
  {
    // Feed data to the proxy device, which will emit readyRead() for NmeaGnssReceiver
    mBuffer->write( value );
  }
}

void BluetoothLowEnergyReceiver::clearService()
{
  if ( mBuffer->isOpen() )
  {
    mBuffer->close();
  }

  if ( mService )
  {
    mService->deleteLater();
    mService = nullptr;
  }

  mRxCharacteristic = QLowEnergyCharacteristic();
  mTxCharacteristic = QLowEnergyCharacteristic();
}

QString BluetoothLowEnergyReceiver::socketStateString()
{
  const QAbstractSocket::SocketState currentState = socketState();
  QString socketStateString = AbstractGnssReceiver::socketStateString();
  if ( currentState == QAbstractSocket::UnconnectedState )
  {
    if ( !mDisconnecting && mController && mController->error() != QLowEnergyController::NoError )
    {
      socketStateString.append( QStringLiteral( ": %1" ).arg( mController->errorString() ) );
    }
  }
  return socketStateString;
}

void BluetoothLowEnergyReceiver::onCorrectionDataReceived( const QByteArray &data )
{
  if ( !mService || !mTxCharacteristic.isValid() )
  {
    return;
  }

  if ( mAddress.startsWith( "C8:47:8C" ) ) // Beken Corp. handling
  {
    auto shortToByteArray = []( qint16 s ) -> QByteArray {
      QByteArray targets;
      targets.resize( 2 );
      for ( int i = 0; i < targets.length(); i++ )
      {
        int offset = ( targets.length() - 1 - i ) * 8;
        targets[i] = static_cast<char>( ( static_cast<quint16>( s ) >> offset ) & 0xFF );
      }
      return targets;
    };

    const QByteArray headByte = QStringLiteral( "$$GI" ).toUtf8();
    qint16 length = static_cast<qint16>( data.length() + 1 );
    QByteArray lengthByte = shortToByteArray( length );
    std::reverse( lengthByte.begin(), lengthByte.end() );

    char startOfData = 0x02;
    int checkCode = 0;
    for ( int i = 0; i < headByte.length(); i++ )
    {
      checkCode ^= static_cast<quint8>( 0xFF & headByte[i] );
    }

    checkCode ^= static_cast<quint8>( 0xFF & lengthByte[0] );
    checkCode ^= static_cast<quint8>( 0xFF & lengthByte[1] );
    checkCode ^= static_cast<quint8>( 0xFF & startOfData );

    for ( int i = 0; i < data.length(); i++ )
    {
      checkCode ^= static_cast<quint8>( 0xFF & data[i] );
    }
    char checkChar = static_cast<char>( checkCode );

    QByteArray packet;
    packet.reserve( headByte.length() + lengthByte.length() + 1 + data.length() + 1 + 2 );

    packet.append( headByte );
    packet.append( lengthByte );
    packet.append( startOfData );
    packet.append( data );
    packet.append( checkChar );
    packet.append( "\r\n" );

    NmeaGnssReceiver::onCorrectionDataReceived( packet );
  }
  else // Generic handling
  {
    NmeaGnssReceiver::onCorrectionDataReceived( data );
  }
}
