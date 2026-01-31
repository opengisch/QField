

# Class QFieldCloudUtils



[**ClassList**](annotated.md) **>** [**QFieldCloudUtils**](classQFieldCloudUtils.md)








Inherits the following classes: QObject




































## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE void | [**addPendingAttachments**](#function-addpendingattachments) (const QString & username, const QString & projectId, const QStringList & fileNames, [**QFieldCloudConnection**](classQFieldCloudConnection.md) \* cloudConnection=nullptr, const bool & checkSumCheck=false) <br> |
|  Q\_INVOKABLE QString | [**documentationFromErrorString**](#function-documentationfromerrorstring) (const QString & errorString) <br> |
|  const QMultiMap&lt; QString, QString &gt; | [**getPendingAttachments**](#function-getpendingattachments) (const QString & username) <br>_Returns the list of attachments that have not yet been uploaded to the cloud._  |
|  Q\_INVOKABLE const QString | [**getProjectId**](#function-getprojectid) (const QString & fileName) <br> |
|  Q\_INVOKABLE bool | [**hasPendingAttachments**](#function-haspendingattachments) (const QString & username) <br>_Returns TRUE if pending attachments are detected._  |
|  bool | [**isCloudAction**](#function-iscloudaction) (const QgsMapLayer \* layer) <br> |
|  const QString | [**localCloudDirectory**](#function-localclouddirectory) () <br> |
|  const QString | [**localProjectFilePath**](#function-localprojectfilepath) (const QString & username, const QString & projectId) <br> |
|  const QVariant | [**projectSetting**](#function-projectsetting) (const QString & projectId, const QString & setting, const QVariant & defaultValue=QVariant()) <br>_Gets a_ _setting_ _value for project with given__projectId_ _from the permanent storage. Return__defaultValue_ _if not present._ |
|  Q\_INVOKABLE void | [**removePendingAttachment**](#function-removependingattachment) (const QString & username, const QString & projectId, const QString & fileName) <br>_Removes a_ _fileName_ _for a given__projectId_ _to the pending attachments list._ |
|  void | [**setLocalCloudDirectory**](#function-setlocalclouddirectory) (const QString & path) <br> |
|  void | [**setProjectSetting**](#function-setprojectsetting) (const QString & projectId, const QString & setting, const QVariant & value) <br>_Sets a_ _setting_ _to a given__value_ _for project with given__projectId_ _to the permanent storage._ |
|  Q\_INVOKABLE QString | [**userFriendlyErrorString**](#function-userfriendlyerrorstring) (const QString & errorString) <br> |


























## Public Static Functions Documentation




### function addPendingAttachments 

```C++
static Q_INVOKABLE void QFieldCloudUtils::addPendingAttachments (
    const QString & username,
    const QString & projectId,
    const QStringList & fileNames,
    QFieldCloudConnection * cloudConnection=nullptr,
    const bool & checkSumCheck=false
) 
```



Adds an array of files and/or folders for a given cloud project to the pending upload attachments list. If _checkSumCheck_ is true, checks file checksums with the server; otherwise, adds all files without validation.




**Parameters:**


* `projectId` The project ID for which files are added. 
* `fileNames` The list of file and/or folder path(s) to be added. 
* `cloudConnection` The cloud connection used to fetch file data. 
* `checkSumCheck` Whether to validate files by comparing checksums with the server. 




        

<hr>



### function documentationFromErrorString 

```C++
static Q_INVOKABLE QString QFieldCloudUtils::documentationFromErrorString (
    const QString & errorString
) 
```



Returns a documentation page hyperlink related to the provided error string.




**Parameters:**


* `errorString` the error string to be processed 



**Returns:**

The hyperlink to the documentation page related to the provided error code, or an empty string if no match is found. 





        

<hr>



### function getPendingAttachments 

_Returns the list of attachments that have not yet been uploaded to the cloud._ 
```C++
static const QMultiMap< QString, QString > QFieldCloudUtils::getPendingAttachments (
    const QString & username
) 
```




<hr>



### function getProjectId 

```C++
static Q_INVOKABLE const QString QFieldCloudUtils::getProjectId (
    const QString & fileName
) 
```



Returns the cloud project ID for a given file path.


This function checks if the given file path is under the QField local cloud directory. If it is, it extracts and returns the project ID (UUID-like string) from the path. Otherwise, it returns a null QString.




**Parameters:**


* `fileName` Full path to a file or directory inside a cloud project 



**Returns:**

QString Project ID if found; otherwise, an empty string 





        

<hr>



### function hasPendingAttachments 

_Returns TRUE if pending attachments are detected._ 
```C++
static Q_INVOKABLE bool QFieldCloudUtils::hasPendingAttachments (
    const QString & username
) 
```




<hr>



### function isCloudAction 

```C++
static bool QFieldCloudUtils::isCloudAction (
    const QgsMapLayer * layer
) 
```



Returns if the layer action has do be handled with QFieldCloud.




**Parameters:**


* `layer` to be checked 



**Returns:**

const bool true if the layer has the action not selected 'no\_action' (on online databases) or 'remove' 





        

<hr>



### function localCloudDirectory 

```C++
static const QString QFieldCloudUtils::localCloudDirectory () 
```



Returns the path to the local cloud directory. By default inside the user profile unless overwritten with setLocalCloudDirectory 

**Note:**

The returned path will never have have a trailing '/' or '\' . 





        

<hr>



### function localProjectFilePath 

```C++
static const QString QFieldCloudUtils::localProjectFilePath (
    const QString & username,
    const QString & projectId
) 
```



Returns the file path to the locally-stored cloud project file. 


        

<hr>



### function projectSetting 

_Gets a_ _setting_ _value for project with given__projectId_ _from the permanent storage. Return__defaultValue_ _if not present._
```C++
static const QVariant QFieldCloudUtils::projectSetting (
    const QString & projectId,
    const QString & setting,
    const QVariant & defaultValue=QVariant()
) 
```




<hr>



### function removePendingAttachment 

_Removes a_ _fileName_ _for a given__projectId_ _to the pending attachments list._
```C++
static Q_INVOKABLE void QFieldCloudUtils::removePendingAttachment (
    const QString & username,
    const QString & projectId,
    const QString & fileName
) 
```




<hr>



### function setLocalCloudDirectory 

```C++
static void QFieldCloudUtils::setLocalCloudDirectory (
    const QString & path
) 
```



Sets the local cloud directory. Must be set before the first call to localCloudDirectory. 


        

<hr>



### function setProjectSetting 

_Sets a_ _setting_ _to a given__value_ _for project with given__projectId_ _to the permanent storage._
```C++
static void QFieldCloudUtils::setProjectSetting (
    const QString & projectId,
    const QString & setting,
    const QVariant & value
) 
```




<hr>



### function userFriendlyErrorString 

```C++
static Q_INVOKABLE QString QFieldCloudUtils::userFriendlyErrorString (
    const QString & errorString
) 
```



Returns a user-friendly error message.




**Parameters:**


* `errorString` the error string to be processed. 



**Returns:**

A user-friendly error message that will be displayed to the user, translated based on received error code. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfieldcloudutils.h`

