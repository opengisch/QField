

# Class IosPlatformUtilities



[**ClassList**](annotated.md) **>** [**IosPlatformUtilities**](classIosPlatformUtilities.md)








Inherits the following classes: [PlatformUtilities](classPlatformUtilities.md)
















## Public Types inherited from PlatformUtilities

See [PlatformUtilities](classPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| enum  | [**Capability**](classPlatformUtilities.md#enum-capability)  <br> |
| enum  | [**FileType**](classPlatformUtilities.md#enum-filetype)  <br> |
























## Public Properties inherited from PlatformUtilities

See [PlatformUtilities](classPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| property PlatformUtilities::Capabilities | [**capabilities**](classPlatformUtilities.md#property-capabilities-12)  <br> |








## Public Signals inherited from PlatformUtilities

See [PlatformUtilities](classPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| signal void | [**resourceCanceled**](classPlatformUtilities.md#signal-resourcecanceled) (const QString & message) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceOpened**](classPlatformUtilities.md#signal-resourceopened) (const QString & path) <br>_Emitted when a resource has been opened._  |
| signal void | [**resourceReceived**](classPlatformUtilities.md#signal-resourcereceived) (const QString & path) <br>_Emitted when a resource has been received._  |






## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**IosPlatformUtilities**](#function-iosplatformutilities) () <br> |
| virtual void | [**afterUpdate**](#function-afterupdate) () override<br> |
| virtual QStringList | [**appDataDirs**](#function-appdatadirs) () override const<br> |
| virtual QString | [**applicationDirectory**](#function-applicationdirectory) () override const<br> |
| virtual PlatformUtilities::Capabilities | [**capabilities**](#function-capabilities) () override const<br> |
| virtual Qt::PermissionStatus | [**checkCameraPermission**](#function-checkcamerapermission) () override const<br> |
| virtual Qt::PermissionStatus | [**checkMicrophonePermission**](#function-checkmicrophonepermission) () override const<br> |
| virtual [**ResourceSource**](classResourceSource.md) \* | [**getCameraPicture**](#function-getcamerapicture) (const QString & prefix, const QString & pictureFilePath, const QString & suffix, QObject \* parent=nullptr) override<br> |
| virtual [**ResourceSource**](classResourceSource.md) \* | [**getCameraVideo**](#function-getcameravideo) (const QString & prefix, const QString & videoFilePath, const QString & suffix, QObject \* parent=nullptr) override<br> |
| virtual [**ResourceSource**](classResourceSource.md) \* | [**getGalleryPicture**](#function-getgallerypicture) (const QString & prefix, const QString & pictureFilePath, QObject \* parent=nullptr) override<br> |
| virtual [**ResourceSource**](classResourceSource.md) \* | [**getGalleryVideo**](#function-getgalleryvideo) (const QString & prefix, const QString & videoFilePath, QObject \* parent=nullptr) override<br> |
| virtual bool | [**isSystemDarkTheme**](#function-issystemdarktheme) () override const<br> |
| virtual [**ViewStatus**](classViewStatus.md) \* | [**open**](#function-open) (const QString & uri, bool editing=false, QObject \* parent=nullptr) override<br> |
| virtual [**ProjectSource**](classProjectSource.md) \* | [**openProject**](#function-openproject) (QObject \* parent=nullptr) override<br> |
| virtual void | [**requestCameraPermission**](#function-requestcamerapermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) override<br> |
| virtual void | [**requestMicrophonePermission**](#function-requestmicrophonepermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) override<br> |
| virtual QStringList | [**rootDirectories**](#function-rootdirectories) () override const<br> |
| virtual void | [**setScreenLockPermission**](#function-setscreenlockpermission) (const bool allowLock) override<br> |
| virtual double | [**systemFontPointSize**](#function-systemfontpointsize) () override const<br> |
| virtual QString | [**systemSharedDataLocation**](#function-systemshareddatalocation) () override const<br> |


## Public Functions inherited from PlatformUtilities

See [PlatformUtilities](classPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
| virtual QStringList | [**additionalApplicationDirectories**](classPlatformUtilities.md#function-additionalapplicationdirectories) () const<br> |
| virtual Q\_INVOKABLE QStringList | [**appDataDirs**](classPlatformUtilities.md#function-appdatadirs) () const<br> |
| virtual Q\_INVOKABLE QString | [**applicationDirectory**](classPlatformUtilities.md#function-applicationdirectory) () const<br> |
|  Q\_INVOKABLE QStringList | [**availableGrids**](classPlatformUtilities.md#function-availablegrids) () const<br> |
| virtual PlatformUtilities::Capabilities | [**capabilities**](classPlatformUtilities.md#function-capabilities-22) () const<br> |
| virtual Qt::PermissionStatus | [**checkCameraPermission**](classPlatformUtilities.md#function-checkcamerapermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkCameraPermissions**](classPlatformUtilities.md#function-checkcamerapermissions) () const<br> |
| virtual Qt::PermissionStatus | [**checkMicrophonePermission**](classPlatformUtilities.md#function-checkmicrophonepermission) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkMicrophonePermissions**](classPlatformUtilities.md#function-checkmicrophonepermissions) () const<br> |
| virtual Q\_DECL\_DEPRECATED virtual Q\_INVOKABLE bool | [**checkPositioningPermissions**](classPlatformUtilities.md#function-checkpositioningpermissions) () const<br> |
| virtual Q\_INVOKABLE void | [**copyTextToClipboard**](classPlatformUtilities.md#function-copytexttoclipboard) (const QString & string) const<br> |
|  Q\_INVOKABLE bool | [**createDir**](classPlatformUtilities.md#function-createdir) (const QString & path, const QString & dirname) const<br> |
| virtual Q\_INVOKABLE void | [**dimBrightness**](classPlatformUtilities.md#function-dimbrightness) () <br> |
| virtual void | [**executeQfAction**](classPlatformUtilities.md#function-executeqfaction) () const<br> |
| virtual Q\_INVOKABLE void | [**exportDatasetTo**](classPlatformUtilities.md#function-exportdatasetto) (const QString & path) const<br>_Exports a dataset_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE void | [**exportFolderTo**](classPlatformUtilities.md#function-exportfolderto) (const QString & path) const<br>_Exports a folder_ _path_ _to a user-specified location._ |
| virtual Q\_INVOKABLE [**ResourceSource**](classResourceSource.md) \* | [**getCameraPicture**](classPlatformUtilities.md#function-getcamerapicture) (const QString & prefix, const QString & pictureFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**ResourceSource**](classResourceSource.md) \* | [**getCameraVideo**](classPlatformUtilities.md#function-getcameravideo) (const QString & prefix, const QString & videoFilePath, const QString & suffix, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**ResourceSource**](classResourceSource.md) \* | [**getFile**](classPlatformUtilities.md#function-getfile) (const QString & prefix, const QString & filePath, [**FileType**](classPlatformUtilities.md#enum-filetype) fileType=AllFiles, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**ResourceSource**](classResourceSource.md) \* | [**getGalleryPicture**](classPlatformUtilities.md#function-getgallerypicture) (const QString & prefix, const QString & pictureFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**ResourceSource**](classResourceSource.md) \* | [**getGalleryVideo**](classPlatformUtilities.md#function-getgalleryvideo) (const QString & prefix, const QString & videoFilePath, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE QString | [**getTextFromClipboard**](classPlatformUtilities.md#function-gettextfromclipboard) () const<br> |
| virtual bool | [**hasQfAction**](classPlatformUtilities.md#function-hasqfaction) () const<br> |
| virtual bool | [**hasQgsProject**](classPlatformUtilities.md#function-hasqgsproject) () const<br> |
| virtual Q\_INVOKABLE void | [**importDatasets**](classPlatformUtilities.md#function-importdatasets) () const<br>_Requests and imports one or more datasets into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectArchive**](classPlatformUtilities.md#function-importprojectarchive) () const<br>_Requests and imports a project archive into QField's application directory action._  |
| virtual Q\_INVOKABLE void | [**importProjectFolder**](classPlatformUtilities.md#function-importprojectfolder) () const<br>_Requests and imports a project folder into QField's application directory action._  |
|  void | [**initSystem**](classPlatformUtilities.md#function-initsystem) () <br> |
| virtual Q\_INVOKABLE bool | [**isSystemDarkTheme**](classPlatformUtilities.md#function-issystemdarktheme) () const<br> |
| virtual void | [**loadQgsProject**](classPlatformUtilities.md#function-loadqgsproject) () const<br> |
| virtual Q\_INVOKABLE [**ViewStatus**](classViewStatus.md) \* | [**open**](classPlatformUtilities.md#function-open) (const QString & uri, bool editing=false, QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE [**ProjectSource**](classProjectSource.md) \* | [**openProject**](classPlatformUtilities.md#function-openproject) (QObject \* parent=nullptr) <br> |
| virtual Q\_INVOKABLE void | [**removeDataset**](classPlatformUtilities.md#function-removedataset) (const QString & path) const<br>_Removes a given dataset_ _path_ _._ |
| virtual Q\_INVOKABLE void | [**removeFolder**](classPlatformUtilities.md#function-removefolder) (const QString & path) const<br>_Removes a given folder_ _path_ _._ |
|  Q\_INVOKABLE bool | [**renameFile**](classPlatformUtilities.md#function-renamefile) (const QString & oldFilePath, const QString & newFilePath, bool overwrite=true) const<br> |
| virtual void | [**requestBackgroundPositioningPermissions**](classPlatformUtilities.md#function-requestbackgroundpositioningpermissions) () <br> |
| virtual void | [**requestCameraPermission**](classPlatformUtilities.md#function-requestcamerapermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual void | [**requestMicrophonePermission**](classPlatformUtilities.md#function-requestmicrophonepermission) (std::function&lt; void(Qt::PermissionStatus)&gt; func) <br> |
| virtual Q\_INVOKABLE void | [**requestStoragePermission**](classPlatformUtilities.md#function-requeststoragepermission) () const<br> |
| virtual Q\_INVOKABLE void | [**restoreBrightness**](classPlatformUtilities.md#function-restorebrightness) () <br> |
|  Q\_INVOKABLE bool | [**rmFile**](classPlatformUtilities.md#function-rmfile) (const QString & filename) const<br> |
| virtual QStringList | [**rootDirectories**](classPlatformUtilities.md#function-rootdirectories) () const<br> |
| virtual Q\_INVOKABLE void | [**sendCompressedFolderTo**](classPlatformUtilities.md#function-sendcompressedfolderto) (const QString & path) const<br>_Compresses a folder_ _path_ _and sends it via the platform native API._ |
| virtual Q\_INVOKABLE void | [**sendDatasetTo**](classPlatformUtilities.md#function-senddatasetto) (const QString & path) const<br>_Sends a dataset_ _path_ _via the platform native API._ |
| virtual Q\_INVOKABLE void | [**setHandleVolumeKeys**](classPlatformUtilities.md#function-sethandlevolumekeys) (const bool handle) <br> |
| virtual Q\_INVOKABLE void | [**setScreenLockPermission**](classPlatformUtilities.md#function-setscreenlockpermission) (const bool allowLock) <br> |
| virtual QString | [**startPositioningService**](classPlatformUtilities.md#function-startpositioningservice) () const<br> |
| virtual void | [**stopPositioningService**](classPlatformUtilities.md#function-stoppositioningservice) () const<br> |
| virtual Q\_INVOKABLE double | [**systemFontPointSize**](classPlatformUtilities.md#function-systemfontpointsize) () const<br> |
| virtual QString | [**systemLocalDataLocation**](classPlatformUtilities.md#function-systemlocaldatalocation) (const QString & subDir=QString()) const<br> |
| virtual QString | [**systemSharedDataLocation**](classPlatformUtilities.md#function-systemshareddatalocation) () const<br> |
| virtual Q\_INVOKABLE void | [**updateProjectFromArchive**](classPlatformUtilities.md#function-updateprojectfromarchive) (const QString & projectPath) const<br> |
| virtual Q\_INVOKABLE void | [**uploadPendingAttachments**](classPlatformUtilities.md#function-uploadpendingattachments) ([**QFieldCloudConnection**](classQFieldCloudConnection.md) \* connection) const<br> |
| virtual Q\_INVOKABLE void | [**vibrate**](classPlatformUtilities.md#function-vibrate) (int milliseconds) const<br> |
| virtual  | [**~PlatformUtilities**](classPlatformUtilities.md#function-platformutilities) () <br> |




## Public Static Functions inherited from PlatformUtilities

See [PlatformUtilities](classPlatformUtilities.md)

| Type | Name |
| ---: | :--- |
|  [**PlatformUtilities**](classPlatformUtilities.md) \* | [**instance**](classPlatformUtilities.md#function-instance) () <br> |


















































## Public Functions Documentation




### function IosPlatformUtilities 

```C++
IosPlatformUtilities::IosPlatformUtilities () 
```




<hr>



### function afterUpdate 

```C++
virtual void IosPlatformUtilities::afterUpdate () override
```



This method can be used to implement platform specific initialization tasks to be performed after an app update. This is implemented for Android to extract app assets to location where it can be accessed via filesystem. 


        
Implements [*PlatformUtilities::afterUpdate*](classPlatformUtilities.md#function-afterupdate)


<hr>



### function appDataDirs 

```C++
virtual QStringList IosPlatformUtilities::appDataDirs () override const
```





**Returns:**

a list of data directories where user data is searched. User data are pg\_service.conf, authentication config, grids, ... 





        
Implements [*PlatformUtilities::appDataDirs*](classPlatformUtilities.md#function-appdatadirs)


<hr>



### function applicationDirectory 

```C++
virtual QString IosPlatformUtilities::applicationDirectory () override const
```



The main application directory within which projects and datasets can be imported. 


        
Implements [*PlatformUtilities::applicationDirectory*](classPlatformUtilities.md#function-applicationdirectory)


<hr>



### function capabilities 

```C++
virtual PlatformUtilities::Capabilities IosPlatformUtilities::capabilities () override const
```



Returns flags containing the supported capabilities of the platform. 


        
Implements [*PlatformUtilities::capabilities*](classPlatformUtilities.md#function-capabilities-22)


<hr>



### function checkCameraPermission 

```C++
virtual Qt::PermissionStatus IosPlatformUtilities::checkCameraPermission () override const
```



Implements [*PlatformUtilities::checkCameraPermission*](classPlatformUtilities.md#function-checkcamerapermission)


<hr>



### function checkMicrophonePermission 

```C++
virtual Qt::PermissionStatus IosPlatformUtilities::checkMicrophonePermission () override const
```



Implements [*PlatformUtilities::checkMicrophonePermission*](classPlatformUtilities.md#function-checkmicrophonepermission)


<hr>



### function getCameraPicture 

```C++
virtual ResourceSource * IosPlatformUtilities::getCameraPicture (
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





        
Implements [*PlatformUtilities::getCameraPicture*](classPlatformUtilities.md#function-getcamerapicture)


<hr>



### function getCameraVideo 

```C++
virtual ResourceSource * IosPlatformUtilities::getCameraVideo (
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





        
Implements [*PlatformUtilities::getCameraVideo*](classPlatformUtilities.md#function-getcameravideo)


<hr>



### function getGalleryPicture 

```C++
virtual ResourceSource * IosPlatformUtilities::getGalleryPicture (
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





        
Implements [*PlatformUtilities::getGalleryPicture*](classPlatformUtilities.md#function-getgallerypicture)


<hr>



### function getGalleryVideo 

```C++
virtual ResourceSource * IosPlatformUtilities::getGalleryVideo (
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





        
Implements [*PlatformUtilities::getGalleryVideo*](classPlatformUtilities.md#function-getgalleryvideo)


<hr>



### function isSystemDarkTheme 

```C++
virtual bool IosPlatformUtilities::isSystemDarkTheme () override const
```



Returns TRUE is the system uses a dark theme. 


        
Implements [*PlatformUtilities::isSystemDarkTheme*](classPlatformUtilities.md#function-issystemdarktheme)


<hr>



### function open 

```C++
virtual ViewStatus * IosPlatformUtilities::open (
    const QString & uri,
    bool editing=false,
    QObject * parent=nullptr
) override
```



Open the resource (file, image, ...) that is available under _uri_. The mimetype is detected to indicate the system how the file should be opened. An optional _editing_ parameter can be set to true to indicate to supported systems the resource is expected to be edited. 


        
Implements [*PlatformUtilities::open*](classPlatformUtilities.md#function-open)


<hr>



### function openProject 

```C++
virtual ProjectSource * IosPlatformUtilities::openProject (
    QObject * parent=nullptr
) override
```



Indicates the system that we want to open a project. The system shall show a suitable user interface element (like a filebrowser) to let the user select a project. The call returns immediately and the returned [**ProjectSource**](classProjectSource.md) will notify when the project has actually been chosen. 


        
Implements [*PlatformUtilities::openProject*](classPlatformUtilities.md#function-openproject)


<hr>



### function requestCameraPermission 

```C++
virtual void IosPlatformUtilities::requestCameraPermission (
    std::function< void(Qt::PermissionStatus)> func
) override
```



Implements [*PlatformUtilities::requestCameraPermission*](classPlatformUtilities.md#function-requestcamerapermission)


<hr>



### function requestMicrophonePermission 

```C++
virtual void IosPlatformUtilities::requestMicrophonePermission (
    std::function< void(Qt::PermissionStatus)> func
) override
```



Implements [*PlatformUtilities::requestMicrophonePermission*](classPlatformUtilities.md#function-requestmicrophonepermission)


<hr>



### function rootDirectories 

```C++
inline virtual QStringList IosPlatformUtilities::rootDirectories () override const
```



A list of root directories (e.g. root of an external storage) when system-wide access is available. 


        
Implements [*PlatformUtilities::rootDirectories*](classPlatformUtilities.md#function-rootdirectories)


<hr>



### function setScreenLockPermission 

```C++
virtual void IosPlatformUtilities::setScreenLockPermission (
    const bool allowLock
) override
```



Sets whether the device screen is allowed to go in lock mode. 

**Parameters:**


* `allowLock` if set to FALSE, the screen will not be allowed to lock. 




        
Implements [*PlatformUtilities::setScreenLockPermission*](classPlatformUtilities.md#function-setscreenlockpermission)


<hr>



### function systemFontPointSize 

```C++
inline virtual double IosPlatformUtilities::systemFontPointSize () override const
```



Returns the default system font size. 


        
Implements [*PlatformUtilities::systemFontPointSize*](classPlatformUtilities.md#function-systemfontpointsize)


<hr>



### function systemSharedDataLocation 

```C++
virtual QString IosPlatformUtilities::systemSharedDataLocation () override const
```



The path to share data location. Under this path, there should be the app specific directories qgis/ proj/ qfield/ ... Refers to /share or /usr/share on Linux. This path is assumed to be read only. 


        
Implements [*PlatformUtilities::systemSharedDataLocation*](classPlatformUtilities.md#function-systemshareddatalocation)


<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/platforms/ios/iosplatformutilities.h`

