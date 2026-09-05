

# File qfpositioningutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfpositioningutils.h**](qfpositioningutils_8h.md)

[Go to the documentation of this file](qfpositioningutils_8h.md)


```C++
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

class QFIELD_CORE_EXPORT QfPositioningUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfPositioningUtils( QObject *parent = nullptr );

    static Q_INVOKABLE QfGnssPositionInformation createGnssPositionInformation( double latitude, double longitude, double altitude, double speed, double direction, double horizontalAccuracy, double verticalAcurracy, double verticalSpeed, double magneticVariation, const QDateTime &timestamp, const QString &sourceName );

    static Q_INVOKABLE QfGnssPositionInformation createEmptyGnssPositionInformation();

    static Q_INVOKABLE QfGnssPositionInformation averagedPositionInformation( const QList<QfGnssPositionInformation> &positionsInformation );

    static Q_INVOKABLE QfGnssPositionInformation averagedPositionInformation( const QList<QVariant> &positionsInformation );

    static Q_INVOKABLE double bearingTrueNorth( const QgsPoint &position, const QgsCoordinateReferenceSystem &crs );

    static Q_INVOKABLE QgsRectangle createExtentForDevice( const QfGnssPositionInformation &positionInformation, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem::fromEpsgId( 4326 ), const QgsRectangle &withinRectangle = QgsRectangle() );

    static Q_INVOKABLE QfNtripSettings createNtripSettings( const QVariantMap &settings );

  private:
    static void initTimeZones();
    static void buildTimeZones();
};

#endif // QFPOSITIONINGUTILS_H
```


