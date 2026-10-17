

# Class QgsQuickMapCanvasMap



[**ClassList**](annotated.md) **>** [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md)



[More...](#detailed-description)

* `#include <qgsquickmapcanvasmap.h>`



Inherits the following classes: QQuickItem






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**pan**](classQgsQuickMapCanvasMap.md#slot-pan) (const QPointF oldPos, const QPointF newPos) <br> |
| slot void | [**refresh**](classQgsQuickMapCanvasMap.md#slot-refresh) (bool ignoreFreeze=false) <br> |
| slot void | [**rotate**](classQgsQuickMapCanvasMap.md#slot-rotate) (double degrees) <br> |
| slot void | [**schedulePreviewJob**](classQgsQuickMapCanvasMap.md#slot-schedulepreviewjob) (int number) <br> |
| slot void | [**startPreviewJobs**](classQgsQuickMapCanvasMap.md#slot-startpreviewjobs)  <br> |
| slot void | [**stopPreviewJobs**](classQgsQuickMapCanvasMap.md#slot-stoppreviewjobs)  <br> |
| slot void | [**stopRendering**](classQgsQuickMapCanvasMap.md#slot-stoprendering)  <br>_Stop map rendering._  |
| slot void | [**zoomByFactor**](classQgsQuickMapCanvasMap.md#slot-zoombyfactor) (const QPointF center, qreal factor, bool handleMargins=false) <br> |
| slot void | [**zoomScale**](classQgsQuickMapCanvasMap.md#slot-zoomscale) (const QPointF center, qreal scale, bool handleMargins=false) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**bottomMargin**](classQgsQuickMapCanvasMap.md#property-bottommargin-12)  <br> |
| property double | [**forceDeferredLayersRepaint**](classQgsQuickMapCanvasMap.md#property-forcedeferredlayersrepaint-12)  <br> |
| property bool | [**freeze**](classQgsQuickMapCanvasMap.md#property-freeze-12)  <br> |
| property bool | [**incrementalRendering**](classQgsQuickMapCanvasMap.md#property-incrementalrendering-12)  <br> |
| property bool | [**isRendering**](classQgsQuickMapCanvasMap.md#property-isrendering-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQgsQuickMapCanvasMap.md#property-mapsettings-12)  <br> |
| property int | [**mapUpdateInterval**](classQgsQuickMapCanvasMap.md#property-mapupdateinterval-12)  <br> |
| property bool | [**previewJobsEnabled**](classQgsQuickMapCanvasMap.md#property-previewjobsenabled-12)  <br> |
| property QList&lt; int &gt; | [**previewJobsQuadrants**](classQgsQuickMapCanvasMap.md#property-previewjobsquadrants-12)  <br> |
| property double | [**quality**](classQgsQuickMapCanvasMap.md#property-quality-12)  <br> |
| property double | [**rightMargin**](classQgsQuickMapCanvasMap.md#property-rightmargin-12)  <br> |
| property bool | [**smooth**](classQgsQuickMapCanvasMap.md#property-smooth-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**bottomMarginChanged**](classQgsQuickMapCanvasMap.md#signal-bottommarginchanged)  <br> |
| signal void | [**forceDeferredLayersRepaintChanged**](classQgsQuickMapCanvasMap.md#signal-forcedeferredlayersrepaintchanged)  <br> |
| signal void | [**freezeChanged**](classQgsQuickMapCanvasMap.md#signal-freezechanged)  <br> |
| signal void | [**incrementalRenderingChanged**](classQgsQuickMapCanvasMap.md#signal-incrementalrenderingchanged)  <br> |
| signal void | [**isRenderingChanged**](classQgsQuickMapCanvasMap.md#signal-isrenderingchanged)  <br> |
| signal void | [**mapCanvasRefreshed**](classQgsQuickMapCanvasMap.md#signal-mapcanvasrefreshed)  <br> |
| signal void | [**mapUpdateIntervalChanged**](classQgsQuickMapCanvasMap.md#signal-mapupdateintervalchanged)  <br> |
| signal void | [**previewJobsEnabledChanged**](classQgsQuickMapCanvasMap.md#signal-previewjobsenabledchanged)  <br> |
| signal void | [**previewJobsQuadrantsChanged**](classQgsQuickMapCanvasMap.md#signal-previewjobsquadrantschanged)  <br> |
| signal void | [**qualityChanged**](classQgsQuickMapCanvasMap.md#signal-qualitychanged)  <br> |
| signal void | [**renderStarting**](classQgsQuickMapCanvasMap.md#signal-renderstarting)  <br> |
| signal void | [**rightMarginChanged**](classQgsQuickMapCanvasMap.md#signal-rightmarginchanged)  <br> |
| signal void | [**smoothChanged**](classQgsQuickMapCanvasMap.md#signal-smoothchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsQuickMapCanvasMap**](#function-qgsquickmapcanvasmap) (QQuickItem \* parent=nullptr) <br>_Create map canvas map._  |
|  double | [**bottomMargin**](#function-bottommargin-22) () const<br> |
|  bool | [**forceDeferredLayersRepaint**](#function-forcedeferredlayersrepaint-22) () const<br> |
|  bool | [**freeze**](#function-freeze-22) () const<br> |
|  QImage | [**image**](#function-image) () const<br> |
|  bool | [**incrementalRendering**](#function-incrementalrendering-22) () const<br> |
|  bool | [**isRendering**](#function-isrendering-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  int | [**mapUpdateInterval**](#function-mapupdateinterval-22) () const<br> |
|  bool | [**previewJobsEnabled**](#function-previewjobsenabled-22) () const<br> |
|  QList&lt; int &gt; | [**previewJobsQuadrants**](#function-previewjobsquadrants-22) () const<br> |
|  double | [**quality**](#function-quality-22) () const<br> |
|  double | [**rightMargin**](#function-rightmargin-22) () const<br> |
|  void | [**setBottomMargin**](#function-setbottommargin) (double bottomMargin) <br> |
|  void | [**setForceDeferredLayersRepaint**](#function-setforcedeferredlayersrepaint) (bool deferred) <br> |
|  void | [**setFreeze**](#function-setfreeze) (bool freeze) <br> |
|  void | [**setIncrementalRendering**](#function-setincrementalrendering) (bool incrementalRendering) <br> |
|  void | [**setMapUpdateInterval**](#function-setmapupdateinterval) (int mapUpdateInterval) <br> |
|  void | [**setPreviewJobsEnabled**](#function-setpreviewjobsenabled) (bool enabled) <br> |
|  void | [**setPreviewJobsQuadrants**](#function-setpreviewjobsquadrants) (const QList&lt; int &gt; & quadrants) <br> |
|  void | [**setQuality**](#function-setquality) (double quality) <br> |
|  void | [**setRightMargin**](#function-setrightmargin) (double rightMargin) <br> |
|  void | [**setSmooth**](#function-setsmooth) (bool smooth) <br> |
|  bool | [**smooth**](#function-smooth-22) () const<br> |
|  QSGNode \* | [**updatePaintNode**](#function-updatepaintnode) (QSGNode \* oldNode, QQuickItem::UpdatePaintNodeData \*) override<br> |
|   | [**~QgsQuickMapCanvasMap**](#function-qgsquickmapcanvasmap) () <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**geometryChange**](#function-geometrychange) (const QRectF & newGeometry, const QRectF & oldGeometry) override<br> |




## Detailed Description


This class implements a visual Qt Quick Item that does map rendering according to the current map settings. Client code is expected to use [**MapCanvas**](classMapCanvas.md) item rather than using this class directly.


[**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) instance internally creates [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) in constructor. The QgsProject should be attached to the [**QgsQuickMapSettings**](classQgsQuickMapSettings.md). The map settings for other QgsQuick components should be initialized from [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md)'s mapSettings




**Note:**

QML Type: MapCanvasMap 





    
## Public Properties Documentation




### property bottomMargin [1/2]

```C++
double QgsQuickMapCanvasMap::bottomMargin;
```



The bottom margin used by the map settings when calculating map extent or center. 


        

<hr>



### property forceDeferredLayersRepaint [1/2]

```C++
double QgsQuickMapCanvasMap::forceDeferredLayersRepaint;
```



When the forceDeferredLayersRepaint property is set to true, all layer repaint signals will be deferred. 


        

<hr>



### property freeze [1/2]

```C++
bool QgsQuickMapCanvasMap::freeze;
```



When freeze property is set to true, the map canvas does not refresh. The value temporary changes during the rendering process. 


        

<hr>



### property incrementalRendering [1/2]

```C++
bool QgsQuickMapCanvasMap::incrementalRendering;
```



When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed. 


        

<hr>



### property isRendering [1/2]

```C++
bool QgsQuickMapCanvasMap::isRendering;
```



The isRendering property is set to true while a rendering job is pending for this map canvas map. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property. 


        

<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QgsQuickMapCanvasMap::mapSettings;
```



The mapSettings property contains configuration for rendering of the map.


It should be used as a primary source of map settings (and project) for all other components in the application.


This is a readonly property. 


        

<hr>



### property mapUpdateInterval [1/2]

```C++
int QgsQuickMapCanvasMap::mapUpdateInterval;
```



Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing. This only has an effect if incrementalRendering is activated. Default is 250 [ms]. 


        

<hr>



### property previewJobsEnabled [1/2]

```C++
bool QgsQuickMapCanvasMap::previewJobsEnabled;
```



When the previewJobsEnabled property is set to true, canvas map preview jobs (low priority render jobs which render portions of the view just outside of the canvas extent, to allow preview of these out-of-canvas areas when panning or zooming out the map) while be rendered. 


        

<hr>



### property previewJobsQuadrants [1/2]

```C++
QList<int> QgsQuickMapCanvasMap::previewJobsQuadrants;
```



The previewJobsQuadrants property is used to customize the preview jobs ordering. The possible quadrant integer values are:


0 (top left) \| 1 (top) \| 2 (top right) 3 (left) \| canvas \| 5 (right) 6 (bottom left) \| 7 (bottom) \| 8 (bottom right) 


        

<hr>



### property quality [1/2]

```C++
double QgsQuickMapCanvasMap::quality;
```



The quality property allows for an increase in rendering speed and memory usage reduction at the cost of rendering quality.


By default, the value is set to 1.0, providing for the best rendering. The lowest quality value is 0.5. 


        

<hr>



### property rightMargin [1/2]

```C++
double QgsQuickMapCanvasMap::rightMargin;
```



The right margin used by the map settings when calculating map extent or center. 


        

<hr>



### property smooth [1/2]

```C++
bool QgsQuickMapCanvasMap::smooth;
```



When the smooth property is set to true, canvas map panning, zooming, and rotation changes while be interpolated to create a smoother transition. 


        

<hr>
## Public Slots Documentation




### slot pan 

```C++
void QgsQuickMapCanvasMap::pan;
```



Set map setting's extent (pan the map) based on the difference of positions 


        

<hr>



### slot refresh 

```C++
void QgsQuickMapCanvasMap::refresh;
```



Refresh the map canvas. Does nothing when output size of map settings is not set 


        

<hr>



### slot rotate 

```C++
void QgsQuickMapCanvasMap::rotate;
```



Change the map rotation by _degrees_. 


        

<hr>



### slot schedulePreviewJob 

```C++
void QgsQuickMapCanvasMap::schedulePreviewJob;
```




<hr>



### slot startPreviewJobs 

```C++
void QgsQuickMapCanvasMap::startPreviewJobs;
```




<hr>



### slot stopPreviewJobs 

```C++
void QgsQuickMapCanvasMap::stopPreviewJobs;
```




<hr>



### slot stopRendering 

_Stop map rendering._ 
```C++
void QgsQuickMapCanvasMap::stopRendering;
```




<hr>



### slot zoomByFactor 

```C++
void QgsQuickMapCanvasMap::zoomByFactor;
```



Set map setting's extent (zoom the map) on the _center_ by given _factor_ 


        

<hr>



### slot zoomScale 

```C++
void QgsQuickMapCanvasMap::zoomScale;
```



Set map setting's extent (zoom the map) on the _center_ at a given _scale_ 


        

<hr>
## Public Signals Documentation




### signal bottomMarginChanged 

```C++
void QgsQuickMapCanvasMap::bottomMarginChanged;
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### signal forceDeferredLayersRepaintChanged 

```C++
void QgsQuickMapCanvasMap::forceDeferredLayersRepaintChanged;
```



When the forceDeferredLayersRepaint property is set to true, all layer repaint signals will be deferred.   


        

<hr>



### signal freezeChanged 

```C++
void QgsQuickMapCanvasMap::freezeChanged;
```



When freeze property is set to true, the map canvas does not refresh. The value temporary changes during the rendering process.   


        

<hr>



### signal incrementalRenderingChanged 

```C++
void QgsQuickMapCanvasMap::incrementalRenderingChanged;
```



When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed.   


        

<hr>



### signal isRenderingChanged 

```C++
void QgsQuickMapCanvasMap::isRenderingChanged;
```



The isRendering property is set to true while a rendering job is pending for this map canvas map. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property.   


        

<hr>



### signal mapCanvasRefreshed 

```C++
void QgsQuickMapCanvasMap::mapCanvasRefreshed;
```



Signal is emitted when a canvas is refreshed 


        

<hr>



### signal mapUpdateIntervalChanged 

```C++
void QgsQuickMapCanvasMap::mapUpdateIntervalChanged;
```



Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing. This only has an effect if incrementalRendering is activated. Default is 250 [ms].   


        

<hr>



### signal previewJobsEnabledChanged 

```C++
void QgsQuickMapCanvasMap::previewJobsEnabledChanged;
```



When the previewJobsEnabled property is set to true, canvas map preview jobs (low priority render jobs which render portions of the view just outside of the canvas extent, to allow preview of these out-of-canvas areas when panning or zooming out the map) while be rendered.   


        

<hr>



### signal previewJobsQuadrantsChanged 

```C++
void QgsQuickMapCanvasMap::previewJobsQuadrantsChanged;
```



The previewJobsQuadrants property is used to customize the preview jobs ordering. The possible quadrant integer values are:


0 (top left) \| 1 (top) \| 2 (top right) 3 (left) \| canvas \| 5 (right) 6 (bottom left) \| 7 (bottom) \| 8 (bottom right)   


        

<hr>



### signal qualityChanged 

```C++
void QgsQuickMapCanvasMap::qualityChanged;
```



The quality property allows for an increase in rendering speed and memory usage reduction at the cost of rendering quality.


By default, the value is set to 1.0, providing for the best rendering. The lowest quality value is 0.5.   


        

<hr>



### signal renderStarting 

```C++
void QgsQuickMapCanvasMap::renderStarting;
```



Signal is emitted when a rendering is starting 


        

<hr>



### signal rightMarginChanged 

```C++
void QgsQuickMapCanvasMap::rightMarginChanged;
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### signal smoothChanged 

```C++
void QgsQuickMapCanvasMap::smoothChanged;
```



The previewJobsQuadrants property is used to customize the preview jobs ordering. The possible quadrant integer values are:


0 (top left) \| 1 (top) \| 2 (top right) 3 (left) \| canvas \| 5 (right) 6 (bottom left) \| 7 (bottom) \| 8 (bottom right)   


        

<hr>
## Public Functions Documentation




### function QgsQuickMapCanvasMap 

_Create map canvas map._ 
```C++
explicit QgsQuickMapCanvasMap::QgsQuickMapCanvasMap (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function bottomMargin [2/2]

```C++
double QgsQuickMapCanvasMap::bottomMargin () const
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### function forceDeferredLayersRepaint [2/2]

```C++
bool QgsQuickMapCanvasMap::forceDeferredLayersRepaint () const
```



When the forceDeferredLayersRepaint property is set to true, all layer repaint signals will be deferred.   


        

<hr>



### function freeze [2/2]

```C++
bool QgsQuickMapCanvasMap::freeze () const
```



When freeze property is set to true, the map canvas does not refresh. The value temporary changes during the rendering process.   


        

<hr>



### function image 

```C++
inline QImage QgsQuickMapCanvasMap::image () const
```



Returns an image of the last successful map canvas rendering 


        

<hr>



### function incrementalRendering [2/2]

```C++
bool QgsQuickMapCanvasMap::incrementalRendering () const
```



When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed.   


        

<hr>



### function isRendering [2/2]

```C++
bool QgsQuickMapCanvasMap::isRendering () const
```



The isRendering property is set to true while a rendering job is pending for this map canvas map. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property.   


        

<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QgsQuickMapCanvasMap::mapSettings () const
```



The mapSettings property contains configuration for rendering of the map.


It should be used as a primary source of map settings (and project) for all other components in the application.


This is a readonly property.   


        

<hr>



### function mapUpdateInterval [2/2]

```C++
int QgsQuickMapCanvasMap::mapUpdateInterval () const
```



Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing. This only has an effect if incrementalRendering is activated. Default is 250 [ms].   


        

<hr>



### function previewJobsEnabled [2/2]

```C++
bool QgsQuickMapCanvasMap::previewJobsEnabled () const
```



When the previewJobsEnabled property is set to true, canvas map preview jobs (low priority render jobs which render portions of the view just outside of the canvas extent, to allow preview of these out-of-canvas areas when panning or zooming out the map) while be rendered.   


        

<hr>



### function previewJobsQuadrants [2/2]

```C++
QList< int > QgsQuickMapCanvasMap::previewJobsQuadrants () const
```



The previewJobsQuadrants property is used to customize the preview jobs ordering. The possible quadrant integer values are:


0 (top left) \| 1 (top) \| 2 (top right) 3 (left) \| canvas \| 5 (right) 6 (bottom left) \| 7 (bottom) \| 8 (bottom right)   


        

<hr>



### function quality [2/2]

```C++
double QgsQuickMapCanvasMap::quality () const
```



The quality property allows for an increase in rendering speed and memory usage reduction at the cost of rendering quality.


By default, the value is set to 1.0, providing for the best rendering. The lowest quality value is 0.5.   


        

<hr>



### function rightMargin [2/2]

```C++
double QgsQuickMapCanvasMap::rightMargin () const
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### function setBottomMargin 

```C++
void QgsQuickMapCanvasMap::setBottomMargin (
    double bottomMargin
) 
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### function setForceDeferredLayersRepaint 

```C++
void QgsQuickMapCanvasMap::setForceDeferredLayersRepaint (
    bool deferred
) 
```



When the forceDeferredLayersRepaint property is set to true, all layer repaint signals will be deferred.   


        

<hr>



### function setFreeze 

```C++
void QgsQuickMapCanvasMap::setFreeze (
    bool freeze
) 
```



When freeze property is set to true, the map canvas does not refresh. The value temporary changes during the rendering process.   


        

<hr>



### function setIncrementalRendering 

```C++
void QgsQuickMapCanvasMap::setIncrementalRendering (
    bool incrementalRendering
) 
```



When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed.   


        

<hr>



### function setMapUpdateInterval 

```C++
void QgsQuickMapCanvasMap::setMapUpdateInterval (
    int mapUpdateInterval
) 
```



Interval in milliseconds after which the map canvas will be updated while a rendering job is ongoing. This only has an effect if incrementalRendering is activated. Default is 250 [ms].   


        

<hr>



### function setPreviewJobsEnabled 

```C++
void QgsQuickMapCanvasMap::setPreviewJobsEnabled (
    bool enabled
) 
```



When the previewJobsEnabled property is set to true, canvas map preview jobs (low priority render jobs which render portions of the view just outside of the canvas extent, to allow preview of these out-of-canvas areas when panning or zooming out the map) while be rendered.   


        

<hr>



### function setPreviewJobsQuadrants 

```C++
void QgsQuickMapCanvasMap::setPreviewJobsQuadrants (
    const QList< int > & quadrants
) 
```



The previewJobsQuadrants property is used to customize the preview jobs ordering. The possible quadrant integer values are:


0 (top left) \| 1 (top) \| 2 (top right) 3 (left) \| canvas \| 5 (right) 6 (bottom left) \| 7 (bottom) \| 8 (bottom right)   


        

<hr>



### function setQuality 

```C++
void QgsQuickMapCanvasMap::setQuality (
    double quality
) 
```



When the incrementalRendering property is set to true, the automatic refresh of map canvas during rendering is allowed.   


        

<hr>



### function setRightMargin 

```C++
void QgsQuickMapCanvasMap::setRightMargin (
    double rightMargin
) 
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### function setSmooth 

```C++
void QgsQuickMapCanvasMap::setSmooth (
    bool smooth
) 
```



When the smooth property is set to true, canvas map panning, zooming, and rotation changes while be interpolated to create a smoother transition.   


        

<hr>



### function smooth [2/2]

```C++
bool QgsQuickMapCanvasMap::smooth () const
```



When the smooth property is set to true, canvas map panning, zooming, and rotation changes while be interpolated to create a smoother transition.   


        

<hr>



### function updatePaintNode 

```C++
QSGNode * QgsQuickMapCanvasMap::updatePaintNode (
    QSGNode * oldNode,
    QQuickItem::UpdatePaintNodeData *
) override
```




<hr>



### function ~QgsQuickMapCanvasMap 

```C++
QgsQuickMapCanvasMap::~QgsQuickMapCanvasMap () 
```




<hr>
## Protected Functions Documentation




### function geometryChange 

```C++
void QgsQuickMapCanvasMap::geometryChange (
    const QRectF & newGeometry,
    const QRectF & oldGeometry
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickmapcanvasmap.h`

