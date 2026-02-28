

# Class VertexEditor



[**ClassList**](annotated.md) **>** [**VertexEditor**](classVertexEditor.md)








Inherits the following classes: [GeometryEditorBase](classGeometryEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**blocking**](classVertexEditor.md#property-blocking)  <br> |
| property int | [**currentVertexId**](classVertexEditor.md#property-currentvertexid)  <br> |
| property bool | [**currentVertexModified**](classVertexEditor.md#property-currentvertexmodified)  <br> |
| property bool | [**screenHovering**](classVertexEditor.md#property-screenhovering)  <br> |
| property bool | [**vertexRubberbandVisible**](classVertexEditor.md#property-vertexrubberbandvisible)  <br> |


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
|  void | [**applyChanges**](#function-applychanges) (apply) <br> |
|  void | [**cancel**](#function-cancel) () <br> |
|  void | [**canvasClicked**](#function-canvasclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**init**](#function-init) (featureModel, mapSettings, editorRubberbandModel, editorRenderer) <br> |




















































































## Public Properties Documentation




### property blocking 

```C++
bool VertexEditor::blocking;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentVertexId 

```C++
int VertexEditor::currentVertexId;
```




<hr>



### property currentVertexModified 

```C++
bool VertexEditor::currentVertexModified;
```




<hr>



### property screenHovering 

```C++
bool VertexEditor::screenHovering;
```




<hr>



### property vertexRubberbandVisible 

```C++
bool VertexEditor::vertexRubberbandVisible;
```




<hr>
## Public Functions Documentation




### function applyChanges 

```C++
void VertexEditor::applyChanges (
    apply
) 
```




<hr>



### function cancel 

```C++
void VertexEditor::cancel () 
```




<hr>



### function canvasClicked 

```C++
void VertexEditor::canvasClicked (
    point,
    type
) 
```




<hr>



### function init 

```C++
void VertexEditor::init (
    featureModel,
    mapSettings,
    editorRubberbandModel,
    editorRenderer
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/geometryeditors/VertexEditor.qml`

