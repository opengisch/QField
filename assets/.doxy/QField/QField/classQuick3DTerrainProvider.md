

# Class Quick3DTerrainProvider



[**ClassList**](annotated.md) **>** [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md)



[More...](#detailed-description)

* `#include <quick3dterrainprovider.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsRectangle | [**extent**](classQuick3DTerrainProvider.md#property-extent-12)  <br>_Geographic extent used for terrain data._  |
| property bool | [**forceSquareSize**](classQuick3DTerrainProvider.md#property-forcesquaresize-12)  <br>_Whether the extent's shorter side will be expanded to form a square terrain._  |
| property QSize | [**gridSize**](classQuick3DTerrainProvider.md#property-gridsize-12)  <br>_Grid dimensions (width x height) for terrain sampling._  |
| property bool | [**isLoading**](classQuick3DTerrainProvider.md#property-isloading-12)  <br>_Whether terrain data is currently being loaded._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQuick3DTerrainProvider.md#property-mapsettings-12)  <br>_The map settings from which to get extent for terrain generation._  |
| property QVariantList | [**normalizedData**](classQuick3DTerrainProvider.md#property-normalizeddata-12)  <br>_Normalized height data array [0.0-1.0] for terrain mesh generation._  |
| property QgsProject \* | [**project**](classQuick3DTerrainProvider.md#property-project-12)  <br>_The project from which to read the terrain configuration and DEM layer._  |
| property QSizeF | [**size**](classQuick3DTerrainProvider.md#property-size-12)  <br>_Size of the terrain._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**extentChanged**](classQuick3DTerrainProvider.md#signal-extentchanged)  <br> |
| signal void | [**forceSquareSizeChanged**](classQuick3DTerrainProvider.md#signal-forcesquaresizechanged)  <br> |
| signal void | [**gridSizeChanged**](classQuick3DTerrainProvider.md#signal-gridsizechanged)  <br> |
| signal void | [**isLoadingChanged**](classQuick3DTerrainProvider.md#signal-isloadingchanged)  <br> |
| signal void | [**mapSettingsChanged**](classQuick3DTerrainProvider.md#signal-mapsettingschanged)  <br> |
| signal void | [**normalizedDataChanged**](classQuick3DTerrainProvider.md#signal-normalizeddatachanged)  <br> |
| signal void | [**projectChanged**](classQuick3DTerrainProvider.md#signal-projectchanged)  <br> |
| signal void | [**terrainDataReady**](classQuick3DTerrainProvider.md#signal-terraindataready)  <br>_Emitted when terrain data has been successfully loaded and is ready for use._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DTerrainProvider**](#function-quick3dterrainprovider) (QObject \* parent=nullptr) <br>_Creates a new terrain provider._  |
|  Q\_INVOKABLE double | [**calculateVisualExaggeration**](#function-calculatevisualexaggeration) () const<br> |
|  QgsRectangle | [**extent**](#function-extent-22) () const<br>_Returns the geographic extent for terrain data retrieval._  |
|  bool | [**forceSquareSize**](#function-forcesquaresize-22) () const<br> |
|  Q\_INVOKABLE QVector3D | [**geoTo3D**](#function-geoto3d) (double geoX, double geoY, float heightOffset=0.0f) const<br> |
|  QSize | [**gridSize**](#function-gridsize-22) () const<br>_Returns the grid dimensions for terrain sampling._  |
|  Q\_INVOKABLE double | [**heightAt**](#function-heightat) (double x, double y) const<br> |
|  bool | [**isLoading**](#function-isloading-22) () const<br>_Returns TRUE if terrain data is currently being loaded._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings._  |
|  QVariantList | [**normalizedData**](#function-normalizeddata-22) () const<br>_Returns the normalized height data array [0.0-1.0]._  |
|  Q\_INVOKABLE double | [**normalizedHeightAt**](#function-normalizedheightat) (double x, double y) const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the project from which to read terrain configuration and DEM layer._  |
|  void | [**setForceSquareSize**](#function-setforcesquaresize) (bool forceSquareSize) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Sets the map settings._  |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the project._  |
|  QSizeF | [**size**](#function-size-22) () const<br>_Returns the size of the terrain._  |
|   | [**~Quick3DTerrainProvider**](#function-quick3dterrainprovider) () override<br> |




























## Detailed Description


Provides terrain elevation data for 3D visualization from DEM raster layers or online elevation services.


This class handles elevation data retrieval from either a project's DEM raster layer or falls back to QGIS terrain providers (e.g., online Terrarium tiles) when no DEM is available. The data is normalized and provided as height samples for terrain mesh generation in Qt Quick 3D.




**Note:**

QML Type: [**Quick3DTerrainProvider**](classQuick3DTerrainProvider.md) 





    
## Public Properties Documentation




### property extent [1/2]

_Geographic extent used for terrain data._ 
```C++
QgsRectangle Quick3DTerrainProvider::extent;
```




<hr>



### property forceSquareSize [1/2]

_Whether the extent's shorter side will be expanded to form a square terrain._ 
```C++
bool Quick3DTerrainProvider::forceSquareSize;
```




<hr>



### property gridSize [1/2]

_Grid dimensions (width x height) for terrain sampling._ 
```C++
QSize Quick3DTerrainProvider::gridSize;
```




<hr>



### property isLoading [1/2]

_Whether terrain data is currently being loaded._ 
```C++
bool Quick3DTerrainProvider::isLoading;
```




<hr>



### property mapSettings [1/2]

_The map settings from which to get extent for terrain generation._ 
```C++
QgsQuickMapSettings * Quick3DTerrainProvider::mapSettings;
```




<hr>



### property normalizedData [1/2]

_Normalized height data array [0.0-1.0] for terrain mesh generation._ 
```C++
QVariantList Quick3DTerrainProvider::normalizedData;
```




<hr>



### property project [1/2]

_The project from which to read the terrain configuration and DEM layer._ 
```C++
QgsProject * Quick3DTerrainProvider::project;
```




<hr>



### property size [1/2]

_Size of the terrain._ 
```C++
QSizeF Quick3DTerrainProvider::size;
```




<hr>
## Public Signals Documentation




### signal extentChanged 

```C++
void Quick3DTerrainProvider::extentChanged;
```




<hr>



### signal forceSquareSizeChanged 

```C++
void Quick3DTerrainProvider::forceSquareSizeChanged;
```




<hr>



### signal gridSizeChanged 

```C++
void Quick3DTerrainProvider::gridSizeChanged;
```




<hr>



### signal isLoadingChanged 

```C++
void Quick3DTerrainProvider::isLoadingChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void Quick3DTerrainProvider::mapSettingsChanged;
```




<hr>



### signal normalizedDataChanged 

```C++
void Quick3DTerrainProvider::normalizedDataChanged;
```




<hr>



### signal projectChanged 

```C++
void Quick3DTerrainProvider::projectChanged;
```




<hr>



### signal terrainDataReady 

_Emitted when terrain data has been successfully loaded and is ready for use._ 
```C++
void Quick3DTerrainProvider::terrainDataReady;
```




<hr>
## Public Functions Documentation




### function Quick3DTerrainProvider 

_Creates a new terrain provider._ 
```C++
explicit Quick3DTerrainProvider::Quick3DTerrainProvider (
    QObject * parent=nullptr
) 
```




<hr>



### function calculateVisualExaggeration 

```C++
Q_INVOKABLE double Quick3DTerrainProvider::calculateVisualExaggeration () const
```



Calculates recommended vertical exaggeration factor based on terrain height range. 

**Returns:**

Exaggeration multiplier to enhance terrain visibility 





        

<hr>



### function extent [2/2]

_Returns the geographic extent for terrain data retrieval._ 
```C++
QgsRectangle Quick3DTerrainProvider::extent () const
```




<hr>



### function forceSquareSize [2/2]

```C++
bool Quick3DTerrainProvider::forceSquareSize () const
```




<hr>



### function geoTo3D 

```C++
Q_INVOKABLE QVector3D Quick3DTerrainProvider::geoTo3D (
    double geoX,
    double geoY,
    float heightOffset=0.0f
) const
```



Converts geographic coordinates to a 3D scene position. 

**Parameters:**


* `geoX` X coordinate in map CRS 
* `geoY` Y coordinate in map CRS 
* `heightOffset` additional vertical offset (default 0) 



**Returns:**

3D position in scene space, or a null vector if the extent is invalid 





        

<hr>



### function gridSize [2/2]

_Returns the grid dimensions for terrain sampling._ 
```C++
QSize Quick3DTerrainProvider::gridSize () const
```




<hr>



### function heightAt 

```C++
Q_INVOKABLE double Quick3DTerrainProvider::heightAt (
    double x,
    double y
) const
```



Returns the real height value at the specified map coordinates. 

**Parameters:**


* `x` X coordinate in map CRS 
* `y` Y coordinate in map CRS 



**Returns:**

Height in map units, or 0 if outside extent 





        

<hr>



### function isLoading [2/2]

_Returns TRUE if terrain data is currently being loaded._ 
```C++
bool Quick3DTerrainProvider::isLoading () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings._ 
```C++
QgsQuickMapSettings * Quick3DTerrainProvider::mapSettings () const
```




<hr>



### function normalizedData [2/2]

_Returns the normalized height data array [0.0-1.0]._ 
```C++
QVariantList Quick3DTerrainProvider::normalizedData () const
```




<hr>



### function normalizedHeightAt 

```C++
Q_INVOKABLE double Quick3DTerrainProvider::normalizedHeightAt (
    double x,
    double y
) const
```



Returns the normalized height value [0.0-1.0] at the specified map coordinates. 

**Parameters:**


* `x` X coordinate in map CRS 
* `y` Y coordinate in map CRS 



**Returns:**

Normalized height between 0 and 1, or 0 if outside extent 





        

<hr>



### function project [2/2]

_Returns the project from which to read terrain configuration and DEM layer._ 
```C++
QgsProject * Quick3DTerrainProvider::project () const
```




<hr>



### function setForceSquareSize 

```C++
void Quick3DTerrainProvider::setForceSquareSize (
    bool forceSquareSize
) 
```




<hr>



### function setMapSettings 

_Sets the map settings._ 
```C++
void Quick3DTerrainProvider::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setProject 

_Sets the project._ 
```C++
void Quick3DTerrainProvider::setProject (
    QgsProject * project
) 
```




<hr>



### function size [2/2]

_Returns the size of the terrain._ 
```C++
QSizeF Quick3DTerrainProvider::size () const
```




<hr>



### function ~Quick3DTerrainProvider 

```C++
Quick3DTerrainProvider::~Quick3DTerrainProvider () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/quick3dterrainprovider.h`

