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
#include "iosprojectsource.h"
#include "qfield.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreLocation/CoreLocation.h>
#include <MobileCoreServices/MobileCoreServices.h>
#import <UIKit/UIDocumentInteractionController.h>
#import <UIKit/UIKit.h>

#include <QGuiApplication>
#include <QStandardPaths>
#include <qpa/qplatformnativeinterface.h>

#include <QtGui>
#include <QtQuick>

IosPlatformUtilities::IosPlatformUtilities() : PlatformUtilities() {}

@interface DocViewController
    : UIViewController <UIDocumentInteractionControllerDelegate>
@end

@implementation DocViewController
#pragma mark -
#pragma mark View Life Cycle
- (void)viewDidLoad {
  [super viewDidLoad];
}
#pragma mark -
#pragma mark Document Interaction Controller Delegate Methods
- (UIViewController *)documentInteractionControllerViewControllerForPreview:
    (UIDocumentInteractionController *)controller {
  return self;
}
- (void)documentInteractionControllerDidEndPreview:
    (UIDocumentInteractionController *)controller {
  [self removeFromParentViewController];
}
@end

PlatformUtilities::Capabilities IosPlatformUtilities::capabilities() const {
  PlatformUtilities::Capabilities capabilities =
      Capabilities() | NativeCamera | AdjustBrightness | CustomLocalDataPicker;
#ifdef WITH_SENTRY
  capabilities |= SentryFramework;
#endif
  return capabilities;
}

void IosPlatformUtilities::afterUpdate() {
  // Create imported projects and datasets folders
  QDir appDir(applicationDirectory());
  appDir.mkdir(QStringLiteral("Imported Projects"));
  appDir.mkdir(QStringLiteral("Imported Datasets"));
  appDir.mkpath(QStringLiteral("QField/proj"));
  appDir.mkpath(QStringLiteral("QField/auth"));
  appDir.mkpath(QStringLiteral("QField/fonts"));
  appDir.mkpath(QStringLiteral("QField/basemaps"));
}

QString IosPlatformUtilities::systemSharedDataLocation() const {
  NSBundle *main = [NSBundle mainBundle];
  NSString *bundlePath = [main bundlePath];
  QString path = [bundlePath UTF8String];
  return path + "/share";
}

QString IosPlatformUtilities::applicationDirectory() const {
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

QStringList IosPlatformUtilities::appDataDirs() const {
  return QStringList() << QStringLiteral("%1/QField/")
                              .arg(QStandardPaths::writableLocation(
                                  QStandardPaths::DocumentsLocation));
}

bool IosPlatformUtilities::checkPositioningPermissions() const { return true; }

bool IosPlatformUtilities::checkCameraPermissions() const {
  // see https://stackoverflow.com/a/20464727/1548052
  return true;
}

void IosPlatformUtilities::setScreenLockPermission(const bool allowLock) {
  if (allowLock) {
    [UIApplication sharedApplication].idleTimerDisabled = NO;
  } else {
    [UIApplication sharedApplication].idleTimerDisabled = YES;
  }
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

ViewStatus *IosPlatformUtilities::open(const QString &uri, bool) {
  // Code from https://bugreports.qt.io/browse/QTBUG-42942
  NSString *nsFilePath = uri.toNSString();
  NSURL *nsFileUrl = [NSURL fileURLWithPath:nsFilePath];

  static DocViewController *mtv = nil;
  if (mtv != nil) {
    [mtv removeFromParentViewController];
  }

  UIDocumentInteractionController *documentInteractionController = nil;
  documentInteractionController =
      [UIDocumentInteractionController interactionControllerWithURL:nsFileUrl];
  UIViewController *rootv = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];
  if (rootv != nil) {
    mtv = [[DocViewController alloc] init];
    [rootv addChildViewController:mtv];
    documentInteractionController.delegate = mtv;
    [documentInteractionController presentPreviewAnimated:NO];
  }

  return nullptr;
}

ProjectSource *IosPlatformUtilities::openProject(QObject *parent) {
  QSettings settings;
  IosProjectSource *projectSource = new IosProjectSource(parent);
  projectSource->pickProject();
  return projectSource;
}
