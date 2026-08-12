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
 * \brief Describes the running application: how it was built, and what it was built against.
 *
 * Everything here is settled by the time the application is up and never changes
 * afterwards, hence the CONSTANT properties. QML reaches it through `import org.qfield.core`.
 *
 * \ingroup core
 */
class QFIELD_CORE_EXPORT QfApplicationInformation : public QObject
{
    Q_OBJECT

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

    //! QGIS version the application runs against
    Q_PROPERTY( QString qgisVersion READ qgisVersion CONSTANT )

    //! GDAL version the application runs against
    Q_PROPERTY( QString gdalVersion READ gdalVersion CONSTANT )

    //! TRUE when the build supports NFC and the device actually carries a reader
    Q_PROPERTY( bool hasNearFieldReader READ hasNearFieldReader CONSTANT )

    //! TRUE when the build supports Bluetooth
    Q_PROPERTY( bool hasBluetooth READ hasBluetooth CONSTANT )

    //! TRUE when the build supports serial ports
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
