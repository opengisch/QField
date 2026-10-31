

# Class QfLocalFilesModel



[**ClassList**](annotated.md) **>** [**QfLocalFilesModel**](classQfLocalFilesModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**CloudStatus**](#enum-cloudstatus)  <br>_Synchronization status of a local file within a cloud project, compared against the files stored on QFieldCloud._  |
| enum  | [**ItemMetaType**](#enum-itemmetatype)  <br> |
| enum  | [**ItemType**](#enum-itemtype)  <br> |
| enum  | [**Role**](#enum-role)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](classQfLocalFilesModel.md#property-cloudconnection-12)  <br> |
| property int | [**currentDepth**](classQfLocalFilesModel.md#property-currentdepth-12)  <br> |
| property QString | [**currentPath**](classQfLocalFilesModel.md#property-currentpath-12)  <br> |
| property QString | [**currentTitle**](classQfLocalFilesModel.md#property-currenttitle-12)  <br> |
| property bool | [**inSelectionMode**](classQfLocalFilesModel.md#property-inselectionmode-12)  <br> |
| property bool | [**isDeletedAllowedInCurrentPath**](classQfLocalFilesModel.md#property-isdeletedallowedincurrentpath-12)  <br> |
| property bool | [**isFetchingCloudStatus**](classQfLocalFilesModel.md#property-isfetchingcloudstatus-12)  <br> |
| property int | [**selectedCount**](classQfLocalFilesModel.md#property-selectedcount-12)  <br> |
| property qint64 | [**selectedSize**](classQfLocalFilesModel.md#property-selectedsize-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudConnectionChanged**](classQfLocalFilesModel.md#signal-cloudconnectionchanged)  <br> |
| signal void | [**currentPathChanged**](classQfLocalFilesModel.md#signal-currentpathchanged)  <br> |
| signal void | [**inSelectionModeChanged**](classQfLocalFilesModel.md#signal-inselectionmodechanged)  <br> |
| signal void | [**isFetchingCloudStatusChanged**](classQfLocalFilesModel.md#signal-isfetchingcloudstatuschanged)  <br> |
| signal void | [**selectionDetailsChanged**](classQfLocalFilesModel.md#signal-selectiondetailschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocalFilesModel**](#function-qflocalfilesmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addToFavorites**](#function-addtofavorites) (const QString & path) <br>_Adds the_ _path_ _to the favorites list._ |
|  Q\_INVOKABLE void | [**clearSelection**](#function-clearselection) () <br>_Set checked state of all items to false._  |
|  [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection-22) () const<br>_Returns the cloud connection used to fetch the cloud status of files within cloud projects._  |
|  int | [**currentDepth**](#function-currentdepth-22) () const<br>_Returns the current navigation history depth._  |
|  QString | [**currentPath**](#function-currentpath-22) () const<br>_Returns the current path._  |
|  QString | [**currentTitle**](#function-currenttitle-22) () const<br>_Returns the current title derived from the current path._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE [**QfLocalFileItem**](classQfLocalFileItem.md) | [**get**](#function-get) (int index) const<br>_Returns the_ [_**QfLocalFileItem**_](classQfLocalFileItem.md) _at the given__index_ _._ |
|  bool | [**inSelectionMode**](#function-inselectionmode-22) () <br>_Returns whether list is in multi-selection mode or not._  |
|  bool | [**isDeletedAllowedInCurrentPath**](#function-isdeletedallowedincurrentpath-22) () const<br> |
|  bool | [**isFetchingCloudStatus**](#function-isfetchingcloudstatus-22) () const<br>_Returns TRUE while the cloud project file list is being fetched._  |
|  Q\_INVOKABLE bool | [**isPathFavoriteEditable**](#function-ispathfavoriteeditable) (const QString & path) <br>_Returns whether_ _path's_ _favorite state is editable._ |
|  Q\_INVOKABLE void | [**moveUp**](#function-moveup) () <br>_Walks the navigation history back up on step._  |
|  Q\_INVOKABLE void | [**removeFromFavorites**](#function-removefromfavorites) (const QString & path) <br>_Removes the_ _path_ _to the favorites list._ |
|  Q\_INVOKABLE void | [**resetToPath**](#function-resettopath) (const QString & path) <br>_Resets the model and sets the first navigation history item to the provided_ _path_ _._ |
|  Q\_INVOKABLE void | [**resetToRoot**](#function-resettoroot) () <br>_Resets the model and sets the first navigation history item to root._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  int | [**selectedCount**](#function-selectedcount-22) () const<br>_Returns the number of checked items._  |
|  qint64 | [**selectedSize**](#function-selectedsize-22) () const<br>_Returns the summed size of checked items in bytes._  |
|  Q\_INVOKABLE void | [**setChecked**](#function-setchecked) (const int & mIdx, const bool & checked) <br>_Set checked state of an item in list._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QfCloudConnection**](classQfCloudConnection.md) \* cloudConnection) <br>_Sets the cloud connection used to fetch the cloud status of files within cloud projects._  |
|  void | [**setCurrentPath**](#function-setcurrentpath) (const QString & path) <br>_Appends the provided_ _path_ _to the navigation history._ |




























## Public Types Documentation




### enum CloudStatus 

_Synchronization status of a local file within a cloud project, compared against the files stored on QFieldCloud._ 
```C++
enum QfLocalFilesModel::CloudStatus {
    LocalOnly,
    Synchronized,
    Divergent
};
```




<hr>



### enum ItemMetaType 

```C++
enum QfLocalFilesModel::ItemMetaType {
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
enum QfLocalFilesModel::ItemType {
    ApplicationFolder,
    QfExternalStorage,
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
enum QfLocalFilesModel::Role {
    ItemMetaTypeRole = Qt::UserRole,
    ItemTypeRole,
    ItemTitleRole,
    ItemFormatRole,
    ItemPathRole,
    ItemSizeRole,
    ItemHasThumbnailRole,
    ItemIsFavoriteRole,
    ItemHasWebdavConfigurationRole,
    ItemCheckedRole,
    ItemCloudStatusRole
};
```




<hr>
## Public Properties Documentation




### property cloudConnection [1/2]

```C++
QfCloudConnection * QfLocalFilesModel::cloudConnection;
```




<hr>



### property currentDepth [1/2]

```C++
int QfLocalFilesModel::currentDepth;
```




<hr>



### property currentPath [1/2]

```C++
QString QfLocalFilesModel::currentPath;
```




<hr>



### property currentTitle [1/2]

```C++
QString QfLocalFilesModel::currentTitle;
```




<hr>



### property inSelectionMode [1/2]

```C++
bool QfLocalFilesModel::inSelectionMode;
```




<hr>



### property isDeletedAllowedInCurrentPath [1/2]

```C++
bool QfLocalFilesModel::isDeletedAllowedInCurrentPath;
```




<hr>



### property isFetchingCloudStatus [1/2]

```C++
bool QfLocalFilesModel::isFetchingCloudStatus;
```




<hr>



### property selectedCount [1/2]

```C++
int QfLocalFilesModel::selectedCount;
```




<hr>



### property selectedSize [1/2]

```C++
qint64 QfLocalFilesModel::selectedSize;
```




<hr>
## Public Signals Documentation




### signal cloudConnectionChanged 

```C++
void QfLocalFilesModel::cloudConnectionChanged;
```




<hr>



### signal currentPathChanged 

```C++
void QfLocalFilesModel::currentPathChanged;
```




<hr>



### signal inSelectionModeChanged 

```C++
void QfLocalFilesModel::inSelectionModeChanged;
```




<hr>



### signal isFetchingCloudStatusChanged 

```C++
void QfLocalFilesModel::isFetchingCloudStatusChanged;
```




<hr>



### signal selectionDetailsChanged 

```C++
void QfLocalFilesModel::selectionDetailsChanged;
```




<hr>
## Public Functions Documentation




### function QfLocalFilesModel 

```C++
explicit QfLocalFilesModel::QfLocalFilesModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addToFavorites 

_Adds the_ _path_ _to the favorites list._
```C++
Q_INVOKABLE void QfLocalFilesModel::addToFavorites (
    const QString & path
) 
```




<hr>



### function clearSelection 

_Set checked state of all items to false._ 
```C++
Q_INVOKABLE void QfLocalFilesModel::clearSelection () 
```




<hr>



### function cloudConnection [2/2]

_Returns the cloud connection used to fetch the cloud status of files within cloud projects._ 
```C++
QfCloudConnection * QfLocalFilesModel::cloudConnection () const
```




<hr>



### function currentDepth [2/2]

_Returns the current navigation history depth._ 
```C++
inline int QfLocalFilesModel::currentDepth () const
```




<hr>



### function currentPath [2/2]

_Returns the current path._ 
```C++
QString QfLocalFilesModel::currentPath () const
```




<hr>



### function currentTitle [2/2]

_Returns the current title derived from the current path._ 
```C++
QString QfLocalFilesModel::currentTitle () const
```




<hr>



### function data 

```C++
QVariant QfLocalFilesModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function get 

_Returns the_ [_**QfLocalFileItem**_](classQfLocalFileItem.md) _at the given__index_ _._
```C++
Q_INVOKABLE QfLocalFileItem QfLocalFilesModel::get (
    int index
) const
```




<hr>



### function inSelectionMode [2/2]

_Returns whether list is in multi-selection mode or not._ 
```C++
bool QfLocalFilesModel::inSelectionMode () 
```




<hr>



### function isDeletedAllowedInCurrentPath [2/2]

```C++
bool QfLocalFilesModel::isDeletedAllowedInCurrentPath () const
```



Returns TRUE if deletation of files and folders is allow in the current path 

**Note:**

Currently, deletion is only allowed when the path is a sub-child of an application directory 





        

<hr>



### function isFetchingCloudStatus [2/2]

_Returns TRUE while the cloud project file list is being fetched._ 
```C++
inline bool QfLocalFilesModel::isFetchingCloudStatus () const
```




<hr>



### function isPathFavoriteEditable 

_Returns whether_ _path's_ _favorite state is editable._
```C++
Q_INVOKABLE bool QfLocalFilesModel::isPathFavoriteEditable (
    const QString & path
) 
```




<hr>



### function moveUp 

_Walks the navigation history back up on step._ 
```C++
Q_INVOKABLE void QfLocalFilesModel::moveUp () 
```




<hr>



### function removeFromFavorites 

_Removes the_ _path_ _to the favorites list._
```C++
Q_INVOKABLE void QfLocalFilesModel::removeFromFavorites (
    const QString & path
) 
```




<hr>



### function resetToPath 

_Resets the model and sets the first navigation history item to the provided_ _path_ _._
```C++
Q_INVOKABLE void QfLocalFilesModel::resetToPath (
    const QString & path
) 
```




<hr>



### function resetToRoot 

_Resets the model and sets the first navigation history item to root._ 
```C++
Q_INVOKABLE void QfLocalFilesModel::resetToRoot () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfLocalFilesModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfLocalFilesModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function selectedCount [2/2]

_Returns the number of checked items._ 
```C++
int QfLocalFilesModel::selectedCount () const
```




<hr>



### function selectedSize [2/2]

_Returns the summed size of checked items in bytes._ 
```C++
qint64 QfLocalFilesModel::selectedSize () const
```




<hr>



### function setChecked 

_Set checked state of an item in list._ 
```C++
Q_INVOKABLE void QfLocalFilesModel::setChecked (
    const int & mIdx,
    const bool & checked
) 
```




<hr>



### function setCloudConnection 

_Sets the cloud connection used to fetch the cloud status of files within cloud projects._ 
```C++
void QfLocalFilesModel::setCloudConnection (
    QfCloudConnection * cloudConnection
) 
```




<hr>



### function setCurrentPath 

_Appends the provided_ _path_ _to the navigation history._
```C++
void QfLocalFilesModel::setCurrentPath (
    const QString & path
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qflocalfilesmodel.h`

