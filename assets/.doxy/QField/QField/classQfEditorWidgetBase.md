

# Class QfEditorWidgetBase



[**ClassList**](annotated.md) **>** [**QfEditorWidgetBase**](classQfEditorWidgetBase.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [QfEditorWidgetBinary](classQfEditorWidgetBinary.md),  [QfEditorWidgetCheckBox](classQfEditorWidgetCheckBox.md),  [QfEditorWidgetColor](classQfEditorWidgetColor.md),  [QfEditorWidgetDateTime](classQfEditorWidgetDateTime.md),  [QfEditorWidgetExternalResource](classQfEditorWidgetExternalResource.md),  [QfEditorWidgetRange](classQfEditorWidgetRange.md),  [QfEditorWidgetRelationReference](classQfEditorWidgetRelationReference.md),  [QfEditorWidgetTextEdit](classQfEditorWidgetTextEdit.md),  [QfEditorWidgetUuidGenerator](classQfEditorWidgetUuidGenerator.md),  [QfEditorWidgetValueMap](classQfEditorWidgetValueMap.md),  [QfEditorWidgetValueRelation](classQfEditorWidgetValueRelation.md),  [QfRelationEditorBase](classQfRelationEditorBase.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classQfEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classQfEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classQfEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classQfEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classQfEditorWidgetBase.md#property-menu)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classQfEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classQfEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classQfEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classQfEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |
































## Public Properties Documentation




### property hasMenu 

```C++
bool QfEditorWidgetBase::hasMenu;
```




<hr>



### property isEmpty 

```C++
bool QfEditorWidgetBase::isEmpty;
```





**Remark:**

This property is read-only 





        

<hr>



### property isLoaded 

```C++
bool QfEditorWidgetBase::isLoaded;
```



This property indicates whether the editor widget has been fully loaded by its Loader. Note: prior to this property being true, signals emitted by the editor widget will not be propagated. 


        

<hr>



### property isNull 

```C++
bool QfEditorWidgetBase::isNull;
```





**Remark:**

This property is read-only 





        

<hr>



### property menu 

```C++
Menu QfEditorWidgetBase::menu;
```




<hr>
## Public Signals Documentation




### signal requestBarcode 

```C++
void QfEditorWidgetBase::requestBarcode;
```



This signal is emitted when an editor widget is requesting a barcode value. The decoded barcode value will be returned as a string through calling a requestedBarcodeReceived(string) function attached to editor widget which signaled the request. The corresponding handler is `onRequestBarcode`. 


        

<hr>



### signal requestGeometry 

```C++
void QfEditorWidgetBase::requestGeometry;
```



This signal is emitted when an editor widget is in need of a digitized geometry. The geometry will be returned through calling a requestedGeometryReceived(geometry) function attached to editor widget which signaled the request. The corresponding handler is `onRequestGeometry`. 


        

<hr>



### signal requestJumpToPoint 

```C++
void QfEditorWidgetBase::requestJumpToPoint;
```



This signal is emitted when an editor widget is requesting to jump to a specific point on the map. The corresponding handler is `onRequestJumpToPoint`. 


        

<hr>



### signal valueChangeRequested 

```C++
void QfEditorWidgetBase::valueChangeRequested;
```



This signal is emmited when an editor widget has changed the value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/editorwidgets/QfEditorWidgetBase.qml`

