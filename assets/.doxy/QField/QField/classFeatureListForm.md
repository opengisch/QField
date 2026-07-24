

# Class FeatureListForm



[**ClassList**](annotated.md) **>** [**FeatureListForm**](classFeatureListForm.md)








Inherits the following classes: [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**ProcessingAlgorithm**](classProcessingAlgorithm.md) | [**algorithm**](classFeatureListForm.md#property-algorithm)  <br> |
| property bool | [**allowDelete**](classFeatureListForm.md#property-allowdelete)  <br> |
| property bool | [**allowEdit**](classFeatureListForm.md#property-allowedit)  <br> |
| property bool | [**canvasOperationRequested**](classFeatureListForm.md#property-canvasoperationrequested)  <br> |
| property [**CodeReader**](classCodeReader.md) | [**codeReader**](classFeatureListForm.md#property-codereader)  <br> |
| property [**DigitizingToolbar**](classDigitizingToolbar.md) | [**digitizingToolbar**](classFeatureListForm.md#property-digitizingtoolbar)  <br> |
| property FeaturelistExtentController | [**extentController**](classFeatureListForm.md#property-extentcontroller)  <br> |
| property bool | [**formOpenedFromSingleIdentify**](classFeatureListForm.md#property-formopenedfromsingleidentify)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classFeatureListForm.md#property-mapsettings)  <br> |
| property [**MultiFeatureListModel**](classMultiFeatureListModel.md) | [**model**](classFeatureListForm.md#property-model)  <br> |
| property [**ConfirmationToolbar**](classConfirmationToolbar.md) | [**moveFeaturesToolbar**](classFeatureListForm.md#property-movefeaturestoolbar)  <br> |
| property bool | [**multiSelection**](classFeatureListForm.md#property-multiselection)  <br> |
| property [**ConfirmationToolbar**](classConfirmationToolbar.md) | [**rotateFeaturesToolbar**](classFeatureListForm.md#property-rotatefeaturestoolbar)  <br> |
| property [**FeatureListModelSelection**](classFeatureListModelSelection.md) | [**selection**](classFeatureListForm.md#property-selection)  <br> |
| property color | [**selectionColor**](classFeatureListForm.md#property-selectioncolor)  <br> |


## Public Properties inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| property bool | [**contentVisible**](classTheme_1_1QfPaneDrawer.md#property-contentvisible)  <br>_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._  |
| property string | [**freezeKey**](classTheme_1_1QfPaneDrawer.md#property-freezekey)  <br>_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._  |
| property real | [**headerHeight**](classTheme_1_1QfPaneDrawer.md#property-headerheight)  <br>_Height of the header, used to compute the collapse threshold._  |
| property bool | [**isDragging**](classTheme_1_1QfPaneDrawer.md#property-isdragging)  <br>_TRUE while the header is being dragged to resize the pane._  |
| property bool | [**isFullscreen**](classTheme_1_1QfPaneDrawer.md#property-isfullscreen)  <br>_When TRUE the pane expands to fill the whole available area._  |
| property bool | [**isVertical**](classTheme_1_1QfPaneDrawer.md#property-isvertical)  <br>_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._  |
| property real | [**minContentHeight**](classTheme_1_1QfPaneDrawer.md#property-mincontentheight)  <br>_Content driven minimum height the pane snaps to at rest, clamped to half the available height._  |
| property bool | [**useDefaultMinHeight**](classTheme_1_1QfPaneDrawer.md#property-usedefaultminheight)  <br>_When TRUE the resting height ignores a minContentHeight and uses the default minimum._  |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**editGeometry**](classFeatureListForm.md#signal-editgeometry)  <br> |
| signal void | [**requestJumpToPoint**](classFeatureListForm.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**showMessage**](classFeatureListForm.md#signal-showmessage) (string message) <br> |


## Public Signals inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classTheme_1_1QfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |






## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**requestCancel**](#function-requestcancel) () <br> |


## Public Functions inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](classTheme_1_1QfPaneDrawer.md#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](classTheme_1_1QfPaneDrawer.md#function-statusindicatordragged) (deltaX, deltaY) <br> |






















































## Public Properties Documentation




### property algorithm 

```C++
ProcessingAlgorithm FeatureListForm::algorithm;
```




<hr>



### property allowDelete 

```C++
bool FeatureListForm::allowDelete;
```




<hr>



### property allowEdit 

```C++
bool FeatureListForm::allowEdit;
```




<hr>



### property canvasOperationRequested 

```C++
bool FeatureListForm::canvasOperationRequested;
```




<hr>



### property codeReader 

```C++
CodeReader FeatureListForm::codeReader;
```




<hr>



### property digitizingToolbar 

```C++
DigitizingToolbar FeatureListForm::digitizingToolbar;
```




<hr>



### property extentController 

```C++
FeaturelistExtentController FeatureListForm::extentController;
```




<hr>



### property formOpenedFromSingleIdentify 

```C++
bool FeatureListForm::formOpenedFromSingleIdentify;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapSettings 

```C++
QgsQuickMapSettings FeatureListForm::mapSettings;
```




<hr>



### property model 

```C++
MultiFeatureListModel FeatureListForm::model;
```




<hr>



### property moveFeaturesToolbar 

```C++
ConfirmationToolbar FeatureListForm::moveFeaturesToolbar;
```




<hr>



### property multiSelection 

```C++
bool FeatureListForm::multiSelection;
```




<hr>



### property rotateFeaturesToolbar 

```C++
ConfirmationToolbar FeatureListForm::rotateFeaturesToolbar;
```




<hr>



### property selection 

```C++
FeatureListModelSelection FeatureListForm::selection;
```




<hr>



### property selectionColor 

```C++
color FeatureListForm::selectionColor;
```




<hr>
## Public Signals Documentation




### signal editGeometry 

```C++
void FeatureListForm::editGeometry;
```




<hr>



### signal requestJumpToPoint 

```C++
void FeatureListForm::requestJumpToPoint;
```




<hr>



### signal showMessage 

```C++
void FeatureListForm::showMessage;
```




<hr>
## Public Functions Documentation




### function requestCancel 

```C++
void FeatureListForm::requestCancel () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/FeatureListForm.qml`

