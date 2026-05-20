

# Class KineticHandler



[**ClassList**](annotated.md) **>** [**KineticHandler**](classKineticHandler.md)



[More...](#detailed-description)




Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property Item | [**mapCanvas**](classKineticHandler.md#property-mapcanvas)  <br> |
| property Item | [**mapCanvasWrapper**](classKineticHandler.md#property-mapcanvaswrapper)  <br> |
| property real | [**panAccumX**](classKineticHandler.md#property-panaccumx)  <br> |
| property real | [**panAccumY**](classKineticHandler.md#property-panaccumy)  <br> |
| property bool | [**panRunning**](classKineticHandler.md#property-panrunning)  <br> |
| property var | [**panSamples**](classKineticHandler.md#property-pansamples)  <br> |
| property real | [**panVx**](classKineticHandler.md#property-panvx)  <br> |
| property real | [**panVy**](classKineticHandler.md#property-panvy)  <br> |
| property [**point**](classpoint.md) | [**zoomCenter**](classKineticHandler.md#property-zoomcenter)  <br> |
| property bool | [**zoomRunning**](classKineticHandler.md#property-zoomrunning)  <br> |
| property var | [**zoomSamples**](classKineticHandler.md#property-zoomsamples)  <br> |
| property real | [**zoomVelocity**](classKineticHandler.md#property-zoomvelocity)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**activate**](#function-activate) () <br> |
|  void | [**addPanSample**](#function-addpansample) (x, y) <br> |
|  void | [**addZoomSample**](#function-addzoomsample) (scale) <br> |
|  void | [**resetPanSamples**](#function-resetpansamples) () <br> |
|  void | [**resetZoomSamples**](#function-resetzoomsamples) () <br> |
|  void | [**startPanInertia**](#function-startpaninertia) () <br> |
|  void | [**startZoomInertia**](#function-startzoominertia) (center) <br> |
|  void | [**stopAll**](#function-stopall) () <br> |




























## Detailed Description


Provides kinetic (inertia) scrolling for map pan and pinch-zoom gestures.


Qt's pointer handlers only report instantaneous position, not velocity. We sample finger positions over a short time window to compute release velocity. After the gesture ends motion continues with exponential friction decay (~60fps timer). 


    
## Public Properties Documentation




### property mapCanvas 

```C++
Item KineticHandler::mapCanvas;
```




<hr>



### property mapCanvasWrapper 

```C++
Item KineticHandler::mapCanvasWrapper;
```




<hr>



### property panAccumX 

```C++
real KineticHandler::panAccumX;
```




<hr>



### property panAccumY 

```C++
real KineticHandler::panAccumY;
```




<hr>



### property panRunning 

```C++
bool KineticHandler::panRunning;
```




<hr>



### property panSamples 

```C++
var KineticHandler::panSamples;
```




<hr>



### property panVx 

```C++
real KineticHandler::panVx;
```




<hr>



### property panVy 

```C++
real KineticHandler::panVy;
```




<hr>



### property zoomCenter 

```C++
point KineticHandler::zoomCenter;
```




<hr>



### property zoomRunning 

```C++
bool KineticHandler::zoomRunning;
```




<hr>



### property zoomSamples 

```C++
var KineticHandler::zoomSamples;
```




<hr>



### property zoomVelocity 

```C++
real KineticHandler::zoomVelocity;
```




<hr>
## Public Functions Documentation




### function activate 

```C++
void KineticHandler::activate () 
```




<hr>



### function addPanSample 

```C++
void KineticHandler::addPanSample (
    x,
    y
) 
```




<hr>



### function addZoomSample 

```C++
void KineticHandler::addZoomSample (
    scale
) 
```




<hr>



### function resetPanSamples 

```C++
void KineticHandler::resetPanSamples () 
```




<hr>



### function resetZoomSamples 

```C++
void KineticHandler::resetZoomSamples () 
```




<hr>



### function startPanInertia 

```C++
void KineticHandler::startPanInertia () 
```




<hr>



### function startZoomInertia 

```C++
void KineticHandler::startZoomInertia (
    center
) 
```




<hr>



### function stopAll 

```C++
void KineticHandler::stopAll () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/KineticHandler.qml`

