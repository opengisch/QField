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

class PictureSource;

class IosPlatformUtilities : public PlatformUtilities
{
  public:
    IosPlatformUtilities();

    PlatformUtilities::Capabilities capabilities() const override;
    void afterUpdate() override;

    QString systemSharedDataLocation() const override;
    QString applicationDirectory() const override;
    QStringList appDataDirs() const override;

    bool checkPositioningPermissions() const override;
    bool checkCameraPermissions() const override;
    void setScreenLockPermission( const bool allowLock ) override;
    virtual PictureSource *getCameraPicture( QQuickItem *parent,
                                             const QString &prefix,
                                             const QString &pictureFilePath,
                                             const QString &suffix ) override;
    virtual PictureSource *
      getGalleryPicture( QQuickItem *parent, const QString &prefix,
                         const QString &pictureFilePath ) override;
    virtual ViewStatus *open( const QString &uri, bool editing = false ) override;
    virtual ProjectSource *openProject( QObject *parent = nullptr ) override;

    double systemFontPointSize() const override { return 16.0; }
};

#endif
