/***************************************************************************
  IosResourceSource.mm - IosResourceSource

  begin                : September 2021
  copyright            : (C) 2020 by Denis Rouzaud
  email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <MobileCoreServices/MobileCoreServices.h>
#include <UIKit/UIKit.h>

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <qpa/qplatformnativeinterface.h>

#include "iosresourcesource.h"
#include "stringutils.h"

@interface CameraDelegate : NSObject <UIImagePickerControllerDelegate,
                                      UINavigationControllerDelegate> {
  IosResourceSource *mIosCamera;
}
@end

@implementation CameraDelegate

- (id)initWithIosResourceSource:(IosResourceSource *)iosCamera {
  self = [super init];
  if (self) {
    mIosCamera = iosCamera;
  }
  return self;
}

- (void)imagePickerController:(UIImagePickerController *)picker
    didFinishPickingMediaWithInfo:(NSDictionary *)info {
  Q_UNUSED(picker);
  NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
  if (CFStringCompare((__bridge CFStringRef)mediaType, kUTTypeMovie, 0) ==
      kCFCompareEqualTo) {
    NSURL *videoUrl =
        (NSURL *)[info objectForKey:UIImagePickerControllerMediaURL];
    const QString originalFilename =
        QString::fromNSString([videoUrl lastPathComponent]);

    QString finalResourceFilePath = mIosCamera->resourceFilePath();
    if (!originalFilename.isEmpty()) {
      QFileInfo fi(originalFilename);
      finalResourceFilePath = StringUtils::replaceFilenameTags(
          finalResourceFilePath, fi.fileName());
    }
    NSString *path = [[NSString alloc]
        initWithUTF8String:(mIosCamera->prefixPath() + finalResourceFilePath)
                               .toUtf8()
                               .constData()];

    NSData *videoData = [NSData dataWithContentsOfURL:videoUrl];
    [videoData writeToFile:path atomically:NO];

    emit mIosCamera->resourceReceived(finalResourceFilePath);
  } else {
    NSURL *imageUrl =
        (NSURL *)[info objectForKey:UIImagePickerControllerImageURL];
    const QString originalFilename =
        QString::fromNSString([imageUrl lastPathComponent]);

    QString finalResourceFilePath = mIosCamera->resourceFilePath();
    if (!originalFilename.isEmpty()) {
      QFileInfo fi(originalFilename);
      finalResourceFilePath = StringUtils::replaceFilenameTags(
          finalResourceFilePath, fi.fileName());
    }
    NSString *path = [[NSString alloc]
        initWithUTF8String:(mIosCamera->prefixPath() + finalResourceFilePath)
                               .toUtf8()
                               .constData()];

    // Save image:
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    [UIImagePNGRepresentation(image) writeToFile:path
                                         options:NSAtomicWrite
                                           error:nil];

    emit mIosCamera->resourceReceived(finalResourceFilePath);
  }

  // Bring back Qt's view controller:
  UIViewController *rvc =
      [[[UIApplication sharedApplication] keyWindow] rootViewController];
  [rvc dismissViewControllerAnimated:YES completion:nil];
}
@end

class IosResourceSource::CameraDelegateContainer {
public:
  CameraDelegate *_cameraDelegate = nullptr;
};

IosResourceSource::IosResourceSource(const QString &prefix,
                                     const QString &resourceFilePath,
                                     QObject *parent)
    : ResourceSource(parent, prefix, resourceFilePath),
      mDelegate(new CameraDelegateContainer()) {
  mParent = qobject_cast<QQuickItem *>(parent);
  Q_ASSERT(mParent);
  mPrefixPath = prefix;
  mResourceFilePath = resourceFilePath;

  QString destinationFile = prefix + resourceFilePath;
  QFileInfo destinationInfo(destinationFile);
  QDir prefixDir(prefix);
  prefixDir.mkpath(destinationInfo.absolutePath());

  mDelegate->_cameraDelegate =
      [[CameraDelegate alloc] initWithIosResourceSource:this];
}

void IosResourceSource::takePicture() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  // Create a new image picker controller to show on top of Qt's view
  // controller:
  UIImagePickerController *imageController =
      [[UIImagePickerController alloc] init];
  [imageController setSourceType:UIImagePickerControllerSourceTypeCamera];
  [imageController
      setCameraCaptureMode:UIImagePickerControllerCameraCaptureModePhoto];
  [imageController setDelegate:id(mDelegate->_cameraDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:imageController
                             animated:YES
                           completion:nil];
}

void IosResourceSource::takeVideo() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  // Create a new image picker controller to show on top of Qt's view
  // controller:
  UIImagePickerController *imageController =
      [[UIImagePickerController alloc] init];
  [imageController setSourceType:UIImagePickerControllerSourceTypeCamera];

  imageController.mediaTypes =
      [NSArray arrayWithObject:(NSString *)kUTTypeMovie];

  [imageController setDelegate:id(mDelegate->_cameraDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:imageController
                             animated:YES
                           completion:nil];
}

void IosResourceSource::pickGalleryPicture() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  // Create a new image picker controller to show on top of Qt's view
  // controller:
  UIImagePickerController *imageController =
      [[UIImagePickerController alloc] init];
  [imageController setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
  [imageController setDelegate:id(mDelegate->_cameraDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:imageController
                             animated:YES
                           completion:nil];
}

void IosResourceSource::pickGalleryVideo() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  // Create a new image picker controller to show on top of Qt's view
  // controller:
  UIImagePickerController *imageController =
      [[UIImagePickerController alloc] init];
  [imageController setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
  [imageController setDelegate:id(mDelegate->_cameraDelegate)];

  imageController.mediaTypes =
      [NSArray arrayWithObject:(NSString *)kUTTypeMovie];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:imageController
                             animated:YES
                           completion:nil];
}
