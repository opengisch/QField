/***************************************************************************
                            androidplatformutilities.h  -  utilities for qfield

                              -------------------
              begin                : February 2016
              copyright            : (C) 2016 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ANDROIDPLATFORMUTILITIES_H
#define ANDROIDPLATFORMUTILITIES_H

#include "platformutilities.h"

#include <QAndroidJniObject>

class AndroidPlatformUtilities : public PlatformUtilities
{
public:
  AndroidPlatformUtilities();

  virtual QString configDir() const override;
  virtual QString shareDir() const override;
  virtual QString packagePath() const override;
  virtual QString qgsProject() const override;
  virtual PictureSource *getCameraPicture( const QString &prefix, const QString &pictureFilePath, const QString &suffix ) override;
  virtual PictureSource *getGalleryPicture( const QString &prefix, const QString &pictureFilePath ) override;
  virtual ViewStatus *open( const QString &uri ) override;
  virtual ProjectSource *openProject() override;

  virtual bool checkPositioningPermissions() const override;

  virtual bool checkCameraPermissions() const override;

  bool checkWriteExternalStoragePermissions() const override;

  void showRateThisApp() const override;

  void setScreenLockPermission( const bool allowLock ) override;

private:
  bool checkAndAcquirePermissions( const QString &permissionString ) const;
  QString getIntentExtra( const QString &, QAndroidJniObject = nullptr ) const;
  QAndroidJniObject getNativeIntent() const;
  QAndroidJniObject getNativeExtras() const;
  QAndroidJniObject mActivity;
};

#endif // ANDROIDPLATFORMUTILITIES_H
