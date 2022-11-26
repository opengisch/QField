/***************************************************************************
  iospicturesource.mm - IosPictureSource

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

#include <UIKit/UIKit.h>

#include <QDir>
#include <QFileInfo>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <qpa/qplatformnativeinterface.h>

#include "iospicturesource.h"

@interface CameraDelegate : NSObject <UIImagePickerControllerDelegate,
                                      UINavigationControllerDelegate> {
  IosPictureSource *mIosCamera;
}
@end

@implementation CameraDelegate

- (id)initWithIosPictureSource:(IosPictureSource *)iosCamera {
  self = [super init];
  if (self) {
    mIosCamera = iosCamera;
  }
  return self;
}

- (void)imagePickerController:(UIImagePickerController *)picker
    didFinishPickingMediaWithInfo:(NSDictionary *)info {
  Q_UNUSED(picker);

  NSString *path =
      [[NSString alloc] initWithUTF8String:(mIosCamera->prefixPath() +
                                            mIosCamera->pictureFilePath())
                                               .toUtf8()
                                               .constData()];

  // Save image:
  UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
  [UIImagePNGRepresentation(image) writeToFile:path
                                       options:NSAtomicWrite
                                         error:nil];

  // Update imagePath property to trigger QML code:
  QString filePath = /*StringLiteral("file:") +*/ QString::fromNSString(path);
  emit mIosCamera->pictureReceived(mIosCamera->pictureFilePath());

  // Bring back Qt's view controller:
  UIViewController *rvc =
      [[[UIApplication sharedApplication] keyWindow] rootViewController];
  [rvc dismissViewControllerAnimated:YES completion:nil];
}
@end

class IosPictureSource::CameraDelegateContainer {
public:
  CameraDelegate *_cameraDelegate = nullptr;
};

IosPictureSource::IosPictureSource(QObject *parent, const QString &prefix,
                                   const QString &pictureFilePath)
    : PictureSource(parent, prefix, pictureFilePath),
      mDelegate(new CameraDelegateContainer()) {
  mParent = qobject_cast<QQuickItem *>(parent);
  Q_ASSERT(mParent);
  mPrefixPath = prefix;
  mPictureFilePath = pictureFilePath;

  QString destinationFile = prefix + pictureFilePath;
  QFileInfo destinationInfo(destinationFile);
  QDir prefixDir(prefix);
  prefixDir.mkpath(destinationInfo.absolutePath());

  mDelegate->_cameraDelegate =
      [[CameraDelegate alloc] initWithIosPictureSource:this];
}

void IosPictureSource::takePicture() {
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
  [imageController setDelegate:id(mDelegate->_cameraDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:imageController
                             animated:YES
                           completion:nil];
}

void IosPictureSource::pickGalleryPicture() {
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
