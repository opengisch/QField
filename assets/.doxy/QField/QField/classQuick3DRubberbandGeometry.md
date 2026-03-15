

# Class Quick3DRubberbandGeometry



[**ClassList**](annotated.md) **>** [**Quick3DRubberbandGeometry**](classQuick3DRubberbandGeometry.md)



[More...](#detailed-description)

* `#include <quick3drubberbandgeometry.h>`



Inherits the following classes: QQuick3DGeometry


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQuick3DRubberbandGeometry.md#property-color-12)  <br>_Per-vertex color applied to both tubes and joint spheres._  |
| property float | [**heightOffset**](classQuick3DRubberbandGeometry.md#property-heightoffset-12)  <br>_Extra vertical offset above the terrain surface._  |
| property float | [**radius**](classQuick3DRubberbandGeometry.md#property-radius-12)  <br>_Tube thickness in scene units._  |
| property QML\_ELEMENT [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](classQuick3DRubberbandGeometry.md#property-rubberbandmodel-12)  <br>_The rubberband model providing the vertex coordinates._  |
| property int | [**segments**](classQuick3DRubberbandGeometry.md#property-segments-12)  <br>_Number of sides in the tube cross-section._  |
| property [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* | [**terrainProvider**](classQuick3DRubberbandGeometry.md#property-terrainprovider-12)  <br>_The terrain provider used for geo-to-3D coordinate conversion._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classQuick3DRubberbandGeometry.md#signal-colorchanged)  <br> |
| signal void | [**heightOffsetChanged**](classQuick3DRubberbandGeometry.md#signal-heightoffsetchanged)  <br> |
| signal void | [**radiusChanged**](classQuick3DRubberbandGeometry.md#signal-radiuschanged)  <br> |
| signal void | [**rubberbandModelChanged**](classQuick3DRubberbandGeometry.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**segmentsChanged**](classQuick3DRubberbandGeometry.md#signal-segmentschanged)  <br> |
| signal void | [**terrainProviderChanged**](classQuick3DRubberbandGeometry.md#signal-terrainproviderchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DRubberbandGeometry**](#function-quick3drubberbandgeometry) (QQuick3DObject \* parent=nullptr) <br> |
|  QColor | [**color**](#function-color-22) () const<br>_Returns the color used for both tubes and sphere joints._  |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br>_Returns the height offset above the terrain surface._  |
|  float | [**radius**](#function-radius-22) () const<br>_Returns the tube radius in 3D scene units._  |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br>_Returns the rubberband model providing vertex coordinates._  |
|  int | [**segments**](#function-segments-22) () const<br>_Returns the number of segments around the tube circumference._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br>_Sets the rubberband color._  |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br>_Sets the height offset._  |
|  void | [**setRadius**](#function-setradius) (float radius) <br>_Sets the tube radius._  |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**RubberbandModel**](classRubberbandModel.md) \* model) <br>_Sets the rubberband model._  |
|  void | [**setSegments**](#function-setsegments) (int segments) <br>_Sets the segment count._  |
|  void | [**setTerrainProvider**](#function-setterrainprovider) ([**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* provider) <br>_Sets the terrain provider._  |
|  [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* | [**terrainProvider**](#function-terrainprovider-22) () const<br>_Returns the terrain provider used for geo-to-3D conversion._  |




























## Detailed Description


Generates 3D tube + sphere geometry to visualize a rubberband path on terrain.


Geo coordinates from a [**RubberbandModel**](classRubberbandModel.md) are projected onto the terrain surface and connected by extruded tube segments. A sphere joint is placed at each vertex. Everything is packed into a single indexed triangle mesh with per-vertex colors.




**Note:**

QML Type: [**Quick3DRubberbandGeometry**](classQuick3DRubberbandGeometry.md) 





    
## Public Properties Documentation




### property color [1/2]

_Per-vertex color applied to both tubes and joint spheres._ 
```C++
QColor Quick3DRubberbandGeometry::color;
```




<hr>



### property heightOffset [1/2]

_Extra vertical offset above the terrain surface._ 
```C++
float Quick3DRubberbandGeometry::heightOffset;
```




<hr>



### property radius [1/2]

_Tube thickness in scene units._ 
```C++
float Quick3DRubberbandGeometry::radius;
```




<hr>



### property rubberbandModel [1/2]

_The rubberband model providing the vertex coordinates._ 
```C++
QML_ELEMENT RubberbandModel * Quick3DRubberbandGeometry::rubberbandModel;
```




<hr>



### property segments [1/2]

_Number of sides in the tube cross-section._ 
```C++
int Quick3DRubberbandGeometry::segments;
```




<hr>



### property terrainProvider [1/2]

_The terrain provider used for geo-to-3D coordinate conversion._ 
```C++
Quick3DTerrainProvider * Quick3DRubberbandGeometry::terrainProvider;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

```C++
void Quick3DRubberbandGeometry::colorChanged;
```




<hr>



### signal heightOffsetChanged 

```C++
void Quick3DRubberbandGeometry::heightOffsetChanged;
```




<hr>



### signal radiusChanged 

```C++
void Quick3DRubberbandGeometry::radiusChanged;
```




<hr>



### signal rubberbandModelChanged 

```C++
void Quick3DRubberbandGeometry::rubberbandModelChanged;
```




<hr>



### signal segmentsChanged 

```C++
void Quick3DRubberbandGeometry::segmentsChanged;
```




<hr>



### signal terrainProviderChanged 

```C++
void Quick3DRubberbandGeometry::terrainProviderChanged;
```




<hr>
## Public Functions Documentation




### function Quick3DRubberbandGeometry 

```C++
explicit Quick3DRubberbandGeometry::Quick3DRubberbandGeometry (
    QQuick3DObject * parent=nullptr
) 
```




<hr>



### function color [2/2]

_Returns the color used for both tubes and sphere joints._ 
```C++
inline QColor Quick3DRubberbandGeometry::color () const
```




<hr>



### function heightOffset [2/2]

_Returns the height offset above the terrain surface._ 
```C++
inline float Quick3DRubberbandGeometry::heightOffset () const
```




<hr>



### function radius [2/2]

_Returns the tube radius in 3D scene units._ 
```C++
inline float Quick3DRubberbandGeometry::radius () const
```




<hr>



### function rubberbandModel [2/2]

_Returns the rubberband model providing vertex coordinates._ 
```C++
inline RubberbandModel * Quick3DRubberbandGeometry::rubberbandModel () const
```




<hr>



### function segments [2/2]

_Returns the number of segments around the tube circumference._ 
```C++
inline int Quick3DRubberbandGeometry::segments () const
```




<hr>



### function setColor 

_Sets the rubberband color._ 
```C++
void Quick3DRubberbandGeometry::setColor (
    const QColor & color
) 
```




<hr>



### function setHeightOffset 

_Sets the height offset._ 
```C++
void Quick3DRubberbandGeometry::setHeightOffset (
    float offset
) 
```




<hr>



### function setRadius 

_Sets the tube radius._ 
```C++
void Quick3DRubberbandGeometry::setRadius (
    float radius
) 
```




<hr>



### function setRubberbandModel 

_Sets the rubberband model._ 
```C++
void Quick3DRubberbandGeometry::setRubberbandModel (
    RubberbandModel * model
) 
```




<hr>



### function setSegments 

_Sets the segment count._ 
```C++
void Quick3DRubberbandGeometry::setSegments (
    int segments
) 
```




<hr>



### function setTerrainProvider 

_Sets the terrain provider._ 
```C++
void Quick3DRubberbandGeometry::setTerrainProvider (
    Quick3DTerrainProvider * provider
) 
```




<hr>



### function terrainProvider [2/2]

_Returns the terrain provider used for geo-to-3D conversion._ 
```C++
inline Quick3DTerrainProvider * Quick3DRubberbandGeometry::terrainProvider () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/quick3drubberbandgeometry.h`

