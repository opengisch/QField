/***************************************************************************
  positioningutils.h - PositioningUtils

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
#ifndef POSITIONINGUTILS_H
#define POSITIONINGUTILS_H

#include "qfield_core_export.h"

#include <QObject>

class GnssPositionInformation;

class QFIELD_CORE_EXPORT PositioningUtils : public QObject
{
    Q_OBJECT

  public:
    explicit PositioningUtils( QObject *parent = nullptr );

    /**
     * Creates a GnssPositionInformation with values.
     * The status will be set to "A"ctive by default.
     */
    static Q_INVOKABLE GnssPositionInformation createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName );

    /**
     * Returns an average GnssPositionInformation from a list of position information
     */
    static GnssPositionInformation averagedPositionInformation( const QList<GnssPositionInformation> &positionsInformation );

    /**
     * Returns an average GnssPositionInformation from a list of position information
     */
    static Q_INVOKABLE GnssPositionInformation averagedPositionInformation( const QList<QVariant> &positionsInformation );
};

#endif // POSITIONINGUTILS_H
