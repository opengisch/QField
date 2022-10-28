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

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"
#include "qgsquickcoordinatetransformer.h"

#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>
#include <qgspoint.h>

/**
 * This class connects to GNSS devices (internal or bluetooth NMEA) and provides
 * positioning details.
 */
class Positioning : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( QString deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged )
    Q_PROPERTY( AbstractGnssReceiver *device READ device NOTIFY deviceChanged )

    Q_PROPERTY( QgsQuickCoordinateTransformer *coordinateTransformer READ coordinateTransformer WRITE setCoordinateTransformer NOTIFY coordinateTransformerChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY validChanged )

    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition NOTIFY positionInformationChanged )
    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )
    Q_PROPERTY( double projectedHorizontalAccuracy READ projectedHorizontalAccuracy NOTIFY projectedPositionChanged )

    Q_PROPERTY( bool averagedPosition READ averagedPosition WRITE setAveragedPosition NOTIFY averagedPositionChanged )
    Q_PROPERTY( int averagedPositionCount READ averagedPositionCount NOTIFY averagedPositionCountChanged )

    Q_PROPERTY( bool ellipsoidalElevation READ ellipsoidalElevation WRITE setEllipsoidalElevation NOTIFY ellipsoidalElevationChanged )

  public:
    explicit Positioning( QObject *parent = nullptr );

    virtual ~Positioning() = default;

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
     * Returns the current positioning device \a id used to fetch position information.
     * \see setDevice
     */
    QString deviceId() const { return mDeviceId; }

    /**
     * Sets the positioning device \a id used to fetch position information.
     * \note A blank string will connect the internal positioning device;
     * bluetooth addresses will trigger an NMEA connection to external devices.
     */
    void setDeviceId( const QString &id );

    AbstractGnssReceiver *device() const { return mReceiver.get(); }

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
    GnssPositionInformation positionInformation() const { return mPositionInformation; };

    /**
     * Returns TRUE when the positioning device is valid.
     */
    bool valid() const { return mValid; }

    /**
     * Sets whether the positioning device is valid.
     */
    void setValid( bool valid );

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
    bool averagedPosition() const { return mAveragedPosition; }

    /**
     * Sets whether the position information is \a averaged from an ongoing stream of incoming positions from the device.
     */
    void setAveragedPosition( bool averaged );

    /**
     * Returns the current number of collected position informations from which the averaged position is calculated.
     * \note When averaged position is off, the value is zero.
     */
    int averagedPositionCount() const { return mCollectedPositionInformations.size(); }

    bool ellipsoidalElevation() const { return mEllipsoidalElevation; }

    void setEllipsoidalElevation( bool ellipsoidal );

  signals:

    void activeChanged();
    void deviceIdChanged();
    void deviceChanged();
    void validChanged();
    void coordinateTransformerChanged();
    void positionInformationChanged();
    void averagedPositionChanged();
    void averagedPositionCountChanged();
    void projectedPositionChanged();
    void ellipsoidalElevationChanged();

  private slots:

    void lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation );
    void projectedPositionTransformed();

  private:
    void setupDevice();

    bool mActive = false;

    QString mDeviceId;
    bool mValid = false;

    QgsCoordinateReferenceSystem mSourceCrs;
    QgsCoordinateReferenceSystem mDestinationCrs;
    QgsCoordinateTransformContext mTransformContext;

    GnssPositionInformation mPositionInformation;
    QList<GnssPositionInformation> mCollectedPositionInformations;

    QgsQuickCoordinateTransformer *mCoordinateTransformer = nullptr;
    QgsPoint mSourcePosition;
    QgsPoint mProjectedPosition;
    double mProjectedHorizontalAccuracy = 0.0;

    bool mAveragedPosition = false;

    bool mEllipsoidalElevation = false;

    std::unique_ptr<AbstractGnssReceiver> mReceiver;
};

#endif // POSITIONING_H
