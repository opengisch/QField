

# Class QgsGeometryWrapper



[**ClassList**](annotated.md) **>** [**QgsGeometryWrapper**](classQgsGeometryWrapper.md)



_The_ [_**QgsGeometryWrapper**_](classQgsGeometryWrapper.md) _class wraps QGIS geometry and CRS classes to have their contents available from QML._

* `#include <qgsgeometrywrapper.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsCoordinateReferenceSystem | [**crs**](classQgsGeometryWrapper.md#property-crs-12)  <br> |
| property QgsGeometry | [**qgsGeometry**](classQgsGeometryWrapper.md#property-qgsgeometry-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**crsChanged**](classQgsGeometryWrapper.md#signal-crschanged)  <br> |
| signal void | [**qgsGeometryChanged**](classQgsGeometryWrapper.md#signal-qgsgeometrychanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsGeometryWrapper**](#function-qgsgeometrywrapper-12) (QObject \* parent=nullptr) <br> |
|   | [**QgsGeometryWrapper**](#function-qgsgeometrywrapper-22) (const QgsGeometry & geometry, QgsCoordinateReferenceSystem crs, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br>_Clear the wrapper by setting empty geometry and invalid CRS._  |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  Q\_INVOKABLE QVariantList | [**pointList**](#function-pointlist) () const<br>_Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise._  |
|  QgsGeometry | [**qgsGeometry**](#function-qgsgeometry-22) () const<br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setQgsGeometry**](#function-setqgsgeometry) (const QgsGeometry & qgsGeometry) <br> |




























## Public Properties Documentation




### property crs [1/2]

```C++
QgsCoordinateReferenceSystem QgsGeometryWrapper::crs;
```




<hr>



### property qgsGeometry [1/2]

```C++
QgsGeometry QgsGeometryWrapper::qgsGeometry;
```




<hr>
## Public Signals Documentation




### signal crsChanged 

```C++
void QgsGeometryWrapper::crsChanged;
```




<hr>



### signal qgsGeometryChanged 

```C++
void QgsGeometryWrapper::qgsGeometryChanged;
```




<hr>
## Public Functions Documentation




### function QgsGeometryWrapper [1/2]

```C++
inline explicit QgsGeometryWrapper::QgsGeometryWrapper (
    QObject * parent=nullptr
) 
```




<hr>



### function QgsGeometryWrapper [2/2]

```C++
inline QgsGeometryWrapper::QgsGeometryWrapper (
    const QgsGeometry & geometry,
    QgsCoordinateReferenceSystem crs,
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

_Clear the wrapper by setting empty geometry and invalid CRS._ 
```C++
Q_INVOKABLE void QgsGeometryWrapper::clear () 
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem QgsGeometryWrapper::crs () const
```




<hr>



### function pointList 

_Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise._ 
```C++
Q_INVOKABLE QVariantList QgsGeometryWrapper::pointList () const
```




<hr>



### function qgsGeometry [2/2]

```C++
QgsGeometry QgsGeometryWrapper::qgsGeometry () const
```




<hr>



### function setCrs 

```C++
void QgsGeometryWrapper::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setQgsGeometry 

```C++
void QgsGeometryWrapper::setQgsGeometry (
    const QgsGeometry & qgsGeometry
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsgeometrywrapper.h`

