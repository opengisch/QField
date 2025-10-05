/***************************************************************************
  positioningsource.cpp - PositioningSource

 ---------------------
 begin                : 20.12.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef WITH_BLUETOOTH
#include "bluetoothreceiver.h"
#endif
#ifdef WITH_SERIALPORT
#include "serialportreceiver.h"
#endif
#include "egenioussreceiver.h"
#include "filereceiver.h"
#include "internalgnssreceiver.h"
#include "ntripclient.h"
#include "positioningsource.h"
#include "positioningutils.h"
#include "tcpreceiver.h"
#include "udpreceiver.h"

#include <QStandardPaths>

QString PositioningSource::backgroundFilePath = QStringLiteral( "%1/positioning.background" ).arg( QStandardPaths::writableLocation( QStandardPaths::AppDataLocation ) );

PositioningSource::PositioningSource( QObject *parent )
  : QObject( parent )
{
  // Setup internal gnss receiver by default
  setupDevice();

  // Setup the compass, use a timer instead of the compass's readingChanged signal to avoid handling
  // too many signals
  mCompassTimer.setInterval( 200 );
  connect( &mCompassTimer, &QTimer::timeout, this, &PositioningSource::processCompassReading );
}

void PositioningSource::setActive( bool active )
{
  if ( mActive == active )
    return;

  mActive = active;

  if ( mActive )
  {
    if ( !mReceiver )
    {
      setupDevice();
    }
    else
    {
      mReceiver->connectDevice();
    }
    if ( !QSensor::sensorsForType( QCompass::sensorType ).isEmpty() )
    {
      mCompass.setActive( true );
      mCompassTimer.start();
    }
  }
  else
  {
    if ( mReceiver )
    {
      mReceiver->disconnectDevice();
    }
    mCompassTimer.stop();
    mCompass.setActive( false );
    mOrientation = std::numeric_limits<double>::quiet_NaN();
    emit orientationChanged();
  }

  emit activeChanged();
}

void PositioningSource::setDeviceId( const QString &id )
{
  if ( mDeviceId == id )
    return;

  mDeviceId = id;
  setupDevice();

  emit deviceIdChanged();
}

void PositioningSource::setValid( bool valid )
{
  if ( mValid == valid )
    return;

  mValid = valid;

  emit validChanged();
}

void PositioningSource::setLogging( bool logging )
{
  if ( mLogging == logging )
    return;

  mLogging = logging;

  if ( mReceiver )
  {
    if ( mLogging && !mLoggingPath.isEmpty() )
    {
      mReceiver->startLogging( mLoggingPath );
    }
    else
    {
      mReceiver->stopLogging();
    }
  }

  emit loggingChanged();
}

void PositioningSource::setLoggingPath( const QString &path )
{
  if ( mLoggingPath == path )
    return;

  mLoggingPath = path;

  if ( mReceiver && mLogging )
  {
    mReceiver->startLogging( mLoggingPath );
  }

  emit loggingPathChanged();
}

void PositioningSource::setBackgroundMode( bool backgroundMode )
{
  if ( mBackgroundMode == backgroundMode )
    return;

  mBackgroundMode = backgroundMode;

  if ( mBackgroundMode )
  {
    if ( QFile::exists( QStringLiteral( "%1.information" ).arg( backgroundFilePath ) ) )
    {
      // Remove previously collected position information
      QFile::remove( QStringLiteral( "%1.information" ).arg( backgroundFilePath ) );
    }
  }

  emit backgroundModeChanged();
}

void PositioningSource::setEnableNtripClient( bool enableNtripClient )
{
  if ( mEnableNtripClient == enableNtripClient )
    return;

  mEnableNtripClient = enableNtripClient;

  // Start or stop NTRIP client based on the setting
  if ( mEnableNtripClient )
  {
    startNtripClient();
  }
  else
  {
    stopNtripClient();
  }

  emit enableNtripClientChanged();
}

void PositioningSource::setNtripHost( const QString &ntripHost )
{
  if ( mNtripHost == ntripHost )
    return;

  mNtripHost = ntripHost;

  // Restart NTRIP client if enabled and parameters changed
  if ( mEnableNtripClient && mNtripClient )
  {
    stopNtripClient();
    startNtripClient();
  }

  emit ntripHostChanged();
}

void PositioningSource::setNtripPort( int ntripPort )
{
  if ( mNtripPort == ntripPort )
    return;

  mNtripPort = ntripPort;

  // Restart NTRIP client if enabled and parameters changed
  if ( mEnableNtripClient && mNtripClient )
  {
    stopNtripClient();
    startNtripClient();
  }

  emit ntripPortChanged();
}

void PositioningSource::setNtripMountpoint( const QString &ntripMountpoint )
{
  if ( mNtripMountpoint == ntripMountpoint )
    return;

  mNtripMountpoint = ntripMountpoint;

  // Restart NTRIP client if enabled and parameters changed
  if ( mEnableNtripClient && mNtripClient )
  {
    stopNtripClient();
    startNtripClient();
  }

  emit ntripMountpointChanged();
}

void PositioningSource::setNtripUsername( const QString &ntripUsername )
{
  if ( mNtripUsername == ntripUsername )
    return;

  mNtripUsername = ntripUsername;

  // Restart NTRIP client if enabled and parameters changed
  if ( mEnableNtripClient && mNtripClient )
  {
    stopNtripClient();
    startNtripClient();
  }

  emit ntripUsernameChanged();
}

void PositioningSource::setNtripPassword( const QString &ntripPassword )
{
  if ( mNtripPassword == ntripPassword )
    return;

  mNtripPassword = ntripPassword;

  // Restart NTRIP client if enabled and parameters changed
  if ( mEnableNtripClient && mNtripClient )
  {
    stopNtripClient();
    startNtripClient();
  }

  emit ntripPasswordChanged();
}

QList<GnssPositionInformation> PositioningSource::getBackgroundPositionInformation() const
{
  QList<GnssPositionInformation> positionInformationList;

  QFile file( QStringLiteral( "%1.information" ).arg( backgroundFilePath ) );
  if ( file.exists() )
  {
    file.open( QFile::ReadOnly );
    QDataStream stream( &file );
    while ( !stream.atEnd() )
    {
      GnssPositionInformation positionInformation;
      stream >> positionInformation;
      positionInformationList << positionInformation;
    }
    file.close();
  }

  return positionInformationList;
}

void PositioningSource::setElevationCorrectionMode( ElevationCorrectionMode elevationCorrectionMode )
{
  if ( mElevationCorrectionMode == elevationCorrectionMode )
    return;

  mElevationCorrectionMode = elevationCorrectionMode;

  emit elevationCorrectionModeChanged();
}

void PositioningSource::setAntennaHeight( double antennaHeight )
{
  if ( mAntennaHeight == antennaHeight )
    return;

  mAntennaHeight = antennaHeight;

  emit antennaHeightChanged();
}

void PositioningSource::setupDevice()
{
  if ( mReceiver )
  {
    mReceiver->disconnectDevice();
    mReceiver->stopLogging();
    disconnect( mReceiver, &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
    disconnect( mReceiver, &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
    disconnect( mReceiver, &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
    disconnect( mReceiver, &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::onDeviceSocketStateChanged );
    disconnect( mReceiver, &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
    mReceiver->deleteLater();
    mReceiver = nullptr;

    // Stop NTRIP client when receiver is being replaced
    stopNtripClient();
  }

  if ( mDeviceId.isEmpty() )
  {
    // Using internal receiver - NTRIP client not needed
    mReceiver = new InternalGnssReceiver( this );
  }
  else
  {
    if ( mDeviceId.startsWith( FileReceiver::identifier + ":" ) )
    {
      const int prefixLength = FileReceiver::identifier.length() + 1;
      const int intervalSeparator = mDeviceId.lastIndexOf( ':' );
      const QString filePath = mDeviceId.mid( prefixLength, intervalSeparator - prefixLength );
      const int interval = mDeviceId.mid( intervalSeparator + 1 ).toInt();
      mReceiver = new FileReceiver( filePath, interval, this );
    }
    else if ( mDeviceId.startsWith( TcpReceiver::identifier + ":" ) )
    {
      const int prefixLength = TcpReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = new TcpReceiver( address, port, this );
    }
    else if ( mDeviceId.startsWith( UdpReceiver::identifier + ":" ) )
    {
      const int prefixLength = UdpReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = new UdpReceiver( address, port, this );
    }
    else if ( mDeviceId.startsWith( EgenioussReceiver::identifier + ":" ) )
    {
      const int prefixLength = EgenioussReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = new EgenioussReceiver( address, port, this );
    }
#ifdef WITH_SERIALPORT
    else if ( mDeviceId.startsWith( SerialPortReceiver::identifier + ":" ) )
    {
      const int prefixLength = SerialPortReceiver::identifier.length() + 1;
      const QString address = mDeviceId.mid( prefixLength );
      mReceiver = new SerialPortReceiver( address, this );
    }
#endif
    else
    {
#ifdef WITH_BLUETOOTH
      mReceiver = new BluetoothReceiver( mDeviceId, this );

      // Start NTRIP client if enabled for Bluetooth receivers
      if ( mEnableNtripClient )
      {
        startNtripClient();
      }
#endif
    }
  }

  // Reset the position information to insure no cross contamination between receiver types
  lastGnssPositionInformationChanged( GnssPositionInformation() );
  connect( mReceiver, &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
  connect( mReceiver, &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
  connect( mReceiver, &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
  connect( mReceiver, &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::onDeviceSocketStateChanged );
  connect( mReceiver, &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
  setValid( mReceiver->valid() );

  emit deviceChanged();

  if ( mLogging && !mLoggingPath.isEmpty() )
  {
    mReceiver->startLogging( mLoggingPath );
  }

  if ( mActive )
  {
    mReceiver->connectDevice();
  }

  return;
}

void PositioningSource::lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation )
{
  if ( mPositionInformation == lastGnssPositionInformation )
    return;

  const GnssPositionInformation positionInformation( lastGnssPositionInformation.latitude(),
                                                     lastGnssPositionInformation.longitude(),
                                                     lastGnssPositionInformation.elevation(),
                                                     lastGnssPositionInformation.speed(),
                                                     lastGnssPositionInformation.direction(),
                                                     lastGnssPositionInformation.satellitesInView(),
                                                     lastGnssPositionInformation.pdop(),
                                                     lastGnssPositionInformation.hdop(),
                                                     lastGnssPositionInformation.vdop(),
                                                     lastGnssPositionInformation.hacc(),
                                                     lastGnssPositionInformation.vacc(),
                                                     lastGnssPositionInformation.utcDateTime().isValid() ? lastGnssPositionInformation.utcDateTime() : QDateTime::currentDateTimeUtc(),
                                                     lastGnssPositionInformation.fixMode(),
                                                     lastGnssPositionInformation.fixType(),
                                                     lastGnssPositionInformation.quality(),
                                                     lastGnssPositionInformation.satellitesUsed(),
                                                     lastGnssPositionInformation.status(),
                                                     lastGnssPositionInformation.satPrn(),
                                                     lastGnssPositionInformation.satInfoComplete(),
                                                     lastGnssPositionInformation.verticalSpeed(),
                                                     lastGnssPositionInformation.magneticVariation(),
                                                     lastGnssPositionInformation.averagedCount(),
                                                     lastGnssPositionInformation.sourceName(),
                                                     lastGnssPositionInformation.imuCorrection(),
                                                     lastGnssPositionInformation.imuRoll(),
                                                     lastGnssPositionInformation.imuPitch(),
                                                     lastGnssPositionInformation.imuHeading(),
                                                     lastGnssPositionInformation.imuSteering(),
                                                     mOrientation );
  mPositionInformation = positionInformation;

  if ( !mBackgroundMode )
  {
    emit positionInformationChanged();
  }
  else
  {
    QFile file( QStringLiteral( "%1.information" ).arg( backgroundFilePath ) );
    file.open( QFile::Append );
    QDataStream stream( &file );
    stream << mPositionInformation;
    file.close();
  }
}

void PositioningSource::processCompassReading()
{
  if ( mCompass.reading() )
  {
    double orientation = 0.0;
    orientation += mCompass.reading()->azimuth();
    if ( orientation < 0.0 )
    {
      orientation = 360 + orientation;
    }

    if ( mOrientation != orientation )
    {
      mOrientation = orientation;
      if ( !mBackgroundMode )
      {
        emit orientationChanged();
      }
    }
  }
}

void PositioningSource::onDeviceSocketStateChanged()
{
  if ( mReceiver )
  {
    QAbstractSocket::SocketState state = mReceiver->socketState();

    // Stop NTRIP client when receiver is disconnected or has connection error
    if ( mNtripClient && ( state == QAbstractSocket::UnconnectedState || state == QAbstractSocket::ClosingState ) )
    {
      stopNtripClient();
    }
    // Start NTRIP client when external receiver connects and setting is enabled
    else if ( !mNtripClient && mEnableNtripClient && !mDeviceId.isEmpty() && state == QAbstractSocket::ConnectedState )
    {
      startNtripClient();
    }
  }
}

void PositioningSource::triggerConnectDevice()
{
  if ( mReceiver )
  {
    mReceiver->connectDevice();
  }
}

void PositioningSource::triggerDisconnectDevice()
{
  if ( mReceiver )
  {
    mReceiver->disconnectDevice();
  }
}

void PositioningSource::startNtripClient()
{
  // Only start NTRIP client if we have an external receiver that can use RTK corrections
  if ( mReceiver && !mDeviceId.isEmpty() )
  {
    // Check that all required NTRIP parameters are configured
    if ( mNtripHost.isEmpty() || mNtripMountpoint.isEmpty() || mNtripUsername.isEmpty() || mNtripPassword.isEmpty() )
    {
      setNtripStatus( QStringLiteral( "Missing parameters" ) );
      qWarning() << "NTRIP Client: Missing required connection parameters (host, mountpoint, username, or password)";
      return;
    }

    if ( !mNtripClient )
      mNtripClient = std::make_unique<NtripClient>( this );

    // Reset byte counters
    mNtripBytesSent = 0;
    mNtripBytesReceived = 0;
    emit ntripBytesSentChanged();
    emit ntripBytesReceivedChanged();

    setNtripStatus( QStringLiteral( "Connecting..." ) );
    mNtripClient->start( mNtripHost, static_cast<quint16>( mNtripPort ), mNtripMountpoint, mNtripUsername, mNtripPassword );

    // Connect to receiver if it supports RTK corrections
#ifdef WITH_BLUETOOTH
    if ( auto bluetoothReceiver = dynamic_cast<BluetoothReceiver *>( mReceiver ) )
    {
      connect( mNtripClient.get(), &NtripClient::correctionDataReceived, bluetoothReceiver, &BluetoothReceiver::onCorrectionDataReceived );
    }
#endif

    // Track connection status through signals
    connect( mNtripClient.get(), &NtripClient::streamConnected,
             this, [this]() {
               setNtripStatus( QStringLiteral( "Connected" ) );
             } );

    connect( mNtripClient.get(), &NtripClient::errorOccurred,
             this, [this]( const QString &msg ) {
               setNtripStatus( QStringLiteral( "Error: %1" ).arg( msg ) );
               qWarning() << "NTRIP Client Error:" << msg;
             } );

    // Track byte counters
    connect( mNtripClient.get(), &NtripClient::bytesCountersChanged,
             this, [this]() {
               mNtripBytesSent = mNtripClient->bytesSent();
               mNtripBytesReceived = mNtripClient->bytesReceived();
               emit ntripBytesSentChanged();
               emit ntripBytesReceivedChanged();
             } );
  }
  else
  {
    setNtripStatus( QStringLiteral( "No external receiver" ) );
  }
}

void PositioningSource::stopNtripClient()
{
  if ( mNtripClient )
  {
    mNtripClient->stop();
    mNtripClient.reset();
  }
  setNtripStatus( QStringLiteral( "Disconnected" ) );
}

void PositioningSource::setNtripStatus( const QString &status )
{
  if ( mNtripStatus == status )
    return;

  mNtripStatus = status;
  emit ntripStatusChanged();
}
