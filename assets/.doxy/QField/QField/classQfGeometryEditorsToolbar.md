

# Class QfGeometryEditorsToolbar



[**ClassList**](annotated.md) **>** [**QfGeometryEditorsToolbar**](classQfGeometryEditorsToolbar.md)



[More...](#detailed-description)




Inherits the following classes: [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)


Inherited by the following classes: [GeometryEditorsToolbar](classGeometryEditorsToolbar.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfGeometryRenderer**](classQfGeometryRenderer.md) | [**editorRenderer**](classQfGeometryEditorsToolbar.md#property-editorrenderer)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) | [**editorRubberbandModel**](classQfGeometryEditorsToolbar.md#property-editorrubberbandmodel)  <br> |
| property [**QfFeatureModel**](classQfFeatureModel.md) | [**featureModel**](classQfGeometryEditorsToolbar.md#property-featuremodel)  <br> |
| property string | [**image**](classQfGeometryEditorsToolbar.md#property-image)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfGeometryEditorsToolbar.md#property-mapsettings)  <br> |
| property bool | [**screenHovering**](classQfGeometryEditorsToolbar.md#property-screenhovering)  <br> |


## Public Properties inherited from QfVisibilityFadingRow

See [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classQfVisibilityFadingRow.md#property-statevisible)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**editorChanged**](classQfGeometryEditorsToolbar.md#signal-editorchanged)  <br> |
| signal void | [**requestJumpToPoint**](classQfGeometryEditorsToolbar.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**cancelEditors**](#function-canceleditors) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**canvasFreehandBegin**](#function-canvasfreehandbegin) () <br> |
|  void | [**canvasFreehandEnd**](#function-canvasfreehandend) () <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) (point, type) <br> |
|  void | [**init**](#function-init) () <br> |
























































## Detailed Description


This contains several geometry editing tools A tool must subclass [**QfVisibilityFadingRow**](classQfVisibilityFadingRow.md) and contains following functions:
* function init(featureModel, mapSettings, editorRubberbandModel)
* function cancel() The following signal:
* signal finished() It can optionally implement properties:
* blocking (bool) which prevents from switching tools
* vertexRubberbandVisible (bool) to show the vertex rubberband (false by default) It can optionally implement properties which will be bind:
* screenHovering determines if a pen is currently hovering the screen It can optionally implement the functions:
* canvasClicked(point)
* canvasLongPressed(point) These functions must return true if they catch the event. 




    
## Public Properties Documentation




### property editorRenderer 

```C++
QfGeometryRenderer QfGeometryEditorsToolbar::editorRenderer;
```




<hr>



### property editorRubberbandModel 

```C++
QfRubberbandModel QfGeometryEditorsToolbar::editorRubberbandModel;
```




<hr>



### property featureModel 

```C++
QfFeatureModel QfGeometryEditorsToolbar::featureModel;
```




<hr>



### property image 

```C++
string QfGeometryEditorsToolbar::image;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfGeometryEditorsToolbar::mapSettings;
```




<hr>



### property screenHovering 

```C++
bool QfGeometryEditorsToolbar::screenHovering;
```




<hr>
## Public Signals Documentation




### signal editorChanged 

```C++
void QfGeometryEditorsToolbar::editorChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfGeometryEditorsToolbar::requestJumpToPoint;
```




<hr>
## Public Functions Documentation




### function cancelEditors 

```C++
void QfGeometryEditorsToolbar::cancelEditors () 
```




<hr>



### function canvasClicked 

```C++
void QfGeometryEditorsToolbar::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasFreehandBegin 

```C++
void QfGeometryEditorsToolbar::canvasFreehandBegin () 
```




<hr>



### function canvasFreehandEnd 

```C++
void QfGeometryEditorsToolbar::canvasFreehandEnd () 
```




<hr>



### function canvasLongPressed 

```C++
void QfGeometryEditorsToolbar::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void QfGeometryEditorsToolbar::init () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfGeometryEditorsToolbar.qml`

