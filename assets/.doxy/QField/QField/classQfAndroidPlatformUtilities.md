

# Class QfAndroidPlatformUtilities



[**ClassList**](annotated.md) **>** [**QfAndroidPlatformUtilities**](classQfAndroidPlatformUtilities.md)








Inherits the following classes: [QfPlatformUtilities](classQfPlatformUtilities.md)
















## Public Types inherited from QfPlatformUtilities

See [QfPlatformUtilities](classQfPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classQfPlatformUtilities.md#enum-capability)  <br> |
























## Public Properties inherited from QfPlatformUtilities

See [QfPlatformUtilities](classQfPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| property QfPlatformUtilities::Capabilities | [**capabilities**](classQfPlatformUtilities.md#property-capabilities-12)  <br> |








## Public Signals inherited from QfPlatformUtilities

See [QfPlatformUtilities](classQfPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| signal void | [**resourceCanceled**](classQfPlatformUtilities.md#signal-resourcecanceled) (const QString & message) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceOpened**](classQfPlatformUtilities.md#signal-resourceopened) (const QString & path) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceReceived**](classQfPlatformUtilities.md#signal-resourcereceived) (const QString & path) <br>_Emitted when a resource has been received._  |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAndroidPlatformUtilities**](#function-qfandroidplatformutilities) () <br> |
| virtual QStringList | [**additionalApplicationDirectories**](#function-additionalapplicationdirectories) () override const<br> |
| virtual void | [**afterUpdate**](#function-afterupdate) () override<br> |
| virtual QStringList | [**appDataDirs**](#function-appdatadirs) () override const<br> |
| virtual QString | [**applicationDirectory**](#function-applicationdirectory) () override const<br> |
| virtual QfPlatformUtilities::Capabilities | [**capabilities**](#function-capabilities) () override const<br> |
| virtual bool | [**checkCameraPermissions**](#function-checkcamerapermissions) () override const<br> |
| virtual bool | [**checkMicrophonePermissions**](#function-checkmicrophonepermissions) () override const<br> |
| virtual bool | [**checkPositioningPermissions**](#function-checkpositioningpermissions) () override const<br> |
| virtual void | [**dimBrightness**](#function-dimbrightness) () override<br> |
| virtual void | [**executeQfAction**](#function-executeqfaction) () override const<br> |
| virtual void | [**exportDatasetTo**](#function-exportdatasetto) (const QString & path) override const<br>_Exports a dataset_ _path_ _to a user-specified location._ |
| virtual void | [**exportFolderTo**](#function-exportfolderto) (const QString & path) override const<br>_Exports a folder_ _path_ _to a user-specified location._ |
| virtual [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraPicture**](#function-getcamerapicture) (const QString & prefix, const QString & pictureFilePath, const QString & suffix, QObject \* parent=nullptr) override<br> |
| virtual [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraVideo**](#function-getcameravideo) (const QString & prefix, const QString & videoFilePath, const QString & suffix, QObject \* parent=nullptr) override<br> |
| virtual [**QfResourceSource**](classQfResourceSource.md) \* | [**getFile**](#function-getfile) (const QString & prefix, const QString & filePath, const QString & mimeType, QObject \* parent=nullptr) override<br> |
| virtual [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryPicture**](#function-getgallerypicture) (const QString & prefix, const QString & pictureFilePath, QObject \* parent=nullptr) override<br> |
| virtual [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryVideo**](#function-getgalleryvideo) (const QString & prefix, const QString & videoFilePath, QObject \* parent=nullptr) override<br> |
| virtual bool | [**hasQfAction**](#function-hasqfaction) () override const<br> |
| virtual bool | [**hasQgsProject**](#function-hasqgsproject) () override const<br> |
| virtual void | [**importDatasets**](#function-importdatasets) () override const<br>_Requests and imports one or more datasets into QField's application directory action._  |
| virtual void | [**importProjectArchive**](#function-importprojectarchive) () override const<br>_Requests and imports a project archive into QField's application directory action._  |
| virtual void | [**importProjectFolder**](#function-importprojectfolder) () override const<br>_Requests and imports a project folder into QField's application directory action._  |
| virtual bool | [**isSystemDarkTheme**](#function-issystemdarktheme) () override const<br> |
| virtual void | [**loadQgsProject**](#function-loadqgsproject) () override const<br> |
| virtual [**QfViewStatus**](classQfViewStatus.md) \* | [**open**](#function-open) (const QString & uri, bool editing, QObject \* parent=nullptr) override<br> |
| virtual void | [**removeDataset**](#function-removedataset) (const QString & path) override const<br>_Removes a given dataset_ _path_ _._ |
| virtual void | [**removeFolder**](#function-removefolder) (const QString & path) override const<br>_Removes a given folder_ _path_ _._ |
| virtual void | [**requestBackgroundPositioningPermissions**](#function-requestbackgroundpositioningpermissions) () override<br> |
| virtual void | [**requestStoragePermission**](#function-requeststoragepermission) () override const<br> |
| virtual void | [**restoreBrightness**](#function-restorebrightness) () override<br> |
| virtual QStringList | [**rootDirectories**](#function-rootdirectories) () override const<br> |
| virtual void | [**sendCompressedFolderTo**](#function-sendcompressedfolderto) (const QString & path) override const<br>_Compresses a folder_ _path_ _and sends it via the platform native API._ |
| virtual void | [**sendDatasetTo**](#function-senddatasetto) (const QString & path) override const<br>_Sends a dataset_ _path_ _via the platform native API._ |
| virtual void | [**setHandleVolumeKeys**](#function-sethandlevolumekeys) (const bool handle) override<br> |
| virtual void | [**setScreenLockPermission**](#function-setscreenlockpermission) (const bool allowLock) override<br> |
| virtual QString | [**startPositioningService**](#function-startpositioningservice) () override const<br> |
| virtual void | [**stopPositioningService**](#function-stoppositioningservice) () override const<br> |
| virtual double | [**systemFontPointSize**](#function-systemfontpointsize) () override const<br> |
| virtual QString | [**systemSharedDataLocation**](#function-systemshareddatalocation) () override const<br> |
| virtual void | [**updateProjectFromArchive**](#function-updateprojectfromarchive) (const QString & projectPath) override const<br> |
| virtual void | [**uploadPendingAttachments**](#function-uploadpendingattachments) ([**QfCloudConnection**](classQfCloudConnection.md) \* connection) override const<br> |
| virtual void | [**vibrate**](#function-vibrate) (int milliseconds) override const<br> |


## Public Functions inherited from QfPlatformUtilities

See [QfPlatformUtilities](classQfPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| virtual QStringList | [**additionalApplicationDirectories**](classQfPlatformUtilities.md#function-additionalapplicationdirectories) () const<br> |
| virtual void | [**afterUpdate**](classQfPlatformUtilities.md#function-afterupdate) () <br> |
| virtual Q\_INVOKABLE QStringList | [**appDataDirs**](classQfPlatformUtilities.md#function-appdatadirs) () const<br> |
| virtual Q\_INVOKABLE QString | [**applicationDirectory**](classQfPlatformUtilities.md#function-applicationdirectory) () const<br> |
|  Q\_INVOKABLE QStringList | [**availableGrids**](classQfPlatformUtilities.md#function-availablegrids) () const<br> |
| virtual QfPlatformUtilities::Capabilities | [**capabilities**](classQfPlatformUtilities.md#function-capabilities-22) () const<br> |
| virtual Qt::PermissionStatus | [**checkCameraPermission**](classQfPlatformUtilities.md#function-checkcamerapermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkCameraPermissions**](classQfPlatformUtilities.md#function-checkcamerapermissions) () const<br> |
| virtual Qt::PermissionStatus | [**checkMicrophonePermission**](classQfPlatformUtilities.md#function-checkmicrophonepermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkMicrophonePermissions**](classQfPlatformUtilities.md#function-checkmicrophonepermissions) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkPositioningPermissions**](classQfPlatformUtilities.md#function-checkpositioningpermissions) () const<br> |
| virtual Q\_INVOKABLE void | [**copyTextToClipboard**](classQfPlatformUtilities.md#function-copytexttoclipboard) (const QString & string) const<br> |
|  Q\_INVOKABLE bool | [**createDir**](classQfPlatformUtilities.md#function-createdir) (const QString & path, const QString & dirname) const<br> |
| virtual Q\_INVOKABLE void | [**dimBrightness**](classQfPlatformUtilities.md#function-dimbrightness) () <br> |
| virtual void | [**executeQfAction**](classQfPlatformUtilities.md#function-executeqfaction) () const<br> |
| virtual Q\_INVOKABLE void | [**exportDatasetTo**](classQfPlatformUtilities.md#function-exportdatasetto) (const QString & path) const<br>_Exports a dataset_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE void | [**exportFolderTo**](classQfPlatformUtilities.md#function-exportfolderto) (const QString & path) const<br>_Exports a folder_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraPicture**](classQfPlatformUtilities.md#function-getcamerapicture) (const QString & prefix, const QString & pictureFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getCameraVideo**](classQfPlatformUtilities.md#function-getcameravideo) (const QString & prefix, const QString & videoFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getFile**](classQfPlatformUtilities.md#function-getfile) (const QString & prefix, const QString & filePath, const QString & mimeType=QStringLiteral("\*/\*"), QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryPicture**](classQfPlatformUtilities.md#function-getgallerypicture) (const QString & prefix, const QString & pictureFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfResourceSource**](classQfResourceSource.md) \* | [**getGalleryVideo**](classQfPlatformUtilities.md#function-getgalleryvideo) (const QString & prefix, const QString & videoFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE QString | [**getTextFromClipboard**](classQfPlatformUtilities.md#function-gettextfromclipboard) () const<br> |
| virtual bool | [**hasQfAction**](classQfPlatformUtilities.md#function-hasqfaction) () const<br> |
| virtual bool | [**hasQgsProject**](classQfPlatformUtilities.md#function-hasqgsproject) () const<br> |
| virtual Q\_INVOKABLE void | [**importDatasets**](classQfPlatformUtilities.md#function-importdatasets) () const<br>_Requests and imports one or more datasets into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectArchive**](classQfPlatformUtilities.md#function-importprojectarchive) () const<br>_Requests and imports a project archive into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectFolder**](classQfPlatformUtilities.md#function-importprojectfolder) () const<br>_Requests and imports a project folder into QField's application directory action._  |
|  void | [**initSystem**](classQfPlatformUtilities.md#function-initsystem) () <br> |
| virtual Q\_INVOKABLE bool | [**isSystemDarkTheme**](classQfPlatformUtilities.md#function-issystemdarktheme) () const<br> |
| virtual void | [**loadQgsProject**](classQfPlatformUtilities.md#function-loadqgsproject) () const<br> |
| virtual Q\_INVOKABLE [**QfViewStatus**](classQfViewStatus.md) \* | [**open**](classQfPlatformUtilities.md#function-open) (const QString & uri, bool editing=false, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**QfProjectSource**](classQfProjectSource.md) \* | [**openProject**](classQfPlatformUtilities.md#function-openproject) (QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE void | [**removeDataset**](classQfPlatformUtilities.md#function-removedataset) (const QString & path) const<br>_Removes a given dataset_ _path_ _._ |
| virtual Q\_INVOKABLE void | [**removeFolder**](classQfPlatformUtilities.md#function-removefolder) (const QString & path) const<br>_Removes a given folder_ _path_ _._ |
|  Q\_INVOKABLE bool | [**renameFile**](classQfPlatformUtilities.md#function-renamefile) (const QString & oldFilePath, const QString & newFilePath, bool overwrite=true) const<br> |
| virtual void | [**requestBackgroundPositioningPermissions**](classQfPlatformUtilities.md#function-requestbackgroundpositioningpermissions) () <br> |
| virtual void | [**requestCameraPermission**](classQfPlatformUtilities.md#function-requestcamerapermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual void | [**requestMicrophonePermission**](classQfPlatformUtilities.md#function-requestmicrophonepermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual Q\_INVOKABLE void | [**requestStoragePermission**](classQfPlatformUtilities.md#function-requeststoragepermission) () const<br> |
| virtual Q\_INVOKABLE void | [**restoreBrightness**](classQfPlatformUtilities.md#function-restorebrightness) () <br> |
|  Q\_INVOKABLE bool | [**rmFile**](classQfPlatformUtilities.md#function-rmfile) (const QString & filename) const<br> |
| virtual QStringList | [**rootDirectories**](classQfPlatformUtilities.md#function-rootdirectories) () const<br> |
| virtual Q\_INVOKABLE void | [**sendCompressedFilesTo**](classQfPlatformUtilities.md#function-sendcompressedfilesto) (const QStringList & paths) const<br>_Compresses a list of file_ _paths_ _and sends it via the platform native API._ |
| virtual Q\_INVOKABLE void | [**sendCompressedFolderTo**](classQfPlatformUtilities.md#function-sendcompressedfolderto) (const QString & path) const<br>_Compresses a folder_ _path_ _and sends it via the platform native API._ |
| virtual Q\_INVOKABLE void | [**sendDatasetTo**](classQfPlatformUtilities.md#function-senddatasetto) (const QString & path) const<br>_Sends a dataset_ _path_ _via the platform native API._ |
| virtual Q\_INVOKABLE void | [**setHandleVolumeKeys**](classQfPlatformUtilities.md#function-sethandlevolumekeys) (const bool handle) <br> |
| virtual Q\_INVOKABLE void | [**setScreenLockPermission**](classQfPlatformUtilities.md#function-setscreenlockpermission) (const bool allowLock) <br> |
| virtual QString | [**startPositioningService**](classQfPlatformUtilities.md#function-startpositioningservice) () const<br> |
| virtual void | [**stopPositioningService**](classQfPlatformUtilities.md#function-stoppositioningservice) () const<br> |
| virtual Q\_INVOKABLE double | [**systemFontPointSize**](classQfPlatformUtilities.md#function-systemfontpointsize) () const<br> |
| virtual Q\_INVOKABLE QString | [**systemLocalDataLocation**](classQfPlatformUtilities.md#function-systemlocaldatalocation) (const QString & subDir=QString()) const<br> |
| virtual QString | [**systemSharedDataLocation**](classQfPlatformUtilities.md#function-systemshareddatalocation) () const<br> |
| virtual Q\_INVOKABLE void | [**updateProjectFromArchive**](classQfPlatformUtilities.md#function-updateprojectfromarchive) (const QString & projectPath) const<br> |
| virtual Q\_INVOKABLE void | [**uploadPendingAttachments**](classQfPlatformUtilities.md#function-uploadpendingattachments) ([**QfCloudConnection**](classQfCloudConnection.md) \* connection) const<br> |
| virtual Q\_INVOKABLE void | [**vibrate**](classQfPlatformUtilities.md#function-vibrate) (int milliseconds) const<br> |
| virtual  | [**~QfPlatformUtilities**](classQfPlatformUtilities.md#function-qfplatformutilities) () <br> |




## Public Static Functions inherited from QfPlatformUtilities

See [QfPlatformUtilities](classQfPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
|  [**QfPlatformUtilities**](classQfPlatformUtilities.md) \* | [**instance**](classQfPlatformUtilities.md#function-instance) () <br> |


















































## Public Functions Documentation




### function QfAndroidPlatformUtilities 

```C++
QfAndroidPlatformUtilities::QfAndroidPlatformUtilities () 
```




<hr>



### function additionalApplicationDirectories 

```C++
virtual QStringList QfAndroidPlatformUtilities::additionalApplicationDirectories () override const
```



Secondary application directories which can be used by individual platforms. 


        
Implements [*QfPlatformUtilities::additionalApplicationDirectories*](classQfPlatformUtilities.md#function-additionalapplicationdirectories)


<hr>



### function afterUpdate 

```C++
virtual void QfAndroidPlatformUtilities::afterUpdate () override
```



This method can be used to implement platform specific initialization tasks to be performed after an app update. This is implemented for Android to extract app assets to location where it can be accessed via filesystem. 


        
Implements [*QfPlatformUtilities::afterUpdate*](classQfPlatformUtilities.md#function-afterupdate)


<hr>



### function appDataDirs 

```C++
virtual QStringList QfAndroidPlatformUtilities::appDataDirs () override const
```





**Returns:**

a list of data directories where user data is searched. User data are pg\_service.conf, authentication config, grids, ... 





        
Implements [*QfPlatformUtilities::appDataDirs*](classQfPlatformUtilities.md#function-appdatadirs)


<hr>



### function applicationDirectory 

```C++
virtual QString QfAndroidPlatformUtilities::applicationDirectory () override const
```



The main application directory within which projects and datasets can be imported. 


        
Implements [*QfPlatformUtilities::applicationDirectory*](classQfPlatformUtilities.md#function-applicationdirectory)


<hr>



### function capabilities 

```C++
virtual QfPlatformUtilities::Capabilities QfAndroidPlatformUtilities::capabilities () override const
```



Returns flags containing the supported capabilities of the platform. 


        
Implements [*QfPlatformUtilities::capabilities*](classQfPlatformUtilities.md#function-capabilities-22)


<hr>



### function checkCameraPermissions 

```C++
virtual bool QfAndroidPlatformUtilities::checkCameraPermissions () override const
```



Checks for camera permissions on the device. If the permissions are not given, the user will be asked to grant permissions. 

**Deprecated**

Since QField 3.1 




        
Implements [*QfPlatformUtilities::checkCameraPermissions*](classQfPlatformUtilities.md#function-checkcamerapermissions)


<hr>



### function checkMicrophonePermissions 

```C++
virtual bool QfAndroidPlatformUtilities::checkMicrophonePermissions () override const
```



Checks for camera microphone on the device. If the permissions are not given, the user will be asked to grant permissions. 

**Deprecated**

Since QField 3.1 




        
Implements [*QfPlatformUtilities::checkMicrophonePermissions*](classQfPlatformUtilities.md#function-checkmicrophonepermissions)


<hr>



### function checkPositioningPermissions 

```C++
virtual bool QfAndroidPlatformUtilities::checkPositioningPermissions () override const
```



Checks for positioning (GPS etc) permissions on the device. If the permissions are not given, the user will be asked to grant permissions. It will return true, if at least coarse permissions are granted. It will ask for fine permissions if none are granted. 

**Deprecated**

Since QField 3.1 




        
Implements [*QfPlatformUtilities::checkPositioningPermissions*](classQfPlatformUtilities.md#function-checkpositioningpermissions)


<hr>



### function dimBrightness 

```C++
virtual void QfAndroidPlatformUtilities::dimBrightness () override
```



Dims the brightness of the screen on supported devices. 


        
Implements [*QfPlatformUtilities::dimBrightness*](classQfPlatformUtilities.md#function-dimbrightness)


<hr>



### function executeQfAction 

```C++
virtual void QfAndroidPlatformUtilities::executeQfAction () override const
```



Loads the action provided during launch. 

**See also:** [**hasQfAction**](classQfAndroidPlatformUtilities.md#function-hasqfaction) 



        
Implements [*QfPlatformUtilities::executeQfAction*](classQfPlatformUtilities.md#function-executeqfaction)


<hr>



### function exportDatasetTo 

_Exports a dataset_ _path_ _to a user-specified location._
```C++
virtual void QfAndroidPlatformUtilities::exportDatasetTo (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::exportDatasetTo*](classQfPlatformUtilities.md#function-exportdatasetto)


<hr>



### function exportFolderTo 

_Exports a folder_ _path_ _to a user-specified location._
```C++
virtual void QfAndroidPlatformUtilities::exportFolderTo (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::exportFolderTo*](classQfPlatformUtilities.md#function-exportfolderto)


<hr>



### function getCameraPicture 

```C++
virtual QfResourceSource * QfAndroidPlatformUtilities::getCameraPicture (
    const QString & prefix,
    const QString & pictureFilePath,
    const QString & suffix,
    QObject * parent=nullptr
) override
```



Get a picture from camera and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `pictureFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the picture or null 





        
Implements [*QfPlatformUtilities::getCameraPicture*](classQfPlatformUtilities.md#function-getcamerapicture)


<hr>



### function getCameraVideo 

```C++
virtual QfResourceSource * QfAndroidPlatformUtilities::getCameraVideo (
    const QString & prefix,
    const QString & videoFilePath,
    const QString & suffix,
    QObject * parent=nullptr
) override
```



Get a video from camera and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `videoFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the video or null 





        
Implements [*QfPlatformUtilities::getCameraVideo*](classQfPlatformUtilities.md#function-getcameravideo)


<hr>



### function getFile 

```C++
virtual QfResourceSource * QfAndroidPlatformUtilities::getFile (
    const QString & prefix,
    const QString & filePath,
    const QString & mimeType,
    QObject * parent=nullptr
) override
```



Get a file from the operating system and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `filePath` The path (including subfolders and name) of the file 
* `filter` The filter string (e.g. \*.mp3) 



**Returns:**

The name of the file or null 





        
Implements [*QfPlatformUtilities::getFile*](classQfPlatformUtilities.md#function-getfile)


<hr>



### function getGalleryPicture 

```C++
virtual QfResourceSource * QfAndroidPlatformUtilities::getGalleryPicture (
    const QString & prefix,
    const QString & pictureFilePath,
    QObject * parent=nullptr
) override
```



Get a picture from gallery and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `pictureFilePath` The path (including subfolders and name) of the file 



**Returns:**

The name of the picture or null 





        
Implements [*QfPlatformUtilities::getGalleryPicture*](classQfPlatformUtilities.md#function-getgallerypicture)


<hr>



### function getGalleryVideo 

```C++
virtual QfResourceSource * QfAndroidPlatformUtilities::getGalleryVideo (
    const QString & prefix,
    const QString & videoFilePath,
    QObject * parent=nullptr
) override
```



Get a video from gallery and copy it to the requested prefix 

**Parameters:**


* `prefix` The project folder 
* `videoFilePath` The path (including subfolders and name) of the video 



**Returns:**

The name of the video or null 





        
Implements [*QfPlatformUtilities::getGalleryVideo*](classQfPlatformUtilities.md#function-getgalleryvideo)


<hr>



### function hasQfAction 

```C++
virtual bool QfAndroidPlatformUtilities::hasQfAction () override const
```



Returns the current action string 

**See also:** loadQfAction 



        
Implements [*QfPlatformUtilities::hasQfAction*](classQfPlatformUtilities.md#function-hasqfaction)


<hr>



### function hasQgsProject 

```C++
virtual bool QfAndroidPlatformUtilities::hasQgsProject () override const
```



Returns TRUE is a project file has been provided and should be opened at launch. 

**See also:** [**loadQgsProject**](classQfAndroidPlatformUtilities.md#function-loadqgsproject) 



        
Implements [*QfPlatformUtilities::hasQgsProject*](classQfPlatformUtilities.md#function-hasqgsproject)


<hr>



### function importDatasets 

_Requests and imports one or more datasets into QField's application directory action._ 
```C++
virtual void QfAndroidPlatformUtilities::importDatasets () override const
```



Implements [*QfPlatformUtilities::importDatasets*](classQfPlatformUtilities.md#function-importdatasets)


<hr>



### function importProjectArchive 

_Requests and imports a project archive into QField's application directory action._ 
```C++
virtual void QfAndroidPlatformUtilities::importProjectArchive () override const
```



Implements [*QfPlatformUtilities::importProjectArchive*](classQfPlatformUtilities.md#function-importprojectarchive)


<hr>



### function importProjectFolder 

_Requests and imports a project folder into QField's application directory action._ 
```C++
virtual void QfAndroidPlatformUtilities::importProjectFolder () override const
```



Implements [*QfPlatformUtilities::importProjectFolder*](classQfPlatformUtilities.md#function-importprojectfolder)


<hr>



### function isSystemDarkTheme 

```C++
virtual bool QfAndroidPlatformUtilities::isSystemDarkTheme () override const
```



Returns TRUE is the system uses a dark theme. 


        
Implements [*QfPlatformUtilities::isSystemDarkTheme*](classQfPlatformUtilities.md#function-issystemdarktheme)


<hr>



### function loadQgsProject 

```C++
virtual void QfAndroidPlatformUtilities::loadQgsProject () override const
```



Loads the project file provided during launch. 

**See also:** [**hasQgsProject**](classQfAndroidPlatformUtilities.md#function-hasqgsproject) 



        
Implements [*QfPlatformUtilities::loadQgsProject*](classQfPlatformUtilities.md#function-loadqgsproject)


<hr>



### function open 

```C++
virtual QfViewStatus * QfAndroidPlatformUtilities::open (
    const QString & uri,
    bool editing,
    QObject * parent=nullptr
) override
```



Open the resource (file, image, ...) that is available under _uri_. The mimetype is detected to indicate the system how the file should be opened. An optional _editing_ parameter can be set to true to indicate to supported systems the resource is expected to be edited. 


        
Implements [*QfPlatformUtilities::open*](classQfPlatformUtilities.md#function-open)


<hr>



### function removeDataset 

_Removes a given dataset_ _path_ _._
```C++
virtual void QfAndroidPlatformUtilities::removeDataset (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::removeDataset*](classQfPlatformUtilities.md#function-removedataset)


<hr>



### function removeFolder 

_Removes a given folder_ _path_ _._
```C++
virtual void QfAndroidPlatformUtilities::removeFolder (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::removeFolder*](classQfPlatformUtilities.md#function-removefolder)


<hr>



### function requestBackgroundPositioningPermissions 

```C++
virtual void QfAndroidPlatformUtilities::requestBackgroundPositioningPermissions () override
```



Implements [*QfPlatformUtilities::requestBackgroundPositioningPermissions*](classQfPlatformUtilities.md#function-requestbackgroundpositioningpermissions)


<hr>



### function requestStoragePermission 

```C++
virtual void QfAndroidPlatformUtilities::requestStoragePermission () override const
```



Implements [*QfPlatformUtilities::requestStoragePermission*](classQfPlatformUtilities.md#function-requeststoragepermission)


<hr>



### function restoreBrightness 

```C++
virtual void QfAndroidPlatformUtilities::restoreBrightness () override
```



Restores the brightness of the screen to its original value on supported devices. 


        
Implements [*QfPlatformUtilities::restoreBrightness*](classQfPlatformUtilities.md#function-restorebrightness)


<hr>



### function rootDirectories 

```C++
virtual QStringList QfAndroidPlatformUtilities::rootDirectories () override const
```



A list of root directories (e.g. root of an external storage) when system-wide access is available. 


        
Implements [*QfPlatformUtilities::rootDirectories*](classQfPlatformUtilities.md#function-rootdirectories)


<hr>



### function sendCompressedFolderTo 

_Compresses a folder_ _path_ _and sends it via the platform native API._
```C++
virtual void QfAndroidPlatformUtilities::sendCompressedFolderTo (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::sendCompressedFolderTo*](classQfPlatformUtilities.md#function-sendcompressedfolderto)


<hr>



### function sendDatasetTo 

_Sends a dataset_ _path_ _via the platform native API._
```C++
virtual void QfAndroidPlatformUtilities::sendDatasetTo (
    const QString & path
) override const
```



Implements [*QfPlatformUtilities::sendDatasetTo*](classQfPlatformUtilities.md#function-senddatasetto)


<hr>



### function setHandleVolumeKeys 

```C++
virtual void QfAndroidPlatformUtilities::setHandleVolumeKeys (
    const bool handle
) override
```



Sets whether the device volume keys are handled by QField. 


        
Implements [*QfPlatformUtilities::setHandleVolumeKeys*](classQfPlatformUtilities.md#function-sethandlevolumekeys)


<hr>



### function setScreenLockPermission 

```C++
virtual void QfAndroidPlatformUtilities::setScreenLockPermission (
    const bool allowLock
) override
```



Sets whether the device screen is allowed to go in lock mode. 

**Parameters:**


* `allowLock` if set to FALSE, the screen will not be allowed to lock. 




        
Implements [*QfPlatformUtilities::setScreenLockPermission*](classQfPlatformUtilities.md#function-setscreenlockpermission)


<hr>



### function startPositioningService 

```C++
virtual QString QfAndroidPlatformUtilities::startPositioningService () override const
```



Starts a positioning service on supported platforms and returns the remote object URL. 


        
Implements [*QfPlatformUtilities::startPositioningService*](classQfPlatformUtilities.md#function-startpositioningservice)


<hr>



### function stopPositioningService 

```C++
virtual void QfAndroidPlatformUtilities::stopPositioningService () override const
```



Stops a positioning service on supported platforms. 


        
Implements [*QfPlatformUtilities::stopPositioningService*](classQfPlatformUtilities.md#function-stoppositioningservice)


<hr>



### function systemFontPointSize 

```C++
inline virtual double QfAndroidPlatformUtilities::systemFontPointSize () override const
```



Returns the default system font size. 


        
Implements [*QfPlatformUtilities::systemFontPointSize*](classQfPlatformUtilities.md#function-systemfontpointsize)


<hr>



### function systemSharedDataLocation 

```C++
virtual QString QfAndroidPlatformUtilities::systemSharedDataLocation () override const
```



The path to share data location. Under this path, there should be the app specific directories qgis/ proj/ qfield/ ... Refers to /share or /usr/share on Linux. This path is assumed to be read only. 


        
Implements [*QfPlatformUtilities::systemSharedDataLocation*](classQfPlatformUtilities.md#function-systemshareddatalocation)


<hr>



### function updateProjectFromArchive 

```C++
virtual void QfAndroidPlatformUtilities::updateProjectFromArchive (
    const QString & projectPath
) override const
```



Update a local project content from a user-picked archive file action 

**Parameters:**


* `projectPath` the project file path 




        
Implements [*QfPlatformUtilities::updateProjectFromArchive*](classQfPlatformUtilities.md#function-updateprojectfromarchive)


<hr>



### function uploadPendingAttachments 

```C++
virtual void QfAndroidPlatformUtilities::uploadPendingAttachments (
    QfCloudConnection * connection
) override const
```



Uploads any pending attachments linked to the logged in user account. 


        
Implements [*QfPlatformUtilities::uploadPendingAttachments*](classQfPlatformUtilities.md#function-uploadpendingattachments)


<hr>



### function vibrate 

```C++
virtual void QfAndroidPlatformUtilities::vibrate (
    int milliseconds
) override const
```



Vibrates the device on supported platforms. 


        
Implements [*QfPlatformUtilities::vibrate*](classQfPlatformUtilities.md#function-vibrate)


<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/android/qfandroidplatformutilities.h`

