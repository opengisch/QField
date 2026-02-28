

# Class CogoOperations



[**ClassList**](annotated.md) **>** [**CogoOperations**](classCogoOperations.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**cogoOperationSettings**](classCogoOperations.md#property-cogooperationsettings)  <br> |
| property [**FeatureModel**](classFeatureModel.md) | [**featureModel**](classCogoOperations.md#property-featuremodel)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classCogoOperations.md#property-mapsettings)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) | [**rubberbandModel**](classCogoOperations.md#property-rubberbandmodel)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classCogoOperations.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**requestPosition**](classCogoOperations.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**canvasClicked**](#function-canvasclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**canvasLongPressed**](#function-canvaslongpressed) ([**point**](classpoint.md), type) <br> |




























## Public Properties Documentation




### property cogoOperationSettings 

```C++
var CogoOperations::cogoOperationSettings;
```




<hr>



### property featureModel 

```C++
FeatureModel CogoOperations::featureModel;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings CogoOperations::mapSettings;
```




<hr>



### property rubberbandModel 

```C++
RubberbandModel CogoOperations::rubberbandModel;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void CogoOperations::requestJumpToPoint;
```




<hr>



### signal requestPosition 

```C++
void CogoOperations::requestPosition;
```




<hr>
## Public Functions Documentation




### function canvasClicked 

```C++
void CogoOperations::canvasClicked (
    point,
    type
) 
```




<hr>



### function canvasLongPressed 

```C++
void CogoOperations::canvasLongPressed (
    point,
    type
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/CogoOperations.qml`

