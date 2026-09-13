

# Class QfMapToView3D



[**ClassList**](annotated.md) **>** [**QfMapToView3D**](classQfMapToView3D.md)



[More...](#detailed-description)

* `#include <qfmaptoview3d.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property float | [**heightOffset**](classQfMapToView3D.md#property-heightoffset-12)  <br>_Additional vertical offset above the terrain surface._  |
| property QgsPoint | [**mapPoint**](classQfMapToView3D.md#property-mappoint-12)  <br>_The point in map coordinates._  |
| property [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* | [**terrainProvider**](classQfMapToView3D.md#property-terrainprovider-12)  <br>_The terrain provider used for geo-to-3D coordinate conversion._  |
| property QVector3D | [**viewPoint**](classQfMapToView3D.md#property-viewpoint-12)  <br>_The point in 3D coordinates._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**heightOffsetChanged**](classQfMapToView3D.md#signal-heightoffsetchanged)  <br> |
| signal void | [**mapPointChanged**](classQfMapToView3D.md#signal-mappointchanged)  <br> |
| signal void | [**terrainProviderChanged**](classQfMapToView3D.md#signal-terrainproviderchanged)  <br> |
| signal void | [**viewPointChanged**](classQfMapToView3D.md#signal-viewpointchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMapToView3D**](#function-qfmaptoview3d) (QObject \* parent=nullptr) <br> |
|  float | [**heightOffset**](#function-heightoffset-22) () const<br> |
|  QgsPoint | [**mapPoint**](#function-mappoint-22) () const<br> |
|  void | [**setHeightOffset**](#function-setheightoffset) (float offset) <br> |
|  void | [**setMapPoint**](#function-setmappoint) (const QgsPoint & point) <br> |
|  void | [**setTerrainProvider**](#function-setterrainprovider) ([**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* provider) <br> |
|  [**Qf3DTerrainProvider**](classQf3DTerrainProvider.md) \* | [**terrainProvider**](#function-terrainprovider-22) () const<br> |
|  QVector3D | [**viewPoint**](#function-viewpoint-22) () const<br> |




























## Detailed Description


Transforms a map point to a 3D position using a terrain provider.


The 3D point is automatically updated when the map point or terrain provider extent changes, enabling dynamic positioning of 3D elements.




**Note:**

QML Type: [**QfMapToView3D**](classQfMapToView3D.md) 





    
## Public Properties Documentation




### property heightOffset [1/2]

_Additional vertical offset above the terrain surface._ 
```C++
float QfMapToView3D::heightOffset;
```




<hr>



### property mapPoint [1/2]

_The point in map coordinates._ 
```C++
QgsPoint QfMapToView3D::mapPoint;
```




<hr>



### property terrainProvider [1/2]

_The terrain provider used for geo-to-3D coordinate conversion._ 
```C++
Qf3DTerrainProvider * QfMapToView3D::terrainProvider;
```




<hr>



### property viewPoint [1/2]

_The point in 3D coordinates._ 
```C++
QVector3D QfMapToView3D::viewPoint;
```




<hr>
## Public Signals Documentation




### signal heightOffsetChanged 

```C++
void QfMapToView3D::heightOffsetChanged;
```




<hr>



### signal mapPointChanged 

```C++
void QfMapToView3D::mapPointChanged;
```




<hr>



### signal terrainProviderChanged 

```C++
void QfMapToView3D::terrainProviderChanged;
```




<hr>



### signal viewPointChanged 

```C++
void QfMapToView3D::viewPointChanged;
```




<hr>
## Public Functions Documentation




### function QfMapToView3D 

```C++
explicit QfMapToView3D::QfMapToView3D (
    QObject * parent=nullptr
) 
```




<hr>



### function heightOffset [2/2]

```C++
float QfMapToView3D::heightOffset () const
```




<hr>



### function mapPoint [2/2]

```C++
QgsPoint QfMapToView3D::mapPoint () const
```




<hr>



### function setHeightOffset 

```C++
void QfMapToView3D::setHeightOffset (
    float offset
) 
```




<hr>



### function setMapPoint 

```C++
void QfMapToView3D::setMapPoint (
    const QgsPoint & point
) 
```




<hr>



### function setTerrainProvider 

```C++
void QfMapToView3D::setTerrainProvider (
    Qf3DTerrainProvider * provider
) 
```




<hr>



### function terrainProvider [2/2]

```C++
Qf3DTerrainProvider * QfMapToView3D::terrainProvider () const
```




<hr>



### function viewPoint [2/2]

```C++
QVector3D QfMapToView3D::viewPoint () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/3d/qfmaptoview3d.h`

