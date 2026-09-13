

# Class QfCogoOperationSettings



[**ClassList**](annotated.md) **>** [**QfCogoOperationSettings**](classQfCogoOperationSettings.md)








Inherits the following classes: [QfOverlayContainer](classQfOverlayContainer.md)


Inherited by the following classes: [CogoOperationSettings](classCogoOperationSettings.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfCogoOperationSettings.md#property-mapsettings)  <br> |
| property string | [**name**](classQfCogoOperationSettings.md#property-name)  <br> |
| property var | [**parameterValues**](classQfCogoOperationSettings.md#property-parametervalues)  <br> |
| property var | [**parameters**](classQfCogoOperationSettings.md#property-parameters)  <br> |
| property gnssPositionInformation | [**positionInformation**](classQfCogoOperationSettings.md#property-positioninformation)  <br> |
| property var | [**positionInformations**](classQfCogoOperationSettings.md#property-positioninformations)  <br> |


## Public Properties inherited from QfOverlayContainer

See [QfOverlayContainer](classQfOverlayContainer.md)

| Type | Name |
| ---: | :--- |
| property alias | [**contents**](classQfOverlayContainer.md#property-contents)  <br> |
| property alias | [**header**](classQfOverlayContainer.md#property-header)  <br> |
| property alias | [**layout**](classQfOverlayContainer.md#property-layout)  <br> |
| property alias | [**title**](classQfOverlayContainer.md#property-title-12)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classQfCogoOperationSettings.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classQfCogoOperationSettings.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**clear**](#function-clear) () <br> |
|  void | [**requestedPositionReceived**](#function-requestedpositionreceived) (name, position, positionInformation) <br> |
























































## Public Properties Documentation




### property mapSettings 

```C++
QgsQuickMapSettings QfCogoOperationSettings::mapSettings;
```




<hr>



### property name 

```C++
string QfCogoOperationSettings::name;
```




<hr>



### property parameterValues 

```C++
var QfCogoOperationSettings::parameterValues;
```




<hr>



### property parameters 

```C++
var QfCogoOperationSettings::parameters;
```




<hr>



### property positionInformation 

```C++
gnssPositionInformation QfCogoOperationSettings::positionInformation;
```




<hr>



### property positionInformations 

```C++
var QfCogoOperationSettings::positionInformations;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void QfCogoOperationSettings::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void QfCogoOperationSettings::requestPosition;
```




<hr>
## Public Functions Documentation




### function clear 

```C++
void QfCogoOperationSettings::clear () 
```




<hr>



### function requestedPositionReceived 

```C++
void QfCogoOperationSettings::requestedPositionReceived (
    name,
    position,
    positionInformation
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfCogoOperationSettings.qml`

