

# Class WebdavConnection



[**ClassList**](annotated.md) **>** [**WebdavConnection**](classWebdavConnection.md)



[More...](#detailed-description)

* `#include <webdavconnection.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QStringList | [**availablePaths**](classWebdavConnection.md#property-availablepaths-12)  <br> |
| property QStringList | [**checkedPaths**](classWebdavConnection.md#property-checkedpaths-12)  <br> |
| property bool | [**isDownloadingPath**](classWebdavConnection.md#property-isdownloadingpath-12)  <br> |
| property bool | [**isFetchingAvailablePaths**](classWebdavConnection.md#property-isfetchingavailablepaths-12)  <br> |
| property bool | [**isImportingPath**](classWebdavConnection.md#property-isimportingpath-12)  <br> |
| property bool | [**isPasswordStored**](classWebdavConnection.md#property-ispasswordstored-12)  <br> |
| property bool | [**isUploadingPath**](classWebdavConnection.md#property-isuploadingpath-12)  <br> |
| property QString | [**lastError**](classWebdavConnection.md#property-lasterror-12)  <br> |
| property QString | [**password**](classWebdavConnection.md#property-password-12)  <br> |
| property double | [**progress**](classWebdavConnection.md#property-progress-12)  <br> |
| property bool | [**storePassword**](classWebdavConnection.md#property-storepassword-12)  <br> |
| property QString | [**url**](classWebdavConnection.md#property-url-12)  <br> |
| property QString | [**username**](classWebdavConnection.md#property-username-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**availablePathsChanged**](classWebdavConnection.md#signal-availablepathschanged)  <br> |
| signal void | [**checkedPathsChanged**](classWebdavConnection.md#signal-checkedpathschanged)  <br> |
| signal void | [**confirmationRequested**](classWebdavConnection.md#signal-confirmationrequested) (const QString & host, const QString & username) <br> |
| signal void | [**importSuccessful**](classWebdavConnection.md#signal-importsuccessful) (const QString & path) <br> |
| signal void | [**isDownloadingPathChanged**](classWebdavConnection.md#signal-isdownloadingpathchanged)  <br> |
| signal void | [**isFetchingAvailablePathsChanged**](classWebdavConnection.md#signal-isfetchingavailablepathschanged)  <br> |
| signal void | [**isImportingPathChanged**](classWebdavConnection.md#signal-isimportingpathchanged)  <br> |
| signal void | [**isPasswordStoredChanged**](classWebdavConnection.md#signal-ispasswordstoredchanged)  <br> |
| signal void | [**isUploadingPathChanged**](classWebdavConnection.md#signal-isuploadingpathchanged)  <br> |
| signal void | [**lastErrorChanged**](classWebdavConnection.md#signal-lasterrorchanged)  <br> |
| signal void | [**passwordChanged**](classWebdavConnection.md#signal-passwordchanged)  <br> |
| signal void | [**progressChanged**](classWebdavConnection.md#signal-progresschanged)  <br> |
| signal void | [**storePasswordChanged**](classWebdavConnection.md#signal-storepasswordchanged)  <br> |
| signal void | [**uploadFinished**](classWebdavConnection.md#signal-uploadfinished) (bool success, const QString & message) <br> |
| signal void | [**uploadSkipped**](classWebdavConnection.md#signal-uploadskipped) (const QString & reason) <br> |
| signal void | [**urlChanged**](classWebdavConnection.md#signal-urlchanged)  <br> |
| signal void | [**usernameChanged**](classWebdavConnection.md#signal-usernamechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**WebdavConnection**](#function-webdavconnection) (QObject \* parent=nullptr) <br> |
|  QStringList | [**availablePaths**](#function-availablepaths-22) () const<br> |
|  Q\_INVOKABLE void | [**cancelRequest**](#function-cancelrequest) () <br> |
|  QStringList | [**checkedPaths**](#function-checkedpaths-22) () const<br> |
|  Q\_INVOKABLE void | [**confirmRequest**](#function-confirmrequest) () <br> |
|  Q\_INVOKABLE void | [**downloadPath**](#function-downloadpath) (const QString & localPath) <br> |
|  Q\_INVOKABLE void | [**fetchAvailablePaths**](#function-fetchavailablepaths) (const QString & remotePath=QString()) <br> |
|  Q\_INVOKABLE void | [**importPath**](#function-importpath) (const QString & remotePath, const QString & localPath, QString localFolder=QString()) <br> |
|  bool | [**isDownloadingPath**](#function-isdownloadingpath-22) () const<br> |
|  bool | [**isFetchingAvailablePaths**](#function-isfetchingavailablepaths-22) () const<br> |
|  bool | [**isImportingPath**](#function-isimportingpath-22) () const<br> |
|  bool | [**isPasswordStored**](#function-ispasswordstored-22) () const<br> |
|  bool | [**isUploadingPath**](#function-isuploadingpath-22) () const<br> |
|  QString | [**lastError**](#function-lasterror-22) () const<br> |
|  QString | [**password**](#function-password-22) () const<br> |
|  double | [**progress**](#function-progress-22) () const<br> |
|  Q\_INVOKABLE void | [**requestUpload**](#function-requestupload) (const QString & projectPath, bool force=false) <br> |
|  void | [**setPassword**](#function-setpassword) (const QString & password) <br> |
|  void | [**setStorePassword**](#function-setstorepassword) (bool storePassword) <br> |
|  void | [**setUrl**](#function-seturl) (const QString & url) <br> |
|  void | [**setUsername**](#function-setusername) (const QString & username) <br> |
|  bool | [**storePassword**](#function-storepassword-22) () const<br> |
|  Q\_INVOKABLE void | [**uploadPaths**](#function-uploadpaths) (const QStringList & localPaths) <br> |
|  QString | [**url**](#function-url-22) () const<br> |
|  QString | [**username**](#function-username-22) () const<br> |
|   | [**~WebdavConnection**](#function-webdavconnection) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE QStringList | [**findWebdavProjectFolders**](#function-findwebdavprojectfolders) (const QString & basePath) <br> |
|  Q\_INVOKABLE QString | [**findWebdavRootForPath**](#function-findwebdavrootforpath) (const QString & path) <br> |
|  Q\_INVOKABLE void | [**forgetHistory**](#function-forgethistory) (const QString & url=QString(), const QString & username=QString()) <br> |
|  Q\_INVOKABLE bool | [**hasWebdavConfiguration**](#function-haswebdavconfiguration) (const QString & path) <br> |
|  Q\_INVOKABLE QVariantMap | [**importHistory**](#function-importhistory) () <br> |


























## Detailed Description


The WebDAV connection object allows connecting to a WebDAV endpoint and performing push/pull operations for imported content. 


    
## Public Properties Documentation




### property availablePaths [1/2]

```C++
QStringList WebdavConnection::availablePaths;
```




<hr>



### property checkedPaths [1/2]

```C++
QStringList WebdavConnection::checkedPaths;
```




<hr>



### property isDownloadingPath [1/2]

```C++
bool WebdavConnection::isDownloadingPath;
```




<hr>



### property isFetchingAvailablePaths [1/2]

```C++
bool WebdavConnection::isFetchingAvailablePaths;
```




<hr>



### property isImportingPath [1/2]

```C++
bool WebdavConnection::isImportingPath;
```




<hr>



### property isPasswordStored [1/2]

```C++
bool WebdavConnection::isPasswordStored;
```




<hr>



### property isUploadingPath [1/2]

```C++
bool WebdavConnection::isUploadingPath;
```




<hr>



### property lastError [1/2]

```C++
QString WebdavConnection::lastError;
```




<hr>



### property password [1/2]

```C++
QString WebdavConnection::password;
```




<hr>



### property progress [1/2]

```C++
double WebdavConnection::progress;
```




<hr>



### property storePassword [1/2]

```C++
bool WebdavConnection::storePassword;
```




<hr>



### property url [1/2]

```C++
QString WebdavConnection::url;
```




<hr>



### property username [1/2]

```C++
QString WebdavConnection::username;
```




<hr>
## Public Signals Documentation




### signal availablePathsChanged 

```C++
void WebdavConnection::availablePathsChanged;
```




<hr>



### signal checkedPathsChanged 

```C++
void WebdavConnection::checkedPathsChanged;
```




<hr>



### signal confirmationRequested 

```C++
void WebdavConnection::confirmationRequested;
```




<hr>



### signal importSuccessful 

```C++
void WebdavConnection::importSuccessful;
```




<hr>



### signal isDownloadingPathChanged 

```C++
void WebdavConnection::isDownloadingPathChanged;
```




<hr>



### signal isFetchingAvailablePathsChanged 

```C++
void WebdavConnection::isFetchingAvailablePathsChanged;
```




<hr>



### signal isImportingPathChanged 

```C++
void WebdavConnection::isImportingPathChanged;
```




<hr>



### signal isPasswordStoredChanged 

```C++
void WebdavConnection::isPasswordStoredChanged;
```




<hr>



### signal isUploadingPathChanged 

```C++
void WebdavConnection::isUploadingPathChanged;
```




<hr>



### signal lastErrorChanged 

```C++
void WebdavConnection::lastErrorChanged;
```




<hr>



### signal passwordChanged 

```C++
void WebdavConnection::passwordChanged;
```




<hr>



### signal progressChanged 

```C++
void WebdavConnection::progressChanged;
```




<hr>



### signal storePasswordChanged 

```C++
void WebdavConnection::storePasswordChanged;
```




<hr>



### signal uploadFinished 

```C++
void WebdavConnection::uploadFinished;
```




<hr>



### signal uploadSkipped 

```C++
void WebdavConnection::uploadSkipped;
```




<hr>



### signal urlChanged 

```C++
void WebdavConnection::urlChanged;
```




<hr>



### signal usernameChanged 

```C++
void WebdavConnection::usernameChanged;
```




<hr>
## Public Functions Documentation




### function WebdavConnection 

```C++
explicit WebdavConnection::WebdavConnection (
    QObject * parent=nullptr
) 
```




<hr>



### function availablePaths [2/2]

```C++
inline QStringList WebdavConnection::availablePaths () const
```



Returns a list of fetched available paths. 

**See also:** [**fetchAvailablePaths**](classWebdavConnection.md#function-fetchavailablepaths) 



        

<hr>



### function cancelRequest 

```C++
Q_INVOKABLE void WebdavConnection::cancelRequest () 
```



Cancels a requested download or upload operation. If an upload is active, the per-project upload lock is released. For automatic uploads, uploadFinished() is emitted with a failure status. 

**See also:** confirmationRequested 


**See also:** isDownloadingPath 


**See also:** isUploadingPath 



        

<hr>



### function checkedPaths [2/2]

```C++
inline QStringList WebdavConnection::checkedPaths () const
```



Returns a list of checked paths. 

**See also:** [**fetchAvailablePaths**](classWebdavConnection.md#function-fetchavailablepaths) 



        

<hr>



### function confirmRequest 

```C++
Q_INVOKABLE void WebdavConnection::confirmRequest () 
```



Launches a requested download or upload operation after the user has confirmed they want to proceed. This is typically called in response to confirmationRequested(). 

**See also:** confirmationRequested 


**See also:** isDownloadingPath 


**See also:** isUploadingPath 



        

<hr>



### function downloadPath 

```C++
Q_INVOKABLE void WebdavConnection::downloadPath (
    const QString & localPath
) 
```



Download new and modified files from an imported remote path. _localPath_ the local path within which a remote path was imported into 

**Note:**

This is not a synchronization process; files removed remotely will _not_ be removed locally. Furthermore, all files modified remotely will overwrite locally-stored files. 





        

<hr>



### function fetchAvailablePaths 

```C++
Q_INVOKABLE void WebdavConnection::fetchAvailablePaths (
    const QString & remotePath=QString()
) 
```



Fetches available remote paths from the current WebDAV endpoint. 


        

<hr>



### function importPath 

```C++
Q_INVOKABLE void WebdavConnection::importPath (
    const QString & remotePath,
    const QString & localPath,
    QString localFolder=QString()
) 
```



Imports a remote path into a local path stored on the device. _remotePath_ the remote path on the WebDAV endpoint. _localPath_ the local path _localFolder_ the local folder name containing the imported remote path content 


        

<hr>



### function isDownloadingPath [2/2]

```C++
inline bool WebdavConnection::isDownloadingPath () const
```



Returns TRUE if the WebDAV connection is conducting a download operation. 


        

<hr>



### function isFetchingAvailablePaths [2/2]

```C++
inline bool WebdavConnection::isFetchingAvailablePaths () const
```



Returns TRUE if the WebDAV connection is conducting a fetching of available remote paths. 


        

<hr>



### function isImportingPath [2/2]

```C++
inline bool WebdavConnection::isImportingPath () const
```



Returns TRUE if the WebDAV connection is conducting an import operation. 


        

<hr>



### function isPasswordStored [2/2]

```C++
inline bool WebdavConnection::isPasswordStored () const
```



Returns TRUE if a password has been stored for a given pair of username and WebDAV URL endpoint. 


        

<hr>



### function isUploadingPath [2/2]

```C++
inline bool WebdavConnection::isUploadingPath () const
```



Returns TRUE if the WebDAV connection is conducting an upload operation. 


        

<hr>



### function lastError [2/2]

```C++
inline QString WebdavConnection::lastError () const
```



Returns the last error string captured by the WebDAV connection. 


        

<hr>



### function password [2/2]

```C++
inline QString WebdavConnection::password () const
```



Returns the authentication password. 


        

<hr>



### function progress [2/2]

```C++
double WebdavConnection::progress () const
```



Returns the progress of an ongoing import, download, or upload operation. 

**Note:**

The returned value's range is 0.0 to 1.0. 





        

<hr>



### function requestUpload 

```C++
Q_INVOKABLE void WebdavConnection::requestUpload (
    const QString & projectPath,
    bool force=false
) 
```



Requests an automatic upload for the WebDAV imported project that contains _projectPath_. This is intended for background or scheduled upload workflows and will not prompt the user for credentials. The upload is skipped if the project is not WebDAV imported, if another upload is already running for the same project (upload lock), if no stored password is available, or if no local changes are detected since the last successful automatic upload (unless _force_ is TRUE).


If the upload does not start, uploadSkipped() is emitted with the reason. If it starts, uploadFinished() is emitted when the upload completes. 


        

<hr>



### function setPassword 

```C++
void WebdavConnection::setPassword (
    const QString & password
) 
```



Sets the authentication _password_. 


        

<hr>



### function setStorePassword 

```C++
void WebdavConnection::setStorePassword (
    bool storePassword
) 
```



Sets whether the current authentication password will be saved upon successful list or file transfer operation 

**Parameters:**


* `storePassword` set to TRUE to store the current authentication password. 




        

<hr>



### function setUrl 

```C++
void WebdavConnection::setUrl (
    const QString & url
) 
```



Sets the WebDAV URL endpoint used to connect, list, and transfer files. 

**Parameters:**


* `url` the URL endpoint string 




        

<hr>



### function setUsername 

```C++
void WebdavConnection::setUsername (
    const QString & username
) 
```



Sets the authentication _username_. 


        

<hr>



### function storePassword [2/2]

```C++
inline bool WebdavConnection::storePassword () const
```



Returns TRUE if the current authentication password will be saved upon successful list or file transfer operation. 


        

<hr>



### function uploadPaths 

```C++
Q_INVOKABLE void WebdavConnection::uploadPaths (
    const QStringList & localPaths
) 
```



Uploads one or more local files and/or folders to the WebDAV endpoint associated with a project previously imported via WebDAV. The provided _localPaths_ must all belong to the same imported project (i.e. share the same WebDAV configuration); otherwise no upload is started and lastError() is set.




**Note:**

This is not a synchronization process; files removed locally will not be removed remotely. Modified local files will overwrite their remote versions. Hidden dot-folders are ignored, and the WebDAV configuration and lock files are never uploaded.


Before the upload starts, confirmationRequested() is emitted so the UI can ask the user to confirm. Call [**confirmRequest()**](classWebdavConnection.md#function-confirmrequest) to proceed or [**cancelRequest()**](classWebdavConnection.md#function-cancelrequest) to abort. 


        

<hr>



### function url [2/2]

```C++
inline QString WebdavConnection::url () const
```



Returns the WebDAV URL endpoint used to connect, list, and transfer files. 


        

<hr>



### function username [2/2]

```C++
inline QString WebdavConnection::username () const
```



Returns the authentication username. 


        

<hr>



### function ~WebdavConnection 

```C++
WebdavConnection::~WebdavConnection () = default
```




<hr>
## Public Static Functions Documentation




### function findWebdavProjectFolders 

```C++
static Q_INVOKABLE QStringList WebdavConnection::findWebdavProjectFolders (
    const QString & basePath
) 
```



Scans a directory recursively and returns all folders containing a WebDAV configuration file. 

**Parameters:**


* `basePath` the directory to scan 



**Returns:**

list of absolute paths to WebDAV project folders 





        

<hr>



### function findWebdavRootForPath 

```C++
static Q_INVOKABLE QString WebdavConnection::findWebdavRootForPath (
    const QString & path
) 
```



Finds the WebDAV project root by walking up from the given path. 

**Parameters:**


* `path` the file or folder path to start from 



**Returns:**

the project root path, or empty string if not found 





        

<hr>



### function forgetHistory 

```C++
static Q_INVOKABLE void WebdavConnection::forgetHistory (
    const QString & url=QString(),
    const QString & username=QString()
) 
```




<hr>



### function hasWebdavConfiguration 

```C++
static Q_INVOKABLE bool WebdavConnection::hasWebdavConfiguration (
    const QString & path
) 
```



Returns TRUE if a given path contains a WebDAV configuration JSON file or is parented to a folder containing such a file. 


        

<hr>



### function importHistory 

```C++
static Q_INVOKABLE QVariantMap WebdavConnection::importHistory () 
```



Returns a list of import history. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/webdavconnection.h`

