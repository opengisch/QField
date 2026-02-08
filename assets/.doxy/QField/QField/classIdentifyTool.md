

# Class IdentifyTool



[**ClassList**](annotated.md) **>** [**IdentifyTool**](classIdentifyTool.md)








Inherits the following classes: QObject












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**IdentifyResult**](structIdentifyTool_1_1IdentifyResult.md) <br> |










## Public Slots

| Type | Name |
| ---: | :--- |
| slot void | [**identify**](classIdentifyTool.md#slot-identify) (const QPointF & point) <br> |
| slot QList&lt; [**IdentifyResult**](structIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyRasterLayer**](classIdentifyTool.md#slot-identifyrasterlayer) (QgsRasterLayer \* layer, const QgsPointXY & point) <br> |
| slot QList&lt; [**IdentifyResult**](structIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorLayer**](classIdentifyTool.md#slot-identifyvectorlayer) (QgsVectorLayer \* layer, const QgsPointXY & point) <br> |
| slot QList&lt; [**IdentifyResult**](structIdentifyTool_1_1IdentifyResult.md) &gt; | [**identifyVectorTileLayer**](classIdentifyTool.md#slot-identifyvectortilelayer) (QgsVectorTileLayer \* layer, const QgsPointXY & point) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**deactivated**](classIdentifyTool.md#property-deactivated-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classIdentifyTool.md#property-mapsettings-12)  <br> |
| property [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](classIdentifyTool.md#property-model-12)  <br> |
| property double | [**searchRadiusMm**](classIdentifyTool.md#property-searchradiusmm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deactivatedChanged**](classIdentifyTool.md#signal-deactivatedchanged)  <br> |
| signal void | [**identifyFinished**](classIdentifyTool.md#signal-identifyfinished)  <br> |
| signal void | [**mapSettingsChanged**](classIdentifyTool.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classIdentifyTool.md#signal-modelchanged)  <br> |
| signal void | [**searchRadiusMmChanged**](classIdentifyTool.md#signal-searchradiusmmchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IdentifyTool**](#function-identifytool) (QObject \* parent=nullptr) <br> |
|  bool | [**deactivated**](#function-deactivated-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br> |
|  double | [**searchRadiusMm**](#function-searchradiusmm-22) () const<br> |
|  void | [**setDeactivated**](#function-setdeactivated) (bool deactivated) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](#function-setmodel) ([**MultiFeatureListModel**](classMultiFeatureListModel.md) \* model) <br> |
|  void | [**setSearchRadiusMm**](#function-setsearchradiusmm) (double searchRadiusMm) <br> |




























## Public Properties Documentation




### property deactivated [1/2]

```C++
bool IdentifyTool::deactivated;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * IdentifyTool::mapSettings;
```




<hr>



### property model [1/2]

```C++
MultiFeatureListModel * IdentifyTool::model;
```




<hr>



### property searchRadiusMm [1/2]

```C++
double IdentifyTool::searchRadiusMm;
```




<hr>
## Public Slots Documentation




### slot identify 

```C++
void IdentifyTool::identify;
```




<hr>



### slot identifyRasterLayer 

```C++
QList< IdentifyTool::IdentifyResult > IdentifyTool::identifyRasterLayer;
```




<hr>



### slot identifyVectorLayer 

```C++
QList< IdentifyTool::IdentifyResult > IdentifyTool::identifyVectorLayer;
```




<hr>



### slot identifyVectorTileLayer 

```C++
QList< IdentifyTool::IdentifyResult > IdentifyTool::identifyVectorTileLayer;
```




<hr>
## Public Signals Documentation




### signal deactivatedChanged 

```C++
void IdentifyTool::deactivatedChanged;
```




<hr>



### signal identifyFinished 

```C++
void IdentifyTool::identifyFinished;
```




<hr>



### signal mapSettingsChanged 

```C++
void IdentifyTool::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void IdentifyTool::modelChanged;
```




<hr>



### signal searchRadiusMmChanged 

```C++
void IdentifyTool::searchRadiusMmChanged;
```




<hr>
## Public Functions Documentation




### function IdentifyTool 

```C++
explicit IdentifyTool::IdentifyTool (
    QObject * parent=nullptr
) 
```




<hr>



### function deactivated [2/2]

```C++
inline bool IdentifyTool::deactivated () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * IdentifyTool::mapSettings () const
```




<hr>



### function model [2/2]

```C++
MultiFeatureListModel * IdentifyTool::model () const
```




<hr>



### function searchRadiusMm [2/2]

```C++
double IdentifyTool::searchRadiusMm () const
```




<hr>



### function setDeactivated 

```C++
void IdentifyTool::setDeactivated (
    bool deactivated
) 
```




<hr>



### function setMapSettings 

```C++
void IdentifyTool::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setModel 

```C++
void IdentifyTool::setModel (
    MultiFeatureListModel * model
) 
```




<hr>



### function setSearchRadiusMm 

```C++
void IdentifyTool::setSearchRadiusMm (
    double searchRadiusMm
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/identifytool.h`

