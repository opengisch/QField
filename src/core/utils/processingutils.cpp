/***************************************************************************
  processingutils.cpp - ProcessingUtils

 ---------------------
 begin                : 20.08.2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "processingutils.h"

#include <qgsunittypes.h>

ProcessingUtils::ProcessingUtils( QObject *parent )
  : QObject( parent )
{
}

double ProcessingUtils::fromAreaUnitToAreaUnitFactor( Qgis::AreaUnit fromUnit, Qgis::AreaUnit toUnit )
{
  return QgsUnitTypes::fromUnitToUnitFactor( fromUnit, toUnit );
}
