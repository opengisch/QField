

# Class MapToView3D



[**ClassList**](annotated.md) **>** [**MapToView3D**](classMapToView3D.md)



[More...](#detailed-description)

* `#include <maptoview3d.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property float | [**heightOffset**](classMapToView3D.md#property-heightoffset-12)  <br>_Additional vertical offset above the terrain surface._  |
| property QgsPoint | [**mapPoint**](classMapToView3D.md#property-mappoint-12)  <br>_The point in map coordinates._  |
| property QML\_ELEMENT [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* | [**terrainProvider**](classMapToView3D.md#property-terrainprovider-12)  <br>_The terrain provider used for geo-to-3D coordinate conversion._  |
| property QVector3D | [**viewPoint**](classMapToView3D.md#property-viewpoint-12)  <br>_The point in 3D coordinates._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**heightOffsetChanged**](classMapToView3D.md#signal-heightoffsetchanged)  <br> |
| signal void | [**mapPointChanged**](classMapToView3D.md#signal-mappointchanged)  <br> |
| signal void | [**terrainProviderChanged**](classMapToView3D.md#signal-terrainproviderchanged)  <br> |
| signal void | [**viewPointChanged**](classMapToView3D.md#signal-viewpointchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapToView3D**](#function-maptoview3d) (QObject \* parent=nullptr) <br> |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br> |
|  QgsPoint | [**mapPoint**](#function-mappoint-22) () const<br> |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br> |
|  void | [**setMapPoint**](#function-setmappoint) (const QgsPoint & point) <br> |
|  void | [**setTerrainProvider**](#function-setterrainprovider) ([**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* provider) <br> |
|  [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) \* | [**terrainProvider**](#function-terrainprovider-22) () const<br> |
|  QVector3D | [**viewPoint**](#function-viewpoint-22) () const<br> |




























## Detailed Description


Transforms a map point to a 3D position using a terrain provider.


The 3D point is automatically updated when the map point or terrain provider extent changes, enabling dynamic positioning of 3D elements.




**Note:**

QML Type: [**MapToView3D**](classMapToView3D.md) 





    
## Public Properties Documentation




### property heightOffset [1/2]

_Additional vertical offset above the terrain surface._ 
```C++
float MapToView3D::heightOffset;
```




<hr>



### property mapPoint [1/2]

_The point in map coordinates._ 
```C++
QgsPoint MapToView3D::mapPoint;
```




<hr>



### property terrainProvider [1/2]

_The terrain provider used for geo-to-3D coordinate conversion._ 
```C++
QML_ELEMENT Quick3DTerrainProvider * MapToView3D::terrainProvider;
```




<hr>



### property viewPoint [1/2]

_The point in 3D coordinates._ 
```C++
QVector3D MapToView3D::viewPoint;
```




<hr>
## Public Signals Documentation




### signal heightOffsetChanged 

```C++
void MapToView3D::heightOffsetChanged;
```




<hr>



### signal mapPointChanged 

```C++
void MapToView3D::mapPointChanged;
```




<hr>



### signal terrainProviderChanged 

```C++
void MapToView3D::terrainProviderChanged;
```




<hr>



### signal viewPointChanged 

```C++
void MapToView3D::viewPointChanged;
```




<hr>
## Public Functions Documentation




### function MapToView3D 

```C++
explicit MapToView3D::MapToView3D (
    QObject * parent=nullptr
) 
```




<hr>



### function heightOffset [2/2]

```C++
float MapToView3D::heightOffset () const
```




<hr>



### function mapPoint [2/2]

```C++
QgsPoint MapToView3D::mapPoint () const
```




<hr>



### function setHeightOffset 

```C++
void MapToView3D::setHeightOffset (
    float offset
) 
```




<hr>



### function setMapPoint 

```C++
void MapToView3D::setMapPoint (
    const QgsPoint & point
) 
```




<hr>



### function setTerrainProvider 

```C++
void MapToView3D::setTerrainProvider (
    Quick3DTerrainProvider * provider
) 
```




<hr>



### function terrainProvider [2/2]

```C++
Quick3DTerrainProvider * MapToView3D::terrainProvider () const
```




<hr>



### function viewPoint [2/2]

```C++
QVector3D MapToView3D::viewPoint () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/maptoview3d.h`

