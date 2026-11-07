

# Class QfDeltaListModel



[**ClassList**](annotated.md) **>** [**QfDeltaListModel**](classQfDeltaListModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](classQfDeltaListModel.md#property-cloudconnection-12)  <br> |
| property QString | [**cloudProjectId**](classQfDeltaListModel.md#property-cloudprojectid-12)  <br> |
| property QString | [**errorString**](classQfDeltaListModel.md#property-errorstring-12)  <br> |
| property bool | [**hasNextPage**](classQfDeltaListModel.md#property-hasnextpage-12)  <br> |
| property bool | [**isRefreshing**](classQfDeltaListModel.md#property-isrefreshing-12)  <br> |
| property bool | [**isValid**](classQfDeltaListModel.md#property-isvalid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudConnectionChanged**](classQfDeltaListModel.md#signal-cloudconnectionchanged)  <br> |
| signal void | [**cloudProjectIdChanged**](classQfDeltaListModel.md#signal-cloudprojectidchanged)  <br> |
| signal void | [**errorStringChanged**](classQfDeltaListModel.md#signal-errorstringchanged)  <br> |
| signal void | [**hasNextPageChanged**](classQfDeltaListModel.md#signal-hasnextpagechanged)  <br> |
| signal void | [**isRefreshingChanged**](classQfDeltaListModel.md#signal-isrefreshingchanged)  <br> |
| signal void | [**isValidChanged**](classQfDeltaListModel.md#signal-isvalidchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDeltaListModel**](#function-qfdeltalistmodel) () <br> |
|  [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection-22) () const<br> |
|  QString | [**cloudProjectId**](#function-cloudprojectid-22) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  QString | [**errorString**](#function-errorstring-22) () const<br>_Holds the reason why it is invalid. Null string if not invalid._  |
|  Q\_INVOKABLE void | [**fetchNextPage**](#function-fetchnextpage) () <br>_! Retrieves the next available delta list page_  |
|  bool | [**hasNextPage**](#function-hasnextpage-22) () const<br>_Whether the delta list has a next page available to retrieve._  |
|  bool | [**isRefreshing**](#function-isrefreshing-22) () const<br>_Whether the model is refreshing._  |
|  bool | [**isValid**](#function-isvalid-22) () const<br>_Whether the model is valid and can be used._  |
|  QJsonDocument | [**lastJson**](#function-lastjson) () const<br>_Returns the last json document used to populate the model._  |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br>_Refreshes the delta list model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br>_Returns number of rows._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QfCloudConnection**](classQfCloudConnection.md) \* cloudConnection) <br> |
|  void | [**setCloudProjectId**](#function-setcloudprojectid) (const QString & cloudProjectId) <br> |




























## Public Types Documentation




### enum ColumnRole 

```C++
enum QfDeltaListModel::ColumnRole {
    IdRole,
    DeltafileIdRole,
    CreatedByRole,
    CreatedAtRole,
    UpdatedAtRole,
    StatusRole,
    SummaryRole,
    OutputRole
};
```




<hr>
## Public Properties Documentation




### property cloudConnection [1/2]

```C++
QfCloudConnection * QfDeltaListModel::cloudConnection;
```




<hr>



### property cloudProjectId [1/2]

```C++
QString QfDeltaListModel::cloudProjectId;
```




<hr>



### property errorString [1/2]

```C++
QString QfDeltaListModel::errorString;
```




<hr>



### property hasNextPage [1/2]

```C++
bool QfDeltaListModel::hasNextPage;
```




<hr>



### property isRefreshing [1/2]

```C++
bool QfDeltaListModel::isRefreshing;
```




<hr>



### property isValid [1/2]

```C++
bool QfDeltaListModel::isValid;
```




<hr>
## Public Signals Documentation




### signal cloudConnectionChanged 

```C++
void QfDeltaListModel::cloudConnectionChanged;
```




<hr>



### signal cloudProjectIdChanged 

```C++
void QfDeltaListModel::cloudProjectIdChanged;
```




<hr>



### signal errorStringChanged 

```C++
void QfDeltaListModel::errorStringChanged;
```




<hr>



### signal hasNextPageChanged 

```C++
void QfDeltaListModel::hasNextPageChanged;
```




<hr>



### signal isRefreshingChanged 

```C++
void QfDeltaListModel::isRefreshingChanged;
```




<hr>



### signal isValidChanged 

```C++
void QfDeltaListModel::isValidChanged;
```




<hr>
## Public Functions Documentation




### function QfDeltaListModel 

```C++
explicit QfDeltaListModel::QfDeltaListModel () 
```




<hr>



### function cloudConnection [2/2]

```C++
inline QfCloudConnection * QfDeltaListModel::cloudConnection () const
```




<hr>



### function cloudProjectId [2/2]

```C++
inline QString QfDeltaListModel::cloudProjectId () const
```




<hr>



### function data 

_Returns the data at given_ _index_ _with given__role_ _._
```C++
QVariant QfDeltaListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function errorString [2/2]

_Holds the reason why it is invalid. Null string if not invalid._ 
```C++
QString QfDeltaListModel::errorString () const
```




<hr>



### function fetchNextPage 

_! Retrieves the next available delta list page_ 
```C++
Q_INVOKABLE void QfDeltaListModel::fetchNextPage () 
```




<hr>



### function hasNextPage [2/2]

_Whether the delta list has a next page available to retrieve._ 
```C++
bool QfDeltaListModel::hasNextPage () const
```




<hr>



### function isRefreshing [2/2]

_Whether the model is refreshing._ 
```C++
bool QfDeltaListModel::isRefreshing () const
```




<hr>



### function isValid [2/2]

_Whether the model is valid and can be used._ 
```C++
bool QfDeltaListModel::isValid () const
```




<hr>



### function lastJson 

_Returns the last json document used to populate the model._ 
```C++
QJsonDocument QfDeltaListModel::lastJson () const
```




<hr>



### function refresh 

_Refreshes the delta list model._ 
```C++
Q_INVOKABLE void QfDeltaListModel::refresh () 
```




<hr>



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > QfDeltaListModel::roleNames () override const
```




<hr>



### function rowCount 

_Returns number of rows._ 
```C++
int QfDeltaListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setCloudConnection 

```C++
void QfDeltaListModel::setCloudConnection (
    QfCloudConnection * cloudConnection
) 
```




<hr>



### function setCloudProjectId 

```C++
void QfDeltaListModel::setCloudProjectId (
    const QString & cloudProjectId
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfdeltalistmodel.h`

