

# Class QfCloudAttachmentsModel



[**ClassList**](annotated.md) **>** [**QfCloudAttachmentsModel**](classQfCloudAttachmentsModel.md)



[More...](#detailed-description)

* `#include <qfcloudattachmentsmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |
| enum  | [**Status**](#enum-status)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](classQfCloudAttachmentsModel.md#property-cloudconnection-12)  <br> |
| property [**QfCloudProject**](classQfCloudProject.md) \* | [**cloudProject**](classQfCloudAttachmentsModel.md#property-cloudproject-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**cloudConnectionChanged**](classQfCloudAttachmentsModel.md#signal-cloudconnectionchanged)  <br> |
| signal void | [**cloudProjectChanged**](classQfCloudAttachmentsModel.md#signal-cloudprojectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudAttachmentsModel**](#function-qfcloudattachmentsmodel) (QObject \* parent=nullptr) <br> |
|  [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection-22) () const<br>_Returns the cloud connection used to read the pending attachments and the attachments upload history._  |
|  [**QfCloudProject**](classQfCloudProject.md) \* | [**cloudProject**](#function-cloudproject-22) () const<br>_Returns the cloud project whose attachments are listed._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br>_Reloads the attachments not yet uploaded, the pending attachments and the attachments upload history._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br>_Returns number of rows._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QfCloudConnection**](classQfCloudConnection.md) \* cloudConnection) <br>_Sets the_ _cloudConnection_ _used to read the pending attachments and the attachments upload history._ |
|  void | [**setCloudProject**](#function-setcloudproject) ([**QfCloudProject**](classQfCloudProject.md) \* cloudProject) <br>_Sets the_ _cloudProject_ _whose attachments are listed._ |




























## Detailed Description


A model listing the attachments of a QFieldCloud project that are not yet uploaded or pending upload, alongside the outcome of past attachment uploads. 


    
## Public Types Documentation




### enum ColumnRole 

```C++
enum QfCloudAttachmentsModel::ColumnRole {
    FilePathRole = Qt::UserRole + 1,
    FileNameRole,
    StatusRole,
    HttpCodeRole,
    TimestampRole
};
```




<hr>



### enum Status 

```C++
enum QfCloudAttachmentsModel::Status {
    LocalStatus,
    PendingStatus,
    UploadedStatus,
    FailedStatus
};
```




<hr>
## Public Properties Documentation




### property cloudConnection [1/2]

```C++
QfCloudConnection * QfCloudAttachmentsModel::cloudConnection;
```




<hr>



### property cloudProject [1/2]

```C++
QfCloudProject * QfCloudAttachmentsModel::cloudProject;
```




<hr>
## Public Signals Documentation




### signal cloudConnectionChanged 

```C++
void QfCloudAttachmentsModel::cloudConnectionChanged;
```




<hr>



### signal cloudProjectChanged 

```C++
void QfCloudAttachmentsModel::cloudProjectChanged;
```




<hr>
## Public Functions Documentation




### function QfCloudAttachmentsModel 

```C++
explicit QfCloudAttachmentsModel::QfCloudAttachmentsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function cloudConnection [2/2]

_Returns the cloud connection used to read the pending attachments and the attachments upload history._ 
```C++
QfCloudConnection * QfCloudAttachmentsModel::cloudConnection () const
```




<hr>



### function cloudProject [2/2]

_Returns the cloud project whose attachments are listed._ 
```C++
QfCloudProject * QfCloudAttachmentsModel::cloudProject () const
```




<hr>



### function data 

_Returns the data at given_ _index_ _with given__role_ _._
```C++
QVariant QfCloudAttachmentsModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function refresh 

_Reloads the attachments not yet uploaded, the pending attachments and the attachments upload history._ 
```C++
Q_INVOKABLE void QfCloudAttachmentsModel::refresh () 
```




<hr>



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > QfCloudAttachmentsModel::roleNames () override const
```




<hr>



### function rowCount 

_Returns number of rows._ 
```C++
int QfCloudAttachmentsModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setCloudConnection 

_Sets the_ _cloudConnection_ _used to read the pending attachments and the attachments upload history._
```C++
void QfCloudAttachmentsModel::setCloudConnection (
    QfCloudConnection * cloudConnection
) 
```




<hr>



### function setCloudProject 

_Sets the_ _cloudProject_ _whose attachments are listed._
```C++
void QfCloudAttachmentsModel::setCloudProject (
    QfCloudProject * cloudProject
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudattachmentsmodel.h`

