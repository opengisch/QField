

# Class DistanceArea



[**ClassList**](annotated.md) **>** [**DistanceArea**](classDistanceArea.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property qreal | [**area**](classDistanceArea.md#property-area-12)  <br> |
| property Qgis::AreaUnit | [**areaUnits**](classDistanceArea.md#property-areaunits-12)  <br> |
| property bool | [**areaValid**](classDistanceArea.md#property-areavalid-12)  <br> |
| property qreal | [**azimuth**](classDistanceArea.md#property-azimuth-12)  <br> |
| property QgsCoordinateReferenceSystem | [**crs**](classDistanceArea.md#property-crs-12)  <br> |
| property qreal | [**length**](classDistanceArea.md#property-length-12)  <br> |
| property Qgis::DistanceUnit | [**lengthUnits**](classDistanceArea.md#property-lengthunits-12)  <br> |
| property bool | [**lengthValid**](classDistanceArea.md#property-lengthvalid-12)  <br> |
| property qreal | [**perimeter**](classDistanceArea.md#property-perimeter-12)  <br> |
| property bool | [**perimeterValid**](classDistanceArea.md#property-perimetervalid-12)  <br> |
| property QgsProject \* | [**project**](classDistanceArea.md#property-project-12)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](classDistanceArea.md#property-rubberbandmodel-12)  <br> |
| property qreal | [**segmentLength**](classDistanceArea.md#property-segmentlength-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**areaChanged**](classDistanceArea.md#signal-areachanged)  <br> |
| signal void | [**areaUnitsChanged**](classDistanceArea.md#signal-areaunitschanged)  <br> |
| signal void | [**azimuthChanged**](classDistanceArea.md#signal-azimuthchanged)  <br> |
| signal void | [**crsChanged**](classDistanceArea.md#signal-crschanged)  <br> |
| signal void | [**lengthChanged**](classDistanceArea.md#signal-lengthchanged)  <br> |
| signal void | [**lengthUnitsChanged**](classDistanceArea.md#signal-lengthunitschanged)  <br> |
| signal void | [**perimeterChanged**](classDistanceArea.md#signal-perimeterchanged)  <br> |
| signal void | [**projectChanged**](classDistanceArea.md#signal-projectchanged)  <br> |
| signal void | [**rubberbandModelChanged**](classDistanceArea.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**segmentLengthChanged**](classDistanceArea.md#signal-segmentlengthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DistanceArea**](#function-distancearea) (QObject \* parent=nullptr) <br> |
|  qreal | [**area**](#function-area-22) () const<br> |
|  Qgis::AreaUnit | [**areaUnits**](#function-areaunits-22) () const<br> |
|  bool | [**areaValid**](#function-areavalid-22) () const<br> |
|  qreal | [**azimuth**](#function-azimuth-22) () const<br> |
|  Q\_INVOKABLE double | [**convertAreaMeansurement**](#function-convertareameansurement) (double area, Qgis::AreaUnit toUnits) const<br> |
|  Q\_INVOKABLE double | [**convertLengthMeansurement**](#function-convertlengthmeansurement) (double length, Qgis::DistanceUnit toUnits) const<br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  qreal | [**length**](#function-length-22) () const<br> |
|  Qgis::DistanceUnit | [**lengthUnits**](#function-lengthunits-22) () const<br> |
|  bool | [**lengthValid**](#function-lengthvalid-22) () const<br> |
|  qreal | [**perimeter**](#function-perimeter-22) () const<br> |
|  bool | [**perimeterValid**](#function-perimetervalid-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br> |
|  qreal | [**segmentLength**](#function-segmentlength-22) () const<br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel) <br> |




























## Public Properties Documentation




### property area [1/2]

```C++
qreal DistanceArea::area;
```




<hr>



### property areaUnits [1/2]

```C++
Qgis::AreaUnit DistanceArea::areaUnits;
```




<hr>



### property areaValid [1/2]

```C++
bool DistanceArea::areaValid;
```




<hr>



### property azimuth [1/2]

```C++
qreal DistanceArea::azimuth;
```



Returns the Cartesian azimuth (in degrees) between the second to last point and last point of the rubber band model (clockwise in degree, starting from north) 


        

<hr>



### property crs [1/2]

```C++
QgsCoordinateReferenceSystem DistanceArea::crs;
```




<hr>



### property length [1/2]

```C++
qreal DistanceArea::length;
```




<hr>



### property lengthUnits [1/2]

```C++
Qgis::DistanceUnit DistanceArea::lengthUnits;
```




<hr>



### property lengthValid [1/2]

```C++
bool DistanceArea::lengthValid;
```




<hr>



### property perimeter [1/2]

```C++
qreal DistanceArea::perimeter;
```




<hr>



### property perimeterValid [1/2]

```C++
bool DistanceArea::perimeterValid;
```




<hr>



### property project [1/2]

```C++
QgsProject * DistanceArea::project;
```




<hr>



### property rubberbandModel [1/2]

```C++
RubberbandModel * DistanceArea::rubberbandModel;
```




<hr>



### property segmentLength [1/2]

```C++
qreal DistanceArea::segmentLength;
```



Returns the length of the last segment 


        

<hr>
## Public Signals Documentation




### signal areaChanged 

```C++
void DistanceArea::areaChanged;
```




<hr>



### signal areaUnitsChanged 

```C++
void DistanceArea::areaUnitsChanged;
```




<hr>



### signal azimuthChanged 

```C++
void DistanceArea::azimuthChanged;
```




<hr>



### signal crsChanged 

```C++
void DistanceArea::crsChanged;
```




<hr>



### signal lengthChanged 

```C++
void DistanceArea::lengthChanged;
```




<hr>



### signal lengthUnitsChanged 

```C++
void DistanceArea::lengthUnitsChanged;
```




<hr>



### signal perimeterChanged 

```C++
void DistanceArea::perimeterChanged;
```




<hr>



### signal projectChanged 

```C++
void DistanceArea::projectChanged;
```




<hr>



### signal rubberbandModelChanged 

```C++
void DistanceArea::rubberbandModelChanged;
```




<hr>



### signal segmentLengthChanged 

```C++
void DistanceArea::segmentLengthChanged;
```




<hr>
## Public Functions Documentation




### function DistanceArea 

```C++
explicit DistanceArea::DistanceArea (
    QObject * parent=nullptr
) 
```




<hr>



### function area [2/2]

```C++
qreal DistanceArea::area () const
```




<hr>



### function areaUnits [2/2]

```C++
Qgis::AreaUnit DistanceArea::areaUnits () const
```




<hr>



### function areaValid [2/2]

```C++
bool DistanceArea::areaValid () const
```




<hr>



### function azimuth [2/2]

```C++
qreal DistanceArea::azimuth () const
```




<hr>



### function convertAreaMeansurement 

```C++
Q_INVOKABLE double DistanceArea::convertAreaMeansurement (
    double area,
    Qgis::AreaUnit toUnits
) const
```




<hr>



### function convertLengthMeansurement 

```C++
Q_INVOKABLE double DistanceArea::convertLengthMeansurement (
    double length,
    Qgis::DistanceUnit toUnits
) const
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem DistanceArea::crs () const
```




<hr>



### function length [2/2]

```C++
qreal DistanceArea::length () const
```




<hr>



### function lengthUnits [2/2]

```C++
Qgis::DistanceUnit DistanceArea::lengthUnits () const
```




<hr>



### function lengthValid [2/2]

```C++
bool DistanceArea::lengthValid () const
```




<hr>



### function perimeter [2/2]

```C++
qreal DistanceArea::perimeter () const
```




<hr>



### function perimeterValid [2/2]

```C++
bool DistanceArea::perimeterValid () const
```




<hr>



### function project [2/2]

```C++
QgsProject * DistanceArea::project () const
```




<hr>



### function rubberbandModel [2/2]

```C++
RubberbandModel * DistanceArea::rubberbandModel () const
```




<hr>



### function segmentLength [2/2]

```C++
qreal DistanceArea::segmentLength () const
```




<hr>



### function setCrs 

```C++
void DistanceArea::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setProject 

```C++
void DistanceArea::setProject (
    QgsProject * project
) 
```




<hr>



### function setRubberbandModel 

```C++
void DistanceArea::setRubberbandModel (
    RubberbandModel * rubberbandModel
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/distancearea.h`

