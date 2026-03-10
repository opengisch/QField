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
#include "iosprojectsource.h"
#include "iosresourcesource.h"
#include "qfield.h"

#include <AVFoundation/AVFoundation.h>
#include <CoreLocation/CoreLocation.h>
#include <MobileCoreServices/MobileCoreServices.h>
#include <UIKit/UIDocumentInteractionController.h>
#include <UIKit/UIKit.h>

#include <QGuiApplication>
#include <QStandardPaths>
#include <qpa/qplatformnativeinterface.h>

#include <QtGui>
#include <QtQuick>

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

IosPlatformUtilities::IosPlatformUtilities() : PlatformUtilities() {
  NSError *sessionError = nil;
  [[AVAudioSession sharedInstance]
      setCategory:AVAudioSessionCategoryPlayAndRecord
            error:&sessionError];
}

PlatformUtilities::Capabilities IosPlatformUtilities::capabilities() const {
  PlatformUtilities::Capabilities capabilities =
      Capabilities() | NativeCamera | AdjustBrightness | FilePicker;
#if WITH_SENTRY
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
  appDir.mkpath(QStringLiteral("QField/logs"));
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

void IosPlatformUtilities::setScreenLockPermission(const bool allowLock) {
  if (allowLock) {
    [UIApplication sharedApplication].idleTimerDisabled = NO;
  } else {
    [UIApplication sharedApplication].idleTimerDisabled = YES;
  }
}

ResourceSource *
IosPlatformUtilities::getCameraPicture(const QString &prefix,
                                       const QString &pictureFilePath,
                                       const QString &suffix, QObject *parent) {
  IosResourceSource *pictureSource =
      new IosResourceSource(prefix, pictureFilePath, parent);
  pictureSource->takePicture();
  return pictureSource;
}

ResourceSource *
IosPlatformUtilities::getCameraVideo(const QString &prefix,
                                     const QString &videoFilePath,
                                     const QString &suffix, QObject *parent) {
  IosResourceSource *pictureSource =
      new IosResourceSource(prefix, videoFilePath, parent);
  pictureSource->takeVideo();
  return pictureSource;
}

ResourceSource *IosPlatformUtilities::getGalleryPicture(
    const QString &prefix, const QString &pictureFilePath, QObject *parent) {
  IosResourceSource *pictureSource =
      new IosResourceSource(prefix, pictureFilePath, parent);
  pictureSource->pickGalleryPicture();
  return pictureSource;
}

ResourceSource *IosPlatformUtilities::getGalleryVideo(
    const QString &prefix, const QString &videoFilePath, QObject *parent) {
  IosResourceSource *videoSource =
      new IosResourceSource(prefix, videoFilePath, parent);
  videoSource->pickGalleryVideo();
  return videoSource;
}

ViewStatus *IosPlatformUtilities::open(const QString &uri, bool, QObject *) {
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

bool IosPlatformUtilities::isSystemDarkTheme() const {
  if (@available(iOS 12.0, *)) {
    switch (UIScreen.mainScreen.traitCollection.userInterfaceStyle) {
    case UIUserInterfaceStyleDark:
      return true;

    case UIUserInterfaceStyleLight:
    case UIUserInterfaceStyleUnspecified:
    default:
      return false;
    }
  }
  return false;
}

Qt::PermissionStatus IosPlatformUtilities::checkCameraPermission() const {
  switch ([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo]) {
  case AVAuthorizationStatusNotDetermined: {
    return Qt::PermissionStatus::Undetermined;
  }
  case AVAuthorizationStatusAuthorized: {
    return Qt::PermissionStatus::Granted;
  }
  case AVAuthorizationStatusDenied:
  case AVAuthorizationStatusRestricted: {
    return Qt::PermissionStatus::Denied;
  }
    return Qt::PermissionStatus::Undetermined;
  }
}

void IosPlatformUtilities::requestCameraPermission(
    std::function<void(Qt::PermissionStatus)> func) {
  [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                           completionHandler:^(BOOL granted) {
                               func(granted ? Qt::PermissionStatus::Granted
                                            : Qt::PermissionStatus::Denied);
                           }];
}

Qt::PermissionStatus IosPlatformUtilities::checkMicrophonePermission() const {
  switch ([AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio]) {
  case AVAuthorizationStatusNotDetermined: {
    return Qt::PermissionStatus::Undetermined;
  }
  case AVAuthorizationStatusAuthorized: {
    return Qt::PermissionStatus::Granted;
  }
  case AVAuthorizationStatusDenied:
  case AVAuthorizationStatusRestricted: {
    return Qt::PermissionStatus::Denied;
  }
    return Qt::PermissionStatus::Undetermined;
  }
}

void IosPlatformUtilities::requestMicrophonePermission(
    std::function<void(Qt::PermissionStatus)> func) {
  [AVCaptureDevice requestAccessForMediaType:AVMediaTypeAudio
                           completionHandler:^(BOOL granted) {
                               func(granted ? Qt::PermissionStatus::Granted
                                            : Qt::PermissionStatus::Denied);
                           }];
}
