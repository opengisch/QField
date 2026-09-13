

# Class QfGeometryEditorReshape



[**ClassList**](annotated.md) **>** [**QfGeometryEditorReshape**](classQfGeometryEditorReshape.md)








Inherits the following classes: [QfGeometryEditorBase](classQfGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classQfGeometryEditorReshape.md#property-blocking)  <br> |
| property bool | [**screenHovering**](classQfGeometryEditorReshape.md#property-screenhovering)  <br> |


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
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) (point, type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool QfGeometryEditorReshape::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property screenHovering 

```C++
bool QfGeometryEditorReshape::screenHovering;
```




<hr>
## Public Functions Documentation




### function cancel 

```C++
void QfGeometryEditorReshape::cancel () 
```




<hr>



### function canvasClicked 

```C++
void QfGeometryEditorReshape::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void QfGeometryEditorReshape::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void QfGeometryEditorReshape::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/geometryeditors/QfGeometryEditorReshape.qml`

