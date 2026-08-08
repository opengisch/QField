

# Class DeltaListModel



[**ClassList**](annotated.md) **>** [**DeltaListModel**](classDeltaListModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* | [**cloudConnection**](classDeltaListModel.md#property-cloudconnection-12)  <br> |
| property QString | [**cloudProjectId**](classDeltaListModel.md#property-cloudprojectid-12)  <br> |
| property QString | [**errorString**](classDeltaListModel.md#property-errorstring-12)  <br> |
| property bool | [**isRefreshing**](classDeltaListModel.md#property-isrefreshing-12)  <br> |
| property bool | [**isValid**](classDeltaListModel.md#property-isvalid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudConnectionChanged**](classDeltaListModel.md#signal-cloudconnectionchanged)  <br> |
| signal void | [**cloudProjectIdChanged**](classDeltaListModel.md#signal-cloudprojectidchanged)  <br> |
| signal void | [**errorStringChanged**](classDeltaListModel.md#signal-errorstringchanged)  <br> |
| signal void | [**isRefreshingChanged**](classDeltaListModel.md#signal-isrefreshingchanged)  <br> |
| signal void | [**isValidChanged**](classDeltaListModel.md#signal-isvalidchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DeltaListModel**](#function-deltalistmodel) () <br> |
|  [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection-22) () const<br> |
|  QString | [**cloudProjectId**](#function-cloudprojectid-22) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  QString | [**errorString**](#function-errorstring-22) () const<br>_Holds the reason why it is invalid. Null string if not invalid._  |
|  bool | [**isRefreshing**](#function-isrefreshing-22) () const<br>_Whether the model is refreshing._  |
|  bool | [**isValid**](#function-isvalid-22) () const<br>_Whether the model is valid and can be used._  |
|  QJsonDocument | [**json**](#function-json) () const<br>_Returns the json document used to initialize the model._  |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br>_Refreshes the delta list model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br>_Returns number of rows._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QFieldCloudConnection**](classQFieldCloudConnection.md) \* cloudConnection) <br> |
|  void | [**setCloudProjectId**](#function-setcloudprojectid) (const QString & cloudProjectId) <br> |




























## Public Types Documentation




### enum ColumnRole 

```C++
enum DeltaListModel::ColumnRole {
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
QFieldCloudConnection * DeltaListModel::cloudConnection;
```




<hr>



### property cloudProjectId [1/2]

```C++
QString DeltaListModel::cloudProjectId;
```




<hr>



### property errorString [1/2]

```C++
QString DeltaListModel::errorString;
```




<hr>



### property isRefreshing [1/2]

```C++
bool DeltaListModel::isRefreshing;
```




<hr>



### property isValid [1/2]

```C++
bool DeltaListModel::isValid;
```




<hr>
## Public Signals Documentation




### signal cloudConnectionChanged 

```C++
void DeltaListModel::cloudConnectionChanged;
```




<hr>



### signal cloudProjectIdChanged 

```C++
void DeltaListModel::cloudProjectIdChanged;
```




<hr>



### signal errorStringChanged 

```C++
void DeltaListModel::errorStringChanged;
```




<hr>



### signal isRefreshingChanged 

```C++
void DeltaListModel::isRefreshingChanged;
```




<hr>



### signal isValidChanged 

```C++
void DeltaListModel::isValidChanged;
```




<hr>
## Public Functions Documentation




### function DeltaListModel 

```C++
explicit DeltaListModel::DeltaListModel () 
```




<hr>



### function cloudConnection [2/2]

```C++
inline QFieldCloudConnection * DeltaListModel::cloudConnection () const
```




<hr>



### function cloudProjectId [2/2]

```C++
inline QString DeltaListModel::cloudProjectId () const
```




<hr>



### function data 

_Returns the data at given_ _index_ _with given__role_ _._
```C++
QVariant DeltaListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function errorString [2/2]

_Holds the reason why it is invalid. Null string if not invalid._ 
```C++
QString DeltaListModel::errorString () const
```




<hr>



### function isRefreshing [2/2]

_Whether the model is refreshing._ 
```C++
bool DeltaListModel::isRefreshing () const
```




<hr>



### function isValid [2/2]

_Whether the model is valid and can be used._ 
```C++
bool DeltaListModel::isValid () const
```




<hr>



### function json 

_Returns the json document used to initialize the model._ 
```C++
QJsonDocument DeltaListModel::json () const
```




<hr>



### function refresh 

_Refreshes the delta list model._ 
```C++
Q_INVOKABLE void DeltaListModel::refresh () 
```




<hr>



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > DeltaListModel::roleNames () override const
```




<hr>



### function rowCount 

_Returns number of rows._ 
```C++
int DeltaListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setCloudConnection 

```C++
void DeltaListModel::setCloudConnection (
    QFieldCloudConnection * cloudConnection
) 
```




<hr>



### function setCloudProjectId 

```C++
void DeltaListModel::setCloudProjectId (
    const QString & cloudProjectId
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/deltalistmodel.h`

