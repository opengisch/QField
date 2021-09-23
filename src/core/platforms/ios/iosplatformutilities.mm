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

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import <AVFoundation/AVFoundation.h>
#include <MobileCoreServices/MobileCoreServices.h>

#include <QDebug>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>

#include <QtGui>
#include <QtQuick>
#define PICKER 6
#define CAMERA 7

#include "ioscamera.h"

IosPlatformUtilities::IosPlatformUtilities()
  : PlatformUtilities()
{

}

QString IosPlatformUtilities::systemGenericDataLocation() const
{
  NSBundle *main = [NSBundle mainBundle];
  NSString *bundlePath = [main bundlePath];
  QString path = [bundlePath UTF8String];
  return path + "/share";
}

bool IosPlatformUtilities::checkPositioningPermissions() const
{
  return true;
}

bool IosPlatformUtilities::checkCameraPermissions() const
{
  // see https://stackoverflow.com/a/20464727/1548052
  return true;
    
  NSString *mediaType = AVMediaTypeVideo;
  AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:mediaType];
  switch(authStatus)
  {
      case AVAuthorizationStatusAuthorized:
          return true;

      case AVAuthorizationStatusDenied:
          return false;

      case AVAuthorizationStatusRestricted:
         // restricted, normally won't happen
        return false;
  
      case AVAuthorizationStatusNotDetermined:
      {
        // not determined?!
        [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted){}];
        return false;
      }
  }
}
/*
@interface IosImagePickerControllerDelegate : UIViewController <UIImagePickerControllerDelegate,UINavigationControllerDelegate> {
    UIImagePickerController *imagePickerController;
}
@end

@implementation IosImagePickerControllerDelegate
- (void)pickPhoto {
    UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
    imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePicker.delegate = self;
    [self presentViewController:imagePicker animated:YES completion:nil];
}
// Implement UIImagePickerControllerDelegate method
-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary<UIImagePickerControllerInfoKey,id> *)info{
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    [picker dismissViewControllerAnimated:YES completion:nil];
}
@end
*/






/*

@interface ImagePickerDelegate : NSObject <UIImagePickerControllerDelegate, UINavigationControllerDelegate> {
                                     ImagePicker *m_imagePicker;
}
@end

@implementation ImagePickerDelegate

- (id) initWithObject:(ImagePicker *)imagePicker
{
    self = [super init];
    if (self) {
        m_imagePicker = imagePicker;
    }
    return self;
}


- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    NSString *fileName = [NSString stringWithFormat:@"/image-%f.jpg", [[NSDate date] timeIntervalSince1970]];
    path = [path stringByAppendingString:fileName];
    UIImage *image = nil;
    if([info objectForKey:UIImagePickerControllerEditedImage])
    {
        image = [info objectForKey:UIImagePickerControllerEditedImage];
    }
    else
    {
        image = [info objectForKey:UIImagePickerControllerOriginalImage];
    }
    qDebug() << m_imagePicker->imageScale();
    qDebug() << m_imagePicker->imageQuality();
    qDebug() << m_imagePicker->saveImageToCameraRoll();
    if(picker.view.tag == m_imagePicker->saveImageToCameraRoll())
    {
        qDebug() << "#####" << m_imagePicker->saveImageToCameraRoll();
        UIImageWriteToSavedPhotosAlbum([info objectForKey:UIImagePickerControllerOriginalImage], nil, nil, nil);
    }
    UIImage *uImage = [UIImage imageWithCGImage:[image CGImage]
            scale:m_imagePicker->imageScale()
            orientation:UIImageOrientationDown];
    [UIImageJPEGRepresentation(uImage, m_imagePicker->imageQuality()) writeToFile:path options:NSAtomicWrite error:nil];
    m_imagePicker->setImagePath(QString::fromNSString(path));
    [picker dismissViewControllerAnimated:YES completion:nil];
}

@end

class ImagePicker::ImagePickerDelegateContainer
{
  public:
    ImagePickerDelegate *_imagePickerDelegate = nullptr;
};

ImagePicker::ImagePicker(QQuickItem *parent) :
    QQuickItem(parent),
    m_delegate( new ImagePicker::ImagePickerDelegateContainer() ),
    m_imagePath(QString()),
    m_imageScale(1.0),
    m_imageQuality(1.0),
    m_saveImageToCameraRoll(false)
{
  m_delegate->_imagePickerDelegate = [[ImagePickerDelegate alloc] initWithObject:this];
}


void ImagePicker::openPicker()
{
    // UIView *view = static_cast<UIView *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window()));
    UIView *view = (__bridge UIView*)(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("mainWindow", window()));
    UIViewController *qtController = [[view window] rootViewController];
    UIImagePickerController *imageController = [[UIImagePickerController alloc] init];
    [imageController setSourceType:UIImagePickerControllerSourceTypePhotoLibrary];
    [imageController setAllowsEditing: YES];
    [imageController setDelegate:id(m_delegate->_imagePickerDelegate)];
    [imageController setMediaTypes: [NSArray arrayWithObjects:(NSString*)kUTTypeImage, nil]];
    [[imageController view] setTag:PICKER];
    [qtController presentViewController:imageController animated:YES completion:nil];
}

void ImagePicker::openCamera()
{
    // UIView *view = static_cast<UIView *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window()));
    UIView *view = (__bridge UIView*)(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("mainWindow", window()));
    qDebug() << view;
    UIViewController *qtController = [[view window] rootViewController];
    UIImagePickerController *imageController = [[UIImagePickerController alloc] init];
    [imageController setSourceType:UIImagePickerControllerSourceTypeCamera];
    [imageController setAllowsEditing: YES];
    [imageController setDelegate:id(m_delegate->_imagePickerDelegate)];
    [imageController setMediaTypes: [NSArray arrayWithObjects:(NSString*)kUTTypeImage, nil]];
    [[imageController view] setTag:CAMERA];
    [qtController presentViewController:imageController animated:YES completion:nil];
}
*/


ImagePicker *IosPlatformUtilities::getCameraPicture( QQuickItem* item, const QString &prefix, const QString &pictureFilePath, const QString &suffix )
{
  qDebug() << "Take the fucking picture";
  qDebug() << "parent: " << item;

  IosPictureSource *pictureSource = new IosPictureSource( prefix );

//  IosImagePickerControllerDelegate *imagePicker = [[IosImagePickerControllerDelegate alloc] init];
//  [imagePicker pickPhoto];

  ImagePicker* ip = new ImagePicker(item);
  ip->open();


  return ip;
    
    //https://stackoverflow.com/questions/42172454/ios-uiimagecontroller-delegate-undeclared-error
    
//    SATController *vc = (__bridge SATController *)object;
//    UIImagePickerController *picker = [UIImagePickerController new];
//    picker.delegate = self;
//    picker.sourceType = UIImagePickerControllerSourceTypeCamera;
//    picker.allowsEditing = YES;
//
//    [self presentModalViewController:picker animated:YES];
 
//  YourCurrentViewController *vc = (__bridge YourCurrentViewController *)object;
//  UIImagePickerController *picker = [[UIImagePickerController alloc] init];
//  picker.delegate = vc;
//  picker.allowsEditing = YES;
//  picker.sourceType = UIImagePickerControllerSourceTypeCamera;
//  //[self presentViewController:picker animated:YES completion:nil];

}

//- (void)cameraCapture {
//    UIImagePickerController *picker = [UIImagePickerController new];
//    picker.delegate = self;
//}
