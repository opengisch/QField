/***************************************************************************
                            platformutilities.h  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PLATFORMUTILITIES_H
#define PLATFORMUTILITIES_H

#include <QObject>
#include <qgsfield.h>
#include "picturesource.h"

class ProjectSource;

class PlatformUtilities : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString configDir READ configDir )
    Q_PROPERTY( QString shareDir READ shareDir )

  public:
    virtual ~PlatformUtilities();

    virtual QString configDir() const;
    virtual QString shareDir() const;
    virtual QString qgsProject() const;
    Q_INVOKABLE bool createDir( const QString &path, const QString &dirname ) const;
    Q_INVOKABLE bool rmFile( const QString &filename ) const;
    Q_INVOKABLE bool renameFile( const QString &filename, const QString &newname ) const;

    /**
     * Get a picture and copy it to the requested prefix
     * @param prefix The folder where the picture should be put
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual PictureSource *getPicture( const QString &prefix );

    /**
     * Open the resource (file, image, ...) that is available under \a uri.
     * A \a mimeType can be provided to indicate the system how the file should
     * be opened.
     */
    Q_INVOKABLE virtual void open( const QString &uri, const QString &mimeType );

    /**
     * Returns the QVariant typeName of a \a field.
     * This is a stable identifier (compared to the provider field name).
     */
    Q_INVOKABLE QString fieldType( const QgsField &field ) const;

    /**
     * Indicates the system that we want to open a project.
     * The system shall show a suitable user interface element (like a filebrowser)
     * to let the user select a project.
     * The call returns immediately and the returned ProjectSource will notify
     * when the project has actually been chosen.
     */
    Q_INVOKABLE virtual ProjectSource *openProject();

    /**
     * Checks for positioning (GPS etc) permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     * It will return true, if at least coarse permissions are granted. It will
     * ask for fine permissions if none are granted.
     */
    Q_INVOKABLE virtual bool checkPositioningPermissions() const;

    /**
     * Checks for camera permissions on the device.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     */
    Q_INVOKABLE virtual bool checkCameraPermissions() const;

    /**
     * Checks for permissions to write exeternal storage.
     * If the permissions are not given, the user will be asked to grant
     * permissions.
     */
    Q_INVOKABLE virtual bool checkWriteExternalStoragePermissions() const;

  /**
   * Show the rate this app screen if required.
   */
    Q_INVOKABLE virtual void showRateThisApp() const {};

};
#endif // PLATFORMUTILITIES_H
