

# File iosplatformutilities.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**ios**](dir_493ad750a5d458d3fc370128c0d017ff.md) **>** [**iosplatformutilities.h**](iosplatformutilities_8h.md)

[Go to the documentation of this file](iosplatformutilities_8h.md)


```C++
/***************************************************************************
    iosplatformutilities.h  -  utilities for qfield

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

#ifndef IOSPLATFORMUTILITIES_H
#define IOSPLATFORMUTILITIES_H

#include "platformutilities.h"

class ResourceSource;

class IosPlatformUtilities : public PlatformUtilities
{
  public:
    IosPlatformUtilities();

    PlatformUtilities::Capabilities capabilities() const override;
    void afterUpdate() override;

    QString systemSharedDataLocation() const override;
    QString applicationDirectory() const override;
    QStringList appDataDirs() const override;
    QStringList rootDirectories() const override { return QStringList(); }

    void setScreenLockPermission( const bool allowLock ) override;
    virtual ResourceSource *getCameraPicture( const QString &prefix,
                                              const QString &pictureFilePath,
                                              const QString &suffix,
                                              QObject *parent = nullptr ) override;
    virtual ResourceSource *getCameraVideo( const QString &prefix,
                                            const QString &videoFilePath,
                                            const QString &suffix,
                                            QObject *parent = nullptr ) override;
    virtual ResourceSource *
      getGalleryPicture( const QString &prefix, const QString &pictureFilePath, QObject *parent = nullptr ) override;
    virtual ResourceSource *
      getGalleryVideo( const QString &prefix, const QString &videoFilePath, QObject *parent = nullptr ) override;
    virtual ViewStatus *open( const QString &uri, bool editing = false, QObject *parent = nullptr ) override;
    virtual ProjectSource *openProject( QObject *parent = nullptr ) override;

    double systemFontPointSize() const override { return 16.0; }
    bool isSystemDarkTheme() const override;

    Qt::PermissionStatus checkCameraPermission() const override;
    void requestCameraPermission( std::function<void( Qt::PermissionStatus )> func ) override;
    virtual Qt::PermissionStatus checkMicrophonePermission() const override;
    virtual void requestMicrophonePermission( std::function<void( Qt::PermissionStatus )> func ) override;
};

#endif
```


