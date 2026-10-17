

# Class QfRecentProjectListModel



[**ClassList**](annotated.md) **>** [**QfRecentProjectListModel**](classQfRecentProjectListModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**RecentProject**](structQfRecentProjectListModel_1_1RecentProject.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ProjectType**](#enum-projecttype)  <br> |
| enum  | [**Role**](#enum-role)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**projectChanged**](classQfRecentProjectListModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfRecentProjectListModel**](#function-qfrecentprojectlistmodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE void | [**reloadModel**](#function-reloadmodel) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QList&lt; [**RecentProject**](structQfRecentProjectListModel_1_1RecentProject.md) &gt; | [**recentProjects**](#function-recentprojects) (bool skipNonAvailable=false) <br> |
|  Q\_INVOKABLE void | [**removeRecentProject**](#function-removerecentproject) (const QString & path) <br> |
|  void | [**saveRecentProjects**](#function-saverecentprojects) (const QList&lt; [**RecentProject**](structQfRecentProjectListModel_1_1RecentProject.md) &gt; & projects) <br> |


























## Public Types Documentation




### enum ProjectType 

```C++
enum QfRecentProjectListModel::ProjectType {
    LocalProject,
    CloudProject,
    LinkProject,
    LocalDataset
};
```




<hr>



### enum Role 

```C++
enum QfRecentProjectListModel::Role {
    ProjectTypeRole = Qt::UserRole,
    ProjectTitleRole,
    ProjectPathRole,
    ProjectThumbnailRole
};
```



Roles to get the data of the model. 


        

<hr>
## Public Signals Documentation




### signal projectChanged 

```C++
void QfRecentProjectListModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfRecentProjectListModel 

```C++
explicit QfRecentProjectListModel::QfRecentProjectListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfRecentProjectListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function reloadModel 

```C++
Q_INVOKABLE void QfRecentProjectListModel::reloadModel () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfRecentProjectListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfRecentProjectListModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>
## Public Static Functions Documentation




### function recentProjects 

```C++
static QList< RecentProject > QfRecentProjectListModel::recentProjects (
    bool skipNonAvailable=false
) 
```




<hr>



### function removeRecentProject 

```C++
static Q_INVOKABLE void QfRecentProjectListModel::removeRecentProject (
    const QString & path
) 
```




<hr>



### function saveRecentProjects 

```C++
static void QfRecentProjectListModel::saveRecentProjects (
    const QList< RecentProject > & projects
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfrecentprojectlistmodel.h`

