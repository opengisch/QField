

# File qfield.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfield.cpp**](qfield_8cpp.md)

[Go to the documentation of this file](qfield_8cpp.md)


```C++
/***************************************************************************
                        qfield.cpp
                        ----------
  begin                : August 2026
  copyright            : (C) 2026 by Mohsen Dehghanzadeh
  email                : mohsen@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfield.h"
#include "qfnearfieldreader.h"

#include <qgis.h>

#include <gdal_version.h>

QString Qfield::qtVersion() const
{
  return QString::fromLatin1( qVersion() );
}

QString Qfield::qgisVersion() const
{
  return Qgis::version();
}

QString Qfield::gdalVersion() const
{
  return QStringLiteral( GDAL_RELEASE_NAME );
}

bool Qfield::hasNfc() const
{
  return QfNearFieldReader::isSupported();
}

bool Qfield::hasBluetooth() const
{
#ifdef WITH_BLUETOOTH
  return true;
#else
  return false;
#endif
}

bool Qfield::hasSerialPort() const
{
#ifdef WITH_SERIALPORT
  return true;
#else
  return false;
#endif
}
```


