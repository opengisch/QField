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
#include "platformutilities.h"
#include "positioning.h"

#include <QSettings>

NmeaGnssReceiver::NmeaGnssReceiver( QObject *parent )
  : AbstractGnssReceiver( parent )
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

  if ( info.utcTime != mLastGnssPositionUtcTime )
  {
    mLastGnssPositionUtcTime = info.utcTime;

    if ( mIMUCorrectedPosition.valid )
    {
      mLastGnssPositionInformation = GnssPositionInformation( mIMUCorrectedPosition.latitude, mIMUCorrectedPosition.longitude,
                                                              mIMUCorrectedPosition.altitude,
                                                              mIMUCorrectedPosition.speed * 1000 / 60 / 60, mIMUCorrectedPosition.direction,
                                                              info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                              info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality, info.satellitesUsed, info.status,
                                                              info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                              0, QStringLiteral( "nmea" ),
                                                              mIMUCorrectedPosition.valid );
    }
    else
      mLastGnssPositionInformation = mCurrentNmeaGnssPositionInformation;

    emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );
  }

  bool ellipsoidalElevation = false;
  if ( Positioning *positioning = qobject_cast<Positioning *>( parent() ) )
  {
    ellipsoidalElevation = positioning->ellipsoidalElevation();
  }

  // QgsGpsInformation's speed is served in km/h, translate to m/s
  mCurrentNmeaGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude, ellipsoidalElevation ? info.elevation + info.elevation_diff : info.elevation,
                                                                 info.speed * 1000 / 60 / 60, info.direction, info.satellitesInView, info.pdop, info.hdop, info.vdop,
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
    handleIMUCorrection( substring );
  }
}

void NmeaGnssReceiver::handleStartLogging()
{
  const QStringList appDataDirs = PlatformUtilities::instance()->appDataDirs();
  if ( !appDataDirs.isEmpty() )
  {
    mLogFile.setFileName( QStringLiteral( "%1/logs/nmea-%2.log" ).arg( appDataDirs.at( 0 ), QDateTime::currentDateTime().toString( QStringLiteral( "yyyy-MM-ddThh:mm:ss" ) ) ) );
    mLogFile.open( QIODevice::WriteOnly );
    mLogStream.setDevice( &mLogFile );
  }
}

void NmeaGnssReceiver::handleStopLogging()
{
  mLogFile.close();
}

void NmeaGnssReceiver::handleIMUCorrection( const QString &sentence )
{
  static const int PARAMETER_STATUS = 19;

  static const int IMU_KQGEO_STATUS_OK = 1026;
  static const int IMU_KQGEO_STATUS_OK_NEW = 1967106;

  QStringList parameters = sentence.split( ',' );
  if ( parameters.size() < PARAMETER_STATUS )
    return;

  // Parse Status
  bool ok = false;
  const int status = parameters[PARAMETER_STATUS].toInt( &ok );
  if ( ok == false )
  {
    mIMUCorrectedPosition.valid = false;
    return;
  }

  if ( status != IMU_KQGEO_STATUS_OK && status != IMU_KQGEO_STATUS_OK_NEW )
  {
    mIMUCorrectedPosition.valid = false;
    return;
  }

  mIMUCorrectedPosition.valid = true;

  // Parse other parameters

  QDateTime utcDateTime = QDateTime::currentDateTime();
  QTime time = QTime::fromString( parameters[1], "hhmmss.zzz" );
  if ( time.isValid() )
    utcDateTime.setTime( time );

  mIMUCorrectedPosition.latitude = parameters[2].toDouble();
  mIMUCorrectedPosition.longitude = parameters[3].toDouble();
  mIMUCorrectedPosition.altitude = parameters[4].toDouble();

  double speedNorth = parameters[5].toDouble();
  double speedEast = parameters[6].toDouble();
  mIMUCorrectedPosition.speed = sqrt( speedNorth * speedNorth + speedEast * speedEast );
  mIMUCorrectedPosition.speedDown = parameters[7].toDouble();
  mIMUCorrectedPosition.direction = atan( speedNorth / speedEast );

  mIMUCorrectedPosition.roll = parameters[8].toDouble();
  mIMUCorrectedPosition.pitch = parameters[9].toDouble();
  mIMUCorrectedPosition.heading = parameters[10].toDouble();
  mIMUCorrectedPosition.steering = parameters[11].toDouble();
  mIMUCorrectedPosition.accelerometerX = parameters[12].toDouble();
  mIMUCorrectedPosition.accelerometerY = parameters[13].toDouble();
  mIMUCorrectedPosition.accelerometerZ = parameters[14].toDouble();
  mIMUCorrectedPosition.gyroX = parameters[15].toDouble();
  mIMUCorrectedPosition.gyroY = parameters[16].toDouble();
  mIMUCorrectedPosition.gyroZ = parameters[17].toDouble();
  mIMUCorrectedPosition.steeringZ = parameters[18].toDouble();
}
