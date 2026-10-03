

# Class QfCoordinateLocator



[**ClassList**](annotated.md) **>** [**QfCoordinateLocator**](classQfCoordinateLocator.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [CoordinateLocator](classCoordinateLocator.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**averagedPosition**](classQfCoordinateLocator.md#property-averagedposition)  <br> |
| property int | [**averagedPositionCount**](classQfCoordinateLocator.md#property-averagedpositioncount)  <br> |
| property real | [**crossHalfLength**](classQfCoordinateLocator.md#property-crosshalflength)  <br> |
| property variant | [**currentCoordinate**](classQfCoordinateLocator.md#property-currentcoordinate)  <br> |
| property alias | [**currentLayer**](classQfCoordinateLocator.md#property-currentlayer)  <br> |
| property color | [**cursorFillColor**](classQfCoordinateLocator.md#property-cursorfillcolor)  <br> |
| property color | [**cursorOutlineColor**](classQfCoordinateLocator.md#property-cursoroutlinecolor)  <br> |
| property real | [**cursorSizeScale**](classQfCoordinateLocator.md#property-cursorsizescale)  <br> |
| property point | [**displayPosition**](classQfCoordinateLocator.md#property-displayposition)  <br> |
| property color | [**highlightColor**](classQfCoordinateLocator.md#property-highlightcolor)  <br> |
| property color | [**mainColor**](classQfCoordinateLocator.md#property-maincolor)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfCoordinateLocator.md#property-mapsettings)  <br> |
| property variant | [**overrideLocation**](classQfCoordinateLocator.md#property-overridelocation)  <br> |
| property var | [**positionInformation**](classQfCoordinateLocator.md#property-positioninformation)  <br> |
| property bool | [**positionLocked**](classQfCoordinateLocator.md#property-positionlocked)  <br> |
| property variant | [**rubberbandModel**](classQfCoordinateLocator.md#property-rubberbandmodel)  <br> |
| property bool | [**snapToCommonAngles**](classQfCoordinateLocator.md#property-snaptocommonangles)  <br> |
| property alias | [**snappedCoordinate**](classQfCoordinateLocator.md#property-snappedcoordinate)  <br> |
| property alias | [**snappedPoint**](classQfCoordinateLocator.md#property-snappedpoint)  <br> |
| property real | [**snappingAngleDegrees**](classQfCoordinateLocator.md#property-snappingangledegrees)  <br> |
| property bool | [**snappingIsRelative**](classQfCoordinateLocator.md#property-snappingisrelative)  <br> |
| property real | [**snappingTolerance**](classQfCoordinateLocator.md#property-snappingtolerance)  <br> |
| property variant | [**sourceLocation**](classQfCoordinateLocator.md#property-sourcelocation)  <br> |
| property alias | [**topSnappingResult**](classQfCoordinateLocator.md#property-topsnappingresult)  <br> |




































## Public Properties Documentation




### property averagedPosition 

```C++
bool QfCoordinateLocator::averagedPosition;
```




<hr>



### property averagedPositionCount 

```C++
int QfCoordinateLocator::averagedPositionCount;
```




<hr>



### property crossHalfLength 

```C++
real QfCoordinateLocator::crossHalfLength;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentCoordinate 

```C++
variant QfCoordinateLocator::currentCoordinate;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentLayer 

```C++
alias QfCoordinateLocator::currentLayer;
```



Set the current layer on which snapping should be performed. 


        

<hr>



### property cursorFillColor 

```C++
color QfCoordinateLocator::cursorFillColor;
```




<hr>



### property cursorOutlineColor 

```C++
color QfCoordinateLocator::cursorOutlineColor;
```




<hr>



### property cursorSizeScale 

```C++
real QfCoordinateLocator::cursorSizeScale;
```




<hr>



### property displayPosition 

```C++
point QfCoordinateLocator::displayPosition;
```





**Remark:**

This property is read-only 





        

<hr>



### property highlightColor 

```C++
color QfCoordinateLocator::highlightColor;
```




<hr>



### property mainColor 

```C++
color QfCoordinateLocator::mainColor;
```




<hr>



### property mapSettings 

```C++
QgsQuickMapSettings QfCoordinateLocator::mapSettings;
```




<hr>



### property overrideLocation 

```C++
variant QfCoordinateLocator::overrideLocation;
```



Overrides any possibility for the user to modify the coordinate. There will be no user interaction or snapping if this is set to a QgsPoint. Set this to `undefined` to revert to the user and snapping controlled behavior. 


        

<hr>



### property positionInformation 

```C++
var QfCoordinateLocator::positionInformation;
```



Position-related information 


        

<hr>



### property positionLocked 

```C++
bool QfCoordinateLocator::positionLocked;
```




<hr>



### property rubberbandModel 

```C++
variant QfCoordinateLocator::rubberbandModel;
```




<hr>



### property snapToCommonAngles 

```C++
bool QfCoordinateLocator::snapToCommonAngles;
```



Snapping-related information 


        

<hr>



### property snappedCoordinate 

```C++
alias QfCoordinateLocator::snappedCoordinate;
```





**Remark:**

This property is read-only 





        

<hr>



### property snappedPoint 

```C++
alias QfCoordinateLocator::snappedPoint;
```





**Remark:**

This property is read-only 





        

<hr>



### property snappingAngleDegrees 

```C++
real QfCoordinateLocator::snappingAngleDegrees;
```




<hr>



### property snappingIsRelative 

```C++
bool QfCoordinateLocator::snappingIsRelative;
```




<hr>



### property snappingTolerance 

```C++
real QfCoordinateLocator::snappingTolerance;
```




<hr>



### property sourceLocation 

```C++
variant QfCoordinateLocator::sourceLocation;
```



Source location for snapping. If this is set to undefined, the center of the screen will be used. Overwritten by stylus / hoverHandler. 


        

<hr>



### property topSnappingResult 

```C++
alias QfCoordinateLocator::topSnappingResult;
```





**Remark:**

This property is read-only 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qml/QfCoordinateLocator.qml`

