/***************************************************************************
  positioning.h - Positioning

 ---------------------
 begin                : 22.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef POSITIONING_H
#define POSITIONING_H

#include "gnsspositioninformation.h"
#include "positioningsource.h"
#include "qgsquickcoordinatetransformer.h"

#include <QObject>
#include <QRemoteObjectDynamicReplica>
#include <QRemoteObjectHost>
#include <QRemoteObjectNode>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>
#include <qgspoint.h>

/**
 * This class manages the positioning source and offers positioning details.
 * \ingroup core
 */
class Positioning : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY validChanged )

    Q_PROPERTY( QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged )
    Q_PROPERTY( AbstractGnssReceiver::Capabilities deviceCapabilities READ deviceCapabilities NOTIFY deviceIdChanged )
    Q_PROPERTY( QString deviceLastError READ deviceLastError NOTIFY deviceLastErrorChanged )
    Q_PROPERTY( QAbstractSocket::SocketState deviceSocketState READ deviceSocketState NOTIFY deviceSocketStateChanged )
    Q_PROPERTY( QString deviceSocketStateString READ deviceSocketStateString NOTIFY deviceSocketStateStringChanged )

    Q_PROPERTY( QgsQuickCoordinateTransformer *coordinateTransformer READ coordinateTransformer WRITE setCoordinateTransformer NOTIFY coordinateTransformerChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )

    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition NOTIFY positionInformationChanged )
    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY positionInformationChanged )
    Q_PROPERTY( double projectedHorizontalAccuracy READ projectedHorizontalAccuracy NOTIFY positionInformationChanged )

    Q_PROPERTY( bool averagedPosition READ averagedPosition WRITE setAveragedPosition NOTIFY averagedPositionChanged )
    Q_PROPERTY( int averagedPositionCount READ averagedPositionCount NOTIFY averagedPositionCountChanged )

    Q_PROPERTY( PositioningSource::ElevationCorrectionMode elevationCorrectionMode READ elevationCorrectionMode WRITE setElevationCorrectionMode NOTIFY elevationCorrectionModeChanged )
    Q_PROPERTY( double antennaHeight READ antennaHeight WRITE setAntennaHeight NOTIFY antennaHeightChanged )

    Q_PROPERTY( double orientation READ orientation NOTIFY orientationChanged );

    Q_PROPERTY( bool logging READ logging WRITE setLogging NOTIFY loggingChanged )
    Q_PROPERTY( QString loggingPath READ loggingPath WRITE setLoggingPath NOTIFY loggingPathChanged )

    Q_PROPERTY( bool backgroundMode READ backgroundMode WRITE setBackgroundMode NOTIFY backgroundModeChanged )

  public:
    explicit Positioning( QObject *parent = nullptr );
    virtual ~Positioning() = default;

    /**
     * Returns TRUE when positioning is active.
     * \see setActive
     */
    bool active() const;

    /**
     * Sets the positioning \a active status.
     * \see active
     */
    void setActive( bool active );

    /**
     * Returns TRUE when the positioning device is valid.
     */
    bool valid() const;

    /**
     * Sets whether the positioning device is valid.
     */
    void setValid( bool valid );

    /**
     * Returns the current positioning device \a id used to fetch position information.
     * \see setDevice
     */
    QString deviceId() const;

    /**
     * Returns extra details (such as hdop, vdop, pdop) provided by the positioning device.
     */
    GnssPositionDetails deviceDetails() const;

    /**
     * Returns positioning device's last error string.
     */
    QString deviceLastError() const;

    /**
     * Returns positioning device's socket state.
     * \see deviceSocketStateString
     */
    QAbstractSocket::SocketState deviceSocketState() const;

    /**
     * Returns a string representation of the positioning device's socket state.
     * \see deviceSocketState
     */
    QString deviceSocketStateString() const;

    /**
     * Sets the positioning device \a id used to fetch position information.
     * \note A blank string will connect the internal positioning device;
     * bluetooth addresses will trigger an NMEA connection to external devices.
     */
    void setDeviceId( const QString &id );

    AbstractGnssReceiver::Capabilities deviceCapabilities() const;

    /**
     * Returns the coordinate transformer object used to reproject the position location.
     */
    QgsQuickCoordinateTransformer *coordinateTransformer() const { return mCoordinateTransformer; }

    /**
     * Sets the coordinate transformer object used to reproject the position location.
     * \param coordinateTransformer A QgsQuickCoordinateTransformer object.
     */
    void setCoordinateTransformer( QgsQuickCoordinateTransformer *coordinateTransformer );

    /**
     * Returns a GnssPositionInformation position information object.
     */
    GnssPositionInformation positionInformation() const;

    /**
     * Returns the position point in its original WGS84 projection.
     */
    QgsPoint sourcePosition() const;

    /**
     * Returns the position point in its destination CRS defined in the coordinate transformer.
     */
    QgsPoint projectedPosition() const;

    /**
     * Returns the position horizontal accuracy in the destination CRS' map units.
     */
    double projectedHorizontalAccuracy() const;

    /**
     * Returns whether the position information is averaged from an ongoing stream of incoming positions from the device.
     */
    bool averagedPosition() const;

    /**
     * Sets whether the position information is \a averaged from an ongoing stream of incoming positions from the device.
     */
    void setAveragedPosition( bool averaged );

    /**
     * Returns the current number of collected position informations from which the averaged position is calculated.
     * \note When averaged position is off, the value is zero.
     */
    int averagedPositionCount() const;

    /**
     * Returns the current elevation correction mode.
     * \note Some modes depends on device capabilities.
     */
    PositioningSource::ElevationCorrectionMode elevationCorrectionMode() const;

    /**
     * Sets the current elevation correction mode.
     * \note Some modes depends on device capabilities.
     */
    void setElevationCorrectionMode( PositioningSource::ElevationCorrectionMode elevationCorrectionMode );

    /**
     * Sets the GNSS device antenna height. This should be the pole height + sensore phase height.
     * \note When IMU is active this value is ignored as the device does the correction internally.
    **/
    double antennaHeight() const;

    /**
     * Returns the GNSS device antenna height. This should be the pole height + sensore phase height.
     * \note When IMU is active this value is ignored as the device does the correction internally.
    **/
    void setAntennaHeight( double antennaHeight );

    /**
     * Returns the current device orientation
     */
    double orientation() const;

    /**
     * Returns whether GNSS devices will log their incoming position stream into a logfile.
     * \note Requires a device type with logging capability
     */
    bool logging() const;

    /**
     * Sets whether GNSS devices will log their incoming position stream into a logfile.
     * \note Requires a device type with logging capability
     */
    void setLogging( bool logging );

    /**
     * Returns the path where GNSS devices will log their incoming position stream into logfiles.
     * \note Requires a device type with logging capability
     */
    QString loggingPath() const;

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
    bool backgroundMode() const;

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

  signals:
    void activeChanged();
    void validChanged();
    void deviceIdChanged();
    void deviceLastErrorChanged();
    void deviceSocketStateChanged();
    void deviceSocketStateStringChanged();
    void coordinateTransformerChanged();
    void positionInformationChanged();
    void averagedPositionChanged();
    void averagedPositionCountChanged();
    void projectedPositionChanged();
    void elevationCorrectionModeChanged();
    void antennaHeightChanged();
    void orientationChanged();
    void loggingChanged();
    void loggingPathChanged();
    void backgroundModeChanged();

    void triggerConnectDevice();
    void triggerDisconnectDevice();

  private slots:
    void onApplicationStateChanged( Qt::ApplicationState state );
    void processGnssPositionInformation();
    void processProjectedPosition();

  private:
    void setupSource();
    bool isSourceAvailable() const;

    double adjustOrientation( double orientation ) const;

    bool mValid = true;

    PositioningSource *mPositioningSource = nullptr;
    QRemoteObjectHost mHost;
    QRemoteObjectNode mNode;
    QSharedPointer<QRemoteObjectDynamicReplica> mPositioningSourceReplica; //skip-keyword-check

    GnssPositionInformation mPositionInformation;

    QgsQuickCoordinateTransformer *mCoordinateTransformer = nullptr;
    QgsPoint mSourcePosition;
    QgsPoint mProjectedPosition;
    double mProjectedHorizontalAccuracy = 0.0;
    virtual QList<QPair<QString, QVariant>> details() const { return {}; }

    bool mInternalPermissionChecked = false;
    bool mBluetoothPermissionChecked = false;

    bool mBackgroundMode = false;

    QVariantMap mPropertiesToSync;
};

#endif // POSITIONING_H
