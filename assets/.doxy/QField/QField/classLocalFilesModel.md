

# Class LocalFilesModel



[**ClassList**](annotated.md) **>** [**LocalFilesModel**](classLocalFilesModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ItemMetaType**](#enum-itemmetatype)  <br> |
| enum  | [**ItemType**](#enum-itemtype)  <br> |
| enum  | [**Role**](#enum-role)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**currentDepth**](classLocalFilesModel.md#property-currentdepth-12)  <br> |
| property QString | [**currentPath**](classLocalFilesModel.md#property-currentpath-12)  <br> |
| property QString | [**currentTitle**](classLocalFilesModel.md#property-currenttitle-12)  <br> |
| property bool | [**inSelectionMode**](classLocalFilesModel.md#property-inselectionmode-12)  <br> |
| property bool | [**isDeletedAllowedInCurrentPath**](classLocalFilesModel.md#property-isdeletedallowedincurrentpath-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentPathChanged**](classLocalFilesModel.md#signal-currentpathchanged)  <br> |
| signal void | [**inSelectionModeChanged**](classLocalFilesModel.md#signal-inselectionmodechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LocalFilesModel**](#function-localfilesmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addToFavorites**](#function-addtofavorites) (const QString & path) <br>_Adds the_ _path_ _to the favorites list._ |
|  Q\_INVOKABLE void | [**clearSelection**](#function-clearselection) () <br>_Set checked state of all items to false._  |
|  int | [**currentDepth**](#function-currentdepth-22) () const<br>_Returns the current navigation history depth._  |
|  QString | [**currentPath**](#function-currentpath-22) () const<br>_Returns the current path._  |
|  QString | [**currentTitle**](#function-currenttitle-22) () const<br>_Returns the current title derived from the current path._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE [**LocalFileItem**](classLocalFileItem.md) | [**get**](#function-get) (int index) const<br>_Returns the_ [_**LocalFileItem**_](classLocalFileItem.md) _at the given__index_ _._ |
|  bool | [**inSelectionMode**](#function-inselectionmode-22) () <br>_Returns whether list is in multi-selection mode or not._  |
|  bool | [**isDeletedAllowedInCurrentPath**](#function-isdeletedallowedincurrentpath-22) () const<br> |
|  Q\_INVOKABLE bool | [**isPathFavoriteEditable**](#function-ispathfavoriteeditable) (const QString & path) <br>_Returns whether_ _path's_ _favorite state is editable._ |
|  Q\_INVOKABLE void | [**moveUp**](#function-moveup) () <br>_Walks the navigation history back up on step._  |
|  Q\_INVOKABLE void | [**removeFromFavorites**](#function-removefromfavorites) (const QString & path) <br>_Removes the_ _path_ _to the favorites list._ |
|  Q\_INVOKABLE void | [**resetToPath**](#function-resettopath) (const QString & path) <br>_Resets the model and sets the first navigation history item to the provided_ _path_ _._ |
|  Q\_INVOKABLE void | [**resetToRoot**](#function-resettoroot) () <br>_Resets the model and sets the first navigation history item to root._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  Q\_INVOKABLE void | [**setChecked**](#function-setchecked) (const int & mIdx, const bool & checked) <br>_Set checked state of an item in list._  |
|  void | [**setCurrentPath**](#function-setcurrentpath) (const QString & path) <br>_Appends the provided_ _path_ _to the navigation history._ |




























## Public Types Documentation




### enum ItemMetaType 

```C++
enum LocalFilesModel::ItemMetaType {
    Favorite,
    Folder,
    File,
    Project,
    Dataset
};
```




<hr>



### enum ItemType 

```C++
enum LocalFilesModel::ItemType {
    ApplicationFolder,
    ExternalStorage,
    SimpleFolder,
    ProjectFile,
    VectorDataset,
    RasterDataset,
    OtherFile
};
```




<hr>



### enum Role 

```C++
enum LocalFilesModel::Role {
    ItemMetaTypeRole = Qt::UserRole,
    ItemTypeRole,
    ItemTitleRole,
    ItemFormatRole,
    ItemPathRole,
    ItemSizeRole,
    ItemHasThumbnailRole,
    ItemIsFavoriteRole,
    ItemHasWebdavConfigurationRole,
    ItemCheckedRole
};
```




<hr>
## Public Properties Documentation




### property currentDepth [1/2]

```C++
int LocalFilesModel::currentDepth;
```




<hr>



### property currentPath [1/2]

```C++
QString LocalFilesModel::currentPath;
```




<hr>



### property currentTitle [1/2]

```C++
QString LocalFilesModel::currentTitle;
```




<hr>



### property inSelectionMode [1/2]

```C++
bool LocalFilesModel::inSelectionMode;
```




<hr>



### property isDeletedAllowedInCurrentPath [1/2]

```C++
bool LocalFilesModel::isDeletedAllowedInCurrentPath;
```




<hr>
## Public Signals Documentation




### signal currentPathChanged 

```C++
void LocalFilesModel::currentPathChanged;
```




<hr>



### signal inSelectionModeChanged 

```C++
void LocalFilesModel::inSelectionModeChanged;
```




<hr>
## Public Functions Documentation




### function LocalFilesModel 

```C++
explicit LocalFilesModel::LocalFilesModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addToFavorites 

_Adds the_ _path_ _to the favorites list._
```C++
Q_INVOKABLE void LocalFilesModel::addToFavorites (
    const QString & path
) 
```




<hr>



### function clearSelection 

_Set checked state of all items to false._ 
```C++
Q_INVOKABLE void LocalFilesModel::clearSelection () 
```




<hr>



### function currentDepth [2/2]

_Returns the current navigation history depth._ 
```C++
inline int LocalFilesModel::currentDepth () const
```




<hr>



### function currentPath [2/2]

_Returns the current path._ 
```C++
QString LocalFilesModel::currentPath () const
```




<hr>



### function currentTitle [2/2]

_Returns the current title derived from the current path._ 
```C++
QString LocalFilesModel::currentTitle () const
```




<hr>



### function data 

```C++
QVariant LocalFilesModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function get 

_Returns the_ [_**LocalFileItem**_](classLocalFileItem.md) _at the given__index_ _._
```C++
Q_INVOKABLE LocalFileItem LocalFilesModel::get (
    int index
) const
```




<hr>



### function inSelectionMode [2/2]

_Returns whether list is in multi-selection mode or not._ 
```C++
bool LocalFilesModel::inSelectionMode () 
```




<hr>



### function isDeletedAllowedInCurrentPath [2/2]

```C++
bool LocalFilesModel::isDeletedAllowedInCurrentPath () const
```



Returns TRUE if deletation of files and folders is allow in the current path 

**Note:**

Currently, deletion is only allowed when the path is a sub-child of an application directory 





        

<hr>



### function isPathFavoriteEditable 

_Returns whether_ _path's_ _favorite state is editable._
```C++
Q_INVOKABLE bool LocalFilesModel::isPathFavoriteEditable (
    const QString & path
) 
```




<hr>



### function moveUp 

_Walks the navigation history back up on step._ 
```C++
Q_INVOKABLE void LocalFilesModel::moveUp () 
```




<hr>



### function removeFromFavorites 

_Removes the_ _path_ _to the favorites list._
```C++
Q_INVOKABLE void LocalFilesModel::removeFromFavorites (
    const QString & path
) 
```




<hr>



### function resetToPath 

_Resets the model and sets the first navigation history item to the provided_ _path_ _._
```C++
Q_INVOKABLE void LocalFilesModel::resetToPath (
    const QString & path
) 
```




<hr>



### function resetToRoot 

_Resets the model and sets the first navigation history item to root._ 
```C++
Q_INVOKABLE void LocalFilesModel::resetToRoot () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > LocalFilesModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int LocalFilesModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setChecked 

_Set checked state of an item in list._ 
```C++
Q_INVOKABLE void LocalFilesModel::setChecked (
    const int & mIdx,
    const bool & checked
) 
```




<hr>



### function setCurrentPath 

_Appends the provided_ _path_ _to the navigation history._
```C++
void LocalFilesModel::setCurrentPath (
    const QString & path
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/localfilesmodel.h`

