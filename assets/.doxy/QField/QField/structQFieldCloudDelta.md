

# Struct QFieldCloudDelta



[**ClassList**](annotated.md) **>** [**QFieldCloudDelta**](structQFieldCloudDelta.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Status**](#enum-status)  <br> |




## Public Attributes

| Type | Name |
| ---: | :--- |
|  QDateTime | [**createdAt**](#variable-createdat)  <br> |
|  QString | [**createdBy**](#variable-createdby)  <br> |
|  QUuid | [**deltafileId**](#variable-deltafileid)  <br> |
|  QUuid | [**id**](#variable-id)  <br> |
|  QString | [**output**](#variable-output)  <br> |
|  Status | [**status**](#variable-status)   = `PendingStatus`<br> |
|  QString | [**summary**](#variable-summary)  <br> |
|  QDateTime | [**updatedAt**](#variable-updatedat)  <br> |












































## Public Types Documentation




### enum Status 

```C++
enum QFieldCloudDelta::Status {
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
## Public Attributes Documentation




### variable createdAt 

```C++
QDateTime QFieldCloudDelta::createdAt;
```




<hr>



### variable createdBy 

```C++
QString QFieldCloudDelta::createdBy;
```




<hr>



### variable deltafileId 

```C++
QUuid QFieldCloudDelta::deltafileId;
```




<hr>



### variable id 

```C++
QUuid QFieldCloudDelta::id;
```




<hr>



### variable output 

```C++
QString QFieldCloudDelta::output;
```




<hr>



### variable status 

```C++
Status QFieldCloudDelta::status;
```




<hr>



### variable summary 

```C++
QString QFieldCloudDelta::summary;
```




<hr>



### variable updatedAt 

```C++
QDateTime QFieldCloudDelta::updatedAt;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

