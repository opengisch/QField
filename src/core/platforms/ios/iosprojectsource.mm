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

#include <QDir>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>

#include <MobileCoreServices/MobileCoreServices.h>
#include <UIKit/UIKit.h>
#include <qpa/qplatformnativeinterface.h>

#include "iosprojectsource.h"

static NSString *const FIELD_URI = @"uri";
static NSString *const FIELD_FILE_COPY_URI = @"fileCopyUri";
static NSString *const FIELD_COPY_ERR = @"copyError";
static NSString *const FIELD_NAME = @"name";

@interface ProjectDelegate
    : NSObject <UIDocumentPickerDelegate, UINavigationControllerDelegate> {
  IosProjectSource *mIosProjectSource;
}
@end

@implementation ProjectDelegate {
  NSMutableArray *urlsInOpenMode;
}

- (id)initWithIosProjectSource:(IosProjectSource *)iosProjectSource {
  self = [super init];
  if (self) {
    mIosProjectSource = iosProjectSource;
    urlsInOpenMode = [NSMutableArray new];
  }
  return self;
}

- (void)dealloc {
  for (NSURL *url in urlsInOpenMode) {
    [url stopAccessingSecurityScopedResource];
  }
}

+ (BOOL)requiresMainQueueSetup {
  return NO;
}

- (dispatch_queue_t)methodQueue {
  return dispatch_get_main_queue();
}

- (void)documentPicker:(UIDocumentPickerViewController *)picker
    didPickDocumentsAtURLs:(NSArray<NSURL *> *)urls {

  NSMutableArray *results = [NSMutableArray array];
  for (id url in urls) {
    NSError *error;
    NSMutableDictionary *result = [self getMetadataForUrl:url error:&error];
    const QString projectPath = mIosProjectSource->projectFromFolder(
        QString::fromNSString(result[FIELD_FILE_COPY_URI]));
    emit mIosProjectSource->projectOpened(projectPath);
    break;
  }
}

- (NSMutableDictionary *)getMetadataForUrl:(NSURL *)url
                                     error:(NSError **)error {
  __block NSMutableDictionary *result = [NSMutableDictionary dictionary];

  [urlsInOpenMode addObject:url];

  // TODO handle error
  [url startAccessingSecurityScopedResource];

  NSFileCoordinator *coordinator = [NSFileCoordinator new];
  NSError *fileError;

  // TODO double check this implemenation, see eg.
  // https://developer.apple.com/documentation/foundation/nsfilecoordinator/1412420-prepareforreadingitemsaturls
  [coordinator
      coordinateReadingItemAtURL:url
                         options:NSFileCoordinatorReadingResolvesSymbolicLink
                           error:&fileError
                      byAccessor:^(NSURL *newURL) {
                          // If the coordinated operation fails, then the
                          // accessor block never runs
                          result[FIELD_URI] = url.absoluteString;

                          NSError *copyError;
                          NSString *copyPath =
                              [ProjectDelegate
                                  copyToUniqueDestinationFrom:newURL
                                                        error:copyError]
                                  .absoluteString;

                          if (!copyError) {
                            result[FIELD_FILE_COPY_URI] = copyPath;
                          } else {
                            result[FIELD_COPY_ERR] =
                                copyError.localizedDescription;
                            result[FIELD_FILE_COPY_URI] = [NSNull null];
                          }

                          result[FIELD_NAME] = newURL.lastPathComponent;
                      }];

  if (fileError) {
    *error = fileError;
    return nil;
  } else {
    return result;
  }
}

- (void)documentPickerWasCancelled:
    (UIDocumentPickerViewController *)controller {
  [self rejectAsUserCancellationError];
}

- (void)presentationControllerDidDismiss:
    (UIPresentationController *)presentationController {
  [self rejectAsUserCancellationError];
}

- (void)rejectAsUserCancellationError {
  // TODO make error nullable?
  NSError *error = [NSError errorWithDomain:NSCocoaErrorDomain
                                       code:NSUserCancelledError
                                   userInfo:nil];
  //[promiseWrapper reject:@"user canceled the document picker"
  // withCode:E_DOCUMENT_PICKER_CANCELED withError:error];
}

+ (NSURL *)copyToUniqueDestinationFrom:(NSURL *)url error:(NSError *)error {
  // destination can be NSDocumentDirectory or NSCachesDirectory
  NSURL *destinationRootDir =
      [NSFileManager.defaultManager URLsForDirectory:NSDocumentDirectory
                                           inDomains:NSUserDomainMask]
          .firstObject;
  // we don't want to rename the file so we put it into a unique location
  NSString *uniqueSubDirName = [[NSUUID UUID] UUIDString];
  NSURL *destinationDir = [destinationRootDir
      URLByAppendingPathComponent:[NSString stringWithFormat:@"%@/",
                                                             uniqueSubDirName]];
  NSURL *destinationUrl = [destinationDir
      URLByAppendingPathComponent:[NSString
                                      stringWithFormat:@"%@",
                                                       url.lastPathComponent]];

  [NSFileManager.defaultManager createDirectoryAtURL:destinationDir
                         withIntermediateDirectories:YES
                                          attributes:nil
                                               error:&error];
  if (error) {
    return url;
  }
  [NSFileManager.defaultManager copyItemAtURL:url
                                        toURL:destinationUrl
                                        error:&error];
  if (error) {
    return url;
  } else {
    return destinationUrl;
  }
}

@end

class IosProjectSource::ProjectDelegateContainer {
public:
  ProjectDelegate *_projectDelegate = nullptr;
};

IosProjectSource::IosProjectSource(QObject *parent)
    : ProjectSource(parent), mDelegate(new ProjectDelegateContainer()) {
  mParent = qobject_cast<QQuickItem *>(parent);
  Q_ASSERT(mParent);
  mDelegate->_projectDelegate =
      [[ProjectDelegate alloc] initWithIosProjectSource:this];
}

void IosProjectSource::pickProject() {
  // Get the UIView that backs our QQuickWindow:
  UIView *view = (__bridge UIView *)(QGuiApplication::platformNativeInterface()
                                         ->nativeResourceForWindow(
                                             "uiview", mParent->window()));
  UIViewController *qtController = [[view window] rootViewController];

  UIDocumentPickerViewController *controller =
      [[UIDocumentPickerViewController alloc]
          initWithDocumentTypes:@[ (__bridge NSString *)kUTTypeFolder ]
                         inMode:UIDocumentPickerModeOpen
          // inMode:UIDocumentPickerModeImport
  ];

  [controller setDelegate:id(mDelegate->_projectDelegate)];

  // Tell the imagecontroller to animate on top:
  [qtController presentViewController:controller animated:YES completion:nil];
}

QString IosProjectSource::projectFromFolder(const QString &folder) const {
  QString folderPath = folder;
  folderPath.remove(QStringLiteral("file://"));
  QDir directory(folderPath);
  if (!directory.exists()) {
    return;
  }
  const QStringList projects = directory.entryList(QStringList() << "*.qgs"
                                                                 << "*.QGS"
                                                                 << "*.qgz"
                                                                 << "*.QGZ",
                                                   QDir::Files);
  if (projects.count() > 0) {
    const QString projectPath =
        QStringLiteral("%1/%2").arg(folderPath, projects.at(0));
    return projectPath;
  } else {
  }
  return QString();
}
