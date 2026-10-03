

# Class QfGeometryEditorBase



[**ClassList**](annotated.md) **>** [**QfGeometryEditorBase**](classQfGeometryEditorBase.md)








Inherits the following classes: [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)


Inherited by the following classes: [QfGeometryEditorErase](classQfGeometryEditorErase.md),  [QfGeometryEditorFillRing](classQfGeometryEditorFillRing.md),  [QfGeometryEditorReshape](classQfGeometryEditorReshape.md),  [QfGeometryEditorSplitFeature](classQfGeometryEditorSplitFeature.md),  [QfGeometryEditorVertexEditor](classQfGeometryEditorVertexEditor.md)




































## Public Properties

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






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**finished**](classQfGeometryEditorBase.md#signal-finished)  <br> |
| signal void | [**requestJumpToPoint**](classQfGeometryEditorBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
































































## Public Properties Documentation




### property editorRenderer 

```C++
QfGeometryRenderer QfGeometryEditorBase::editorRenderer;
```




<hr>



### property featureModel 

```C++
QfFeatureModel QfGeometryEditorBase::featureModel;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfGeometryEditorBase::mapSettings;
```




<hr>
## Public Signals Documentation




### signal finished 

```C++
void QfGeometryEditorBase::finished;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfGeometryEditorBase::requestJumpToPoint;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/geometryeditors/QfGeometryEditorBase.qml`

