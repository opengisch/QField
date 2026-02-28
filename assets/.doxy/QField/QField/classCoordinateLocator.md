

# Class CoordinateLocator



[**ClassList**](annotated.md) **>** [**CoordinateLocator**](classCoordinateLocator.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**averagedPosition**](classCoordinateLocator.md#property-averagedposition)  <br> |
| property int | [**averagedPositionCount**](classCoordinateLocator.md#property-averagedpositioncount)  <br> |
| property variant | [**currentCoordinate**](classCoordinateLocator.md#property-currentcoordinate)  <br> |
| property alias | [**currentLayer**](classCoordinateLocator.md#property-currentlayer)  <br> |
| property [**point**](classpoint.md) | [**displayPosition**](classCoordinateLocator.md#property-displayposition)  <br> |
| property color | [**highlightColor**](classCoordinateLocator.md#property-highlightcolor)  <br> |
| property color | [**mainColor**](classCoordinateLocator.md#property-maincolor)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classCoordinateLocator.md#property-mapsettings)  <br> |
| property variant | [**overrideLocation**](classCoordinateLocator.md#property-overridelocation)  <br> |
| property var | [**positionInformation**](classCoordinateLocator.md#property-positioninformation)  <br> |
| property bool | [**positionLocked**](classCoordinateLocator.md#property-positionlocked)  <br> |
| property variant | [**rubberbandModel**](classCoordinateLocator.md#property-rubberbandmodel)  <br> |
| property bool | [**snapToCommonAngles**](classCoordinateLocator.md#property-snaptocommonangles)  <br> |
| property alias | [**snappedCoordinate**](classCoordinateLocator.md#property-snappedcoordinate)  <br> |
| property alias | [**snappedPoint**](classCoordinateLocator.md#property-snappedpoint)  <br> |
| property real | [**snappingAngleDegrees**](classCoordinateLocator.md#property-snappingangledegrees)  <br> |
| property bool | [**snappingIsRelative**](classCoordinateLocator.md#property-snappingisrelative)  <br> |
| property real | [**snappingTolerance**](classCoordinateLocator.md#property-snappingtolerance)  <br> |
| property variant | [**sourceLocation**](classCoordinateLocator.md#property-sourcelocation)  <br> |
| property alias | [**topSnappingResult**](classCoordinateLocator.md#property-topsnappingresult)  <br> |




































## Public Properties Documentation




### property averagedPosition 

```C++
bool CoordinateLocator::averagedPosition;
```




<hr>



### property averagedPositionCount 

```C++
int CoordinateLocator::averagedPositionCount;
```




<hr>



### property currentCoordinate 

```C++
variant CoordinateLocator::currentCoordinate;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentLayer 

```C++
alias CoordinateLocator::currentLayer;
```



Set the current layer on which snapping should be performed. 


        

<hr>



### property displayPosition 

```C++
point CoordinateLocator::displayPosition;
```





**Remark:**

This property is read-only 





        

<hr>



### property highlightColor 

```C++
color CoordinateLocator::highlightColor;
```




<hr>



### property mainColor 

```C++
color CoordinateLocator::mainColor;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings CoordinateLocator::mapSettings;
```




<hr>



### property overrideLocation 

```C++
variant CoordinateLocator::overrideLocation;
```



Overrides any possibility for the user to modify the coordinate. There will be no user interaction or snapping if this is set to a QgsPoint. Set this to `undefined` to revert to the user and snapping controlled behavior. 


        

<hr>



### property positionInformation 

```C++
var CoordinateLocator::positionInformation;
```



Position-related information 


        

<hr>



### property positionLocked 

```C++
bool CoordinateLocator::positionLocked;
```




<hr>



### property rubberbandModel 

```C++
variant CoordinateLocator::rubberbandModel;
```




<hr>



### property snapToCommonAngles 

```C++
bool CoordinateLocator::snapToCommonAngles;
```



Snapping-related information 


        

<hr>



### property snappedCoordinate 

```C++
alias CoordinateLocator::snappedCoordinate;
```





**Remark:**

This property is read-only 





        

<hr>



### property snappedPoint 

```C++
alias CoordinateLocator::snappedPoint;
```





**Remark:**

This property is read-only 





        

<hr>



### property snappingAngleDegrees 

```C++
real CoordinateLocator::snappingAngleDegrees;
```




<hr>



### property snappingIsRelative 

```C++
bool CoordinateLocator::snappingIsRelative;
```




<hr>



### property snappingTolerance 

```C++
real CoordinateLocator::snappingTolerance;
```




<hr>



### property sourceLocation 

```C++
variant CoordinateLocator::sourceLocation;
```



Source location for snapping. If this is set to undefined, the center of the screen will be used. Overwritten by stylus / hoverHandler. 


        

<hr>



### property topSnappingResult 

```C++
alias CoordinateLocator::topSnappingResult;
```





**Remark:**

This property is read-only 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/CoordinateLocator.qml`

