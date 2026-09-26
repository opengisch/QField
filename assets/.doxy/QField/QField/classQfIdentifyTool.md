

# Class QfIdentifyTool



[**ClassList**](annotated.md) **>** [**QfIdentifyTool**](classQfIdentifyTool.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) <br> |














## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classQfIdentifyTool.md#property-enabled-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfIdentifyTool.md#property-mapsettings-12)  <br> |
| property [**QfMarkupManager**](classQfMarkupManager.md) \* | [**markups**](classQfIdentifyTool.md#property-markups)  <br> |
| property [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](classQfIdentifyTool.md#property-model-12)  <br> |
| property double | [**searchRadiusMm**](classQfIdentifyTool.md#property-searchradiusmm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfIdentifyTool.md#signal-enabledchanged)  <br> |
| signal void | [**identifyFinished**](classQfIdentifyTool.md#signal-identifyfinished)  <br> |
| signal void | [**mapSettingsChanged**](classQfIdentifyTool.md#signal-mapsettingschanged)  <br> |
| signal void | [**markupManagerChanged**](classQfIdentifyTool.md#signal-markupmanagerchanged)  <br> |
| signal void | [**modelChanged**](classQfIdentifyTool.md#signal-modelchanged)  <br> |
| signal void | [**searchRadiusMmChanged**](classQfIdentifyTool.md#signal-searchradiusmmchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfIdentifyTool**](#function-qfidentifytool) (QObject \* parent=nullptr) <br> |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  Q\_INVOKABLE void | [**identify**](#function-identify) (const QPointF & point) const<br> |
|  QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyRasterLayer**](#function-identifyrasterlayer) (QgsRasterLayer \* layer, const QgsPointXY & point) const<br> |
|  QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorLayer**](#function-identifyvectorlayer) (QgsVectorLayer \* layer, const QgsPointXY & point) const<br> |
|  QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorTileLayer**](#function-identifyvectortilelayer) (QgsVectorTileLayer \* layer, const QgsPointXY & point) const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**QfMarkupManager**](classQfMarkupManager.md) \* | [**markupManager**](#function-markupmanager) () const<br> |
|  [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br> |
|  double | [**searchRadiusMm**](#function-searchradiusmm-22) () const<br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setMarkupManager**](#function-setmarkupmanager) ([**QfMarkupManager**](classQfMarkupManager.md) \* markupManager) <br> |
|  void | [**setModel**](#function-setmodel) ([**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* model) <br> |
|  void | [**setSearchRadiusMm**](#function-setsearchradiusmm) (double searchRadiusMm) <br> |




























## Public Properties Documentation




### property enabled [1/2]

```C++
bool QfIdentifyTool::enabled;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfIdentifyTool::mapSettings;
```




<hr>



### property markups 

```C++
QfMarkupManager * QfIdentifyTool::markups;
```




<hr>



### property model [1/2]

```C++
QfMultiFeatureListModel * QfIdentifyTool::model;
```




<hr>



### property searchRadiusMm [1/2]

```C++
double QfIdentifyTool::searchRadiusMm;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void QfIdentifyTool::enabledChanged;
```




<hr>



### signal identifyFinished 

```C++
void QfIdentifyTool::identifyFinished;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfIdentifyTool::mapSettingsChanged;
```




<hr>



### signal markupManagerChanged 

```C++
void QfIdentifyTool::markupManagerChanged;
```




<hr>



### signal modelChanged 

```C++
void QfIdentifyTool::modelChanged;
```




<hr>



### signal searchRadiusMmChanged 

```C++
void QfIdentifyTool::searchRadiusMmChanged;
```




<hr>
## Public Functions Documentation




### function QfIdentifyTool 

```C++
explicit QfIdentifyTool::QfIdentifyTool (
    QObject * parent=nullptr
) 
```




<hr>



### function enabled [2/2]

```C++
inline bool QfIdentifyTool::enabled () const
```




<hr>



### function identify 

```C++
Q_INVOKABLE void QfIdentifyTool::identify (
    const QPointF & point
) const
```




<hr>



### function identifyRasterLayer 

```C++
QList< IdentifyResult > QfIdentifyTool::identifyRasterLayer (
    QgsRasterLayer * layer,
    const QgsPointXY & point
) const
```




<hr>



### function identifyVectorLayer 

```C++
QList< IdentifyResult > QfIdentifyTool::identifyVectorLayer (
    QgsVectorLayer * layer,
    const QgsPointXY & point
) const
```




<hr>



### function identifyVectorTileLayer 

```C++
QList< IdentifyResult > QfIdentifyTool::identifyVectorTileLayer (
    QgsVectorTileLayer * layer,
    const QgsPointXY & point
) const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfIdentifyTool::mapSettings () const
```




<hr>



### function markupManager 

```C++
QfMarkupManager * QfIdentifyTool::markupManager () const
```




<hr>



### function model [2/2]

```C++
QfMultiFeatureListModel * QfIdentifyTool::model () const
```




<hr>



### function searchRadiusMm [2/2]

```C++
double QfIdentifyTool::searchRadiusMm () const
```




<hr>



### function setEnabled 

```C++
void QfIdentifyTool::setEnabled (
    bool enabled
) 
```




<hr>



### function setMapSettings 

```C++
void QfIdentifyTool::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setMarkupManager 

```C++
void QfIdentifyTool::setMarkupManager (
    QfMarkupManager * markupManager
) 
```




<hr>



### function setModel 

```C++
void QfIdentifyTool::setModel (
    QfMultiFeatureListModel * model
) 
```




<hr>



### function setSearchRadiusMm 

```C++
void QfIdentifyTool::setSearchRadiusMm (
    double searchRadiusMm
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfidentifytool.h`

