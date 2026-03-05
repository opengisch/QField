

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
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classBookmarkModel.md#property-mapsettings-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapSettingsChanged**](classBookmarkModel.md#signal-mapsettingschanged)  <br> |
| signal void | [**requestJumpToPoint**](classBookmarkModel.md#signal-requestjumptopoint) (const QgsPoint & center, const double & scale=-1.0, bool handleMargins=false) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BookmarkModel**](#function-bookmarkmodel) (QgsBookmarkManager \* manager, QgsBookmarkManager \* projectManager=nullptr, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QString | [**addBookmarkAtPoint**](#function-addbookmarkatpoint) (QgsPoint point, const QString & name=QString(), const QString & group=QString()) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE QgsCoordinateReferenceSystem | [**getBookmarkCrs**](#function-getbookmarkcrs) (const QString & id) <br> |
|  Q\_INVOKABLE QgsPoint | [**getBookmarkPoint**](#function-getbookmarkpoint) (const QString & id) <br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  Q\_INVOKABLE void | [**removeBookmark**](#function-removebookmark) (const QString & id) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  Q\_INVOKABLE void | [**setExtentFromBookmark**](#function-setextentfrombookmark) (const QModelIndex & index) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  Q\_INVOKABLE void | [**store**](#function-store) () <br> |
|  Q\_INVOKABLE void | [**updateBookmarkDetails**](#function-updatebookmarkdetails) (const QString & id, const QString & name, const QString & group) <br> |




























## Public Types Documentation




### enum Roles 

```C++
enum BookmarkModel::Roles {
    BookmarkId = Qt::UserRole + 1,
    BookmarkName,
    BookmarkGroup,
    BookmarkPoint,
    BookmarkCrs,
    BookmarkUser
};
```




<hr>
## Public Properties Documentation




### property mapSettings [1/2]

```C++
QgsQuickMapSettings * BookmarkModel::mapSettings;
```




<hr>
## Public Signals Documentation




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



### function data 

```C++
QVariant BookmarkModel::data (
    const QModelIndex & index,
    int role
) override const
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



### function setExtentFromBookmark 

```C++
Q_INVOKABLE void BookmarkModel::setExtentFromBookmark (
    const QModelIndex & index
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



### function updateBookmarkDetails 

```C++
Q_INVOKABLE void BookmarkModel::updateBookmarkDetails (
    const QString & id,
    const QString & name,
    const QString & group
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/bookmarkmodel.h`

