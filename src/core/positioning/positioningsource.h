/***************************************************************************
  positioningsource.h - PositioningSource

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

#ifndef POSITIONINGSOURCE_H
#define POSITIONINGSOURCE_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"

#include <QCompass>
#include <QObject>
#include <QTimer>

/**
 * This class connects to GNSS devices (internal or bluetooth NMEA) and provides
 * positioning details.
 * \ingroup core
 */
class PositioningSource : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY validChanged )

    Q_PROPERTY( QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged )
    Q_PROPERTY( GnssPositionDetails deviceDetails READ deviceDetails NOTIFY positionInformationChanged )
    Q_PROPERTY( QString deviceLastError READ deviceLastError NOTIFY deviceLastErrorChanged )
    Q_PROPERTY( QAbstractSocket::SocketState deviceSocketState READ deviceSocketState NOTIFY deviceSocketStateChanged )
    Q_PROPERTY( QString deviceSocketStateString READ deviceSocketStateString NOTIFY deviceSocketStateStringChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )

    Q_PROPERTY( bool averagedPosition READ averagedPosition WRITE setAveragedPosition NOTIFY averagedPositionChanged )
    Q_PROPERTY( int averagedPositionCount READ averagedPositionCount NOTIFY averagedPositionCountChanged )

    Q_PROPERTY( ElevationCorrectionMode elevationCorrectionMode READ elevationCorrectionMode WRITE setElevationCorrectionMode NOTIFY elevationCorrectionModeChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )

    Q_PROPERTY( double orientation READ orientation NOTIFY orientationChanged );

    Q_PROPERTY( bool logging READ logging WRITE setLogging NOTIFY loggingChanged )
    Q_PROPERTY( QString loggingPath READ loggingPath WRITE setLoggingPath NOTIFY loggingPathChanged )

    Q_PROPERTY( bool backgroundMode READ backgroundMode WRITE setBackgroundMode NOTIFY backgroundModeChanged )

  public:
    /**
     * Elevation correction modes
     */
    enum class ElevationCorrectionMode
    {
      None,                    //! Elevation is used as it comes from the device. For most devices including Android internal this is ellipsoidic.
      OrthometricFromDevice,   //! Apply the geoid correction provided by the device. Available only for external devices.
      OrthometricFromGeoidFile //! Apply the geoid correction from a geoid file.
    };
    Q_ENUM( ElevationCorrectionMode )

    explicit PositioningSource( QObject *parent = nullptr );
    virtual ~PositioningSource() = default;

    /**
     * Returns TRUE when positioning is active.
     * \see setActive
     */
    bool active() const { return mActive; }

    /**
     * Sets the positioning \a active status.
     * \see active
     */
    void setActive( bool active );

    /**
     * Returns TRUE when the positioning device is valid.
     */
    bool valid() const { return mValid; }

    /**
     * Sets whether the positioning device is valid.
     */
    void setValid( bool valid );

    /**
     * Returns the current positioning device \a id used to fetch position information.
     * \see setDevice
     */
    Q_INVOKABLE QString deviceId() const { return mDeviceId; }

    /**
     * Sets the positioning device \a id used to fetch position information.
     * \note A blank string will connect the internal positioning device;
     * bluetooth addresses will trigger an NMEA connection to external devices.
     */
    void setDeviceId( const QString &id );

    /**
     * Returns the current positioning device.
     * \see deviceId
     */
    AbstractGnssReceiver *device() const { return mReceiver; }

    /**
     * Returns extra details (such as hdop, vdop, pdop) provided by the positioning device.
     */
    GnssPositionDetails deviceDetails() const { return mReceiver ? mReceiver->details() : GnssPositionDetails(); }

    /**
     * Returns positioning device's last error string.
     */
    QString deviceLastError() const { return mReceiver ? mReceiver->lastError() : QString(); }

    /**
     * Returns positioning device's socket state.
     * \see deviceSocketStateString
     */
    QAbstractSocket::SocketState deviceSocketState() const { return mReceiver ? mReceiver->socketState() : QAbstractSocket::UnconnectedState; }

    /**
     * Returns a string representation of the positioning device's socket state.
     * \see deviceSocketState
     */
    QString deviceSocketStateString() const { return mReceiver ? mReceiver->socketStateString() : QString(); }

    /**
     * Returns a GnssPositionInformation position information object.
     */
    GnssPositionInformation positionInformation() const { return mPositionInformation; };

    /**
     * Returns whether the position information is averaged from an ongoing stream of incoming positions from the device.
     */
    bool averagedPosition() const { return mAveragedPosition; }

    /**
     * Sets whether the position information is \a averaged from an ongoing stream of incoming positions from the device.
     */
    void setAveragedPosition( bool averaged );

    /**
     * Returns the current number of collected position informations from which the averaged position is calculated.
     * \note When averaged position is off, the value is zero.
     */
    int averagedPositionCount() const { return static_cast<int>( mCollectedPositionInformations.size() ); }

    /**
     * Returns the current elevation correction mode.
     * \note Some modes depends on device capabilities.
     */
    ElevationCorrectionMode elevationCorrectionMode() const { return mElevationCorrectionMode; }

    /**
     * Sets the current elevation correction mode.
     * \note Some modes depends on device capabilities.
     */
    void setElevationCorrectionMode( ElevationCorrectionMode elevationCorrectionMode );

    /**
     * Sets the GNSS device antenna height. This should be the pole height + sensore phase height.
     * \note When IMU is active this value is ignored as the device does the correction internally.
    **/
    double antennaHeight() const { return mAntennaHeight; }

    /**
     * Returns the GNSS device antenna height. This should be the pole height + sensore phase height.
     * \note When IMU is active this value is ignored as the device does the correction internally.
    **/
    void setAntennaHeight( double antennaHeight );

    /**
     * Returns the current device orientation
     */
    double orientation() const { return mOrientation; }

    /**
     * Returns whether GNSS devices will log their incoming position stream into a logfile.
     * \note Requires a device type with logging capability
     */
    bool logging() const { return mLogging; }

    /**
     * Sets whether GNSS devices will log their incoming position stream into a logfile.
     * \note Requires a device type with logging capability
     */
    void setLogging( bool logging );

    /**
     * Returns the path where GNSS devices will log their incoming position stream into logfiles.
     * \note Requires a device type with logging capability
     */
    QString loggingPath() const { return mLoggingPath; }

    /**
     * Sets the path where GNSS devices will log their incoming position stream into logfiles.
     * \note Requires a device type with logging capability
     */
    void setLoggingPath( const QString &path );

    /**
     * Returns TRUE if the background mode is active. When activated, position information details
     * will not be signaled but instead saved to disk until deactivated.
     * \see getBackgroundPositionInformation()
     */
    bool backgroundMode() const { return mBackgroundMode; }

    /**
     * Sets whether the background mode is active. When activated, position information details
     * will not be signaled but instead saved to disk until deactivated.
     * \see getBackgroundPositionInformation()
     */
    void setBackgroundMode( bool backgroundMode );

    /**
     * Returns a list of position information collected while background mode is active.
     * \see backgroundMode()
     * \see setBackgroundMode()
     */
    Q_INVOKABLE QList<GnssPositionInformation> getBackgroundPositionInformation() const;

    static QString backgroundFilePath;

  signals:
    void activeChanged();
    void validChanged();
    void deviceIdChanged();
    void deviceChanged();
    void deviceLastErrorChanged();
    void deviceSocketStateChanged();
    void deviceSocketStateStringChanged();
    void positionInformationChanged();
    void averagedPositionChanged();
    void averagedPositionCountChanged();
    void elevationCorrectionModeChanged();
    void antennaHeightChanged();
    void orientationChanged();
    void loggingChanged();
    void loggingPathChanged();
    void backgroundModeChanged();

  public slots:

    void triggerConnectDevice();
    void triggerDisconnectDevice();

  private slots:

    void lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation );
    void processCompassReading();

  private:
    void setupDevice();

    bool mActive = false;

    QString mDeviceId;
    bool mValid = false;

    GnssPositionInformation mPositionInformation;
    QList<GnssPositionInformation> mCollectedPositionInformations;

    bool mAveragedPosition = false;

    ElevationCorrectionMode mElevationCorrectionMode = ElevationCorrectionMode::None;
    double mAntennaHeight = 0.0;

    bool mLogging = false;
    QString mLoggingPath;

    bool mBackgroundMode = false;

    AbstractGnssReceiver *mReceiver = nullptr;

    QCompass mCompass;
    QTimer mCompassTimer;
    double mOrientation = std::numeric_limits<double>::quiet_NaN();
};

Q_DECLARE_METATYPE( PositioningSource::ElevationCorrectionMode )

#endif // POSITIONINGSOURCE_H
