

# Class BookmarkModel



[**ClassList**](annotated.md) **>** [**BookmarkModel**](classBookmarkModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hideProjectBookmarks**](classBookmarkModel.md#property-hideprojectbookmarks-12)  <br>_When TRUE, project bookmarks are hidden so only user bookmarks (those created in QField) are listed._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classBookmarkModel.md#property-mapsettings-12)  <br> |
| property qsizetype | [**selectedCount**](classBookmarkModel.md#property-selectedcount-12)  <br>_Number of currently selected bookmarks._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hideProjectBookmarksChanged**](classBookmarkModel.md#signal-hideprojectbookmarkschanged)  <br> |
| signal void | [**mapSettingsChanged**](classBookmarkModel.md#signal-mapsettingschanged)  <br> |
| signal void | [**requestJumpToPoint**](classBookmarkModel.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |
| signal void | [**selectedCountChanged**](classBookmarkModel.md#signal-selectedcountchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BookmarkModel**](#function-bookmarkmodel) (QgsBookmarkManager \* manager, QgsBookmarkManager \* projectManager=nullptr, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QString | [**addBookmarkAtPoint**](#function-addbookmarkatpoint) (QgsPoint point, const QString & name=QString(), const QString & group=QString()) <br> |
|  Q\_INVOKABLE void | [**clearSelection**](#function-clearselection) () <br>_Clears the current selection._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE int | [**deleteSelected**](#function-deleteselected) () <br>_Deletes all currently selected bookmarks, persisting once. Returns the number deleted._  |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**getBookmarkCrs**](#function-getbookmarkcrs) (const QString & id) <br> |
|  Q\_INVOKABLE QgsPoint | [**getBookmarkPoint**](#function-getbookmarkpoint) (const QString & id) <br> |
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
enum BookmarkModel::Roles {
    BookmarkId = Qt::UserRole + 1,
    BookmarkName,
    BookmarkGroup,
    BookmarkPoint,
    BookmarkCrs,
    BookmarkUser,
    BookmarkSelected
};
```




<hr>
## Public Properties Documentation




### property hideProjectBookmarks [1/2]

_When TRUE, project bookmarks are hidden so only user bookmarks (those created in QField) are listed._ 
```C++
bool BookmarkModel::hideProjectBookmarks;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * BookmarkModel::mapSettings;
```




<hr>



### property selectedCount [1/2]

_Number of currently selected bookmarks._ 
```C++
qsizetype BookmarkModel::selectedCount;
```




<hr>
## Public Signals Documentation




### signal hideProjectBookmarksChanged 

```C++
void BookmarkModel::hideProjectBookmarksChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void BookmarkModel::mapSettingsChanged;
```




<hr>



### signal requestJumpToPoint 

```C++
void BookmarkModel::requestJumpToPoint;
```




<hr>



### signal selectedCountChanged 

```C++
void BookmarkModel::selectedCountChanged;
```




<hr>
## Public Functions Documentation




### function BookmarkModel 

```C++
explicit BookmarkModel::BookmarkModel (
    QgsBookmarkManager * manager,
    QgsBookmarkManager * projectManager=nullptr,
    QObject * parent=nullptr
) 
```




<hr>



### function addBookmarkAtPoint 

```C++
Q_INVOKABLE QString BookmarkModel::addBookmarkAtPoint (
    QgsPoint point,
    const QString & name=QString(),
    const QString & group=QString()
) 
```




<hr>



### function clearSelection 

_Clears the current selection._ 
```C++
Q_INVOKABLE void BookmarkModel::clearSelection () 
```




<hr>



### function data 

```C++
QVariant BookmarkModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deleteSelected 

_Deletes all currently selected bookmarks, persisting once. Returns the number deleted._ 
```C++
Q_INVOKABLE int BookmarkModel::deleteSelected () 
```




<hr>



### function getBookmarkCrs 

```C++
Q_INVOKABLE QgsCoordinateReferenceSystem BookmarkModel::getBookmarkCrs (
    const QString & id
) 
```




<hr>



### function getBookmarkPoint 

```C++
Q_INVOKABLE QgsPoint BookmarkModel::getBookmarkPoint (
    const QString & id
) 
```




<hr>



### function hideProjectBookmarks [2/2]

```C++
inline bool BookmarkModel::hideProjectBookmarks () const
```




<hr>



### function mapSettings [2/2]

```C++
inline QgsQuickMapSettings * BookmarkModel::mapSettings () const
```




<hr>



### function removeBookmark 

```C++
Q_INVOKABLE void BookmarkModel::removeBookmark (
    const QString & id
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > BookmarkModel::roleNames () override const
```




<hr>



### function selectedCount [2/2]

```C++
inline qsizetype BookmarkModel::selectedCount () const
```




<hr>



### function setExtentFromBookmark 

```C++
Q_INVOKABLE void BookmarkModel::setExtentFromBookmark (
    const QModelIndex & index
) 
```




<hr>



### function setHideProjectBookmarks 

```C++
void BookmarkModel::setHideProjectBookmarks (
    bool hideProjectBookmarks
) 
```




<hr>



### function setMapSettings 

```C++
void BookmarkModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function store 

```C++
Q_INVOKABLE void BookmarkModel::store () 
```




<hr>



### function toggleSelected 

_Toggles the selection state of the bookmark identified by_ _id_ _._
```C++
Q_INVOKABLE void BookmarkModel::toggleSelected (
    const QString & id
) 
```




<hr>



### function updateBookmarkDetails 

```C++
Q_INVOKABLE void BookmarkModel::updateBookmarkDetails (
    const QString & id,
    const QString & name,
    const QString & group
) 
```




<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool BookmarkModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function lessThan 

```C++
bool BookmarkModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/bookmarkmodel.h`

