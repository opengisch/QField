

# Struct RecentProjectListModel::RecentProject



[**ClassList**](annotated.md) **>** [**RecentProjectListModel**](classRecentProjectListModel.md) **>** [**RecentProject**](structRecentProjectListModel_1_1RecentProject.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**path**](#variable-path)  <br> |
|  bool | [**sample**](#variable-sample)   = `false`<br> |
|  QString | [**title**](#variable-title)  <br> |
|  ProjectType | [**type**](#variable-type)   = `ProjectType::LocalProject`<br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RecentProject**](#function-recentproject-12) () = default<br> |
|   | [**RecentProject**](#function-recentproject-22) (ProjectType type, const QString & title, const QString & path, bool sample) <br> |




























## Public Attributes Documentation




### variable path 

```C++
QString RecentProjectListModel::RecentProject::path;
```




<hr>



### variable sample 

```C++
bool RecentProjectListModel::RecentProject::sample;
```




<hr>



### variable title 

```C++
QString RecentProjectListModel::RecentProject::title;
```




<hr>



### variable type 

```C++
ProjectType RecentProjectListModel::RecentProject::type;
```




<hr>
## Public Functions Documentation




### function RecentProject [1/2]

```C++
RecentProjectListModel::RecentProject::RecentProject () = default
```




<hr>



### function RecentProject [2/2]

```C++
inline RecentProjectListModel::RecentProject::RecentProject (
    ProjectType type,
    const QString & title,
    const QString & path,
    bool sample
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/recentprojectlistmodel.h`

