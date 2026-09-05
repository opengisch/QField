

# Class QfFeatureForm



[**ClassList**](annotated.md) **>** [**QfFeatureForm**](classQfFeatureForm.md)








Inherits the following classes: Page


Inherited by the following classes: [FeatureForm](classFeatureForm.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**bottomMargin**](classQfFeatureForm.md#property-bottommargin)  <br> |
| property [**QfCodeReader**](classQfCodeReader.md) | [**codeReader**](classQfFeatureForm.md#property-codereader)  <br> |
| property alias | [**currentTab**](classQfFeatureForm.md#property-currenttab)  <br> |
| property [**QfDigitizingToolbar**](classQfDigitizingToolbar.md) | [**digitizingToolbar**](classQfFeatureForm.md#property-digitizingtoolbar)  <br> |
| property bool | [**embedded**](classQfFeatureForm.md#property-embedded)  <br>_if embedded form called by RelationEditor or RelationReferenceWidget_  |
| property int | [**embeddedLevel**](classQfFeatureForm.md#property-embeddedlevel)  <br> |
| property bool | [**featureCreated**](classQfFeatureForm.md#property-featurecreated)  <br> |
| property bool | [**isDraggable**](classQfFeatureForm.md#property-isdraggable)  <br> |
| property bool | [**isVertical**](classQfFeatureForm.md#property-isvertical)  <br> |
| property double | [**leftMargin**](classQfFeatureForm.md#property-leftmargin)  <br> |
| property [**QfAttributeFormModel**](classQfAttributeFormModel.md) | [**model**](classQfFeatureForm.md#property-model)  <br> |
| property double | [**rightMargin**](classQfFeatureForm.md#property-rightmargin)  <br> |
| property bool | [**setupOnly**](classQfFeatureForm.md#property-setuponly)  <br> |
| property alias | [**toolbarVisible**](classQfFeatureForm.md#property-toolbarvisible)  <br> |
| property double | [**topMargin**](classQfFeatureForm.md#property-topmargin)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**aboutToSave**](classQfFeatureForm.md#signal-abouttosave)  <br> |
| signal void | [**cancelled**](classQfFeatureForm.md#signal-cancelled)  <br> |
| signal void | [**confirmed**](classQfFeatureForm.md#signal-confirmed)  <br> |
| signal void | [**created**](classQfFeatureForm.md#signal-created)  <br> |
| signal void | [**requestBarcode**](classQfFeatureForm.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classQfFeatureForm.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classQfFeatureForm.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**saved**](classQfFeatureForm.md#signal-saved)  <br> |
| signal void | [**temporaryStored**](classQfFeatureForm.md#signal-temporarystored)  <br> |
| signal void | [**toolbarDragAcquired**](classQfFeatureForm.md#signal-toolbardragacquired)  <br> |
| signal void | [**toolbarDragReleased**](classQfFeatureForm.md#signal-toolbardragreleased)  <br> |
| signal void | [**toolbarDragged**](classQfFeatureForm.md#signal-toolbardragged) (var deltaX, var deltaY) <br> |
| signal void | [**valueChanged**](classQfFeatureForm.md#signal-valuechanged) (var field, var oldValue, var newValue) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**requestCancel**](#function-requestcancel) () <br> |
|  void | [**resetTabs**](#function-resettabs) () <br> |




























## Public Properties Documentation




### property bottomMargin 

```C++
double QfFeatureForm::bottomMargin;
```




<hr>



### property codeReader 

```C++
QfCodeReader QfFeatureForm::codeReader;
```




<hr>



### property currentTab 

```C++
alias QfFeatureForm::currentTab;
```




<hr>



### property digitizingToolbar 

```C++
QfDigitizingToolbar QfFeatureForm::digitizingToolbar;
```




<hr>



### property embedded 

_if embedded form called by RelationEditor or RelationReferenceWidget_ 
```C++
bool QfFeatureForm::embedded;
```




<hr>



### property embeddedLevel 

```C++
int QfFeatureForm::embeddedLevel;
```




<hr>



### property featureCreated 

```C++
bool QfFeatureForm::featureCreated;
```




<hr>



### property isDraggable 

```C++
bool QfFeatureForm::isDraggable;
```




<hr>



### property isVertical 

```C++
bool QfFeatureForm::isVertical;
```




<hr>



### property leftMargin 

```C++
double QfFeatureForm::leftMargin;
```




<hr>



### property model 

```C++
QfAttributeFormModel QfFeatureForm::model;
```




<hr>



### property rightMargin 

```C++
double QfFeatureForm::rightMargin;
```




<hr>



### property setupOnly 

```C++
bool QfFeatureForm::setupOnly;
```




<hr>



### property toolbarVisible 

```C++
alias QfFeatureForm::toolbarVisible;
```




<hr>



### property topMargin 

```C++
double QfFeatureForm::topMargin;
```




<hr>
## Public Signals Documentation




### signal aboutToSave 

```C++
void QfFeatureForm::aboutToSave;
```




<hr>



### signal cancelled 

```C++
void QfFeatureForm::cancelled;
```




<hr>



### signal confirmed 

```C++
void QfFeatureForm::confirmed;
```




<hr>



### signal created 

```C++
void QfFeatureForm::created;
```




<hr>



### signal requestBarcode 

```C++
void QfFeatureForm::requestBarcode;
```




<hr>



### signal requestGeometry 

```C++
void QfFeatureForm::requestGeometry;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfFeatureForm::requestJumpToPoint;
```




<hr>



### signal saved 

```C++
void QfFeatureForm::saved;
```




<hr>



### signal temporaryStored 

```C++
void QfFeatureForm::temporaryStored;
```




<hr>



### signal toolbarDragAcquired 

```C++
void QfFeatureForm::toolbarDragAcquired;
```




<hr>



### signal toolbarDragReleased 

```C++
void QfFeatureForm::toolbarDragReleased;
```




<hr>



### signal toolbarDragged 

```C++
void QfFeatureForm::toolbarDragged;
```




<hr>



### signal valueChanged 

```C++
void QfFeatureForm::valueChanged;
```




<hr>
## Public Functions Documentation




### function requestCancel 

```C++
void QfFeatureForm::requestCancel () 
```




<hr>



### function resetTabs 

```C++
void QfFeatureForm::resetTabs () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfFeatureForm.qml`

