

# Class ProjectInfo



[**ClassList**](annotated.md) **>** [**ProjectInfo**](classProjectInfo.md)



[More...](#detailed-description)

* `#include <projectinfo.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsMapLayer \* | [**activeLayer**](classProjectInfo.md#property-activelayer-12)  <br> |
| property [**CloudUserInformation**](structCloudUserInformation.md) | [**cloudUserInformation**](classProjectInfo.md#property-clouduserinformation-12)  <br> |
| property QString | [**filePath**](classProjectInfo.md#property-filepath-12)  <br> |
| property [**FlatLayerTreeModel**](classFlatLayerTreeModel.md) \* | [**layerTree**](classProjectInfo.md#property-layertree-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classProjectInfo.md#property-mapsettings-12)  <br> |
| property bool | [**snappingEnabled**](classProjectInfo.md#property-snappingenabled-12)  <br> |
| property QString | [**stateMode**](classProjectInfo.md#property-statemode-12)  <br> |
| property [**TrackingModel**](classTrackingModel.md) \* | [**trackingModel**](classProjectInfo.md#property-trackingmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeLayerChanged**](classProjectInfo.md#signal-activelayerchanged)  <br> |
| signal void | [**cloudUserInformationChanged**](classProjectInfo.md#signal-clouduserinformationchanged)  <br> |
| signal void | [**filePathChanged**](classProjectInfo.md#signal-filepathchanged)  <br> |
| signal void | [**layerTreeChanged**](classProjectInfo.md#signal-layertreechanged)  <br> |
| signal void | [**mapSettingsChanged**](classProjectInfo.md#signal-mapsettingschanged)  <br> |
| signal void | [**snappingEnabledChanged**](classProjectInfo.md#signal-snappingenabledchanged)  <br> |
| signal void | [**stateModeChanged**](classProjectInfo.md#signal-statemodechanged)  <br> |
| signal void | [**trackingModelChanged**](classProjectInfo.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProjectInfo**](#function-projectinfo) (QObject \* parent=nullptr) <br> |
|  QgsMapLayer \* | [**activeLayer**](#function-activelayer-22) () const<br> |
|  [**CloudUserInformation**](structCloudUserInformation.md) | [**cloudUserInformation**](#function-clouduserinformation-22) () const<br> |
|  QString | [**filePath**](#function-filepath-22) () const<br> |
|  Q\_INVOKABLE QVariantMap | [**getCopyrightDecorationConfiguration**](#function-getcopyrightdecorationconfiguration) () <br>_Retrieves configuration of the copyright decoration._  |
|  Q\_INVOKABLE QgsMapLayer \* | [**getDefaultActiveLayerForMapTheme**](#function-getdefaultactivelayerformaptheme) (const QString & mapTheme) <br>_Retrieves the default active layer for a given map theme._  |
|  Q\_INVOKABLE QVariantMap | [**getGridDecorationConfiguration**](#function-getgriddecorationconfiguration) () <br>_Retrieves configuration of the grid decoration._  |
|  Q\_INVOKABLE QVariantMap | [**getImageDecorationConfiguration**](#function-getimagedecorationconfiguration) () <br>_Retrieves configuration of the image decoration._  |
|  Q\_INVOKABLE QVariantMap | [**getTitleDecorationConfiguration**](#function-gettitledecorationconfiguration) () <br>_Retrieves configuration of the title decoration._  |
|  [**FlatLayerTreeModel**](classFlatLayerTreeModel.md) \* | [**layerTree**](#function-layertree-22) () const<br> |
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
|  void | [**setCloudUserInformation**](#function-setclouduserinformation) (const [**CloudUserInformation**](structCloudUserInformation.md) cloudUserInformation) <br> |
|  void | [**setFilePath**](#function-setfilepath) (const QString & filePath) <br> |
|  void | [**setLayerTree**](#function-setlayertree) ([**FlatLayerTreeModel**](classFlatLayerTreeModel.md) \* layerTree) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setSnappingEnabled**](#function-setsnappingenabled) (bool enabled) <br> |
|  void | [**setStateMode**](#function-setstatemode) (const QString & mode) <br> |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**TrackingModel**](classTrackingModel.md) \* trackingModel) <br> |
|  bool | [**snappingEnabled**](#function-snappingenabled-22) () const<br> |
|  QString | [**stateMode**](#function-statemode-22) () const<br> |
|  [**TrackingModel**](classTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel-22) () const<br> |
| virtual  | [**~ProjectInfo**](#function-projectinfo) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  void | [**restoreSettings**](#function-restoresettings) (QString & projectFilePath, QgsProject \* project, [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) \* mapCanvas, [**FlatLayerTreeModel**](classFlatLayerTreeModel.md) \* layerTree) <br>_Restore various project settings._  |


























## Detailed Description


This class stores current projection information such as extent and map theme in order for those to be saved and afterwards restored when the project is re-opened. 


    
## Public Properties Documentation




### property activeLayer [1/2]

```C++
QgsMapLayer * ProjectInfo::activeLayer;
```



The active layer for the currently opened project. 


        

<hr>



### property cloudUserInformation [1/2]

```C++
CloudUserInformation ProjectInfo::cloudUserInformation;
```



Set cloud user information for offline usage. 


        

<hr>



### property filePath [1/2]

```C++
QString ProjectInfo::filePath;
```



The file path associated with the currently opened project. 


        

<hr>



### property layerTree [1/2]

```C++
FlatLayerTreeModel * ProjectInfo::layerTree;
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc. 


        

<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * ProjectInfo::mapSettings;
```



The map settings object, used to keep track of details such as the map extent. 


        

<hr>



### property snappingEnabled [1/2]

```C++
bool ProjectInfo::snappingEnabled;
```



The snapping enabled state for the currently opened project. 


        

<hr>



### property stateMode [1/2]

```C++
QString ProjectInfo::stateMode;
```



The state mode (browse vs. digitizing) for the currently opened project. 


        

<hr>



### property trackingModel [1/2]

```C++
TrackingModel * ProjectInfo::trackingModel;
```



The tracking model object, used to save and restore tracking session for individual vector layers. 


        

<hr>
## Public Signals Documentation




### signal activeLayerChanged 

```C++
void ProjectInfo::activeLayerChanged;
```




<hr>



### signal cloudUserInformationChanged 

```C++
void ProjectInfo::cloudUserInformationChanged;
```




<hr>



### signal filePathChanged 

```C++
void ProjectInfo::filePathChanged;
```




<hr>



### signal layerTreeChanged 

```C++
void ProjectInfo::layerTreeChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void ProjectInfo::mapSettingsChanged;
```




<hr>



### signal snappingEnabledChanged 

```C++
void ProjectInfo::snappingEnabledChanged;
```




<hr>



### signal stateModeChanged 

```C++
void ProjectInfo::stateModeChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void ProjectInfo::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function ProjectInfo 

```C++
explicit ProjectInfo::ProjectInfo (
    QObject * parent=nullptr
) 
```




<hr>



### function activeLayer [2/2]

```C++
QgsMapLayer * ProjectInfo::activeLayer () const
```



Returns the saved active layer for the current project or nullptr if active layer was not saved or isn't present 


        

<hr>



### function cloudUserInformation [2/2]

```C++
CloudUserInformation ProjectInfo::cloudUserInformation () const
```



Returns the saved cloud user infomation for offline usage 


        

<hr>



### function filePath [2/2]

```C++
QString ProjectInfo::filePath () const
```



The file path associated with the currently opened project.   


        

<hr>



### function getCopyrightDecorationConfiguration 

_Retrieves configuration of the copyright decoration._ 
```C++
Q_INVOKABLE QVariantMap ProjectInfo::getCopyrightDecorationConfiguration () 
```




<hr>



### function getDefaultActiveLayerForMapTheme 

_Retrieves the default active layer for a given map theme._ 
```C++
Q_INVOKABLE QgsMapLayer * ProjectInfo::getDefaultActiveLayerForMapTheme (
    const QString & mapTheme
) 
```




<hr>



### function getGridDecorationConfiguration 

_Retrieves configuration of the grid decoration._ 
```C++
Q_INVOKABLE QVariantMap ProjectInfo::getGridDecorationConfiguration () 
```




<hr>



### function getImageDecorationConfiguration 

_Retrieves configuration of the image decoration._ 
```C++
Q_INVOKABLE QVariantMap ProjectInfo::getImageDecorationConfiguration () 
```




<hr>



### function getTitleDecorationConfiguration 

_Retrieves configuration of the title decoration._ 
```C++
Q_INVOKABLE QVariantMap ProjectInfo::getTitleDecorationConfiguration () 
```




<hr>



### function layerTree [2/2]

```C++
FlatLayerTreeModel * ProjectInfo::layerTree () const
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc.   


        

<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * ProjectInfo::mapSettings () const
```



The map settings object, used to keep track of details such as the map extent.   


        

<hr>



### function restoreCloudUserInformation 

```C++
Q_INVOKABLE void ProjectInfo::restoreCloudUserInformation () 
```



Restores last saved cloud user information details attached to the current project 


        

<hr>



### function restoreTracker 

_Restore the last tracking session that occured within a vector_ _layer_ _._
```C++
Q_INVOKABLE QModelIndex ProjectInfo::restoreTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function saveLayerRememberedFields 

```C++
Q_INVOKABLE void ProjectInfo::saveLayerRememberedFields (
    QgsMapLayer * layer
) 
```



Saves the vector _layer_ fields that are remembered during feature additions 


        

<hr>



### function saveLayerSnappingConfiguration 

```C++
Q_INVOKABLE void ProjectInfo::saveLayerSnappingConfiguration (
    QgsMapLayer * layer
) 
```



Saves the current snapping configuration settings 


        

<hr>



### function saveLayerStyle 

```C++
Q_INVOKABLE void ProjectInfo::saveLayerStyle (
    QgsMapLayer * layer
) 
```



Saves the _layer_ style to the current project information settings 


        

<hr>



### function saveLayerTreeState 

```C++
Q_INVOKABLE void ProjectInfo::saveLayerTreeState () 
```



Saves the current state (visibility and collapse status) of the layer tree 


        

<hr>



### function saveTracker 

_Save an ongoing vector_ _layer_ _tracking session details._
```C++
Q_INVOKABLE void ProjectInfo::saveTracker (
    QgsVectorLayer * layer
) 
```




<hr>



### function saveVariable 

```C++
Q_INVOKABLE void ProjectInfo::saveVariable (
    const QString & name,
    const QString & value
) 
```




<hr>



### function setActiveLayer 

```C++
void ProjectInfo::setActiveLayer (
    QgsMapLayer * layer
) 
```



Saves the active _layer_ within the current project 


        

<hr>



### function setCloudUserInformation 

```C++
void ProjectInfo::setCloudUserInformation (
    const CloudUserInformation cloudUserInformation
) 
```



Saves the cloud user infomation for offline usage 


        

<hr>



### function setFilePath 

```C++
void ProjectInfo::setFilePath (
    const QString & filePath
) 
```



The file path associated with the currently opened project.   


        

<hr>



### function setLayerTree 

```C++
void ProjectInfo::setLayerTree (
    FlatLayerTreeModel * layerTree
) 
```



The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc.   


        

<hr>



### function setMapSettings 

```C++
void ProjectInfo::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



The map settings object, used to keep track of details such as the map extent.   


        

<hr>



### function setSnappingEnabled 

```C++
void ProjectInfo::setSnappingEnabled (
    bool enabled
) 
```



Saves the snapping _enabled_ state for the current project 


        

<hr>



### function setStateMode 

```C++
void ProjectInfo::setStateMode (
    const QString & mode
) 
```



Saves the state _mode_ for the current project 


        

<hr>



### function setTrackingModel 

```C++
void ProjectInfo::setTrackingModel (
    TrackingModel * trackingModel
) 
```



The tracking model object, used to save and restore tracking session for individual vector layers.   


        

<hr>



### function snappingEnabled [2/2]

```C++
bool ProjectInfo::snappingEnabled () const
```



Returns the saved snapping enabed state for the current project 


        

<hr>



### function stateMode [2/2]

```C++
QString ProjectInfo::stateMode () const
```



Returns the saved state mode for the current project 


        

<hr>



### function trackingModel [2/2]

```C++
TrackingModel * ProjectInfo::trackingModel () const
```



The tracking model object, used to save and restore tracking session for individual vector layers.   


        

<hr>



### function ~ProjectInfo 

```C++
virtual ProjectInfo::~ProjectInfo () = default
```




<hr>
## Public Static Functions Documentation




### function restoreSettings 

_Restore various project settings._ 
```C++
static void ProjectInfo::restoreSettings (
    QString & projectFilePath,
    QgsProject * project,
    QgsQuickMapCanvasMap * mapCanvas,
    FlatLayerTreeModel * layerTree
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/projectinfo.h`

