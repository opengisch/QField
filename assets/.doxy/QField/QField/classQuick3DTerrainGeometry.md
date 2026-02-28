

# Class Quick3DTerrainGeometry



[**ClassList**](annotated.md) **>** [**Quick3DTerrainGeometry**](classQuick3DTerrainGeometry.md)



[More...](#detailed-description)

* `#include <quick3dterraingeometry.h>`



Inherits the following classes: QQuick3DGeometry


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QML\_ELEMENT QSize | [**gridSize**](classQuick3DTerrainGeometry.md#property-gridsize-12)  <br>_Grid dimensions (width x height) for terrain vertices._  |
| property QVariantList | [**heightData**](classQuick3DTerrainGeometry.md#property-heightdata)  <br>_Height data array for terrain elevation values._  |
| property QSizeF | [**size**](classQuick3DTerrainGeometry.md#property-size-12)  <br>_Width of the terrain mesh in 3D scene units._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**gridSizeChanged**](classQuick3DTerrainGeometry.md#signal-gridsizechanged)  <br> |
| signal void | [**heightDataChanged**](classQuick3DTerrainGeometry.md#signal-heightdatachanged)  <br> |
| signal void | [**sizeChanged**](classQuick3DTerrainGeometry.md#signal-sizechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DTerrainGeometry**](#function-quick3dterraingeometry) (QQuick3DObject \* parent=nullptr) <br>_Creates a new terrain geometry._  |
|  QSize | [**gridSize**](#function-gridsize-22) () const<br>_Returns the grid dimensions._  |
|  void | [**setGridSize**](#function-setgridsize) (const QSize & size) <br>_Sets the grid dimensions._  |
|  void | [**setHeightData**](#function-setheightdata) (const QVariantList & data) <br>_Sets the height data array._  |
|  void | [**setSize**](#function-setsize) (QSizeF size) <br>_Sets the terrain width._  |
|  QSizeF | [**size**](#function-size-22) () const<br>_Returns the width of the terrain mesh in 3D scene units._  |




























## Detailed Description


Custom Qt Quick 3D geometry for rendering terrain meshes with height data.


This class generates a triangulated mesh grid with vertex positions, normals, and texture coordinates based on provided elevation data. The geometry is suitable for use with Qt Quick 3D Model items and supports dynamic updates when height data changes.




**Note:**

QML Type: [**Quick3DTerrainGeometry**](classQuick3DTerrainGeometry.md) 





    
## Public Properties Documentation




### property gridSize [1/2]

_Grid dimensions (width x height) for terrain vertices._ 
```C++
QML_ELEMENT QSize Quick3DTerrainGeometry::gridSize;
```




<hr>



### property heightData 

_Height data array for terrain elevation values._ 
```C++
QVariantList Quick3DTerrainGeometry::heightData;
```




<hr>



### property size [1/2]

_Width of the terrain mesh in 3D scene units._ 
```C++
QSizeF Quick3DTerrainGeometry::size;
```




<hr>
## Public Signals Documentation




### signal gridSizeChanged 

```C++
void Quick3DTerrainGeometry::gridSizeChanged;
```




<hr>



### signal heightDataChanged 

```C++
void Quick3DTerrainGeometry::heightDataChanged;
```




<hr>



### signal sizeChanged 

```C++
void Quick3DTerrainGeometry::sizeChanged;
```




<hr>
## Public Functions Documentation




### function Quick3DTerrainGeometry 

_Creates a new terrain geometry._ 
```C++
explicit Quick3DTerrainGeometry::Quick3DTerrainGeometry (
    QQuick3DObject * parent=nullptr
) 
```




<hr>



### function gridSize [2/2]

_Returns the grid dimensions._ 
```C++
inline QSize Quick3DTerrainGeometry::gridSize () const
```




<hr>



### function setGridSize 

_Sets the grid dimensions._ 
```C++
void Quick3DTerrainGeometry::setGridSize (
    const QSize & size
) 
```




<hr>



### function setHeightData 

_Sets the height data array._ 
```C++
void Quick3DTerrainGeometry::setHeightData (
    const QVariantList & data
) 
```




<hr>



### function setSize 

_Sets the terrain width._ 
```C++
void Quick3DTerrainGeometry::setSize (
    QSizeF size
) 
```




<hr>



### function size [2/2]

_Returns the width of the terrain mesh in 3D scene units._ 
```C++
inline QSizeF Quick3DTerrainGeometry::size () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/quick3dterraingeometry.h`

