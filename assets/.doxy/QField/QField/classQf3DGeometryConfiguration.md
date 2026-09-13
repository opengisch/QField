

# Class Qf3DGeometryConfiguration



[**ClassList**](annotated.md) **>** [**Qf3DGeometryConfiguration**](classQf3DGeometryConfiguration.md)



_Lightweight data item used by plugins to inject geometry into the 3D map scene._ [More...](#detailed-description)

* `#include <qf3dgeometryconfiguration.h>`



Inherits the following classes: QQuickItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**Qf3DGeometry::AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](classQf3DGeometryConfiguration.md#property-altitudeclamping-12)  <br>_How the geometry's Z values are combined with the terrain elevation._  |
| property QColor | [**color**](classQf3DGeometryConfiguration.md#property-color-12)  <br>_Color of the rendered geometry._  |
| property QgsCoordinateReferenceSystem | [**crs**](classQf3DGeometryConfiguration.md#property-crs-12)  <br>_Coordinate reference system the geometry is expressed in._  |
| property float | [**heightOffset**](classQf3DGeometryConfiguration.md#property-heightoffset-12)  <br>_Extra vertical offset added on top of the elevation chosen by altitudeClamping._  |
| property float | [**lineWidth**](classQf3DGeometryConfiguration.md#property-linewidth-12)  <br>_Outline tube thickness in scene units._  |
| property QString | [**wkt**](classQf3DGeometryConfiguration.md#property-wkt-12)  <br>_WKT representation of the geometry to render._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**altitudeClampingChanged**](classQf3DGeometryConfiguration.md#signal-altitudeclampingchanged)  <br> |
| signal void | [**colorChanged**](classQf3DGeometryConfiguration.md#signal-colorchanged)  <br> |
| signal void | [**crsChanged**](classQf3DGeometryConfiguration.md#signal-crschanged)  <br> |
| signal void | [**heightOffsetChanged**](classQf3DGeometryConfiguration.md#signal-heightoffsetchanged)  <br> |
| signal void | [**lineWidthChanged**](classQf3DGeometryConfiguration.md#signal-linewidthchanged)  <br> |
| signal void | [**wktChanged**](classQf3DGeometryConfiguration.md#signal-wktchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Qf3DGeometryConfiguration**](#function-qf3dgeometryconfiguration) (QQuickItem \* parent=nullptr) <br> |
|  [**Qf3DGeometry::AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](#function-altitudeclamping-22) () const<br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  void | [**setAltitudeClamping**](#function-setaltitudeclamping) ([**Qf3DGeometry::AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) clamping) <br> |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setWkt**](#function-setwkt) (const QString & wkt) <br> |
|  QString | [**wkt**](#function-wkt-22) () const<br> |




























## Detailed Description




**Note:**

QML Type: [**Qf3DGeometryConfiguration**](classQf3DGeometryConfiguration.md) 





    
## Public Properties Documentation




### property altitudeClamping [1/2]

_How the geometry's Z values are combined with the terrain elevation._ 
```C++
Qf3DGeometry::AltitudeClamping Qf3DGeometryConfiguration::altitudeClamping;
```




<hr>



### property color [1/2]

_Color of the rendered geometry._ 
```C++
QColor Qf3DGeometryConfiguration::color;
```




<hr>



### property crs [1/2]

_Coordinate reference system the geometry is expressed in._ 
```C++
QgsCoordinateReferenceSystem Qf3DGeometryConfiguration::crs;
```




<hr>



### property heightOffset [1/2]

_Extra vertical offset added on top of the elevation chosen by altitudeClamping._ 
```C++
float Qf3DGeometryConfiguration::heightOffset;
```




<hr>



### property lineWidth [1/2]

_Outline tube thickness in scene units._ 
```C++
float Qf3DGeometryConfiguration::lineWidth;
```




<hr>



### property wkt [1/2]

_WKT representation of the geometry to render._ 
```C++
QString Qf3DGeometryConfiguration::wkt;
```




<hr>
## Public Signals Documentation




### signal altitudeClampingChanged 

```C++
void Qf3DGeometryConfiguration::altitudeClampingChanged;
```




<hr>



### signal colorChanged 

```C++
void Qf3DGeometryConfiguration::colorChanged;
```




<hr>



### signal crsChanged 

```C++
void Qf3DGeometryConfiguration::crsChanged;
```




<hr>



### signal heightOffsetChanged 

```C++
void Qf3DGeometryConfiguration::heightOffsetChanged;
```




<hr>



### signal lineWidthChanged 

```C++
void Qf3DGeometryConfiguration::lineWidthChanged;
```




<hr>



### signal wktChanged 

```C++
void Qf3DGeometryConfiguration::wktChanged;
```




<hr>
## Public Functions Documentation




### function Qf3DGeometryConfiguration 

```C++
explicit Qf3DGeometryConfiguration::Qf3DGeometryConfiguration (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function altitudeClamping [2/2]

```C++
inline Qf3DGeometry::AltitudeClamping Qf3DGeometryConfiguration::altitudeClamping () const
```




<hr>



### function color [2/2]

```C++
inline QColor Qf3DGeometryConfiguration::color () const
```




<hr>



### function crs [2/2]

```C++
inline QgsCoordinateReferenceSystem Qf3DGeometryConfiguration::crs () const
```




<hr>



### function heightOffset [2/2]

```C++
inline float Qf3DGeometryConfiguration::heightOffset () const
```




<hr>



### function lineWidth [2/2]

```C++
inline float Qf3DGeometryConfiguration::lineWidth () const
```




<hr>



### function setAltitudeClamping 

```C++
void Qf3DGeometryConfiguration::setAltitudeClamping (
    Qf3DGeometry::AltitudeClamping clamping
) 
```




<hr>



### function setColor 

```C++
void Qf3DGeometryConfiguration::setColor (
    const QColor & color
) 
```




<hr>



### function setCrs 

```C++
void Qf3DGeometryConfiguration::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setHeightOffset 

```C++
void Qf3DGeometryConfiguration::setHeightOffset (
    float offset
) 
```




<hr>



### function setLineWidth 

```C++
void Qf3DGeometryConfiguration::setLineWidth (
    float width
) 
```




<hr>



### function setWkt 

```C++
void Qf3DGeometryConfiguration::setWkt (
    const QString & wkt
) 
```




<hr>



### function wkt [2/2]

```C++
inline QString Qf3DGeometryConfiguration::wkt () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qf3dgeometryconfiguration.h`

