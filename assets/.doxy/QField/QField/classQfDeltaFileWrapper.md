

# Class QfDeltaFileWrapper



[**ClassList**](annotated.md) **>** [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md)



[More...](#detailed-description)

* `#include <qfdeltafilewrapper.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ErrorType**](#enum-errortype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**addedCount**](classQfDeltaFileWrapper.md#property-addedcount-12)  <br> |
| property int | [**count**](classQfDeltaFileWrapper.md#property-count-12)  <br> |
| property int | [**deletedCount**](classQfDeltaFileWrapper.md#property-deletedcount-12)  <br> |
| property int | [**editedCount**](classQfDeltaFileWrapper.md#property-editedcount-12)  <br> |
| property QString | [**errorString**](classQfDeltaFileWrapper.md#property-errorstring-12)  <br> |
| property [**ErrorType**](classQfDeltaFileWrapper.md#enum-errortype) | [**errorType**](classQfDeltaFileWrapper.md#property-errortype-12)  <br> |
| property bool | [**hasError**](classQfDeltaFileWrapper.md#property-haserror-12)  <br> |
| property bool | [**isPushing**](classQfDeltaFileWrapper.md#property-ispushing-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classQfDeltaFileWrapper.md#signal-countchanged)  <br> |
| signal void | [**errorChanged**](classQfDeltaFileWrapper.md#signal-errorchanged)  <br> |
| signal void | [**isPushingChanged**](classQfDeltaFileWrapper.md#signal-ispushingchanged)  <br> |
| signal void | [**savedToFile**](classQfDeltaFileWrapper.md#signal-savedtofile)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDeltaFileWrapper**](#function-qfdeltafilewrapper) (const QString & projectId, const QString & fileName) <br> |
|  void | [**addCreate**](#function-addcreate) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & newFeature) <br> |
|  void | [**addDelete**](#function-adddelete) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & oldFeature) <br> |
|  void | [**addPatch**](#function-addpatch) (const QgsProject \* project, const QString & localLayerId, const QString & sourceLayerId, const QString & localPkAttrName, const QString & sourcePkAttrName, const QgsFeature & oldFeature, const QgsFeature & newFeature, bool storeSnapshot=true) <br> |
|  int | [**addedCount**](#function-addedcount-22) () const<br> |
|  bool | [**append**](#function-append) (const [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* deltaFileWrapper) <br> |
|  Q\_INVOKABLE bool | [**apply**](#function-apply) (const QgsProject \* project) <br> |
|  Q\_INVOKABLE bool | [**applyReversed**](#function-applyreversed) (const QgsProject \* project) <br> |
|  QMap&lt; QString, QString &gt; | [**attachmentFileNames**](#function-attachmentfilenames) () const<br> |
|  Q\_INVOKABLE int | [**count**](#function-count-22) () const<br> |
|  int | [**deletedCount**](#function-deletedcount-22) () const<br> |
|  QStringList | [**deltaLayerIds**](#function-deltalayerids) () const<br> |
|  QJsonArray | [**deltas**](#function-deltas) () const<br> |
|  int | [**editedCount**](#function-editedcount-22) () const<br> |
|  QString | [**errorString**](#function-errorstring-22) () const<br> |
|  [**ErrorType**](classQfDeltaFileWrapper.md#enum-errortype) | [**errorType**](#function-errortype-22) () const<br> |
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
|   | [**~QfDeltaFileWrapper**](#function-qfdeltafilewrapper) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QStringList | [**attachmentFieldNames**](#function-attachmentfieldnames) (const QgsProject \* project, const QString & layerId) <br> |
|  QString | [**crsByLayerId**](#function-crsbylayerid) (const QgsProject \* project, const QString & layerId) <br> |
|  QPair&lt; int, QString &gt; | [**getLocalPkAttribute**](#function-getlocalpkattribute) (const QgsVectorLayer \* vl) <br>_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer. We assume there is no multi-column primary key._ |
|  QString | [**getSourceLayerId**](#function-getsourcelayerid) (const QgsVectorLayer \* vl) <br> |
|  QPair&lt; int, QString &gt; | [**getSourcePkAttribute**](#function-getsourcepkattribute) (const QgsVectorLayer \* vl) <br>_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer, as they are in the original source._ |
|  QgsFeatureRequest | [**localPkRequest**](#function-localpkrequest) (const QgsVectorLayer \* vl, const QStringList & localPks) <br>_Get a request matching the features of given_ _vl_ _layer whose local primary key is one of__localPks_ _. Returns a request matching nothing when the layer has no usable primary key._ |
|  QString | [**nameByLayerId**](#function-namebylayerid) (const QgsProject \* project, const QString & layerId) <br> |


























## Detailed Description


A class that wraps the operations with a delta file. All read and write operations to a delta file should go through this class. 


    
## Public Types Documentation




### enum ErrorType 

```C++
enum QfDeltaFileWrapper::ErrorType {
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




### property addedCount [1/2]

```C++
int QfDeltaFileWrapper::addedCount;
```




<hr>



### property count [1/2]

```C++
int QfDeltaFileWrapper::count;
```




<hr>



### property deletedCount [1/2]

```C++
int QfDeltaFileWrapper::deletedCount;
```




<hr>



### property editedCount [1/2]

```C++
int QfDeltaFileWrapper::editedCount;
```




<hr>



### property errorString [1/2]

```C++
QString QfDeltaFileWrapper::errorString;
```




<hr>



### property errorType [1/2]

```C++
ErrorType QfDeltaFileWrapper::errorType;
```




<hr>



### property hasError [1/2]

```C++
bool QfDeltaFileWrapper::hasError;
```




<hr>



### property isPushing [1/2]

```C++
bool QfDeltaFileWrapper::isPushing;
```




<hr>
## Public Signals Documentation




### signal countChanged 

```C++
void QfDeltaFileWrapper::countChanged;
```



Emitted when the `deltas` list has changed. 


        

<hr>



### signal errorChanged 

```C++
void QfDeltaFileWrapper::errorChanged;
```



Emmitted when the latest error has changed 


        

<hr>



### signal isPushingChanged 

```C++
void QfDeltaFileWrapper::isPushingChanged;
```



Emmitted when the pushing state has changed. 


        

<hr>



### signal savedToFile 

```C++
void QfDeltaFileWrapper::savedToFile;
```



Emmitted when the deltas have been written to a file 


        

<hr>
## Public Functions Documentation




### function QfDeltaFileWrapper 

```C++
QfDeltaFileWrapper::QfDeltaFileWrapper (
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
void QfDeltaFileWrapper::addCreate (
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
void QfDeltaFileWrapper::addDelete (
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
void QfDeltaFileWrapper::addPatch (
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



### function addedCount [2/2]

```C++
int QfDeltaFileWrapper::addedCount () const
```



Returns the number of delta elements creating a feature




**Returns:**

int number of created features 





        

<hr>



### function append 

```C++
bool QfDeltaFileWrapper::append (
    const QfDeltaFileWrapper * deltaFileWrapper
) 
```



Appends the provided deltas JSON array at the end of the current file. 


        

<hr>



### function apply 

```C++
Q_INVOKABLE bool QfDeltaFileWrapper::apply (
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
Q_INVOKABLE bool QfDeltaFileWrapper::applyReversed (
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
QMap< QString, QString > QfDeltaFileWrapper::attachmentFileNames () const
```



Returns a set of file names to be uploaded




**Returns:**

QMap&lt;QString, QString&gt; unique file names 





        

<hr>



### function count [2/2]

```C++
Q_INVOKABLE int QfDeltaFileWrapper::count () const
```



Returns the number of delta elements




**Returns:**

int number of delta elements 





        

<hr>



### function deletedCount [2/2]

```C++
int QfDeltaFileWrapper::deletedCount () const
```



Returns the number of delta elements removing a feature




**Returns:**

int number of deleted features 





        

<hr>



### function deltaLayerIds 

```C++
QStringList QfDeltaFileWrapper::deltaLayerIds () const
```



Returns the list of layers that have applied deltas on them. 

**Returns:**

list of layers ids 




**Todo**

TEST 




        

<hr>



### function deltas 

```C++
QJsonArray QfDeltaFileWrapper::deltas () const
```



Returns the deltas as a JSON array of delta elements




**Returns:**

QJsonArray deltas JSON array 





        

<hr>



### function editedCount [2/2]

```C++
int QfDeltaFileWrapper::editedCount () const
```



Returns the number of delta elements modifying a feature




**Returns:**

int number of edited features 





        

<hr>



### function errorString [2/2]

```C++
QString QfDeltaFileWrapper::errorString () const
```



Human readable error description why the class has an error.




**Returns:**

QString human readable error reason 





        

<hr>



### function errorType [2/2]

```C++
ErrorType QfDeltaFileWrapper::errorType () const
```



Error type why the class has an error.




**Returns:**

ErrorType error type 





        

<hr>



### function fileName 

```C++
QString QfDeltaFileWrapper::fileName () const
```



Returns deltas file name.




**Returns:**

QString file name 





        

<hr>



### function getDeltaIndexByUuid 

```C++
int QfDeltaFileWrapper::getDeltaIndexByUuid (
    const QString & uuid
) const
```



Retuns the index position of a delta with given _uuid_ in the deltas list or -1 if missing.




**Parameters:**


* `uuid` the uuid we are looking for 




        

<hr>



### function hasError [2/2]

```C++
bool QfDeltaFileWrapper::hasError () const
```



Returns whether the class has encountered I/O error regarding the delta file. If true is returned, the behaviour of the class instance is no more defined.




**Returns:**

bool whether an error has been encountered 





        

<hr>



### function id 

```C++
QString QfDeltaFileWrapper::id () const
```



Returns deltas file id.




**Returns:**

QString id 





        

<hr>



### function isCreatedFeature 

```C++
Q_INVOKABLE bool QfDeltaFileWrapper::isCreatedFeature (
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
bool QfDeltaFileWrapper::isDeltaBeingApplied () const
```



Whether or not the current delta file is being applied.




**Todo**

TEST 




        

<hr>



### function isDirty 

```C++
bool QfDeltaFileWrapper::isDirty () const
```



Returns whether the instance contents differs from the data saved on the disk.




**Returns:**

bool whether there is a difference with the data saved on the disk 





        

<hr>



### function isPushing [2/2]

```C++
inline bool QfDeltaFileWrapper::isPushing () const
```



Returns TRUE if the pushing state is active. 


        

<hr>



### function projectId 

```C++
QString QfDeltaFileWrapper::projectId () const
```



Returns deltas file project id.




**Returns:**

QString project id 





        

<hr>



### function reset 

```C++
Q_INVOKABLE void QfDeltaFileWrapper::reset () 
```



Clears the deltas from memory as there are no deltas at all. Does not affect the permanent storage until `toFile()` is called. 


        

<hr>



### function resetId 

```C++
Q_INVOKABLE void QfDeltaFileWrapper::resetId () 
```



Assigns a new unique delta file id. 


        

<hr>



### function setIsPushing 

```C++
void QfDeltaFileWrapper::setIsPushing (
    bool isPushing
) 
```



Sets the pushing state.




**Parameters:**


* `isPushing` set to TRUE to reflect an ongoing pushing state. 




        

<hr>



### function toFile 

```C++
Q_INVOKABLE bool QfDeltaFileWrapper::toFile () 
```



Writes deltas file to the permanent storage.




**Returns:**

bool whether write has been successful 





        

<hr>



### function toFileForPush 

```C++
Q_INVOKABLE QString QfDeltaFileWrapper::toFileForPush (
    const QString & outFileName=QString()
) const
```



Writes deltas file to the permanent storage with replaced layerIds, ready for upload.




**Returns:**

QString an empty string 





        

<hr>



### function toJson 

```C++
QByteArray QfDeltaFileWrapper::toJson (
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
QString QfDeltaFileWrapper::toString () const
```



Returns deltas as JSON string.




**Returns:**

QString JSON representation 





        

<hr>



### function ~QfDeltaFileWrapper 

```C++
QfDeltaFileWrapper::~QfDeltaFileWrapper () 
```



Destroy the Delta File Wrapper object 


        

<hr>
## Public Static Functions Documentation




### function attachmentFieldNames 

```C++
static QStringList QfDeltaFileWrapper::attachmentFieldNames (
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
static QString QfDeltaFileWrapper::crsByLayerId (
    const QgsProject * project,
    const QString & layerId
) 
```



Returns the CRS as WKT for given _layerId_ in the _project_ 


        

<hr>



### function getLocalPkAttribute 

_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer. We assume there is no multi-column primary key._
```C++
static QPair< int, QString > QfDeltaFileWrapper::getLocalPkAttribute (
    const QgsVectorLayer * vl
) 
```




<hr>



### function getSourceLayerId 

```C++
static QString QfDeltaFileWrapper::getSourceLayerId (
    const QgsVectorLayer * vl
) 
```



Get the layer id as it is in the original source. 


        

<hr>



### function getSourcePkAttribute 

_Get a pair of the index and the name of the primary key column of given_ _vl_ _layer, as they are in the original source._
```C++
static QPair< int, QString > QfDeltaFileWrapper::getSourcePkAttribute (
    const QgsVectorLayer * vl
) 
```




<hr>



### function localPkRequest 

_Get a request matching the features of given_ _vl_ _layer whose local primary key is one of__localPks_ _. Returns a request matching nothing when the layer has no usable primary key._
```C++
static QgsFeatureRequest QfDeltaFileWrapper::localPkRequest (
    const QgsVectorLayer * vl,
    const QStringList & localPks
) 
```




<hr>



### function nameByLayerId 

```C++
static QString QfDeltaFileWrapper::nameByLayerId (
    const QgsProject * project,
    const QString & layerId
) 
```



Returns the name for given _layerId_ in the _project_ 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfdeltafilewrapper.h`

