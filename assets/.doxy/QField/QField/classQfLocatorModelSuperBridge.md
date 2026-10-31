

# Class QfLocatorModelSuperBridge



[**ClassList**](annotated.md) **>** [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md)



[More...](#detailed-description)

* `#include <qflocatormodelsuperbridge.h>`



Inherits the following classes: QgsLocatorModelBridge






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot Q\_INVOKABLE void | [**triggerResultAtRow**](classQfLocatorModelSuperBridge.md#slot-triggerresultatrow) (const int row, const int id=-1) <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsMapLayer \* | [**activeLayer**](classQfLocatorModelSuperBridge.md#property-activelayer-12)  <br>_The current project's active layer._  |
| property [**QfBookmarkModel**](classQfBookmarkModel.md) \* | [**bookmarks**](classQfLocatorModelSuperBridge.md#property-bookmarks-12)  <br>_The bookmark manager containing user and current project bookmarks._  |
| property [**QfFeatureListExtentController**](classQfFeatureListExtentController.md) \* | [**featureListController**](classQfLocatorModelSuperBridge.md#property-featurelistcontroller-12)  <br>_The feature list extent controller._  |
| property QObject \* | [**geometryHighlighter**](classQfLocatorModelSuperBridge.md#property-geometryhighlighter-12)  <br>_The locator highlight geometry object through which locator actions can highhlight features._  |
| property bool | [**keepScale**](classQfLocatorModelSuperBridge.md#property-keepscale-12)  <br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfLocatorModelSuperBridge.md#property-mapsettings-12)  <br>_The current project's map settings._  |
| property [**QfNavigation**](classQfNavigation.md) \* | [**navigation**](classQfLocatorModelSuperBridge.md#property-navigation-12)  <br>_The navigation object from which destination can be set or modified._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeLayerChanged**](classQfLocatorModelSuperBridge.md#signal-activelayerchanged)  <br> |
| signal void | [**bookmarksChanged**](classQfLocatorModelSuperBridge.md#signal-bookmarkschanged)  <br> |
| signal void | [**featureListControllerChanged**](classQfLocatorModelSuperBridge.md#signal-featurelistcontrollerchanged)  <br> |
| signal void | [**geometryHighlighterChanged**](classQfLocatorModelSuperBridge.md#signal-geometryhighlighterchanged)  <br> |
| signal void | [**keepScaleChanged**](classQfLocatorModelSuperBridge.md#signal-keepscalechanged)  <br> |
| signal void | [**locatorFiltersChanged**](classQfLocatorModelSuperBridge.md#signal-locatorfilterschanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfLocatorModelSuperBridge.md#signal-mapsettingschanged)  <br> |
| signal void | [**messageEmitted**](classQfLocatorModelSuperBridge.md#signal-messageemitted) (const QString & text) <br> |
| signal void | [**navigationChanged**](classQfLocatorModelSuperBridge.md#signal-navigationchanged)  <br> |
| signal void | [**requestJumpToPoint**](classQfLocatorModelSuperBridge.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**searchRequested**](classQfLocatorModelSuperBridge.md#signal-searchrequested) (const QString & text) <br> |
| signal void | [**searchTextChangeRequested**](classQfLocatorModelSuperBridge.md#signal-searchtextchangerequested) (const QString & text) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocatorModelSuperBridge**](#function-qflocatormodelsuperbridge) (QObject \* parent=nullptr) <br> |
|  QgsMapLayer \* | [**activeLayer**](#function-activelayer-22) () const<br>_The current project's active layer._  |
|  [**QfBookmarkModel**](classQfBookmarkModel.md) \* | [**bookmarks**](#function-bookmarks-22) () const<br>_The bookmark manager containing user and current project bookmarks._  |
|  Q\_INVOKABLE [**QfLocatorActionsModel**](classQfLocatorActionsModel.md) \* | [**contextMenuActionsModel**](#function-contextmenuactionsmodel) (const int row) <br> |
|  Q\_INVOKABLE void | [**deregisterQFieldLocatorFilter**](#function-deregisterqfieldlocatorfilter) ([**QfLocatorFilter**](classQfLocatorFilter.md) \* filter) <br> |
|  void | [**emitMessage**](#function-emitmessage) (const QString & text) <br> |
|  [**QfFeatureListExtentController**](classQfFeatureListExtentController.md) \* | [**featureListController**](#function-featurelistcontroller-22) () const<br>_The feature list extent controller._  |
|  QObject \* | [**geometryHighlighter**](#function-geometryhighlighter-22) () const<br>_The locator highlight geometry object through which locator actions can highhlight features._  |
|  Q\_INVOKABLE QString | [**getLocatorModelDescription**](#function-getlocatormodeldescription) (const int row) <br> |
|  Q\_INVOKABLE QString | [**getPrefixFromSearchString**](#function-getprefixfromsearchstring) (const QString & string) <br> |
|  bool | [**keepScale**](#function-keepscale-22) () const<br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_The current project's map settings._  |
|  [**QfNavigation**](classQfNavigation.md) \* | [**navigation**](#function-navigation-22) () const<br>_The navigation object from which destination can be set or modified._  |
|  Q\_INVOKABLE void | [**registerQFieldLocatorFilter**](#function-registerqfieldlocatorfilter) ([**QfLocatorFilter**](classQfLocatorFilter.md) \* filter) <br> |
|  Q\_INVOKABLE void | [**requestSearch**](#function-requestsearch) (const QString & text) <br> |
|  void | [**requestSearchTextChange**](#function-requestsearchtextchange) (const QString & text) <br> |
|  void | [**setActiveLayer**](#function-setactivelayer) (QgsMapLayer \* layer) <br>_The current project's active layer._  |
|  void | [**setBookmarks**](#function-setbookmarks) ([**QfBookmarkModel**](classQfBookmarkModel.md) \* bookmarks) <br>_The bookmark manager containing user and current project bookmarks._  |
|  void | [**setFeatureListController**](#function-setfeaturelistcontroller) ([**QfFeatureListExtentController**](classQfFeatureListExtentController.md) \* featureListController) <br>_The feature list extent controller._  |
|  void | [**setGeometryHighlighter**](#function-setgeometryhighlighter) (QObject \* geometryHighlighter) <br>_The locator highlight geometry object through which locator actions can highhlight features._  |
|  void | [**setKeepScale**](#function-setkeepscale) (bool keepScale) <br>_The keep scale flag. When turned on, locator actions should not result in changed scale._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_The current project's map settings._  |
|  void | [**setNavigation**](#function-setnavigation) ([**QfNavigation**](classQfNavigation.md) \* navigation) <br>_The navigation object from which destination can be set or modified._  |
|   | [**~QfLocatorModelSuperBridge**](#function-qflocatormodelsuperbridge) () = default<br> |




























## Detailed Description


[**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) reimplements QgsLocatorModelBridge for specific needs of QField / QML implementation. 


    
## Public Properties Documentation




### property activeLayer [1/2]

_The current project's active layer._ 
```C++
QgsMapLayer * QfLocatorModelSuperBridge::activeLayer;
```




<hr>



### property bookmarks [1/2]

_The bookmark manager containing user and current project bookmarks._ 
```C++
QfBookmarkModel * QfLocatorModelSuperBridge::bookmarks;
```




<hr>



### property featureListController [1/2]

_The feature list extent controller._ 
```C++
QfFeatureListExtentController * QfLocatorModelSuperBridge::featureListController;
```




<hr>



### property geometryHighlighter [1/2]

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
QObject * QfLocatorModelSuperBridge::geometryHighlighter;
```




<hr>



### property keepScale [1/2]

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
bool QfLocatorModelSuperBridge::keepScale;
```




<hr>



### property mapSettings [1/2]

_The current project's map settings._ 
```C++
QgsQuickMapSettings * QfLocatorModelSuperBridge::mapSettings;
```




<hr>



### property navigation [1/2]

_The navigation object from which destination can be set or modified._ 
```C++
QfNavigation * QfLocatorModelSuperBridge::navigation;
```




<hr>
## Public Slots Documentation




### slot triggerResultAtRow 

```C++
void QfLocatorModelSuperBridge::triggerResultAtRow;
```




<hr>
## Public Signals Documentation




### signal activeLayerChanged 

```C++
void QfLocatorModelSuperBridge::activeLayerChanged;
```




<hr>



### signal bookmarksChanged 

```C++
void QfLocatorModelSuperBridge::bookmarksChanged;
```




<hr>



### signal featureListControllerChanged 

```C++
void QfLocatorModelSuperBridge::featureListControllerChanged;
```




<hr>



### signal geometryHighlighterChanged 

```C++
void QfLocatorModelSuperBridge::geometryHighlighterChanged;
```




<hr>



### signal keepScaleChanged 

```C++
void QfLocatorModelSuperBridge::keepScaleChanged;
```




<hr>



### signal locatorFiltersChanged 

```C++
void QfLocatorModelSuperBridge::locatorFiltersChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfLocatorModelSuperBridge::mapSettingsChanged;
```




<hr>



### signal messageEmitted 

```C++
void QfLocatorModelSuperBridge::messageEmitted;
```




<hr>



### signal navigationChanged 

```C++
void QfLocatorModelSuperBridge::navigationChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfLocatorModelSuperBridge::requestJumpToPoint;
```




<hr>



### signal searchRequested 

```C++
void QfLocatorModelSuperBridge::searchRequested;
```




<hr>



### signal searchTextChangeRequested 

```C++
void QfLocatorModelSuperBridge::searchTextChangeRequested;
```




<hr>
## Public Functions Documentation




### function QfLocatorModelSuperBridge 

```C++
explicit QfLocatorModelSuperBridge::QfLocatorModelSuperBridge (
    QObject * parent=nullptr
) 
```




<hr>



### function activeLayer [2/2]

_The current project's active layer._ 
```C++
QgsMapLayer * QfLocatorModelSuperBridge::activeLayer () const
```




<hr>



### function bookmarks [2/2]

_The bookmark manager containing user and current project bookmarks._ 
```C++
QfBookmarkModel * QfLocatorModelSuperBridge::bookmarks () const
```




<hr>



### function contextMenuActionsModel 

```C++
Q_INVOKABLE QfLocatorActionsModel * QfLocatorModelSuperBridge::contextMenuActionsModel (
    const int row
) 
```



Returns the actions model for a given locator search result list item. 


        

<hr>



### function deregisterQFieldLocatorFilter 

```C++
Q_INVOKABLE void QfLocatorModelSuperBridge::deregisterQFieldLocatorFilter (
    QfLocatorFilter * filter
) 
```



Deregisters a given _filter_ with the locator. 


        

<hr>



### function emitMessage 

```C++
void QfLocatorModelSuperBridge::emitMessage (
    const QString & text
) 
```




<hr>



### function featureListController [2/2]

_The feature list extent controller._ 
```C++
QfFeatureListExtentController * QfLocatorModelSuperBridge::featureListController () const
```




<hr>



### function geometryHighlighter [2/2]

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
QObject * QfLocatorModelSuperBridge::geometryHighlighter () const
```




<hr>



### function getLocatorModelDescription 

```C++
Q_INVOKABLE QString QfLocatorModelSuperBridge::getLocatorModelDescription (
    const int row
) 
```



Returns the description for a given locator search result list item. 


        

<hr>



### function getPrefixFromSearchString 

```C++
Q_INVOKABLE QString QfLocatorModelSuperBridge::getPrefixFromSearchString (
    const QString & string
) 
```



Looks for and if present returns the locator filter prefix from a given search string. If not prefix is detected, an empty string will be returned. 


        

<hr>



### function keepScale [2/2]

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
bool QfLocatorModelSuperBridge::keepScale () const
```




<hr>



### function mapSettings [2/2]

_The current project's map settings._ 
```C++
QgsQuickMapSettings * QfLocatorModelSuperBridge::mapSettings () const
```




<hr>



### function navigation [2/2]

_The navigation object from which destination can be set or modified._ 
```C++
QfNavigation * QfLocatorModelSuperBridge::navigation () const
```




<hr>



### function registerQFieldLocatorFilter 

```C++
Q_INVOKABLE void QfLocatorModelSuperBridge::registerQFieldLocatorFilter (
    QfLocatorFilter * filter
) 
```



Registers a given _filter_ with the locator. 


        

<hr>



### function requestSearch 

```C++
Q_INVOKABLE void QfLocatorModelSuperBridge::requestSearch (
    const QString & text
) 
```



Requests a _text_ query against the search bar. 


        

<hr>



### function requestSearchTextChange 

```C++
void QfLocatorModelSuperBridge::requestSearchTextChange (
    const QString & text
) 
```



Requests for the current text in the search bar to be changed to the provided _text_ string. 


        

<hr>



### function setActiveLayer 

_The current project's active layer._ 
```C++
void QfLocatorModelSuperBridge::setActiveLayer (
    QgsMapLayer * layer
) 
```




<hr>



### function setBookmarks 

_The bookmark manager containing user and current project bookmarks._ 
```C++
void QfLocatorModelSuperBridge::setBookmarks (
    QfBookmarkModel * bookmarks
) 
```




<hr>



### function setFeatureListController 

_The feature list extent controller._ 
```C++
void QfLocatorModelSuperBridge::setFeatureListController (
    QfFeatureListExtentController * featureListController
) 
```




<hr>



### function setGeometryHighlighter 

_The locator highlight geometry object through which locator actions can highhlight features._ 
```C++
void QfLocatorModelSuperBridge::setGeometryHighlighter (
    QObject * geometryHighlighter
) 
```




<hr>



### function setKeepScale 

_The keep scale flag. When turned on, locator actions should not result in changed scale._ 
```C++
void QfLocatorModelSuperBridge::setKeepScale (
    bool keepScale
) 
```




<hr>



### function setMapSettings 

_The current project's map settings._ 
```C++
void QfLocatorModelSuperBridge::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setNavigation 

_The navigation object from which destination can be set or modified._ 
```C++
void QfLocatorModelSuperBridge::setNavigation (
    QfNavigation * navigation
) 
```




<hr>



### function ~QfLocatorModelSuperBridge 

```C++
QfLocatorModelSuperBridge::~QfLocatorModelSuperBridge () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qflocatormodelsuperbridge.h`

