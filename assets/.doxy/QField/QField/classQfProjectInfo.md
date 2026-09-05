

# Class QfProjectInfo



[**ClassList**](annotated.md) **>** [**QfProjectInfo**](classQfProjectInfo.md)



[More...](#detailed-description)

* `#include <qfprojectinfo.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsMapLayer \* | [**activeLayer**](classQfProjectInfo.md#property-activelayer-12)  <br> |
| property [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**cloudUserInformation**](classQfProjectInfo.md#property-clouduserinformation-12)  <br> |
| property QString | [**filePath**](classQfProjectInfo.md#property-filepath-12)  <br> |
| property [**QfFlatLayerTreeModel**](classQfFlatLayerTreeModel.md) \* | [**layerTree**](classQfProjectInfo.md#property-layertree-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfProjectInfo.md#property-mapsettings-12)  <br> |
| property bool | [**snappingEnabled**](classQfProjectInfo.md#property-snappingenabled-12)  <br> |
| property QString | [**stateMode**](classQfProjectInfo.md#property-statemode-12)  <br> |
| property [**QfTrackingModel**](classQfTrackingModel.md) \* | [**trackingModel**](classQfProjectInfo.md#property-trackingmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeLayerChanged**](classQfProjectInfo.md#signal-activelayerchanged)  <br> |
| signal void | [**cloudUserInformationChanged**](classQfProjectInfo.md#signal-clouduserinformationchanged)  <br> |
| signal void | [**filePathChanged**](classQfProjectInfo.md#signal-filepathchanged)  <br> |
| signal void | [**layerTreeChanged**](classQfProjectInfo.md#signal-layertreechanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfProjectInfo.md#signal-mapsettingschanged)  <br> |
| signal void | [**snappingEnabledChanged**](classQfProjectInfo.md#signal-snappingenabledchanged)  <br> |
| signal void | [**stateModeChanged**](classQfProjectInfo.md#signal-statemodechanged)  <br> |
| signal void | [**trackingModelChanged**](classQfProjectInfo.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProjectInfo**](#function-qfprojectinfo) (QObject \* parent=nullptr) <br> |
|  QgsMapLayer \* | [**activeLayer**](#function-activelayer-22) () const<br> |
|  [**QfCloudUserInformation**](structQfCloudUserInformation.md) | [**cloudUserInformation**](#function-clouduserinformation-22) () const<br> |
|  QString | [**filePath**](#function-filepath-22) () const<br> |
|  Q\_INVOKABLE QVariantMap | [**getCopyrightDecorationConfiguration**](#function-getcopyrightdecorationconfiguration) () <br>_Retrieves configuration of the copyright decoration._  |
|  Q\_INVOKABLE QgsMapLayer \* | [**getDefaultActiveLayerForMapTheme**](#function-getdefaultactivelayerformaptheme) (const QString & mapTheme) <br>_Retrieves the default active layer for a given map theme._  |
|  Q\_INVOKABLE QVariantMap | [**getGridDecorationConfiguration**](#function-getgriddecorationconfiguration) () <br>_Retrieves configuration of the grid decoration._  |
|  Q\_INVOKABLE QVariantMap | [**getImageDecorationConfiguration**](#function-getimagedecorationconfiguration) () <br>_Retrieves configuration of the image decoration._  |
|  Q\_INVOKABLE QVariantMap | [**getTitleDecorationConfiguration**](#function-gettitledecorationconfiguration) () <br>_Retrieves configuration of the title decoration._  |
|  [**QfFlatLayerTreeModel**](classQfFlatLayerTreeModel.md) \* | [**layerTree**](#function-layertree-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  Q\_INVOKABLE void | [**restoreCloudUserInformation**](#function-restoreclouduserinformation) () <br> |
|  Q\_INVOKABLE QModelIndex | [**restoreTracker**](#function-restoretracker) (QgsVectorLayer \* layer) <br>_Restore the last tracking session that occured within a vector_ _layer_ _._ |
|  Q\_INVOKABLE void | [**saveLayerRememberedFields**](#function-savelayerrememberedfields) (QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE void | [**saveLayerSnappingConfiguration**](#function-savelayersnappingconfiguration) (QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE void | [**saveLayerStyle**](#function-savelayerstyle) (QgsMapLayer \* layer) <br> |
|  Q\_INVOKABLE void | [**saveLayerTreeState**](#function-savelayertreestate) () <br> |
|  Q\_INVOKABLE void | [**saveTracker**](#function-savetracker) (QgsVectorLayer \* layer) <br>_Save an ongoing vector_ _layer_ _tracking session details._ |
|  Q\_INVOKABLE void | [**saveVariable**](#function-savevariable) (const QString & name, const QString & value) <br> |
|  void | [**setActiveLayer**](#function-setactivelayer) (QgsMapLayer \* layer) <br> |
|  void | [**setCloudUserInformation**](#function-setclouduserinformation) (const [**QfCloudUserInformation**](structQfCloudUserInformation.md) & cloudUserInformation) <br> |
|  void | [**setFilePath**](#function-setfilepath) (const QString & filePath) <br> |
|  void | [**setLayerTree**](#function-setlayertree) ([**QfFlatLayerTreeModel**](classQfFlatLayerTreeModel.md) \* layerTree) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setSnappingEnabled**](#function-setsnappingenabled) (bool enabled) <br> |
|  void | [**setStateMode**](#function-setstatemode) (const QString & mode) <br> |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**QfTrackingModel**](classQfTrackingModel.md) \* trackingModel) <br> |
|  bool | [**snappingEnabled**](#function-snappingenabled-22) () const<br> |
|  QString | [**stateMode**](#function-statemode-22) () const<br> |
|  [**QfTrackingModel**](classQfTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel-22) () const<br> |
| virtual  | [**~QfProjectInfo**](#function-qfprojectinfo) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**restoreSettings**](#function-restoresettings) (QString & projectFilePath, QgsProject \* project, [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) \* mapCanvas, [**QfFlatLayerTreeModel**](classQfFlatLayerTreeModel.md) \* layerTree) <br>_Restore various project settings._  |


























## Detailed Description


This class stores current projection information such as extent and map theme in order for those to be saved and afterwards restored when the project is re-opened. 


    
## Public Properties Documentation




### property activeLayer [1/2]

```C++
QgsMapLayer * QfProjectInfo::activeLayer;
```



The active layer for the currently opened project. 


        

<hr>



### property cloudUserInformation [1/2]

```C++
QfCloudUserInformation QfProjectInfo::cloudUserInformation;
```



Set cloud user information for offline usage. 


        

<hr>



### property filePath [1/2]

```C++
QString QfProjectInfo::filePath;
```



The file path associated with the currently opened project. 


        

<hr>



### property layerTree [1/2]

```C++
QfFlatLayerTreeModel * QfProjectInfo::layerTree;
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc. 


        

<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfProjectInfo::mapSettings;
```



The map settings object, used to keep track of details such as the map extent. 


        

<hr>



### property snappingEnabled [1/2]

```C++
bool QfProjectInfo::snappingEnabled;
```



The snapping enabled state for the currently opened project. 


        

<hr>



### property stateMode [1/2]

```C++
QString QfProjectInfo::stateMode;
```



The state mode (browse vs. digitizing) for the currently opened project. 


        

<hr>



### property trackingModel [1/2]

```C++
QfTrackingModel * QfProjectInfo::trackingModel;
```



The tracking model object, used to save and restore tracking session for individual vector layers. 


        

<hr>
## Public Signals Documentation




### signal activeLayerChanged 

```C++
void QfProjectInfo::activeLayerChanged;
```




<hr>



### signal cloudUserInformationChanged 

```C++
void QfProjectInfo::cloudUserInformationChanged;
```




<hr>



### signal filePathChanged 

```C++
void QfProjectInfo::filePathChanged;
```




<hr>



### signal layerTreeChanged 

```C++
void QfProjectInfo::layerTreeChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfProjectInfo::mapSettingsChanged;
```




<hr>



### signal snappingEnabledChanged 

```C++
void QfProjectInfo::snappingEnabledChanged;
```




<hr>



### signal stateModeChanged 

```C++
void QfProjectInfo::stateModeChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void QfProjectInfo::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function QfProjectInfo 

```C++
explicit QfProjectInfo::QfProjectInfo (
    QObject * parent=nullptr
) 
```




<hr>



### function activeLayer [2/2]

```C++
QgsMapLayer * QfProjectInfo::activeLayer () const
```



Returns the saved active layer for the current project or nullptr if active layer was not saved or isn't present 


        

<hr>



### function cloudUserInformation [2/2]

```C++
QfCloudUserInformation QfProjectInfo::cloudUserInformation () const
```



Returns the saved cloud user infomation for offline usage 


        

<hr>



### function filePath [2/2]

```C++
QString QfProjectInfo::filePath () const
```



The file path associated with the currently opened project.   


        

<hr>



### function getCopyrightDecorationConfiguration 

_Retrieves configuration of the copyright decoration._ 
```C++
Q_INVOKABLE QVariantMap QfProjectInfo::getCopyrightDecorationConfiguration () 
```




<hr>



### function getDefaultActiveLayerForMapTheme 

_Retrieves the default active layer for a given map theme._ 
```C++
Q_INVOKABLE QgsMapLayer * QfProjectInfo::getDefaultActiveLayerForMapTheme (
    const QString & mapTheme
) 
```




<hr>



### function getGridDecorationConfiguration 

_Retrieves configuration of the grid decoration._ 
```C++
Q_INVOKABLE QVariantMap QfProjectInfo::getGridDecorationConfiguration () 
```




<hr>



### function getImageDecorationConfiguration 

_Retrieves configuration of the image decoration._ 
```C++
Q_INVOKABLE QVariantMap QfProjectInfo::getImageDecorationConfiguration () 
```




<hr>



### function getTitleDecorationConfiguration 

_Retrieves configuration of the title decoration._ 
```C++
Q_INVOKABLE QVariantMap QfProjectInfo::getTitleDecorationConfiguration () 
```




<hr>



### function layerTree [2/2]

```C++
QfFlatLayerTreeModel * QfProjectInfo::layerTree () const
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc.   


        

<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfProjectInfo::mapSettings () const
```



The map settings object, used to keep track of details such as the map extent.   


        

<hr>



### function restoreCloudUserInformation 

```C++
Q_INVOKABLE void QfProjectInfo::restoreCloudUserInformation () 
```



Restores last saved cloud user information details attached to the current project 


        

<hr>



### function restoreTracker 

_Restore the last tracking session that occured within a vector_ _layer_ _._
```C++
Q_INVOKABLE QModelIndex QfProjectInfo::restoreTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function saveLayerRememberedFields 

```C++
Q_INVOKABLE void QfProjectInfo::saveLayerRememberedFields (
    QgsMapLayer * layer
) 
```



Saves the vector _layer_ fields that are remembered during feature additions 


        

<hr>



### function saveLayerSnappingConfiguration 

```C++
Q_INVOKABLE void QfProjectInfo::saveLayerSnappingConfiguration (
    QgsMapLayer * layer
) 
```



Saves the current snapping configuration settings 


        

<hr>



### function saveLayerStyle 

```C++
Q_INVOKABLE void QfProjectInfo::saveLayerStyle (
    QgsMapLayer * layer
) 
```



Saves the _layer_ style to the current project information settings 


        

<hr>



### function saveLayerTreeState 

```C++
Q_INVOKABLE void QfProjectInfo::saveLayerTreeState () 
```



Saves the current state (visibility and collapse status) of the layer tree 


        

<hr>



### function saveTracker 

_Save an ongoing vector_ _layer_ _tracking session details._
```C++
Q_INVOKABLE void QfProjectInfo::saveTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function saveVariable 

```C++
Q_INVOKABLE void QfProjectInfo::saveVariable (
    const QString & name,
    const QString & value
) 
```




<hr>



### function setActiveLayer 

```C++
void QfProjectInfo::setActiveLayer (
    QgsMapLayer * layer
) 
```



Saves the active _layer_ within the current project 


        

<hr>



### function setCloudUserInformation 

```C++
void QfProjectInfo::setCloudUserInformation (
    const QfCloudUserInformation & cloudUserInformation
) 
```



Saves the cloud user infomation for offline usage 


        

<hr>



### function setFilePath 

```C++
void QfProjectInfo::setFilePath (
    const QString & filePath
) 
```



The file path associated with the currently opened project.   


        

<hr>



### function setLayerTree 

```C++
void QfProjectInfo::setLayerTree (
    QfFlatLayerTreeModel * layerTree
) 
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc.   


        

<hr>



### function setMapSettings 

```C++
void QfProjectInfo::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



The map settings object, used to keep track of details such as the map extent.   


        

<hr>



### function setSnappingEnabled 

```C++
void QfProjectInfo::setSnappingEnabled (
    bool enabled
) 
```



Saves the snapping _enabled_ state for the current project 


        

<hr>



### function setStateMode 

```C++
void QfProjectInfo::setStateMode (
    const QString & mode
) 
```



Saves the state _mode_ for the current project 


        

<hr>



### function setTrackingModel 

```C++
void QfProjectInfo::setTrackingModel (
    QfTrackingModel * trackingModel
) 
```



The tracking model object, used to save and restore tracking session for individual vector layers.   


        

<hr>



### function snappingEnabled [2/2]

```C++
bool QfProjectInfo::snappingEnabled () const
```



Returns the saved snapping enabed state for the current project 


        

<hr>



### function stateMode [2/2]

```C++
QString QfProjectInfo::stateMode () const
```



Returns the saved state mode for the current project 


        

<hr>



### function trackingModel [2/2]

```C++
QfTrackingModel * QfProjectInfo::trackingModel () const
```



The tracking model object, used to save and restore tracking session for individual vector layers.   


        

<hr>



### function ~QfProjectInfo 

```C++
virtual QfProjectInfo::~QfProjectInfo () = default
```




<hr>
## Public Static Functions Documentation




### function restoreSettings 

_Restore various project settings._ 
```C++
static void QfProjectInfo::restoreSettings (
    QString & projectFilePath,
    QgsProject * project,
    QgsQuickMapCanvasMap * mapCanvas,
    QfFlatLayerTreeModel * layerTree
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfprojectinfo.h`

