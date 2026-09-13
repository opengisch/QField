

# Class QfMapCanvas3D



[**ClassList**](annotated.md) **>** [**QfMapCanvas3D**](classQfMapCanvas3D.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**extentMode**](classQfMapCanvas3D.md#property-extentmode)  <br> |
| property bool | [**eyeDomeLightingMode**](classQfMapCanvas3D.md#property-eyedomelightingmode)  <br> |
| property bool | [**gnssActive**](classQfMapCanvas3D.md#property-gnssactive)  <br> |
| property real | [**gnssDirection**](classQfMapCanvas3D.md#property-gnssdirection)  <br> |
| property color | [**gnssMarkerColor**](classQfMapCanvas3D.md#property-gnssmarkercolor)  <br> |
| property color | [**gnssMarkerSemiOpaqueColor**](classQfMapCanvas3D.md#property-gnssmarkersemiopaquecolor)  <br> |
| property var | [**gnssPosition**](classQfMapCanvas3D.md#property-gnssposition)  <br> |
| property real | [**gnssSpeed**](classQfMapCanvas3D.md#property-gnssspeed)  <br> |
| property bool | [**isFirstLoad**](classQfMapCanvas3D.md#property-isfirstload)  <br> |
| property bool | [**isLoading**](classQfMapCanvas3D.md#property-isloading)  <br> |
| property alias | [**mapSettings**](classQfMapCanvas3D.md#property-mapsettings)  <br> |
| property Item | [**pluginContainer**](classQfMapCanvas3D.md#property-plugincontainer)  <br> |
| property alias | [**selectionColor**](classQfMapCanvas3D.md#property-selectioncolor)  <br> |
| property [**QfFeatureListModelSelection**](classQfFeatureListModelSelection.md) | [**selectionModel**](classQfMapCanvas3D.md#property-selectionmodel)  <br> |
| property alias | [**terrainExtent**](classQfMapCanvas3D.md#property-terrainextent)  <br> |
| property alias | [**terrainGeometry**](classQfMapCanvas3D.md#property-terraingeometry)  <br> |
| property alias | [**terrainProvider**](classQfMapCanvas3D.md#property-terrainprovider)  <br> |
| property [**QfTrackingModel**](classQfTrackingModel.md) | [**trackingModel**](classQfMapCanvas3D.md#property-trackingmodel)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cameraInteractionDetected**](classQfMapCanvas3D.md#signal-camerainteractiondetected)  <br> |
| signal void | [**featureIdentifyRequested**](classQfMapCanvas3D.md#signal-featureidentifyrequested) (point screenPoint) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**geoTo3D**](#function-geoto3d) (geoX, geoY) <br> |
|  void | [**lookAtPoint**](#function-lookatpoint) (pos3d, distance) <br> |
|  void | [**playClosingAnimation**](#function-playclosinganimation) (callback) <br> |
|  void | [**playOpeningAnimation**](#function-playopeninganimation) () <br> |
|  void | [**positionCameraForTerrain**](#function-positioncameraforterrain) () <br> |
|  void | [**zoomIn**](#function-zoomin) () <br> |
|  void | [**zoomOut**](#function-zoomout) () <br> |




























## Public Properties Documentation




### property extentMode 

```C++
alias QfMapCanvas3D::extentMode;
```




<hr>



### property eyeDomeLightingMode 

```C++
bool QfMapCanvas3D::eyeDomeLightingMode;
```




<hr>



### property gnssActive 

```C++
bool QfMapCanvas3D::gnssActive;
```




<hr>



### property gnssDirection 

```C++
real QfMapCanvas3D::gnssDirection;
```




<hr>



### property gnssMarkerColor 

```C++
color QfMapCanvas3D::gnssMarkerColor;
```




<hr>



### property gnssMarkerSemiOpaqueColor 

```C++
color QfMapCanvas3D::gnssMarkerSemiOpaqueColor;
```




<hr>



### property gnssPosition 

```C++
var QfMapCanvas3D::gnssPosition;
```




<hr>



### property gnssSpeed 

```C++
real QfMapCanvas3D::gnssSpeed;
```




<hr>



### property isFirstLoad 

```C++
bool QfMapCanvas3D::isFirstLoad;
```




<hr>



### property isLoading 

```C++
bool QfMapCanvas3D::isLoading;
```




<hr>



### property mapSettings 

```C++
alias QfMapCanvas3D::mapSettings;
```




<hr>



### property pluginContainer 

```C++
Item QfMapCanvas3D::pluginContainer;
```




<hr>



### property selectionColor 

```C++
alias QfMapCanvas3D::selectionColor;
```




<hr>



### property selectionModel 

```C++
QfFeatureListModelSelection QfMapCanvas3D::selectionModel;
```




<hr>



### property terrainExtent 

```C++
alias QfMapCanvas3D::terrainExtent;
```




<hr>



### property terrainGeometry 

```C++
alias QfMapCanvas3D::terrainGeometry;
```




<hr>



### property terrainProvider 

```C++
alias QfMapCanvas3D::terrainProvider;
```




<hr>



### property trackingModel 

```C++
QfTrackingModel QfMapCanvas3D::trackingModel;
```




<hr>
## Public Signals Documentation




### signal cameraInteractionDetected 

```C++
void QfMapCanvas3D::cameraInteractionDetected;
```




<hr>



### signal featureIdentifyRequested 

```C++
void QfMapCanvas3D::featureIdentifyRequested;
```




<hr>
## Public Functions Documentation




### function geoTo3D 

```C++
void QfMapCanvas3D::geoTo3D (
    geoX,
    geoY
) 
```




<hr>



### function lookAtPoint 

```C++
void QfMapCanvas3D::lookAtPoint (
    pos3d,
    distance
) 
```




<hr>



### function playClosingAnimation 

```C++
void QfMapCanvas3D::playClosingAnimation (
    callback
) 
```




<hr>



### function playOpeningAnimation 

```C++
void QfMapCanvas3D::playOpeningAnimation () 
```




<hr>



### function positionCameraForTerrain 

```C++
void QfMapCanvas3D::positionCameraForTerrain () 
```




<hr>



### function zoomIn 

```C++
void QfMapCanvas3D::zoomIn () 
```




<hr>



### function zoomOut 

```C++
void QfMapCanvas3D::zoomOut () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qml/QfMapCanvas3D.qml`

