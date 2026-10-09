

# Class QfEditorWidgetDateTime



[**ClassList**](annotated.md) **>** [**QfEditorWidgetDateTime**](classQfEditorWidgetDateTime.md)








Inherits the following classes: [QfEditorWidgetBase](classQfEditorWidgetBase.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**currentValue**](classQfEditorWidgetDateTime.md#property-currentvalue)  <br> |
| property bool | [**fieldIsDate**](classQfEditorWidgetDateTime.md#property-fieldisdate)  <br> |
| property bool | [**fieldIsDateTime**](classQfEditorWidgetDateTime.md#property-fieldisdatetime)  <br> |
| property bool | [**fieldIsString**](classQfEditorWidgetDateTime.md#property-fieldisstring)  <br> |
| property bool | [**fieldIsTime**](classQfEditorWidgetDateTime.md#property-fieldistime)  <br> |
| property bool | [**isDateTimeType**](classQfEditorWidgetDateTime.md#property-isdatetimetype)  <br> |


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
|  void | [**convertDateToFieldFormattedString**](#function-convertdatetofieldformattedstring) (date) <br> |
|  void | [**convertValueToDate**](#function-convertvaluetodate) (value) <br> |
|  void | [**formatDateTime**](#function-formatdatetime) (value) <br> |
























































## Public Properties Documentation




### property currentValue 

```C++
var QfEditorWidgetDateTime::currentValue;
```




<hr>



### property fieldIsDate 

```C++
bool QfEditorWidgetDateTime::fieldIsDate;
```




<hr>



### property fieldIsDateTime 

```C++
bool QfEditorWidgetDateTime::fieldIsDateTime;
```




<hr>



### property fieldIsString 

```C++
bool QfEditorWidgetDateTime::fieldIsString;
```




<hr>



### property fieldIsTime 

```C++
bool QfEditorWidgetDateTime::fieldIsTime;
```




<hr>



### property isDateTimeType 

```C++
bool QfEditorWidgetDateTime::isDateTimeType;
```




<hr>
## Public Functions Documentation




### function convertDateToFieldFormattedString 

```C++
void QfEditorWidgetDateTime::convertDateToFieldFormattedString (
    date
) 
```




<hr>



### function convertValueToDate 

```C++
void QfEditorWidgetDateTime::convertValueToDate (
    value
) 
```




<hr>



### function formatDateTime 

```C++
void QfEditorWidgetDateTime::formatDateTime (
    value
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/editorwidgets/QfEditorWidgetDateTime.qml`

