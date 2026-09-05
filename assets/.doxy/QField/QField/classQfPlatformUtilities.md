

# Class QfPlatformUtilities



[**ClassList**](annotated.md) **>** [**QfPlatformUtilities**](classQfPlatformUtilities.md)








Inherits the following classes: QObject


Inherited by the following classes: [QfAndroidPlatformUtilities](classQfAndroidPlatformUtilities.md),  [QfIosPlatformUtilities](classQfIosPlatformUtilities.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](#enum-capability)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QfPlatformUtilities::Capabilities | [**capabilities**](classQfPlatformUtilities.md#property-capabilities-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**resourceCanceled**](classQfPlatformUtilities.md#signal-resourcecanceled) (const QString & message) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceOpened**](classQfPlatformUtilities.md#signal-resourceopened) (const QString & path) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceReceived**](classQfPlatformUtilities.md#signal-resourcereceived) (const QString & path) <br>_Emitted when a resource has been received._  |




## Public Functions

| Type | Name |
| ---: | :--- |
| virtual QStringList | [**additionalApplicationDirectories**](#function-additionalapplicationdirectories) () const<br> |
| virtual void | [**afterUpdate**](#function-afterupdate) () <br> |
| virtual Q\_INVOKABLE QStringList | [**appDataDirs**](#function-appdatadirs) () const<br> |
| virtual Q\_INVOKABLE QString | [**applicationDirectory**](#function-applicationdirectory) () const<br> |
|  Q\_INVOKABLE QStringList | [**availableGrids**](#function-availablegrids) () const<br> |
| virtual QfPlatformUtilities::Capabilities | [**capabilities**](#function-capabilities-22) () const<br> |
| virtual Qt::PermissionStatus | [**checkCameraPermission**](#function-checkcamerapermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkCameraPermissions**](#function-checkcamerapermissions) () const<br> |
| virtual Qt::PermissionStatus | [**checkMicrophonePermission**](#function-checkmicrophonepermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkMicrophonePermissions**](#function-checkmicrophonepermissions) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkPositioningPermissions**](#function-checkpositioningpermissions) () const<br> |
| virtual Q\_INVOKABLE void | [**copyTextToClipboard**](#function-copytexttoclipboard) (const QString & string) const<br> |
|  Q\_INVOKABLE bool | [**createDir**](#function-createdir) (const QString & path, const QString & dirname) const<br> |
| virtual Q\_INVOKABLE void | [**dimBrightness**](#function-dimbrightness) () <br> |
| virtual void | [**executeQfAction**](#function-executeqfaction) () const<br> |
| virtual Q\_INVOKABLE void | [**exportDatasetTo**](#function-exportdatasetto) (const QString & path) const<br>_Exports a dataset_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE void | [**exportFolderTo**](#function-exportfolderto) (const QString & path) const<br>_Exports a folder_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraPicture**](#function-getcamerapicture) (const QString & prefix, const QString & pictureFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraVideo**](#function-getcameravideo) (const QString & prefix, const QString & videoFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getFile**](#function-getfile) (const QString & prefix, const QString & filePath, const QString & mimeType=QStringLiteral("\*/\*"), QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryPicture**](#function-getgallerypicture) (const QString & prefix, const QString & pictureFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryVideo**](#function-getgalleryvideo) (const QString & prefix, const QString & videoFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE QString | [**getTextFromClipboard**](#function-gettextfromclipboard) () const<br> |
| virtual bool | [**hasQfAction**](#function-hasqfaction) () const<br> |
| virtual bool | [**hasQgsProject**](#function-hasqgsproject) () const<br> |
| virtual Q\_INVOKABLE void | [**importDatasets**](#function-importdatasets) () const<br>_Requests and imports one or more datasets into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectArchive**](#function-importprojectarchive) () const<br>_Requests and imports a project archive into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectFolder**](#function-importprojectfolder) () const<br>_Requests and imports a project folder into QField's application directory action._  |
|  void | [**initSystem**](#function-initsystem) () <br> |
| virtual Q\_INVOKABLE bool | [**isSystemDarkTheme**](#function-issystemdarktheme) () const<br> |
| virtual void | [**loadQgsProject**](#function-loadqgsproject) () const<br> |
| virtual Q\_INVOKABLE [**QfViewStatus**](classQfViewStatus.md) \* | [**open**](#function-open) (const QString & uri, bool editing=false, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfProjectSource**](classQfProjectSource.md) \* | [**openProject**](#function-openproject) (QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE void | [**removeDataset**](#function-removedataset) (const QString & path) const<br>_Removes a given dataset_ _path_ _._ |
| virtual Q\_INVOKABLE void | [**removeFolder**](#function-removefolder) (const QString & path) const<br>_Removes a given folder_ _path_ _._ |
|  Q\_INVOKABLE bool | [**renameFile**](#function-renamefile) (const QString & oldFilePath, const QString & newFilePath, bool overwrite=true) const<br> |
| virtual void | [**requestBackgroundPositioningPermissions**](#function-requestbackgroundpositioningpermissions) () <br> |
| virtual void | [**requestCameraPermission**](#function-requestcamerapermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual void | [**requestMicrophonePermission**](#function-requestmicrophonepermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual Q\_INVOKABLE void | [**requestStoragePermission**](#function-requeststoragepermission) () const<br> |
| virtual Q\_INVOKABLE void | [**restoreBrightness**](#function-restorebrightness) () <br> |
|  Q\_INVOKABLE bool | [**rmFile**](#function-rmfile) (const QString & filename) const<br> |
| virtual QStringList | [**rootDirectories**](#function-rootdirectories) () const<br> |
| virtual Q\_INVOKABLE void | [**sendCompressedFilesTo**](#function-sendcompressedfilesto) (const QStringList & paths) const<br>_Compresses a list of file_ _paths_ _and sends it via the platform native API._ |
| virtual Q\_INVOKABLE void | [**sendCompressedFolderTo**](#function-sendcompressedfolderto) (const QString & path) const<br>_Compresses a folder_ _path_ _and sends it via the platform native API._ |
| virtual Q\_INVOKABLE void | [**sendDatasetTo**](#function-senddatasetto) (const QString & path) const<br>_Sends a dataset_ _path_ _via the platform native API._ |
| virtual Q\_INVOKABLE void | [**setHandleVolumeKeys**](#function-sethandlevolumekeys) (const bool handle) <br> |
| virtual Q\_INVOKABLE void | [**setScreenLockPermission**](#function-setscreenlockpermission) (const bool allowLock) <br> |
| virtual QString | [**startPositioningService**](#function-startpositioningservice) () const<br> |
| virtual void | [**stopPositioningService**](#function-stoppositioningservice) () const<br> |
| virtual Q\_INVOKABLE double | [**systemFontPointSize**](#function-systemfontpointsize) () const<br> |
| virtual Q\_INVOKABLE QString | [**systemLocalDataLocation**](#function-systemlocaldatalocation) (const QString & subDir=QString()) const<br> |
| virtual QString | [**systemSharedDataLocation**](#function-systemshareddatalocation) () const<br> |
| virtual Q\_INVOKABLE void | [**updateProjectFromArchive**](#function-updateprojectfromarchive) (const QString & projectPath) const<br> |
| virtual Q\_INVOKABLE void | [**uploadPendingAttachments**](#function-uploadpendingattachments) ([**QfCloudConnection**](classQfCloudConnection.md) \* connection) const<br> |
| virtual Q\_INVOKABLE void | [**vibrate**](#function-vibrate) (int milliseconds) const<br> |
| virtual  | [**~QfPlatformUtilities**](#function-qfplatformutilities) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**QfPlatformUtilities**](classQfPlatformUtilities.md) \* | [**instance**](#function-instance) () <br> |


























## Public Types Documentation




### enum Capability 

```C++
enum QfPlatformUtilities::Capability {
    NoCapabilities = 0,
    NativeCamera = 1,
    AdjustBrightness = 1 << 1,
    SentryFramework = 1 << 2,
    NativeLocalDataPicker = 1 << 3,
    CustomImport = 1 << 4,
    CustomExport = 1 << 5,
    CustomSend = 1 << 6,
    FilePicker = 1 << 7,
    VolumeKeys = 1 << 8,
    Vibrate = 1 << 9,
    UpdateProjectFromArchive = 1 << 10,
    PositioningService = 1 << 11
};
```




<hr>
## Public Properties Documentation




### property capabilities [1/2]

```C++
QfPlatformUtilities::Capabilities QfPlatformUtilities::capabilities;
```




<hr>
## Public Signals Documentation




### signal resourceCanceled 

_Emitted when a resource has been opened._ 
```C++
void QfPlatformUtilities::resourceCanceled;
```




<hr>



### signal resourceOpened 

_Emitted when a resource has been opened._ 
```C++
void QfPlatformUtilities::resourceOpened;
```




<hr>



### signal resourceReceived 

_Emitted when a resource has been received._ 
```C++
void QfPlatformUtilities::resourceReceived;
```




<hr>
## Public Functions Documentation




### function additionalApplicationDirectories 

```C++
virtual QStringList QfPlatformUtilities::additionalApplicationDirectories () const
```



Secondary application directories which can be used by individual platforms. 


        

<hr>



### function afterUpdate 

```C++
virtual void QfPlatformUtilities::afterUpdate () 
```



This method can be used to implement platform specific initialization tasks to be performed after an app update. This is implemented for Android to extract app assets to location where it can be accessed via filesystem. 


        

<hr>



### function appDataDirs 

```C++
virtual Q_INVOKABLE QStringList QfPlatformUtilities::appDataDirs () const
```





**Returns:**

a list of data directories where user data is searched. User data are pg\_service.conf, authentication config, grids, ... 





        

<hr>



### function applicationDirectory 

```C++
virtual Q_INVOKABLE QString QfPlatformUtilities::applicationDirectory () const
```



The main application directory within which projects and datasets can be imported. 


        

<hr>



### function availableGrids 

```C++
Q_INVOKABLE QStringList QfPlatformUtilities::availableGrids () const
```




<hr>



### function capabilities [2/2]

```C++
virtual QfPlatformUtilities::Capabilities QfPlatformUtilities::capabilities () const
```



Returns flags containing the supported capabilities of the platform. 


        

<hr>



### function checkCameraPermission 

```C++
virtual Qt::PermissionStatus QfPlatformUtilities::checkCameraPermission () const
```




<hr>



### function checkCameraPermissions 

```C++
virtual Q_DECL_DEPRECATED virtual Q_INVOKABLE bool QfPlatformUtilities::checkCameraPermissions () const
```



Checks for camera permissions on the device. If the permissions are not given, the user will be asked to grant permissions. 

**Deprecated**

Since QField 3.1 




        

<hr>



### function checkMicrophonePermission 

```C++
virtual Qt::PermissionStatus QfPlatformUtilities::checkMicrophonePermission () const
```




<hr>



### function checkMicrophonePermissions 

```C++
virtual Q_DECL_DEPRECATED virtual Q_INVOKABLE bool QfPlatformUtilities::checkMicrophonePermissions () const
```



Checks for camera microphone on the device. If the permissions are not given, the user will be asked to grant permissions. 

**Deprecated**

Since QField 3.1 




        

<hr>



### function checkPositioningPermissions 

```C++
virtual Q_DECL_DEPRECATED virtual Q_INVOKABLE bool QfPlatformUtilities::checkPositioningPermissions () const
```



Checks for positioning (GPS etc) permissions on the device. If the permissions are not given, the user will be asked to grant permissions. It will return true, if at least coarse permissions are granted. It will ask for fine permissions if none are granted. 

**Deprecated**

Since QField 3.1 




        

<hr>



### function copyTextToClipboard 

```C++
virtual Q_INVOKABLE void QfPlatformUtilities::copyTextToClipboard (
    const QString & string
) const
```



Copies a text _string_ to the system clipboard. 


        

<hr>



### function createDir 

```C++
Q_INVOKABLE bool QfPlatformUtilities::createDir (
    const QString & path,
    const QString & dirname
) const
```




<hr>



### function dimBrightness 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::dimBrightness () 
```



Dims the brightness of the screen on supported devices. 


        

<hr>



### function executeQfAction 

```C++
virtual void QfPlatformUtilities::executeQfAction () const
```



Loads the action provided during launch. 

**See also:** [**hasQfAction**](classQfPlatformUtilities.md#function-hasqfaction) 



        

<hr>



### function exportDatasetTo 

_Exports a dataset_ _path_ _to a user-specified location._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::exportDatasetTo (
    const QString & path
) const
```




<hr>



### function exportFolderTo 

_Exports a folder_ _path_ _to a user-specified location._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::exportFolderTo (
    const QString & path
) const
```




<hr>



### function getCameraPicture 

```C++
virtual Q_INVOKABLE QfResourceSource * QfPlatformUtilities::getCameraPicture (
    const QString & prefix,
    const QString & pictureFilePath,
    const QString & suffix,
    QObject * parent=nullptr
) 
```



Get a picture from camera and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `pictureFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the picture or null 





        

<hr>



### function getCameraVideo 

```C++
virtual Q_INVOKABLE QfResourceSource * QfPlatformUtilities::getCameraVideo (
    const QString & prefix,
    const QString & videoFilePath,
    const QString & suffix,
    QObject * parent=nullptr
) 
```



Get a video from camera and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `videoFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the video or null 





        

<hr>



### function getFile 

```C++
virtual Q_INVOKABLE QfResourceSource * QfPlatformUtilities::getFile (
    const QString & prefix,
    const QString & filePath,
    const QString & mimeType=QStringLiteral("*/*"),
    QObject * parent=nullptr
) 
```



Get a file from the operating system and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `filePath` The path (including subfolders and name) of the file 
* `filter` The filter string (e.g. \*.mp3) 



**Returns:**

The name of the file or null 





        

<hr>



### function getGalleryPicture 

```C++
virtual Q_INVOKABLE QfResourceSource * QfPlatformUtilities::getGalleryPicture (
    const QString & prefix,
    const QString & pictureFilePath,
    QObject * parent=nullptr
) 
```



Get a picture from gallery and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `pictureFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the picture or null 





        

<hr>



### function getGalleryVideo 

```C++
virtual Q_INVOKABLE QfResourceSource * QfPlatformUtilities::getGalleryVideo (
    const QString & prefix,
    const QString & videoFilePath,
    QObject * parent=nullptr
) 
```



Get a video from gallery and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `videoFilePath` The path (including subfolders and name) of the video 



**Returns:**

The name of the video or null 





        

<hr>



### function getTextFromClipboard 

```C++
virtual Q_INVOKABLE QString QfPlatformUtilities::getTextFromClipboard () const
```



Returns the current text from the system clipboard. 


        

<hr>



### function hasQfAction 

```C++
virtual bool QfPlatformUtilities::hasQfAction () const
```



Returns the current action string 

**See also:** loadQfAction 



        

<hr>



### function hasQgsProject 

```C++
virtual bool QfPlatformUtilities::hasQgsProject () const
```



Returns TRUE is a project file has been provided and should be opened at launch. 

**See also:** [**loadQgsProject**](classQfPlatformUtilities.md#function-loadqgsproject) 



        

<hr>



### function importDatasets 

_Requests and imports one or more datasets into QField's application directory action._ 
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::importDatasets () const
```




<hr>



### function importProjectArchive 

_Requests and imports a project archive into QField's application directory action._ 
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::importProjectArchive () const
```




<hr>



### function importProjectFolder 

_Requests and imports a project folder into QField's application directory action._ 
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::importProjectFolder () const
```




<hr>



### function initSystem 

```C++
void QfPlatformUtilities::initSystem () 
```



This method will do initialization tasks and copy sample projects to a writable location It will also call afterUpdate whenever a new version is detected. 


        

<hr>



### function isSystemDarkTheme 

```C++
virtual Q_INVOKABLE bool QfPlatformUtilities::isSystemDarkTheme () const
```



Returns TRUE is the system uses a dark theme. 


        

<hr>



### function loadQgsProject 

```C++
virtual void QfPlatformUtilities::loadQgsProject () const
```



Loads the project file provided during launch. 

**See also:** [**hasQgsProject**](classQfPlatformUtilities.md#function-hasqgsproject) 



        

<hr>



### function open 

```C++
virtual Q_INVOKABLE QfViewStatus * QfPlatformUtilities::open (
    const QString & uri,
    bool editing=false,
    QObject * parent=nullptr
) 
```



Open the resource (file, image, ...) that is available under _uri_. The mimetype is detected to indicate the system how the file should be opened. An optional _editing_ parameter can be set to true to indicate to supported systems the resource is expected to be edited. 


        

<hr>



### function openProject 

```C++
virtual Q_INVOKABLE QfProjectSource * QfPlatformUtilities::openProject (
    QObject * parent=nullptr
) 
```



Indicates the system that we want to open a project. The system shall show a suitable user interface element (like a filebrowser) to let the user select a project. The call returns immediately and the returned [**QfProjectSource**](classQfProjectSource.md) will notify when the project has actually been chosen. 


        

<hr>



### function removeDataset 

_Removes a given dataset_ _path_ _._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::removeDataset (
    const QString & path
) const
```




<hr>



### function removeFolder 

_Removes a given folder_ _path_ _._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::removeFolder (
    const QString & path
) const
```




<hr>



### function renameFile 

```C++
Q_INVOKABLE bool QfPlatformUtilities::renameFile (
    const QString & oldFilePath,
    const QString & newFilePath,
    bool overwrite=true
) const
```




<hr>



### function requestBackgroundPositioningPermissions 

```C++
inline virtual void QfPlatformUtilities::requestBackgroundPositioningPermissions () 
```




<hr>



### function requestCameraPermission 

```C++
virtual void QfPlatformUtilities::requestCameraPermission (
    std::function< void(Qt::PermissionStatus)> func
) 
```




<hr>



### function requestMicrophonePermission 

```C++
virtual void QfPlatformUtilities::requestMicrophonePermission (
    std::function< void(Qt::PermissionStatus)> func
) 
```




<hr>



### function requestStoragePermission 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::requestStoragePermission () const
```




<hr>



### function restoreBrightness 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::restoreBrightness () 
```



Restores the brightness of the screen to its original value on supported devices. 


        

<hr>



### function rmFile 

```C++
Q_INVOKABLE bool QfPlatformUtilities::rmFile (
    const QString & filename
) const
```




<hr>



### function rootDirectories 

```C++
virtual QStringList QfPlatformUtilities::rootDirectories () const
```



A list of root directories (e.g. root of an external storage) when system-wide access is available. 


        

<hr>



### function sendCompressedFilesTo 

_Compresses a list of file_ _paths_ _and sends it via the platform native API._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::sendCompressedFilesTo (
    const QStringList & paths
) const
```




<hr>



### function sendCompressedFolderTo 

_Compresses a folder_ _path_ _and sends it via the platform native API._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::sendCompressedFolderTo (
    const QString & path
) const
```




<hr>



### function sendDatasetTo 

_Sends a dataset_ _path_ _via the platform native API._
```C++
virtual Q_INVOKABLE void QfPlatformUtilities::sendDatasetTo (
    const QString & path
) const
```




<hr>



### function setHandleVolumeKeys 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::setHandleVolumeKeys (
    const bool handle
) 
```



Sets whether the device volume keys are handled by QField. 


        

<hr>



### function setScreenLockPermission 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::setScreenLockPermission (
    const bool allowLock
) 
```



Sets whether the device screen is allowed to go in lock mode. 

**Parameters:**


* `allowLock` if set to FALSE, the screen will not be allowed to lock. 




        

<hr>



### function startPositioningService 

```C++
inline virtual QString QfPlatformUtilities::startPositioningService () const
```



Starts a positioning service on supported platforms and returns the remote object URL. 


        

<hr>



### function stopPositioningService 

```C++
inline virtual void QfPlatformUtilities::stopPositioningService () const
```



Stops a positioning service on supported platforms. 


        

<hr>



### function systemFontPointSize 

```C++
virtual Q_INVOKABLE double QfPlatformUtilities::systemFontPointSize () const
```



Returns the default system font size. 


        

<hr>



### function systemLocalDataLocation 

```C++
virtual Q_INVOKABLE QString QfPlatformUtilities::systemLocalDataLocation (
    const QString & subDir=QString()
) const
```





**Returns:**

a directory where local data can be stored. this includes local qfieldcloud data or sample projects. A _subDir_ is appended to the path. 





        

<hr>



### function systemSharedDataLocation 

```C++
virtual QString QfPlatformUtilities::systemSharedDataLocation () const
```



The path to share data location. Under this path, there should be the app specific directories qgis/ proj/ qfield/ ... Refers to /share or /usr/share on Linux. This path is assumed to be read only. 


        

<hr>



### function updateProjectFromArchive 

```C++
virtual Q_INVOKABLE void QfPlatformUtilities::updateProjectFromArchive (
    const QString & projectPath
) const
```



Update a local project content from a user-picked archive file action 

**Parameters:**


* `projectPath` the project file path 




        

<hr>



### function uploadPendingAttachments 

```C++
virtual Q_INVOKABLE void QfPlatformUtilities::uploadPendingAttachments (
    QfCloudConnection * connection
) const
```



Uploads any pending attachments linked to the logged in user account. 


        

<hr>



### function vibrate 

```C++
inline virtual Q_INVOKABLE void QfPlatformUtilities::vibrate (
    int milliseconds
) const
```



Vibrates the device on supported platforms. 


        

<hr>



### function ~QfPlatformUtilities 

```C++
virtual QfPlatformUtilities::~QfPlatformUtilities () 
```




<hr>
## Public Static Functions Documentation




### function instance 

```C++
static QfPlatformUtilities * QfPlatformUtilities::instance () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/qfplatformutilities.h`

