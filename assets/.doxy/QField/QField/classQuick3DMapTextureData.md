

# Class Quick3DMapTextureData



[**ClassList**](annotated.md) **>** [**Quick3DMapTextureData**](classQuick3DMapTextureData.md)



[More...](#detailed-description)

* `#include <quick3dmaptexturedata.h>`



Inherits the following classes: QQuick3DTextureData


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsRectangle | [**extent**](classQuick3DMapTextureData.md#property-extent-12)  <br>_Optional custom extent to render. If not set, uses mapSettings extent._  |
| property bool | [**forceDeferredLayersRepaint**](classQuick3DMapTextureData.md#property-forcedeferredlayersrepaint-12)  <br>_Defers layer repaint requests while a render job is running._  |
| property bool | [**incrementalRendering**](classQuick3DMapTextureData.md#property-incrementalrendering-12)  <br>_When the incrementalRendering property is set to true, the incremental refresh of the terrain data during rendering is allowed._  |
| property bool | [**isReady**](classQuick3DMapTextureData.md#property-isready-12)  <br>_Whether the texture data is ready to use._  |
| property bool | [**isRendering**](classQuick3DMapTextureData.md#property-isrendering-12)  <br>_Whether the texture data is being rendered._  |
| property QML\_ELEMENT [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQuick3DMapTextureData.md#property-mapsettings-12)  <br>_The map settings from which to get layers, extent, and output size for rendering._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**extentChanged**](classQuick3DMapTextureData.md#signal-extentchanged)  <br>_Emitted when extent changes._  |
| signal void | [**forceDeferredLayersRepaintChanged**](classQuick3DMapTextureData.md#signal-forcedeferredlayersrepaintchanged)  <br>_Emitted when forceDeferredLayersRepaint changes._  |
| signal void | [**incrementalRenderingChanged**](classQuick3DMapTextureData.md#signal-incrementalrenderingchanged)  <br>_Emitted when incremental rendering setting changes._  |
| signal void | [**isReadyChanged**](classQuick3DMapTextureData.md#signal-isreadychanged)  <br>_Emitted when texture rendering is complete and data is ready._  |
| signal void | [**isRenderingChanged**](classQuick3DMapTextureData.md#signal-isrenderingchanged)  <br>_Emitted when the texture data is being rendered._  |
| signal void | [**mapSettingsChanged**](classQuick3DMapTextureData.md#signal-mapsettingschanged)  <br>_Emitted when map settings changes._  |
| signal void | [**textureUpdated**](classQuick3DMapTextureData.md#signal-textureupdated)  <br>_Emitted every time texture data has been updated._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Quick3DMapTextureData**](#function-quick3dmaptexturedata) (QQuick3DObject \* parent=nullptr) <br>_Creates a new map texture data provider._  |
|  QgsRectangle | [**extent**](#function-extent-22) () const<br>_Returns the custom extent for rendering._  |
|  bool | [**forceDeferredLayersRepaint**](#function-forcedeferredlayersrepaint-22) () const<br>_Returns whether deferred layers repaint is forced._  |
|  bool | [**incrementalRendering**](#function-incrementalrendering-22) () const<br>_Returns whether incremental rendering is enabled._  |
|  bool | [**isReady**](#function-isready-22) () const<br>_Returns whether the texture data is ready to use._  |
|  bool | [**isRendering**](#function-isrendering-22) () const<br>_Returns true while a rendering job is pending for this texture data._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings from which to get layers._  |
|  Q\_INVOKABLE void | [**render**](#function-render) () <br> |
|  void | [**setExtent**](#function-setextent) (const QgsRectangle & extent) <br>_Sets a custom extent for rendering._  |
|  void | [**setForceDeferredLayersRepaint**](#function-setforcedeferredlayersrepaint) (bool deferred) <br>_Sets whether deferred layers repaint is forced._  |
|  void | [**setIncrementalRendering**](#function-setincrementalrendering) (bool incrementalRendering) <br>_Sets whether incremental rendering is enabled._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Sets the map settings._  |
|   | [**~Quick3DMapTextureData**](#function-quick3dmaptexturedata) () override<br> |




























## Detailed Description


Provides in-memory texture data from rendered map layers for 3D terrain visualization.


This class inherits from QQuick3DTextureData to provide texture data directly in memory, avoiding the need for temporary files. The map is rendered using QGIS rendering engine and the result is exposed as texture data for Qt Quick 3D.




**Note:**

QML Type: [**Quick3DMapTextureData**](classQuick3DMapTextureData.md) 





    
## Public Properties Documentation




### property extent [1/2]

_Optional custom extent to render. If not set, uses mapSettings extent._ 
```C++
QgsRectangle Quick3DMapTextureData::extent;
```




<hr>



### property forceDeferredLayersRepaint [1/2]

_Defers layer repaint requests while a render job is running._ 
```C++
bool Quick3DMapTextureData::forceDeferredLayersRepaint;
```




<hr>



### property incrementalRendering [1/2]

_When the incrementalRendering property is set to true, the incremental refresh of the terrain data during rendering is allowed._ 
```C++
bool Quick3DMapTextureData::incrementalRendering;
```




<hr>



### property isReady [1/2]

_Whether the texture data is ready to use._ 
```C++
bool Quick3DMapTextureData::isReady;
```




<hr>



### property isRendering [1/2]

_Whether the texture data is being rendered._ 
```C++
bool Quick3DMapTextureData::isRendering;
```




<hr>



### property mapSettings [1/2]

_The map settings from which to get layers, extent, and output size for rendering._ 
```C++
QML_ELEMENT QgsQuickMapSettings * Quick3DMapTextureData::mapSettings;
```




<hr>
## Public Signals Documentation




### signal extentChanged 

_Emitted when extent changes._ 
```C++
void Quick3DMapTextureData::extentChanged;
```




<hr>



### signal forceDeferredLayersRepaintChanged 

_Emitted when forceDeferredLayersRepaint changes._ 
```C++
void Quick3DMapTextureData::forceDeferredLayersRepaintChanged;
```




<hr>



### signal incrementalRenderingChanged 

_Emitted when incremental rendering setting changes._ 
```C++
void Quick3DMapTextureData::incrementalRenderingChanged;
```




<hr>



### signal isReadyChanged 

_Emitted when texture rendering is complete and data is ready._ 
```C++
void Quick3DMapTextureData::isReadyChanged;
```




<hr>



### signal isRenderingChanged 

_Emitted when the texture data is being rendered._ 
```C++
void Quick3DMapTextureData::isRenderingChanged;
```




<hr>



### signal mapSettingsChanged 

_Emitted when map settings changes._ 
```C++
void Quick3DMapTextureData::mapSettingsChanged;
```




<hr>



### signal textureUpdated 

_Emitted every time texture data has been updated._ 
```C++
void Quick3DMapTextureData::textureUpdated;
```




<hr>
## Public Functions Documentation




### function Quick3DMapTextureData 

_Creates a new map texture data provider._ 
```C++
explicit Quick3DMapTextureData::Quick3DMapTextureData (
    QQuick3DObject * parent=nullptr
) 
```




<hr>



### function extent [2/2]

_Returns the custom extent for rendering._ 
```C++
QgsRectangle Quick3DMapTextureData::extent () const
```




<hr>



### function forceDeferredLayersRepaint [2/2]

_Returns whether deferred layers repaint is forced._ 
```C++
bool Quick3DMapTextureData::forceDeferredLayersRepaint () const
```




<hr>



### function incrementalRendering [2/2]

_Returns whether incremental rendering is enabled._ 
```C++
bool Quick3DMapTextureData::incrementalRendering () const
```




<hr>



### function isReady [2/2]

_Returns whether the texture data is ready to use._ 
```C++
bool Quick3DMapTextureData::isReady () const
```




<hr>



### function isRendering [2/2]

_Returns true while a rendering job is pending for this texture data._ 
```C++
bool Quick3DMapTextureData::isRendering () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings from which to get layers._ 
```C++
QgsQuickMapSettings * Quick3DMapTextureData::mapSettings () const
```




<hr>



### function render 

```C++
Q_INVOKABLE void Quick3DMapTextureData::render () 
```



Starts the asynchronous map rendering process. 


        

<hr>



### function setExtent 

_Sets a custom extent for rendering._ 
```C++
void Quick3DMapTextureData::setExtent (
    const QgsRectangle & extent
) 
```




<hr>



### function setForceDeferredLayersRepaint 

_Sets whether deferred layers repaint is forced._ 
```C++
void Quick3DMapTextureData::setForceDeferredLayersRepaint (
    bool deferred
) 
```




<hr>



### function setIncrementalRendering 

_Sets whether incremental rendering is enabled._ 
```C++
void Quick3DMapTextureData::setIncrementalRendering (
    bool incrementalRendering
) 
```




<hr>



### function setMapSettings 

_Sets the map settings._ 
```C++
void Quick3DMapTextureData::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function ~Quick3DMapTextureData 

```C++
Quick3DMapTextureData::~Quick3DMapTextureData () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/3d/quick3dmaptexturedata.h`

