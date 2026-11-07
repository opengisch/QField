

# Class QfMapCanvas



[**ClassList**](annotated.md) **>** [**QfMapCanvas**](classQfMapCanvas.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [MapCanvas](classMapCanvas.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property real | [**bottomMargin**](classQfMapCanvas.md#property-bottommargin)  <br> |
| property bool | [**forceDeferredLayersRepaint**](classQfMapCanvas.md#property-forcedeferredlayersrepaint)  <br> |
| property bool | [**freehandDigitizing**](classQfMapCanvas.md#property-freehanddigitizing)  <br> |
| property bool | [**hovered**](classQfMapCanvas.md#property-hovered)  <br> |
| property bool | [**incrementalRendering**](classQfMapCanvas.md#property-incrementalrendering)  <br> |
| property bool | [**interactive**](classQfMapCanvas.md#property-interactive)  <br> |
| property bool | [**isMapRotationEnabled**](classQfMapCanvas.md#property-ismaprotationenabled)  <br> |
| property bool | [**isRendering**](classQfMapCanvas.md#property-isrendering)  <br> |
| property bool | [**jumping**](classQfMapCanvas.md#property-jumping)  <br> |
| property [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) | [**mapCanvasWrapper**](classQfMapCanvas.md#property-mapcanvaswrapper-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfMapCanvas.md#property-mapsettings)  <br> |
| property bool | [**mouseAsTouchScreen**](classQfMapCanvas.md#property-mouseastouchscreen)  <br>_Treat mouse and touchpad input as touch screen input._  |
| property bool | [**pinched**](classQfMapCanvas.md#property-pinched)  <br> |
| property bool | [**previewJobsEnabled**](classQfMapCanvas.md#property-previewjobsenabled)  <br> |
| property list&lt; int &gt; | [**previewJobsQuadrants**](classQfMapCanvas.md#property-previewjobsquadrants)  <br> |
| property real | [**quality**](classQfMapCanvas.md#property-quality)  <br> |
| property real | [**rightMargin**](classQfMapCanvas.md#property-rightmargin)  <br> |
| property bool | [**smooth**](classQfMapCanvas.md#property-smooth)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**aboutToWheelZoom**](classQfMapCanvas.md#signal-abouttowheelzoom)  <br>_Emitted when a zoom action is about to occur, allowing for pre-zoom adjustments._  |
| signal void | [**clicked**](classQfMapCanvas.md#signal-clicked) (var point, var type) <br>_This signal is emitted independently of double tap / click._  |
| signal void | [**confirmedClicked**](classQfMapCanvas.md#signal-confirmedclicked) (var point) <br>_This signal is only emitted if there is no double tap/click coming after a short delay._  |
| signal void | [**doubleClicked**](classQfMapCanvas.md#signal-doubleclicked) (var point, var type) <br>_This signal is emitted on a double tap._  |
| signal void | [**longPressReleased**](classQfMapCanvas.md#signal-longpressreleased) (var type) <br>_Emitted when a release happens after a long press._  |
| signal void | [**longPressed**](classQfMapCanvas.md#signal-longpressed) (var point, var type) <br> |
| signal void | [**rightClicked**](classQfMapCanvas.md#signal-rightclicked) (var point, var type) <br>_This signal is emitted when a right mouse/stylus click occurs._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**freeze**](#function-freeze) (id) <br> |
|  void | [**jumpTo**](#function-jumpto) (point, scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**jumpToPosition**](#function-jumptoposition) (positionSource, scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**refresh**](#function-refresh) (ignoreFreeze) <br> |
|  void | [**stopRendering**](#function-stoprendering) () <br> |
|  void | [**unfreeze**](#function-unfreeze) (id) <br> |
|  void | [**zoomIn**](#function-zoomin) (point) <br> |
|  void | [**zoomOut**](#function-zoomout) (point) <br> |




























## Public Properties Documentation




### property bottomMargin 

```C++
real QfMapCanvas::bottomMargin;
```




<hr>



### property forceDeferredLayersRepaint 

```C++
bool QfMapCanvas::forceDeferredLayersRepaint;
```




<hr>



### property freehandDigitizing 

```C++
bool QfMapCanvas::freehandDigitizing;
```




<hr>



### property hovered 

```C++
bool QfMapCanvas::hovered;
```




<hr>



### property incrementalRendering 

```C++
bool QfMapCanvas::incrementalRendering;
```




<hr>



### property interactive 

```C++
bool QfMapCanvas::interactive;
```




<hr>



### property isMapRotationEnabled 

```C++
bool QfMapCanvas::isMapRotationEnabled;
```




<hr>



### property isRendering 

```C++
bool QfMapCanvas::isRendering;
```




<hr>



### property jumping 

```C++
bool QfMapCanvas::jumping;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapCanvasWrapper [1/2]

```C++
QgsQuickMapCanvasMap QfMapCanvas::mapCanvasWrapper;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfMapCanvas::mapSettings;
```




<hr>



### property mouseAsTouchScreen 

_Treat mouse and touchpad input as touch screen input._ 
```C++
bool QfMapCanvas::mouseAsTouchScreen;
```




<hr>



### property pinched 

```C++
bool QfMapCanvas::pinched;
```





**Remark:**

This property is read-only 





        

<hr>



### property previewJobsEnabled 

```C++
bool QfMapCanvas::previewJobsEnabled;
```




<hr>



### property previewJobsQuadrants 

```C++
list<int> QfMapCanvas::previewJobsQuadrants;
```




<hr>



### property quality 

```C++
real QfMapCanvas::quality;
```




<hr>



### property rightMargin 

```C++
real QfMapCanvas::rightMargin;
```




<hr>



### property smooth 

```C++
bool QfMapCanvas::smooth;
```




<hr>
## Public Signals Documentation




### signal aboutToWheelZoom 

_Emitted when a zoom action is about to occur, allowing for pre-zoom adjustments._ 
```C++
void QfMapCanvas::aboutToWheelZoom;
```




<hr>



### signal clicked 

_This signal is emitted independently of double tap / click._ 
```C++
void QfMapCanvas::clicked;
```




<hr>



### signal confirmedClicked 

_This signal is only emitted if there is no double tap/click coming after a short delay._ 
```C++
void QfMapCanvas::confirmedClicked;
```




<hr>



### signal doubleClicked 

_This signal is emitted on a double tap._ 
```C++
void QfMapCanvas::doubleClicked;
```




<hr>



### signal longPressReleased 

_Emitted when a release happens after a long press._ 
```C++
void QfMapCanvas::longPressReleased;
```




<hr>



### signal longPressed 

```C++
void QfMapCanvas::longPressed;
```




<hr>



### signal rightClicked 

_This signal is emitted when a right mouse/stylus click occurs._ 
```C++
void QfMapCanvas::rightClicked;
```




<hr>
## Public Functions Documentation




### function freeze 

```C++
void QfMapCanvas::freeze (
    id
) 
```



Freezes the map canvas refreshes.


In case of repeated geometry changes (animated resizes, pinch, pan...) triggering refreshes all the time can cause severe performance impacts.


If freeze is called, an internal counter is incremented and only when the counter is 0, refreshes will happen. It is therefore important to call [**freeze()**](classQfMapCanvas.md#function-freeze) and unfreeze() exactly the same number of times. 


        

<hr>



### function jumpTo 

```C++
void QfMapCanvas::jumpTo (
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
void QfMapCanvas::jumpToPosition (
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
void QfMapCanvas::refresh (
    ignoreFreeze
) 
```




<hr>



### function stopRendering 

```C++
void QfMapCanvas::stopRendering () 
```




<hr>



### function unfreeze 

```C++
void QfMapCanvas::unfreeze (
    id
) 
```




<hr>



### function zoomIn 

```C++
void QfMapCanvas::zoomIn (
    point
) 
```




<hr>



### function zoomOut 

```C++
void QfMapCanvas::zoomOut (
    point
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qml/QfMapCanvas.qml`

