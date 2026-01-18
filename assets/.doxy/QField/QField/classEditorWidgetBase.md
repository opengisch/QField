

# Class EditorWidgetBase



[**ClassList**](annotated.md) **>** [**EditorWidgetBase**](classEditorWidgetBase.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [Binary](classBinary.md),  [CheckBox](classCheckBox.md),  [Color](classColor.md),  [DateTime](classDateTime.md),  [ExternalResource](classExternalResource.md),  [Range](classRange.md),  [RelationEditorBase](classRelationEditorBase.md),  [RelationReference](classRelationReference.md),  [TextEdit](classTextEdit.md),  [UuidGenerator](classUuidGenerator.md),  [ValueMap](classValueMap.md),  [ValueRelation](classValueRelation.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classEditorWidgetBase.md#property-menu)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |
































## Public Properties Documentation




### property hasMenu 

```C++
bool EditorWidgetBase::hasMenu;
```




<hr>



### property isEmpty 

```C++
bool EditorWidgetBase::isEmpty;
```





**Remark:**

This property is read-only 





        

<hr>



### property isLoaded 

```C++
bool EditorWidgetBase::isLoaded;
```



This property indicates whether the editor widget has been fully loaded by its Loader. Note: prior to this property being true, signals emitted by the editor widget will not be propagated. 


        

<hr>



### property isNull 

```C++
bool EditorWidgetBase::isNull;
```





**Remark:**

This property is read-only 





        

<hr>



### property menu 

```C++
Menu EditorWidgetBase::menu;
```




<hr>
## Public Signals Documentation




### signal requestBarcode 

```C++
void EditorWidgetBase::requestBarcode;
```



This signal is emitted when an editor widget is requesting a barcode value. The decoded barcode value will be returned as a string through calling a requestedBarcodeReceived(string) function attached to editor widget which signaled the request. The corresponding handler is `onRequestBarcode`. 


        

<hr>



### signal requestGeometry 

```C++
void EditorWidgetBase::requestGeometry;
```



This signal is emitted when an editor widget is in need of a digitized geometry. The geometry will be returned through calling a requestedGeometryReceived(geometry) function attached to editor widget which signaled the request. The corresponding handler is `onRequestGeometry`. 


        

<hr>



### signal requestJumpToPoint 

```C++
void EditorWidgetBase::requestJumpToPoint;
```



This signal is emitted when an editor widget is requesting to jump to a specific point on the map. The corresponding handler is `onRequestJumpToPoint`. 


        

<hr>



### signal valueChangeRequested 

```C++
void EditorWidgetBase::valueChangeRequested;
```



This signal is emmited when an editor widget has changed the value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/editorwidgets/EditorWidgetBase.qml`

