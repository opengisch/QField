/***************************************************************************
                            iosplatformutilities.mm  -  utilities for qfield

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

#include "iosplatformutilities.h"
#include "iospicturesource.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreLocation/CoreLocation.h>
#include <MobileCoreServices/MobileCoreServices.h>
#import <UIKit/UIKit.h>

#include <QDebug>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include <QtGui>
#include <QtQuick>

IosPlatformUtilities::IosPlatformUtilities() : PlatformUtilities() {}

QString IosPlatformUtilities::systemGenericDataLocation() const {
  NSBundle *main = [NSBundle mainBundle];
  NSString *bundlePath = [main bundlePath];
  QString path = [bundlePath UTF8String];
  return path + "/share";
}

bool IosPlatformUtilities::checkPositioningPermissions() const { return true; }

bool IosPlatformUtilities::checkCameraPermissions() const {
  // see https://stackoverflow.com/a/20464727/1548052
  return true;
}

PictureSource *IosPlatformUtilities::getCameraPicture(
    QQuickItem *parent, const QString &prefix, const QString &pictureFilePath,
    const QString &suffix) {
  IosPictureSource *pictureSource =
      new IosPictureSource(parent, prefix, pictureFilePath);
  pictureSource->takePicture();
  return pictureSource;
}

PictureSource *IosPlatformUtilities::getGalleryPicture(
    QQuickItem *parent, const QString &prefix, const QString &pictureFilePath) {
  IosPictureSource *pictureSource =
      new IosPictureSource(parent, prefix, pictureFilePath);
  pictureSource->pickGalleryPicture();
  return pictureSource;
}
