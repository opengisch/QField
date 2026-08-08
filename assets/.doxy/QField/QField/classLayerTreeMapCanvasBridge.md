

# Class LayerTreeMapCanvasBridge



[**ClassList**](annotated.md) **>** [**LayerTreeMapCanvasBridge**](classLayerTreeMapCanvasBridge.md)



[More...](#detailed-description)

* `#include <layertreemapcanvasbridge.h>`



Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LayerTreeMapCanvasBridge**](#function-layertreemapcanvasbridge) ([**FlatLayerTreeModel**](classFlatLayerTreeModel.md) \* model, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings, [**TrackingModel**](classTrackingModel.md) \* trackingModel, QObject \* parent=nullptr) <br>_Constructor: does not take ownership of the layer tree nor canvas._  |
|  bool | [**autoEnableCrsTransform**](#function-autoenablecrstransform) () const<br> |
|  bool | [**autoSetupOnFirstLayer**](#function-autosetuponfirstlayer) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings) () const<br> |
|  QgsLayerTree \* | [**rootGroup**](#function-rootgroup) () const<br> |
|  void | [**setAutoEnableCrsTransform**](#function-setautoenablecrstransform) (bool enabled) <br> |
|  void | [**setAutoSetupOnFirstLayer**](#function-setautosetuponfirstlayer) (bool enabled) <br> |
|  Q\_INVOKABLE void | [**setCanvasLayers**](#function-setcanvaslayers-12) () <br>_force update of canvas layers from the layer tree. Normally this should not be needed to be called._  |




























## Detailed Description


The QgsLayerTreeMapCanvasBridge class takes care of updates of layer set for QgsMapCanvas from a layer tree. The class listens to the updates in the layer tree and updates the list of layers for rendering whenever some layers are added, removed, or their visibility changes.


The update of layers is not done immediately - it is postponed, so a series of updates to the layer tree will trigger just one update of canvas layers.


Also allows the client to override the default order of layers. This is useful in advanced cases where the grouping in layer tree should be independent from the actual order in the canvas.


Passes to the legend (LayerTreeModel) the information, if the layer is in a tracking session 


    
## Public Functions Documentation




### function LayerTreeMapCanvasBridge 

_Constructor: does not take ownership of the layer tree nor canvas._ 
```C++
LayerTreeMapCanvasBridge::LayerTreeMapCanvasBridge (
    FlatLayerTreeModel * model,
    QgsQuickMapSettings * mapSettings,
    TrackingModel * trackingModel,
    QObject * parent=nullptr
) 
```




<hr>



### function autoEnableCrsTransform 

```C++
inline bool LayerTreeMapCanvasBridge::autoEnableCrsTransform () const
```




<hr>



### function autoSetupOnFirstLayer 

```C++
inline bool LayerTreeMapCanvasBridge::autoSetupOnFirstLayer () const
```




<hr>



### function mapSettings 

```C++
inline QgsQuickMapSettings * LayerTreeMapCanvasBridge::mapSettings () const
```




<hr>



### function rootGroup 

```C++
inline QgsLayerTree * LayerTreeMapCanvasBridge::rootGroup () const
```




<hr>



### function setAutoEnableCrsTransform 

```C++
inline void LayerTreeMapCanvasBridge::setAutoEnableCrsTransform (
    bool enabled
) 
```



if enabled, will automatically turn on on-the-fly reprojection of layers if a layer with different source CRS is added 


        

<hr>



### function setAutoSetupOnFirstLayer 

```C++
inline void LayerTreeMapCanvasBridge::setAutoSetupOnFirstLayer (
    bool enabled
) 
```



if enabled, will automatically set full canvas extent and destination CRS + map units when first layer(s) are added 


        

<hr>



### function setCanvasLayers [1/2]

_force update of canvas layers from the layer tree. Normally this should not be needed to be called._ 
```C++
Q_INVOKABLE void LayerTreeMapCanvasBridge::setCanvasLayers () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/layertreemapcanvasbridge.h`

