

# File positioningsource.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**positioningsource.cpp**](positioningsource_8cpp.md)

[Go to the documentation of this file](positioningsource_8cpp.md)


```C++
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
    disconnect( mReceiver.get(), &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
    disconnect( mReceiver.get(), &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
    mReceiver->deleteLater();
    mReceiver.reset();
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
    else
    {
#ifdef WITH_BLUETOOTH
      mReceiver = std::make_unique<BluetoothReceiver>( mDeviceId, this );
#endif
    }
  }

  // Reset the position information to insure no cross contamination between receiver types
  lastGnssPositionInformationChanged( GnssPositionInformation() );
  connect( mReceiver.get(), &AbstractGnssReceiver::lastGnssPositionInformationChanged, this, &PositioningSource::lastGnssPositionInformationChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::lastErrorChanged, this, &PositioningSource::deviceLastErrorChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::socketStateChanged, this, &PositioningSource::deviceSocketStateChanged );
  connect( mReceiver.get(), &AbstractGnssReceiver::socketStateStringChanged, this, &PositioningSource::deviceSocketStateStringChanged );
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
```


