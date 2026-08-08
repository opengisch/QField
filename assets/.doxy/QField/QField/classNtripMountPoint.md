

# Class NtripMountPoint



[**ClassList**](annotated.md) **>** [**NtripMountPoint**](classNtripMountPoint.md)



_A class containing information on an NTRIP mount point._ 

* `#include <ntripsourcetablefetcher.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**format**](classNtripMountPoint.md#property-format)  <br> |
| property QString | [**identifier**](classNtripMountPoint.md#property-identifier)  <br> |
| property QString | [**mountPoint**](classNtripMountPoint.md#property-mountpoint)  <br> |
| property QgsPoint | [**point**](classNtripMountPoint.md#property-point)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NtripMountPoint**](#function-ntripmountpoint) (const QString & mountPoint=QString(), const QString & identifier=QString(), const QString & format=QString(), const QgsPointXY & point=QgsPointXY()) <br> |
|  bool | [**operator!=**](#function-operator) (const [**NtripMountPoint**](classNtripMountPoint.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**NtripMountPoint**](classNtripMountPoint.md) & other) const<br> |




























## Public Properties Documentation




### property format 

```C++
QString NtripMountPoint::format;
```




<hr>



### property identifier 

```C++
QString NtripMountPoint::identifier;
```




<hr>



### property mountPoint 

```C++
QString NtripMountPoint::mountPoint;
```




<hr>



### property point 

```C++
QgsPoint NtripMountPoint::point;
```




<hr>
## Public Functions Documentation




### function NtripMountPoint 

```C++
inline explicit NtripMountPoint::NtripMountPoint (
    const QString & mountPoint=QString(),
    const QString & identifier=QString(),
    const QString & format=QString(),
    const QgsPointXY & point=QgsPointXY()
) 
```




<hr>



### function operator!= 

```C++
inline bool NtripMountPoint::operator!= (
    const NtripMountPoint & other
) const
```




<hr>



### function operator== 

```C++
inline bool NtripMountPoint::operator== (
    const NtripMountPoint & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/ntripsourcetablefetcher.h`

