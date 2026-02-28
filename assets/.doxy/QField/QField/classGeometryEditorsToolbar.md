

# Class GeometryEditorsToolbar



[**ClassList**](annotated.md) **>** [**GeometryEditorsToolbar**](classGeometryEditorsToolbar.md)



[More...](#detailed-description)




Inherits the following classes: [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**GeometryRenderer**](classGeometryRenderer.md) | [**editorRenderer**](classGeometryEditorsToolbar.md#property-editorrenderer)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) | [**editorRubberbandModel**](classGeometryEditorsToolbar.md#property-editorrubberbandmodel)  <br> |
| property [**FeatureModel**](classFeatureModel.md) | [**featureModel**](classGeometryEditorsToolbar.md#property-featuremodel)  <br> |
| property string | [**image**](classGeometryEditorsToolbar.md#property-image)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classGeometryEditorsToolbar.md#property-mapsettings)  <br> |
| property bool | [**screenHovering**](classGeometryEditorsToolbar.md#property-screenhovering)  <br> |


## Public Properties inherited from Theme::QfVisibilityFadingRow

See [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classTheme_1_1QfVisibilityFadingRow.md#property-statevisible)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**editorChanged**](classGeometryEditorsToolbar.md#signal-editorchanged)  <br> |
| signal void | [**requestJumpToPoint**](classGeometryEditorsToolbar.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**cancelEditors**](#function-canceleditors) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**canvasFreehandBegin**](#function-canvasfreehandbegin) () <br> |
|  void | [**canvasFreehandEnd**](#function-canvasfreehandend) () <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) ([**point**](classpoint.md), type) <br> |
|  void | [**init**](#function-init) () <br> |
























































## Detailed Description


This contains several geometry editing tools A tool must subclass QfVisibilityFadingRow and contains following functions:
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
GeometryRenderer GeometryEditorsToolbar::editorRenderer;
```




<hr>



### property editorRubberbandModel 

```C++
RubberbandModel GeometryEditorsToolbar::editorRubberbandModel;
```




<hr>



### property featureModel 

```C++
FeatureModel GeometryEditorsToolbar::featureModel;
```




<hr>



### property image 

```C++
string GeometryEditorsToolbar::image;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings GeometryEditorsToolbar::mapSettings;
```




<hr>



### property screenHovering 

```C++
bool GeometryEditorsToolbar::screenHovering;
```




<hr>
## Public Signals Documentation




### signal editorChanged 

```C++
void GeometryEditorsToolbar::editorChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void GeometryEditorsToolbar::requestJumpToPoint;
```




<hr>
## Public Functions Documentation




### function cancelEditors 

```C++
void GeometryEditorsToolbar::cancelEditors () 
```




<hr>



### function canvasClicked 

```C++
void GeometryEditorsToolbar::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasFreehandBegin 

```C++
void GeometryEditorsToolbar::canvasFreehandBegin () 
```




<hr>



### function canvasFreehandEnd 

```C++
void GeometryEditorsToolbar::canvasFreehandEnd () 
```




<hr>



### function canvasLongPressed 

```C++
void GeometryEditorsToolbar::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void GeometryEditorsToolbar::init () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/GeometryEditorsToolbar.qml`

