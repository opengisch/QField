

# Class QfDistanceArea



[**ClassList**](annotated.md) **>** [**QfDistanceArea**](classQfDistanceArea.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property qreal | [**area**](classQfDistanceArea.md#property-area-12)  <br> |
| property Qgis::AreaUnit | [**areaUnits**](classQfDistanceArea.md#property-areaunits-12)  <br> |
| property bool | [**areaValid**](classQfDistanceArea.md#property-areavalid-12)  <br> |
| property qreal | [**azimuth**](classQfDistanceArea.md#property-azimuth-12)  <br> |
| property QgsCoordinateReferenceSystem | [**crs**](classQfDistanceArea.md#property-crs-12)  <br> |
| property qreal | [**length**](classQfDistanceArea.md#property-length-12)  <br> |
| property Qgis::DistanceUnit | [**lengthUnits**](classQfDistanceArea.md#property-lengthunits-12)  <br> |
| property bool | [**lengthValid**](classQfDistanceArea.md#property-lengthvalid-12)  <br> |
| property qreal | [**perimeter**](classQfDistanceArea.md#property-perimeter-12)  <br> |
| property bool | [**perimeterValid**](classQfDistanceArea.md#property-perimetervalid-12)  <br> |
| property QgsProject \* | [**project**](classQfDistanceArea.md#property-project-12)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](classQfDistanceArea.md#property-rubberbandmodel-12)  <br> |
| property qreal | [**segmentLength**](classQfDistanceArea.md#property-segmentlength-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**areaChanged**](classQfDistanceArea.md#signal-areachanged)  <br> |
| signal void | [**areaUnitsChanged**](classQfDistanceArea.md#signal-areaunitschanged)  <br> |
| signal void | [**azimuthChanged**](classQfDistanceArea.md#signal-azimuthchanged)  <br> |
| signal void | [**crsChanged**](classQfDistanceArea.md#signal-crschanged)  <br> |
| signal void | [**lengthChanged**](classQfDistanceArea.md#signal-lengthchanged)  <br> |
| signal void | [**lengthUnitsChanged**](classQfDistanceArea.md#signal-lengthunitschanged)  <br> |
| signal void | [**perimeterChanged**](classQfDistanceArea.md#signal-perimeterchanged)  <br> |
| signal void | [**projectChanged**](classQfDistanceArea.md#signal-projectchanged)  <br> |
| signal void | [**rubberbandModelChanged**](classQfDistanceArea.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**segmentLengthChanged**](classQfDistanceArea.md#signal-segmentlengthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDistanceArea**](#function-qfdistancearea) (QObject \* parent=nullptr) <br> |
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
|  [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br> |
|  qreal | [**segmentLength**](#function-segmentlength-22) () const<br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel) <br> |




























## Public Properties Documentation




### property area [1/2]

```C++
qreal QfDistanceArea::area;
```




<hr>



### property areaUnits [1/2]

```C++
Qgis::AreaUnit QfDistanceArea::areaUnits;
```




<hr>



### property areaValid [1/2]

```C++
bool QfDistanceArea::areaValid;
```




<hr>



### property azimuth [1/2]

```C++
qreal QfDistanceArea::azimuth;
```



Returns the Cartesian azimuth (in degrees) between the second to last point and last point of the rubber band model (clockwise in degree, starting from north) 


        

<hr>



### property crs [1/2]

```C++
QgsCoordinateReferenceSystem QfDistanceArea::crs;
```




<hr>



### property length [1/2]

```C++
qreal QfDistanceArea::length;
```




<hr>



### property lengthUnits [1/2]

```C++
Qgis::DistanceUnit QfDistanceArea::lengthUnits;
```




<hr>



### property lengthValid [1/2]

```C++
bool QfDistanceArea::lengthValid;
```




<hr>



### property perimeter [1/2]

```C++
qreal QfDistanceArea::perimeter;
```




<hr>



### property perimeterValid [1/2]

```C++
bool QfDistanceArea::perimeterValid;
```




<hr>



### property project [1/2]

```C++
QgsProject * QfDistanceArea::project;
```




<hr>



### property rubberbandModel [1/2]

```C++
QfRubberbandModel * QfDistanceArea::rubberbandModel;
```




<hr>



### property segmentLength [1/2]

```C++
qreal QfDistanceArea::segmentLength;
```



Returns the length of the last segment 


        

<hr>
## Public Signals Documentation




### signal areaChanged 

```C++
void QfDistanceArea::areaChanged;
```




<hr>



### signal areaUnitsChanged 

```C++
void QfDistanceArea::areaUnitsChanged;
```




<hr>



### signal azimuthChanged 

```C++
void QfDistanceArea::azimuthChanged;
```




<hr>



### signal crsChanged 

```C++
void QfDistanceArea::crsChanged;
```




<hr>



### signal lengthChanged 

```C++
void QfDistanceArea::lengthChanged;
```




<hr>



### signal lengthUnitsChanged 

```C++
void QfDistanceArea::lengthUnitsChanged;
```




<hr>



### signal perimeterChanged 

```C++
void QfDistanceArea::perimeterChanged;
```




<hr>



### signal projectChanged 

```C++
void QfDistanceArea::projectChanged;
```




<hr>



### signal rubberbandModelChanged 

```C++
void QfDistanceArea::rubberbandModelChanged;
```




<hr>



### signal segmentLengthChanged 

```C++
void QfDistanceArea::segmentLengthChanged;
```




<hr>
## Public Functions Documentation




### function QfDistanceArea 

```C++
explicit QfDistanceArea::QfDistanceArea (
    QObject * parent=nullptr
) 
```




<hr>



### function area [2/2]

```C++
qreal QfDistanceArea::area () const
```




<hr>



### function areaUnits [2/2]

```C++
Qgis::AreaUnit QfDistanceArea::areaUnits () const
```




<hr>



### function areaValid [2/2]

```C++
bool QfDistanceArea::areaValid () const
```




<hr>



### function azimuth [2/2]

```C++
qreal QfDistanceArea::azimuth () const
```




<hr>



### function convertAreaMeansurement 

```C++
Q_INVOKABLE double QfDistanceArea::convertAreaMeansurement (
    double area,
    Qgis::AreaUnit toUnits
) const
```




<hr>



### function convertLengthMeansurement 

```C++
Q_INVOKABLE double QfDistanceArea::convertLengthMeansurement (
    double length,
    Qgis::DistanceUnit toUnits
) const
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem QfDistanceArea::crs () const
```




<hr>



### function length [2/2]

```C++
qreal QfDistanceArea::length () const
```




<hr>



### function lengthUnits [2/2]

```C++
Qgis::DistanceUnit QfDistanceArea::lengthUnits () const
```




<hr>



### function lengthValid [2/2]

```C++
bool QfDistanceArea::lengthValid () const
```




<hr>



### function perimeter [2/2]

```C++
qreal QfDistanceArea::perimeter () const
```




<hr>



### function perimeterValid [2/2]

```C++
bool QfDistanceArea::perimeterValid () const
```




<hr>



### function project [2/2]

```C++
QgsProject * QfDistanceArea::project () const
```




<hr>



### function rubberbandModel [2/2]

```C++
QfRubberbandModel * QfDistanceArea::rubberbandModel () const
```




<hr>



### function segmentLength [2/2]

```C++
qreal QfDistanceArea::segmentLength () const
```




<hr>



### function setCrs 

```C++
void QfDistanceArea::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setProject 

```C++
void QfDistanceArea::setProject (
    QgsProject * project
) 
```




<hr>



### function setRubberbandModel 

```C++
void QfDistanceArea::setRubberbandModel (
    QfRubberbandModel * rubberbandModel
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfdistancearea.h`

