

# Class Quick3DGeometryConfiguration



[**ClassList**](annotated.md) **>** [**Quick3DGeometryConfiguration**](classQuick3DGeometryConfiguration.md)



_Lightweight data item used by plugins to inject geometry into the 3D map scene._ [More...](#detailed-description)

* `#include <quick3dgeometryconfiguration.h>`



Inherits the following classes: QQuickItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsCoordinateReferenceSystem | [**crs**](classQuick3DGeometryConfiguration.md#property-crs-12)  <br>_Coordinate reference system the geometry is expressed in._  |
| property QColor | [**lineColor**](classQuick3DGeometryConfiguration.md#property-linecolor-12)  <br>[_**Color**_](classColor.md) _of the rendered geometry._ |
| property float | [**lineWidth**](classQuick3DGeometryConfiguration.md#property-linewidth-12)  <br>_Outline tube thickness in scene units._  |
| property QString | [**wkt**](classQuick3DGeometryConfiguration.md#property-wkt-12)  <br>_WKT representation of the geometry to render._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**crsChanged**](classQuick3DGeometryConfiguration.md#signal-crschanged)  <br> |
| signal void | [**lineColorChanged**](classQuick3DGeometryConfiguration.md#signal-linecolorchanged)  <br> |
| signal void | [**lineWidthChanged**](classQuick3DGeometryConfiguration.md#signal-linewidthchanged)  <br> |
| signal void | [**wktChanged**](classQuick3DGeometryConfiguration.md#signal-wktchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DGeometryConfiguration**](#function-quick3dgeometryconfiguration) (QQuickItem \* parent=nullptr) <br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  QColor | [**lineColor**](#function-linecolor-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setLineColor**](#function-setlinecolor) (const QColor & color) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setWkt**](#function-setwkt) (const QString & wkt) <br> |
|  QString | [**wkt**](#function-wkt-22) () const<br> |




























## Detailed Description




**Note:**

QML Type: [**Quick3DGeometryConfiguration**](classQuick3DGeometryConfiguration.md) 





    
## Public Properties Documentation




### property crs [1/2]

_Coordinate reference system the geometry is expressed in._ 
```C++
QgsCoordinateReferenceSystem Quick3DGeometryConfiguration::crs;
```




<hr>



### property lineColor [1/2]

[_**Color**_](classColor.md) _of the rendered geometry._
```C++
QColor Quick3DGeometryConfiguration::lineColor;
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




### signal crsChanged 

```C++
void Quick3DGeometryConfiguration::crsChanged;
```




<hr>



### signal lineColorChanged 

```C++
void Quick3DGeometryConfiguration::lineColorChanged;
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



### function crs [2/2]

```C++
inline QgsCoordinateReferenceSystem Quick3DGeometryConfiguration::crs () const
```




<hr>



### function lineColor [2/2]

```C++
inline QColor Quick3DGeometryConfiguration::lineColor () const
```




<hr>



### function lineWidth [2/2]

```C++
inline float Quick3DGeometryConfiguration::lineWidth () const
```




<hr>



### function setCrs 

```C++
void Quick3DGeometryConfiguration::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setLineColor 

```C++
void Quick3DGeometryConfiguration::setLineColor (
    const QColor & color
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
The documentation for this class was generated from the following file `src/core/3d/quick3dgeometryconfiguration.h`

