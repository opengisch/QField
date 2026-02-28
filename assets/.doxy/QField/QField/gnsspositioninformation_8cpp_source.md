

# File gnsspositioninformation.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**gnsspositioninformation.cpp**](gnsspositioninformation_8cpp.md)

[Go to the documentation of this file](gnsspositioninformation_8cpp.md)


```C++
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
#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QTime>


GnssPositionInformation::GnssPositionInformation( double latitude, double longitude, double elevation, double speed, double direction,
                                                  const QList<QgsSatelliteInfo> &satellitesInView, double pdop, double hdop, double vdop, double hacc, double vacc,
                                                  QDateTime utcDateTime, QChar fixMode, int fixType, int quality, int satellitesUsed, QChar status, const QList<int> &satPrn,
                                                  bool satInfoComplete, double verticalSpeed, double magneticVariation, int averagedCount, const QString &sourceName,
                                                  bool imuCorrection, double imuRoll, double imuPitch, double imuHeading, double imuSteering, double orientation )
  : mLatitude( latitude )
  , mLongitude( longitude )
  , mElevation( elevation )
  , mSpeed( speed )
  , mDirection( direction )
  , mSatellitesInView( satellitesInView )
  , mPdop( pdop )
  , mHdop( hdop )
  , mVdop( vdop )
  , mHacc( hacc )
  , mVacc( vacc )
  , mHvacc( sqrt( ( pow( hacc, 2 ) + pow( hacc, 2 ) + pow( vacc, 2 ) ) / 3 ) )
  , mUtcDateTime( utcDateTime )
  , mFixMode( fixMode )
  , mFixType( fixType )
  , mQuality( quality )
  , mSatellitesUsed( satellitesUsed )
  , mStatus( status )
  , mSatPrn( satPrn )
  , mSatInfoComplete( satInfoComplete )
  , mVerticalSpeed( verticalSpeed )
  , mMagneticVariation( magneticVariation )
  , mAveragedCount( averagedCount )
  , mSourceName( sourceName )
  , mImuCorrection( imuCorrection )
  , mImuRoll( imuRoll )
  , mImuPitch( imuPitch )
  , mImuHeading( imuHeading )
  , mImuSteering( imuSteering )
  , mOrientation( orientation )
{
}

bool GnssPositionInformation::operator==( const GnssPositionInformation &other ) const
{
  // clang-format off
  return mLatitude == other.mLatitude &&
         mLongitude == other.mLongitude &&
         mElevation == other.mElevation &&
         mSpeed == other.mSpeed &&
         mDirection == other.mDirection &&
         mPdop == other.mPdop &&
         mHdop == other.mHdop &&
         mVdop == other.mVdop &&
         mHacc == other.mHacc &&
         mVacc == other.mVacc &&
         mUtcDateTime == other.mUtcDateTime &&
         mFixMode == other.mFixMode &&
         mQuality == other.mQuality &&
         mStatus == other.mStatus &&
         mSatPrn == other.mSatPrn &&
         mSatInfoComplete == other.mSatInfoComplete &&
         mVerticalSpeed == other.mVerticalSpeed &&
         mMagneticVariation == other.mMagneticVariation &&
         mSourceName == other.mSourceName &&
         mImuCorrection== other.mImuCorrection &&
         mImuRoll == other.mImuRoll &&
         mImuPitch == other.mImuPitch &&
         mImuHeading == other.mImuHeading &&
         mImuSteering == other.mImuSteering &&
         mOrientation == other.mOrientation;
  // clang-format on
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
  QString quality;
  switch ( mQuality )
  {
    case 8:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Simulation mode" );
      break;
    case 7:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Manual input mode" );
      break;
    case 6:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Estimated" );
      break;
    case 5:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Float RTK" );
      break;
    case 4:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Fixed RTK" );
      break;
    case 3:
      quality = QCoreApplication::translate( "QgsGpsInformation", "PPS" );
      break;
    case 2:
      quality = QCoreApplication::translate( "QgsGpsInformation", "DGPS" );
      break;
    case 1:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Autonomous" );
      break;
    case 0:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Invalid" );
      break;
    default:
      quality = QCoreApplication::translate( "QgsGpsInformation", "Unknown (%1)" ).arg( QString::number( mQuality ) );
  }

  if ( mImuCorrection )
    quality.append( QCoreApplication::translate( "QgsGpsInformation", " + IMU" ) );

  return quality;
}

QString GnssPositionInformation::fixStatusDescription() const
{
  return QString( QMetaEnum::fromType<FixStatus>().valueToKey( fixStatus() ) );
}

QDataStream &operator<<( QDataStream &stream, const GnssPositionDetails &positionDetails )
{
  return stream << positionDetails.mNames << positionDetails.mValues;
}

//cppcheck-suppress constParameter
QDataStream &operator>>( QDataStream &stream, GnssPositionDetails &positionDetails )
{
  return stream >> positionDetails.mNames >> positionDetails.mValues;
}

QDataStream &operator<<( QDataStream &stream, const GnssPositionInformation &position )
{
  return stream << position.mLatitude << position.mLongitude << position.mElevation << position.mSpeed << position.mDirection
                << position.mSatellitesInView << position.mHdop << position.mVdop << position.mPdop
                << position.mHacc << position.mVacc << position.mHvacc << position.mUtcDateTime
                << position.mFixMode << position.mFixType << position.mQuality
                << position.mSatellitesUsed << position.mStatus << position.mSatPrn << position.mSatInfoComplete
                << position.mVerticalSpeed << position.mMagneticVariation << position.mSourceName
                << position.mAveragedCount << position.mImuCorrection << position.mImuRoll << position.mImuPitch << position.mImuHeading << position.mImuSteering
                << position.mOrientation;
}

//cppcheck-suppress constParameter
QDataStream &operator>>( QDataStream &stream, GnssPositionInformation &position )
{
  return stream >> position.mLatitude >> position.mLongitude >> position.mElevation >> position.mSpeed >> position.mDirection
         >> position.mSatellitesInView >> position.mHdop >> position.mVdop >> position.mPdop
         >> position.mHacc >> position.mVacc >> position.mHvacc >> position.mUtcDateTime
         >> position.mFixMode >> position.mFixType >> position.mQuality
         >> position.mSatellitesUsed >> position.mStatus >> position.mSatPrn >> position.mSatInfoComplete
         >> position.mVerticalSpeed >> position.mMagneticVariation >> position.mSourceName
         >> position.mAveragedCount >> position.mImuCorrection >> position.mImuRoll >> position.mImuPitch >> position.mImuHeading >> position.mImuSteering
         >> position.mOrientation;
}

QDataStream &operator<<( QDataStream &stream, const QgsSatelliteInfo &satelliteInfo )
{
  return stream << satelliteInfo.azimuth << satelliteInfo.elevation << satelliteInfo.id << satelliteInfo.inUse << satelliteInfo.satType << satelliteInfo.signal;
}

//cppcheck-suppress constParameter
QDataStream &operator>>( QDataStream &stream, QgsSatelliteInfo &satelliteInfo )
{
  return stream >> satelliteInfo.azimuth >> satelliteInfo.elevation >> satelliteInfo.id >> satelliteInfo.inUse >> satelliteInfo.satType >> satelliteInfo.signal;
}
```


