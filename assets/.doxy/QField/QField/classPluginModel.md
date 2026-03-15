

# Class PluginModel



[**ClassList**](annotated.md) **>** [**PluginModel**](classPluginModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**PluginRoles**](#enum-pluginroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isRefreshing**](classPluginModel.md#property-isrefreshing-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**isRefreshingChanged**](classPluginModel.md#signal-isrefreshingchanged)  <br> |
| signal void | [**remoteFetched**](classPluginModel.md#signal-remotefetched)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginModel**](#function-pluginmodel) ([**PluginManager**](classPluginManager.md) \* manager, QObject \* parent=nullptr) <br> |
|  QList&lt; [**PluginInformation**](classPluginInformation.md) &gt; | [**availableAppPlugins**](#function-availableappplugins) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  bool | [**hasPluginInformation**](#function-hasplugininformation) (const QString & uuid) const<br> |
|  bool | [**isRefreshing**](#function-isrefreshing-22) () const<br> |
|  [**PluginInformation**](classPluginInformation.md) | [**pluginInformation**](#function-plugininformation) (const QString & uuid) const<br> |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) (bool fetchRemote=false) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |
|  Q\_INVOKABLE void | [**updatePluginEnabledStateByUuid**](#function-updatepluginenabledstatebyuuid) (const QString & uuid, bool enabled, bool configurable) <br> |




























## Public Types Documentation




### enum PluginRoles 

```C++
enum PluginModel::PluginRoles {
    UuidRole = Qt::UserRole + 1,
    TrustedRole,
    BundledRole,
    EnabledRole,
    ConfigurableRole,
    NameRole,
    DescriptionRole,
    AuthorRole,
    HomepageRole,
    IconRole,
    VersionRole,
    InstalledLocallyRole,
    AvailableRemotelyRole,
    AvailableUpdateRole
};
```




<hr>
## Public Properties Documentation




### property isRefreshing [1/2]

```C++
bool PluginModel::isRefreshing;
```




<hr>
## Public Signals Documentation




### signal isRefreshingChanged 

```C++
void PluginModel::isRefreshingChanged;
```




<hr>



### signal remoteFetched 

```C++
void PluginModel::remoteFetched;
```




<hr>
## Public Functions Documentation




### function PluginModel 

```C++
explicit PluginModel::PluginModel (
    PluginManager * manager,
    QObject * parent=nullptr
) 
```



Constructs a [**PluginModel**](classPluginModel.md). 


        

<hr>



### function availableAppPlugins 

```C++
QList< PluginInformation > PluginModel::availableAppPlugins () const
```



Returns the list of available app plugins installed locally. 


        

<hr>



### function data 

```C++
QVariant PluginModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```



Returns the data for _index_ and _role_. 


        

<hr>



### function hasPluginInformation 

```C++
bool PluginModel::hasPluginInformation (
    const QString & uuid
) const
```



Returns true if the model contains a plugin with the given _uuid_. 


        

<hr>



### function isRefreshing [2/2]

```C++
bool PluginModel::isRefreshing () const
```



Returns true if the model is currently fetching remote plugins. 


        

<hr>



### function pluginInformation 

```C++
PluginInformation PluginModel::pluginInformation (
    const QString & uuid
) const
```



Returns the pluginInformation information for the pluginInformation identified by _uuid_. 


        

<hr>



### function refresh 

```C++
Q_INVOKABLE void PluginModel::refresh (
    bool fetchRemote=false
) 
```



Refreshes the model. 

**Parameters:**


* `fetchRemote` set to TRUE to fetch remotely available plugins 




        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > PluginModel::roleNames () override const
```



Returns the role names used by this model. 


        

<hr>



### function rowCount 

```C++
int PluginModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```



Returns the number of plugins in the model. 


        

<hr>



### function setData 

```C++
bool PluginModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```



Sets the data for _index_ and _role_ to _value_. 


        

<hr>



### function updatePluginEnabledStateByUuid 

```C++
Q_INVOKABLE void PluginModel::updatePluginEnabledStateByUuid (
    const QString & uuid,
    bool enabled,
    bool configurable
) 
```



Updates the enabled and configurable state of the plugin with _uuid_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/pluginmodel.h`

