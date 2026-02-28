

# Class CogoOperationSettings



[**ClassList**](annotated.md) **>** [**CogoOperationSettings**](classCogoOperationSettings.md)








Inherits the following classes: [Theme::QfOverlayContainer](classTheme_1_1QfOverlayContainer.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classCogoOperationSettings.md#property-mapsettings)  <br> |
| property string | [**name**](classCogoOperationSettings.md#property-name)  <br> |
| property var | [**parameterValues**](classCogoOperationSettings.md#property-parametervalues)  <br> |
| property var | [**parameters**](classCogoOperationSettings.md#property-parameters)  <br> |
| property [**GnssPositionInformation**](classGnssPositionInformation.md) | [**positionInformation**](classCogoOperationSettings.md#property-positioninformation)  <br> |
| property var | [**positionInformations**](classCogoOperationSettings.md#property-positioninformations)  <br> |


## Public Properties inherited from Theme::QfOverlayContainer

See [Theme::QfOverlayContainer](classTheme_1_1QfOverlayContainer.md)

| Type | Name |
| ---: | :--- |
| property alias | [**contents**](classTheme_1_1QfOverlayContainer.md#property-contents)  <br> |
| property alias | [**layout**](classTheme_1_1QfOverlayContainer.md#property-layout)  <br> |
| property alias | [**title**](classTheme_1_1QfOverlayContainer.md#property-title-12)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classCogoOperationSettings.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classCogoOperationSettings.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**clear**](#function-clear) () <br> |
|  void | [**requestedPositionReceived**](#function-requestedpositionreceived) (name, position, positionInformation) <br> |
























































## Public Properties Documentation




### property mapSettings 

```C++
QgsQuickMapSettings CogoOperationSettings::mapSettings;
```




<hr>



### property name 

```C++
string CogoOperationSettings::name;
```




<hr>



### property parameterValues 

```C++
var CogoOperationSettings::parameterValues;
```




<hr>



### property parameters 

```C++
var CogoOperationSettings::parameters;
```




<hr>



### property positionInformation 

```C++
GnssPositionInformation CogoOperationSettings::positionInformation;
```




<hr>



### property positionInformations 

```C++
var CogoOperationSettings::positionInformations;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void CogoOperationSettings::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void CogoOperationSettings::requestPosition;
```




<hr>
## Public Functions Documentation




### function clear 

```C++
void CogoOperationSettings::clear () 
```




<hr>



### function requestedPositionReceived 

```C++
void CogoOperationSettings::requestedPositionReceived (
    name,
    position,
    positionInformation
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/CogoOperationSettings.qml`

