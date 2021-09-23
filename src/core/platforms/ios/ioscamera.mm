#include <UIKit/UIKit.h>

#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include <QQuickItem>
#include <QQuickWindow>

#include "ioscamera.h"

@interface CameraDelegate : NSObject <UIImagePickerControllerDelegate, UINavigationControllerDelegate> {
    ImagePicker *m_iosCamera;
}
@end

@implementation CameraDelegate

- (id) initWithImagePicker:(ImagePicker *)iosCamera
{
    self = [super init];
    if (self) {
        m_iosCamera = iosCamera;
    }
    return self;
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    Q_UNUSED(picker);

    // Create the path where we want to save the image:
    NSString *path = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    path = [path stringByAppendingString:@"/capture.png"];

    // Save image:
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    [UIImagePNGRepresentation(image) writeToFile:path options:NSAtomicWrite error:nil];

    // Update imagePath property to trigger QML code:
    m_iosCamera->m_imagePath = QStringLiteral("file:") + QString::fromNSString(path);
    emit m_iosCamera->imagePathChanged();

    // Bring back Qt's view controller:
    UIViewController *rvc = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    [rvc dismissViewControllerAnimated:YES completion:nil];
}

@end


class ImagePicker::CameraDelegateContainer
{
  public:
    CameraDelegate *_cameraDelegate = nullptr;
};

ImagePicker::ImagePicker(QQuickItem *parent)
    : QQuickItem(parent)
    , m_delegate( new CameraDelegateContainer() )
{
  m_delegate->_cameraDelegate = [[CameraDelegate alloc] initWithImagePicker:this];
}

void ImagePicker::open()
{
    // Get the UIView that backs our QQuickWindow:
    // UIView *view = static_cast<UIView *>(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window()));
    UIView *view = (__bridge UIView*)(QGuiApplication::platformNativeInterface()->nativeResourceForWindow("uiview", window()));
    UIViewController *qtController = [[view window] rootViewController];

    // Create a new image picker controller to show on top of Qt's view controller:
    UIImagePickerController *imageController = [[UIImagePickerController alloc] init];
    [imageController setSourceType:UIImagePickerControllerSourceTypeCamera];
    [imageController setDelegate:id(m_delegate->_cameraDelegate)];

    // Tell the imagecontroller to animate on top:
    [qtController presentViewController:imageController animated:YES completion:nil];
}
