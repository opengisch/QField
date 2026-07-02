

# Class MapCanvas3D



[**ClassList**](annotated.md) **>** [**MapCanvas3D**](classMapCanvas3D.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**extentMode**](classMapCanvas3D.md#property-extentmode)  <br> |
| property bool | [**eyeDomeLightingMode**](classMapCanvas3D.md#property-eyedomelightingmode)  <br> |
| property bool | [**gnssActive**](classMapCanvas3D.md#property-gnssactive)  <br> |
| property real | [**gnssDirection**](classMapCanvas3D.md#property-gnssdirection)  <br> |
| property color | [**gnssMarkerColor**](classMapCanvas3D.md#property-gnssmarkercolor)  <br> |
| property color | [**gnssMarkerSemiOpaqueColor**](classMapCanvas3D.md#property-gnssmarkersemiopaquecolor)  <br> |
| property var | [**gnssPosition**](classMapCanvas3D.md#property-gnssposition)  <br> |
| property real | [**gnssSpeed**](classMapCanvas3D.md#property-gnssspeed)  <br> |
| property bool | [**isFirstLoad**](classMapCanvas3D.md#property-isfirstload)  <br> |
| property bool | [**isLoading**](classMapCanvas3D.md#property-isloading)  <br> |
| property alias | [**mapSettings**](classMapCanvas3D.md#property-mapsettings)  <br> |
| property Item | [**pluginContainer**](classMapCanvas3D.md#property-plugincontainer)  <br> |
| property [**FeatureListModelSelection**](classFeatureListModelSelection.md) | [**selectionModel**](classMapCanvas3D.md#property-selectionmodel)  <br> |
| property alias | [**terrainExtent**](classMapCanvas3D.md#property-terrainextent)  <br> |
| property alias | [**terrainGeometry**](classMapCanvas3D.md#property-terraingeometry)  <br> |
| property alias | [**terrainProvider**](classMapCanvas3D.md#property-terrainprovider)  <br> |
| property [**TrackingModel**](classTrackingModel.md) | [**trackingModel**](classMapCanvas3D.md#property-trackingmodel)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cameraInteractionDetected**](classMapCanvas3D.md#signal-camerainteractiondetected)  <br> |
| signal void | [**featureIdentifyRequested**](classMapCanvas3D.md#signal-featureidentifyrequested) ([**point**](classpoint.md) screenPoint) <br> |




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




### property extentMode 

```C++
alias MapCanvas3D::extentMode;
```




<hr>



### property eyeDomeLightingMode 

```C++
bool MapCanvas3D::eyeDomeLightingMode;
```




<hr>



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



### property gnssMarkerColor 

```C++
color MapCanvas3D::gnssMarkerColor;
```




<hr>



### property gnssMarkerSemiOpaqueColor 

```C++
color MapCanvas3D::gnssMarkerSemiOpaqueColor;
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



### property isFirstLoad 

```C++
bool MapCanvas3D::isFirstLoad;
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



### property pluginContainer 

```C++
Item MapCanvas3D::pluginContainer;
```




<hr>



### property selectionModel 

```C++
FeatureListModelSelection MapCanvas3D::selectionModel;
```




<hr>



### property terrainExtent 

```C++
alias MapCanvas3D::terrainExtent;
```




<hr>



### property terrainGeometry 

```C++
alias MapCanvas3D::terrainGeometry;
```




<hr>



### property terrainProvider 

```C++
alias MapCanvas3D::terrainProvider;
```




<hr>



### property trackingModel 

```C++
TrackingModel MapCanvas3D::trackingModel;
```




<hr>
## Public Signals Documentation




### signal cameraInteractionDetected 

```C++
void MapCanvas3D::cameraInteractionDetected;
```




<hr>



### signal featureIdentifyRequested 

```C++
void MapCanvas3D::featureIdentifyRequested;
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

