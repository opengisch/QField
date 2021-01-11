/***************************************************************************
  gnsspositioninformation.cpp - GnssPositionInformation
 ---------------------
 begin                : 1.12.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gnsspositioninformation.h"

#include <QCoreApplication>
#include <QTime>
#include <QIODevice>
#include <QStringList>
#include <QFileInfo>

#include "qgsnmeaconnection.h"
#include "qgslogger.h"


GnssPositionInformation::GnssPositionInformation( double latitude, double longitude, double elevation, double speed, double direction, const QList<QgsSatelliteInfo> &satellitesInView, double pdop, double hdop, double vdop, double hacc, double vacc,
    QDateTime utcDateTime, QChar fixMode, int fixType, int quality, int satellitesUsed, QChar status, const QList<int> &satPrn, bool satInfoComplete, double verticalSpeed, double magneticVariation, const QString &sourceName )
  : mLatitude( latitude ),
    mLongitude( longitude ),
    mElevation( elevation ),
    mSpeed( speed ),
    mDirection( direction ),
    mSatellitesInView( satellitesInView ),
    mPdop( pdop ),
    mHdop( hdop ),
    mVdop( vdop ),
    mHacc( hacc ),
    mVacc( vacc ),
    mHvacc( sqrt( ( pow( hacc, 2 ) + pow( hacc, 2 ) + pow( vacc, 2 ) ) / 3 ) ),
    mUtcDateTime( utcDateTime ),
    mFixMode( fixMode ),
    mFixType( fixType ),
    mQuality( quality ),
    mSatellitesUsed( satellitesUsed ),
    mStatus( status ),
    mSatPrn( satPrn ),
    mSatInfoComplete( satInfoComplete ),
    mVerticalSpeed( verticalSpeed ),
    mMagneticVariation( magneticVariation ),
    mSourceName( sourceName )
{

}

bool GnssPositionInformation::isValid() const
{
  bool valid = false;
  if ( mStatus == 'V' || mFixType == NMEA_FIX_BAD || mQuality == 0 ) // some sources say that 'V' indicates position fix, but is below acceptable quality
  {
    valid = false;
  }
  else if ( mFixType == NMEA_FIX_2D )
  {
    valid = true;
  }
  else if ( mStatus == 'A' || mFixType == NMEA_FIX_3D || mQuality > 0 ) // good
  {
    valid = true;
  }

  return valid;
}

GnssPositionInformation::FixStatus GnssPositionInformation::fixStatus() const
{
  FixStatus fixStatus = NoData;

  // no fix if any of the three report bad; default values are invalid values and won't be changed if the corresponding NMEA msg is not received
  if ( mStatus == 'V' || mFixType == NMEA_FIX_BAD || mQuality == 0 ) // some sources say that 'V' indicates position fix, but is below acceptable quality
  {
    fixStatus = NoFix;
  }
  else if ( mFixType == NMEA_FIX_2D ) // 2D indication (from GGA)
  {
    fixStatus = Fix2D;
  }
  else if ( mStatus == 'A' || mFixType == NMEA_FIX_3D || mQuality > 0 ) // good
  {
    fixStatus = Fix3D;
  }
  return fixStatus;
}

QString GnssPositionInformation::qualityDescription() const
{
  switch ( mQuality )
  {
    case 8:
      return QCoreApplication::translate( "QgsGpsInformation", "Simulation mode" );

    case 7:
      return QCoreApplication::translate( "QgsGpsInformation", "Manual input mode" );

    case 6:
      return QCoreApplication::translate( "QgsGpsInformation", "Estimated" );

    case 5:
      return QCoreApplication::translate( "QgsGpsInformation", "Float RTK" );

    case 4:
      return QCoreApplication::translate( "QgsGpsInformation", "Fixed RTK" );

    case 3:
      return QCoreApplication::translate( "QgsGpsInformation", "PPS" );

    case 2:
      return QCoreApplication::translate( "QgsGpsInformation", "DGPS" );

    case 1:
      return QCoreApplication::translate( "QgsGpsInformation", "Autonomous" );

    case 0:
      return QCoreApplication::translate( "QgsGpsInformation", "Invalid" );

    default:
      return QCoreApplication::translate( "QgsGpsInformation", "Unknown (%1)" ).arg( QString::number( mQuality ) );
  }
}

QString GnssPositionInformation::fixStatusDescription() const
{
  return QString( QMetaEnum::fromType<FixStatus>().valueToKey( fixStatus() ) );
}
