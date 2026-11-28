

# Class QfLinePolygonShape



[**ClassList**](annotated.md) **>** [**QfLinePolygonShape**](classQfLinePolygonShape.md)



_The_ [_**QfLinePolygonShape**_](classQfLinePolygonShape.md) _class is used to provide the shape data to draw geometries on the map canvas using the QML Shape item._

* `#include <qflinepolygonshape.h>`



Inherits the following classes: QQuickItem


Inherited by the following classes: [QfLinePolygon](classQfLinePolygon.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfLinePolygonShape.md#property-color-12)  <br> |
| property [**QfGeometryWrapper**](classQfGeometryWrapper.md) \* | [**geometry**](classQfLinePolygonShape.md#property-geometry-12)  <br> |
| property float | [**lineWidth**](classQfLinePolygonShape.md#property-linewidth-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfLinePolygonShape.md#property-mapsettings-12)  <br> |
| property QList&lt; QPolygonF &gt; | [**polylines**](classQfLinePolygonShape.md#property-polylines-12)  <br>_List of polylines representing the geometry._  |
| property Qgis::GeometryType | [**polylinesType**](classQfLinePolygonShape.md#property-polylinestype-12)  <br>_The geometry type associated to the polylines._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classQfLinePolygonShape.md#signal-colorchanged)  <br> |
| signal void | [**geometryChanged**](classQfLinePolygonShape.md#signal-geometrychanged)  <br> |
| signal void | [**lineWidthChanged**](classQfLinePolygonShape.md#signal-linewidthchanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfLinePolygonShape.md#signal-mapsettingschanged)  <br> |
| signal void | [**polylinesChanged**](classQfLinePolygonShape.md#signal-polylineschanged)  <br>_List of polylines representing the geometry._  |
| signal void | [**polylinesTypeChanged**](classQfLinePolygonShape.md#signal-polylinestypechanged)  <br>_The geometry type associated to the polylines._  |
| signal void | [**updated**](classQfLinePolygonShape.md#signal-updated)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLinePolygonShape**](#function-qflinepolygonshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  [**QfGeometryWrapper**](classQfGeometryWrapper.md) \* | [**geometry**](#function-geometry-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  QList&lt; QPolygonF &gt; | [**polylines**](#function-polylines-22) () const<br>_List of polylines representing the geometry._  |
|  Qgis::GeometryType | [**polylinesType**](#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br> |
|  void | [**setGeometry**](#function-setgeometry) ([**QfGeometryWrapper**](classQfGeometryWrapper.md) \* geometry) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |




























## Public Properties Documentation




### property color [1/2]

```C++
QColor QfLinePolygonShape::color;
```




<hr>



### property geometry [1/2]

```C++
QfGeometryWrapper * QfLinePolygonShape::geometry;
```




<hr>



### property lineWidth [1/2]

```C++
float QfLinePolygonShape::lineWidth;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfLinePolygonShape::mapSettings;
```




<hr>



### property polylines [1/2]

_List of polylines representing the geometry._ 
```C++
QList<QPolygonF> QfLinePolygonShape::polylines;
```




<hr>



### property polylinesType [1/2]

_The geometry type associated to the polylines._ 
```C++
Qgis::GeometryType QfLinePolygonShape::polylinesType;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

```C++
void QfLinePolygonShape::colorChanged;
```




<hr>



### signal geometryChanged 

```C++
void QfLinePolygonShape::geometryChanged;
```




<hr>



### signal lineWidthChanged 

```C++
void QfLinePolygonShape::lineWidthChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfLinePolygonShape::mapSettingsChanged;
```




<hr>



### signal polylinesChanged 

_List of polylines representing the geometry._ 
```C++
void QfLinePolygonShape::polylinesChanged;
```




<hr>



### signal polylinesTypeChanged 

_The geometry type associated to the polylines._ 
```C++
void QfLinePolygonShape::polylinesTypeChanged;
```




<hr>



### signal updated 

```C++
void QfLinePolygonShape::updated;
```




<hr>
## Public Functions Documentation




### function QfLinePolygonShape 

```C++
explicit QfLinePolygonShape::QfLinePolygonShape (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function color [2/2]

```C++
QColor QfLinePolygonShape::color () const
```




<hr>



### function geometry [2/2]

```C++
QfGeometryWrapper * QfLinePolygonShape::geometry () const
```




<hr>



### function lineWidth [2/2]

```C++
float QfLinePolygonShape::lineWidth () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfLinePolygonShape::mapSettings () const
```




<hr>



### function polylines [2/2]

_List of polylines representing the geometry._ 
```C++
inline QList< QPolygonF > QfLinePolygonShape::polylines () const
```




<hr>



### function polylinesType [2/2]

_The geometry type associated to the polylines._ 
```C++
inline Qgis::GeometryType QfLinePolygonShape::polylinesType () const
```




<hr>



### function setColor 

```C++
void QfLinePolygonShape::setColor (
    const QColor & color
) 
```




<hr>



### function setGeometry 

```C++
void QfLinePolygonShape::setGeometry (
    QfGeometryWrapper * geometry
) 
```




<hr>



### function setLineWidth 

```C++
void QfLinePolygonShape::setLineWidth (
    float width
) 
```




<hr>



### function setMapSettings 

```C++
void QfLinePolygonShape::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qflinepolygonshape.h`

