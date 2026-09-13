

# Struct QfCloudDelta



[**ClassList**](annotated.md) **>** [**QfCloudDelta**](structQfCloudDelta.md)






















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
enum QfCloudDelta::Status {
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
QDateTime QfCloudDelta::createdAt;
```




<hr>



### variable createdBy 

```C++
QString QfCloudDelta::createdBy;
```




<hr>



### variable deltafileId 

```C++
QUuid QfCloudDelta::deltafileId;
```




<hr>



### variable id 

```C++
QUuid QfCloudDelta::id;
```




<hr>



### variable output 

```C++
QString QfCloudDelta::output;
```




<hr>



### variable status 

```C++
Status QfCloudDelta::status;
```




<hr>



### variable summary 

```C++
QString QfCloudDelta::summary;
```




<hr>



### variable updatedAt 

```C++
QDateTime QfCloudDelta::updatedAt;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfcloudutils.h`

