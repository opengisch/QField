

# Class QfDigitizingToolbar



[**ClassList**](annotated.md) **>** [**QfDigitizingToolbar**](classQfDigitizingToolbar.md)








Inherits the following classes: [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)


Inherited by the following classes: [DigitizingToolbar](classDigitizingToolbar.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**cancelDialog**](classQfDigitizingToolbar.md#property-canceldialog-12)  <br> |
| property bool | [**cogoEnabled**](classQfDigitizingToolbar.md#property-cogoenabled)  <br> |
| property alias | [**cogoExecutor**](classQfDigitizingToolbar.md#property-cogoexecutor-12)  <br> |
| property var | [**cogoOperationSettings**](classQfDigitizingToolbar.md#property-cogooperationsettings)  <br> |
| property alias | [**digitizingLogger**](classQfDigitizingToolbar.md#property-digitizinglogger-12)  <br> |
| property bool | [**geometryRequested**](classQfDigitizingToolbar.md#property-geometryrequested)  <br> |
| property var | [**geometryRequestedItem**](classQfDigitizingToolbar.md#property-geometryrequesteditem)  <br> |
| property VectorLayer | [**geometryRequestedLayer**](classQfDigitizingToolbar.md#property-geometryrequestedlayer)  <br> |
| property bool | [**geometryValid**](classQfDigitizingToolbar.md#property-geometryvalid)  <br> |
| property bool | [**isDigitizing**](classQfDigitizingToolbar.md#property-isdigitizing)  <br>_Readonly._  |
| property MapSettings | [**mapSettings**](classQfDigitizingToolbar.md#property-mapsettings)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) | [**rubberbandModel**](classQfDigitizingToolbar.md#property-rubberbandmodel)  <br> |
| property bool | [**screenHovering**](classQfDigitizingToolbar.md#property-screenhovering)  <br> |
| property bool | [**showConfirmButton**](classQfDigitizingToolbar.md#property-showconfirmbutton)  <br> |


## Public Properties inherited from QfVisibilityFadingRow

See [QfVisibilityFadingRow](classQfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classQfVisibilityFadingRow.md#property-statevisible)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cancel**](classQfDigitizingToolbar.md#signal-cancel)  <br> |
| signal void | [**confirmed**](classQfDigitizingToolbar.md#signal-confirmed)  <br> |
| signal void | [**requestJumpToPoint**](classQfDigitizingToolbar.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classQfDigitizingToolbar.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |
| signal void | [**vertexCountChanged**](classQfDigitizingToolbar.md#signal-vertexcountchanged)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**addVertex**](#function-addvertex) () <br> |
|  void | [**checkAccuracyRequirement**](#function-checkaccuracyrequirement) () <br> |
|  void | [**checkGeometryValidity**](#function-checkgeometryvalidity) () <br> |
|  void | [**confirm**](#function-confirm) () <br> |
|  void | [**processVertexCountChanged**](#function-processvertexcountchanged) () <br> |
|  void | [**removeVertex**](#function-removevertex) () <br> |
|  void | [**triggerAddVertex**](#function-triggeraddvertex) () <br> |
























































## Public Properties Documentation




### property cancelDialog [1/2]

```C++
alias QfDigitizingToolbar::cancelDialog;
```




<hr>



### property cogoEnabled 

```C++
bool QfDigitizingToolbar::cogoEnabled;
```




<hr>



### property cogoExecutor [1/2]

```C++
alias QfDigitizingToolbar::cogoExecutor;
```




<hr>



### property cogoOperationSettings 

```C++
var QfDigitizingToolbar::cogoOperationSettings;
```




<hr>



### property digitizingLogger [1/2]

```C++
alias QfDigitizingToolbar::digitizingLogger;
```




<hr>



### property geometryRequested 

```C++
bool QfDigitizingToolbar::geometryRequested;
```




<hr>



### property geometryRequestedItem 

```C++
var QfDigitizingToolbar::geometryRequestedItem;
```




<hr>



### property geometryRequestedLayer 

```C++
VectorLayer QfDigitizingToolbar::geometryRequestedLayer;
```




<hr>



### property geometryValid 

```C++
bool QfDigitizingToolbar::geometryValid;
```




<hr>



### property isDigitizing 

_Readonly._ 
```C++
bool QfDigitizingToolbar::isDigitizing;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapSettings 

```C++
MapSettings QfDigitizingToolbar::mapSettings;
```




<hr>



### property rubberbandModel 

```C++
QfRubberbandModel QfDigitizingToolbar::rubberbandModel;
```




<hr>



### property screenHovering 

```C++
bool QfDigitizingToolbar::screenHovering;
```




<hr>



### property showConfirmButton 

```C++
bool QfDigitizingToolbar::showConfirmButton;
```




<hr>
## Public Signals Documentation




### signal cancel 

```C++
void QfDigitizingToolbar::cancel;
```




<hr>



### signal confirmed 

```C++
void QfDigitizingToolbar::confirmed;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfDigitizingToolbar::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void QfDigitizingToolbar::requestPosition;
```




<hr>



### signal vertexCountChanged 

```C++
void QfDigitizingToolbar::vertexCountChanged;
```




<hr>
## Public Functions Documentation




### function addVertex 

```C++
void QfDigitizingToolbar::addVertex () 
```




<hr>



### function checkAccuracyRequirement 

```C++
void QfDigitizingToolbar::checkAccuracyRequirement () 
```




<hr>



### function checkGeometryValidity 

```C++
void QfDigitizingToolbar::checkGeometryValidity () 
```




<hr>



### function confirm 

```C++
void QfDigitizingToolbar::confirm () 
```




<hr>



### function processVertexCountChanged 

```C++
void QfDigitizingToolbar::processVertexCountChanged () 
```




<hr>



### function removeVertex 

```C++
void QfDigitizingToolbar::removeVertex () 
```




<hr>



### function triggerAddVertex 

```C++
void QfDigitizingToolbar::triggerAddVertex () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfDigitizingToolbar.qml`

