/***************************************************************************
  processingutils.h - ProcessingUtils

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
#ifndef PROCESSINGUTILS_H
#define PROCESSINGUTILS_H

#include "qfield_core_export.h"

#include <QObject>
#include <qgis.h>


/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT ProcessingUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ProcessingUtils( QObject *parent = nullptr );

    /**
     * Returns the conversion factor between the specified areal units.
     * \param fromUnit area unit to convert from
     * \param toUnit area unit to convert to
     * \returns multiplication factor to convert between units
     */
    static Q_INVOKABLE double fromAreaUnitToAreaUnitFactor( Qgis::AreaUnit fromUnit, Qgis::AreaUnit toUnit );
};

#endif // PROCESSINGUTILS_H
