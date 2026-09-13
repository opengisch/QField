

# Class Qf3DTerrainGeometry



[**ClassList**](annotated.md) **>** [**Qf3DTerrainGeometry**](classQf3DTerrainGeometry.md)



[More...](#detailed-description)

* `#include <qf3dterraingeometry.h>`



Inherits the following classes: QQuick3DGeometry


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QVector3D | [**boundsMax**](classQf3DTerrainGeometry.md#property-boundsmax)  <br>_Maximum bounds of the geometry._  |
| property QVector3D | [**boundsMin**](classQf3DTerrainGeometry.md#property-boundsmin)  <br>_Minimum bounds of the geometry._  |
| property QSize | [**gridSize**](classQf3DTerrainGeometry.md#property-gridsize-12)  <br>_Grid dimensions (width x height) for terrain vertices._  |
| property QVariantList | [**heightData**](classQf3DTerrainGeometry.md#property-heightdata-12)  <br>_Height data array for terrain elevation values._  |
| property double | [**offsetScale**](classQf3DTerrainGeometry.md#property-offsetscale-12)  <br>_Pffset scale from the last generated terrain data._  |
| property QVector3D | [**offsetVector**](classQf3DTerrainGeometry.md#property-offsetvector-12)  <br>_Offset vector from the generated terrain heights._  |
| property QSizeF | [**size**](classQf3DTerrainGeometry.md#property-size-12)  <br>_Width of the terrain mesh in 3D scene units._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**boundsMinMaxChanged**](classQf3DTerrainGeometry.md#signal-boundsminmaxchanged)  <br> |
| signal void | [**gridSizeChanged**](classQf3DTerrainGeometry.md#signal-gridsizechanged)  <br> |
| signal void | [**heightDataChanged**](classQf3DTerrainGeometry.md#signal-heightdatachanged)  <br> |
| signal void | [**offsetScaleChanged**](classQf3DTerrainGeometry.md#signal-offsetscalechanged)  <br> |
| signal void | [**offsetVectorChanged**](classQf3DTerrainGeometry.md#signal-offsetvectorchanged)  <br> |
| signal void | [**sizeChanged**](classQf3DTerrainGeometry.md#signal-sizechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Qf3DTerrainGeometry**](#function-qf3dterraingeometry) (QQuick3DObject \* parent=nullptr) <br>_Creates a new terrain geometry._  |
|  Q\_INVOKABLE void | [**buildMetagridFromProvider**](#function-buildmetagridfromprovider) (const [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* provider) <br>_Builds and stores a 3x3 metagrid from the provider's normalized data._  |
|  QSize | [**gridSize**](#function-gridsize-22) () const<br>_Returns the grid dimensions._  |
|  QVariantList | [**heightData**](#function-heightdata-22) () const<br>_Returns the height data in a QML-friendly QVariantList._  |
|  double | [**offsetScale**](#function-offsetscale-22) () const<br> |
|  QVector3D | [**offsetVector**](#function-offsetvector-22) () const<br> |
|  Q\_INVOKABLE void | [**restoreHeightsFromProvider**](#function-restoreheightsfromprovider) (const [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* provider) <br>_Restores the original height data from the provider._  |
|  void | [**setGridSize**](#function-setgridsize) (const QSize & size) <br>_Sets the grid dimensions._  |
|  void | [**setHeightData**](#function-setheightdata) (const QVariantList & data) <br>_Sets the height data array._  |
|  void | [**setOffsetScale**](#function-setoffsetscale) (double offsetScale) <br> |
|  void | [**setOffsetVector**](#function-setoffsetvector) (const QVector3D & offsetVector) <br> |
|  void | [**setSize**](#function-setsize) (const QSizeF & size) <br>_Sets the terrain width._  |
|  QSizeF | [**size**](#function-size-22) () const<br>_Returns the width of the terrain mesh in 3D scene units._  |




























## Detailed Description


Custom Qt Quick 3D geometry for rendering terrain meshes with height data.


This class generates a triangulated mesh grid with vertex positions, normals, and texture coordinates based on provided elevation data. The geometry is suitable for use with Qt Quick 3D Model items and supports dynamic updates when height data changes.




**Note:**

QML Type: [**Qf3DTerrainGeometry**](classQf3DTerrainGeometry.md) 





    
## Public Properties Documentation




### property boundsMax 

_Maximum bounds of the geometry._ 
```C++
QVector3D Qf3DTerrainGeometry::boundsMax;
```




<hr>



### property boundsMin 

_Minimum bounds of the geometry._ 
```C++
QVector3D Qf3DTerrainGeometry::boundsMin;
```




<hr>



### property gridSize [1/2]

_Grid dimensions (width x height) for terrain vertices._ 
```C++
QSize Qf3DTerrainGeometry::gridSize;
```




<hr>



### property heightData [1/2]

_Height data array for terrain elevation values._ 
```C++
QVariantList Qf3DTerrainGeometry::heightData;
```




<hr>



### property offsetScale [1/2]

_Pffset scale from the last generated terrain data._ 
```C++
double Qf3DTerrainGeometry::offsetScale;
```




<hr>



### property offsetVector [1/2]

_Offset vector from the generated terrain heights._ 
```C++
QVector3D Qf3DTerrainGeometry::offsetVector;
```




<hr>



### property size [1/2]

_Width of the terrain mesh in 3D scene units._ 
```C++
QSizeF Qf3DTerrainGeometry::size;
```




<hr>
## Public Signals Documentation




### signal boundsMinMaxChanged 

```C++
void Qf3DTerrainGeometry::boundsMinMaxChanged;
```




<hr>



### signal gridSizeChanged 

```C++
void Qf3DTerrainGeometry::gridSizeChanged;
```




<hr>



### signal heightDataChanged 

```C++
void Qf3DTerrainGeometry::heightDataChanged;
```




<hr>



### signal offsetScaleChanged 

```C++
void Qf3DTerrainGeometry::offsetScaleChanged;
```




<hr>



### signal offsetVectorChanged 

```C++
void Qf3DTerrainGeometry::offsetVectorChanged;
```




<hr>



### signal sizeChanged 

```C++
void Qf3DTerrainGeometry::sizeChanged;
```




<hr>
## Public Functions Documentation




### function Qf3DTerrainGeometry 

_Creates a new terrain geometry._ 
```C++
explicit Qf3DTerrainGeometry::Qf3DTerrainGeometry (
    QQuick3DObject * parent=nullptr
) 
```




<hr>



### function buildMetagridFromProvider 

_Builds and stores a 3x3 metagrid from the provider's normalized data._ 
```C++
Q_INVOKABLE void Qf3DTerrainGeometry::buildMetagridFromProvider (
    const Qf3DTerrainProvider * provider
) 
```




<hr>



### function gridSize [2/2]

_Returns the grid dimensions._ 
```C++
inline QSize Qf3DTerrainGeometry::gridSize () const
```




<hr>



### function heightData [2/2]

_Returns the height data in a QML-friendly QVariantList._ 
```C++
QVariantList Qf3DTerrainGeometry::heightData () const
```




<hr>



### function offsetScale [2/2]

```C++
inline double Qf3DTerrainGeometry::offsetScale () const
```




<hr>



### function offsetVector [2/2]

```C++
inline QVector3D Qf3DTerrainGeometry::offsetVector () const
```




<hr>



### function restoreHeightsFromProvider 

_Restores the original height data from the provider._ 
```C++
Q_INVOKABLE void Qf3DTerrainGeometry::restoreHeightsFromProvider (
    const Qf3DTerrainProvider * provider
) 
```




<hr>



### function setGridSize 

_Sets the grid dimensions._ 
```C++
void Qf3DTerrainGeometry::setGridSize (
    const QSize & size
) 
```




<hr>



### function setHeightData 

_Sets the height data array._ 
```C++
void Qf3DTerrainGeometry::setHeightData (
    const QVariantList & data
) 
```




<hr>



### function setOffsetScale 

```C++
void Qf3DTerrainGeometry::setOffsetScale (
    double offsetScale
) 
```




<hr>



### function setOffsetVector 

```C++
void Qf3DTerrainGeometry::setOffsetVector (
    const QVector3D & offsetVector
) 
```




<hr>



### function setSize 

_Sets the terrain width._ 
```C++
void Qf3DTerrainGeometry::setSize (
    const QSizeF & size
) 
```




<hr>



### function size [2/2]

_Returns the width of the terrain mesh in 3D scene units._ 
```C++
inline QSizeF Qf3DTerrainGeometry::size () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qf3dterraingeometry.h`

