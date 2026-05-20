

# Class DeltaFileWrapper



[**ClassList**](annotated.md) **>** [**DeltaFileWrapper**](classDeltaFileWrapper.md)



[More...](#detailed-description)

* `#include <deltafilewrapper.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ErrorType**](#enum-errortype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**count**](classDeltaFileWrapper.md#property-count-12)  <br> |
| property QString | [**errorString**](classDeltaFileWrapper.md#property-errorstring-12)  <br> |
| property [**ErrorType**](classDeltaFileWrapper.md#enum-errortype) | [**errorType**](classDeltaFileWrapper.md#property-errortype-12)  <br> |
| property bool | [**hasError**](classDeltaFileWrapper.md#property-haserror-12)  <br> |
| property bool | [**isPushing**](classDeltaFileWrapper.md#property-ispushing-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classDeltaFileWrapper.md#signal-countchanged)  <br> |
| signal void | [**errorChanged**](classDeltaFileWrapper.md#signal-errorchanged)  <br> |
| signal void | [**isPushingChanged**](classDeltaFileWrapper.md#signal-ispushingchanged)  <br> |
| signal void | [**savedToFile**](classDeltaFileWrapper.md#signal-savedtofile)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DeltaFileWrapper**](#function-deltafilewrapper) (const QString & projectId, const QString & fileName) <br> |
|  void | [**addCreate**](#function-addcreate) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & newFeature) <br> |
|  void | [**addDelete**](#function-adddelete) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & oldFeature) <br> |
|  void | [**addPatch**](#function-addpatch) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & oldFeature, const QgsFeature & newFeature, bool storeSnapshot=true) <br> |
|  bool | [**append**](#function-append) (const [**DeltaFileWrapper**](classDeltaFileWrapper.md) \* deltaFileWrapper) <br> |
|  Q\_INVOKABLE bool | [**apply**](#function-apply) (const QgsProject \* project) <br> |
|  Q\_INVOKABLE bool | [**applyReversed**](#function-applyreversed) (const QgsProject \* project) <br> |
|  QMap&lt; QString, QString &gt; | [**attachmentFileNames**](#function-attachmentfilenames) () const<br> |
|  Q\_INVOKABLE int | [**count**](#function-count-22) () const<br> |
|  QStringList | [**deltaLayerIds**](#function-deltalayerids) () const<br> |
|  QJsonArray | [**deltas**](#function-deltas) () const<br> |
|  QString | [**errorString**](#function-errorstring-22) () const<br> |
|  [**ErrorType**](classDeltaFileWrapper.md#enum-errortype) | [**errorType**](#function-errortype-22) () const<br> |
|  QString | [**fileName**](#function-filename) () const<br> |
|  int | [**getDeltaIndexByUuid**](#function-getdeltaindexbyuuid) (const QString & uuid) const<br> |
|  bool | [**hasError**](#function-haserror-22) () const<br> |
|  QString | [**id**](#function-id) () const<br> |
|  Q\_INVOKABLE bool | [**isCreatedFeature**](#function-iscreatedfeature) (QgsVectorLayer \* vl, QgsFeature feature) <br> |
|  bool | [**isDeltaBeingApplied**](#function-isdeltabeingapplied) () const<br> |
|  bool | [**isDirty**](#function-isdirty) () const<br> |
|  bool | [**isPushing**](#function-ispushing-22) () const<br> |
|  QString | [**projectId**](#function-projectid) () const<br> |
|  Q\_INVOKABLE void | [**reset**](#function-reset) () <br> |
|  Q\_INVOKABLE void | [**resetId**](#function-resetid) () <br> |
|  void | [**setIsPushing**](#function-setispushing) (bool isPushing) <br> |
|  Q\_INVOKABLE bool | [**toFile**](#function-tofile) () <br> |
|  Q\_INVOKABLE QString | [**toFileForPush**](#function-tofileforpush) (const QString & outFileName=QString()) const<br> |
|  QByteArray | [**toJson**](#function-tojson) (QJsonDocument::JsonFormat jsonFormat=QJsonDocument::Indented) const<br> |
|  QString | [**toString**](#function-tostring) () const<br> |
|   | [**~DeltaFileWrapper**](#function-deltafilewrapper) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QStringList | [**attachmentFieldNames**](#function-attachmentfieldnames) (const QgsProject \* project, const QString & layerId) <br> |
|  QString | [**crsByLayerId**](#function-crsbylayerid) (const QgsProject \* project, const QString & layerId) <br> |
|  QPair&lt; int, QString &gt; | [**getLocalPkAttribute**](#function-getlocalpkattribute) (const QgsVectorLayer \* vl) <br>_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer. We assume there is no multi-column primary key._ |
|  QString | [**getSourceLayerId**](#function-getsourcelayerid) (const QgsVectorLayer \* vl) <br> |
|  QPair&lt; int, QString &gt; | [**getSourcePkAttribute**](#function-getsourcepkattribute) (const QgsVectorLayer \* vl) <br>_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer, as they are in the original source._ |
|  QString | [**nameByLayerId**](#function-namebylayerid) (const QgsProject \* project, const QString & layerId) <br> |


























## Detailed Description


A class that wraps the operations with a delta file. All read and write operations to a delta file should go through this class. 


    
## Public Types Documentation




### enum ErrorType 

```C++
enum DeltaFileWrapper::ErrorType {
    NoError,
    LockError,
    NotCloudProjectError,
    IOError,
    JsonParseError,
    JsonFormatIdError,
    JsonFormatProjectIdError,
    JsonFormatVersionError,
    JsonFormatDeltasError,
    JsonFormatDeltaItemError,
    JsonIncompatibleVersionError
};
```



Error types 


        

<hr>
## Public Properties Documentation




### property count [1/2]

```C++
int DeltaFileWrapper::count;
```




<hr>



### property errorString [1/2]

```C++
QString DeltaFileWrapper::errorString;
```




<hr>



### property errorType [1/2]

```C++
ErrorType DeltaFileWrapper::errorType;
```




<hr>



### property hasError [1/2]

```C++
bool DeltaFileWrapper::hasError;
```




<hr>



### property isPushing [1/2]

```C++
bool DeltaFileWrapper::isPushing;
```




<hr>
## Public Signals Documentation




### signal countChanged 

```C++
void DeltaFileWrapper::countChanged;
```



Emitted when the `deltas` list has changed. 


        

<hr>



### signal errorChanged 

```C++
void DeltaFileWrapper::errorChanged;
```



Emmitted when the latest error has changed 


        

<hr>



### signal isPushingChanged 

```C++
void DeltaFileWrapper::isPushingChanged;
```



Emmitted when the pushing state has changed. 


        

<hr>



### signal savedToFile 

```C++
void DeltaFileWrapper::savedToFile;
```



Emmitted when the deltas have been written to a file 


        

<hr>
## Public Functions Documentation




### function DeltaFileWrapper 

```C++
DeltaFileWrapper::DeltaFileWrapper (
    const QString & projectId,
    const QString & fileName
) 
```



Construct a new Feature Deltas object.




**Parameters:**


* `project` the current project 
* `fileName` complete file name with path where the object should be stored 




        

<hr>



### function addCreate 

```C++
void DeltaFileWrapper::addCreate (
    const QgsProject * project,
    const QString & localLayerId,
    const QString & sourceLayerId,
    const QString & localPkAttrName,
    const QString & sourcePkAttrName,
    const QgsFeature & newFeature
) 
```



Adds create delta.




**Parameters:**


* `localLayerId` layer ID where the old feature belongs to 
* `sourceLayerId` layer ID where the old feature belongs to 
* `localPkAttrName` attribute name of the primary key 
* `sourcePkAttrName` attribute name of the primary key 
* `newFeature` the feature that has been created 




        

<hr>



### function addDelete 

```C++
void DeltaFileWrapper::addDelete (
    const QgsProject * project,
    const QString & localLayerId,
    const QString & sourceLayerId,
    const QString & localPkAttrName,
    const QString & sourcePkAttrName,
    const QgsFeature & oldFeature
) 
```



Adds delete delta.




**Parameters:**


* `localLayerId` layer ID where the old feature belongs to 
* `sourceLayerId` layer ID where the old feature belongs to 
* `localPkAttrName` attribute name of the primary key 
* `sourcePkAttrName` attribute name of the primary key 
* `oldFeature` the feature that has been deleted 




        

<hr>



### function addPatch 

```C++
void DeltaFileWrapper::addPatch (
    const QgsProject * project,
    const QString & localLayerId,
    const QString & sourceLayerId,
    const QString & localPkAttrName,
    const QString & sourcePkAttrName,
    const QgsFeature & oldFeature,
    const QgsFeature & newFeature,
    bool storeSnapshot=true
) 
```



Adds patch delta.




**Parameters:**


* `localLayerId` layer ID where the old feature belongs to 
* `sourceLayerId` layer ID where the old feature belongs to 
* `localPkAttrName` attribute name of the primary key 
* `sourcePkAttrName` attribute name of the primary key 
* `oldFeature` the old version of the feature that has been modified 
* `newFeature` the new version of the feature that has been modified 
* `storeSnapshot` store a complete snapshot of the feature 




        

<hr>



### function append 

```C++
bool DeltaFileWrapper::append (
    const DeltaFileWrapper * deltaFileWrapper
) 
```



Appends the provided deltas JSON array at the end of the current file. 


        

<hr>



### function apply 

```C++
Q_INVOKABLE bool DeltaFileWrapper::apply (
    const QgsProject * project
) 
```



Attempts to apply a delta file. The list of deltas is not being reset after successfull application and should be handled by the caller.




**Note:**

it is not guaranteed that the project layers have not changed in case of failure 




**Returns:**

whether the attempt was successful 




**Todo**

TEST 




        

<hr>



### function applyReversed 

```C++
Q_INVOKABLE bool DeltaFileWrapper::applyReversed (
    const QgsProject * project
) 
```



Attempts to apply a delta file in reverse order (resulting in local changes being discarded). The list of deltas is not being reset after successfull application and should be handled by the caller.




**Note:**

it is not guaranteed that the project layers have not changed in case of failure 




**Returns:**

whether the attempt was successful. 




**Todo**

TEST 




        

<hr>



### function attachmentFileNames 

```C++
QMap< QString, QString > DeltaFileWrapper::attachmentFileNames () const
```



Returns a set of file names to be uploaded




**Returns:**

QMap&lt;QString, QString&gt; unique file names 





        

<hr>



### function count [2/2]

```C++
Q_INVOKABLE int DeltaFileWrapper::count () const
```



Returns the number of delta elements




**Returns:**

int number of delta elements 





        

<hr>



### function deltaLayerIds 

```C++
QStringList DeltaFileWrapper::deltaLayerIds () const
```



Returns the list of layers that have applied deltas on them. 

**Returns:**

list of layers ids 




**Todo**

TEST 




        

<hr>



### function deltas 

```C++
QJsonArray DeltaFileWrapper::deltas () const
```



Returns the deltas as a JSON array of delta elements




**Returns:**

QJsonArray deltas JSON array 





        

<hr>



### function errorString [2/2]

```C++
QString DeltaFileWrapper::errorString () const
```



Human readable error description why the class has an error.




**Returns:**

QString human readable error reason 





        

<hr>



### function errorType [2/2]

```C++
ErrorType DeltaFileWrapper::errorType () const
```



Error type why the class has an error.




**Returns:**

ErrorType error type 





        

<hr>



### function fileName 

```C++
QString DeltaFileWrapper::fileName () const
```



Returns deltas file name.




**Returns:**

QString file name 





        

<hr>



### function getDeltaIndexByUuid 

```C++
int DeltaFileWrapper::getDeltaIndexByUuid (
    const QString & uuid
) const
```



Retuns the index position of a delta with given _uuid_ in the deltas list or -1 if missing.




**Parameters:**


* `uuid` the uuid we are looking for 




        

<hr>



### function hasError [2/2]

```C++
bool DeltaFileWrapper::hasError () const
```



Returns whether the class has encountered I/O error regarding the delta file. If true is returned, the behaviour of the class instance is no more defined.




**Returns:**

bool whether an error has been encountered 





        

<hr>



### function id 

```C++
QString DeltaFileWrapper::id () const
```



Returns deltas file id.




**Returns:**

QString id 





        

<hr>



### function isCreatedFeature 

```C++
Q_INVOKABLE bool DeltaFileWrapper::isCreatedFeature (
    QgsVectorLayer * vl,
    QgsFeature feature
) 
```



Returns TRUE if a feature from a given vector layer is recorded as being created in the deltas file.




**Parameters:**


* `vl` the vector layer 
* `feature` the feature 




        

<hr>



### function isDeltaBeingApplied 

```C++
bool DeltaFileWrapper::isDeltaBeingApplied () const
```



Whether or not the current delta file is being applied.




**Todo**

TEST 




        

<hr>



### function isDirty 

```C++
bool DeltaFileWrapper::isDirty () const
```



Returns whether the instance contents differs from the data saved on the disk.




**Returns:**

bool whether there is a difference with the data saved on the disk 





        

<hr>



### function isPushing [2/2]

```C++
inline bool DeltaFileWrapper::isPushing () const
```



Returns TRUE if the pushing state is active. 


        

<hr>



### function projectId 

```C++
QString DeltaFileWrapper::projectId () const
```



Returns deltas file project id.




**Returns:**

QString project id 





        

<hr>



### function reset 

```C++
Q_INVOKABLE void DeltaFileWrapper::reset () 
```



Clears the deltas from memory as there are no deltas at all. Does not affect the permanent storage until `toFile()` is called. 


        

<hr>



### function resetId 

```C++
Q_INVOKABLE void DeltaFileWrapper::resetId () 
```



Assigns a new unique delta file id. 


        

<hr>



### function setIsPushing 

```C++
void DeltaFileWrapper::setIsPushing (
    bool isPushing
) 
```



Sets the pushing state.




**Parameters:**


* `isPushing` set to TRUE to reflect an ongoing pushing state. 




        

<hr>



### function toFile 

```C++
Q_INVOKABLE bool DeltaFileWrapper::toFile () 
```



Writes deltas file to the permanent storage.




**Returns:**

bool whether write has been successful 





        

<hr>



### function toFileForPush 

```C++
Q_INVOKABLE QString DeltaFileWrapper::toFileForPush (
    const QString & outFileName=QString()
) const
```



Writes deltas file to the permanent storage with replaced layerIds, ready for upload.




**Returns:**

QString an empty string 





        

<hr>



### function toJson 

```C++
QByteArray DeltaFileWrapper::toJson (
    QJsonDocument::JsonFormat jsonFormat=QJsonDocument::Indented
) const
```



Returns deltas as JSON QByteArray, ready for I/O operations.




**Parameters:**


* `jsonFormat` formatting of the output JSON. Default: QJsonDocument::Indented 



**Returns:**

QByteArray JSON representation 





        

<hr>



### function toString 

```C++
QString DeltaFileWrapper::toString () const
```



Returns deltas as JSON string.




**Returns:**

QString JSON representation 





        

<hr>



### function ~DeltaFileWrapper 

```C++
DeltaFileWrapper::~DeltaFileWrapper () 
```



Destroy the Delta File Wrapper object 


        

<hr>
## Public Static Functions Documentation




### function attachmentFieldNames 

```C++
static QStringList DeltaFileWrapper::attachmentFieldNames (
    const QgsProject * project,
    const QString & layerId
) 
```



Returns a list of field names that have edit form as attachments




**Parameters:**


* `project` current project instance 
* `layerId` layer ID 



**Returns:**

QStringList list of field names 





        

<hr>



### function crsByLayerId 

```C++
static QString DeltaFileWrapper::crsByLayerId (
    const QgsProject * project,
    const QString & layerId
) 
```



Returns the CRS as WKT for given _layerId_ in the _project_ 


        

<hr>



### function getLocalPkAttribute 

_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer. We assume there is no multi-column primary key._
```C++
static QPair< int, QString > DeltaFileWrapper::getLocalPkAttribute (
    const QgsVectorLayer * vl
) 
```




<hr>



### function getSourceLayerId 

```C++
static QString DeltaFileWrapper::getSourceLayerId (
    const QgsVectorLayer * vl
) 
```



Get the layer id as it is in the original source. 


        

<hr>



### function getSourcePkAttribute 

_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer, as they are in the original source._
```C++
static QPair< int, QString > DeltaFileWrapper::getSourcePkAttribute (
    const QgsVectorLayer * vl
) 
```




<hr>



### function nameByLayerId 

```C++
static QString DeltaFileWrapper::nameByLayerId (
    const QgsProject * project,
    const QString & layerId
) 
```



Returns the name for given _layerId_ in the _project_ 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/deltafilewrapper.h`

