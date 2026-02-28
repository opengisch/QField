

# Class SnappingUtils



[**ClassList**](annotated.md) **>** [**SnappingUtils**](classSnappingUtils.md)








Inherits the following classes: QgsSnappingUtils


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**currentLayer**](classSnappingUtils.md#property-currentlayer-12)  <br> |
| property bool | [**enabled**](classSnappingUtils.md#property-enabled-12)  <br> |
| property QPointF | [**inputCoordinate**](classSnappingUtils.md#property-inputcoordinate-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classSnappingUtils.md#property-mapsettings-12)  <br> |
| property [**SnappingResult**](classSnappingResult.md) | [**snappingResult**](classSnappingUtils.md#property-snappingresult-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentLayerChanged**](classSnappingUtils.md#signal-currentlayerchanged)  <br> |
| signal void | [**enabledChanged**](classSnappingUtils.md#signal-enabledchanged)  <br> |
| signal void | [**indexingFinished**](classSnappingUtils.md#signal-indexingfinished)  <br> |
| signal void | [**indexingProgress**](classSnappingUtils.md#signal-indexingprogress) (int index) <br> |
| signal void | [**indexingStarted**](classSnappingUtils.md#signal-indexingstarted) (int count) <br> |
| signal void | [**inputCoordinateChanged**](classSnappingUtils.md#signal-inputcoordinatechanged)  <br> |
| signal void | [**mapSettingsChanged**](classSnappingUtils.md#signal-mapsettingschanged)  <br> |
| signal void | [**snappingResultChanged**](classSnappingUtils.md#signal-snappingresultchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SnappingUtils**](#function-snappingutils) (QObject \* parent=nullptr) <br> |
|  QgsVectorLayer \* | [**currentLayer**](#function-currentlayer-22) () const<br> |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  QPointF | [**inputCoordinate**](#function-inputcoordinate-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setInputCoordinate**](#function-setinputcoordinate) (const QPointF & inputCoordinate) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* settings) <br> |
|  [**SnappingResult**](classSnappingResult.md) | [**snappingResult**](#function-snappingresult-22) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE QgsSnappingConfig | [**emptySnappingConfig**](#function-emptysnappingconfig) () <br> |
|  QgsPoint | [**newPoint**](#function-newpoint) (const QgsPoint & snappedPoint, const Qgis::WkbType wkbType) <br> |






















## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual void | [**prepareIndexProgress**](#function-prepareindexprogress) (int index) override<br> |
| virtual void | [**prepareIndexStarting**](#function-prepareindexstarting) (int count) override<br> |




## Public Properties Documentation




### property currentLayer [1/2]

```C++
QgsVectorLayer * SnappingUtils::currentLayer;
```




<hr>



### property enabled [1/2]

```C++
bool SnappingUtils::enabled;
```




<hr>



### property inputCoordinate [1/2]

```C++
QPointF SnappingUtils::inputCoordinate;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * SnappingUtils::mapSettings;
```




<hr>



### property snappingResult [1/2]

```C++
SnappingResult SnappingUtils::snappingResult;
```




<hr>
## Public Signals Documentation




### signal currentLayerChanged 

```C++
void SnappingUtils::currentLayerChanged;
```




<hr>



### signal enabledChanged 

```C++
void SnappingUtils::enabledChanged;
```




<hr>



### signal indexingFinished 

```C++
void SnappingUtils::indexingFinished;
```




<hr>



### signal indexingProgress 

```C++
void SnappingUtils::indexingProgress;
```




<hr>



### signal indexingStarted 

```C++
void SnappingUtils::indexingStarted;
```




<hr>



### signal inputCoordinateChanged 

```C++
void SnappingUtils::inputCoordinateChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void SnappingUtils::mapSettingsChanged;
```




<hr>



### signal snappingResultChanged 

```C++
void SnappingUtils::snappingResultChanged;
```




<hr>
## Public Functions Documentation




### function SnappingUtils 

```C++
explicit SnappingUtils::SnappingUtils (
    QObject * parent=nullptr
) 
```




<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * SnappingUtils::currentLayer () const
```




<hr>



### function enabled [2/2]

```C++
bool SnappingUtils::enabled () const
```




<hr>



### function inputCoordinate [2/2]

```C++
QPointF SnappingUtils::inputCoordinate () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * SnappingUtils::mapSettings () const
```




<hr>



### function setCurrentLayer 

```C++
void SnappingUtils::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setEnabled 

```C++
void SnappingUtils::setEnabled (
    bool enabled
) 
```




<hr>



### function setInputCoordinate 

```C++
void SnappingUtils::setInputCoordinate (
    const QPointF & inputCoordinate
) 
```




<hr>



### function setMapSettings 

```C++
void SnappingUtils::setMapSettings (
    QgsQuickMapSettings * settings
) 
```




<hr>



### function snappingResult [2/2]

```C++
SnappingResult SnappingUtils::snappingResult () const
```




<hr>
## Public Static Functions Documentation




### function emptySnappingConfig 

```C++
static inline Q_INVOKABLE QgsSnappingConfig SnappingUtils::emptySnappingConfig () 
```



Returns an empty snapping configuration object 

**Note:**

This can be used in QML to avoid errors when a parent object pointer goes undefined 





        

<hr>



### function newPoint 

```C++
static QgsPoint SnappingUtils::newPoint (
    const QgsPoint & snappedPoint,
    const Qgis::WkbType wkbType
) 
```




<hr>
## Protected Functions Documentation




### function prepareIndexProgress 

```C++
virtual void SnappingUtils::prepareIndexProgress (
    int index
) override
```




<hr>



### function prepareIndexStarting 

```C++
virtual void SnappingUtils::prepareIndexStarting (
    int count
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/snappingutils.h`

