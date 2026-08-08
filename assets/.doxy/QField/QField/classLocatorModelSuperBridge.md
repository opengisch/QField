

# Class LocatorModelSuperBridge



[**ClassList**](annotated.md) **>** [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md)



[More...](#detailed-description)

* `#include <locatormodelsuperbridge.h>`



Inherits the following classes: QgsLocatorModelBridge






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot Q\_INVOKABLE void | [**triggerResultAtRow**](classLocatorModelSuperBridge.md#slot-triggerresultatrow) (const int row, const int id=-1) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsMapLayer \* | [**activeLayer**](classLocatorModelSuperBridge.md#property-activelayer-12)  <br>_The current project's active layer._  |
| property [**BookmarkModel**](classBookmarkModel.md) \* | [**bookmarks**](classLocatorModelSuperBridge.md#property-bookmarks-12)  <br>_The bookmark manager containing user and current project bookmarks._  |
| property [**FeatureListExtentController**](classFeatureListExtentController.md) \* | [**featureListController**](classLocatorModelSuperBridge.md#property-featurelistcontroller-12)  <br>_The feature list extent controller._  |
| property QObject \* | [**geometryHighlighter**](classLocatorModelSuperBridge.md#property-geometryhighlighter-12)  <br>_The locator highlight geometry object through which locator actions can highhlight features._  |
| property bool | [**keepScale**](classLocatorModelSuperBridge.md#property-keepscale-12)  <br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classLocatorModelSuperBridge.md#property-mapsettings-12)  <br>_The current project's map settings._  |
| property [**Navigation**](classNavigation.md) \* | [**navigation**](classLocatorModelSuperBridge.md#property-navigation-12)  <br>_The navigation object from which destination can be set or modified._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeLayerChanged**](classLocatorModelSuperBridge.md#signal-activelayerchanged)  <br> |
| signal void | [**bookmarksChanged**](classLocatorModelSuperBridge.md#signal-bookmarkschanged)  <br> |
| signal void | [**featureListControllerChanged**](classLocatorModelSuperBridge.md#signal-featurelistcontrollerchanged)  <br> |
| signal void | [**geometryHighlighterChanged**](classLocatorModelSuperBridge.md#signal-geometryhighlighterchanged)  <br> |
| signal void | [**keepScaleChanged**](classLocatorModelSuperBridge.md#signal-keepscalechanged)  <br> |
| signal void | [**locatorFiltersChanged**](classLocatorModelSuperBridge.md#signal-locatorfilterschanged)  <br> |
| signal void | [**mapSettingsChanged**](classLocatorModelSuperBridge.md#signal-mapsettingschanged)  <br> |
| signal void | [**messageEmitted**](classLocatorModelSuperBridge.md#signal-messageemitted) (const QString & text) <br> |
| signal void | [**navigationChanged**](classLocatorModelSuperBridge.md#signal-navigationchanged)  <br> |
| signal void | [**requestJumpToPoint**](classLocatorModelSuperBridge.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**searchRequested**](classLocatorModelSuperBridge.md#signal-searchrequested) (const QString & text) <br> |
| signal void | [**searchTextChangeRequested**](classLocatorModelSuperBridge.md#signal-searchtextchangerequested) (const QString & text) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LocatorModelSuperBridge**](#function-locatormodelsuperbridge) (QObject \* parent=nullptr) <br> |
|  QgsMapLayer \* | [**activeLayer**](#function-activelayer-22) () const<br>_The current project's active layer._  |
|  [**BookmarkModel**](classBookmarkModel.md) \* | [**bookmarks**](#function-bookmarks-22) () const<br>_The bookmark manager containing user and current project bookmarks._  |
|  Q\_INVOKABLE [**LocatorActionsModel**](classLocatorActionsModel.md) \* | [**contextMenuActionsModel**](#function-contextmenuactionsmodel) (const int row) <br> |
|  Q\_INVOKABLE void | [**deregisterQFieldLocatorFilter**](#function-deregisterqfieldlocatorfilter) ([**QFieldLocatorFilter**](classQFieldLocatorFilter.md) \* filter) <br> |
|  void | [**emitMessage**](#function-emitmessage) (const QString & text) <br> |
|  [**FeatureListExtentController**](classFeatureListExtentController.md) \* | [**featureListController**](#function-featurelistcontroller-22) () const<br>_The feature list extent controller._  |
|  QObject \* | [**geometryHighlighter**](#function-geometryhighlighter-22) () const<br>_The locator highlight geometry object through which locator actions can highhlight features._  |
|  Q\_INVOKABLE QString | [**getLocatorModelDescription**](#function-getlocatormodeldescription) (const int row) <br> |
|  Q\_INVOKABLE QString | [**getPrefixFromSearchString**](#function-getprefixfromsearchstring) (const QString & string) <br> |
|  bool | [**keepScale**](#function-keepscale-22) () const<br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_The current project's map settings._  |
|  [**Navigation**](classNavigation.md) \* | [**navigation**](#function-navigation-22) () const<br>_The navigation object from which destination can be set or modified._  |
|  Q\_INVOKABLE void | [**registerQFieldLocatorFilter**](#function-registerqfieldlocatorfilter) ([**QFieldLocatorFilter**](classQFieldLocatorFilter.md) \* filter) <br> |
|  Q\_INVOKABLE void | [**requestSearch**](#function-requestsearch) (const QString & text) <br> |
|  void | [**requestSearchTextChange**](#function-requestsearchtextchange) (const QString & text) <br> |
|  void | [**setActiveLayer**](#function-setactivelayer) (QgsMapLayer \* layer) <br>_The current project's active layer._  |
|  void | [**setBookmarks**](#function-setbookmarks) ([**BookmarkModel**](classBookmarkModel.md) \* bookmarks) <br>_The bookmark manager containing user and current project bookmarks._  |
|  void | [**setFeatureListController**](#function-setfeaturelistcontroller) ([**FeatureListExtentController**](classFeatureListExtentController.md) \* featureListController) <br>_The feature list extent controller._  |
|  void | [**setGeometryHighlighter**](#function-setgeometryhighlighter) (QObject \* geometryHighlighter) <br>_The locator highlight geometry object through which locator actions can highhlight features._  |
|  void | [**setKeepScale**](#function-setkeepscale) (bool keepScale) <br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_The current project's map settings._  |
|  void | [**setNavigation**](#function-setnavigation) ([**Navigation**](classNavigation.md) \* navigation) <br>_The navigation object from which destination can be set or modified._  |
|   | [**~LocatorModelSuperBridge**](#function-locatormodelsuperbridge) () = default<br> |




























## Detailed Description


[**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) reimplements QgsLocatorModelBridge for specific needs of QField / QML implementation. 


    
## Public Properties Documentation




### property activeLayer [1/2]

_The current project's active layer._ 
```C++
QgsMapLayer * LocatorModelSuperBridge::activeLayer;
```




<hr>



### property bookmarks [1/2]

_The bookmark manager containing user and current project bookmarks._ 
```C++
BookmarkModel * LocatorModelSuperBridge::bookmarks;
```




<hr>



### property featureListController [1/2]

_The feature list extent controller._ 
```C++
FeatureListExtentController * LocatorModelSuperBridge::featureListController;
```




<hr>



### property geometryHighlighter [1/2]

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
QObject * LocatorModelSuperBridge::geometryHighlighter;
```




<hr>



### property keepScale [1/2]

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
bool LocatorModelSuperBridge::keepScale;
```




<hr>



### property mapSettings [1/2]

_The current project's map settings._ 
```C++
QgsQuickMapSettings * LocatorModelSuperBridge::mapSettings;
```




<hr>



### property navigation [1/2]

_The navigation object from which destination can be set or modified._ 
```C++
Navigation * LocatorModelSuperBridge::navigation;
```




<hr>
## Public Slots Documentation




### slot triggerResultAtRow 

```C++
void LocatorModelSuperBridge::triggerResultAtRow;
```




<hr>
## Public Signals Documentation




### signal activeLayerChanged 

```C++
void LocatorModelSuperBridge::activeLayerChanged;
```




<hr>



### signal bookmarksChanged 

```C++
void LocatorModelSuperBridge::bookmarksChanged;
```




<hr>



### signal featureListControllerChanged 

```C++
void LocatorModelSuperBridge::featureListControllerChanged;
```




<hr>



### signal geometryHighlighterChanged 

```C++
void LocatorModelSuperBridge::geometryHighlighterChanged;
```




<hr>



### signal keepScaleChanged 

```C++
void LocatorModelSuperBridge::keepScaleChanged;
```




<hr>



### signal locatorFiltersChanged 

```C++
void LocatorModelSuperBridge::locatorFiltersChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void LocatorModelSuperBridge::mapSettingsChanged;
```




<hr>



### signal messageEmitted 

```C++
void LocatorModelSuperBridge::messageEmitted;
```




<hr>



### signal navigationChanged 

```C++
void LocatorModelSuperBridge::navigationChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void LocatorModelSuperBridge::requestJumpToPoint;
```




<hr>



### signal searchRequested 

```C++
void LocatorModelSuperBridge::searchRequested;
```




<hr>



### signal searchTextChangeRequested 

```C++
void LocatorModelSuperBridge::searchTextChangeRequested;
```




<hr>
## Public Functions Documentation




### function LocatorModelSuperBridge 

```C++
explicit LocatorModelSuperBridge::LocatorModelSuperBridge (
    QObject * parent=nullptr
) 
```




<hr>



### function activeLayer [2/2]

_The current project's active layer._ 
```C++
QgsMapLayer * LocatorModelSuperBridge::activeLayer () const
```




<hr>



### function bookmarks [2/2]

_The bookmark manager containing user and current project bookmarks._ 
```C++
BookmarkModel * LocatorModelSuperBridge::bookmarks () const
```




<hr>



### function contextMenuActionsModel 

```C++
Q_INVOKABLE LocatorActionsModel * LocatorModelSuperBridge::contextMenuActionsModel (
    const int row
) 
```



Returns the actions model for a given locator search result list item. 


        

<hr>



### function deregisterQFieldLocatorFilter 

```C++
Q_INVOKABLE void LocatorModelSuperBridge::deregisterQFieldLocatorFilter (
    QFieldLocatorFilter * filter
) 
```



Deregisters a given _filter_ with the locator. 


        

<hr>



### function emitMessage 

```C++
void LocatorModelSuperBridge::emitMessage (
    const QString & text
) 
```




<hr>



### function featureListController [2/2]

_The feature list extent controller._ 
```C++
FeatureListExtentController * LocatorModelSuperBridge::featureListController () const
```




<hr>



### function geometryHighlighter [2/2]

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
QObject * LocatorModelSuperBridge::geometryHighlighter () const
```




<hr>



### function getLocatorModelDescription 

```C++
Q_INVOKABLE QString LocatorModelSuperBridge::getLocatorModelDescription (
    const int row
) 
```



Returns the description for a given locator search result list item. 


        

<hr>



### function getPrefixFromSearchString 

```C++
Q_INVOKABLE QString LocatorModelSuperBridge::getPrefixFromSearchString (
    const QString & string
) 
```



Looks for and if present returns the locator filter prefix from a given search string. If not prefix is detected, an empty string will be returned. 


        

<hr>



### function keepScale [2/2]

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
bool LocatorModelSuperBridge::keepScale () const
```




<hr>



### function mapSettings [2/2]

_The current project's map settings._ 
```C++
QgsQuickMapSettings * LocatorModelSuperBridge::mapSettings () const
```




<hr>



### function navigation [2/2]

_The navigation object from which destination can be set or modified._ 
```C++
Navigation * LocatorModelSuperBridge::navigation () const
```




<hr>



### function registerQFieldLocatorFilter 

```C++
Q_INVOKABLE void LocatorModelSuperBridge::registerQFieldLocatorFilter (
    QFieldLocatorFilter * filter
) 
```



Registers a given _filter_ with the locator. 


        

<hr>



### function requestSearch 

```C++
Q_INVOKABLE void LocatorModelSuperBridge::requestSearch (
    const QString & text
) 
```



Requests a _text_ query against the search bar. 


        

<hr>



### function requestSearchTextChange 

```C++
void LocatorModelSuperBridge::requestSearchTextChange (
    const QString & text
) 
```



Requests for the current text in the search bar to be changed to the provided _text_ string. 


        

<hr>



### function setActiveLayer 

_The current project's active layer._ 
```C++
void LocatorModelSuperBridge::setActiveLayer (
    QgsMapLayer * layer
) 
```




<hr>



### function setBookmarks 

_The bookmark manager containing user and current project bookmarks._ 
```C++
void LocatorModelSuperBridge::setBookmarks (
    BookmarkModel * bookmarks
) 
```




<hr>



### function setFeatureListController 

_The feature list extent controller._ 
```C++
void LocatorModelSuperBridge::setFeatureListController (
    FeatureListExtentController * featureListController
) 
```




<hr>



### function setGeometryHighlighter 

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
void LocatorModelSuperBridge::setGeometryHighlighter (
    QObject * geometryHighlighter
) 
```




<hr>



### function setKeepScale 

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
void LocatorModelSuperBridge::setKeepScale (
    bool keepScale
) 
```




<hr>



### function setMapSettings 

_The current project's map settings._ 
```C++
void LocatorModelSuperBridge::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setNavigation 

_The navigation object from which destination can be set or modified._ 
```C++
void LocatorModelSuperBridge::setNavigation (
    Navigation * navigation
) 
```




<hr>



### function ~LocatorModelSuperBridge 

```C++
LocatorModelSuperBridge::~LocatorModelSuperBridge () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/locatormodelsuperbridge.h`

