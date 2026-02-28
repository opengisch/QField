

# Class CogoParameterWidgetBase



[**ClassList**](annotated.md) **>** [**CogoParameterWidgetBase**](classCogoParameterWidgetBase.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [angle](classangle.md),  [distance](classdistance.md),  [elevation](classelevation.md),  [enum](classenum.md),  [point](classpoint.md)




























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestPosition**](classCogoParameterWidgetBase.md#signal-requestposition) (var item, bool fromCoordinateLocator) <br> |
| signal void | [**valueChangeRequested**](classCogoParameterWidgetBase.md#signal-valuechangerequested) (var value) <br> |
































## Public Signals Documentation




### signal requestPosition 

```C++
void CogoParameterWidgetBase::requestPosition;
```



This signal is emitted when a parameter requests a position from the internal/external GNSS. 


        

<hr>



### signal valueChangeRequested 

```C++
void CogoParameterWidgetBase::valueChangeRequested;
```



This signal is emmited when a processing parameter widget has changed the value. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/cogoparameterwidgets/CogoParameterWidgetBase.qml`

