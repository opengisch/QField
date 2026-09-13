

# Class QfPluginManager



[**ClassList**](annotated.md) **>** [**QfPluginManager**](classQfPluginManager.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QList&lt; [**QfPluginInformation**](classQfPluginInformation.md) &gt; | [**availableAppPlugins**](classQfPluginManager.md#property-availableappplugins-12)  <br> |
| property [**QfPluginModel**](classQfPluginModel.md) \* | [**pluginModel**](classQfPluginManager.md#property-pluginmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appPluginDisabled**](classQfPluginManager.md#signal-appplugindisabled) (const QString & uuid) <br> |
| signal void | [**appPluginEnabled**](classQfPluginManager.md#signal-apppluginenabled) (const QString & uuid) <br> |
| signal void | [**availableAppPluginsChanged**](classQfPluginManager.md#signal-availableapppluginschanged)  <br> |
| signal void | [**installEnded**](classQfPluginManager.md#signal-installended) (const QString & uuid=QString(), const QString & error=QString()) <br> |
| signal void | [**installProgress**](classQfPluginManager.md#signal-installprogress) (double progress) <br> |
| signal void | [**installTriggered**](classQfPluginManager.md#signal-installtriggered) (const QString & name) <br> |
| signal void | [**pluginModelChanged**](classQfPluginManager.md#signal-pluginmodelchanged)  <br> |
| signal void | [**pluginPermissionRequested**](classQfPluginManager.md#signal-pluginpermissionrequested) (const QString & pluginName, bool isProjectPlugin) <br> |
| signal void | [**projectPluginEnabled**](classQfPluginManager.md#signal-projectpluginenabled)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPluginManager**](#function-qfpluginmanager) (QQmlEngine \* engine) <br> |
|  QList&lt; [**QfPluginInformation**](classQfPluginInformation.md) &gt; | [**availableAppPlugins**](#function-availableappplugins-22) () const<br> |
|  Q\_INVOKABLE void | [**clearPluginPermissions**](#function-clearpluginpermissions) () <br> |
|  Q\_INVOKABLE void | [**configureAppPlugin**](#function-configureappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**denyProjectPluginPermission**](#function-denyprojectpluginpermission) (const QString & path) <br> |
|  Q\_INVOKABLE void | [**denyRequestedPluginPermission**](#function-denyrequestedpluginpermission) (bool permanent=false) <br> |
|  Q\_INVOKABLE void | [**disableAppPlugin**](#function-disableappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**enableAppPlugin**](#function-enableappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**grantRequestedPluginPermission**](#function-grantrequestedpluginpermission) (bool permanent=false) <br> |
|  Q\_INVOKABLE void | [**installFromRepository**](#function-installfromrepository) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**installFromUrl**](#function-installfromurl) (const QString & url) <br> |
|  Q\_INVOKABLE bool | [**isAppPluginConfigurable**](#function-isapppluginconfigurable) (const QString & uuid) const<br> |
|  Q\_INVOKABLE bool | [**isAppPluginEnabled**](#function-isapppluginenabled) (const QString & uuid) const<br> |
|  Q\_INVOKABLE bool | [**isProjectPluginEnabled**](#function-isprojectpluginenabled) (const QString & path) const<br> |
|  void | [**loadPlugin**](#function-loadplugin) (const QString & pluginPath, const QString & pluginName, bool skipPermissionCheck=false, bool isProjectPlugin=false) <br> |
|  [**QfPluginModel**](classQfPluginModel.md) \* | [**pluginModel**](#function-pluginmodel-22) () const<br> |
|  void | [**restoreAppPlugins**](#function-restoreappplugins) () <br> |
|  Q\_INVOKABLE void | [**uninstall**](#function-uninstall) (const QString & uuid) <br> |
|  void | [**unloadPlugin**](#function-unloadplugin) (const QString & pluginPath) <br> |
|  void | [**unloadPlugins**](#function-unloadplugins) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QString | [**findProjectPlugin**](#function-findprojectplugin) (const QString & projectPath) <br> |


























## Public Properties Documentation




### property availableAppPlugins [1/2]

```C++
QList<QfPluginInformation> QfPluginManager::availableAppPlugins;
```




<hr>



### property pluginModel [1/2]

```C++
QfPluginModel * QfPluginManager::pluginModel;
```




<hr>
## Public Signals Documentation




### signal appPluginDisabled 

```C++
void QfPluginManager::appPluginDisabled;
```




<hr>



### signal appPluginEnabled 

```C++
void QfPluginManager::appPluginEnabled;
```




<hr>



### signal availableAppPluginsChanged 

```C++
void QfPluginManager::availableAppPluginsChanged;
```




<hr>



### signal installEnded 

```C++
void QfPluginManager::installEnded;
```




<hr>



### signal installProgress 

```C++
void QfPluginManager::installProgress;
```




<hr>



### signal installTriggered 

```C++
void QfPluginManager::installTriggered;
```




<hr>



### signal pluginModelChanged 

```C++
void QfPluginManager::pluginModelChanged;
```




<hr>



### signal pluginPermissionRequested 

```C++
void QfPluginManager::pluginPermissionRequested;
```




<hr>



### signal projectPluginEnabled 

```C++
void QfPluginManager::projectPluginEnabled;
```




<hr>
## Public Functions Documentation




### function QfPluginManager 

```C++
explicit QfPluginManager::QfPluginManager (
    QQmlEngine * engine
) 
```



Constructs a PluginManager with the given _engine_. 


        

<hr>



### function availableAppPlugins [2/2]

```C++
QList< QfPluginInformation > QfPluginManager::availableAppPlugins () const
```



Returns the list of available app plugins installed locally. 


        

<hr>



### function clearPluginPermissions 

```C++
Q_INVOKABLE void QfPluginManager::clearPluginPermissions () 
```



Clears all saved plugin permissions except those explicitly enabled by the user. 


        

<hr>



### function configureAppPlugin 

```C++
Q_INVOKABLE void QfPluginManager::configureAppPlugin (
    const QString & uuid
) 
```



Opens the configuration interface for the plugin identified by _uuid_. 


        

<hr>



### function denyProjectPluginPermission 

```C++
Q_INVOKABLE void QfPluginManager::denyProjectPluginPermission (
    const QString & path
) 
```



Denies permission for a given project _path_ to load a project plugin. 


        

<hr>



### function denyRequestedPluginPermission 

```C++
Q_INVOKABLE void QfPluginManager::denyRequestedPluginPermission (
    bool permanent=false
) 
```



Denies permission to the last plugin that requested it. If _permanent_ is true, saves this choice. 


        

<hr>



### function disableAppPlugin 

```C++
Q_INVOKABLE void QfPluginManager::disableAppPlugin (
    const QString & uuid
) 
```



Disables the application plugin identified by _uuid_. 


        

<hr>



### function enableAppPlugin 

```C++
Q_INVOKABLE void QfPluginManager::enableAppPlugin (
    const QString & uuid
) 
```



Enables the application plugin identified by _uuid_. 


        

<hr>



### function grantRequestedPluginPermission 

```C++
Q_INVOKABLE void QfPluginManager::grantRequestedPluginPermission (
    bool permanent=false
) 
```



Grants permission to the last plugin that requested it. If _permanent_ is true, saves this choice. 


        

<hr>



### function installFromRepository 

```C++
Q_INVOKABLE void QfPluginManager::installFromRepository (
    const QString & uuid
) 
```



Installs a plugin from the given repository _uuid_. 


        

<hr>



### function installFromUrl 

```C++
Q_INVOKABLE void QfPluginManager::installFromUrl (
    const QString & url
) 
```



Installs a plugin from the given _url_. 


        

<hr>



### function isAppPluginConfigurable 

```C++
Q_INVOKABLE bool QfPluginManager::isAppPluginConfigurable (
    const QString & uuid
) const
```



Returns true if the application plugin with _uuid_ is configurable. 


        

<hr>



### function isAppPluginEnabled 

```C++
Q_INVOKABLE bool QfPluginManager::isAppPluginEnabled (
    const QString & uuid
) const
```



Returns true if the application plugin with _uuid_ is currently enabled. 


        

<hr>



### function isProjectPluginEnabled 

```C++
Q_INVOKABLE bool QfPluginManager::isProjectPluginEnabled (
    const QString & path
) const
```



Returns true if a plugin is currently enabled for a given project _path_. 


        

<hr>



### function loadPlugin 

```C++
void QfPluginManager::loadPlugin (
    const QString & pluginPath,
    const QString & pluginName,
    bool skipPermissionCheck=false,
    bool isProjectPlugin=false
) 
```



Loads a plugin from _pluginPath_ and registers it under _pluginName_. Optionally skips permission checks and marks it as a project plugin. 


        

<hr>



### function pluginModel [2/2]

```C++
QfPluginModel * QfPluginManager::pluginModel () const
```



Returns the plugin model used by this manager. 


        

<hr>



### function restoreAppPlugins 

```C++
void QfPluginManager::restoreAppPlugins () 
```



Restores and loads previously enabled application plugins. 


        

<hr>



### function uninstall 

```C++
Q_INVOKABLE void QfPluginManager::uninstall (
    const QString & uuid
) 
```



Uninstalls the application plugin identified by _uuid_. 


        

<hr>



### function unloadPlugin 

```C++
void QfPluginManager::unloadPlugin (
    const QString & pluginPath
) 
```



Unloads the plugin loaded from _pluginPath_. 


        

<hr>



### function unloadPlugins 

```C++
void QfPluginManager::unloadPlugins () 
```



Unloads all currently loaded plugins. 


        

<hr>
## Public Static Functions Documentation




### function findProjectPlugin 

```C++
static QString QfPluginManager::findProjectPlugin (
    const QString & projectPath
) 
```



Finds and returns the plugin path corresponding to a project at _projectPath_. Returns an empty string if none is found. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfpluginmanager.h`

