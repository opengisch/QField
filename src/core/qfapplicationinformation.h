/***************************************************************************
                        qfapplicationinformation.h
                        --------------------------
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

#ifndef QFAPPLICATIONINFORMATION_H
#define QFAPPLICATIONINFORMATION_H

#include "qfield_core_export.h"

#include <QObject>
#include <QString>

/**
 * \brief Build-time facts about the running application, exposed to QML as a singleton.
 *
 * Every value here is decided when the application starts and never changes, which is
 * why the properties are CONSTANT. Reach them from QML through `import org.qfield.core`.
 *
 * \ingroup core
 */
class QFIELD_CORE_EXPORT QfApplicationInformation : public QObject
{
    Q_OBJECT

    //! Application name, e.g. `QField`
    Q_PROPERTY( QString name READ name CONSTANT )

    //! Machine readable application version, e.g. `3.6.0`
    Q_PROPERTY( QString version READ version CONSTANT )

    //! Human readable application version, e.g. `QField 3.6 Congo`
    Q_PROPERTY( QString versionString READ versionString CONSTANT )

    //! Git revision the application was built from
    Q_PROPERTY( QString gitRevision READ gitRevision CONSTANT )

    //! Version of the Qt library the application runs against
    Q_PROPERTY( QString qtVersion READ qtVersion CONSTANT )

    //! Version of the QGIS library the application runs against, e.g. `3.44.0-Solothurn`
    Q_PROPERTY( QString qgisVersion READ qgisVersion CONSTANT )

    //! Version of the GDAL library the application runs against
    Q_PROPERTY( QString gdalVersion READ gdalVersion CONSTANT )

    //! Returns TRUE when the application was built with NFC support and the device provides a reader
    Q_PROPERTY( bool hasNearFieldReader READ hasNearFieldReader CONSTANT )

    //! Returns TRUE when the application was built with Bluetooth support
    Q_PROPERTY( bool hasBluetooth READ hasBluetooth CONSTANT )

    //! Returns TRUE when the application was built with serial port support
    Q_PROPERTY( bool hasSerialPort READ hasSerialPort CONSTANT )

  public:
    explicit QfApplicationInformation( QObject *parent = nullptr );

    QString name() const;
    QString version() const;
    QString versionString() const;
    QString gitRevision() const;
    QString qtVersion() const;
    QString qgisVersion() const;
    QString gdalVersion() const;
    bool hasNearFieldReader() const;
    bool hasBluetooth() const;
    bool hasSerialPort() const;
};

#endif // QFAPPLICATIONINFORMATION_H
