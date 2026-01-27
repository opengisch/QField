

# Class RecentProjectListModel



[**ClassList**](annotated.md) **>** [**RecentProjectListModel**](classRecentProjectListModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**RecentProject**](structRecentProjectListModel_1_1RecentProject.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ProjectType**](#enum-projecttype)  <br> |
| enum  | [**Role**](#enum-role)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**projectChanged**](classRecentProjectListModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RecentProjectListModel**](#function-recentprojectlistmodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE void | [**reloadModel**](#function-reloadmodel) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |




























## Public Types Documentation




### enum ProjectType 

```C++
enum RecentProjectListModel::ProjectType {
    LocalProject,
    CloudProject,
    LocalDataset
};
```




<hr>



### enum Role 

```C++
enum RecentProjectListModel::Role {
    ProjectTypeRole = Qt::UserRole,
    ProjectTitleRole,
    ProjectPathRole,
    ProjectSampleRole
};
```



Roles to get the data of the model. 


        

<hr>
## Public Signals Documentation




### signal projectChanged 

```C++
void RecentProjectListModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function RecentProjectListModel 

```C++
explicit RecentProjectListModel::RecentProjectListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant RecentProjectListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function reloadModel 

```C++
Q_INVOKABLE void RecentProjectListModel::reloadModel () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > RecentProjectListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int RecentProjectListModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/recentprojectlistmodel.h`

