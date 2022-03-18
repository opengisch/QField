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


#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDir>

#include <UIKit/UIKit.h>
#include <qpa/qplatformnativeinterface.h>
#include <MobileCoreServices/MobileCoreServices.h>

#include "iosprojectsource.h"

@interface ProjectDelegate : NSObject <UIDocumentPickerDelegate,
                                      UINavigationControllerDelegate> {
  IosProjectSource *mIosProjectSource;
}
@end

@implementation ProjectDelegate

- (id)initWithIosProjectSource:(IosProjectSource *)iosProjectSource {
  self = [super init];
  if (self) {
    mIosProjectSource = iosProjectSource;
  }
  return self;
}


- (void)documentPicker:(UIDocumentPickerViewController *)picker
  didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {
  Q_UNUSED(picker);
      qDebug() << "hey";

  for (id url in urls) {

      NSString *urlString = [url absoluteString];

      BOOL success = [url startAccessingSecurityScopedResource];

      const QString folderPath = QString::fromNSString( urlString );

      qDebug() << folderPath;

      const QString project = mIosProjectSource->projectFromFolder( folderPath );

      emit mIosProjectSource->projectOpened( project );

      // Bring back Qt's view controller:
      UIViewController *rvc =
          [[[UIApplication sharedApplication] keyWindow] rootViewController];
      [rvc dismissViewControllerAnimated:YES completion:nil];

      break;
  }
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

  UIDocumentPickerViewController *controller=[[UIDocumentPickerViewController alloc]
        initWithDocumentTypes:@[(__bridge NSString*)kUTTypeFolder]
        inMode:UIDocumentPickerModeOpen];

  [controller setDelegate:id(mDelegate->_projectDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:controller
                             animated:YES
                           completion:nil];
}

QString IosProjectSource::projectFromFolder(const QString &folder ) const
{
  qDebug() << "hoho";

  QDir directory(folder);
  if (!directory.exists())
  {
   qDebug() << "lol dir doesn't exists " << folder;
    return QString();
  }
  const QStringList projects = directory.entryList(QStringList() << "*.qgs" << "*.QGS", QDir::Files );
  for (const QString &project : projects)
    qDebug() << project;

  return projects.at(0);
}

