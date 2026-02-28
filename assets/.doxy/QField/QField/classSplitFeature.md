

# Class SplitFeature



[**ClassList**](annotated.md) **>** [**SplitFeature**](classSplitFeature.md)








Inherits the following classes: [GeometryEditorBase](classGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classSplitFeature.md#property-blocking)  <br> |
| property bool | [**screenHovering**](classSplitFeature.md#property-screenhovering)  <br> |


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
bool SplitFeature::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property screenHovering 

```C++
bool SplitFeature::screenHovering;
```




<hr>
## Public Functions Documentation




### function cancel 

```C++
void SplitFeature::cancel () 
```




<hr>



### function canvasClicked 

```C++
void SplitFeature::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void SplitFeature::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function init 

```C++
void SplitFeature::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/geometryeditors/SplitFeature.qml`

