

# Class LinePolygonShape



[**ClassList**](annotated.md) **>** [**LinePolygonShape**](classLinePolygonShape.md)



_The_ [_**LinePolygonShape**_](classLinePolygonShape.md) _class is used to provide the shape data to draw geometries on the map canvas using the QML Shape item._

* `#include <linepolygonshape.h>`



Inherits the following classes: QQuickItem


Inherited by the following classes: [LinePolygon](classLinePolygon.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classLinePolygonShape.md#property-color-12)  <br> |
| property [**QgsGeometryWrapper**](classQgsGeometryWrapper.md) \* | [**geometry**](classLinePolygonShape.md#property-geometry-12)  <br> |
| property float | [**lineWidth**](classLinePolygonShape.md#property-linewidth-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classLinePolygonShape.md#property-mapsettings-12)  <br> |
| property QList&lt; QPolygonF &gt; | [**polylines**](classLinePolygonShape.md#property-polylines-12)  <br>_List of polylines representing the geometry._  |
| property Qgis::GeometryType | [**polylinesType**](classLinePolygonShape.md#property-polylinestype-12)  <br>_The geometry type associated to the polylines._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classLinePolygonShape.md#signal-colorchanged)  <br> |
| signal void | [**lineWidthChanged**](classLinePolygonShape.md#signal-linewidthchanged)  <br> |
| signal void | [**mapSettingsChanged**](classLinePolygonShape.md#signal-mapsettingschanged)  <br> |
| signal void | [**polylinesChanged**](classLinePolygonShape.md#signal-polylineschanged)  <br>_List of polylines representing the geometry._  |
| signal void | [**polylinesTypeChanged**](classLinePolygonShape.md#signal-polylinestypechanged)  <br>_The geometry type associated to the polylines._  |
| signal void | [**qgsGeometryChanged**](classLinePolygonShape.md#signal-qgsgeometrychanged)  <br> |
| signal void | [**updated**](classLinePolygonShape.md#signal-updated)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LinePolygonShape**](#function-linepolygonshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  [**QgsGeometryWrapper**](classQgsGeometryWrapper.md) \* | [**geometry**](#function-geometry-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  QList&lt; QPolygonF &gt; | [**polylines**](#function-polylines-22) () const<br>_List of polylines representing the geometry._  |
|  Qgis::GeometryType | [**polylinesType**](#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br> |
|  void | [**setGeometry**](#function-setgeometry) ([**QgsGeometryWrapper**](classQgsGeometryWrapper.md) \* geometry) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |




























## Public Properties Documentation




### property color [1/2]

```C++
QColor LinePolygonShape::color;
```




<hr>



### property geometry [1/2]

```C++
QgsGeometryWrapper * LinePolygonShape::geometry;
```




<hr>



### property lineWidth [1/2]

```C++
float LinePolygonShape::lineWidth;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * LinePolygonShape::mapSettings;
```




<hr>



### property polylines [1/2]

_List of polylines representing the geometry._ 
```C++
QList<QPolygonF> LinePolygonShape::polylines;
```




<hr>



### property polylinesType [1/2]

_The geometry type associated to the polylines._ 
```C++
Qgis::GeometryType LinePolygonShape::polylinesType;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

```C++
void LinePolygonShape::colorChanged;
```




<hr>



### signal lineWidthChanged 

```C++
void LinePolygonShape::lineWidthChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void LinePolygonShape::mapSettingsChanged;
```




<hr>



### signal polylinesChanged 

_List of polylines representing the geometry._ 
```C++
void LinePolygonShape::polylinesChanged;
```




<hr>



### signal polylinesTypeChanged 

_The geometry type associated to the polylines._ 
```C++
void LinePolygonShape::polylinesTypeChanged;
```




<hr>



### signal qgsGeometryChanged 

```C++
void LinePolygonShape::qgsGeometryChanged;
```




<hr>



### signal updated 

```C++
void LinePolygonShape::updated;
```




<hr>
## Public Functions Documentation




### function LinePolygonShape 

```C++
explicit LinePolygonShape::LinePolygonShape (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function color [2/2]

```C++
QColor LinePolygonShape::color () const
```




<hr>



### function geometry [2/2]

```C++
QgsGeometryWrapper * LinePolygonShape::geometry () const
```




<hr>



### function lineWidth [2/2]

```C++
float LinePolygonShape::lineWidth () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * LinePolygonShape::mapSettings () const
```




<hr>



### function polylines [2/2]

_List of polylines representing the geometry._ 
```C++
inline QList< QPolygonF > LinePolygonShape::polylines () const
```




<hr>



### function polylinesType [2/2]

_The geometry type associated to the polylines._ 
```C++
inline Qgis::GeometryType LinePolygonShape::polylinesType () const
```




<hr>



### function setColor 

```C++
void LinePolygonShape::setColor (
    const QColor & color
) 
```




<hr>



### function setGeometry 

```C++
void LinePolygonShape::setGeometry (
    QgsGeometryWrapper * geometry
) 
```




<hr>



### function setLineWidth 

```C++
void LinePolygonShape::setLineWidth (
    float width
) 
```




<hr>



### function setMapSettings 

```C++
void LinePolygonShape::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/linepolygonshape.h`

