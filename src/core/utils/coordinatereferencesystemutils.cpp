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
