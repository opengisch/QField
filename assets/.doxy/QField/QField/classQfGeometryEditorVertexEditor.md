

# Class QfGeometryEditorVertexEditor



[**ClassList**](annotated.md) **>** [**QfGeometryEditorVertexEditor**](classQfGeometryEditorVertexEditor.md)








Inherits the following classes: [QfGeometryEditorBase](classQfGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classQfGeometryEditorVertexEditor.md#property-blocking)  <br> |
| property int | [**currentVertexId**](classQfGeometryEditorVertexEditor.md#property-currentvertexid)  <br> |
| property bool | [**currentVertexModified**](classQfGeometryEditorVertexEditor.md#property-currentvertexmodified)  <br> |
| property bool | [**screenHovering**](classQfGeometryEditorVertexEditor.md#property-screenhovering)  <br> |
| property bool | [**vertexRubberbandVisible**](classQfGeometryEditorVertexEditor.md#property-vertexrubberbandvisible)  <br> |


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
|  void | [**applyChanges**](#function-applychanges) (apply) <br> |
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool QfGeometryEditorVertexEditor::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentVertexId 

```C++
int QfGeometryEditorVertexEditor::currentVertexId;
```




<hr>



### property currentVertexModified 

```C++
bool QfGeometryEditorVertexEditor::currentVertexModified;
```




<hr>



### property screenHovering 

```C++
bool QfGeometryEditorVertexEditor::screenHovering;
```




<hr>



### property vertexRubberbandVisible 

```C++
bool QfGeometryEditorVertexEditor::vertexRubberbandVisible;
```




<hr>
## Public Functions Documentation




### function applyChanges 

```C++
void QfGeometryEditorVertexEditor::applyChanges (
    apply
) 
```




<hr>



### function cancel 

```C++
void QfGeometryEditorVertexEditor::cancel () 
```




<hr>



### function canvasClicked 

```C++
void QfGeometryEditorVertexEditor::canvasClicked (
    point,
    type
) 
```




<hr>



### function init 

```C++
void QfGeometryEditorVertexEditor::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/geometryeditors/QfGeometryEditorVertexEditor.qml`

