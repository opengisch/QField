

# Class QfBookmarkModel



[**ClassList**](annotated.md) **>** [**QfBookmarkModel**](classQfBookmarkModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hideProjectBookmarks**](classQfBookmarkModel.md#property-hideprojectbookmarks-12)  <br>_When TRUE, project bookmarks are hidden so only user bookmarks (those created in QField) are listed._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfBookmarkModel.md#property-mapsettings-12)  <br> |
| property qsizetype | [**selectedCount**](classQfBookmarkModel.md#property-selectedcount-12)  <br>_Number of currently selected bookmarks._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hideProjectBookmarksChanged**](classQfBookmarkModel.md#signal-hideprojectbookmarkschanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfBookmarkModel.md#signal-mapsettingschanged)  <br> |
| signal void | [**requestJumpToPoint**](classQfBookmarkModel.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**selectedCountChanged**](classQfBookmarkModel.md#signal-selectedcountchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBookmarkModel**](#function-qfbookmarkmodel) (QgsBookmarkManager \* manager, QgsBookmarkManager \* projectManager=nullptr, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QString | [**addBookmarkAtPoint**](#function-addbookmarkatpoint) (QgsPoint point, const QString & name=QString(), const QString & group=QString()) <br> |
|  Q\_INVOKABLE void | [**clearSelection**](#function-clearselection) () <br>_Clears the current selection._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE int | [**deleteSelected**](#function-deleteselected) () <br>_Deletes all currently selected bookmarks, persisting once. Returns the number deleted._  |
|  Q\_INVOKABLE bool | [**exportBookmarks**](#function-exportbookmarks) (bool selectedOnly) <br>_Exports user bookmarks to a temporary GeoPackage and sends it via the platform's native sharing. When_ _selectedOnly_ _is TRUE, only selected bookmarks are exported._ |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**getBookmarkCrs**](#function-getbookmarkcrs) (const QString & id) <br> |
|  Q\_INVOKABLE QgsPoint | [**getBookmarkPoint**](#function-getbookmarkpoint-12) (int idx) <br>_Returns the center point of the bookmark at row_ _idx_ _transformed to the map canvas CRS, or an empty point on failure._ |
|  Q\_INVOKABLE QgsPoint | [**getBookmarkPoint**](#function-getbookmarkpoint-22) (const QString & id) <br> |
|  bool | [**hideProjectBookmarks**](#function-hideprojectbookmarks-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  Q\_INVOKABLE void | [**removeBookmark**](#function-removebookmark) (const QString & id) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  qsizetype | [**selectedCount**](#function-selectedcount-22) () const<br> |
|  Q\_INVOKABLE void | [**setExtentFromBookmark**](#function-setextentfrombookmark) (const QModelIndex & index) <br> |
|  void | [**setHideProjectBookmarks**](#function-sethideprojectbookmarks) (bool hideProjectBookmarks) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  Q\_INVOKABLE void | [**store**](#function-store) () <br> |
|  Q\_INVOKABLE void | [**toggleSelected**](#function-toggleselected) (const QString & id) <br>_Toggles the selection state of the bookmark identified by_ _id_ _._ |
|  Q\_INVOKABLE void | [**updateBookmarkDetails**](#function-updatebookmarkdetails) (const QString & id, const QString & name, const QString & group) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |




## Public Types Documentation




### enum Roles 

```C++
enum QfBookmarkModel::Roles {
    BookmarkId = Qt::UserRole + 1,
    BookmarkName,
    BookmarkGroup,
    BookmarkPoint,
    BookmarkCrs,
    BookmarkUser,
    BookmarkSelected,
    BookmarkSection
};
```




<hr>
## Public Properties Documentation




### property hideProjectBookmarks [1/2]

_When TRUE, project bookmarks are hidden so only user bookmarks (those created in QField) are listed._ 
```C++
bool QfBookmarkModel::hideProjectBookmarks;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfBookmarkModel::mapSettings;
```




<hr>



### property selectedCount [1/2]

_Number of currently selected bookmarks._ 
```C++
qsizetype QfBookmarkModel::selectedCount;
```




<hr>
## Public Signals Documentation




### signal hideProjectBookmarksChanged 

```C++
void QfBookmarkModel::hideProjectBookmarksChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfBookmarkModel::mapSettingsChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void QfBookmarkModel::requestJumpToPoint;
```




<hr>



### signal selectedCountChanged 

```C++
void QfBookmarkModel::selectedCountChanged;
```




<hr>
## Public Functions Documentation




### function QfBookmarkModel 

```C++
explicit QfBookmarkModel::QfBookmarkModel (
    QgsBookmarkManager * manager,
    QgsBookmarkManager * projectManager=nullptr,
    QObject * parent=nullptr
) 
```




<hr>



### function addBookmarkAtPoint 

```C++
Q_INVOKABLE QString QfBookmarkModel::addBookmarkAtPoint (
    QgsPoint point,
    const QString & name=QString(),
    const QString & group=QString()
) 
```




<hr>



### function clearSelection 

_Clears the current selection._ 
```C++
Q_INVOKABLE void QfBookmarkModel::clearSelection () 
```




<hr>



### function data 

```C++
QVariant QfBookmarkModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deleteSelected 

_Deletes all currently selected bookmarks, persisting once. Returns the number deleted._ 
```C++
Q_INVOKABLE int QfBookmarkModel::deleteSelected () 
```




<hr>



### function exportBookmarks 

_Exports user bookmarks to a temporary GeoPackage and sends it via the platform's native sharing. When_ _selectedOnly_ _is TRUE, only selected bookmarks are exported._
```C++
Q_INVOKABLE bool QfBookmarkModel::exportBookmarks (
    bool selectedOnly
) 
```




<hr>



### function getBookmarkCrs 

```C++
Q_INVOKABLE QgsCoordinateReferenceSystem QfBookmarkModel::getBookmarkCrs (
    const QString & id
) 
```




<hr>



### function getBookmarkPoint [1/2]

_Returns the center point of the bookmark at row_ _idx_ _transformed to the map canvas CRS, or an empty point on failure._
```C++
Q_INVOKABLE QgsPoint QfBookmarkModel::getBookmarkPoint (
    int idx
) 
```




<hr>



### function getBookmarkPoint [2/2]

```C++
Q_INVOKABLE QgsPoint QfBookmarkModel::getBookmarkPoint (
    const QString & id
) 
```




<hr>



### function hideProjectBookmarks [2/2]

```C++
inline bool QfBookmarkModel::hideProjectBookmarks () const
```




<hr>



### function mapSettings [2/2]

```C++
inline QgsQuickMapSettings * QfBookmarkModel::mapSettings () const
```




<hr>



### function removeBookmark 

```C++
Q_INVOKABLE void QfBookmarkModel::removeBookmark (
    const QString & id
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfBookmarkModel::roleNames () override const
```




<hr>



### function selectedCount [2/2]

```C++
inline qsizetype QfBookmarkModel::selectedCount () const
```




<hr>



### function setExtentFromBookmark 

```C++
Q_INVOKABLE void QfBookmarkModel::setExtentFromBookmark (
    const QModelIndex & index
) 
```




<hr>



### function setHideProjectBookmarks 

```C++
void QfBookmarkModel::setHideProjectBookmarks (
    bool hideProjectBookmarks
) 
```




<hr>



### function setMapSettings 

```C++
void QfBookmarkModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function store 

```C++
Q_INVOKABLE void QfBookmarkModel::store () 
```




<hr>



### function toggleSelected 

_Toggles the selection state of the bookmark identified by_ _id_ _._
```C++
Q_INVOKABLE void QfBookmarkModel::toggleSelected (
    const QString & id
) 
```




<hr>



### function updateBookmarkDetails 

```C++
Q_INVOKABLE void QfBookmarkModel::updateBookmarkDetails (
    const QString & id,
    const QString & name,
    const QString & group
) 
```




<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool QfBookmarkModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function lessThan 

```C++
bool QfBookmarkModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfbookmarkmodel.h`

