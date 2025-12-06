

# File gnsspositioninformation.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**gnsspositioninformation.h**](gnsspositioninformation_8h.md)

[Go to the documentation of this file](gnsspositioninformation_8h.md)


```C++
/***************************************************************************
  gnsspositioninformation.h - GnssPositionInformation
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

#ifndef GNSSPOSITIONINFORMATION_H
#define GNSSPOSITIONINFORMATION_H

#include <QDateTime>
#include <QObject>
#include <QString>
#include <qgis.h>
#include <qgssatelliteinformation.h>

/* Statics from external/nmea/info.h:*/
#define NMEA_SIG_BAD ( 0 )
#define NMEA_SIG_LOW ( 1 )
#define NMEA_SIG_MID ( 2 )
#define NMEA_SIG_HIGH ( 3 )

#define NMEA_FIX_BAD ( 1 )
#define NMEA_FIX_2D ( 2 )
#define NMEA_FIX_3D ( 3 )

#define NMEA_MAXSAT ( 12 )
#define NMEA_SATINPACK ( 4 )
#define NMEA_NSATPACKS ( NMEA_MAXSAT / NMEA_SATINPACK )

#define NMEA_DEF_LAT ( 5001.2621 )
#define NMEA_DEF_LON ( 3613.0595 )

class GnssPositionInformation
{
    Q_GADGET

    Q_PROPERTY( bool isValid READ isValid )
    Q_PROPERTY( FixStatus fixStatus READ fixStatus )
    Q_PROPERTY( QString qualityDescription READ qualityDescription )
    Q_PROPERTY( QString fixStatusDescription READ fixStatusDescription )

    Q_PROPERTY( double latitude READ latitude )
    Q_PROPERTY( bool latitudeValid READ latitudeValid )
    Q_PROPERTY( double longitude READ longitude )
    Q_PROPERTY( bool longitudeValid READ longitudeValid )
    Q_PROPERTY( double elevation READ elevation )
    Q_PROPERTY( bool elevationValid READ elevationValid )
    Q_PROPERTY( double speed READ speed )
    Q_PROPERTY( bool speedValid READ speedValid )
    Q_PROPERTY( double direction READ direction )
    Q_PROPERTY( bool directionValid READ directionValid )
    Q_PROPERTY( QList<QgsSatelliteInfo> satellitesInView READ satellitesInView )
    Q_PROPERTY( double pdop READ pdop )
    Q_PROPERTY( double hdop READ hdop )
    Q_PROPERTY( double vdop READ vdop )
    Q_PROPERTY( double hacc READ hacc )
    Q_PROPERTY( bool haccValid READ haccValid )
    Q_PROPERTY( double vacc READ vacc )
    Q_PROPERTY( bool vaccValid READ vaccValid )
    Q_PROPERTY( double hvacc READ hvacc )
    Q_PROPERTY( AccuracyQuality accuracyQuality READ accuracyQuality WRITE setAccuracyQuality )
    Q_PROPERTY( double hvaccValid READ hvaccValid )
    Q_PROPERTY( QDateTime utcDateTime READ utcDateTime )
    Q_PROPERTY( QChar fixMode READ fixMode )
    Q_PROPERTY( int fixType READ fixType )
    Q_PROPERTY( int quality READ quality )
    Q_PROPERTY( int satellitesUsed READ satellitesUsed )
    Q_PROPERTY( QChar status READ status )
    Q_PROPERTY( QList<int> satPrn READ satPrn )
    Q_PROPERTY( bool satInfoComplete READ satInfoComplete )
    Q_PROPERTY( double verticalSpeed READ verticalSpeed )
    Q_PROPERTY( double magneticVariation READ magneticVariation )
    Q_PROPERTY( int averagedCount READ averagedCount )
    Q_PROPERTY( QString sourceName READ sourceName )
    Q_PROPERTY( bool imuCorrection READ imuCorrection )
    Q_PROPERTY( double imuRoll READ imuRoll )
    Q_PROPERTY( bool imuRollValid READ imuRollValid )
    Q_PROPERTY( double imuPitch READ imuPitch )
    Q_PROPERTY( bool imuPitchValid READ imuPitchValid )
    Q_PROPERTY( double imuHeading READ imuHeading )
    Q_PROPERTY( bool imuHeadingValid READ imuHeadingValid )
    Q_PROPERTY( double imuSteering READ imuSteering )
    Q_PROPERTY( bool imuSteeringValid READ imuSteeringValid )
    Q_PROPERTY( double orientation READ orientation )
    Q_PROPERTY( bool orientationValid READ orientationValid )

  public:
    enum FixStatus
    {
      NoData,
      NoFix,
      Fix2D,
      Fix3D
    };

    Q_ENUM( FixStatus )

    enum AccuracyQuality
    {
      AccuracyUndetermined,
      AccuracyBad,
      AccuracyOk,
      AccuracyExcellent
    };

    Q_ENUM( AccuracyQuality )

    /* !! Adjust PositioningUtils::averagedPositionInformation whenever a new member is added !! */

    
    GnssPositionInformation( double latitude = std::numeric_limits<double>::quiet_NaN(), double longitude = std::numeric_limits<double>::quiet_NaN(), double elevation = std::numeric_limits<double>::quiet_NaN(),
                             double speed = std::numeric_limits<double>::quiet_NaN(), double direction = std::numeric_limits<double>::quiet_NaN(), const QList<QgsSatelliteInfo> &satellitesInView = QList<QgsSatelliteInfo>(),
                             double pdop = 0, double hdop = 0, double vdop = 0,
                             double hacc = std::numeric_limits<double>::quiet_NaN(), double vacc = std::numeric_limits<double>::quiet_NaN(), QDateTime utcDateTime = QDateTime(),
                             QChar fixMode = QChar(), int fixType = 0, int quality = -1, int satellitesUsed = 0, QChar status = QChar(), const QList<int> &satPrn = QList<int>(), bool satInfoComplete = false,
                             double verticalSpeed = std::numeric_limits<double>::quiet_NaN(), double magneticVariation = std::numeric_limits<double>::quiet_NaN(), int averagedCount = 0, const QString &sourceName = QString(),
                             bool imuCorrection = false, double imuRoll = std::numeric_limits<double>::quiet_NaN(), double imuPitch = std::numeric_limits<double>::quiet_NaN(),
                             double imuHeading = std::numeric_limits<double>::quiet_NaN(), double imuSteering = std::numeric_limits<double>::quiet_NaN(),
                             double orientation = std::numeric_limits<double>::quiet_NaN() );

    bool operator==( const GnssPositionInformation &other ) const;
    bool operator!=( const GnssPositionInformation &other ) const { return !operator==( other ); }

    bool isValid() const;

    FixStatus fixStatus() const;

    QString qualityDescription() const;

    QString fixStatusDescription() const;

    void setLatitude( double latitude ) { mLatitude = latitude; }
    double latitude() const { return mLatitude; }
    bool latitudeValid() const { return !std::isnan( mLatitude ); }

    void setLongitude( double longitude ) { mLongitude = longitude; }
    double longitude() const { return mLongitude; }
    bool longitudeValid() const { return !std::isnan( mLongitude ); }

    void setElevation( double elevation ) { mElevation = elevation; }
    double elevation() const { return mElevation; }
    bool elevationValid() const { return !std::isnan( mElevation ); }

    void setSpeed( double speed ) { mSpeed = speed; }
    double speed() const { return mSpeed; }
    bool speedValid() const { return !std::isnan( mSpeed ); }

    void setDirection( double direction ) { mDirection = direction; }
    double direction() const { return mDirection; }
    bool directionValid() const { return !std::isnan( mDirection ); }

    void setSatellitesInView( const QList<QgsSatelliteInfo> &satellitesInView ) { mSatellitesInView = satellitesInView; }
    QList<QgsSatelliteInfo> satellitesInView() const { return mSatellitesInView; }

    void setPdop( double pdop ) { mPdop = pdop; }
    double pdop() const { return mPdop; }

    void setHdop( double hdop ) { mHdop = hdop; }
    double hdop() const { return mHdop; }

    void setVdop( double vdop ) { mVdop = vdop; }
    double vdop() const { return mVdop; }

    void setHacc( double hacc ) { mHacc = hacc; }
    double hacc() const { return mHacc; }
    bool haccValid() const { return !std::isnan( mHacc ); }

    void setVacc( double vacc ) { mVacc = vacc; }
    double vacc() const { return mVacc; }
    bool vaccValid() const { return !std::isnan( mVacc ); }

    void setHVacc( double hvacc ) { mHvacc = hvacc; }
    double hvacc() const { return mHvacc; }
    bool hvaccValid() const { return !std::isnan( mHvacc ); }

    void setAccuracyQuality( AccuracyQuality quality ) { mAccuracyQuality = quality; }
    AccuracyQuality accuracyQuality() const { return mAccuracyQuality; }

    void setUtcDateTime( const QDateTime &utcDateTime ) { mUtcDateTime = utcDateTime; }
    QDateTime utcDateTime() const { return mUtcDateTime; }

    void setFixMode( QChar fixMode ) { mFixMode = fixMode; }
    QChar fixMode() const { return mFixMode; }

    void setFixType( int fixType ) { mFixType = fixType; }
    int fixType() const { return mFixType; }

    void setQuality( int quality ) { mQuality = quality; }
    int quality() const { return mQuality; }

    void setSatellitesUsed( int satellitesUsed ) { mSatellitesUsed = satellitesUsed; }
    int satellitesUsed() const { return mSatellitesUsed; }

    void setStatus( QChar status ) { mStatus = status; }
    QChar status() const { return mStatus; }

    void setSatPrn( const QList<int> &satPrn ) { mSatPrn = satPrn; }
    QList<int> satPrn() const { return mSatPrn; }

    void setSatInfoComplete( bool satInfoComplete ) { mSatInfoComplete = satInfoComplete; }
    bool satInfoComplete() const { return mSatInfoComplete; }

    void setVerticalSpeed( double verticalSpeed ) { mVerticalSpeed = verticalSpeed; }
    double verticalSpeed() const { return mVerticalSpeed; }

    void setMagneticVaritation( double magneticVariation ) { mMagneticVariation = magneticVariation; }
    double magneticVariation() const { return mMagneticVariation; }

    void setSourceName( const QString &sourceName ) { mSourceName = sourceName; }
    QString sourceName() const { return mSourceName; }

    void setAveragedCount( int averagedCount ) { mAveragedCount = averagedCount; }
    int averagedCount() const { return mAveragedCount; }

    void setImuCorrection( bool imuCorrection ) { mImuCorrection = imuCorrection; }
    bool imuCorrection() const { return mImuCorrection; }

    void setImuRoll( double roll ) { mImuRoll = roll; }
    double imuRoll() const { return mImuRoll; }
    bool imuRollValid() const { return !std::isnan( mImuRoll ); }

    void setImuPitch( double pitch ) { mImuPitch = pitch; }
    double imuPitch() const { return mImuPitch; }
    bool imuPitchValid() const { return !std::isnan( mImuPitch ); }

    void setImuHeading( double heading ) { mImuHeading = heading; }
    double imuHeading() const { return mImuHeading; }
    bool imuHeadingValid() const { return !std::isnan( mImuHeading ); }

    void setImuSteering( double steering ) { mImuSteering = steering; }
    double imuSteering() const { return mImuSteering; }
    bool imuSteeringValid() const { return !std::isnan( mImuSteering ); }

    void setOrientation( double orientation ) { mOrientation = orientation; }
    double orientation() const { return mOrientation; }
    bool orientationValid() const { return !std::isnan( mOrientation ); }

  private:
    double mLatitude = std::numeric_limits<double>::quiet_NaN();
    double mLongitude = std::numeric_limits<double>::quiet_NaN();
    double mElevation = std::numeric_limits<double>::quiet_NaN();
    double mSpeed = std::numeric_limits<double>::quiet_NaN();
    double mDirection = std::numeric_limits<double>::quiet_NaN();
    QList<QgsSatelliteInfo> mSatellitesInView;
    double mPdop = 0;
    double mHdop = 0;
    double mVdop = 0;
    double mHacc = std::numeric_limits<double>::quiet_NaN();
    double mVacc = std::numeric_limits<double>::quiet_NaN();
    double mHvacc = std::numeric_limits<double>::quiet_NaN();
    AccuracyQuality mAccuracyQuality = AccuracyUndetermined;
    QDateTime mUtcDateTime;
    QChar mFixMode;
    int mFixType = 0;
    int mQuality = -1;
    int mSatellitesUsed = 0;
    QChar mStatus;
    QList<int> mSatPrn;
    bool mSatInfoComplete = false;
    double mVerticalSpeed = std::numeric_limits<double>::quiet_NaN();
    double mMagneticVariation = std::numeric_limits<double>::quiet_NaN();
    int mAveragedCount = 0;
    QString mSourceName;
    bool mImuCorrection;
    double mImuRoll = std::numeric_limits<double>::quiet_NaN();
    double mImuPitch = std::numeric_limits<double>::quiet_NaN();
    double mImuHeading = std::numeric_limits<double>::quiet_NaN();
    double mImuSteering = std::numeric_limits<double>::quiet_NaN();
    double mOrientation = std::numeric_limits<double>::quiet_NaN();

    friend QDataStream &operator<<( QDataStream &stream, const GnssPositionInformation &position );
    friend QDataStream &operator>>( QDataStream &stream, GnssPositionInformation &position );
};

Q_DECLARE_METATYPE( GnssPositionInformation )
Q_DECLARE_METATYPE( GnssPositionInformation::AccuracyQuality )

class GnssPositionDetails
{
    Q_GADGET

  public:
    GnssPositionDetails() {};

    void append( const QString &name, const QVariant &value )
    {
      mNames << name;
      mValues << value;
    }

    QList<QString> names() const { return mNames; }
    QList<QVariant> values() const { return mValues; }

  private:
    QList<QString> mNames;
    QList<QVariant> mValues;

    friend QDataStream &operator<<( QDataStream &stream, const GnssPositionDetails &position );
    friend QDataStream &operator>>( QDataStream &stream, GnssPositionDetails &position );
};

Q_DECLARE_METATYPE( GnssPositionDetails )

QDataStream &operator<<( QDataStream &stream, const GnssPositionDetails &positionDetails );
QDataStream &operator>>( QDataStream &stream, GnssPositionDetails &positionDetails );

QDataStream &operator<<( QDataStream &stream, const GnssPositionInformation &position );
QDataStream &operator>>( QDataStream &stream, GnssPositionInformation &position );

QDataStream &operator<<( QDataStream &stream, const QgsSatelliteInfo &satelliteInfo );
QDataStream &operator>>( QDataStream &stream, QgsSatelliteInfo &satelliteInfo );

#endif // GNSSPOSITIONINFORMATION_H
```


