

# Class distance



[**ClassList**](annotated.md) **>** [**distance**](classdistance.md)








Inherits the following classes: [CogoParameterWidgetBase](classCogoParameterWidgetBase.md),  [ProcessingParameterWidgetBase](classProcessingParameterWidgetBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**currentValue**](classdistance.md#property-currentvalue)  <br> |
| property bool | [**distanceConvertible**](classdistance.md#property-distanceconvertible)  <br> |
| property int | [**distanceUnit**](classdistance.md#property-distanceunit)  <br> |
| property real | [**max**](classdistance.md#property-max)  <br> |
| property real | [**min**](classdistance.md#property-min)  <br> |
| property real | [**step**](classdistance.md#property-step)  <br> |














## Public Signals inherited from CogoParameterWidgetBase

See [CogoParameterWidgetBase](classCogoParameterWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**requestPosition**](classCogoParameterWidgetBase.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |
| signal void | [**valueChangeRequested**](classCogoParameterWidgetBase.md#signal-valuechangerequested) (var value) <br> |


## Public Signals inherited from ProcessingParameterWidgetBase

See [ProcessingParameterWidgetBase](classProcessingParameterWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**valueChangeRequested**](classProcessingParameterWidgetBase.md#signal-valuechangerequested) (var value) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**adjustValue**](#function-adjustvalue) (direction) <br> |
|  void | [**prepareValueChangeRequest**](#function-preparevaluechangerequest) (newValue) <br> |
|  void | [**processValue**](#function-processvalue) () <br> |




















































































## Public Properties Documentation




### property currentValue 

```C++
double distance::currentValue;
```




<hr>



### property distanceConvertible 

```C++
bool distance::distanceConvertible;
```




<hr>



### property distanceUnit 

```C++
int distance::distanceUnit;
```




<hr>



### property max 

```C++
real distance::max;
```




<hr>



### property min 

```C++
real distance::min;
```




<hr>



### property step 

```C++
real distance::step;
```




<hr>
## Public Functions Documentation




### function adjustValue 

```C++
void distance::adjustValue (
    direction
) 
```




<hr>



### function prepareValueChangeRequest 

```C++
void distance::prepareValueChangeRequest (
    newValue
) 
```




<hr>



### function processValue 

```C++
void distance::processValue () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/cogoparameterwidgets/distance.qml`

