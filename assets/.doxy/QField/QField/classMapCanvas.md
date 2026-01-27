

# Class MapCanvas



[**ClassList**](annotated.md) **>** [**MapCanvas**](classMapCanvas.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property real | [**bottomMargin**](classMapCanvas.md#property-bottommargin)  <br> |
| property bool | [**forceDeferredLayersRepaint**](classMapCanvas.md#property-forcedeferredlayersrepaint)  <br> |
| property bool | [**freehandDigitizing**](classMapCanvas.md#property-freehanddigitizing)  <br> |
| property bool | [**hovered**](classMapCanvas.md#property-hovered)  <br> |
| property bool | [**incrementalRendering**](classMapCanvas.md#property-incrementalrendering)  <br> |
| property bool | [**interactive**](classMapCanvas.md#property-interactive)  <br> |
| property bool | [**isMapRotationEnabled**](classMapCanvas.md#property-ismaprotationenabled)  <br> |
| property bool | [**isRendering**](classMapCanvas.md#property-isrendering)  <br> |
| property bool | [**jumping**](classMapCanvas.md#property-jumping)  <br> |
| property [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) | [**mapCanvasWrapper**](classMapCanvas.md#property-mapcanvaswrapper-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classMapCanvas.md#property-mapsettings)  <br> |
| property bool | [**pinched**](classMapCanvas.md#property-pinched)  <br> |
| property bool | [**previewJobsEnabled**](classMapCanvas.md#property-previewjobsenabled)  <br> |
| property list&lt; int &gt; | [**previewJobsQuadrants**](classMapCanvas.md#property-previewjobsquadrants)  <br> |
| property real | [**quality**](classMapCanvas.md#property-quality)  <br> |
| property real | [**rightMargin**](classMapCanvas.md#property-rightmargin)  <br> |
| property bool | [**smooth**](classMapCanvas.md#property-smooth)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**aboutToWheelZoom**](classMapCanvas.md#signal-abouttowheelzoom)  <br>_Emitted when a zoom action is about to occur, allowing for pre-zoom adjustments._  |
| signal void | [**clicked**](classMapCanvas.md#signal-clicked) (var point, var type) <br>_This signal is emitted independently of double tap / click._  |
| signal void | [**confirmedClicked**](classMapCanvas.md#signal-confirmedclicked) (var point) <br>_This signal is only emitted if there is no double tap/click coming after a short delay._  |
| signal void | [**doubleClicked**](classMapCanvas.md#signal-doubleclicked) (var point, var type) <br>_This signal is emitted on a double tap._  |
| signal void | [**longPressReleased**](classMapCanvas.md#signal-longpressreleased) (var type) <br>_Emitted when a release happens after a long press._  |
| signal void | [**longPressed**](classMapCanvas.md#signal-longpressed) (var point, var type) <br> |
| signal void | [**rightClicked**](classMapCanvas.md#signal-rightclicked) (var point, var type) <br>_This signal is emitted when a right mouse/stylus click occurs._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**freeze**](#function-freeze) (id) <br> |
|  void | [**jumpTo**](#function-jumpto) ([**point**](classpoint.md), scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**jumpToPosition**](#function-jumptoposition) (positionSource, scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**refresh**](#function-refresh) (ignoreFreeze) <br> |
|  void | [**stopRendering**](#function-stoprendering) () <br> |
|  void | [**unfreeze**](#function-unfreeze) (id) <br> |
|  void | [**zoomIn**](#function-zoomin) ([**point**](classpoint.md)) <br> |
|  void | [**zoomOut**](#function-zoomout) ([**point**](classpoint.md)) <br> |




























## Public Properties Documentation




### property bottomMargin 

```C++
real MapCanvas::bottomMargin;
```




<hr>



### property forceDeferredLayersRepaint 

```C++
bool MapCanvas::forceDeferredLayersRepaint;
```




<hr>



### property freehandDigitizing 

```C++
bool MapCanvas::freehandDigitizing;
```




<hr>



### property hovered 

```C++
bool MapCanvas::hovered;
```




<hr>



### property incrementalRendering 

```C++
bool MapCanvas::incrementalRendering;
```




<hr>



### property interactive 

```C++
bool MapCanvas::interactive;
```




<hr>



### property isMapRotationEnabled 

```C++
bool MapCanvas::isMapRotationEnabled;
```




<hr>



### property isRendering 

```C++
bool MapCanvas::isRendering;
```




<hr>



### property jumping 

```C++
bool MapCanvas::jumping;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapCanvasWrapper [1/2]

```C++
QgsQuickMapCanvasMap MapCanvas::mapCanvasWrapper;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings MapCanvas::mapSettings;
```




<hr>



### property pinched 

```C++
bool MapCanvas::pinched;
```





**Remark:**

This property is read-only 





        

<hr>



### property previewJobsEnabled 

```C++
bool MapCanvas::previewJobsEnabled;
```




<hr>



### property previewJobsQuadrants 

```C++
list<int> MapCanvas::previewJobsQuadrants;
```




<hr>



### property quality 

```C++
real MapCanvas::quality;
```




<hr>



### property rightMargin 

```C++
real MapCanvas::rightMargin;
```




<hr>



### property smooth 

```C++
bool MapCanvas::smooth;
```




<hr>
## Public Signals Documentation




### signal aboutToWheelZoom 

_Emitted when a zoom action is about to occur, allowing for pre-zoom adjustments._ 
```C++
void MapCanvas::aboutToWheelZoom;
```




<hr>



### signal clicked 

_This signal is emitted independently of double tap / click._ 
```C++
void MapCanvas::clicked;
```




<hr>



### signal confirmedClicked 

_This signal is only emitted if there is no double tap/click coming after a short delay._ 
```C++
void MapCanvas::confirmedClicked;
```




<hr>



### signal doubleClicked 

_This signal is emitted on a double tap._ 
```C++
void MapCanvas::doubleClicked;
```




<hr>



### signal longPressReleased 

_Emitted when a release happens after a long press._ 
```C++
void MapCanvas::longPressReleased;
```




<hr>



### signal longPressed 

```C++
void MapCanvas::longPressed;
```




<hr>



### signal rightClicked 

_This signal is emitted when a right mouse/stylus click occurs._ 
```C++
void MapCanvas::rightClicked;
```




<hr>
## Public Functions Documentation




### function freeze 

```C++
void MapCanvas::freeze (
    id
) 
```



Freezes the map canvas refreshes.


In case of repeated geometry changes (animated resizes, pinch, pan...) triggering refreshes all the time can cause severe performance impacts.


If freeze is called, an internal counter is incremented and only when the counter is 0, refreshes will happen. It is therefore important to call [**freeze()**](classMapCanvas.md#function-freeze) and unfreeze() exactly the same number of times. 


        

<hr>



### function jumpTo 

```C++
void MapCanvas::jumpTo (
    point,
    scale=-1,
    rotation=-1,
    handleMargins=false,
    callback=null
) 
```



Smoothly animates the map to a new center point 


        

<hr>



### function jumpToPosition 

```C++
void MapCanvas::jumpToPosition (
    positionSource,
    scale=-1,
    rotation=-1,
    handleMargins=false,
    callback=null
) 
```



Jump and track a moving target 


        

<hr>



### function refresh 

```C++
void MapCanvas::refresh (
    ignoreFreeze
) 
```




<hr>



### function stopRendering 

```C++
void MapCanvas::stopRendering () 
```




<hr>



### function unfreeze 

```C++
void MapCanvas::unfreeze (
    id
) 
```




<hr>



### function zoomIn 

```C++
void MapCanvas::zoomIn (
    point
) 
```




<hr>



### function zoomOut 

```C++
void MapCanvas::zoomOut (
    point
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/MapCanvas.qml`

