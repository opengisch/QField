

# Class QfCamera



[**ClassList**](annotated.md) **>** [**QfCamera**](classQfCamera.md)








Inherits the following classes: Popup


Inherited by the following classes: [QFieldCamera](classQFieldCamera.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**allowCaptureModeToggle**](classQfCamera.md#property-allowcapturemodetoggle)  <br> |
| property bool | [**captureLoaderActivated**](classQfCamera.md#property-captureloaderactivated)  <br> |
| property int | [**captureOffset**](classQfCamera.md#property-captureoffset)  <br> |
| property alias | [**currentFeature**](classQfCamera.md#property-currentfeature)  <br> |
| property alias | [**currentLayer**](classQfCamera.md#property-currentlayer)  <br> |
| property string | [**currentPath**](classQfCamera.md#property-currentpath)  <br> |
| property var | [**currentPosition**](classQfCamera.md#property-currentposition)  <br> |
| property var | [**currentProjectedPosition**](classQfCamera.md#property-currentprojectedposition)  <br> |
| property bool | [**isCapturing**](classQfCamera.md#property-iscapturing)  <br> |
| property bool | [**isPortraitMode**](classQfCamera.md#property-isportraitmode)  <br> |
| property bool | [**isReady**](classQfCamera.md#property-isready)  <br> |
| property int | [**panelExtraSpace**](classQfCamera.md#property-panelextraspace)  <br> |
| property int | [**recordedDuration**](classQfCamera.md#property-recordedduration)  <br> |
| property var | [**recordedPositions**](classQfCamera.md#property-recordedpositions)  <br> |
| property color | [**shieldColor**](classQfCamera.md#property-shieldcolor)  <br> |
| property string | [**state**](classQfCamera.md#property-state)  <br> |
| property bool | [**subtitleRecordingActive**](classQfCamera.md#property-subtitlerecordingactive)  <br> |
| property bool | [**userMirror**](classQfCamera.md#property-usermirror)  <br> |
| property int | [**userRotation**](classQfCamera.md#property-userrotation)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**canceled**](classQfCamera.md#signal-canceled)  <br> |
| signal void | [**finished**](classQfCamera.md#signal-finished) (string path) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**discardCapture**](#function-discardcapture) () <br> |
|  void | [**recordSubtitlePosition**](#function-recordsubtitleposition) () <br> |
|  void | [**requiredPermissionsGranted**](#function-requiredpermissionsgranted) () <br> |
|  void | [**tryActivateCaptureLoader**](#function-tryactivatecaptureloader) () <br> |
|  void | [**videoResolutionForQuality**](#function-videoresolutionforquality) (cameraResolution, quality) <br> |
|  void | [**writeSubtitleFile**](#function-writesubtitlefile) (path) <br> |




























## Public Properties Documentation




### property allowCaptureModeToggle 

```C++
bool QfCamera::allowCaptureModeToggle;
```




<hr>



### property captureLoaderActivated 

```C++
bool QfCamera::captureLoaderActivated;
```




<hr>



### property captureOffset 

```C++
int QfCamera::captureOffset;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentFeature 

```C++
alias QfCamera::currentFeature;
```




<hr>



### property currentLayer 

```C++
alias QfCamera::currentLayer;
```




<hr>



### property currentPath 

```C++
string QfCamera::currentPath;
```




<hr>



### property currentPosition 

```C++
var QfCamera::currentPosition;
```




<hr>



### property currentProjectedPosition 

```C++
var QfCamera::currentProjectedPosition;
```




<hr>



### property isCapturing 

```C++
bool QfCamera::isCapturing;
```




<hr>



### property isPortraitMode 

```C++
bool QfCamera::isPortraitMode;
```





**Remark:**

This property is read-only 





        

<hr>



### property isReady 

```C++
bool QfCamera::isReady;
```





**Remark:**

This property is read-only 





        

<hr>



### property panelExtraSpace 

```C++
int QfCamera::panelExtraSpace;
```





**Remark:**

This property is read-only 





        

<hr>



### property recordedDuration 

```C++
int QfCamera::recordedDuration;
```




<hr>



### property recordedPositions 

```C++
var QfCamera::recordedPositions;
```




<hr>



### property shieldColor 

```C++
color QfCamera::shieldColor;
```





**Remark:**

This property is read-only 





        

<hr>



### property state 

```C++
string QfCamera::state;
```




<hr>



### property subtitleRecordingActive 

```C++
bool QfCamera::subtitleRecordingActive;
```




<hr>



### property userMirror 

```C++
bool QfCamera::userMirror;
```




<hr>



### property userRotation 

```C++
int QfCamera::userRotation;
```




<hr>
## Public Signals Documentation




### signal canceled 

```C++
void QfCamera::canceled;
```




<hr>



### signal finished 

```C++
void QfCamera::finished;
```




<hr>
## Public Functions Documentation




### function discardCapture 

```C++
void QfCamera::discardCapture () 
```




<hr>



### function recordSubtitlePosition 

```C++
void QfCamera::recordSubtitlePosition () 
```



Stores the position as it stands right now against the elapsed recording time. The details expression is deliberately not evaluated here: it can be arbitrarily costly and would compete with the running recorder. The cues are built when the recording is accepted instead. 


        

<hr>



### function requiredPermissionsGranted 

```C++
void QfCamera::requiredPermissionsGranted () 
```




<hr>



### function tryActivateCaptureLoader 

```C++
void QfCamera::tryActivateCaptureLoader () 
```




<hr>



### function videoResolutionForQuality 

```C++
void QfCamera::videoResolutionForQuality (
    cameraResolution,
    quality
) 
```




<hr>



### function writeSubtitleFile 

```C++
void QfCamera::writeSubtitleFile (
    path
) 
```



Turns the positions gathered while recording into cues and writes the subtitle file next to the video at _path_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfCamera.qml`

