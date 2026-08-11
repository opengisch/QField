/***************************************************************************
  qfpositioningutils.h - QfPositioningUtils

 ---------------------
 begin                : 20.05.2022
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
#ifndef QFPOSITIONINGUTILS_H
#define QFPOSITIONINGUTILS_H

#include "qfield_core_export.h"
#include "qfntripsettings.h"

#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>

class QfGnssPositionInformation;

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT QfPositioningUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfPositioningUtils( QObject *parent = nullptr );

    /**
     * Creates a QfGnssPositionInformation with values.
     * The status will be set to "A"ctive by default.
     */
    static Q_INVOKABLE QfGnssPositionInformation createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName );

    /**
     * Creates an empty QfGnssPositionInformation.
     */
    static Q_INVOKABLE QfGnssPositionInformation createEmptyGnssPositionInformation();

    /**
     * Returns an average QfGnssPositionInformation from a list of position information.
     */
    static Q_INVOKABLE QfGnssPositionInformation averagedPositionInformation( const QList<QfGnssPositionInformation> &positionsInformation );

    /**
     * Returns an average QfGnssPositionInformation from a list of position information
     */
    static Q_INVOKABLE QfGnssPositionInformation averagedPositionInformation( const QList<QVariant> &positionsInformation );

    /**
     * Returns the true north bearing from a given position and CRS
     */
    static Q_INVOKABLE double bearingTrueNorth( const QgsPoint &position, const QgsCoordinateReferenceSystem &crs );

    /**
     * Returns a reasonable default extent derived from a device's timezone and positioning if available. The extent
     * can optionally be clipped to a QgsRectangle.
     */
    static Q_INVOKABLE QgsRectangle createExtentForDevice( const QfGnssPositionInformation &positionInformation, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 ), const QgsRectangle &withinRectangle = QgsRectangle() );

    static Q_INVOKABLE QfNtripSettings createNtripSettings( const QVariantMap &settings );

  private:
    static void initTimeZones();
    //! \note not available in Python bindings
    static void buildTimeZones();
};

#endif // QFPOSITIONINGUTILS_H
