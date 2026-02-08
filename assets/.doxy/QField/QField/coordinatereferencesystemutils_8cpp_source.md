

# File coordinatereferencesystemutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**coordinatereferencesystemutils.cpp**](coordinatereferencesystemutils_8cpp.md)

[Go to the documentation of this file](coordinatereferencesystemutils_8cpp.md)


```C++
/***************************************************************************
  coordinatereferencesystemutils.cpp - CoordinateReferenceSystemUtils

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

#include "coordinatereferencesystemutils.h"

#include <qgscoordinatereferencesystemutils.h>

CoordinateReferenceSystemUtils::CoordinateReferenceSystemUtils( QObject *parent )
  : QObject( parent )
{
}

bool CoordinateReferenceSystemUtils::defaultCoordinateOrderForCrsIsXY( const QgsCoordinateReferenceSystem &crs )
{
  return QgsCoordinateReferenceSystemUtils::defaultCoordinateOrderForCrs( crs ) == Qgis::CoordinateOrder::XY;
}
```


