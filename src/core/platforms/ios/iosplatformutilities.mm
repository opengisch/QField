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
#include "appinterface.h"
#include "fileutils.h"
#include "iosprojectsource.h"
#include "iosresourcesource.h"
#include "qfield.h"

#include <AVFoundation/AVFoundation.h>
#include <CoreLocation/CoreLocation.h>
#include <MobileCoreServices/MobileCoreServices.h>
#include <UIKit/UIDocumentInteractionController.h>
#include <UIKit/UIKit.h>
#include <UniformTypeIdentifiers/UniformTypeIdentifiers.h>

#include <QGuiApplication>
#include <QStandardPaths>
#include <objc/runtime.h>
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
  PlatformUtilities::Capabilities capabilities = Capabilities() | NativeCamera |
                                                 AdjustBrightness | FilePicker |
                                                 CustomImport;
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

static char kIosImportDelegateKey;

static void copyDirectoryContents(NSURL *sourceURL, NSString *destinationPath) {
  NSFileManager *fileManager = [NSFileManager defaultManager];
  [fileManager createDirectoryAtPath:destinationPath
         withIntermediateDirectories:YES
                          attributes:nil
                               error:nil];
  NSArray *contents = [fileManager contentsOfDirectoryAtURL:sourceURL
                                 includingPropertiesForKeys:nil
                                                    options:0
                                                      error:nil];
  for (NSURL *item in contents) {
    NSString *destPath =
        [destinationPath stringByAppendingPathComponent:item.lastPathComponent];
    BOOL isDir = NO;
    [fileManager fileExistsAtPath:item.path isDirectory:&isDir];
    if (isDir) {
      copyDirectoryContents(item, destPath);
    } else {
      [fileManager removeItemAtPath:destPath error:nil];
      [fileManager copyItemAtPath:item.path toPath:destPath error:nil];
    }
  }
}

@interface IosImportDelegate : NSObject <UIDocumentPickerDelegate>
@property(nonatomic, strong) NSString *importPath;
@property(nonatomic, strong) NSString *mode;
@end

@implementation IosImportDelegate

- (void)documentPicker:(UIDocumentPickerViewController *)controller
    didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
  if (urls.count == 0) {
    return;
  }

  NSURL *url = urls.firstObject;
  [url startAccessingSecurityScopedResource];
  NSFileManager *fileManager = [NSFileManager defaultManager];

  if ([_mode isEqualToString:@"projectFolder"]) {
    NSString *folderName = url.lastPathComponent;
    NSString *destPath =
        [_importPath stringByAppendingPathComponent:folderName];
    [fileManager createDirectoryAtPath:_importPath
           withIntermediateDirectories:YES
                            attributes:nil
                                 error:nil];
    copyDirectoryContents(url, destPath);
    [url stopAccessingSecurityScopedResource];

    QString importedPath = QString::fromNSString(destPath);
    if (AppInterface::instance()) {
      emit AppInterface::instance()->openPath(importedPath);
    }
  } else if ([_mode isEqualToString:@"projectArchive"]) {
    NSString *baseName = [url.lastPathComponent stringByDeletingPathExtension];
    NSString *destPath = [_importPath stringByAppendingPathComponent:baseName];
    [fileManager createDirectoryAtPath:destPath
           withIntermediateDirectories:YES
                            attributes:nil
                                 error:nil];

    QString zipPath = QString::fromNSString(url.path);
    QString destDir = QString::fromNSString(destPath);
    QStringList extractedFiles;
    FileUtils::unzip(zipPath, destDir, extractedFiles, false);
    [url stopAccessingSecurityScopedResource];

    if (AppInterface::instance()) {
      emit AppInterface::instance()->openPath(destDir);
    }
  }
}

- (void)documentPickerWasCancelled:
    (UIDocumentPickerViewController *)controller {
}

@end

void IosPlatformUtilities::importProjectFolder() const {
  QString appDir = applicationDirectory();
  NSString *importBasePath =
      (appDir + QStringLiteral("/Imported Projects/")).toNSString();

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc]
          initForOpeningContentTypes:@[ UTTypeFolder ]];
  picker.allowsMultipleSelection = NO;

  IosImportDelegate *delegate = [[IosImportDelegate alloc] init];
  delegate.importPath = importBasePath;
  delegate.mode = @"projectFolder";
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosImportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}

void IosPlatformUtilities::importProjectArchive() const {
  QString appDir = applicationDirectory();
  NSString *importBasePath =
      (appDir + QStringLiteral("/Imported Projects/")).toNSString();

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc] initForOpeningContentTypes:@[
        [UTType typeWithIdentifier:@"public.zip-archive"]
      ]];
  picker.allowsMultipleSelection = NO;

  IosImportDelegate *delegate = [[IosImportDelegate alloc] init];
  delegate.importPath = importBasePath;
  delegate.mode = @"projectArchive";
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosImportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}
