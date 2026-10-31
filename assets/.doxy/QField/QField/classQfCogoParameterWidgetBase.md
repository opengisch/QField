

# Class QfCogoParameterWidgetBase



[**ClassList**](annotated.md) **>** [**QfCogoParameterWidgetBase**](classQfCogoParameterWidgetBase.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [QfCogoParameterAngle](classQfCogoParameterAngle.md),  [QfCogoParameterDistance](classQfCogoParameterDistance.md),  [QfCogoParameterElevation](classQfCogoParameterElevation.md),  [QfCogoParameterEnum](classQfCogoParameterEnum.md),  [QfCogoParameterPoint](classQfCogoParameterPoint.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestPosition**](classQfCogoParameterWidgetBase.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |
| signal void | [**valueChangeRequested**](classQfCogoParameterWidgetBase.md#signal-valuechangerequested) (var value) <br> |
































## Public Signals Documentation




### signal requestPosition 

```C++
void QfCogoParameterWidgetBase::requestPosition;
```



This signal is emitted when a parameter requests a position from the internal/external GNSS. 


        

<hr>



### signal valueChangeRequested 

```C++
void QfCogoParameterWidgetBase::valueChangeRequested;
```



This signal is emmited when a processing parameter widget has changed the value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/cogoparameterwidgets/QfCogoParameterWidgetBase.qml`

