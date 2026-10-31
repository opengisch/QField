

# Class QfGeometryEditorFillRing



[**ClassList**](annotated.md) **>** [**QfGeometryEditorFillRing**](classQfGeometryEditorFillRing.md)








Inherits the following classes: [QfGeometryEditorBase](classQfGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**addPolygonDialog**](classQfGeometryEditorFillRing.md#property-addpolygondialog-12)  <br> |
| property bool | [**blocking**](classQfGeometryEditorFillRing.md#property-blocking)  <br> |
| property alias | [**formPopupLoader**](classQfGeometryEditorFillRing.md#property-formpopuploader-12)  <br> |
| property bool | [**screenHovering**](classQfGeometryEditorFillRing.md#property-screenhovering)  <br> |


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
|  void | [**cancelRingFeature**](#function-cancelringfeature) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) (point, type) <br> |
|  void | [**commitRingFeature**](#function-commitringfeature) () <br> |
|  void | [**fillWithPolygon**](#function-fillwithpolygon) () <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property addPolygonDialog [1/2]

```C++
alias QfGeometryEditorFillRing::addPolygonDialog;
```




<hr>



### property blocking 

```C++
bool QfGeometryEditorFillRing::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property formPopupLoader [1/2]

```C++
alias QfGeometryEditorFillRing::formPopupLoader;
```




<hr>



### property screenHovering 

```C++
bool QfGeometryEditorFillRing::screenHovering;
```




<hr>
## Public Functions Documentation




### function cancel 

```C++
void QfGeometryEditorFillRing::cancel () 
```




<hr>



### function cancelRingFeature 

```C++
void QfGeometryEditorFillRing::cancelRingFeature () 
```




<hr>



### function canvasClicked 

```C++
void QfGeometryEditorFillRing::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void QfGeometryEditorFillRing::canvasLongPressed (
    point,
    type
) 
```




<hr>



### function commitRingFeature 

```C++
void QfGeometryEditorFillRing::commitRingFeature () 
```




<hr>



### function fillWithPolygon 

```C++
void QfGeometryEditorFillRing::fillWithPolygon () 
```




<hr>



### function init 

```C++
void QfGeometryEditorFillRing::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/geometryeditors/QfGeometryEditorFillRing.qml`

