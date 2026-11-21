

# Class QfCogoOperations



[**ClassList**](annotated.md) **>** [**QfCogoOperations**](classQfCogoOperations.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [CogoOperations](classCogoOperations.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**cogoOperationSettings**](classQfCogoOperations.md#property-cogooperationsettings)  <br> |
| property [**QfFeatureModel**](classQfFeatureModel.md) | [**featureModel**](classQfCogoOperations.md#property-featuremodel)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfCogoOperations.md#property-mapsettings)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) | [**rubberbandModel**](classQfCogoOperations.md#property-rubberbandmodel)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classQfCogoOperations.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classQfCogoOperations.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**canvasClicked**](#function-canvasclicked) (point, type) <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) (point, type) <br> |




























## Public Properties Documentation




### property cogoOperationSettings 

```C++
var QfCogoOperations::cogoOperationSettings;
```




<hr>



### property featureModel 

```C++
QfFeatureModel QfCogoOperations::featureModel;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfCogoOperations::mapSettings;
```




<hr>



### property rubberbandModel 

```C++
QfRubberbandModel QfCogoOperations::rubberbandModel;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void QfCogoOperations::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void QfCogoOperations::requestPosition;
```




<hr>
## Public Functions Documentation




### function canvasClicked 

```C++
void QfCogoOperations::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void QfCogoOperations::canvasLongPressed (
    point,
    type
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfCogoOperations.qml`

