

# File coordinatereferencesystemutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**coordinatereferencesystemutils.h**](coordinatereferencesystemutils_8h.md)

[Go to the documentation of this file](coordinatereferencesystemutils_8h.md)


```C++
/***************************************************************************
  coordinatereferencesystemutils.h - CoordinateReferenceSystemUtils

 ---------------------
 begin                : 28.05.2022
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
#ifndef COORDINATEREFERENCESYSTEMUTILS_H
#define COORDINATEREFERENCESYSTEMUTILS_H

#include "qfield_core_export.h"

#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>

class QFIELD_CORE_EXPORT CoordinateReferenceSystemUtils : public QObject
{
    Q_OBJECT

  public:
    explicit CoordinateReferenceSystemUtils( QObject *parent = nullptr );

    static Q_INVOKABLE QgsCoordinateReferenceSystem fromDescription( const QString &definition ) { return QgsCoordinateReferenceSystem( definition ); }

    static Q_INVOKABLE QgsCoordinateReferenceSystem wgs84Crs() { return QgsCoordinateReferenceSystem::fromEpsgId( 4326 ); }

    static Q_INVOKABLE QgsCoordinateReferenceSystem invalidCrs() { return QgsCoordinateReferenceSystem(); }

    static Q_INVOKABLE QgsCoordinateTransformContext emptyTransformContext() { return QgsCoordinateTransformContext(); }

    static Q_INVOKABLE bool defaultCoordinateOrderForCrsIsXY( const QgsCoordinateReferenceSystem &crs );
};

#endif // COORDINATEREFERENCESYSTEMUTILS_H
```


