

# File qfbluetoothlowenergyreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfbluetoothlowenergyreceiver.cpp**](qfbluetoothlowenergyreceiver_8cpp.md)

[Go to the documentation of this file](qfbluetoothlowenergyreceiver_8cpp.md)


```C++
/***************************************************************************
 qfbluetoothlowenergyreceiver.cpp - QfBluetoothLowEnergyReceiver

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

#include "qfbluetoothlowenergyreceiver.h"

#include <QDebug>
#include <QGuiApplication>
#include <QMetaEnum>
#include <QTimer>

// Map of BLE service UUID (key) and a pair of RX (first, incoming) and TX (second, outgoing) characteristics
QMap<QBluetoothUuid, std::pair<QBluetoothUuid, QBluetoothUuid>> QfBluetoothLowEnergyReceiver::serviceChars = {
  { // Standard Nordic UART Service (NUS)
    QBluetoothUuid( "6e400001-b5a3-f393-e0a9-e50e24dcca9e" ),
    std::make_pair( QBluetoothUuid( "6e400003-b5a3-f393-e0a9-e50e24dcca9e" ), QBluetoothUuid( "6e400002-b5a3-f393-e0a9-e50e24dcca9e" ) ) },
  { // Beken Corp.
    QBluetoothUuid( "0000ffe0-0000-1000-8000-00805f9b34fb" ),
    std::make_pair( QBluetoothUuid( "0000ffe1-0000-1000-8000-00805f9b34fb" ), QBluetoothUuid( "0000ffe1-0000-1000-8000-00805f9b34fb" ) ) },
};


QLatin1String QfBluetoothLowEnergyReceiver::identifier = QLatin1String( "ble" );

QfBluetoothLowEnergyReceiver::QfBluetoothLowEnergyReceiver( const QString &address, QObject *parent )
  : QfNmeaGnssReceiver( parent )
  , mAddress( address )
  , mBuffer( new QBuffer( this ) )
{
  qInfo() << "BluetoothLowEnergyReceiver: Creating the receiver";

  mCorrectionTimer.setInterval( 20 );
  connect( &mCorrectionTimer, &QTimer::timeout, this, &QfBluetoothLowEnergyReceiver::forwardCorrectionDataChunk );

  initNmeaConnection( mBuffer );
  setValid( !mAddress.isEmpty() );
}

QfBluetoothLowEnergyReceiver::~QfBluetoothLowEnergyReceiver()
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

QfAbstractGnssReceiver::Capabilities QfBluetoothLowEnergyReceiver::capabilities() const
{
  return QfAbstractGnssReceiver::Capabilities() | QfAbstractGnssReceiver::OrthometricAltitude | QfAbstractGnssReceiver::Logging | QfAbstractGnssReceiver::NtripCorrection;
}

void QfBluetoothLowEnergyReceiver::handleConnectDevice()
{
  if ( mAddress.isEmpty() )
  {
    return;
  }
  qInfo() << "BluetoothLowEnergyReceiver: Initiating connection to device:" << mAddress;

  mConnectionFailureCount = 0;
  mConnectOnDisconnect = true;

  if ( mController && mController->state() != QLowEnergyController::UnconnectedState )
  {
    doDisconnectDevice();
  }
  else
  {
    doConnectDevice();
  }
}

void QfBluetoothLowEnergyReceiver::handleDisconnectDevice()
{
  setSocketState( QAbstractSocket::UnconnectedState );
  mConnectOnDisconnect = false;
  doDisconnectDevice();
}

void QfBluetoothLowEnergyReceiver::doConnectDevice()
{
  if ( !mController )
  {
#if defined( Q_OS_IOS ) || defined( Q_OS_MACOS )
    QBluetoothDeviceInfo deviceInfo( QBluetoothUuid( mAddress ), QString(), 0 );
#else
    QBluetoothDeviceInfo deviceInfo( QBluetoothAddress( mAddress ), QString(), 0 );
#endif
    mController = QLowEnergyController::createCentral( deviceInfo, this );

    connect( mController, &QLowEnergyController::connected, this, &QfBluetoothLowEnergyReceiver::deviceConnected );
    connect( mController, &QLowEnergyController::disconnected, this, &QfBluetoothLowEnergyReceiver::deviceDisconnected );
    connect( mController, qOverload<QLowEnergyController::Error>( &QLowEnergyController::errorOccurred ), this, &QfBluetoothLowEnergyReceiver::controllerErrorOccurred );
    connect( mController, &QLowEnergyController::serviceDiscovered, this, &QfBluetoothLowEnergyReceiver::serviceDiscovered );
    connect( mController, &QLowEnergyController::discoveryFinished, this, &QfBluetoothLowEnergyReceiver::serviceDiscoveryFinished );
  }

  setSocketState( QAbstractSocket::ConnectingState );

  mController->connectToDevice();
}

void QfBluetoothLowEnergyReceiver::doDisconnectDevice()
{
  if ( mController && mController->state() != QLowEnergyController::UnconnectedState )
  {
    qInfo() << "BluetoothLowEnergyReceiver: Disconnecting from device:" << mAddress;

    clearService();

    mDisconnecting = true;
    mLastGnssPositionValid = false;
    mController->disconnectFromDevice();
  }
}

void QfBluetoothLowEnergyReceiver::deviceConnected()
{
  qInfo() << "BluetoothLowEnergyReceiver: Connected, discovering services";
  mController->discoverServices();
}

void QfBluetoothLowEnergyReceiver::deviceDisconnected()
{
  qInfo() << "BluetoothLowEnergyReceiver: Received disconnected signal.";

  if ( mDisconnecting )
  {
    mDisconnecting = false;
    if ( mConnectOnDisconnect )
    {
      doConnectDevice();
    }
  }
}

void QfBluetoothLowEnergyReceiver::controllerErrorOccurred( QLowEnergyController::Error error )
{
  mLastError = QStringLiteral( "Controller Error: %1, %2" ).arg( QMetaEnum::fromType<QLowEnergyController::Error>().valueToKey( static_cast<int>( error ) ), mController->errorString() );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: %1" ).arg( mLastError );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Controller state %1" ).arg( mController->state() );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Connect on disconnect? %1" ).arg( mConnectOnDisconnect ? "true" : "false" );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Disconnecting? %1" ).arg( mDisconnecting ? "true" : "false" );

  if ( mController->state() != QLowEnergyController::ConnectedState )
  {
    qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Handling reconnection" );
    if ( mConnectOnDisconnect )
    {
      mConnectionFailureCount++;
    }

    if ( mConnectionFailureCount > 10 )
    {
      mConnectOnDisconnect = false;
    }

    if ( !mDisconnecting )
    {
      qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Reconnection attempt %1" ).arg( mConnectionFailureCount );
      doDisconnectDevice();
      mDisconnecting = false;
      QTimer::singleShot( 1000, this, [this] { doConnectDevice(); } );
    }

    return;
  }

  emit lastErrorChanged( mLastError );
}

void QfBluetoothLowEnergyReceiver::serviceDiscovered( const QBluetoothUuid &newService )
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

void QfBluetoothLowEnergyReceiver::serviceDiscoveryFinished()
{
  if ( !mController )
  {
    return;
  }

  clearService();

  const QList<QBluetoothUuid> controllerServices = mController->services();
  if ( !controllerServices.isEmpty() )
  {
    const QList<QBluetoothUuid> validServices = serviceChars.keys();
    qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Finding target within %1 services" ).arg( controllerServices.size() );
    auto match = std::find_if( controllerServices.begin(), controllerServices.end(), [&validServices]( const QBluetoothUuid &service ) { return validServices.contains( service ); } );
    if ( match != controllerServices.end() )
    {
      qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: Connecting to target service (%1)" ).arg( match->toString() );
      mService = mController->createServiceObject( *match, this );
    }
  }

  if ( mService )
  {
    connect( mService, &QLowEnergyService::stateChanged, this, &QfBluetoothLowEnergyReceiver::serviceStateChanged );
    connect( mService, qOverload<QLowEnergyService::ServiceError>( &QLowEnergyService::errorOccurred ), this, &QfBluetoothLowEnergyReceiver::serviceErrorOccurred );
    connect( mService, &QLowEnergyService::characteristicChanged, this, &QfBluetoothLowEnergyReceiver::characteristicChanged );

    mService->discoverDetails();
  }
  else
  {
    mLastError = "BluetoothLowEnergyReceiver: Required target service not found on device.";
    qWarning() << mLastError;
    emit lastErrorChanged( mLastError );
    handleDisconnectDevice();
    return;
  }
}

void QfBluetoothLowEnergyReceiver::serviceStateChanged( QLowEnergyService::ServiceState state )
{
  if ( sender() == mService )
  {
    if ( state == QLowEnergyService::RemoteServiceDiscovered )
    {
      mRxCharacteristic = mService->characteristic( serviceChars[mService->serviceUuid()].first );
      mTxCharacteristic = mService->characteristic( serviceChars[mService->serviceUuid()].second );

      if ( mRxCharacteristic.isValid() )
      {
        qInfo() << "BluetoothLowEnergyReceiver: Subscribing to RX characteristic notification";
        QLowEnergyDescriptor notificationDesc = mRxCharacteristic.descriptor( QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration );
        if ( notificationDesc.isValid() )
        {
          mService->writeDescriptor( notificationDesc, QByteArray::fromHex( "0100" ) );
          qInfo() << "BluetoothLowEnergyReceiver: Subscribed to RX characteristic notifications.";
        }

        if ( mTxCharacteristic.isValid() )
        {
          qInfo() << "BluetoothLowEnergyReceiver: Valid TX characteristic.";
        }

        mBufferData.clear();
        mBuffer->open( QIODevice::ReadWrite );

        setSocketState( QAbstractSocket::ConnectedState );
      }
      else
      {
        qWarning() << "BluetoothLowEnergyReceiver: RX Characteristic not found!";
        handleDisconnectDevice();
      }

      mBatteryService = mController->createServiceObject( QBluetoothUuid::ServiceClassUuid::BatteryService, this );

      if ( mBatteryService )
      {
        qInfo() << "BluetoothLowEnergyReceiver: Battery service initiating";
        connect( mBatteryService, &QLowEnergyService::stateChanged, this, &QfBluetoothLowEnergyReceiver::serviceStateChanged );
        connect( mBatteryService, &QLowEnergyService::characteristicChanged, this, &QfBluetoothLowEnergyReceiver::characteristicChanged );

        mBatteryService->discoverDetails();
      }
    }
  }
  else if ( sender() == mBatteryService )
  {
    if ( state == QLowEnergyService::RemoteServiceDiscovered )
    {
      mBatteryCharacteristic = mBatteryService->characteristic( QBluetoothUuid::CharacteristicType::BatteryLevel );

      if ( mBatteryCharacteristic.isValid() )
      {
        qInfo() << "BluetoothLowEnergyReceiver: Subscribing to battery level characteristic notification";
        QLowEnergyDescriptor notificationDesc = mBatteryCharacteristic.descriptor( QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration );
        if ( notificationDesc.isValid() )
        {
          mBatteryService->writeDescriptor( notificationDesc, QByteArray::fromHex( "0100" ) );
          qInfo() << "BluetoothLowEnergyReceiver: Subscribed to battery level characteristic notifications.";
        }

        mBatteryService->readCharacteristic( mBatteryCharacteristic );
      }
    }
  }
}

void QfBluetoothLowEnergyReceiver::serviceErrorOccurred( QLowEnergyService::ServiceError error )
{
  mLastError = QStringLiteral( "Service Error: %1" ).arg( QMetaEnum::fromType<QLowEnergyService::ServiceError>().valueToKey( static_cast<int>( error ) ) );
  qInfo() << QStringLiteral( "BluetoothLowEnergyReceiver: %1" ).arg( mLastError );
  emit lastErrorChanged( mLastError );
}

void QfBluetoothLowEnergyReceiver::characteristicChanged( const QLowEnergyCharacteristic &characteristic, const QByteArray &value )
{
  if ( characteristic.uuid() == mRxCharacteristic.uuid() )
  {
    mBufferData.append( value );
    int endSentenceIndex = mBufferData.lastIndexOf( QLatin1String( "\r\n" ) );
    if ( endSentenceIndex > -1 )
    {
      mBuffer->buffer().clear();
      mBuffer->seek( 0 );
      mBuffer->write( mBufferData.mid( 0, endSentenceIndex + 2 ) );
      mBuffer->seek( 0 );

      mBufferData = mBufferData.mid( endSentenceIndex + 2 );
    }
  }
  else if ( characteristic.uuid() == mBatteryCharacteristic.uuid() )
  {
    if ( !value.isEmpty() )
    {
      const int batteryPercentage = static_cast<quint8>( value[0] );
      mBatteryLevel = batteryPercentage / 100.0;
      emit batteryLevelChanged( mBatteryLevel );
    }
  }
}

void QfBluetoothLowEnergyReceiver::clearService()
{
  if ( mBuffer->isOpen() )
  {
    mBuffer->close();
    mBufferData.clear();
  }

  if ( mService )
  {
    mService->deleteLater();
    mService = nullptr;
  }

  mRxCharacteristic = QLowEnergyCharacteristic();
  mTxCharacteristic = QLowEnergyCharacteristic();

  if ( mBatteryService )
  {
    mBatteryService->deleteLater();
    mBatteryService = nullptr;
  }

  mBatteryCharacteristic = QLowEnergyCharacteristic();
}

QString QfBluetoothLowEnergyReceiver::socketStateString()
{
  const QAbstractSocket::SocketState currentState = socketState();
  QString socketStateString = QfAbstractGnssReceiver::socketStateString();
  if ( currentState == QAbstractSocket::UnconnectedState )
  {
    if ( !mDisconnecting && mController && mController->error() != QLowEnergyController::NoError )
    {
      socketStateString.append( QStringLiteral( ": %1" ).arg( mController->errorString() ) );
    }
  }
  return socketStateString;
}

void QfBluetoothLowEnergyReceiver::onCorrectionDataReceived( const QByteArray &data )
{
  if ( !mService || !mTxCharacteristic.isValid() )
  {
    return;
  }

  mCorrectionData.append( data );

  if ( !mCorrectionTimer.isActive() )
  {
    mCorrectionTimer.start();
  }
}

void QfBluetoothLowEnergyReceiver::forwardCorrectionDataChunk()
{
  if ( mCorrectionData.isEmpty() || !mService || !mTxCharacteristic.isValid() )
  {
    mCorrectionTimer.stop();
    return;
  }

  // Payloag must not be longer than 20 bytes
  // https://doc.qt.io/qt-6/qlowenergyservice.html#WriteMode-enum
  const qsizetype chunkSize = std::min( static_cast<qsizetype>( 20 ), mCorrectionData.size() );
  QByteArray chunk = mCorrectionData.left( chunkSize );
  mCorrectionData.remove( 0, chunkSize );
  mService->writeCharacteristic( mTxCharacteristic, chunk, QLowEnergyService::WriteWithoutResponse );
}
```


