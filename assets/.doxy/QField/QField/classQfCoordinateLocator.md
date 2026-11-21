

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








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**calculateSnapToAngleLineEndCoords**](#function-calculatesnaptoanglelineendcoords) (currentPoint, angleDegrees, isRelativeAngle, screenSize, forwardMode=false) <br> |
|  void | [**flash**](#function-flash) () <br> |
|  void | [**getCommonAngleInDegrees**](#function-getcommonangleindegrees) (currentPoint, rubberbandModel, commonAngleStepDeg, isRelativeAngle, forwardMode=false) <br> |
|  void | [**getToleranceMultiplier**](#function-gettolerancemultiplier) () <br> |
|  void | [**snapPointToCommonAngle**](#function-snappointtocommonangle) (currentPoint, rubberbandModel, commonAngleDegrees, isRelativeAngle, forwardMode=false) <br> |




























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
## Public Functions Documentation




### function calculateSnapToAngleLineEndCoords 

```C++
void QfCoordinateLocator::calculateSnapToAngleLineEndCoords (
    currentPoint,
    angleDegrees,
    isRelativeAngle,
    screenSize,
    forwardMode=false
) 
```



Calculates snap to angle guide line end coordinate. 

**Parameters:**


* `{QPointF}` currentPoint - the current point being proposed 
* `{number}` angleDegrees - angle of the line in degrees. 
* `{boolean}` isRelativeAngle - whether the angle should be calculated relative to the last geometry segment 
* `{number}` screenSize - size of the screen. Used to make sure the end of the line is outside the screen. 
* `{number}` forwardMode - true: aimed towards first point and false: snap to previous point 




        

<hr>



### function flash 

```C++
void QfCoordinateLocator::flash () 
```




<hr>



### function getCommonAngleInDegrees 

```C++
void QfCoordinateLocator::getCommonAngleInDegrees (
    currentPoint,
    rubberbandModel,
    commonAngleStepDeg,
    isRelativeAngle,
    forwardMode=false
) 
```



Computes the possible common angle




**Parameters:**


* `{QPointF}` currentPoint - the current point being proposed 
* `{QfRubberband}` rubberbandModel - holds all previously added points in the current digitizing session 
* `{number}` commonAngleStepDeg - the common angle in degrees we are trying to snap to 
* `{boolean}` isRelativeAngle - whether the angle should be calculated relative to the last geometry segment 



**Returns:**

{number\|undefined} - the closest common angle. Returns`undefined` if there is no nearby point that forms a common angle. 




**Parameters:**


* `{boolean}` forwardMode - true: snap to firstPoint and false: snap to previousPoint 




        

<hr>



### function getToleranceMultiplier 

```C++
void QfCoordinateLocator::getToleranceMultiplier () 
```



Function to get the multiplier based on the selected tolerance



* Narrow tolerance (index 0) divides by 2.
* Normal tolerance (index 1) keeps unchanged.
* Large tolerance (index 2) multiplies by 4. 




        

<hr>



### function snapPointToCommonAngle 

```C++
void QfCoordinateLocator::snapPointToCommonAngle (
    currentPoint,
    rubberbandModel,
    commonAngleDegrees,
    isRelativeAngle,
    forwardMode=false
) 
```



Computes a new point that forms a common angle




**Parameters:**


* `{QPointF}` currentPoint - the current point being proposed 
* `{QfRubberband}` rubberbandModel - holds all previously added points in the current digitizing session 
* `{number}` commonAngleDegrees - the common angle in degrees we are snapping to 
* `{boolean}` isRelativeAngle - whether the angle should be calculated relative to the last geometry segment 
* `{boolean}` forwardMode - true: snap to firstPoint and false: snap to previousPoint 



**Returns:**

{QPointF} - the resulting snapped to common angle point. If no snapping was possible, then `currentPoint` is returned. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qml/QfCoordinateLocator.qml`

