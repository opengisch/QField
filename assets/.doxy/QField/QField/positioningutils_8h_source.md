

# File positioningutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**positioningutils.h**](positioningutils_8h.md)

[Go to the documentation of this file](positioningutils_8h.md)


```C++
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
#include <qgscoordinatereferencesystem.h>
#include <qgspoint.h>

class GnssPositionInformation;

class QFIELD_CORE_EXPORT PositioningUtils : public QObject
{
    Q_OBJECT

  public:
    explicit PositioningUtils( QObject *parent = nullptr );

    static Q_INVOKABLE GnssPositionInformation createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName );

    static Q_INVOKABLE GnssPositionInformation createEmptyGnssPositionInformation();

    static Q_INVOKABLE GnssPositionInformation averagedPositionInformation( const QList<GnssPositionInformation> &positionsInformation );

    static Q_INVOKABLE GnssPositionInformation averagedPositionInformation( const QList<QVariant> &positionsInformation );

    static Q_INVOKABLE double bearingTrueNorth( const QgsPoint &position, const QgsCoordinateReferenceSystem &crs );

    static Q_INVOKABLE QgsRectangle createExtentForDevice( const GnssPositionInformation &positionInformation, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 ), const QgsRectangle &withinRectangle = QgsRectangle() );

  private:
    static void initTimeZones();
    static void buildTimeZones();
};

#endif // POSITIONINGUTILS_H
```


