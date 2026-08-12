/***************************************************************************
                        qfapplicationinformation.cpp
                        ----------------------------
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

#include "qfapplicationinformation.h"
#include "qfield.h"
#include "qfnearfieldreader.h"

#include <qgis.h>

#include <gdal_version.h>

QfApplicationInformation::QfApplicationInformation( QObject *parent )
  : QObject( parent )
{
}

QString QfApplicationInformation::name() const
{
  return qfield::appName;
}

QString QfApplicationInformation::version() const
{
  return qfield::appVersion;
}

QString QfApplicationInformation::versionString() const
{
  return qfield::appVersionStr;
}

QString QfApplicationInformation::gitRevision() const
{
  return qfield::gitRev;
}

QString QfApplicationInformation::qtVersion() const
{
  return QString::fromLatin1( qVersion() );
}

QString QfApplicationInformation::qgisVersion() const
{
  return Qgis::version();
}

QString QfApplicationInformation::gdalVersion() const
{
  return QStringLiteral( GDAL_RELEASE_NAME );
}

bool QfApplicationInformation::hasNearFieldReader() const
{
  return QfNearFieldReader::isSupported();
}

bool QfApplicationInformation::hasBluetooth() const
{
#ifdef WITH_BLUETOOTH
  return true;
#else
  return false;
#endif
}

bool QfApplicationInformation::hasSerialPort() const
{
#ifdef WITH_SERIALPORT
  return true;
#else
  return false;
#endif
}
