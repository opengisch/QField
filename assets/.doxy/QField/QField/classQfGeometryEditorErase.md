

# Class QfGeometryEditorErase



[**ClassList**](annotated.md) **>** [**QfGeometryEditorErase**](classQfGeometryEditorErase.md)








Inherits the following classes: [QfGeometryEditorBase](classQfGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classQfGeometryEditorErase.md#property-blocking)  <br> |
| property bool | [**screenHovering**](classQfGeometryEditorErase.md#property-screenhovering)  <br> |
| property int | [**size**](classQfGeometryEditorErase.md#property-size)  <br> |


## Public Properties inherited from QfGeometryEditorBase

See [QfGeometryEditorBase](classQfGeometryEditorBase.md)

| Type | Name |
| ---: | :--- |
| property [**QfGeometryRenderer**](classQfGeometryRenderer.md) | [**editorRenderer**](classQfGeometryEditorBase.md#property-editorrenderer)  <br> |
| property [**QfFeatureModel**](classQfFeatureModel.md) | [**featureModel**](classQfGeometryEditorBase.md#property-featuremodel)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfGeometryEditorBase.md#property-mapsettings)  <br> |


## Public Properties inherited from QfVisibilityFadingRow

See [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classQfVisibilityFadingRow.md#property-statevisible)  <br> |










## Public Signals inherited from QfGeometryEditorBase

See [QfGeometryEditorBase](classQfGeometryEditorBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classQfGeometryEditorBase.md#signal-finished)  <br> |
| signal void | [**requestJumpToPoint**](classQfGeometryEditorBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |










## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**adjustSize**](#function-adjustsize) () <br> |
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**canvasFreehandBegin**](#function-canvasfreehandbegin) () <br> |
|  void | [**canvasFreehandEnd**](#function-canvasfreehandend) () <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) (point, type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool QfGeometryEditorErase::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property screenHovering 

```C++
bool QfGeometryEditorErase::screenHovering;
```




<hr>



### property size 

```C++
int QfGeometryEditorErase::size;
```




<hr>
## Public Functions Documentation




### function adjustSize 

```C++
void QfGeometryEditorErase::adjustSize () 
```




<hr>



### function cancel 

```C++
void QfGeometryEditorErase::cancel () 
```




<hr>



### function canvasClicked 

```C++
void QfGeometryEditorErase::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasFreehandBegin 

```C++
void QfGeometryEditorErase::canvasFreehandBegin () 
```




<hr>



### function canvasFreehandEnd 

```C++
void QfGeometryEditorErase::canvasFreehandEnd () 
```




<hr>



### function canvasLongPressed 

```C++
void QfGeometryEditorErase::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void QfGeometryEditorErase::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/geometryeditors/QfGeometryEditorErase.qml`

