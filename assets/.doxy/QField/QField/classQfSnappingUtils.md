

# Class QfSnappingUtils



[**ClassList**](annotated.md) **>** [**QfSnappingUtils**](classQfSnappingUtils.md)








Inherits the following classes: QgsSnappingUtils


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**currentLayer**](classQfSnappingUtils.md#property-currentlayer-12)  <br> |
| property bool | [**enabled**](classQfSnappingUtils.md#property-enabled-12)  <br> |
| property QPointF | [**inputCoordinate**](classQfSnappingUtils.md#property-inputcoordinate-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfSnappingUtils.md#property-mapsettings-12)  <br> |
| property [**QfSnappingResult**](classQfSnappingResult.md) | [**snappingResult**](classQfSnappingUtils.md#property-snappingresult-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentLayerChanged**](classQfSnappingUtils.md#signal-currentlayerchanged)  <br> |
| signal void | [**enabledChanged**](classQfSnappingUtils.md#signal-enabledchanged)  <br> |
| signal void | [**indexingFinished**](classQfSnappingUtils.md#signal-indexingfinished)  <br> |
| signal void | [**indexingProgress**](classQfSnappingUtils.md#signal-indexingprogress) (int index) <br> |
| signal void | [**indexingStarted**](classQfSnappingUtils.md#signal-indexingstarted) (int count) <br> |
| signal void | [**inputCoordinateChanged**](classQfSnappingUtils.md#signal-inputcoordinatechanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfSnappingUtils.md#signal-mapsettingschanged)  <br> |
| signal void | [**snappingResultChanged**](classQfSnappingUtils.md#signal-snappingresultchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSnappingUtils**](#function-qfsnappingutils) (QObject \* parent=nullptr) <br> |
|  QgsVectorLayer \* | [**currentLayer**](#function-currentlayer-22) () const<br> |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  QPointF | [**inputCoordinate**](#function-inputcoordinate-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  void | [**setCurrentLayer**](#function-setcurrentlayer) (QgsVectorLayer \* currentLayer) <br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setInputCoordinate**](#function-setinputcoordinate) (const QPointF & inputCoordinate) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* settings) <br> |
|  [**QfSnappingResult**](classQfSnappingResult.md) | [**snappingResult**](#function-snappingresult-22) () const<br> |


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
QgsVectorLayer * QfSnappingUtils::currentLayer;
```




<hr>



### property enabled [1/2]

```C++
bool QfSnappingUtils::enabled;
```




<hr>



### property inputCoordinate [1/2]

```C++
QPointF QfSnappingUtils::inputCoordinate;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfSnappingUtils::mapSettings;
```




<hr>



### property snappingResult [1/2]

```C++
QfSnappingResult QfSnappingUtils::snappingResult;
```




<hr>
## Public Signals Documentation




### signal currentLayerChanged 

```C++
void QfSnappingUtils::currentLayerChanged;
```




<hr>



### signal enabledChanged 

```C++
void QfSnappingUtils::enabledChanged;
```




<hr>



### signal indexingFinished 

```C++
void QfSnappingUtils::indexingFinished;
```




<hr>



### signal indexingProgress 

```C++
void QfSnappingUtils::indexingProgress;
```




<hr>



### signal indexingStarted 

```C++
void QfSnappingUtils::indexingStarted;
```




<hr>



### signal inputCoordinateChanged 

```C++
void QfSnappingUtils::inputCoordinateChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfSnappingUtils::mapSettingsChanged;
```




<hr>



### signal snappingResultChanged 

```C++
void QfSnappingUtils::snappingResultChanged;
```




<hr>
## Public Functions Documentation




### function QfSnappingUtils 

```C++
explicit QfSnappingUtils::QfSnappingUtils (
    QObject * parent=nullptr
) 
```




<hr>



### function currentLayer [2/2]

```C++
QgsVectorLayer * QfSnappingUtils::currentLayer () const
```




<hr>



### function enabled [2/2]

```C++
bool QfSnappingUtils::enabled () const
```




<hr>



### function inputCoordinate [2/2]

```C++
QPointF QfSnappingUtils::inputCoordinate () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfSnappingUtils::mapSettings () const
```




<hr>



### function setCurrentLayer 

```C++
void QfSnappingUtils::setCurrentLayer (
    QgsVectorLayer * currentLayer
) 
```




<hr>



### function setEnabled 

```C++
void QfSnappingUtils::setEnabled (
    bool enabled
) 
```




<hr>



### function setInputCoordinate 

```C++
void QfSnappingUtils::setInputCoordinate (
    const QPointF & inputCoordinate
) 
```




<hr>



### function setMapSettings 

```C++
void QfSnappingUtils::setMapSettings (
    QgsQuickMapSettings * settings
) 
```




<hr>



### function snappingResult [2/2]

```C++
QfSnappingResult QfSnappingUtils::snappingResult () const
```




<hr>
## Public Static Functions Documentation




### function emptySnappingConfig 

```C++
static inline Q_INVOKABLE QgsSnappingConfig QfSnappingUtils::emptySnappingConfig () 
```



Returns an empty snapping configuration object 

**Note:**

This can be used in QML to avoid errors when a parent object pointer goes undefined 





        

<hr>



### function newPoint 

```C++
static QgsPoint QfSnappingUtils::newPoint (
    const QgsPoint & snappedPoint,
    const Qgis::WkbType wkbType
) 
```




<hr>
## Protected Functions Documentation




### function prepareIndexProgress 

```C++
virtual void QfSnappingUtils::prepareIndexProgress (
    int index
) override
```




<hr>



### function prepareIndexStarting 

```C++
virtual void QfSnappingUtils::prepareIndexStarting (
    int count
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfsnappingutils.h`

