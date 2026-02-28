

# Class PluginManager



[**ClassList**](annotated.md) **>** [**PluginManager**](classPluginManager.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QList&lt; [**PluginInformation**](classPluginInformation.md) &gt; | [**availableAppPlugins**](classPluginManager.md#property-availableappplugins-12)  <br> |
| property [**PluginModel**](classPluginModel.md) \* | [**pluginModel**](classPluginManager.md#property-pluginmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appPluginDisabled**](classPluginManager.md#signal-appplugindisabled) (const QString & uuid) <br> |
| signal void | [**appPluginEnabled**](classPluginManager.md#signal-apppluginenabled) (const QString & uuid) <br> |
| signal void | [**availableAppPluginsChanged**](classPluginManager.md#signal-availableapppluginschanged)  <br> |
| signal void | [**installEnded**](classPluginManager.md#signal-installended) (const QString & uuid=QString(), const QString & error=QString()) <br> |
| signal void | [**installProgress**](classPluginManager.md#signal-installprogress) (double progress) <br> |
| signal void | [**installTriggered**](classPluginManager.md#signal-installtriggered) (const QString & name) <br> |
| signal void | [**pluginModelChanged**](classPluginManager.md#signal-pluginmodelchanged)  <br> |
| signal void | [**pluginPermissionRequested**](classPluginManager.md#signal-pluginpermissionrequested) (const QString & pluginName, bool isProjectPlugin) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginManager**](#function-pluginmanager) (QQmlEngine \* engine) <br> |
|  QList&lt; [**PluginInformation**](classPluginInformation.md) &gt; | [**availableAppPlugins**](#function-availableappplugins-22) () const<br> |
|  Q\_INVOKABLE void | [**clearPluginPermissions**](#function-clearpluginpermissions) () <br> |
|  Q\_INVOKABLE void | [**configureAppPlugin**](#function-configureappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**denyRequestedPluginPermission**](#function-denyrequestedpluginpermission) (bool permanent=false) <br> |
|  Q\_INVOKABLE void | [**disableAppPlugin**](#function-disableappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**enableAppPlugin**](#function-enableappplugin) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**grantRequestedPluginPermission**](#function-grantrequestedpluginpermission) (bool permanent=false) <br> |
|  Q\_INVOKABLE void | [**installFromRepository**](#function-installfromrepository) (const QString & uuid) <br> |
|  Q\_INVOKABLE void | [**installFromUrl**](#function-installfromurl) (const QString & url) <br> |
|  Q\_INVOKABLE bool | [**isAppPluginConfigurable**](#function-isapppluginconfigurable) (const QString & uuid) const<br> |
|  Q\_INVOKABLE bool | [**isAppPluginEnabled**](#function-isapppluginenabled) (const QString & uuid) const<br> |
|  void | [**loadPlugin**](#function-loadplugin) (const QString & pluginPath, const QString & pluginName, bool skipPermissionCheck=false, bool isProjectPlugin=false) <br> |
|  [**PluginModel**](classPluginModel.md) \* | [**pluginModel**](#function-pluginmodel-22) () const<br> |
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
QList<PluginInformation> PluginManager::availableAppPlugins;
```




<hr>



### property pluginModel [1/2]

```C++
PluginModel * PluginManager::pluginModel;
```




<hr>
## Public Signals Documentation




### signal appPluginDisabled 

```C++
void PluginManager::appPluginDisabled;
```




<hr>



### signal appPluginEnabled 

```C++
void PluginManager::appPluginEnabled;
```




<hr>



### signal availableAppPluginsChanged 

```C++
void PluginManager::availableAppPluginsChanged;
```




<hr>



### signal installEnded 

```C++
void PluginManager::installEnded;
```




<hr>



### signal installProgress 

```C++
void PluginManager::installProgress;
```




<hr>



### signal installTriggered 

```C++
void PluginManager::installTriggered;
```




<hr>



### signal pluginModelChanged 

```C++
void PluginManager::pluginModelChanged;
```




<hr>



### signal pluginPermissionRequested 

```C++
void PluginManager::pluginPermissionRequested;
```




<hr>
## Public Functions Documentation




### function PluginManager 

```C++
explicit PluginManager::PluginManager (
    QQmlEngine * engine
) 
```



Constructs a [**PluginManager**](classPluginManager.md) with the given _engine_. 


        

<hr>



### function availableAppPlugins [2/2]

```C++
QList< PluginInformation > PluginManager::availableAppPlugins () const
```



Returns the list of available app plugins installed locally. 


        

<hr>



### function clearPluginPermissions 

```C++
Q_INVOKABLE void PluginManager::clearPluginPermissions () 
```



Clears all saved plugin permissions except those explicitly enabled by the user. 


        

<hr>



### function configureAppPlugin 

```C++
Q_INVOKABLE void PluginManager::configureAppPlugin (
    const QString & uuid
) 
```



Opens the configuration interface for the plugin identified by _uuid_. 


        

<hr>



### function denyRequestedPluginPermission 

```C++
Q_INVOKABLE void PluginManager::denyRequestedPluginPermission (
    bool permanent=false
) 
```



Denies permission to the last plugin that requested it. If _permanent_ is true, saves this choice. 


        

<hr>



### function disableAppPlugin 

```C++
Q_INVOKABLE void PluginManager::disableAppPlugin (
    const QString & uuid
) 
```



Disables the application plugin identified by _uuid_. 


        

<hr>



### function enableAppPlugin 

```C++
Q_INVOKABLE void PluginManager::enableAppPlugin (
    const QString & uuid
) 
```



Enables the application plugin identified by _uuid_. 


        

<hr>



### function grantRequestedPluginPermission 

```C++
Q_INVOKABLE void PluginManager::grantRequestedPluginPermission (
    bool permanent=false
) 
```



Grants permission to the last plugin that requested it. If _permanent_ is true, saves this choice. 


        

<hr>



### function installFromRepository 

```C++
Q_INVOKABLE void PluginManager::installFromRepository (
    const QString & uuid
) 
```



Installs a plugin from the given repository _uuid_. 


        

<hr>



### function installFromUrl 

```C++
Q_INVOKABLE void PluginManager::installFromUrl (
    const QString & url
) 
```



Installs a plugin from the given _url_. 


        

<hr>



### function isAppPluginConfigurable 

```C++
Q_INVOKABLE bool PluginManager::isAppPluginConfigurable (
    const QString & uuid
) const
```



Returns true if the application plugin with _uuid_ is configurable. 


        

<hr>



### function isAppPluginEnabled 

```C++
Q_INVOKABLE bool PluginManager::isAppPluginEnabled (
    const QString & uuid
) const
```



Returns true if the application plugin with _uuid_ is currently enabled. 


        

<hr>



### function loadPlugin 

```C++
void PluginManager::loadPlugin (
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
PluginModel * PluginManager::pluginModel () const
```



Returns the plugin model used by this manager. 


        

<hr>



### function restoreAppPlugins 

```C++
void PluginManager::restoreAppPlugins () 
```



Restores and loads previously enabled application plugins. 


        

<hr>



### function uninstall 

```C++
Q_INVOKABLE void PluginManager::uninstall (
    const QString & uuid
) 
```



Uninstalls the application plugin identified by _uuid_. 


        

<hr>



### function unloadPlugin 

```C++
void PluginManager::unloadPlugin (
    const QString & pluginPath
) 
```



Unloads the plugin loaded from _pluginPath_. 


        

<hr>



### function unloadPlugins 

```C++
void PluginManager::unloadPlugins () 
```



Unloads all currently loaded plugins. 


        

<hr>
## Public Static Functions Documentation




### function findProjectPlugin 

```C++
static QString PluginManager::findProjectPlugin (
    const QString & projectPath
) 
```



Finds and returns the plugin path corresponding to a project at _projectPath_. Returns an empty string if none is found. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/pluginmanager.h`

