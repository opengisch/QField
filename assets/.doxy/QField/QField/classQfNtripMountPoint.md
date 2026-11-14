

# Class QfNtripMountPoint



[**ClassList**](annotated.md) **>** [**QfNtripMountPoint**](classQfNtripMountPoint.md)



_A class containing information on an NTRIP mount point._ 

* `#include <qfntripsourcetablefetcher.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**format**](classQfNtripMountPoint.md#property-format)  <br> |
| property QString | [**identifier**](classQfNtripMountPoint.md#property-identifier)  <br> |
| property QString | [**mountPoint**](classQfNtripMountPoint.md#property-mountpoint)  <br> |
| property QgsPoint | [**point**](classQfNtripMountPoint.md#property-point)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNtripMountPoint**](#function-qfntripmountpoint) (const QString & mountPoint=QString(), const QString & identifier=QString(), const QString & format=QString(), const QgsPointXY & point=QgsPointXY()) <br> |
|  bool | [**operator!=**](#function-operator) (const [**QfNtripMountPoint**](classQfNtripMountPoint.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfNtripMountPoint**](classQfNtripMountPoint.md) & other) const<br> |




























## Public Properties Documentation




### property format 

```C++
QString QfNtripMountPoint::format;
```




<hr>



### property identifier 

```C++
QString QfNtripMountPoint::identifier;
```




<hr>



### property mountPoint 

```C++
QString QfNtripMountPoint::mountPoint;
```




<hr>



### property point 

```C++
QgsPoint QfNtripMountPoint::point;
```




<hr>
## Public Functions Documentation




### function QfNtripMountPoint 

```C++
inline explicit QfNtripMountPoint::QfNtripMountPoint (
    const QString & mountPoint=QString(),
    const QString & identifier=QString(),
    const QString & format=QString(),
    const QgsPointXY & point=QgsPointXY()
) 
```




<hr>



### function operator!= 

```C++
inline bool QfNtripMountPoint::operator!= (
    const QfNtripMountPoint & other
) const
```




<hr>



### function operator== 

```C++
inline bool QfNtripMountPoint::operator== (
    const QfNtripMountPoint & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfntripsourcetablefetcher.h`

