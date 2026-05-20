

# Class Reshape



[**ClassList**](annotated.md) **>** [**Reshape**](classReshape.md)








Inherits the following classes: [GeometryEditorBase](classGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classReshape.md#property-blocking)  <br> |
| property bool | [**screenHovering**](classReshape.md#property-screenhovering)  <br> |


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
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) ([**point**](classpoint.md), type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool Reshape::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property screenHovering 

```C++
bool Reshape::screenHovering;
```




<hr>
## Public Functions Documentation




### function cancel 

```C++
void Reshape::cancel () 
```




<hr>



### function canvasClicked 

```C++
void Reshape::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void Reshape::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void Reshape::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/geometryeditors/Reshape.qml`

