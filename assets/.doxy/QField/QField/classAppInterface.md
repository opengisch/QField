

# Class AppInterface



[**ClassList**](annotated.md) **>** [**AppInterface**](classAppInterface.md)



_App interface made available in QML as_ `iface` _._

* `#include <appinterface.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**executeAction**](classAppInterface.md#signal-executeaction) (const QString & action) <br>_Requests QField to execute a given_ _action_ _._ |
| signal void | [**importEnded**](classAppInterface.md#signal-importended) (const QString & path=QString()) <br> |
| signal void | [**importProgress**](classAppInterface.md#signal-importprogress) (double progress) <br> |
| signal void | [**importTriggered**](classAppInterface.md#signal-importtriggered) (const QString & name) <br> |
| signal void | [**loadProjectEnded**](classAppInterface.md#signal-loadprojectended) (const QString & path, const QString & name) <br> |
| signal void | [**loadProjectTriggered**](classAppInterface.md#signal-loadprojecttriggered) (const QString & path, const QString & name) <br> |
| signal void | [**openPath**](classAppInterface.md#signal-openpath) (const QString & path) <br>_Requests QField to open its local data picker screen to show the_ _path_ _content._ |
| signal void | [**setMapExtent**](classAppInterface.md#signal-setmapextent) (const QgsRectangle & extent) <br>_Requests QField to set its map to the provided_ _extent_ _._ |
| signal void | [**volumeKeyDown**](classAppInterface.md#signal-volumekeydown) (int volumeKeyCode) <br>_Emitted when a volume key is pressed while QField is set to handle those keys._  |
| signal void | [**volumeKeyUp**](classAppInterface.md#signal-volumekeyup) (int volumeKeyCode) <br>_Emitted when a volume key is pressed while QField is set to handle those keys._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AppInterface**](#function-appinterface-12) ([**QgisMobileapp**](classQgisMobileapp.md) \* app) <br> |
|   | [**AppInterface**](#function-appinterface-22) () <br> |
|  Q\_INVOKABLE void | [**addItemToCanvasActionsToolbar**](#function-additemtocanvasactionstoolbar) (QQuickItem \* item) const<br> |
|  Q\_INVOKABLE void | [**addItemToDashboardActionsToolbar**](#function-additemtodashboardactionstoolbar) (QQuickItem \* item) const<br> |
|  Q\_INVOKABLE void | [**addItemToMainMenuActionsToolbar**](#function-additemtomainmenuactionstoolbar) (QQuickItem \* item) const<br> |
|  Q\_INVOKABLE void | [**addItemToPluginsToolbar**](#function-additemtopluginstoolbar) (QQuickItem \* item) const<br> |
|  Q\_INVOKABLE QVariantMap | [**availableLanguages**](#function-availablelanguages) () const<br>_Returns a list of available UI translation languages._  |
|  Q\_INVOKABLE void | [**changeLanguage**](#function-changelanguage) (const QString & languageCode) <br> |
|  Q\_INVOKABLE void | [**clearProject**](#function-clearproject) () const<br> |
|  Q\_INVOKABLE void | [**closeSentry**](#function-closesentry) () const<br> |
|  Q\_INVOKABLE QObject \* | [**createHttpRequest**](#function-createhttprequest) () const<br>_One-shot xmlhttp request. Defaults to autoDelete = true._  |
|  Q\_INVOKABLE QObject \* | [**findItemByObjectName**](#function-finditembyobjectname) (const QString & name) const<br> |
|  Q\_INVOKABLE bool | [**hasProjectOnLaunch**](#function-hasprojectonlaunch) () const<br>_Returns TRUE is a project was passed on when launching QField._  |
|  Q\_INVOKABLE void | [**importUrl**](#function-importurl) (const QString & url, bool loadOnImport=false) <br> |
|  Q\_INVOKABLE void | [**initiateSentry**](#function-initiatesentry) () const<br> |
|  Q\_INVOKABLE bool | [**isAuthenticationConfigurationAvailable**](#function-isauthenticationconfigurationavailable) (const QString & id) const<br> |
|  Q\_INVOKABLE bool | [**isFileExtensionSupported**](#function-isfileextensionsupported) (const QString & filename) const<br>_Returns TRUE if a given_ _filename_ _can be opened as a project or standalone dataset._ |
|  Q\_INVOKABLE bool | [**loadFile**](#function-loadfile) (const QString & path, const QString & name=QString()) <br> |
|  Q\_INVOKABLE void | [**logMessage**](#function-logmessage) (const QString & message) <br> |
|  Q\_INVOKABLE void | [**logRuntimeProfiler**](#function-logruntimeprofiler) () <br> |
|  Q\_INVOKABLE QObject \* | [**mainWindow**](#function-mainwindow) () const<br> |
|  Q\_INVOKABLE QObject \* | [**mapCanvas**](#function-mapcanvas) () const<br> |
|  Q\_INVOKABLE QObject \* | [**positioning**](#function-positioning) () const<br> |
|  Q\_INVOKABLE bool | [**print**](#function-print) (const QString & layoutName) <br> |
|  Q\_INVOKABLE bool | [**printAtlasFeatures**](#function-printatlasfeatures) (const QString & layoutName, const QList&lt; long long &gt; & featureIds) <br> |
|  Q\_INVOKABLE bool | [**readProjectBoolEntry**](#function-readprojectboolentry) (const QString & scope, const QString & key, bool def=false) const<br> |
|  Q\_INVOKABLE double | [**readProjectDoubleEntry**](#function-readprojectdoubleentry) (const QString & scope, const QString & key, double def=0.0) const<br> |
|  Q\_INVOKABLE QString | [**readProjectEntry**](#function-readprojectentry) (const QString & scope, const QString & key, const QString & def=QString()) const<br> |
|  Q\_INVOKABLE int | [**readProjectNumEntry**](#function-readprojectnumentry) (const QString & scope, const QString & key, int def=0) const<br> |
|  Q\_INVOKABLE void | [**reloadProject**](#function-reloadproject) () <br>_Reloads the currently opened project._  |
|  Q\_INVOKABLE void | [**removeRecentProject**](#function-removerecentproject) (const QString & path) <br>_Removes a given project_ _path_ _from the recent projects list._ |
|  Q\_INVOKABLE void | [**sendLog**](#function-sendlog) (const QString & message, const QString & cloudUser) <br> |
|  Q\_INVOKABLE void | [**setScreenDimmerTimeout**](#function-setscreendimmertimeout) (int timeoutSeconds) <br> |




























## Public Signals Documentation




### signal executeAction 

_Requests QField to execute a given_ _action_ _._
```C++
void AppInterface::executeAction;
```




<hr>



### signal importEnded 

```C++
void AppInterface::importEnded;
```



Emitted when an import has ended. 

**Parameters:**


* `path` the path within which the imported dataset or project has been copied into 



**Note:**

if the import was not successful, the path value will be an empty string 





        

<hr>



### signal importProgress 

```C++
void AppInterface::importProgress;
```



Emitted when an ongoing import reports its _progress_. 

**Note:**

when an import is started, its progress will be indefinite by default 





        

<hr>



### signal importTriggered 

```C++
void AppInterface::importTriggered;
```



Emitted when a dataset or project import has been triggered. 

**Parameters:**


* `name` a indentifier-friendly string (e.g. a file being imported) 




        

<hr>



### signal loadProjectEnded 

```C++
void AppInterface::loadProjectEnded;
```



Emitted when a project loading has ended. 


        

<hr>



### signal loadProjectTriggered 

```C++
void AppInterface::loadProjectTriggered;
```



Emitted when a project has begin loading. 


        

<hr>



### signal openPath 

_Requests QField to open its local data picker screen to show the_ _path_ _content._
```C++
void AppInterface::openPath;
```




<hr>



### signal setMapExtent 

_Requests QField to set its map to the provided_ _extent_ _._
```C++
void AppInterface::setMapExtent;
```




<hr>



### signal volumeKeyDown 

_Emitted when a volume key is pressed while QField is set to handle those keys._ 
```C++
void AppInterface::volumeKeyDown;
```




<hr>



### signal volumeKeyUp 

_Emitted when a volume key is pressed while QField is set to handle those keys._ 
```C++
void AppInterface::volumeKeyUp;
```




<hr>
## Public Functions Documentation




### function AppInterface [1/2]

```C++
explicit AppInterface::AppInterface (
    QgisMobileapp * app
) 
```




<hr>



### function AppInterface [2/2]

```C++
inline AppInterface::AppInterface () 
```




<hr>



### function addItemToCanvasActionsToolbar 

```C++
Q_INVOKABLE void AppInterface::addItemToCanvasActionsToolbar (
    QQuickItem * item
) const
```



Adds an _item_ in the map canvas menu's action toolbar container 


        

<hr>



### function addItemToDashboardActionsToolbar 

```C++
Q_INVOKABLE void AppInterface::addItemToDashboardActionsToolbar (
    QQuickItem * item
) const
```



Adds an _item_ in the dashboard's action toolbar container 


        

<hr>



### function addItemToMainMenuActionsToolbar 

```C++
Q_INVOKABLE void AppInterface::addItemToMainMenuActionsToolbar (
    QQuickItem * item
) const
```



Adds an _item_ in the dashboard's action toolbar container 

**Note:**

This function is deprecated and will be removed in the future, use the addItemToDashboardActionsToolbar function instead 





        

<hr>



### function addItemToPluginsToolbar 

```C++
Q_INVOKABLE void AppInterface::addItemToPluginsToolbar (
    QQuickItem * item
) const
```



Adds an _item_ in the plugins toolbar container 


        

<hr>



### function availableLanguages 

_Returns a list of available UI translation languages._ 
```C++
Q_INVOKABLE QVariantMap AppInterface::availableLanguages () const
```




<hr>



### function changeLanguage 

```C++
Q_INVOKABLE void AppInterface::changeLanguage (
    const QString & languageCode
) 
```



Changes the application language to the specified _languageCode_. This will reload translators and refresh all QML translations without restarting the app. 

**Parameters:**


* `languageCode` The language code (e.g., "en", "de") 



**See also:** [**availableLanguages**](classAppInterface.md#function-availablelanguages) 



        

<hr>



### function clearProject 

```C++
Q_INVOKABLE void AppInterface::clearProject () const
```



Clears the currently opened project 


        

<hr>



### function closeSentry 

```C++
Q_INVOKABLE void AppInterface::closeSentry () const
```



Closes active sentry connection. 


        

<hr>



### function createHttpRequest 

_One-shot xmlhttp request. Defaults to autoDelete = true._ 
```C++
Q_INVOKABLE QObject * AppInterface::createHttpRequest () const
```




<hr>



### function findItemByObjectName 

```C++
Q_INVOKABLE QObject * AppInterface::findItemByObjectName (
    const QString & name
) const
```



Returns the item matching the provided object _name_ 


        

<hr>



### function hasProjectOnLaunch 

_Returns TRUE is a project was passed on when launching QField._ 
```C++
Q_INVOKABLE bool AppInterface::hasProjectOnLaunch () const
```




<hr>



### function importUrl 

```C++
Q_INVOKABLE void AppInterface::importUrl (
    const QString & url,
    bool loadOnImport=false
) 
```



Imports a compressed project from a given URL and place the content into the Imported Projects folder. 

**Parameters:**


* `url` the URL where the compressed project ZIP file is 
* `loadOnImport` set to TRUE to load the project on successful import 




        

<hr>



### function initiateSentry 

```C++
Q_INVOKABLE void AppInterface::initiateSentry () const
```



Initalizes sentry connection. 


        

<hr>



### function isAuthenticationConfigurationAvailable 

```C++
Q_INVOKABLE bool AppInterface::isAuthenticationConfigurationAvailable (
    const QString & id
) const
```



Returns TRUE if the authentication configuration _id_ is available. 


        

<hr>



### function isFileExtensionSupported 

_Returns TRUE if a given_ _filename_ _can be opened as a project or standalone dataset._
```C++
Q_INVOKABLE bool AppInterface::isFileExtensionSupported (
    const QString & filename
) const
```




<hr>



### function loadFile 

```C++
Q_INVOKABLE bool AppInterface::loadFile (
    const QString & path,
    const QString & name=QString()
) 
```



Loads a project file or standalone dataset.




**Parameters:**


* `path` the project file (.qgs or .qgz) or standalone dataset path 
* `name` a project name (if left empty, the project file will be used instead) 




        

<hr>



### function logMessage 

```C++
Q_INVOKABLE void AppInterface::logMessage (
    const QString & message
) 
```



Adds a log _message_ that will be visible to the user through the message log panel, as well as added into the device's system logs which will be captured by the sentry's reporting framework when enabled. 


        

<hr>



### function logRuntimeProfiler 

```C++
Q_INVOKABLE void AppInterface::logRuntimeProfiler () 
```



Outputs the current runtime profiler model content into the message log panel, as well as added into the device's system logs which will be captured by the sentry's reporting framework when enabled. 


        

<hr>



### function mainWindow 

```C++
Q_INVOKABLE QObject * AppInterface::mainWindow () const
```



Returns the main window. 


        

<hr>



### function mapCanvas 

```C++
Q_INVOKABLE QObject * AppInterface::mapCanvas () const
```



Returns the main map canvas item. 

**See also:** [**MapCanvas**](classMapCanvas.md) 



        

<hr>



### function positioning 

```C++
Q_INVOKABLE QObject * AppInterface::positioning () const
```



Returns the positioning item. 

**See also:** [**Positioning**](classPositioning.md) 



        

<hr>



### function print 

```C++
Q_INVOKABLE bool AppInterface::print (
    const QString & layoutName
) 
```



Prints a project layout to PDF. 

**Parameters:**


* `layoutName` the layout name 




        

<hr>



### function printAtlasFeatures 

```C++
Q_INVOKABLE bool AppInterface::printAtlasFeatures (
    const QString & layoutName,
    const QList< long long > & featureIds
) 
```



Prints an atlas-driven project layout to PDF. 

**Parameters:**


* `layoutName` the layout name 
* `featureIds` the list of atlas feature IDs 




        

<hr>



### function readProjectBoolEntry 

```C++
Q_INVOKABLE bool AppInterface::readProjectBoolEntry (
    const QString & scope,
    const QString & key,
    bool def=false
) const
```



Reads a double from the specified scope and key. 

**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified key does not exist within the scope 




        

<hr>



### function readProjectDoubleEntry 

```C++
Q_INVOKABLE double AppInterface::readProjectDoubleEntry (
    const QString & scope,
    const QString & key,
    double def=0.0
) const
```



Reads a double from the specified scope and key. 

**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified key does not exist within the scope 




        

<hr>



### function readProjectEntry 

```C++
Q_INVOKABLE QString AppInterface::readProjectEntry (
    const QString & scope,
    const QString & key,
    const QString & def=QString()
) const
```



Reads a string from the specified scope and key. 

**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified key does not exist within the scope 




        

<hr>



### function readProjectNumEntry 

```C++
Q_INVOKABLE int AppInterface::readProjectNumEntry (
    const QString & scope,
    const QString & key,
    int def=0
) const
```



Reads an integer from the specified scope and key. 

**Parameters:**


* `scope` entry scope (group) name 
* `key` entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values 
* `def` default value to return if the specified key does not exist within the scope 




        

<hr>



### function reloadProject 

_Reloads the currently opened project._ 
```C++
Q_INVOKABLE void AppInterface::reloadProject () 
```




<hr>



### function removeRecentProject 

_Removes a given project_ _path_ _from the recent projects list._
```C++
Q_INVOKABLE void AppInterface::removeRecentProject (
    const QString & path
) 
```




<hr>



### function sendLog 

```C++
Q_INVOKABLE void AppInterface::sendLog (
    const QString & message,
    const QString & cloudUser
) 
```



Sends a logs reporting through to sentry when enabled. 


        

<hr>



### function setScreenDimmerTimeout 

```C++
Q_INVOKABLE void AppInterface::setScreenDimmerTimeout (
    int timeoutSeconds
) 
```



Sets the screen drimmer timeout. Dimming can be disabled by setting the timeout to zero. 

**Parameters:**


* `timeoutSeconds` timeout in seconds 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/appinterface.h`

