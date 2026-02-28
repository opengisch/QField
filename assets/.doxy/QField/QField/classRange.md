

# Class Range



[**ClassList**](annotated.md) **>** [**Range**](classRange.md)








Inherits the following classes: [EditorWidgetBase](classEditorWidgetBase.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isDouble**](classRange.md#property-isdouble)  <br> |
| property real | [**max**](classRange.md#property-max)  <br> |
| property real | [**min**](classRange.md#property-min)  <br> |
| property int | [**precision**](classRange.md#property-precision)  <br> |
| property real | [**step**](classRange.md#property-step)  <br> |
| property string | [**suffix**](classRange.md#property-suffix)  <br> |
| property string | [**widgetStyle**](classRange.md#property-widgetstyle)  <br> |


## Public Properties inherited from EditorWidgetBase

See [EditorWidgetBase](classEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classEditorWidgetBase.md#property-menu)  <br> |








## Public Signals inherited from EditorWidgetBase

See [EditorWidgetBase](classEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**decreaseValue**](#function-decreasevalue) () <br> |
|  void | [**increaseValue**](#function-increasevalue) () <br> |
|  void | [**roundValue**](#function-roundvalue) (value, precision) <br> |
























































## Public Properties Documentation




### property isDouble 

```C++
bool Range::isDouble;
```




<hr>



### property max 

```C++
real Range::max;
```




<hr>



### property min 

```C++
real Range::min;
```




<hr>



### property precision 

```C++
int Range::precision;
```




<hr>



### property step 

```C++
real Range::step;
```




<hr>



### property suffix 

```C++
string Range::suffix;
```




<hr>



### property widgetStyle 

```C++
string Range::widgetStyle;
```




<hr>
## Public Functions Documentation




### function decreaseValue 

```C++
void Range::decreaseValue () 
```




<hr>



### function increaseValue 

```C++
void Range::increaseValue () 
```




<hr>



### function roundValue 

```C++
void Range::roundValue (
    value,
    precision
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/editorwidgets/Range.qml`

