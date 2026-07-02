

# Class DigitizingToolbar



[**ClassList**](annotated.md) **>** [**DigitizingToolbar**](classDigitizingToolbar.md)








Inherits the following classes: [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**cancelDialog**](classDigitizingToolbar.md#property-canceldialog-12)  <br> |
| property bool | [**cogoEnabled**](classDigitizingToolbar.md#property-cogoenabled)  <br> |
| property alias | [**cogoExecutor**](classDigitizingToolbar.md#property-cogoexecutor-12)  <br> |
| property var | [**cogoOperationSettings**](classDigitizingToolbar.md#property-cogooperationsettings)  <br> |
| property alias | [**digitizingLogger**](classDigitizingToolbar.md#property-digitizinglogger-12)  <br> |
| property bool | [**geometryRequested**](classDigitizingToolbar.md#property-geometryrequested)  <br> |
| property var | [**geometryRequestedItem**](classDigitizingToolbar.md#property-geometryrequesteditem)  <br> |
| property VectorLayer | [**geometryRequestedLayer**](classDigitizingToolbar.md#property-geometryrequestedlayer)  <br> |
| property bool | [**geometryValid**](classDigitizingToolbar.md#property-geometryvalid)  <br> |
| property bool | [**isDigitizing**](classDigitizingToolbar.md#property-isdigitizing)  <br>_Readonly._  |
| property MapSettings | [**mapSettings**](classDigitizingToolbar.md#property-mapsettings)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) | [**rubberbandModel**](classDigitizingToolbar.md#property-rubberbandmodel)  <br> |
| property bool | [**screenHovering**](classDigitizingToolbar.md#property-screenhovering)  <br> |
| property bool | [**showConfirmButton**](classDigitizingToolbar.md#property-showconfirmbutton)  <br> |


## Public Properties inherited from Theme::QfVisibilityFadingRow

See [Theme::QfVisibilityFadingRow](classTheme_1_1QfVisibilityFadingRow.md)

| Type | Name |
| ---: | :--- |
| property bool | [**stateVisible**](classTheme_1_1QfVisibilityFadingRow.md#property-statevisible)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cancel**](classDigitizingToolbar.md#signal-cancel)  <br> |
| signal void | [**confirmed**](classDigitizingToolbar.md#signal-confirmed)  <br> |
| signal void | [**requestJumpToPoint**](classDigitizingToolbar.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classDigitizingToolbar.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |
| signal void | [**vertexCountChanged**](classDigitizingToolbar.md#signal-vertexcountchanged)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**addVertex**](#function-addvertex) () <br> |
|  void | [**checkAccuracyRequirement**](#function-checkaccuracyrequirement) () <br> |
|  void | [**checkGeometryValidity**](#function-checkgeometryvalidity) () <br> |
|  void | [**confirm**](#function-confirm) () <br> |
|  void | [**removeVertex**](#function-removevertex) () <br> |
|  void | [**triggerAddVertex**](#function-triggeraddvertex) () <br> |
























































## Public Properties Documentation




### property cancelDialog [1/2]

```C++
alias DigitizingToolbar::cancelDialog;
```




<hr>



### property cogoEnabled 

```C++
bool DigitizingToolbar::cogoEnabled;
```




<hr>



### property cogoExecutor [1/2]

```C++
alias DigitizingToolbar::cogoExecutor;
```




<hr>



### property cogoOperationSettings 

```C++
var DigitizingToolbar::cogoOperationSettings;
```




<hr>



### property digitizingLogger [1/2]

```C++
alias DigitizingToolbar::digitizingLogger;
```




<hr>



### property geometryRequested 

```C++
bool DigitizingToolbar::geometryRequested;
```




<hr>



### property geometryRequestedItem 

```C++
var DigitizingToolbar::geometryRequestedItem;
```




<hr>



### property geometryRequestedLayer 

```C++
VectorLayer DigitizingToolbar::geometryRequestedLayer;
```




<hr>



### property geometryValid 

```C++
bool DigitizingToolbar::geometryValid;
```




<hr>



### property isDigitizing 

_Readonly._ 
```C++
bool DigitizingToolbar::isDigitizing;
```





**Remark:**

This property is read-only 





        

<hr>



### property mapSettings 

```C++
MapSettings DigitizingToolbar::mapSettings;
```




<hr>



### property rubberbandModel 

```C++
RubberbandModel DigitizingToolbar::rubberbandModel;
```




<hr>



### property screenHovering 

```C++
bool DigitizingToolbar::screenHovering;
```




<hr>



### property showConfirmButton 

```C++
bool DigitizingToolbar::showConfirmButton;
```




<hr>
## Public Signals Documentation




### signal cancel 

```C++
void DigitizingToolbar::cancel;
```




<hr>



### signal confirmed 

```C++
void DigitizingToolbar::confirmed;
```




<hr>



### signal requestJumpToPoint 

```C++
void DigitizingToolbar::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void DigitizingToolbar::requestPosition;
```




<hr>



### signal vertexCountChanged 

```C++
void DigitizingToolbar::vertexCountChanged;
```




<hr>
## Public Functions Documentation




### function addVertex 

```C++
void DigitizingToolbar::addVertex () 
```




<hr>



### function checkAccuracyRequirement 

```C++
void DigitizingToolbar::checkAccuracyRequirement () 
```




<hr>



### function checkGeometryValidity 

```C++
void DigitizingToolbar::checkGeometryValidity () 
```




<hr>



### function confirm 

```C++
void DigitizingToolbar::confirm () 
```




<hr>



### function removeVertex 

```C++
void DigitizingToolbar::removeVertex () 
```




<hr>



### function triggerAddVertex 

```C++
void DigitizingToolbar::triggerAddVertex () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/DigitizingToolbar.qml`

