

# Class CameraOrientationNormalizer



[**ClassList**](annotated.md) **>** [**CameraOrientationNormalizer**](classCameraOrientationNormalizer.md)



_Compensates for incorrect camera orientation on iOS and Windows._ [More...](#detailed-description)

* `#include <cameraorientationnormalizer.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**previewRotation**](classCameraOrientationNormalizer.md#property-previewrotation-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**previewRotationChanged**](classCameraOrientationNormalizer.md#signal-previewrotationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CameraOrientationNormalizer**](#function-cameraorientationnormalizer) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE bool | [**normalizeImageOrientation**](#function-normalizeimageorientation) (const QString & path) <br> |
|  int | [**previewRotation**](#function-previewrotation-22) () const<br> |
|  Q\_INVOKABLE void | [**recordCaptureOrientation**](#function-recordcaptureorientation) () <br> |




























## Detailed Description


On iOS and Windows, Qt Multimedia's backend produces an inverted camera preview in landscape mode and writes captured photos with incorrect orientation or bogus EXIF tags (QTBUG-118594).


This class provides: _previewRotation_ for correcting the live VideoOutput orientation _[**normalizeImageOrientation()**](classCameraOrientationNormalizer.md#function-normalizeimageorientation)_ for correcting saved JPEG files 


    
## Public Properties Documentation




### property previewRotation [1/2]

```C++
int CameraOrientationNormalizer::previewRotation;
```




<hr>
## Public Signals Documentation




### signal previewRotationChanged 

```C++
void CameraOrientationNormalizer::previewRotationChanged;
```




<hr>
## Public Functions Documentation




### function CameraOrientationNormalizer 

```C++
explicit CameraOrientationNormalizer::CameraOrientationNormalizer (
    QObject * parent=nullptr
) 
```




<hr>



### function normalizeImageOrientation 

```C++
Q_INVOKABLE bool CameraOrientationNormalizer::normalizeImageOrientation (
    const QString & path
) 
```



Ensures the JPEG at _path_ has pixels matching the orientation recorded by [**recordCaptureOrientation()**](classCameraOrientationNormalizer.md#function-recordcaptureorientation). Rotates the image if pixel dimensions contradict the capture orientation and strips any non-identity EXIF orientation tag.


Returns false without touching the file when no correction is needed. 


        

<hr>



### function previewRotation [2/2]

```C++
int CameraOrientationNormalizer::previewRotation () const
```




<hr>



### function recordCaptureOrientation 

```C++
Q_INVOKABLE void CameraOrientationNormalizer::recordCaptureOrientation () 
```



Records the current screen orientation. Call at shutter press so that [**normalizeImageOrientation()**](classCameraOrientationNormalizer.md#function-normalizeimageorientation) has a ground-truth reference. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cameraorientationnormalizer.h`

