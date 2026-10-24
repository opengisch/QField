

# Class Qf3DGeometry



[**ClassList**](annotated.md) **>** [**Qf3DGeometry**](classQf3DGeometry.md)



[More...](#detailed-description)

* `#include <qf3dgeometry.h>`



Inherits the following classes: QQuick3DGeometry














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**AltitudeClamping**](#enum-altitudeclamping)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](classQf3DGeometry.md#property-altitudeclamping-12)  <br>_How the geometry's Z values are combined with the terrain elevation._  |
| property QColor | [**color**](classQf3DGeometry.md#property-color-12)  <br>_Per-vertex color applied to the highlight mesh._  |
| property QgsCoordinateReferenceSystem | [**crs**](classQf3DGeometry.md#property-crs-12)  <br>_Coordinate reference system the geometry is expressed in._  |
| property float | [**extrusion**](classQf3DGeometry.md#property-extrusion-12)  <br>_Extrusion in scene units; when &gt; 0, polygons are rendered as volumetric solids with walls and a roof cap._  |
| property bool | [**fillPolygons**](classQf3DGeometry.md#property-fillpolygons-12)  <br>_Whether polygons get a semi-transparent fill in addition to the outline._  |
| property float | [**heightOffset**](classQf3DGeometry.md#property-heightoffset-12)  <br>_Extra vertical offset added on top of the elevation chosen by altitudeClamping._  |
| property float | [**lineWidth**](classQf3DGeometry.md#property-linewidth-12)  <br>_Outline tube thickness in scene units._  |
| property QgsGeometry | [**qgsGeometry**](classQf3DGeometry.md#property-qgsgeometry-12)  <br>_The geometry to highlight._  |
| property [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* | [**terrainProvider**](classQf3DGeometry.md#property-terrainprovider-12)  <br>_The terrain provider used for geo-to-3D coordinate conversion._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**altitudeClampingChanged**](classQf3DGeometry.md#signal-altitudeclampingchanged)  <br> |
| signal void | [**colorChanged**](classQf3DGeometry.md#signal-colorchanged)  <br> |
| signal void | [**crsChanged**](classQf3DGeometry.md#signal-crschanged)  <br> |
| signal void | [**extrusionChanged**](classQf3DGeometry.md#signal-extrusionchanged)  <br> |
| signal void | [**fillPolygonsChanged**](classQf3DGeometry.md#signal-fillpolygonschanged)  <br> |
| signal void | [**heightOffsetChanged**](classQf3DGeometry.md#signal-heightoffsetchanged)  <br> |
| signal void | [**lineWidthChanged**](classQf3DGeometry.md#signal-linewidthchanged)  <br> |
| signal void | [**qgsGeometryChanged**](classQf3DGeometry.md#signal-qgsgeometrychanged)  <br> |
| signal void | [**terrainProviderChanged**](classQf3DGeometry.md#signal-terrainproviderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Qf3DGeometry**](#function-qf3dgeometry) (QQuick3DObject \* parent=nullptr) <br> |
|  [**AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) | [**altitudeClamping**](#function-altitudeclamping-22) () const<br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  float | [**extrusion**](#function-extrusion-22) () const<br> |
|  bool | [**fillPolygons**](#function-fillpolygons-22) () const<br> |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br> |
|  float | [**lineWidth**](#function-linewidth-22) () const<br> |
|  QgsGeometry | [**qgsGeometry**](#function-qgsgeometry-22) () const<br> |
|  void | [**setAltitudeClamping**](#function-setaltitudeclamping) ([**AltitudeClamping**](classQf3DGeometry.md#enum-altitudeclamping) clamping) <br> |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setExtrusion**](#function-setextrusion) (float extrusion) <br> |
|  void | [**setFillPolygons**](#function-setfillpolygons) (bool fill) <br> |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br> |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br> |
|  void | [**setQgsGeometry**](#function-setqgsgeometry) (const QgsGeometry & geometry) <br> |
|  void | [**setTerrainProvider**](#function-setterrainprovider) ([**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* provider) <br> |
|  [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* | [**terrainProvider**](#function-terrainprovider-22) () const<br> |




























## Detailed Description


Generates 3D geometry for an identified or selected feature on top of the terrain mesh.


Points become spheres, lines become extruded tubes with sphere joints, and polygons get both an outline and a semi-transparent triangulated fill. The geometry is reprojected from the source CRS to the map CRS before being draped on the terrain surface.




**Note:**

QML Type: [**Qf3DGeometry**](classQf3DGeometry.md) 





    
## Public Types Documentation




### enum AltitudeClamping 

```C++
enum Qf3DGeometry::AltitudeClamping {
    Ignore,
    ClampToGround,
    Absolute
};
```



How the geometry's Z values interact with the terrain elevation when computing the vertical position of every emitted vertex. 


        

<hr>
## Public Properties Documentation




### property altitudeClamping [1/2]

_How the geometry's Z values are combined with the terrain elevation._ 
```C++
AltitudeClamping Qf3DGeometry::altitudeClamping;
```




<hr>



### property color [1/2]

_Per-vertex color applied to the highlight mesh._ 
```C++
QColor Qf3DGeometry::color;
```




<hr>



### property crs [1/2]

_Coordinate reference system the geometry is expressed in._ 
```C++
QgsCoordinateReferenceSystem Qf3DGeometry::crs;
```




<hr>



### property extrusion [1/2]

_Extrusion in scene units; when &gt; 0, polygons are rendered as volumetric solids with walls and a roof cap._ 
```C++
float Qf3DGeometry::extrusion;
```




<hr>



### property fillPolygons [1/2]

_Whether polygons get a semi-transparent fill in addition to the outline._ 
```C++
bool Qf3DGeometry::fillPolygons;
```




<hr>



### property heightOffset [1/2]

_Extra vertical offset added on top of the elevation chosen by altitudeClamping._ 
```C++
float Qf3DGeometry::heightOffset;
```




<hr>



### property lineWidth [1/2]

_Outline tube thickness in scene units._ 
```C++
float Qf3DGeometry::lineWidth;
```




<hr>



### property qgsGeometry [1/2]

_The geometry to highlight._ 
```C++
QgsGeometry Qf3DGeometry::qgsGeometry;
```




<hr>



### property terrainProvider [1/2]

_The terrain provider used for geo-to-3D coordinate conversion._ 
```C++
Qf3DTerrainProvider * Qf3DGeometry::terrainProvider;
```




<hr>
## Public Signals Documentation




### signal altitudeClampingChanged 

```C++
void Qf3DGeometry::altitudeClampingChanged;
```




<hr>



### signal colorChanged 

```C++
void Qf3DGeometry::colorChanged;
```




<hr>



### signal crsChanged 

```C++
void Qf3DGeometry::crsChanged;
```




<hr>



### signal extrusionChanged 

```C++
void Qf3DGeometry::extrusionChanged;
```




<hr>



### signal fillPolygonsChanged 

```C++
void Qf3DGeometry::fillPolygonsChanged;
```




<hr>



### signal heightOffsetChanged 

```C++
void Qf3DGeometry::heightOffsetChanged;
```




<hr>



### signal lineWidthChanged 

```C++
void Qf3DGeometry::lineWidthChanged;
```




<hr>



### signal qgsGeometryChanged 

```C++
void Qf3DGeometry::qgsGeometryChanged;
```




<hr>



### signal terrainProviderChanged 

```C++
void Qf3DGeometry::terrainProviderChanged;
```




<hr>
## Public Functions Documentation




### function Qf3DGeometry 

```C++
explicit Qf3DGeometry::Qf3DGeometry (
    QQuick3DObject * parent=nullptr
) 
```




<hr>



### function altitudeClamping [2/2]

```C++
inline AltitudeClamping Qf3DGeometry::altitudeClamping () const
```




<hr>



### function color [2/2]

```C++
inline QColor Qf3DGeometry::color () const
```




<hr>



### function crs [2/2]

```C++
inline QgsCoordinateReferenceSystem Qf3DGeometry::crs () const
```




<hr>



### function extrusion [2/2]

```C++
inline float Qf3DGeometry::extrusion () const
```




<hr>



### function fillPolygons [2/2]

```C++
inline bool Qf3DGeometry::fillPolygons () const
```




<hr>



### function heightOffset [2/2]

```C++
inline float Qf3DGeometry::heightOffset () const
```




<hr>



### function lineWidth [2/2]

```C++
inline float Qf3DGeometry::lineWidth () const
```




<hr>



### function qgsGeometry [2/2]

```C++
inline QgsGeometry Qf3DGeometry::qgsGeometry () const
```




<hr>



### function setAltitudeClamping 

```C++
void Qf3DGeometry::setAltitudeClamping (
    AltitudeClamping clamping
) 
```




<hr>



### function setColor 

```C++
void Qf3DGeometry::setColor (
    const QColor & color
) 
```




<hr>



### function setCrs 

```C++
void Qf3DGeometry::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setExtrusion 

```C++
void Qf3DGeometry::setExtrusion (
    float extrusion
) 
```




<hr>



### function setFillPolygons 

```C++
void Qf3DGeometry::setFillPolygons (
    bool fill
) 
```




<hr>



### function setHeightOffset 

```C++
void Qf3DGeometry::setHeightOffset (
    float offset
) 
```




<hr>



### function setLineWidth 

```C++
void Qf3DGeometry::setLineWidth (
    float width
) 
```




<hr>



### function setQgsGeometry 

```C++
void Qf3DGeometry::setQgsGeometry (
    const QgsGeometry & geometry
) 
```




<hr>



### function setTerrainProvider 

```C++
void Qf3DGeometry::setTerrainProvider (
    Qf3DTerrainProvider * provider
) 
```




<hr>



### function terrainProvider [2/2]

```C++
inline Qf3DTerrainProvider * Qf3DGeometry::terrainProvider () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qf3dgeometry.h`

