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
}

void NmeaGnssReceiver::stateChanged( const QgsGpsInformation &info )
{
  if ( mLastGnssPositionValid && std::isnan( info.latitude )               // we already sent a valid position
       || ( info.utcDateTime == mLastGnssPositionInformation.utcDateTime() // we group updates by timestamp when available, if the last block is not finished, return
            && info.utcDateTime.isValid() ) )
  {
    return;
  }
  mLastGnssPositionValid = !std::isnan( info.latitude );
  emit lastGnssPositionInformationChanged( mLastGnssPositionInformation );

  bool ellipsoidalElevation = false;
  if ( Positioning *positioning = qobject_cast<Positioning *>( parent() ) )
  {
    ellipsoidalElevation = positioning->ellipsoidalElevation();
  }

  // QgsGpsInformation's speed is served in km/h, translate to m/s
  mLastGnssPositionInformation = GnssPositionInformation( info.latitude, info.longitude, ellipsoidalElevation ? info.elevation + info.elevation_diff : info.elevation,
                                                          info.speed * 1000 / 60 / 60, info.direction, info.satellitesInView, info.pdop, info.hdop, info.vdop,
                                                          info.hacc, info.vacc, info.utcDateTime, info.fixMode, info.fixType, info.quality, info.satellitesUsed, info.status,
                                                          info.satPrn, info.satInfoComplete, std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(),
                                                          0, QStringLiteral( "nmea" ) );
}
