

# Class DeltaListModel



[**ClassList**](annotated.md) **>** [**DeltaListModel**](classDeltaListModel.md)








Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Delta**](structDeltaListModel_1_1Delta.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |
| enum  | [**Status**](#enum-status)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**rowCount**](classDeltaListModel.md#property-rowcount-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**rowCountChanged**](classDeltaListModel.md#signal-rowcountchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DeltaListModel**](#function-deltalistmodel-12) () = default<br> |
|   | [**DeltaListModel**](#function-deltalistmodel-22) (QJsonDocument deltasStatusList) <br> |
|  bool | [**allHaveFinalStatus**](#function-allhavefinalstatus) () const<br>_Whether all the deltas are in final status._  |
|  QString | [**combinedOutput**](#function-combinedoutput) () const<br>_Returns a combined output for all deltas, separated by a new line._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  QString | [**errorString**](#function-errorstring) () const<br>_Holds the reason why it is invalid. Null string if not invalid._  |
|  bool | [**isValid**](#function-isvalid) () const<br>_Whether the model is valid and can be used._  |
|  QJsonDocument | [**json**](#function-json) () const<br>_Returns the json document used to initialize the model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount-22) (const QModelIndex & parent=QModelIndex()) override const<br>_Returns number of rows._  |




























## Public Types Documentation




### enum ColumnRole 

```C++
enum DeltaListModel::ColumnRole {
    IdRole,
    DeltafileIdRole,
    CreatedAtRole,
    UpdatedAtRole,
    StatusRole,
    OutputRole
};
```




<hr>



### enum Status 

```C++
enum DeltaListModel::Status {
    PendingStatus,
    BusyStatus,
    AppliedStatus,
    ConflictStatus,
    NotAppliedStatus,
    ErrorStatus,
    IgnoredStatus,
    UnpermittedStatus
};
```




<hr>
## Public Properties Documentation




### property rowCount [1/2]

```C++
int DeltaListModel::rowCount;
```




<hr>
## Public Signals Documentation




### signal rowCountChanged 

```C++
void DeltaListModel::rowCountChanged;
```




<hr>
## Public Functions Documentation




### function DeltaListModel [1/2]

```C++
DeltaListModel::DeltaListModel () = default
```




<hr>



### function DeltaListModel [2/2]

```C++
explicit DeltaListModel::DeltaListModel (
    QJsonDocument deltasStatusList
) 
```




<hr>



### function allHaveFinalStatus 

_Whether all the deltas are in final status._ 
```C++
bool DeltaListModel::allHaveFinalStatus () const
```




<hr>



### function combinedOutput 

_Returns a combined output for all deltas, separated by a new line._ 
```C++
QString DeltaListModel::combinedOutput () const
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



### function errorString 

_Holds the reason why it is invalid. Null string if not invalid._ 
```C++
QString DeltaListModel::errorString () const
```




<hr>



### function isValid 

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



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > DeltaListModel::roleNames () override const
```




<hr>



### function rowCount [2/2]

_Returns number of rows._ 
```C++
int DeltaListModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/deltalistmodel.h`

