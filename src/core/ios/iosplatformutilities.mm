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

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import <AVFoundation/AVFoundation.h>

IosPlatformUtilities::IosPlatformUtilities()
  : PlatformUtilities()
{

}

QString IosPlatformUtilities::packagePath() const
{
  NSBundle *main = [NSBundle mainBundle];
  NSString *bundlePath = [main bundlePath];
  QString path = [bundlePath UTF8String];
  return path;
}


bool IosPlatformUtilities::checkPositioningPermissions() const
{
  CLAuthorizationStatus status = [CLLocationManager authorizationStatus];

  while (true)
  {
    switch (status)
    {
        case kCLAuthorizationStatusNotDetermined:
        {
            CLLocationManager *locationManager = [[CLLocationManager alloc] init];
            [locationManager requestWhenInUseAuthorization];
            break;
        }
            
        case kCLAuthorizationStatusAuthorizedAlways:
        case kCLAuthorizationStatusAuthorizedWhenInUse:
            return true;

        case kCLAuthorizationStatusRestricted:
            //The user can't choose whether or not your app can use location services or not, this could be due to parental controls for example.
            // TODO: check?
            return true;

        case kCLAuthorizationStatusDenied:
            return false;

        default:
            return false;
    }
  }
}


bool IosPlatformUtilities::checkCameraPermissions() const
{
  // see https://stackoverflow.com/a/20464727/1548052
    
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
        [AVCaptureDevice requestAccessForMediaType:mediaType completionHandler:^(BOOL granted) {
          if(granted){
            return true;
          } else {
            return false;
          }
        }];
      }
}
