

# Class QfEditorWidgetRange



[**ClassList**](annotated.md) **>** [**QfEditorWidgetRange**](classQfEditorWidgetRange.md)








Inherits the following classes: [QfEditorWidgetBase](classQfEditorWidgetBase.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isDouble**](classQfEditorWidgetRange.md#property-isdouble)  <br> |
| property real | [**max**](classQfEditorWidgetRange.md#property-max)  <br> |
| property real | [**min**](classQfEditorWidgetRange.md#property-min)  <br> |
| property int | [**precision**](classQfEditorWidgetRange.md#property-precision)  <br> |
| property real | [**step**](classQfEditorWidgetRange.md#property-step)  <br> |
| property string | [**suffix**](classQfEditorWidgetRange.md#property-suffix)  <br> |
| property string | [**widgetStyle**](classQfEditorWidgetRange.md#property-widgetstyle)  <br> |


## Public Properties inherited from QfEditorWidgetBase

See [QfEditorWidgetBase](classQfEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classQfEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classQfEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classQfEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classQfEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classQfEditorWidgetBase.md#property-menu)  <br> |








## Public Signals inherited from QfEditorWidgetBase

See [QfEditorWidgetBase](classQfEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classQfEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classQfEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classQfEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classQfEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**decreaseValue**](#function-decreasevalue) () <br> |
|  void | [**increaseValue**](#function-increasevalue) () <br> |
|  void | [**roundValue**](#function-roundvalue) (value, precision) <br> |
























































## Public Properties Documentation




### property isDouble 

```C++
bool QfEditorWidgetRange::isDouble;
```




<hr>



### property max 

```C++
real QfEditorWidgetRange::max;
```




<hr>



### property min 

```C++
real QfEditorWidgetRange::min;
```




<hr>



### property precision 

```C++
int QfEditorWidgetRange::precision;
```




<hr>



### property step 

```C++
real QfEditorWidgetRange::step;
```




<hr>



### property suffix 

```C++
string QfEditorWidgetRange::suffix;
```




<hr>



### property widgetStyle 

```C++
string QfEditorWidgetRange::widgetStyle;
```




<hr>
## Public Functions Documentation




### function decreaseValue 

```C++
void QfEditorWidgetRange::decreaseValue () 
```




<hr>



### function increaseValue 

```C++
void QfEditorWidgetRange::increaseValue () 
```




<hr>



### function roundValue 

```C++
void QfEditorWidgetRange::roundValue (
    value,
    precision
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/editorwidgets/QfEditorWidgetRange.qml`

