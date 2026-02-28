

# File nmeagnssreceiver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**nmeagnssreceiver.cpp**](nmeagnssreceiver_8cpp.md)

[Go to the documentation of this file](nmeagnssreceiver_8cpp.md)


```C++
/***************************************************************************
 nmeagnssreceiver.cpp - NmeaGnssReceiver

 ---------------------
 begin                : 21.10.2022
 copyright            : (C) 202 by Matthias Kuhn
 email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "nmeagnssreceiver.h"
#include "positioningsource.h"

#include <QFileInfo>
#include <QSettings>

NmeaGnssReceiver::NmeaGnssReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
  , mImuPosition()
{
}

void NmeaGnssReceiver::initNmeaConnection( QIODevice *ioDevice )
{
  mNmeaConnection = std::make_unique<QgsNmeaConnection>( ioDevice );

  //QgsGpsConnection state changed (received location string)
  connect( mNmeaConnection.get(), &QgsGpsConnection::stateChanged, this, &NmeaGnssReceiver::stateChanged );
  connect( mNmeaConnection.get(), &QgsGpsConnection::nmeaSentenceReceived, this, &NmeaGnssReceiver::nmeaSentenceReceived );
}

void NmeaGnssReceiver::stateChanged( const QgsGpsInformation &info )
{
  if ( mLastGnssPositionValid && std::isnan( info.latitude ) )
  {
    return;
  }
  mLastGnssPositionValid = !std::isnan( info.latitude );

  bool ellipsoidalElevation = false;
  double antennaHeight = 0.0;
  if ( const PositioningSource *positioningSource = qobject_cast<PositioningSource *>( parent() ) )
  {
    ellipsoidalElevation = positioningSource->elevationCorrectionMode() != PositioningSource::ElevationCorrectionMode::OrthometricFromDevice;
    antennaHeight = positioningSource->antennaHeight();
  }

  if ( info.utcTime != mLastGnssPositionUtcTime )
  {
    mLastGnssPositionUtcTime = info.utcTime;
    if ( mImuPosition.valid )
    {
      mLastGnssPositionInformation = GnssPositionInformation( mImuPosition.latitude, mImuPosition.longitude,
                                                              ellipsoidalElevation ? mImuPosition.altitude : mImuPosition.altitude - info.elevation_diff,
                                                              mImuPosition.speed * 1000 / 60 / 60, // QgsGpsInformation's speed is served in km/h, translate to m/s
                                                              mImuPosition.direction,
                                                              info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                              info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType,
                                                              info.quality,
                                                              info.satellitesUsed, info.status,
                                                              info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                              0, QStringLiteral( "nmea" ),
                                                              mImuPosition.valid, mImuPosition.roll, mImuPosition.pitch, mImuPosition.heading, mImuPosition.steering );
    }
    else
    {
      mLastGnssPositionInformation = mCurrentNmeaGnssPositionInformation;
    }

    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }

  mCurrentNmeaGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude,
                                                                 info.elevation - antennaHeight + ( ellipsoidalElevation ? info.elevation_diff : 0 ),
                                                                 info.speed * 1000 / 60 / 60, // QgsGpsInformation's speed is served in km/h, translate to m/s
                                                                 info.direction,
                                                                 info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                                 info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality, info.satellitesUsed, info.status,
                                                                 info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                                 0, QStringLiteral( "nmea" ) );
}

void NmeaGnssReceiver::nmeaSentenceReceived( const QString &substring )
{
  if ( mLogFile.isOpen() )
  {
    mLogStream << substring << Qt::endl;
  }

  if ( substring.startsWith( "$INS.NAVI" ) )
  {
    processImuSentence( substring );
  }
}

void NmeaGnssReceiver::handleStartLogging( const QString &path )
{
  if ( QFileInfo::exists( path ) )
  {
    if ( mLogFile.isOpen() )
      handleStopLogging();

    mLogFile.setFileName( QStringLiteral( "%1/nmea-%2.log" ).arg( path, QDateTime::currentDateTime().toString( QStringLiteral( "yyyy-MM-ddThh:mm:ss" ) ) ) );
    mLogFile.open( QIODevice::WriteOnly );
    mLogStream.setDevice( &mLogFile );
  }
}

void NmeaGnssReceiver::handleStopLogging()
{
  mLogFile.close();
}

GnssPositionDetails NmeaGnssReceiver::details() const
{
  GnssPositionDetails dataList;
  dataList.append( "PDOP", QLocale::system().toString( mLastGnssPositionInformation.pdop(), 'f', 1 ) );
  dataList.append( "HDOP", QLocale::system().toString( mLastGnssPositionInformation.hdop(), 'f', 1 ) );
  dataList.append( "VDOP", QLocale::system().toString( mLastGnssPositionInformation.vdop(), 'f', 1 ) );
  dataList.append( "Valid", mLastGnssPositionInformation.isValid() ? "True" : "False" );
  dataList.append( "Fix", mLastGnssPositionInformation.fixStatusDescription() );
  dataList.append( "Quality", mLastGnssPositionInformation.qualityDescription() );
  return dataList;
}

void NmeaGnssReceiver::processImuSentence( const QString &sentence )
{
  static const int PARAMETER_STATUS_INDEX = 19;

  static const int IMU_KQGEO_STATUS_OK = 1026;
  static const int IMU_KQGEO_STATUS_OK_NEW = 1967106;

  // Remove checksum
  const QString sentenceWithoutChecksum = sentence.split( '*' ).first();
  QStringList parameters = sentenceWithoutChecksum.split( ',' );
  if ( parameters.size() <= PARAMETER_STATUS_INDEX )
    return;

  // Parse Status
  bool ok = false;
  const int status = parameters[PARAMETER_STATUS_INDEX].toInt( &ok );
  if ( ok == false )
  {
    mImuPosition.valid = false;
    return;
  }

  if ( status != IMU_KQGEO_STATUS_OK && status != IMU_KQGEO_STATUS_OK_NEW )
  {
    mImuPosition.valid = false;
    return;
  }

  // Parse other parameters
  mImuPosition.utcDateTime = QDateTime::currentDateTime();
  QTime time = QTime::fromString( parameters[1], "hhmmss.zzz" );
  if ( time.isValid() )
    mImuPosition.utcDateTime.setTime( time );

  bool latitudeOk;
  mImuPosition.latitude = parameters[2].toDouble( &latitudeOk );

  bool longitudeOk;
  mImuPosition.longitude = parameters[3].toDouble( &longitudeOk );

  bool altitudeOk;
  mImuPosition.altitude = parameters[4].toDouble( &altitudeOk );

  if ( !latitudeOk || !longitudeOk || !altitudeOk )
  {
    mImuPosition.valid = false;
    return;
  }

  double speedNorth = parameters[5].toDouble();
  double speedEast = parameters[6].toDouble();
  mImuPosition.speed = sqrt( speedNorth * speedNorth + speedEast * speedEast );
  mImuPosition.speedDown = parameters[7].toDouble();
  mImuPosition.direction = 0.0;
  if ( speedEast != 0.0 )
    mImuPosition.direction = atan( speedNorth / speedEast );
  else if ( speedNorth > 0.0 )
    mImuPosition.direction = M_PI_2;
  else if ( speedNorth < 0.0 )
    mImuPosition.direction = -M_PI_2;

  mImuPosition.roll = parameters[8].toDouble();
  mImuPosition.pitch = parameters[9].toDouble();
  mImuPosition.heading = parameters[10].toDouble();
  mImuPosition.steering = parameters[11].toDouble();
  mImuPosition.accelerometerX = parameters[12].toDouble();
  mImuPosition.accelerometerY = parameters[13].toDouble();
  mImuPosition.accelerometerZ = parameters[14].toDouble();
  mImuPosition.gyroX = parameters[15].toDouble();
  mImuPosition.gyroY = parameters[16].toDouble();
  mImuPosition.gyroZ = parameters[17].toDouble();
  mImuPosition.steeringZ = parameters[18].toDouble();

  mImuPosition.valid = true;
}
```


