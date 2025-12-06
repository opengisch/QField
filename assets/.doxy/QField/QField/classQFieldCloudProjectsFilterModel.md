

# Class QFieldCloudProjectsFilterModel



[**ClassList**](annotated.md) **>** [**QFieldCloudProjectsFilterModel**](classQFieldCloudProjectsFilterModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ProjectsFilter**](#enum-projectsfilter)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property ProjectsFilter | [**filter**](classQFieldCloudProjectsFilterModel.md#property-filter-12)  <br> |
| property [**QFieldCloudProjectsModel**](classQFieldCloudProjectsModel.md) \* | [**projectsModel**](classQFieldCloudProjectsFilterModel.md#property-projectsmodel-12)  <br> |
| property bool | [**showFeaturedOnTop**](classQFieldCloudProjectsFilterModel.md#property-showfeaturedontop-12)  <br> |
| property bool | [**showInValidProjects**](classQFieldCloudProjectsFilterModel.md#property-showinvalidprojects-12)  <br> |
| property bool | [**showLocalOnly**](classQFieldCloudProjectsFilterModel.md#property-showlocalonly-12)  <br> |
| property QString | [**textFilter**](classQFieldCloudProjectsFilterModel.md#property-textfilter-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filterChanged**](classQFieldCloudProjectsFilterModel.md#signal-filterchanged)  <br> |
| signal void | [**projectsModelChanged**](classQFieldCloudProjectsFilterModel.md#signal-projectsmodelchanged)  <br> |
| signal void | [**showFeaturedOnTopChanged**](classQFieldCloudProjectsFilterModel.md#signal-showfeaturedontopchanged)  <br> |
| signal void | [**showInValidProjectsChanged**](classQFieldCloudProjectsFilterModel.md#signal-showinvalidprojectschanged)  <br> |
| signal void | [**showLocalOnlyChanged**](classQFieldCloudProjectsFilterModel.md#signal-showlocalonlychanged)  <br> |
| signal void | [**textFilterChanged**](classQFieldCloudProjectsFilterModel.md#signal-textfilterchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldCloudProjectsFilterModel**](#function-qfieldcloudprojectsfiltermodel) (QObject \* parent=nullptr) <br> |
|  ProjectsFilter | [**filter**](#function-filter-22) () const<br> |
|  [**QFieldCloudProjectsModel**](classQFieldCloudProjectsModel.md) \* | [**projectsModel**](#function-projectsmodel-22) () const<br> |
|  void | [**setFilter**](#function-setfilter) (ProjectsFilter filter) <br> |
|  void | [**setProjectsModel**](#function-setprojectsmodel) ([**QFieldCloudProjectsModel**](classQFieldCloudProjectsModel.md) \* projectsModel) <br> |
|  void | [**setShowFeaturedOnTop**](#function-setshowfeaturedontop) (bool showFeaturedOnTop) <br> |
|  void | [**setShowInValidProjects**](#function-setshowinvalidprojects) (bool showInValidProjects) <br> |
|  void | [**setShowLocalOnly**](#function-setshowlocalonly) (bool showLocalOnly) <br> |
|  void | [**setTextFilter**](#function-settextfilter) (const QString & text) <br> |
|  bool | [**showFeaturedOnTop**](#function-showfeaturedontop-22) () const<br> |
|  bool | [**showInValidProjects**](#function-showinvalidprojects-22) () const<br> |
|  bool | [**showLocalOnly**](#function-showlocalonly-22) () const<br> |
|  QString | [**textFilter**](#function-textfilter-22) () const<br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int source\_row, const QModelIndex & source\_parent) override const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |




## Public Types Documentation




### enum ProjectsFilter 

```C++
enum QFieldCloudProjectsFilterModel::ProjectsFilter {
    PrivateProjects,
    PublicProjects
};
```




<hr>
## Public Properties Documentation




### property filter [1/2]

```C++
ProjectsFilter QFieldCloudProjectsFilterModel::filter;
```




<hr>



### property projectsModel [1/2]

```C++
QFieldCloudProjectsModel * QFieldCloudProjectsFilterModel::projectsModel;
```




<hr>



### property showFeaturedOnTop [1/2]

```C++
bool QFieldCloudProjectsFilterModel::showFeaturedOnTop;
```




<hr>



### property showInValidProjects [1/2]

```C++
bool QFieldCloudProjectsFilterModel::showInValidProjects;
```




<hr>



### property showLocalOnly [1/2]

```C++
bool QFieldCloudProjectsFilterModel::showLocalOnly;
```




<hr>



### property textFilter [1/2]

```C++
QString QFieldCloudProjectsFilterModel::textFilter;
```




<hr>
## Public Signals Documentation




### signal filterChanged 

```C++
void QFieldCloudProjectsFilterModel::filterChanged;
```




<hr>



### signal projectsModelChanged 

```C++
void QFieldCloudProjectsFilterModel::projectsModelChanged;
```




<hr>



### signal showFeaturedOnTopChanged 

```C++
void QFieldCloudProjectsFilterModel::showFeaturedOnTopChanged;
```




<hr>



### signal showInValidProjectsChanged 

```C++
void QFieldCloudProjectsFilterModel::showInValidProjectsChanged;
```




<hr>



### signal showLocalOnlyChanged 

```C++
void QFieldCloudProjectsFilterModel::showLocalOnlyChanged;
```




<hr>



### signal textFilterChanged 

```C++
void QFieldCloudProjectsFilterModel::textFilterChanged;
```




<hr>
## Public Functions Documentation




### function QFieldCloudProjectsFilterModel 

```C++
explicit QFieldCloudProjectsFilterModel::QFieldCloudProjectsFilterModel (
    QObject * parent=nullptr
) 
```




<hr>



### function filter [2/2]

```C++
ProjectsFilter QFieldCloudProjectsFilterModel::filter () const
```



Returns the current cloud projects filter. 


        

<hr>



### function projectsModel [2/2]

```C++
QFieldCloudProjectsModel * QFieldCloudProjectsFilterModel::projectsModel () const
```



Returns the source cloud projects model from which the filtered list is derived. 


        

<hr>



### function setFilter 

```C++
void QFieldCloudProjectsFilterModel::setFilter (
    ProjectsFilter filter
) 
```



Sets the cloud project _filter_. 


        

<hr>



### function setProjectsModel 

```C++
void QFieldCloudProjectsFilterModel::setProjectsModel (
    QFieldCloudProjectsModel * projectsModel
) 
```



Sets the source cloud projects model from which the filtered list is derived. 

**Parameters:**


* `projectsModel` the source cloud project model 




        

<hr>



### function setShowFeaturedOnTop 

```C++
void QFieldCloudProjectsFilterModel::setShowFeaturedOnTop (
    bool showFeaturedOnTop
) 
```



Sets whether featured projects will be shown on top of the list. 


        

<hr>



### function setShowInValidProjects 

```C++
void QFieldCloudProjectsFilterModel::setShowInValidProjects (
    bool showInValidProjects
) 
```



Sets whether to include projects that are considered invalid (i.e., failing status) in the filtered list of cloud projects.




**Parameters:**


* `showInValidProjects` If true, invalid (failing) projects will be shown; otherwise, they will be filtered out. 




        

<hr>



### function setShowLocalOnly 

```C++
void QFieldCloudProjectsFilterModel::setShowLocalOnly (
    bool showLocalOnly
) 
```



Sets whether the filtered cloud projects list will only contain those available locally. 


        

<hr>



### function setTextFilter 

```C++
void QFieldCloudProjectsFilterModel::setTextFilter (
    const QString & text
) 
```



Sets a _text_ string filter projects by matching it against the project's name or description as well well as owner's name. 


        

<hr>



### function showFeaturedOnTop [2/2]

```C++
bool QFieldCloudProjectsFilterModel::showFeaturedOnTop () const
```



Returns TRUE if featured projects will be shown on top of the list. 


        

<hr>



### function showInValidProjects [2/2]

```C++
bool QFieldCloudProjectsFilterModel::showInValidProjects () const
```



Returns TRUE if invalid (failing) projects are currently included in the filtered list of cloud projects. 


        

<hr>



### function showLocalOnly [2/2]

```C++
bool QFieldCloudProjectsFilterModel::showLocalOnly () const
```



Returns whether the filtered cloud projects list will only contain those available locally. 


        

<hr>



### function textFilter [2/2]

```C++
QString QFieldCloudProjectsFilterModel::textFilter () const
```



Returns the current text string used to filter projects. 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
virtual bool QFieldCloudProjectsFilterModel::filterAcceptsRow (
    int source_row,
    const QModelIndex & source_parent
) override const
```




<hr>



### function lessThan 

```C++
bool QFieldCloudProjectsFilterModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfieldcloudprojectsmodel.h`

