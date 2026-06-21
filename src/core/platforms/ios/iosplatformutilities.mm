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

#include <QFileInfo>
#include <QGuiApplication>
#include <QStandardPaths>
#include <objc/runtime.h>
#include <qpa/qplatformnativeinterface.h>

#include <QDirIterator>
#include <qgsfileutils.h>
#include <qgsziputils.h>

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
      Capabilities() | NativeCamera | AdjustBrightness | FilePicker |
      CustomImport | CustomSend | CustomExport | UpdateProjectFromArchive;
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
static char kIosExportDelegateKey;

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
  } else if ([_mode isEqualToString:@"datasets"]) {
    [fileManager createDirectoryAtPath:_importPath
           withIntermediateDirectories:YES
                            attributes:nil
                                 error:nil];
    [url stopAccessingSecurityScopedResource];

    for (NSURL *fileUrl in urls) {
      [fileUrl startAccessingSecurityScopedResource];
      NSString *destFile = [_importPath
          stringByAppendingPathComponent:fileUrl.lastPathComponent];
      [fileManager removeItemAtPath:destFile error:nil];
      [fileManager copyItemAtPath:fileUrl.path toPath:destFile error:nil];
      [fileUrl stopAccessingSecurityScopedResource];
    }

    QString importedPath = QString::fromNSString(_importPath);
    if (AppInterface::instance()) {
      emit AppInterface::instance()->openPath(importedPath);
    }
  } else if ([_mode isEqualToString:@"updateFromArchive"]) {
    QString zipPath = QString::fromNSString(url.path);
    QString destDir = QString::fromNSString(_importPath);
    QStringList extractedFiles;
    FileUtils::unzip(zipPath, destDir, extractedFiles, false);
    [url stopAccessingSecurityScopedResource];

    if (AppInterface::instance()) {
      AppInterface::instance()->reloadProject();
    }
  }
}

- (void)documentPickerWasCancelled:
    (UIDocumentPickerViewController *)controller {
}

@end

@interface IosExportDelegate : NSObject <UIDocumentPickerDelegate>
@property(nonatomic, strong) NSArray<NSString *> *sourcePaths;
@end

@implementation IosExportDelegate

- (void)documentPicker:(UIDocumentPickerViewController *)controller
    didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
  if (urls.count == 0) {
    return;
  }

  NSURL *destURL = urls.firstObject;
  [destURL startAccessingSecurityScopedResource];
  NSFileManager *fileManager = [NSFileManager defaultManager];

  for (NSString *srcPath in _sourcePaths) {
    NSString *destPath =
        [destURL.path stringByAppendingPathComponent:srcPath.lastPathComponent];
    [fileManager removeItemAtPath:destPath error:nil];
    [fileManager copyItemAtPath:srcPath toPath:destPath error:nil];
  }

  [destURL stopAccessingSecurityScopedResource];
}

- (void)documentPickerWasCancelled:
    (UIDocumentPickerViewController *)controller {
}

@end

void IosPlatformUtilities::exportFolderTo(const QString &path) const {
  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc]
          initForOpeningContentTypes:@[ UTTypeFolder ]];
  picker.allowsMultipleSelection = NO;

  IosExportDelegate *delegate = [[IosExportDelegate alloc] init];
  delegate.sourcePaths = @[ path.toNSString() ];
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosExportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}

void IosPlatformUtilities::exportDatasetTo(const QString &path) const {
  NSMutableArray<NSString *> *sourcePaths = [NSMutableArray array];
  [sourcePaths addObject:path.toNSString()];

  const QSet<QString> sidecarFiles = QgsFileUtils::sidecarFilesForPath(path);
  for (const QString &file : sidecarFiles) {
    [sourcePaths addObject:file.toNSString()];
  }

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc]
          initForOpeningContentTypes:@[ UTTypeFolder ]];
  picker.allowsMultipleSelection = NO;

  IosExportDelegate *delegate = [[IosExportDelegate alloc] init];
  delegate.sourcePaths = sourcePaths;
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosExportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}

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

void IosPlatformUtilities::importDatasets() const {
  QString appDir = applicationDirectory();
  NSString *importBasePath =
      (appDir + QStringLiteral("/Imported Datasets/")).toNSString();

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc]
          initForOpeningContentTypes:@[ UTTypeData ]];
  picker.allowsMultipleSelection = YES;

  IosImportDelegate *delegate = [[IosImportDelegate alloc] init];
  delegate.importPath = importBasePath;
  delegate.mode = @"datasets";
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosImportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}

void IosPlatformUtilities::sendDatasetTo(const QString &path) const {
  NSMutableArray *items = [NSMutableArray array];
  [items addObject:[NSURL fileURLWithPath:path.toNSString()]];

  const QSet<QString> sidecarFiles = QgsFileUtils::sidecarFilesForPath(path);
  if (!sidecarFiles.isEmpty()) {
    QStringList paths;
    paths << path;
    for (const QString &file : sidecarFiles) {
      paths << file;
    }
    QString tempZipPath =
        QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
        QStringLiteral("/") + QFileInfo(path).fileName() +
        QStringLiteral(".zip");
    QFile::remove(tempZipPath);
    if (QgsZipUtils::zip(tempZipPath, paths)) {
      items = [NSMutableArray array];
      [items addObject:[NSURL fileURLWithPath:tempZipPath.toNSString()]];
    }
  }

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIActivityViewController *activityVC =
      [[UIActivityViewController alloc] initWithActivityItems:items
                                        applicationActivities:nil];
  activityVC.popoverPresentationController.sourceView = root.view;
  activityVC.popoverPresentationController.sourceRect =
      CGRectMake(root.view.bounds.size.width / 2.0,
                 root.view.bounds.size.height / 2.0, 1.0, 1.0);

  [root presentViewController:activityVC animated:YES completion:nil];
}

void IosPlatformUtilities::sendCompressedFolderTo(const QString &path) const {
  QString tempZipPath =
      QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
      QStringLiteral("/") + QFileInfo(path).fileName() + QStringLiteral(".zip");
  QFile::remove(tempZipPath);

  QStringList files;
  QDirIterator it(path, QDir::Files, QDirIterator::Subdirectories);
  while (it.hasNext()) {
    files << it.next();
  }

  if (files.isEmpty() || !QgsZipUtils::zip(tempZipPath, files)) {
    return;
  }

  NSMutableArray *items = [NSMutableArray array];
  [items addObject:[NSURL fileURLWithPath:tempZipPath.toNSString()]];

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIActivityViewController *activityVC =
      [[UIActivityViewController alloc] initWithActivityItems:items
                                        applicationActivities:nil];
  activityVC.popoverPresentationController.sourceView = root.view;
  activityVC.popoverPresentationController.sourceRect =
      CGRectMake(root.view.bounds.size.width / 2.0,
                 root.view.bounds.size.height / 2.0, 1.0, 1.0);

  [root presentViewController:activityVC animated:YES completion:nil];
}

void IosPlatformUtilities::updateProjectFromArchive(
    const QString &projectPath) const {
  QString importPath = QFileInfo(projectPath).absolutePath();
  NSString *importBasePath = importPath.toNSString();

  UIViewController *root = [[[[UIApplication sharedApplication] windows]
      firstObject] rootViewController];

  UIDocumentPickerViewController *picker =
      [[UIDocumentPickerViewController alloc] initForOpeningContentTypes:@[
        [UTType typeWithIdentifier:@"public.zip-archive"]
      ]];
  picker.allowsMultipleSelection = NO;

  IosImportDelegate *delegate = [[IosImportDelegate alloc] init];
  delegate.importPath = importBasePath;
  delegate.mode = @"updateFromArchive";
  picker.delegate = delegate;
  objc_setAssociatedObject(picker, &kIosImportDelegateKey, delegate,
                           OBJC_ASSOCIATION_RETAIN_NONATOMIC);

  [root presentViewController:picker animated:YES completion:nil];
}
