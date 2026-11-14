

# Class QfTouchCameraController



[**ClassList**](annotated.md) **>** [**QfTouchCameraController**](classQfTouchCameraController.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property PerspectiveCamera | [**camera**](classQfTouchCameraController.md#property-camera)  <br> |
| property real | [**defaultDistance**](classQfTouchCameraController.md#property-defaultdistance)  <br> |
| property real | [**defaultPitch**](classQfTouchCameraController.md#property-defaultpitch)  <br> |
| property real | [**defaultYaw**](classQfTouchCameraController.md#property-defaultyaw)  <br> |
| property real | [**distance**](classQfTouchCameraController.md#property-distance)  <br> |
| property bool | [**extentMode**](classQfTouchCameraController.md#property-extentmode)  <br> |
| property real | [**maxDistance**](classQfTouchCameraController.md#property-maxdistance)  <br> |
| property real | [**maxPitch**](classQfTouchCameraController.md#property-maxpitch)  <br> |
| property real | [**minDistance**](classQfTouchCameraController.md#property-mindistance)  <br> |
| property real | [**minPitch**](classQfTouchCameraController.md#property-minpitch)  <br> |
| property real | [**orbitSensitivity**](classQfTouchCameraController.md#property-orbitsensitivity)  <br> |
| property real | [**pitch**](classQfTouchCameraController.md#property-pitch)  <br> |
| property vector3d | [**target**](classQfTouchCameraController.md#property-target)  <br> |
| property real | [**targetX**](classQfTouchCameraController.md#property-targetx)  <br> |
| property real | [**targetY**](classQfTouchCameraController.md#property-targety)  <br> |
| property real | [**targetZ**](classQfTouchCameraController.md#property-targetz)  <br> |
| property real | [**yaw**](classQfTouchCameraController.md#property-yaw)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**extentPan**](classQfTouchCameraController.md#signal-extentpan) (real sceneX, real sceneZ) <br> |
| signal void | [**extentPanFinished**](classQfTouchCameraController.md#signal-extentpanfinished)  <br> |
| signal void | [**extentZoom**](classQfTouchCameraController.md#signal-extentzoom) (real factor) <br> |
| signal void | [**extentZoomFinished**](classQfTouchCameraController.md#signal-extentzoomfinished)  <br> |
| signal void | [**singleTapped**](classQfTouchCameraController.md#signal-singletapped) (real x, real y) <br> |
| signal void | [**userInteractionStarted**](classQfTouchCameraController.md#signal-userinteractionstarted)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**applyPan**](#function-applypan) (dx, dy) <br> |
|  void | [**clampDistance**](#function-clampdistance) (value) <br> |
|  void | [**clampPitch**](#function-clamppitch) (value) <br> |
|  void | [**flyTo**](#function-flyto) (newTarget, newDistance, newYaw, newPitch) <br> |
|  void | [**lookAtPoint**](#function-lookatpoint) (point, zoomDistance) <br> |
|  void | [**resetView**](#function-resetview) () <br> |
|  void | [**updateCameraPosition**](#function-updatecameraposition) () <br> |
|  void | [**zoomToFit**](#function-zoomtofit) (center, radius) <br> |




























## Public Properties Documentation




### property camera 

```C++
PerspectiveCamera QfTouchCameraController::camera;
```




<hr>



### property defaultDistance 

```C++
real QfTouchCameraController::defaultDistance;
```




<hr>



### property defaultPitch 

```C++
real QfTouchCameraController::defaultPitch;
```




<hr>



### property defaultYaw 

```C++
real QfTouchCameraController::defaultYaw;
```




<hr>



### property distance 

```C++
real QfTouchCameraController::distance;
```




<hr>



### property extentMode 

```C++
bool QfTouchCameraController::extentMode;
```




<hr>



### property maxDistance 

```C++
real QfTouchCameraController::maxDistance;
```




<hr>



### property maxPitch 

```C++
real QfTouchCameraController::maxPitch;
```




<hr>



### property minDistance 

```C++
real QfTouchCameraController::minDistance;
```




<hr>



### property minPitch 

```C++
real QfTouchCameraController::minPitch;
```




<hr>



### property orbitSensitivity 

```C++
real QfTouchCameraController::orbitSensitivity;
```




<hr>



### property pitch 

```C++
real QfTouchCameraController::pitch;
```




<hr>



### property target 

```C++
vector3d QfTouchCameraController::target;
```




<hr>



### property targetX 

```C++
real QfTouchCameraController::targetX;
```




<hr>



### property targetY 

```C++
real QfTouchCameraController::targetY;
```




<hr>



### property targetZ 

```C++
real QfTouchCameraController::targetZ;
```




<hr>



### property yaw 

```C++
real QfTouchCameraController::yaw;
```




<hr>
## Public Signals Documentation




### signal extentPan 

```C++
void QfTouchCameraController::extentPan;
```




<hr>



### signal extentPanFinished 

```C++
void QfTouchCameraController::extentPanFinished;
```




<hr>



### signal extentZoom 

```C++
void QfTouchCameraController::extentZoom;
```




<hr>



### signal extentZoomFinished 

```C++
void QfTouchCameraController::extentZoomFinished;
```




<hr>



### signal singleTapped 

```C++
void QfTouchCameraController::singleTapped;
```




<hr>



### signal userInteractionStarted 

```C++
void QfTouchCameraController::userInteractionStarted;
```




<hr>
## Public Functions Documentation




### function applyPan 

```C++
void QfTouchCameraController::applyPan (
    dx,
    dy
) 
```




<hr>



### function clampDistance 

```C++
void QfTouchCameraController::clampDistance (
    value
) 
```




<hr>



### function clampPitch 

```C++
void QfTouchCameraController::clampPitch (
    value
) 
```




<hr>



### function flyTo 

```C++
void QfTouchCameraController::flyTo (
    newTarget,
    newDistance,
    newYaw,
    newPitch
) 
```




<hr>



### function lookAtPoint 

```C++
void QfTouchCameraController::lookAtPoint (
    point,
    zoomDistance
) 
```




<hr>



### function resetView 

```C++
void QfTouchCameraController::resetView () 
```




<hr>



### function updateCameraPosition 

```C++
void QfTouchCameraController::updateCameraPosition () 
```




<hr>



### function zoomToFit 

```C++
void QfTouchCameraController::zoomToFit (
    center,
    radius
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qml/QfTouchCameraController.qml`

