

# Class FeatureForm



[**ClassList**](annotated.md) **>** [**FeatureForm**](classFeatureForm.md)








Inherits the following classes: Page


























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**bottomMargin**](classFeatureForm.md#property-bottommargin)  <br> |
| property [**CodeReader**](classCodeReader.md) | [**codeReader**](classFeatureForm.md#property-codereader)  <br> |
| property alias | [**currentTab**](classFeatureForm.md#property-currenttab)  <br> |
| property [**DigitizingToolbar**](classDigitizingToolbar.md) | [**digitizingToolbar**](classFeatureForm.md#property-digitizingtoolbar)  <br> |
| property bool | [**embedded**](classFeatureForm.md#property-embedded)  <br>_if embedded form called by RelationEditor or RelationReferenceWidget_  |
| property int | [**embeddedLevel**](classFeatureForm.md#property-embeddedlevel)  <br> |
| property bool | [**featureCreated**](classFeatureForm.md#property-featurecreated)  <br> |
| property bool | [**isDraggable**](classFeatureForm.md#property-isdraggable)  <br> |
| property bool | [**isVertical**](classFeatureForm.md#property-isvertical)  <br> |
| property double | [**leftMargin**](classFeatureForm.md#property-leftmargin)  <br> |
| property [**AttributeFormModel**](classAttributeFormModel.md) | [**model**](classFeatureForm.md#property-model)  <br> |
| property double | [**rightMargin**](classFeatureForm.md#property-rightmargin)  <br> |
| property bool | [**setupOnly**](classFeatureForm.md#property-setuponly)  <br> |
| property alias | [**toolbarVisible**](classFeatureForm.md#property-toolbarvisible)  <br> |
| property double | [**topMargin**](classFeatureForm.md#property-topmargin)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**aboutToSave**](classFeatureForm.md#signal-abouttosave)  <br> |
| signal void | [**cancelled**](classFeatureForm.md#signal-cancelled)  <br> |
| signal void | [**confirmed**](classFeatureForm.md#signal-confirmed)  <br> |
| signal void | [**requestBarcode**](classFeatureForm.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classFeatureForm.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classFeatureForm.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**temporaryStored**](classFeatureForm.md#signal-temporarystored)  <br> |
| signal void | [**toolbarDragAcquired**](classFeatureForm.md#signal-toolbardragacquired)  <br> |
| signal void | [**toolbarDragReleased**](classFeatureForm.md#signal-toolbardragreleased)  <br> |
| signal void | [**toolbarDragged**](classFeatureForm.md#signal-toolbardragged) (var deltaX, var deltaY) <br> |
| signal void | [**valueChanged**](classFeatureForm.md#signal-valuechanged) (var field, var oldValue, var newValue) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**requestCancel**](#function-requestcancel) () <br> |
|  void | [**resetTabs**](#function-resettabs) () <br> |




























## Public Properties Documentation




### property bottomMargin 

```C++
double FeatureForm::bottomMargin;
```




<hr>



### property codeReader 

```C++
CodeReader FeatureForm::codeReader;
```




<hr>



### property currentTab 

```C++
alias FeatureForm::currentTab;
```




<hr>



### property digitizingToolbar 

```C++
DigitizingToolbar FeatureForm::digitizingToolbar;
```




<hr>



### property embedded 

_if embedded form called by RelationEditor or RelationReferenceWidget_ 
```C++
bool FeatureForm::embedded;
```




<hr>



### property embeddedLevel 

```C++
int FeatureForm::embeddedLevel;
```




<hr>



### property featureCreated 

```C++
bool FeatureForm::featureCreated;
```




<hr>



### property isDraggable 

```C++
bool FeatureForm::isDraggable;
```




<hr>



### property isVertical 

```C++
bool FeatureForm::isVertical;
```




<hr>



### property leftMargin 

```C++
double FeatureForm::leftMargin;
```




<hr>



### property model 

```C++
AttributeFormModel FeatureForm::model;
```




<hr>



### property rightMargin 

```C++
double FeatureForm::rightMargin;
```




<hr>



### property setupOnly 

```C++
bool FeatureForm::setupOnly;
```




<hr>



### property toolbarVisible 

```C++
alias FeatureForm::toolbarVisible;
```




<hr>



### property topMargin 

```C++
double FeatureForm::topMargin;
```




<hr>
## Public Signals Documentation




### signal aboutToSave 

```C++
void FeatureForm::aboutToSave;
```




<hr>



### signal cancelled 

```C++
void FeatureForm::cancelled;
```




<hr>



### signal confirmed 

```C++
void FeatureForm::confirmed;
```




<hr>



### signal requestBarcode 

```C++
void FeatureForm::requestBarcode;
```




<hr>



### signal requestGeometry 

```C++
void FeatureForm::requestGeometry;
```




<hr>



### signal requestJumpToPoint 

```C++
void FeatureForm::requestJumpToPoint;
```




<hr>



### signal temporaryStored 

```C++
void FeatureForm::temporaryStored;
```




<hr>



### signal toolbarDragAcquired 

```C++
void FeatureForm::toolbarDragAcquired;
```




<hr>



### signal toolbarDragReleased 

```C++
void FeatureForm::toolbarDragReleased;
```




<hr>



### signal toolbarDragged 

```C++
void FeatureForm::toolbarDragged;
```




<hr>



### signal valueChanged 

```C++
void FeatureForm::valueChanged;
```




<hr>
## Public Functions Documentation




### function requestCancel 

```C++
void FeatureForm::requestCancel () 
```




<hr>



### function resetTabs 

```C++
void FeatureForm::resetTabs () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/FeatureForm.qml`

