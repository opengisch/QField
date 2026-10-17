

# File qfield.h.in

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfield.h.in**](qfield_8h_8in.md)

[Go to the documentation of this file](qfield_8h_8in.md)


```C++
/***************************************************************************
                        qfield.h
                        ---------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELD_H
#define QFIELD_H

#include "qfield_core_export.h"

#include <QObject>
#include <QString>

/**
 * \brief Describes the running application: how it was built, and what it was built against.
 *
 * Everything here is settled by the time the application is up and never changes
 * afterwards, hence the CONSTANT properties. QML reaches it as `Qfield` through `import org.qfield.core`.
 *
 * \ingroup core
 */
class QFIELD_CORE_EXPORT Qfield
{
    Q_GADGET

    //! Name the build ships under
    Q_PROPERTY( QString name READ name CONSTANT )

    //! Release tag the build came from
    Q_PROPERTY( QString version READ version CONSTANT )

    //! Version as shown to users
    Q_PROPERTY( QString versionString READ versionString CONSTANT )

    //! Full SHA of the commit the build came from
    Q_PROPERTY( QString gitRevision READ gitRevision CONSTANT )

    //! Qt version the application runs against
    Q_PROPERTY( QString qtVersion READ qtVersion CONSTANT )

    //! QGIS version the application runs against, formatted `<major>.<minor>.<patch>-<release name>`
    Q_PROPERTY( QString qgisVersion READ qgisVersion CONSTANT )

    //! GDAL version the application runs against
    Q_PROPERTY( QString gdalVersion READ gdalVersion CONSTANT )

    //! TRUE when the build supports NFC and the device provides it
    Q_PROPERTY( bool hasNfc READ hasNfc CONSTANT )

    //! TRUE when the build supports Bluetooth
    Q_PROPERTY( bool hasBluetooth READ hasBluetooth CONSTANT )

    //! TRUE when the build supports serial ports
    Q_PROPERTY( bool hasSerialPort READ hasSerialPort CONSTANT )

  public:
    static inline const QString appName{ QStringLiteral( "@APP_NAME@" ) };
    static inline const QString appVersion{ QStringLiteral( "@APP_VERSION@" ) };
    static inline const QString appVersionStr{ QStringLiteral( "@APP_VERSION_STR@" ) };
    static inline const QString gitRev{ QStringLiteral( "@GIT_REV@" ) };

    QString name() const { return appName; }
    QString version() const { return appVersion; }
    QString versionString() const { return appVersionStr; }
    QString gitRevision() const { return gitRev; }
    QString qtVersion() const;
    QString qgisVersion() const;
    QString gdalVersion() const;
    bool hasNfc() const;
    bool hasBluetooth() const;
    bool hasSerialPort() const;
};

#cmakedefine QFIELDDEBUG

#define WITH_SENTRY @WITH_SENTRY@
#define QGIS_PREFIX_PATH "@QGIS_PREFIX_PATH@"

#define SUPPORTED_PROJECT_EXTENSIONS QStringList( { QStringLiteral( "qgs" ), QStringLiteral( "qgz" ) } )
#define SUPPORTED_VECTOR_EXTENSIONS QStringList( { QStringLiteral( "gpkg" ), QStringLiteral( "shp" ), QStringLiteral( "kml" ), QStringLiteral( "kmz" ), QStringLiteral( "geojson" ), QStringLiteral( "json" ), QStringLiteral( "pdf" ), QStringLiteral( "gpx" ), QStringLiteral( "gml" ), QStringLiteral( "mif" ), QStringLiteral( "fgb" ), QStringLiteral( "db" ), QStringLiteral( "sqlite" ), QStringLiteral( "mbtiles" ), QStringLiteral( "vtpk" ), QStringLiteral( "vrt" ), QStringLiteral( "zip" ), QStringLiteral( "7z" ), QStringLiteral( "rar" ) } )
#define SUPPORTED_RASTER_EXTENSIONS QStringList( { QStringLiteral( "tif" ), QStringLiteral( "tiff" ), QStringLiteral( "pdf" ), QStringLiteral( "jpg" ), QStringLiteral( "jpeg" ), QStringLiteral( "png" ), QStringLiteral( "gpkg" ), QStringLiteral( "jp2" ), QStringLiteral( "webp" ), QStringLiteral( "mbtiles" ), QStringLiteral( "vrt" ), QStringLiteral( "zip" ), QStringLiteral( "7z" ) } )
#define SUPPORTED_FILE_EXTENSIONS QStringList( { QStringLiteral( "log" ), QStringLiteral( "txt" ), QStringLiteral( "mp4" ), QStringLiteral( "m4a" ), QStringLiteral( "ogg" ), QStringLiteral( "mp3" ) } )
#define SUPPORTED_DATASET_THUMBNAIL QStringList( { QStringLiteral( "zip" ), QStringLiteral( "tif" ), QStringLiteral( "tiff" ), QStringLiteral( "pdf" ), QStringLiteral( "jpg" ), QStringLiteral( "jpeg" ), QStringLiteral( "png" ), QStringLiteral( "jp2" ), QStringLiteral( "webp" ) } )

#endif // QFIELD_H
```


