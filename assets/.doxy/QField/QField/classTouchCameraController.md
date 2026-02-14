

# Class TouchCameraController



[**ClassList**](annotated.md) **>** [**TouchCameraController**](classTouchCameraController.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property PerspectiveCamera | [**camera**](classTouchCameraController.md#property-camera)  <br> |
| property real | [**defaultDistance**](classTouchCameraController.md#property-defaultdistance)  <br> |
| property real | [**defaultPitch**](classTouchCameraController.md#property-defaultpitch)  <br> |
| property real | [**defaultYaw**](classTouchCameraController.md#property-defaultyaw)  <br> |
| property real | [**distance**](classTouchCameraController.md#property-distance)  <br> |
| property real | [**maxDistance**](classTouchCameraController.md#property-maxdistance)  <br> |
| property real | [**maxPitch**](classTouchCameraController.md#property-maxpitch)  <br> |
| property real | [**minDistance**](classTouchCameraController.md#property-mindistance)  <br> |
| property real | [**minPitch**](classTouchCameraController.md#property-minpitch)  <br> |
| property real | [**orbitSensitivity**](classTouchCameraController.md#property-orbitsensitivity)  <br> |
| property real | [**pitch**](classTouchCameraController.md#property-pitch)  <br> |
| property vector3d | [**target**](classTouchCameraController.md#property-target)  <br> |
| property real | [**targetX**](classTouchCameraController.md#property-targetx)  <br> |
| property real | [**targetY**](classTouchCameraController.md#property-targety)  <br> |
| property real | [**targetZ**](classTouchCameraController.md#property-targetz)  <br> |
| property real | [**yaw**](classTouchCameraController.md#property-yaw)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**singleTapped**](classTouchCameraController.md#signal-singletapped) (real x, real y) <br> |
| signal void | [**userInteractionStarted**](classTouchCameraController.md#signal-userinteractionstarted)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**applyPan**](#function-applypan) (dx, dy) <br> |
|  void | [**clampDistance**](#function-clampdistance) (value) <br> |
|  void | [**clampPitch**](#function-clamppitch) (value) <br> |
|  void | [**flyTo**](#function-flyto) (newTarget, newDistance, newYaw, newPitch) <br> |
|  void | [**lookAtPoint**](#function-lookatpoint) ([**point**](classpoint.md), zoomDistance) <br> |
|  void | [**resetView**](#function-resetview) () <br> |
|  void | [**updateCameraPosition**](#function-updatecameraposition) () <br> |
|  void | [**zoomToFit**](#function-zoomtofit) (center, radius) <br> |




























## Public Properties Documentation




### property camera 

```C++
PerspectiveCamera TouchCameraController::camera;
```




<hr>



### property defaultDistance 

```C++
real TouchCameraController::defaultDistance;
```




<hr>



### property defaultPitch 

```C++
real TouchCameraController::defaultPitch;
```




<hr>



### property defaultYaw 

```C++
real TouchCameraController::defaultYaw;
```




<hr>



### property distance 

```C++
real TouchCameraController::distance;
```




<hr>



### property maxDistance 

```C++
real TouchCameraController::maxDistance;
```




<hr>



### property maxPitch 

```C++
real TouchCameraController::maxPitch;
```




<hr>



### property minDistance 

```C++
real TouchCameraController::minDistance;
```




<hr>



### property minPitch 

```C++
real TouchCameraController::minPitch;
```




<hr>



### property orbitSensitivity 

```C++
real TouchCameraController::orbitSensitivity;
```




<hr>



### property pitch 

```C++
real TouchCameraController::pitch;
```




<hr>



### property target 

```C++
vector3d TouchCameraController::target;
```




<hr>



### property targetX 

```C++
real TouchCameraController::targetX;
```




<hr>



### property targetY 

```C++
real TouchCameraController::targetY;
```




<hr>



### property targetZ 

```C++
real TouchCameraController::targetZ;
```




<hr>



### property yaw 

```C++
real TouchCameraController::yaw;
```




<hr>
## Public Signals Documentation




### signal singleTapped 

```C++
void TouchCameraController::singleTapped;
```




<hr>



### signal userInteractionStarted 

```C++
void TouchCameraController::userInteractionStarted;
```




<hr>
## Public Functions Documentation




### function applyPan 

```C++
void TouchCameraController::applyPan (
    dx,
    dy
) 
```




<hr>



### function clampDistance 

```C++
void TouchCameraController::clampDistance (
    value
) 
```




<hr>



### function clampPitch 

```C++
void TouchCameraController::clampPitch (
    value
) 
```




<hr>



### function flyTo 

```C++
void TouchCameraController::flyTo (
    newTarget,
    newDistance,
    newYaw,
    newPitch
) 
```




<hr>



### function lookAtPoint 

```C++
void TouchCameraController::lookAtPoint (
    point,
    zoomDistance
) 
```




<hr>



### function resetView 

```C++
void TouchCameraController::resetView () 
```




<hr>



### function updateCameraPosition 

```C++
void TouchCameraController::updateCameraPosition () 
```




<hr>



### function zoomToFit 

```C++
void TouchCameraController::zoomToFit (
    center,
    radius
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/3d/TouchCameraController.qml`

