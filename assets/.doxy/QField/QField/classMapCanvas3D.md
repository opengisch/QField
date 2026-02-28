

# Class MapCanvas3D



[**ClassList**](annotated.md) **>** [**MapCanvas3D**](classMapCanvas3D.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**gnssActive**](classMapCanvas3D.md#property-gnssactive)  <br> |
| property real | [**gnssDirection**](classMapCanvas3D.md#property-gnssdirection)  <br> |
| property var | [**gnssPosition**](classMapCanvas3D.md#property-gnssposition)  <br> |
| property real | [**gnssSpeed**](classMapCanvas3D.md#property-gnssspeed)  <br> |
| property bool | [**isLoading**](classMapCanvas3D.md#property-isloading)  <br> |
| property alias | [**mapSettings**](classMapCanvas3D.md#property-mapsettings)  <br> |
| property [**TrackingModel**](classTrackingModel.md) | [**trackingModel**](classMapCanvas3D.md#property-trackingmodel)  <br> |
| property bool | [**wireframeMode**](classMapCanvas3D.md#property-wireframemode)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cameraInteractionDetected**](classMapCanvas3D.md#signal-camerainteractiondetected)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**geoTo3D**](#function-geoto3d) (geoX, geoY) <br> |
|  void | [**lookAtPoint**](#function-lookatpoint) (pos3d, [**distance**](classdistance.md)) <br> |
|  void | [**playClosingAnimation**](#function-playclosinganimation) (callback) <br> |
|  void | [**playOpeningAnimation**](#function-playopeninganimation) () <br> |
|  void | [**positionCameraForTerrain**](#function-positioncameraforterrain) () <br> |
|  void | [**zoomIn**](#function-zoomin) () <br> |
|  void | [**zoomOut**](#function-zoomout) () <br> |




























## Public Properties Documentation




### property gnssActive 

```C++
bool MapCanvas3D::gnssActive;
```




<hr>



### property gnssDirection 

```C++
real MapCanvas3D::gnssDirection;
```




<hr>



### property gnssPosition 

```C++
var MapCanvas3D::gnssPosition;
```




<hr>



### property gnssSpeed 

```C++
real MapCanvas3D::gnssSpeed;
```




<hr>



### property isLoading 

```C++
bool MapCanvas3D::isLoading;
```




<hr>



### property mapSettings 

```C++
alias MapCanvas3D::mapSettings;
```




<hr>



### property trackingModel 

```C++
TrackingModel MapCanvas3D::trackingModel;
```




<hr>



### property wireframeMode 

```C++
bool MapCanvas3D::wireframeMode;
```




<hr>
## Public Signals Documentation




### signal cameraInteractionDetected 

```C++
void MapCanvas3D::cameraInteractionDetected;
```




<hr>
## Public Functions Documentation




### function geoTo3D 

```C++
void MapCanvas3D::geoTo3D (
    geoX,
    geoY
) 
```




<hr>



### function lookAtPoint 

```C++
void MapCanvas3D::lookAtPoint (
    pos3d,
    distance
) 
```




<hr>



### function playClosingAnimation 

```C++
void MapCanvas3D::playClosingAnimation (
    callback
) 
```




<hr>



### function playOpeningAnimation 

```C++
void MapCanvas3D::playOpeningAnimation () 
```




<hr>



### function positionCameraForTerrain 

```C++
void MapCanvas3D::positionCameraForTerrain () 
```




<hr>



### function zoomIn 

```C++
void MapCanvas3D::zoomIn () 
```




<hr>



### function zoomOut 

```C++
void MapCanvas3D::zoomOut () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/3d/MapCanvas3D.qml`

