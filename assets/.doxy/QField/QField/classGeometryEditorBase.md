

# Class GeometryEditorBase



[**ClassList**](annotated.md) **>** [**GeometryEditorBase**](classGeometryEditorBase.md)








Inherits the following classes: [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)


Inherited by the following classes: [Erase](classErase.md),  [FillRing](classFillRing.md),  [Reshape](classReshape.md),  [SplitFeature](classSplitFeature.md),  [VertexEditor](classVertexEditor.md)




































## Public Properties

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






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classGeometryEditorBase.md#signal-finished)  <br> |
| signal void | [**requestJumpToPoint**](classGeometryEditorBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
































































## Public Properties Documentation




### property editorRenderer 

```C++
GeometryRenderer GeometryEditorBase::editorRenderer;
```




<hr>



### property featureModel 

```C++
FeatureModel GeometryEditorBase::featureModel;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings GeometryEditorBase::mapSettings;
```




<hr>
## Public Signals Documentation




### signal finished 

```C++
void GeometryEditorBase::finished;
```




<hr>



### signal requestJumpToPoint 

```C++
void GeometryEditorBase::requestJumpToPoint;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/geometryeditors/GeometryEditorBase.qml`

