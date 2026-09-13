

# File qfiosplatformutilities.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**ios**](dir_493ad750a5d458d3fc370128c0d017ff.md) **>** [**qfiosplatformutilities.h**](qfiosplatformutilities_8h.md)

[Go to the documentation of this file](qfiosplatformutilities_8h.md)


```C++
/***************************************************************************
    qfiosplatformutilities.h  -  utilities for qfield

      -------------------
    begin                : November 2020
    copyright            : (C) 2020 by Denis Rouzaud
    email                : denis@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIOSPLATFORMUTILITIES_H
#define QFIOSPLATFORMUTILITIES_H

#include "qfplatformutilities.h"

class QfResourceSource;

class QfIosPlatformUtilities : public QfPlatformUtilities
{
  public:
    QfIosPlatformUtilities();

    QfPlatformUtilities::Capabilities capabilities() const override;
    void afterUpdate() override;

    QString systemSharedDataLocation() const override;
    QString applicationDirectory() const override;
    QStringList appDataDirs() const override;
    QStringList rootDirectories() const override { return QStringList(); }

    void importProjectFolder() const override;
    void importProjectArchive() const override;
    void importDatasets() const override;
    void exportDatasetTo( const QString &path ) const override;
    void exportFolderTo( const QString &path ) const override;
    void sendDatasetTo( const QString &path ) const override;
    void sendCompressedFolderTo( const QString &path ) const override;
    void updateProjectFromArchive( const QString &projectPath ) const override;

    void setScreenLockPermission( const bool allowLock ) override;
    virtual QfResourceSource *getCameraPicture( const QString &prefix,
                                                const QString &pictureFilePath,
                                                const QString &suffix,
                                                QObject *parent = nullptr ) override;
    virtual QfResourceSource *getCameraVideo( const QString &prefix,
                                              const QString &videoFilePath,
                                              const QString &suffix,
                                              QObject *parent = nullptr ) override;
    virtual QfResourceSource *
      getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent = nullptr ) override;
    virtual QfResourceSource *
      getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent = nullptr ) override;
    virtual QfViewStatus *open( const QString &uri, bool editing = false, QObject *parent = nullptr ) override;
    virtual QfProjectSource *openProject( QObject *parent = nullptr ) override;

    double systemFontPointSize() const override { return 16.0; }
    bool isSystemDarkTheme() const override;

    Qt::PermissionStatus checkCameraPermission() const override;
    void requestCameraPermission( std::function<void( Qt::PermissionStatus )> func ) override;
    virtual Qt::PermissionStatus checkMicrophonePermission() const override;
    virtual void requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func ) override;
};

#endif
```


