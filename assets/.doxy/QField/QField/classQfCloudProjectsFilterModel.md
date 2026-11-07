

# Class QfCloudProjectsFilterModel



[**ClassList**](annotated.md) **>** [**QfCloudProjectsFilterModel**](classQfCloudProjectsFilterModel.md)








Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isSearching**](classQfCloudProjectsFilterModel.md#property-issearching-12)  <br> |
| property [**QfCloudProjectsModel**](classQfCloudProjectsModel.md) \* | [**projectsModel**](classQfCloudProjectsFilterModel.md#property-projectsmodel-12)  <br> |
| property bool | [**showFeaturedOnTop**](classQfCloudProjectsFilterModel.md#property-showfeaturedontop-12)  <br> |
| property bool | [**showInValidProjects**](classQfCloudProjectsFilterModel.md#property-showinvalidprojects-12)  <br> |
| property bool | [**showLocalOnly**](classQfCloudProjectsFilterModel.md#property-showlocalonly-12)  <br> |
| property bool | [**showTemplates**](classQfCloudProjectsFilterModel.md#property-showtemplates-12)  <br> |
| property QString | [**textFilter**](classQfCloudProjectsFilterModel.md#property-textfilter-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filterChanged**](classQfCloudProjectsFilterModel.md#signal-filterchanged)  <br> |
| signal void | [**isSearchingChanged**](classQfCloudProjectsFilterModel.md#signal-issearchingchanged)  <br> |
| signal void | [**projectsModelChanged**](classQfCloudProjectsFilterModel.md#signal-projectsmodelchanged)  <br> |
| signal void | [**showFeaturedOnTopChanged**](classQfCloudProjectsFilterModel.md#signal-showfeaturedontopchanged)  <br> |
| signal void | [**showInValidProjectsChanged**](classQfCloudProjectsFilterModel.md#signal-showinvalidprojectschanged)  <br> |
| signal void | [**showLocalOnlyChanged**](classQfCloudProjectsFilterModel.md#signal-showlocalonlychanged)  <br> |
| signal void | [**showTemplatesChanged**](classQfCloudProjectsFilterModel.md#signal-showtemplateschanged)  <br> |
| signal void | [**textFilterChanged**](classQfCloudProjectsFilterModel.md#signal-textfilterchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudProjectsFilterModel**](#function-qfcloudprojectsfiltermodel) (QObject \* parent=nullptr) <br> |
|  bool | [**isSearching**](#function-issearching-22) () const<br> |
|  [**QfCloudProjectsModel**](classQfCloudProjectsModel.md) \* | [**projectsModel**](#function-projectsmodel-22) () const<br> |
|  void | [**setProjectsModel**](#function-setprojectsmodel) ([**QfCloudProjectsModel**](classQfCloudProjectsModel.md) \* projectsModel) <br> |
|  void | [**setShowFeaturedOnTop**](#function-setshowfeaturedontop) (bool showFeaturedOnTop) <br> |
|  void | [**setShowInValidProjects**](#function-setshowinvalidprojects) (bool showInValidProjects) <br> |
|  void | [**setShowLocalOnly**](#function-setshowlocalonly) (bool showLocalOnly) <br> |
|  void | [**setShowTemplates**](#function-setshowtemplates) (bool showTemplates) <br> |
|  void | [**setTextFilter**](#function-settextfilter) (const QString & text) <br> |
|  bool | [**showFeaturedOnTop**](#function-showfeaturedontop-22) () const<br> |
|  bool | [**showInValidProjects**](#function-showinvalidprojects-22) () const<br> |
|  bool | [**showLocalOnly**](#function-showlocalonly-22) () const<br> |
|  bool | [**showTemplates**](#function-showtemplates-22) () const<br> |
|  QString | [**textFilter**](#function-textfilter-22) () const<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int source\_row, const QModelIndex & source\_parent) override const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |




## Public Properties Documentation




### property isSearching [1/2]

```C++
bool QfCloudProjectsFilterModel::isSearching;
```




<hr>



### property projectsModel [1/2]

```C++
QfCloudProjectsModel * QfCloudProjectsFilterModel::projectsModel;
```




<hr>



### property showFeaturedOnTop [1/2]

```C++
bool QfCloudProjectsFilterModel::showFeaturedOnTop;
```




<hr>



### property showInValidProjects [1/2]

```C++
bool QfCloudProjectsFilterModel::showInValidProjects;
```




<hr>



### property showLocalOnly [1/2]

```C++
bool QfCloudProjectsFilterModel::showLocalOnly;
```




<hr>



### property showTemplates [1/2]

```C++
bool QfCloudProjectsFilterModel::showTemplates;
```




<hr>



### property textFilter [1/2]

```C++
QString QfCloudProjectsFilterModel::textFilter;
```




<hr>
## Public Signals Documentation




### signal filterChanged 

```C++
void QfCloudProjectsFilterModel::filterChanged;
```




<hr>



### signal isSearchingChanged 

```C++
void QfCloudProjectsFilterModel::isSearchingChanged;
```




<hr>



### signal projectsModelChanged 

```C++
void QfCloudProjectsFilterModel::projectsModelChanged;
```




<hr>



### signal showFeaturedOnTopChanged 

```C++
void QfCloudProjectsFilterModel::showFeaturedOnTopChanged;
```




<hr>



### signal showInValidProjectsChanged 

```C++
void QfCloudProjectsFilterModel::showInValidProjectsChanged;
```




<hr>



### signal showLocalOnlyChanged 

```C++
void QfCloudProjectsFilterModel::showLocalOnlyChanged;
```




<hr>



### signal showTemplatesChanged 

```C++
void QfCloudProjectsFilterModel::showTemplatesChanged;
```




<hr>



### signal textFilterChanged 

```C++
void QfCloudProjectsFilterModel::textFilterChanged;
```




<hr>
## Public Functions Documentation




### function QfCloudProjectsFilterModel 

```C++
explicit QfCloudProjectsFilterModel::QfCloudProjectsFilterModel (
    QObject * parent=nullptr
) 
```




<hr>



### function isSearching [2/2]

```C++
bool QfCloudProjectsFilterModel::isSearching () const
```



Returns TRUE while an asynchronous projects appending was triggered by a text filter. 


        

<hr>



### function projectsModel [2/2]

```C++
QfCloudProjectsModel * QfCloudProjectsFilterModel::projectsModel () const
```



Returns the source cloud projects model from which the filtered list is derived. 


        

<hr>



### function setProjectsModel 

```C++
void QfCloudProjectsFilterModel::setProjectsModel (
    QfCloudProjectsModel * projectsModel
) 
```



Sets the source cloud projects model from which the filtered list is derived. 

**Parameters:**


* `projectsModel` the source cloud project model 




        

<hr>



### function setShowFeaturedOnTop 

```C++
void QfCloudProjectsFilterModel::setShowFeaturedOnTop (
    bool showFeaturedOnTop
) 
```



Sets whether featured projects will be shown on top of the list. 


        

<hr>



### function setShowInValidProjects 

```C++
void QfCloudProjectsFilterModel::setShowInValidProjects (
    bool showInValidProjects
) 
```



Sets whether to include projects that are considered invalid (i.e., failing status) in the filtered list of cloud projects.




**Parameters:**


* `showInValidProjects` If true, invalid (failing) projects will be shown; otherwise, they will be filtered out. 




        

<hr>



### function setShowLocalOnly 

```C++
void QfCloudProjectsFilterModel::setShowLocalOnly (
    bool showLocalOnly
) 
```



Sets whether the filtered cloud projects list will only contain those available locally. 


        

<hr>



### function setShowTemplates 

```C++
void QfCloudProjectsFilterModel::setShowTemplates (
    bool showTemplates
) 
```



Sets whether the filtered list shows template projects only. 

**Parameters:**


* `showTemplates` when TRUE, only template projects are shown; when FALSE, they are hidden 




        

<hr>



### function setTextFilter 

```C++
void QfCloudProjectsFilterModel::setTextFilter (
    const QString & text
) 
```



Sets a _text_ string filter projects by matching it against the project's name or description as well well as owner's name. 


        

<hr>



### function showFeaturedOnTop [2/2]

```C++
bool QfCloudProjectsFilterModel::showFeaturedOnTop () const
```



Returns TRUE if featured projects will be shown on top of the list. 


        

<hr>



### function showInValidProjects [2/2]

```C++
bool QfCloudProjectsFilterModel::showInValidProjects () const
```



Returns TRUE if invalid (failing) projects are currently included in the filtered list of cloud projects. 


        

<hr>



### function showLocalOnly [2/2]

```C++
bool QfCloudProjectsFilterModel::showLocalOnly () const
```



Returns whether the filtered cloud projects list will only contain those available locally. 


        

<hr>



### function showTemplates [2/2]

```C++
bool QfCloudProjectsFilterModel::showTemplates () const
```



Returns TRUE when the filtered list shows template projects only, hiding all non-template projects. When FALSE, template projects are excluded from the list. 


        

<hr>



### function textFilter [2/2]

```C++
QString QfCloudProjectsFilterModel::textFilter () const
```



Returns the current text string used to filter projects. 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
virtual bool QfCloudProjectsFilterModel::filterAcceptsRow (
    int source_row,
    const QModelIndex & source_parent
) override const
```




<hr>



### function lessThan 

```C++
bool QfCloudProjectsFilterModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudprojectsmodel.h`

