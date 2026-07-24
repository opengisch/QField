

# Class Quick3DGeometryUtils



[**ClassList**](annotated.md) **>** [**Quick3DGeometryUtils**](classQuick3DGeometryUtils.md)



[More...](#detailed-description)

* `#include <quick3dgeometryutils.h>`























## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  constexpr int | [**VERTEX\_STRIDE**](#variable-vertex_stride)   = `10 \* sizeof( float )`<br>_Byte stride of a single vertex: position(3) + normal(3) + rgba(4) = 10 floats._  |
















## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**generatePolygonFill**](#function-generatepolygonfill) (const QVector&lt; QVector3D &gt; & vertices, float r, float g, float b, float a, float \*& vptr, quint32 \*& iptr, quint32 & vertexOffset, QVector3D & minBound, QVector3D & maxBound, float extrusion=0.0f) <br> |
|  void | [**generatePolygonWalls**](#function-generatepolygonwalls) (const QVector&lt; QVector3D &gt; & vertices, float extrusion, float r, float g, float b, float a, float \*& vptr, quint32 \*& iptr, quint32 & vertexOffset, QVector3D & minBound, QVector3D & maxBound) <br> |
|  void | [**generateSphere**](#function-generatesphere) (const QVector3D & center, float radius, int stacks, int slices, float r, float g, float b, float a, float \*& vptr, quint32 \*& iptr, quint32 & vertexOffset, QVector3D & minBound, QVector3D & maxBound) <br>_Generates a UV sphere centered at_ _center_ _._ |
|  void | [**generateTube**](#function-generatetube) (const QVector&lt; QVector3D &gt; & path, int segments, float radius, float r, float g, float b, float a, float \*& vptr, quint32 \*& iptr, quint32 & vertexOffset, QVector3D & minBound, QVector3D & maxBound) <br>_Extrudes a circular tube of_ _segments_ _sides and given__radius_ _along__path_ _._ |
|  int | [**polygonWallsIndexCount**](#function-polygonwallsindexcount) (int ringSize) <br>_Returns the number of indices produced by_ [_**generatePolygonWalls()**_](classQuick3DGeometryUtils.md#function-generatepolygonwalls) _for a ring with__ringSize_ _unique vertices._ |
|  int | [**polygonWallsVertexCount**](#function-polygonwallsvertexcount) (int ringSize) <br>_Returns the number of vertices produced by_ [_**generatePolygonWalls()**_](classQuick3DGeometryUtils.md#function-generatepolygonwalls) _for a ring with__ringSize_ _unique vertices._ |
|  int | [**sphereIndexCount**](#function-sphereindexcount) (int stacks, int slices) <br>_Returns the number of indices produced by_ [_**generateSphere()**_](classQuick3DGeometryUtils.md#function-generatesphere) _for the given subdivisions._ |
|  int | [**sphereVertexCount**](#function-spherevertexcount) (int stacks, int slices) <br>_Returns the number of vertices produced by_ [_**generateSphere()**_](classQuick3DGeometryUtils.md#function-generatesphere) _for the given subdivisions._ |
|  void | [**updateBounds**](#function-updatebounds) (QVector3D & minBound, QVector3D & maxBound, const QVector3D & pos) <br>_Expands_ _minBound_ _/__maxBound_ _to include__pos_ _._ |
|  void | [**writeVertex**](#function-writevertex) (float \*& vptr, const QVector3D & pos, const QVector3D & normal, float r, float g, float b, float a) <br>_Writes a single vertex (position + normal + rgba) to_ _vptr_ _and advances the pointer._ |


























## Detailed Description


Mesh-building helpers shared between the rubberband and feature highlight geometries. Each helper appends to a raw vertex/index buffer using a fixed 10-float vertex layout: position(3) + normal(3) + rgba(4). 


    
## Public Static Attributes Documentation




### variable VERTEX\_STRIDE 

_Byte stride of a single vertex: position(3) + normal(3) + rgba(4) = 10 floats._ 
```C++
constexpr int Quick3DGeometryUtils::VERTEX_STRIDE;
```




<hr>
## Public Static Functions Documentation




### function generatePolygonFill 

```C++
static void Quick3DGeometryUtils::generatePolygonFill (
    const QVector< QVector3D > & vertices,
    float r,
    float g,
    float b,
    float a,
    float *& vptr,
    quint32 *& iptr,
    quint32 & vertexOffset,
    QVector3D & minBound,
    QVector3D & maxBound,
    float extrusion=0.0f
) 
```



Triangulates a closed polygon ring on the XZ plane with ear clipping and appends the resulting triangle fan to the buffer. Falls back to a simple fan from vertex 0 when ear clipping cannot make further progress (which keeps degenerate or self-touching rings from leaving uninitialised indices in the buffer).


When _extrusion_ is non-zero every fill vertex is shifted upward by that amount so the fill becomes a roof cap on top of extruded walls. 


        

<hr>



### function generatePolygonWalls 

```C++
static void Quick3DGeometryUtils::generatePolygonWalls (
    const QVector< QVector3D > & vertices,
    float extrusion,
    float r,
    float g,
    float b,
    float a,
    float *& vptr,
    quint32 *& iptr,
    quint32 & vertexOffset,
    QVector3D & minBound,
    QVector3D & maxBound
) 
```



Generates vertical wall quads for each edge of a closed polygon ring, extruding from the base vertices upward by _extrusion_ scene units. Each edge produces 4 vertices and 2 triangles (6 indices). 


        

<hr>



### function generateSphere 

_Generates a UV sphere centered at_ _center_ _._
```C++
static void Quick3DGeometryUtils::generateSphere (
    const QVector3D & center,
    float radius,
    int stacks,
    int slices,
    float r,
    float g,
    float b,
    float a,
    float *& vptr,
    quint32 *& iptr,
    quint32 & vertexOffset,
    QVector3D & minBound,
    QVector3D & maxBound
) 
```




<hr>



### function generateTube 

_Extrudes a circular tube of_ _segments_ _sides and given__radius_ _along__path_ _._
```C++
static void Quick3DGeometryUtils::generateTube (
    const QVector< QVector3D > & path,
    int segments,
    float radius,
    float r,
    float g,
    float b,
    float a,
    float *& vptr,
    quint32 *& iptr,
    quint32 & vertexOffset,
    QVector3D & minBound,
    QVector3D & maxBound
) 
```




<hr>



### function polygonWallsIndexCount 

_Returns the number of indices produced by_ [_**generatePolygonWalls()**_](classQuick3DGeometryUtils.md#function-generatepolygonwalls) _for a ring with__ringSize_ _unique vertices._
```C++
static inline int Quick3DGeometryUtils::polygonWallsIndexCount (
    int ringSize
) 
```




<hr>



### function polygonWallsVertexCount 

_Returns the number of vertices produced by_ [_**generatePolygonWalls()**_](classQuick3DGeometryUtils.md#function-generatepolygonwalls) _for a ring with__ringSize_ _unique vertices._
```C++
static inline int Quick3DGeometryUtils::polygonWallsVertexCount (
    int ringSize
) 
```




<hr>



### function sphereIndexCount 

_Returns the number of indices produced by_ [_**generateSphere()**_](classQuick3DGeometryUtils.md#function-generatesphere) _for the given subdivisions._
```C++
static int Quick3DGeometryUtils::sphereIndexCount (
    int stacks,
    int slices
) 
```




<hr>



### function sphereVertexCount 

_Returns the number of vertices produced by_ [_**generateSphere()**_](classQuick3DGeometryUtils.md#function-generatesphere) _for the given subdivisions._
```C++
static int Quick3DGeometryUtils::sphereVertexCount (
    int stacks,
    int slices
) 
```




<hr>



### function updateBounds 

_Expands_ _minBound_ _/__maxBound_ _to include__pos_ _._
```C++
static void Quick3DGeometryUtils::updateBounds (
    QVector3D & minBound,
    QVector3D & maxBound,
    const QVector3D & pos
) 
```




<hr>



### function writeVertex 

_Writes a single vertex (position + normal + rgba) to_ _vptr_ _and advances the pointer._
```C++
static void Quick3DGeometryUtils::writeVertex (
    float *& vptr,
    const QVector3D & pos,
    const QVector3D & normal,
    float r,
    float g,
    float b,
    float a
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/quick3dgeometryutils.h`

