

# Class QfKineticHandler



[**ClassList**](annotated.md) **>** [**QfKineticHandler**](classQfKineticHandler.md)



[More...](#detailed-description)




Inherits the following classes: QtQuick::Item


Inherited by the following classes: [KineticHandler](classKineticHandler.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property Item | [**mapCanvas**](classQfKineticHandler.md#property-mapcanvas)  <br> |
| property Item | [**mapCanvasWrapper**](classQfKineticHandler.md#property-mapcanvaswrapper)  <br> |
| property real | [**panAccumX**](classQfKineticHandler.md#property-panaccumx)  <br> |
| property real | [**panAccumY**](classQfKineticHandler.md#property-panaccumy)  <br> |
| property bool | [**panRunning**](classQfKineticHandler.md#property-panrunning)  <br> |
| property var | [**panSamples**](classQfKineticHandler.md#property-pansamples)  <br> |
| property real | [**panVx**](classQfKineticHandler.md#property-panvx)  <br> |
| property real | [**panVy**](classQfKineticHandler.md#property-panvy)  <br> |
| property point | [**zoomCenter**](classQfKineticHandler.md#property-zoomcenter)  <br> |
| property bool | [**zoomRunning**](classQfKineticHandler.md#property-zoomrunning)  <br> |
| property var | [**zoomSamples**](classQfKineticHandler.md#property-zoomsamples)  <br> |
| property real | [**zoomVelocity**](classQfKineticHandler.md#property-zoomvelocity)  <br> |








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
Item QfKineticHandler::mapCanvas;
```




<hr>



### property mapCanvasWrapper 

```C++
Item QfKineticHandler::mapCanvasWrapper;
```




<hr>



### property panAccumX 

```C++
real QfKineticHandler::panAccumX;
```




<hr>



### property panAccumY 

```C++
real QfKineticHandler::panAccumY;
```




<hr>



### property panRunning 

```C++
bool QfKineticHandler::panRunning;
```




<hr>



### property panSamples 

```C++
var QfKineticHandler::panSamples;
```




<hr>



### property panVx 

```C++
real QfKineticHandler::panVx;
```




<hr>



### property panVy 

```C++
real QfKineticHandler::panVy;
```




<hr>



### property zoomCenter 

```C++
point QfKineticHandler::zoomCenter;
```




<hr>



### property zoomRunning 

```C++
bool QfKineticHandler::zoomRunning;
```




<hr>



### property zoomSamples 

```C++
var QfKineticHandler::zoomSamples;
```




<hr>



### property zoomVelocity 

```C++
real QfKineticHandler::zoomVelocity;
```




<hr>
## Public Functions Documentation




### function activate 

```C++
void QfKineticHandler::activate () 
```




<hr>



### function addPanSample 

```C++
void QfKineticHandler::addPanSample (
    x,
    y
) 
```




<hr>



### function addZoomSample 

```C++
void QfKineticHandler::addZoomSample (
    scale
) 
```




<hr>



### function resetPanSamples 

```C++
void QfKineticHandler::resetPanSamples () 
```




<hr>



### function resetZoomSamples 

```C++
void QfKineticHandler::resetZoomSamples () 
```




<hr>



### function startPanInertia 

```C++
void QfKineticHandler::startPanInertia () 
```




<hr>



### function startZoomInertia 

```C++
void QfKineticHandler::startZoomInertia (
    center
) 
```




<hr>



### function stopAll 

```C++
void QfKineticHandler::stopAll () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qml/QfKineticHandler.qml`

