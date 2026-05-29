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
#include "bluetoothlowenergyreceiver.h"
#include "bluetoothreceiver.h"
#endif
#ifdef WITH_SERIALPORT
#include "serialportreceiver.h"
#endif
#include "egenioussreceiver.h"
#include "filereceiver.h"
#include "internalgnssreceiver.h"
#include "nmeagnssreceiver.h"
#include "ntripclient.h"
#include "positioningsource.h"
#include "positioningutils.h"
#include "tcpreceiver.h"
#include "udpreceiver.h"

#include <QDateTime>
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
      triggerConnectDevice();
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
      if ( mNtripClient )
      {
        mNtripClient->startLogging( mLoggingPath );
      }
    }
    else
    {
      mReceiver->stopLogging();
      if ( mNtripClient )
      {
        mNtripClient->stopLogging();
      }
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
    if ( mNtripClient )
    {
      mNtripClient->startLogging( mLoggingPath );
    }
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


void PositioningSource::setEnableNtrip( bool enableNtrip )
{
  if ( mEnableNtrip == enableNtrip )
  {
    return;
  }

  mEnableNtrip = enableNtrip;

  if ( mEnableNtrip )
  {
    startNtripClient();
  }
  else
  {
    stopNtripClient();
  }

  emit enableNtripChanged();
}

void PositioningSource::setNtripSettings( const NtripSettings &ntripSettings )
{
  if ( mNtripSettings == ntripSettings )
  {
    return;
  }

  mNtripSettings = ntripSettings;

  if ( mEnableNtrip )
  {
    startNtripClient();
  }

  emit ntripSettingsChanged();
}

QList<GnssPositionInformation> PositioningSource::getBackgroundPositionInformation() const
{
  QList<GnssPositionInformation> positionInformationList;

  QFile file( QStringLiteral( "%1.information" ).arg( backgroundFilePath ) );
  if ( file.exists() )
  {
    if ( file.open( QFile::ReadOnly ) )
    {
      QDataStream stream( &file );
      while ( !stream.atEnd() )
      {
        GnssPositionInformation positionInformation;
        stream >> positionInformation;
        positionInformationList << positionInformation;
      }
      file.close();
    }
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
    triggerDisconnectDevice();
    mReceiver->stopLogging();

    disconnect( mReceiver.get(), &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::onDeviceSocketStateChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::batteryLevelChanged, this, &PositioningSource::deviceBatteryLevelChanged );

    mReceiver->deleteLater();
    mReceiver.reset();
    stopNtripClient();
  }

  if ( mDeviceId.isEmpty() )
  {
    mReceiver = std::make_unique<InternalGnssReceiver>( this );
  }
  else
  {
    if ( mDeviceId.startsWith( FileReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = FileReceiver::identifier.length() + 1;
      const qsizetype intervalSeparator = mDeviceId.lastIndexOf( ':' );
      const QString filePath = mDeviceId.mid( prefixLength, intervalSeparator - prefixLength );
      const int interval = mDeviceId.mid( intervalSeparator + 1 ).toInt();
      mReceiver = std::make_unique<FileReceiver>( filePath, interval, this );
    }
    else if ( mDeviceId.startsWith( TcpReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = TcpReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = std::make_unique<TcpReceiver>( address, port, this );
    }
    else if ( mDeviceId.startsWith( UdpReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = UdpReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = std::make_unique<UdpReceiver>( address, port, this );
    }
    else if ( mDeviceId.startsWith( EgenioussReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = EgenioussReceiver::identifier.length() + 1;
      const qsizetype portSeparator = mDeviceId.lastIndexOf( ':' );
      const QString address = mDeviceId.mid( prefixLength, portSeparator - prefixLength );
      const int port = mDeviceId.mid( portSeparator + 1 ).toInt();
      mReceiver = std::make_unique<EgenioussReceiver>( address, port, this );
    }
#ifdef WITH_SERIALPORT
    else if ( mDeviceId.startsWith( SerialPortReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = SerialPortReceiver::identifier.length() + 1;
      const QString address = mDeviceId.mid( prefixLength );
      mReceiver = std::make_unique<SerialPortReceiver>( address, this );
    }
#endif
#ifdef WITH_BLUETOOTH
    else if ( mDeviceId.startsWith( BluetoothLowEnergyReceiver::identifier + ":" ) )
    {
      const qsizetype prefixLength = BluetoothLowEnergyReceiver::identifier.length() + 1;
      const QString address = mDeviceId.mid( prefixLength );
      mReceiver = std::make_unique<BluetoothLowEnergyReceiver>( address, this );
    }
    else
    {
      mReceiver = std::make_unique<BluetoothReceiver>( mDeviceId, this );
    }
#endif
  }

  // Reset the position information to insure no cross contamination between receiver types
  lastGnssPositionInformationChanged( GnssPositionInformation() );
  connect( mReceiver.get(), &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::onDeviceSocketStateChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::batteryLevelChanged, this, &PositioningSource::deviceBatteryLevelChanged );

  setValid( mReceiver->valid() );

  emit deviceChanged();
  emit deviceBatteryLevelChanged();

  if ( mLogging && !mLoggingPath.isEmpty() )
  {
    mReceiver->startLogging( mLoggingPath );
  }

  if ( mActive )
  {
    triggerConnectDevice();
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
    if ( file.open( QFile::Append ) )
    {
      QDataStream stream( &file );
      stream << mPositionInformation;
      file.close();
    }
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
    else if ( !mNtripClient && mEnableNtrip && !mDeviceId.isEmpty() && state == QAbstractSocket::ConnectedState )
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

    if ( mEnableNtrip )
    {
      startNtripClient();
    }
  }
}

void PositioningSource::triggerDisconnectDevice()
{
  if ( mReceiver )
  {
    mReceiver->disconnectDevice();
    stopNtripClient();
  }
}

void PositioningSource::startNtripClient()
{
  if ( !mNtripSettings.isValid() )
  {
    return;
  }

  if ( !mReceiver || !( mReceiver->capabilities() & AbstractGnssReceiver::NtripCorrection ) )
  {
    return;
  }

  if ( !mNtripClient )
  {
    mNtripClient = std::make_unique<NtripClient>( this );

    connect( mNtripClient.get(), &NtripClient::streamConnected, this, [this]() {
      setNtripState( NtripState::Connected );
      setNtripLastError( QString() );
    } );

    connect( mNtripClient.get(), &NtripClient::streamDisconnected, this, [this]() {
      setNtripState( NtripState::Disconnected );
    } );

    connect( mNtripClient.get(), &NtripClient::errorOccurred, this, [this]( const QString &msg ) {
      setNtripLastError( msg );
      qInfo() << "NTRIP Client Error:" << msg;
    } );

    connect( mNtripClient.get(), &NtripClient::bytesSentChanged, this, [this]() {
      mNtripBytesSent = mNtripClient->bytesSent();
      emit ntripBytesSentChanged();
    } );

    connect( mNtripClient.get(), &NtripClient::bytesReceivedChanged, this, [this]() {
      mNtripBytesReceived = mNtripClient->bytesReceived();
      mNtripLastBytesReceivedUtcDateTime = QDateTime::currentDateTimeUtc();
      emit ntripBytesReceivedChanged();
      emit ntripLastBytesReceivedUtcDateTimeChanged();
    } );
  }

  mNtripBytesSent = 0;
  mNtripBytesReceived = 0;
  mNtripLastBytesReceivedUtcDateTime = QDateTime();
  emit ntripBytesSentChanged();
  emit ntripBytesReceivedChanged();
  emit ntripLastBytesReceivedUtcDateTimeChanged();
  setNtripState( NtripState::Disconnected );

  if ( mLogging )
  {
    mNtripClient->startLogging( mLoggingPath );
  }

  mNtripClient->start( mNtripSettings, mReceiver.get() );
  setNtripState( NtripState::Connecting );
}

void PositioningSource::stopNtripClient()
{
  if ( mNtripClient )
  {
    mNtripClient->stop();
    if ( mLogging )
    {
      mNtripClient->stopLogging();
    }
    mNtripClient.reset();

    setNtripState( NtripState::Disconnected );
    setNtripLastError( QString() );
  }
}

void PositioningSource::setNtripState( NtripState state )
{
  if ( mNtripState == state )
  {
    return;
  }

  mNtripState = state;
  emit ntripStateChanged();
}

void PositioningSource::setNtripLastError( const QString &error )
{
  if ( mNtripLastError == error )
  {
    return;
  }

  mNtripLastError = error;
  emit ntripLastErrorChanged();
}

int PositioningSource::deviceCapabilities() const
{
  return mReceiver ? mReceiver->capabilities() : AbstractGnssReceiver::NoCapabilities;
}
