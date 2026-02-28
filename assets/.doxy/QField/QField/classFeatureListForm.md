

# Class FeatureListForm



[**ClassList**](annotated.md) **>** [**FeatureListForm**](classFeatureListForm.md)








Inherits the following classes: Pane


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**ProcessingAlgorithm**](classProcessingAlgorithm.md) | [**algorithm**](classFeatureListForm.md#property-algorithm)  <br> |
| property bool | [**allowDelete**](classFeatureListForm.md#property-allowdelete)  <br> |
| property bool | [**allowEdit**](classFeatureListForm.md#property-allowedit)  <br> |
| property bool | [**canvasOperationRequested**](classFeatureListForm.md#property-canvasoperationrequested)  <br> |
| property [**CodeReader**](classCodeReader.md) | [**codeReader**](classFeatureListForm.md#property-codereader)  <br> |
| property [**DigitizingToolbar**](classDigitizingToolbar.md) | [**digitizingToolbar**](classFeatureListForm.md#property-digitizingtoolbar)  <br> |
| property real | [**dragHeightAdjustment**](classFeatureListForm.md#property-dragheightadjustment)  <br> |
| property real | [**dragWidthAdjustment**](classFeatureListForm.md#property-dragwidthadjustment)  <br> |
| property FeaturelistExtentController | [**extentController**](classFeatureListForm.md#property-extentcontroller)  <br> |
| property bool | [**fullScreenView**](classFeatureListForm.md#property-fullscreenview)  <br> |
| property bool | [**isDragging**](classFeatureListForm.md#property-isdragging)  <br> |
| property bool | [**isVertical**](classFeatureListForm.md#property-isvertical)  <br> |
| property real | [**lastHeight**](classFeatureListForm.md#property-lastheight)  <br> |
| property real | [**lastWidth**](classFeatureListForm.md#property-lastwidth)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classFeatureListForm.md#property-mapsettings)  <br> |
| property [**MultiFeatureListModel**](classMultiFeatureListModel.md) | [**model**](classFeatureListForm.md#property-model)  <br> |
| property [**ConfirmationToolbar**](classConfirmationToolbar.md) | [**moveFeaturesToolbar**](classFeatureListForm.md#property-movefeaturestoolbar)  <br> |
| property bool | [**multiSelection**](classFeatureListForm.md#property-multiselection)  <br> |
| property [**ConfirmationToolbar**](classConfirmationToolbar.md) | [**rotateFeaturesToolbar**](classFeatureListForm.md#property-rotatefeaturestoolbar)  <br> |
| property [**FeatureListModelSelection**](classFeatureListModelSelection.md) | [**selection**](classFeatureListForm.md#property-selection)  <br> |
| property color | [**selectionColor**](classFeatureListForm.md#property-selectioncolor)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**editGeometry**](classFeatureListForm.md#signal-editgeometry)  <br> |
| signal void | [**requestJumpToPoint**](classFeatureListForm.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**showMessage**](classFeatureListForm.md#signal-showmessage) (string message) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**requestCancel**](#function-requestcancel) () <br> |




























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



### property dragHeightAdjustment 

```C++
real FeatureListForm::dragHeightAdjustment;
```




<hr>



### property dragWidthAdjustment 

```C++
real FeatureListForm::dragWidthAdjustment;
```




<hr>



### property extentController 

```C++
FeaturelistExtentController FeatureListForm::extentController;
```




<hr>



### property fullScreenView 

```C++
bool FeatureListForm::fullScreenView;
```




<hr>



### property isDragging 

```C++
bool FeatureListForm::isDragging;
```




<hr>



### property isVertical 

```C++
bool FeatureListForm::isVertical;
```




<hr>



### property lastHeight 

```C++
real FeatureListForm::lastHeight;
```




<hr>



### property lastWidth 

```C++
real FeatureListForm::lastWidth;
```




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

