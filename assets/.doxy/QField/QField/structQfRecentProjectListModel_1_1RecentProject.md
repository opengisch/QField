

# Struct QfRecentProjectListModel::RecentProject



[**ClassList**](annotated.md) **>** [**QfRecentProjectListModel**](classQfRecentProjectListModel.md) **>** [**RecentProject**](structQfRecentProjectListModel_1_1RecentProject.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**path**](#variable-path)  <br> |
|  QString | [**thumbnail**](#variable-thumbnail)  <br> |
|  QString | [**title**](#variable-title)  <br> |
|  ProjectType | [**type**](#variable-type)   = `ProjectType::LocalProject`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RecentProject**](#function-recentproject-12) () = default<br> |
|   | [**RecentProject**](#function-recentproject-22) (ProjectType type, const QString & title, const QString & path, const QString & thumbnail=QString()) <br> |




























## Public Attributes Documentation




### variable path 

```C++
QString QfRecentProjectListModel::RecentProject::path;
```




<hr>



### variable thumbnail 

```C++
QString QfRecentProjectListModel::RecentProject::thumbnail;
```




<hr>



### variable title 

```C++
QString QfRecentProjectListModel::RecentProject::title;
```




<hr>



### variable type 

```C++
ProjectType QfRecentProjectListModel::RecentProject::type;
```




<hr>
## Public Functions Documentation




### function RecentProject [1/2]

```C++
QfRecentProjectListModel::RecentProject::RecentProject () = default
```




<hr>



### function RecentProject [2/2]

```C++
inline QfRecentProjectListModel::RecentProject::RecentProject (
    ProjectType type,
    const QString & title,
    const QString & path,
    const QString & thumbnail=QString()
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfrecentprojectlistmodel.h`

