

# Class QfFeatureListForm



[**ClassList**](annotated.md) **>** [**QfFeatureListForm**](classQfFeatureListForm.md)








Inherits the following classes: [QfPaneDrawer](classQfPaneDrawer.md)


Inherited by the following classes: [FeatureListForm](classFeatureListForm.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfProcessingAlgorithm**](classQfProcessingAlgorithm.md) | [**algorithm**](classQfFeatureListForm.md#property-algorithm)  <br> |
| property bool | [**allowDelete**](classQfFeatureListForm.md#property-allowdelete)  <br> |
| property bool | [**allowEdit**](classQfFeatureListForm.md#property-allowedit)  <br> |
| property bool | [**canvasOperationRequested**](classQfFeatureListForm.md#property-canvasoperationrequested)  <br> |
| property [**QfCodeReader**](classQfCodeReader.md) | [**codeReader**](classQfFeatureListForm.md#property-codereader)  <br> |
| property [**QfDigitizingToolbar**](classQfDigitizingToolbar.md) | [**digitizingToolbar**](classQfFeatureListForm.md#property-digitizingtoolbar)  <br> |
| property [**QfFeatureListExtentController**](classQfFeatureListExtentController.md) | [**extentController**](classQfFeatureListForm.md#property-extentcontroller)  <br> |
| property bool | [**formOpenedFromSingleIdentify**](classQfFeatureListForm.md#property-formopenedfromsingleidentify)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfFeatureListForm.md#property-mapsettings)  <br> |
| property [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) | [**model**](classQfFeatureListForm.md#property-model)  <br> |
| property [**QfConfirmationToolbar**](classQfConfirmationToolbar.md) | [**moveFeaturesToolbar**](classQfFeatureListForm.md#property-movefeaturestoolbar)  <br> |
| property bool | [**multiSelection**](classQfFeatureListForm.md#property-multiselection)  <br> |
| property [**QfConfirmationToolbar**](classQfConfirmationToolbar.md) | [**rotateFeaturesToolbar**](classQfFeatureListForm.md#property-rotatefeaturestoolbar)  <br> |
| property [**QfFeatureListModelSelection**](classQfFeatureListModelSelection.md) | [**selection**](classQfFeatureListForm.md#property-selection)  <br> |
| property color | [**selectionColor**](classQfFeatureListForm.md#property-selectioncolor)  <br> |


## Public Properties inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| property bool | [**contentVisible**](classQfPaneDrawer.md#property-contentvisible)  <br>_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._  |
| property string | [**freezeKey**](classQfPaneDrawer.md#property-freezekey)  <br>_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._  |
| property real | [**headerHeight**](classQfPaneDrawer.md#property-headerheight)  <br>_Height of the header, used to compute the collapse threshold._  |
| property bool | [**isDragging**](classQfPaneDrawer.md#property-isdragging)  <br>_TRUE while the header is being dragged to resize the pane._  |
| property bool | [**isFullscreen**](classQfPaneDrawer.md#property-isfullscreen)  <br>_When TRUE the pane expands to fill the whole available area._  |
| property bool | [**isVertical**](classQfPaneDrawer.md#property-isvertical)  <br>_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._  |
| property real | [**minContentHeight**](classQfPaneDrawer.md#property-mincontentheight)  <br>_Content driven minimum height the pane snaps to at rest, clamped to half the available height._  |
| property bool | [**useDefaultMinHeight**](classQfPaneDrawer.md#property-usedefaultminheight)  <br>_When TRUE the resting height ignores a minContentHeight and uses the default minimum._  |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**editGeometry**](classQfFeatureListForm.md#signal-editgeometry)  <br> |
| signal void | [**requestJumpToPoint**](classQfFeatureListForm.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**showMessage**](classQfFeatureListForm.md#signal-showmessage) (string message) <br> |


## Public Signals inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classQfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |






## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**requestCancel**](#function-requestcancel) () <br> |


## Public Functions inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](classQfPaneDrawer.md#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](classQfPaneDrawer.md#function-statusindicatordragged) (deltaX, deltaY) <br> |






















































## Public Properties Documentation




### property algorithm 

```C++
QfProcessingAlgorithm QfFeatureListForm::algorithm;
```




<hr>



### property allowDelete 

```C++
bool QfFeatureListForm::allowDelete;
```




<hr>



### property allowEdit 

```C++
bool QfFeatureListForm::allowEdit;
```




<hr>



### property canvasOperationRequested 

```C++
bool QfFeatureListForm::canvasOperationRequested;
```




<hr>



### property codeReader 

```C++
QfCodeReader QfFeatureListForm::codeReader;
```




<hr>



### property digitizingToolbar 

```C++
QfDigitizingToolbar QfFeatureListForm::digitizingToolbar;
```




<hr>



### property extentController 

```C++
QfFeatureListExtentController QfFeatureListForm::extentController;
```




<hr>



### property formOpenedFromSingleIdentify 

```C++
bool QfFeatureListForm::formOpenedFromSingleIdentify;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfFeatureListForm::mapSettings;
```




<hr>



### property model 

```C++
QfMultiFeatureListModel QfFeatureListForm::model;
```




<hr>



### property moveFeaturesToolbar 

```C++
QfConfirmationToolbar QfFeatureListForm::moveFeaturesToolbar;
```




<hr>



### property multiSelection 

```C++
bool QfFeatureListForm::multiSelection;
```




<hr>



### property rotateFeaturesToolbar 

```C++
QfConfirmationToolbar QfFeatureListForm::rotateFeaturesToolbar;
```




<hr>



### property selection 

```C++
QfFeatureListModelSelection QfFeatureListForm::selection;
```




<hr>



### property selectionColor 

```C++
color QfFeatureListForm::selectionColor;
```




<hr>
## Public Signals Documentation




### signal editGeometry 

```C++
void QfFeatureListForm::editGeometry;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfFeatureListForm::requestJumpToPoint;
```




<hr>



### signal showMessage 

```C++
void QfFeatureListForm::showMessage;
```




<hr>
## Public Functions Documentation




### function requestCancel 

```C++
void QfFeatureListForm::requestCancel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfFeatureListForm.qml`

