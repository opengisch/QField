

# Class Quick3DGeometryConfiguration



[**ClassList**](annotated.md) **>** [**Quick3DGeometryConfiguration**](classQuick3DGeometryConfiguration.md)



_Lightweight data item used by plugins to inject geometry into the 3D map scene._ [More...](#detailed-description)

* `#include <quick3dgeometryconfiguration.h>`



Inherits the following classes: QQuickItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**Quick3DGeometry::AltitudeClamping**](classQuick3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](classQuick3DGeometryConfiguration.md#property-altitudeclamping-12)  <br>_How the geometry's Z values are combined with the terrain elevation._  |
| property QColor | [**color**](classQuick3DGeometryConfiguration.md#property-color-12)  <br>_Color of the rendered geometry._  |
| property QgsCoordinateReferenceSystem | [**crs**](classQuick3DGeometryConfiguration.md#property-crs-12)  <br>_Coordinate reference system the geometry is expressed in._  |
| property float | [**heightOffset**](classQuick3DGeometryConfiguration.md#property-heightoffset-12)  <br>_Extra vertical offset added on top of the elevation chosen by altitudeClamping._  |
| property float | [**lineWidth**](classQuick3DGeometryConfiguration.md#property-linewidth-12)  <br>_Outline tube thickness in scene units._  |
| property QString | [**wkt**](classQuick3DGeometryConfiguration.md#property-wkt-12)  <br>_WKT representation of the geometry to render._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**altitudeClampingChanged**](classQuick3DGeometryConfiguration.md#signal-altitudeclampingchanged)  <br> |
| signal void | [**colorChanged**](classQuick3DGeometryConfiguration.md#signal-colorchanged)  <br> |
| signal void | [**crsChanged**](classQuick3DGeometryConfiguration.md#signal-crschanged)  <br> |
| signal void | [**heightOffsetChanged**](classQuick3DGeometryConfiguration.md#signal-heightoffsetchanged)  <br> |
| signal void | [**lineWidthChanged**](classQuick3DGeometryConfiguration.md#signal-linewidthchanged)  <br> |
| signal void | [**wktChanged**](classQuick3DGeometryConfiguration.md#signal-wktchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DGeometryConfiguration**](#function-quick3dgeometryconfiguration) (QQuickItem \* parent=nullptr) <br> |
|  [**Quick3DGeometry::AltitudeClamping**](classQuick3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](#function-altitudeclamping-22) () const<br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  void | [**setAltitudeClamping**](#function-setaltitudeclamping) ([**Quick3DGeometry::AltitudeClamping**](classQuick3DGeometry.md#enum-altitudeclamping) clamping) <br> |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setWkt**](#function-setwkt) (const QString & wkt) <br> |
|  QString | [**wkt**](#function-wkt-22) () const<br> |




























## Detailed Description




**Note:**

QML Type: [**Quick3DGeometryConfiguration**](classQuick3DGeometryConfiguration.md) 





    
## Public Properties Documentation




### property altitudeClamping [1/2]

_How the geometry's Z values are combined with the terrain elevation._ 
```C++
Quick3DGeometry::AltitudeClamping Quick3DGeometryConfiguration::altitudeClamping;
```




<hr>



### property color [1/2]

_Color of the rendered geometry._ 
```C++
QColor Quick3DGeometryConfiguration::color;
```




<hr>



### property crs [1/2]

_Coordinate reference system the geometry is expressed in._ 
```C++
QgsCoordinateReferenceSystem Quick3DGeometryConfiguration::crs;
```




<hr>



### property heightOffset [1/2]

_Extra vertical offset added on top of the elevation chosen by altitudeClamping._ 
```C++
float Quick3DGeometryConfiguration::heightOffset;
```




<hr>



### property lineWidth [1/2]

_Outline tube thickness in scene units._ 
```C++
float Quick3DGeometryConfiguration::lineWidth;
```




<hr>



### property wkt [1/2]

_WKT representation of the geometry to render._ 
```C++
QString Quick3DGeometryConfiguration::wkt;
```




<hr>
## Public Signals Documentation




### signal altitudeClampingChanged 

```C++
void Quick3DGeometryConfiguration::altitudeClampingChanged;
```




<hr>



### signal colorChanged 

```C++
void Quick3DGeometryConfiguration::colorChanged;
```




<hr>



### signal crsChanged 

```C++
void Quick3DGeometryConfiguration::crsChanged;
```




<hr>



### signal heightOffsetChanged 

```C++
void Quick3DGeometryConfiguration::heightOffsetChanged;
```




<hr>



### signal lineWidthChanged 

```C++
void Quick3DGeometryConfiguration::lineWidthChanged;
```




<hr>



### signal wktChanged 

```C++
void Quick3DGeometryConfiguration::wktChanged;
```




<hr>
## Public Functions Documentation




### function Quick3DGeometryConfiguration 

```C++
explicit Quick3DGeometryConfiguration::Quick3DGeometryConfiguration (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function altitudeClamping [2/2]

```C++
inline Quick3DGeometry::AltitudeClamping Quick3DGeometryConfiguration::altitudeClamping () const
```




<hr>



### function color [2/2]

```C++
inline QColor Quick3DGeometryConfiguration::color () const
```




<hr>



### function crs [2/2]

```C++
inline QgsCoordinateReferenceSystem Quick3DGeometryConfiguration::crs () const
```




<hr>



### function heightOffset [2/2]

```C++
inline float Quick3DGeometryConfiguration::heightOffset () const
```




<hr>



### function lineWidth [2/2]

```C++
inline float Quick3DGeometryConfiguration::lineWidth () const
```




<hr>



### function setAltitudeClamping 

```C++
void Quick3DGeometryConfiguration::setAltitudeClamping (
    Quick3DGeometry::AltitudeClamping clamping
) 
```




<hr>



### function setColor 

```C++
void Quick3DGeometryConfiguration::setColor (
    const QColor & color
) 
```




<hr>



### function setCrs 

```C++
void Quick3DGeometryConfiguration::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setHeightOffset 

```C++
void Quick3DGeometryConfiguration::setHeightOffset (
    float offset
) 
```




<hr>



### function setLineWidth 

```C++
void Quick3DGeometryConfiguration::setLineWidth (
    float width
) 
```




<hr>



### function setWkt 

```C++
void Quick3DGeometryConfiguration::setWkt (
    const QString & wkt
) 
```




<hr>



### function wkt [2/2]

```C++
inline QString Quick3DGeometryConfiguration::wkt () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/quick3dgeometryconfiguration.h`

