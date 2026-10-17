

# Class QfPluginProxyModel



[**ClassList**](annotated.md) **>** [**QfPluginProxyModel**](classQfPluginProxyModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**PluginFilter**](#enum-pluginfilter)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property PluginFilter | [**filter**](classQfPluginProxyModel.md#property-filter-12)  <br> |
| property QString | [**searchTerm**](classQfPluginProxyModel.md#property-searchterm-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filterChanged**](classQfPluginProxyModel.md#signal-filterchanged)  <br> |
| signal void | [**searchTermChanged**](classQfPluginProxyModel.md#signal-searchtermchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPluginProxyModel**](#function-qfpluginproxymodel) (QObject \* parent=nullptr) <br> |
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
enum QfPluginProxyModel::PluginFilter {
    LocalPlugin,
    RemotePlugin
};
```




<hr>
## Public Properties Documentation




### property filter [1/2]

```C++
PluginFilter QfPluginProxyModel::filter;
```




<hr>



### property searchTerm [1/2]

```C++
QString QfPluginProxyModel::searchTerm;
```




<hr>
## Public Signals Documentation




### signal filterChanged 

```C++
void QfPluginProxyModel::filterChanged;
```




<hr>



### signal searchTermChanged 

```C++
void QfPluginProxyModel::searchTermChanged;
```




<hr>
## Public Functions Documentation




### function QfPluginProxyModel 

```C++
explicit QfPluginProxyModel::QfPluginProxyModel (
    QObject * parent=nullptr
) 
```




<hr>



### function filter [2/2]

```C++
PluginFilter QfPluginProxyModel::filter () const
```



Returns the current plugin filter. 


        

<hr>



### function searchTerm [2/2]

```C++
QString QfPluginProxyModel::searchTerm () const
```



Returns the current search term used to filter items. 


        

<hr>



### function setFilter 

```C++
void QfPluginProxyModel::setFilter (
    PluginFilter filter
) 
```



Sets the plugin _filter_. 


        

<hr>



### function setSearchTerm 

```C++
void QfPluginProxyModel::setSearchTerm (
    const QString & searchTerm
) 
```



Sets the search term for filtering and updates the filter. 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool QfPluginProxyModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function lessThan 

```C++
bool QfPluginProxyModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfpluginmodel.h`

