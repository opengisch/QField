

# Class Erase



[**ClassList**](annotated.md) **>** [**Erase**](classErase.md)








Inherits the following classes: [GeometryEditorBase](classGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classErase.md#property-blocking)  <br> |
| property bool | [**screenHovering**](classErase.md#property-screenhovering)  <br> |
| property int | [**size**](classErase.md#property-size)  <br> |


## Public Properties inherited from GeometryEditorBase

See [GeometryEditorBase](classGeometryEditorBase.md)

| Type | Name |
| ---: | :--- |
| property [**GeometryRenderer**](classGeometryRenderer.md) | [**editorRenderer**](classGeometryEditorBase.md#property-editorrenderer)  <br> |
| property [**FeatureModel**](classFeatureModel.md) | [**featureModel**](classGeometryEditorBase.md#property-featuremodel)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classGeometryEditorBase.md#property-mapsettings)  <br> |


## Public Properties inherited from Theme::QfVisibilityFadingRow

See [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classTheme_1_1QfVisibilityFadingRow.md#property-statevisible)  <br> |










## Public Signals inherited from GeometryEditorBase

See [GeometryEditorBase](classGeometryEditorBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classGeometryEditorBase.md#signal-finished)  <br> |
| signal void | [**requestJumpToPoint**](classGeometryEditorBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |










## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**adjustSize**](#function-adjustsize) () <br> |
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**canvasFreehandBegin**](#function-canvasfreehandbegin) () <br> |
|  void | [**canvasFreehandEnd**](#function-canvasfreehandend) () <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) ([**point**](classpoint.md), type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool Erase::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property screenHovering 

```C++
bool Erase::screenHovering;
```




<hr>



### property size 

```C++
int Erase::size;
```




<hr>
## Public Functions Documentation




### function adjustSize 

```C++
void Erase::adjustSize () 
```




<hr>



### function cancel 

```C++
void Erase::cancel () 
```




<hr>



### function canvasClicked 

```C++
void Erase::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasFreehandBegin 

```C++
void Erase::canvasFreehandBegin () 
```




<hr>



### function canvasFreehandEnd 

```C++
void Erase::canvasFreehandEnd () 
```




<hr>



### function canvasLongPressed 

```C++
void Erase::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void Erase::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/geometryeditors/Erase.qml`

