

# Class QfIdentifyTool



[**ClassList**](annotated.md) **>** [**QfIdentifyTool**](classQfIdentifyTool.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) <br> |










## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**identify**](classQfIdentifyTool.md#slot-identify) (const QPointF & point) <br> |
| slot QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyRasterLayer**](classQfIdentifyTool.md#slot-identifyrasterlayer) (QgsRasterLayer \* layer, const QgsPointXY & point) <br> |
| slot QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorLayer**](classQfIdentifyTool.md#slot-identifyvectorlayer) (QgsVectorLayer \* layer, const QgsPointXY & point) <br> |
| slot QList&lt; [**IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorTileLayer**](classQfIdentifyTool.md#slot-identifyvectortilelayer) (QgsVectorTileLayer \* layer, const QgsPointXY & point) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**deactivated**](classQfIdentifyTool.md#property-deactivated-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfIdentifyTool.md#property-mapsettings-12)  <br> |
| property [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](classQfIdentifyTool.md#property-model-12)  <br> |
| property double | [**searchRadiusMm**](classQfIdentifyTool.md#property-searchradiusmm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deactivatedChanged**](classQfIdentifyTool.md#signal-deactivatedchanged)  <br> |
| signal void | [**identifyFinished**](classQfIdentifyTool.md#signal-identifyfinished)  <br> |
| signal void | [**mapSettingsChanged**](classQfIdentifyTool.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classQfIdentifyTool.md#signal-modelchanged)  <br> |
| signal void | [**searchRadiusMmChanged**](classQfIdentifyTool.md#signal-searchradiusmmchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfIdentifyTool**](#function-qfidentifytool) (QObject \* parent=nullptr) <br> |
|  bool | [**deactivated**](#function-deactivated-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br> |
|  double | [**searchRadiusMm**](#function-searchradiusmm-22) () const<br> |
|  void | [**setDeactivated**](#function-setdeactivated) (bool deactivated) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](#function-setmodel) ([**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* model) <br> |
|  void | [**setSearchRadiusMm**](#function-setsearchradiusmm) (double searchRadiusMm) <br> |




























## Public Properties Documentation




### property deactivated [1/2]

```C++
bool QfIdentifyTool::deactivated;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfIdentifyTool::mapSettings;
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
## Public Slots Documentation




### slot identify 

```C++
void QfIdentifyTool::identify;
```




<hr>



### slot identifyRasterLayer 

```C++
QList< QfIdentifyTool::IdentifyResult > QfIdentifyTool::identifyRasterLayer;
```




<hr>



### slot identifyVectorLayer 

```C++
QList< QfIdentifyTool::IdentifyResult > QfIdentifyTool::identifyVectorLayer;
```




<hr>



### slot identifyVectorTileLayer 

```C++
QList< QfIdentifyTool::IdentifyResult > QfIdentifyTool::identifyVectorTileLayer;
```




<hr>
## Public Signals Documentation




### signal deactivatedChanged 

```C++
void QfIdentifyTool::deactivatedChanged;
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



### function deactivated [2/2]

```C++
inline bool QfIdentifyTool::deactivated () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfIdentifyTool::mapSettings () const
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



### function setDeactivated 

```C++
void QfIdentifyTool::setDeactivated (
    bool deactivated
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

