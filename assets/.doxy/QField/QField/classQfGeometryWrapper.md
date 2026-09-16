

# Class QfGeometryWrapper



[**ClassList**](annotated.md) **>** [**QfGeometryWrapper**](classQfGeometryWrapper.md)



_The_ [_**QfGeometryWrapper**_](classQfGeometryWrapper.md) _class wraps QGIS geometry and CRS classes to have their contents available from QML._

* `#include <qfgeometrywrapper.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsCoordinateReferenceSystem | [**crs**](classQfGeometryWrapper.md#property-crs-12)  <br> |
| property QgsGeometry | [**qgsGeometry**](classQfGeometryWrapper.md#property-qgsgeometry-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**crsChanged**](classQfGeometryWrapper.md#signal-crschanged)  <br> |
| signal void | [**qgsGeometryChanged**](classQfGeometryWrapper.md#signal-qgsgeometrychanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGeometryWrapper**](#function-qfgeometrywrapper-12) (QObject \* parent=nullptr) <br> |
|   | [**QfGeometryWrapper**](#function-qfgeometrywrapper-22) (const QgsGeometry & geometry, QgsCoordinateReferenceSystem crs, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br>_Clear the wrapper by setting empty geometry and invalid CRS._  |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  Q\_INVOKABLE QVariantList | [**pointList**](#function-pointlist) () const<br>_Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise._  |
|  QgsGeometry | [**qgsGeometry**](#function-qgsgeometry-22) () const<br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setQgsGeometry**](#function-setqgsgeometry) (const QgsGeometry & qgsGeometry) <br> |




























## Public Properties Documentation




### property crs [1/2]

```C++
QgsCoordinateReferenceSystem QfGeometryWrapper::crs;
```




<hr>



### property qgsGeometry [1/2]

```C++
QgsGeometry QfGeometryWrapper::qgsGeometry;
```




<hr>
## Public Signals Documentation




### signal crsChanged 

```C++
void QfGeometryWrapper::crsChanged;
```




<hr>



### signal qgsGeometryChanged 

```C++
void QfGeometryWrapper::qgsGeometryChanged;
```




<hr>
## Public Functions Documentation




### function QfGeometryWrapper [1/2]

```C++
inline explicit QfGeometryWrapper::QfGeometryWrapper (
    QObject * parent=nullptr
) 
```




<hr>



### function QfGeometryWrapper [2/2]

```C++
inline QfGeometryWrapper::QfGeometryWrapper (
    const QgsGeometry & geometry,
    QgsCoordinateReferenceSystem crs,
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

_Clear the wrapper by setting empty geometry and invalid CRS._ 
```C++
Q_INVOKABLE void QfGeometryWrapper::clear () 
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem QfGeometryWrapper::crs () const
```




<hr>



### function pointList 

_Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise._ 
```C++
Q_INVOKABLE QVariantList QfGeometryWrapper::pointList () const
```




<hr>



### function qgsGeometry [2/2]

```C++
QgsGeometry QfGeometryWrapper::qgsGeometry () const
```




<hr>



### function setCrs 

```C++
void QfGeometryWrapper::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setQgsGeometry 

```C++
void QfGeometryWrapper::setQgsGeometry (
    const QgsGeometry & qgsGeometry
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfgeometrywrapper.h`

