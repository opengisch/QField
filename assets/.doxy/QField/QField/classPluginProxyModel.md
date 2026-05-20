

# Class PluginProxyModel



[**ClassList**](annotated.md) **>** [**PluginProxyModel**](classPluginProxyModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**PluginFilter**](#enum-pluginfilter)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property PluginFilter | [**filter**](classPluginProxyModel.md#property-filter-12)  <br> |
| property QString | [**searchTerm**](classPluginProxyModel.md#property-searchterm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filterChanged**](classPluginProxyModel.md#signal-filterchanged)  <br> |
| signal void | [**searchTermChanged**](classPluginProxyModel.md#signal-searchtermchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PluginProxyModel**](#function-pluginproxymodel) (QObject \* parent=nullptr) <br> |
|  PluginFilter | [**filter**](#function-filter-22) () const<br> |
|  QString | [**searchTerm**](#function-searchterm-22) () const<br> |
|  void | [**setFilter**](#function-setfilter) (PluginFilter filter) <br> |
|  void | [**setSearchTerm**](#function-setsearchterm) (const QString & searchTerm) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |




## Public Types Documentation




### enum PluginFilter 

```C++
enum PluginProxyModel::PluginFilter {
    LocalPlugin,
    RemotePlugin
};
```




<hr>
## Public Properties Documentation




### property filter [1/2]

```C++
PluginFilter PluginProxyModel::filter;
```




<hr>



### property searchTerm [1/2]

```C++
QString PluginProxyModel::searchTerm;
```




<hr>
## Public Signals Documentation




### signal filterChanged 

```C++
void PluginProxyModel::filterChanged;
```




<hr>



### signal searchTermChanged 

```C++
void PluginProxyModel::searchTermChanged;
```




<hr>
## Public Functions Documentation




### function PluginProxyModel 

```C++
explicit PluginProxyModel::PluginProxyModel (
    QObject * parent=nullptr
) 
```




<hr>



### function filter [2/2]

```C++
PluginFilter PluginProxyModel::filter () const
```



Returns the current plugin filter. 


        

<hr>



### function searchTerm [2/2]

```C++
QString PluginProxyModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setFilter 

```C++
void PluginProxyModel::setFilter (
    PluginFilter filter
) 
```



Sets the plugin _filter_. 


        

<hr>



### function setSearchTerm 

```C++
void PluginProxyModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool PluginProxyModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function lessThan 

```C++
bool PluginProxyModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/pluginmodel.h`

