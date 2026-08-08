

# File processingutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**processingutils.h**](processingutils_8h.md)

[Go to the documentation of this file](processingutils_8h.md)


```C++
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


class QFIELD_CORE_EXPORT ProcessingUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ProcessingUtils( QObject *parent = nullptr );

    static Q_INVOKABLE double fromAreaUnitToAreaUnitFactor( Qgis::AreaUnit fromUnit, Qgis::AreaUnit toUnit );
};

#endif // PROCESSINGUTILS_H
```


