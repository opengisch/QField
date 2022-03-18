/***************************************************************************
  iosprojectsource.mm - IosProjectSource

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

#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <qpa/qplatformnativeinterface.h>
//#include <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#include <MobileCoreServices/MobileCoreServices.h>

#include "iosprojectsource.h"

@interface ProjectDelegate : NSObject <UIDocumentPickerDelegate,
                                      UINavigationControllerDelegate> {
  IosProjectSource *mIosProject;
}
@end

@implementation ProjectDelegate

- (id)initWithIosProjectSource:(IosProjectSource *)iosProject {
  self = [super init];
  if (self) {
    mIosProject = iosProject;
  }
  return self;
}

- (void)documentPickerController:(UIDocumentPickerViewController *)picker
    didPickDocumentAt:(NSURL *)url {
  Q_UNUSED(picker);

  NSString *urlStr = url.absoluteString;

  // Update imagePath property to trigger QML code:
  QString filePath = /*StringLiteral("file:") +*/ QString::fromNSString(urlStr);
  emit mIosProject->projectReceived(filePath);

  // Bring back Qt's view controller:
  UIViewController *rvc =
      [[[UIApplication sharedApplication] keyWindow] rootViewController];
  [rvc dismissViewControllerAnimated:YES completion:nil];
}
@end

class IosProjectSource::ProjectDelegateContainer {
public:
  ProjectDelegate *_projectDelegate = nullptr;
};

IosProjectSource::IosProjectSource(QObject *parent)
    : ProjectSource(parent),
      mDelegate(new ProjectDelegateContainer()) {
  mParent = qobject_cast<QQuickItem *>(parent);
  Q_ASSERT(mParent);
  mDelegate->_projectDelegate = [[ProjectDelegate alloc] initWithIosProjectSource:this];
}

void IosProjectSource::pickProject() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  //UTType *folderType = [UTType .folder];

   // NSArray *documentTypes = @[ folder ];

  UIDocumentPickerViewController *picker=[[UIDocumentPickerViewController alloc]
        initWithDocumentTypes:@[(__bridge NSString*)kUTTypeFolder]
        inMode:UIDocumentPickerModeOpen];

   //   forOpeningContentTypes: @[@".folder"]];

  //          initWithDocumentTypes:@[@"public.data"]
  //    inMode:UIDocumentPickerModeImport];

  //  NSArray *types = @[(NSString*)kUTTypeImage,(NSString*)kUTTypeSpreadsheet,(NSString*)kUTTypePresentation,(NSString*)kUTTypeDatabase,(NSString*)kUTTypeFolder,(NSString*)kUTTypeZipArchive,(NSString*)kUTTypeVideo];
  
  [picker setDelegate:id(mDelegate->_projectDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:picker
                             animated:YES
                           completion:nil];
}
