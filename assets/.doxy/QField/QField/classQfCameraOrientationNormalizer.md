

# Class QfCameraOrientationNormalizer



[**ClassList**](annotated.md) **>** [**QfCameraOrientationNormalizer**](classQfCameraOrientationNormalizer.md)



_Compensates for incorrect camera orientation on iOS and Windows, and applies user-requested photo adjustments on all platforms._ [More...](#detailed-description)

* `#include <qfcameraorientationnormalizer.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**CameraPosition**](#enum-cameraposition)  <br>_Camera position, matching the integer values of QCameraDevice::Position._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCameraOrientationNormalizer::CameraPosition**](classQfCameraOrientationNormalizer.md#enum-cameraposition) | [**cameraPosition**](classQfCameraOrientationNormalizer.md#property-cameraposition-12)  <br> |
| property int | [**previewRotation**](classQfCameraOrientationNormalizer.md#property-previewrotation-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cameraPositionChanged**](classQfCameraOrientationNormalizer.md#signal-camerapositionchanged)  <br> |
| signal void | [**previewRotationChanged**](classQfCameraOrientationNormalizer.md#signal-previewrotationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCameraOrientationNormalizer**](#function-qfcameraorientationnormalizer) (QObject \* parent=nullptr) <br>_Constructs the normalizer and starts tracking screen orientation._  |
|  Q\_INVOKABLE bool | [**applyEditsToImage**](#function-applyeditstoimage) (const QString & path, int rotation, bool mirror) <br> |
|  [**QfCameraOrientationNormalizer::CameraPosition**](classQfCameraOrientationNormalizer.md#enum-cameraposition) | [**cameraPosition**](#function-cameraposition-22) () const<br> |
|  Q\_INVOKABLE bool | [**normalizeImageOrientation**](#function-normalizeimageorientation) (const QString & path) <br> |
|  int | [**previewRotation**](#function-previewrotation-22) () const<br> |
|  Q\_INVOKABLE void | [**recordCaptureOrientation**](#function-recordcaptureorientation) () <br> |
|  void | [**setCameraPosition**](#function-setcameraposition) ([**QfCameraOrientationNormalizer::CameraPosition**](classQfCameraOrientationNormalizer.md#enum-cameraposition) position) <br> |




























## Detailed Description


On iOS and Windows, Qt Multimedia's backend produces an inverted camera preview in landscape mode and writes captured photos with incorrect orientation or bogus EXIF tags (QTBUG-118594). The preview and saved-image corrections below are limited to those platforms.


Separately, and on all platforms, this class can bake a user-chosen rotation and mirror into a captured photo before it is saved.


This class provides: _previewRotation_ for correcting the live VideoOutput orientation (iOS and Windows only) _cameraPosition_ to distinguish the front-facing camera, which needs an additional mirror correction _[**normalizeImageOrientation()**](classQfCameraOrientationNormalizer.md#function-normalizeimageorientation)_ for correcting saved JPEG files (iOS and Windows only) _[**applyEditsToImage()**](classQfCameraOrientationNormalizer.md#function-applyeditstoimage)_ for baking a user-chosen rotation and mirror into a saved JPEG (all platforms) 


    
## Public Types Documentation




### enum CameraPosition 

_Camera position, matching the integer values of QCameraDevice::Position._ 
```C++
enum QfCameraOrientationNormalizer::CameraPosition {
    UnspecifiedPosition = 0,
    BackFace = 1,
    FrontFace = 2
};
```




<hr>
## Public Properties Documentation




### property cameraPosition [1/2]

```C++
QfCameraOrientationNormalizer::CameraPosition QfCameraOrientationNormalizer::cameraPosition;
```




<hr>



### property previewRotation [1/2]

```C++
int QfCameraOrientationNormalizer::previewRotation;
```




<hr>
## Public Signals Documentation




### signal cameraPositionChanged 

```C++
void QfCameraOrientationNormalizer::cameraPositionChanged;
```




<hr>



### signal previewRotationChanged 

```C++
void QfCameraOrientationNormalizer::previewRotationChanged;
```




<hr>
## Public Functions Documentation




### function QfCameraOrientationNormalizer 

_Constructs the normalizer and starts tracking screen orientation._ 
```C++
explicit QfCameraOrientationNormalizer::QfCameraOrientationNormalizer (
    QObject * parent=nullptr
) 
```




<hr>



### function applyEditsToImage 

```C++
Q_INVOKABLE bool QfCameraOrientationNormalizer::applyEditsToImage (
    const QString & path,
    int rotation,
    bool mirror
) 
```



Bakes a user-chosen adjustment into the JPEG at _path_, letting the user manually correct a photo before saving. Unlike the orientation compensation, this applies on all platforms. _rotation_ is a clockwise angle in degrees (any multiple of 90) and _mirror_ applies a horizontal flip. The mirror is applied before the rotation to match the live preview.


Returns false without touching the file when there is nothing to apply (zero rotation and no mirror). 


        

<hr>



### function cameraPosition [2/2]

```C++
QfCameraOrientationNormalizer::CameraPosition QfCameraOrientationNormalizer::cameraPosition () const
```



Returns the active camera's position (front, back, or unspecified), used to apply the additional horizontal mirror correction that front-facing cameras require when normalizing saved photos. 


        

<hr>



### function normalizeImageOrientation 

```C++
Q_INVOKABLE bool QfCameraOrientationNormalizer::normalizeImageOrientation (
    const QString & path
) 
```



Ensures the JPEG at _path_ has pixels matching the orientation recorded by [**recordCaptureOrientation()**](classQfCameraOrientationNormalizer.md#function-recordcaptureorientation). Rotates the image if pixel dimensions contradict the capture orientation and strips any non-identity EXIF orientation tag.


Returns false without touching the file when no correction is needed. 


        

<hr>



### function previewRotation [2/2]

```C++
int QfCameraOrientationNormalizer::previewRotation () const
```



Returns the rotation in degrees that the live camera preview (VideoOutput) must be rotated by to appear upright, compensating for the incorrect preview orientation on iOS and Windows. 


        

<hr>



### function recordCaptureOrientation 

```C++
Q_INVOKABLE void QfCameraOrientationNormalizer::recordCaptureOrientation () 
```



Records the current screen orientation. Call at shutter press so that [**normalizeImageOrientation()**](classQfCameraOrientationNormalizer.md#function-normalizeimageorientation) has a ground-truth reference. 


        

<hr>



### function setCameraPosition 

```C++
void QfCameraOrientationNormalizer::setCameraPosition (
    QfCameraOrientationNormalizer::CameraPosition position
) 
```



Sets the active camera's _position_. Bound from the QML camera's device position so orientation correction can distinguish the front-facing camera. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfcameraorientationnormalizer.h`

