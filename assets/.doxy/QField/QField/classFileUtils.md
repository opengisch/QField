

# Class FileUtils



[**ClassList**](annotated.md) **>** [**FileUtils**](classFileUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FileUtils**](#function-fileutils) (QObject \* parent=nullptr) <br> |
|   | [**~FileUtils**](#function-fileutils) () = default<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE QString | [**absolutePath**](#function-absolutepath) (const QString & filePath) <br>_Returns the absolute path of the folder containing the_ _filePath_ _._ |
|  Q\_INVOKABLE void | [**addImageMetadata**](#function-addimagemetadata) (const QString & imagePath, const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation) <br> |
|  Q\_INVOKABLE void | [**addImageStamp**](#function-addimagestamp) (const QString & imagePath, const QString & text, const QString & textFormat=QString(), Qgis::TextHorizontalAlignment horizontalAlignment=Qgis::TextHorizontalAlignment::Left, const QString & imageDecoration=QString()) <br> |
|  bool | [**copyRecursively**](#function-copyrecursively) (const QString & sourceFolder, const QString & destFolder, QgsFeedback \* feedback=nullptr, bool wipeDestFolder=true) <br> |
|  Q\_INVOKABLE QVariantMap | [**deleteFiles**](#function-deletefiles) (const QStringList & filePaths) <br> |
|  Q\_INVOKABLE QByteArray | [**fileChecksum**](#function-filechecksum) (const QString & fileName, const QCryptographicHash::Algorithm hashAlgorithm) <br> |
|  Q\_INVOKABLE QString | [**fileEtag**](#function-fileetag) (const QString & fileName, int partSize=8 \*1024 \*1024) <br> |
|  Q\_INVOKABLE bool | [**fileExists**](#function-fileexists) (const QString & filePath) <br>_Returns true if the_ _filePath_ _exists (false if it's a directory)_ |
|  Q\_INVOKABLE QString | [**fileName**](#function-filename) (const QString & filePath) <br>_Returns the filename of a_ _filePath_ _- if no file name exists it's empty._ |
|  Q\_INVOKABLE QString | [**fileSuffix**](#function-filesuffix) (const QString & filePath) <br>_Returns the suffix (extension)_  |
|  Q\_INVOKABLE QVariantMap | [**getFileInfo**](#function-getfileinfo) (const QString & filePath, bool fetchContent=false) <br> |
|  Q\_INVOKABLE bool | [**isDeletable**](#function-isdeletable) (const QString & filePath) <br> |
|  Q\_INVOKABLE bool | [**isImageMimeTypeSupported**](#function-isimagemimetypesupported) (const QString & mimeType) <br>_Returns TRUE if the provided mimetype is a supported image._  |
|  Q\_INVOKABLE bool | [**isWithinProjectDirectory**](#function-iswithinprojectdirectory) (const QString & filePath) <br> |
|  Q\_INVOKABLE QString | [**mimeTypeName**](#function-mimetypename) (const QString & filePath) <br>_Returns the mimetype of a filepath as string._  |
|  Q\_INVOKABLE QByteArray | [**readFileContent**](#function-readfilecontent) (const QString & filePath) <br> |
|  Q\_INVOKABLE QString | [**representFileSize**](#function-representfilesize) (qint64 bytes) <br>_Returns a human-friendly size from bytes._  |
|  Q\_INVOKABLE void | [**restrictImageSize**](#function-restrictimagesize) (const QString & imagePath, int maximumWidthHeight) <br> |
|  Q\_INVOKABLE QString | [**sanitizeFilePath**](#function-sanitizefilepath) (const QString & filePath, const QString & replacement=QLatin1String("\_")) <br>_Tries to convert the given_ _filePath_ _with path to a cross OS valid file name with path._ |
|  Q\_INVOKABLE QString | [**sanitizeFilePathPart**](#function-sanitizefilepathpart) (const QString & filePathPart, const QString & replacement=QLatin1String("\_")) <br>_Tries to convert the given_ _filePathPart_ _to a cross OS valid file name part._ |
|  bool | [**unzip**](#function-unzip) (const QString & zipFilename, const QString & dir, QStringList & files, bool checkConsistency) <br> |
|  Q\_INVOKABLE bool | [**writeFileContent**](#function-writefilecontent) (const QString & filePath, const QByteArray & content) <br> |


























## Public Functions Documentation




### function FileUtils 

```C++
explicit FileUtils::FileUtils (
    QObject * parent=nullptr
) 
```




<hr>



### function ~FileUtils 

```C++
FileUtils::~FileUtils () = default
```




<hr>
## Public Static Functions Documentation




### function absolutePath 

_Returns the absolute path of the folder containing the_ _filePath_ _._
```C++
static Q_INVOKABLE QString FileUtils::absolutePath (
    const QString & filePath
) 
```




<hr>



### function addImageMetadata 

```C++
static Q_INVOKABLE void FileUtils::addImageMetadata (
    const QString & imagePath,
    const GnssPositionInformation & positionInformation
) 
```



Adds positioning metadata to a given image. 

**Parameters:**


* `imagePath` the image path 
* `positionInformation` the GNSS position information used to add metadata details 




        

<hr>



### function addImageStamp 

```C++
static Q_INVOKABLE void FileUtils::addImageStamp (
    const QString & imagePath,
    const QString & text,
    const QString & textFormat=QString(),
    Qgis::TextHorizontalAlignment horizontalAlignment=Qgis::TextHorizontalAlignment::Left,
    const QString & imageDecoration=QString()
) 
```



Prints details to a given image. 

**Parameters:**


* `imagePath` the image path 
* `text` the details text 
* `project` an optional project from which custom stamping settings will be retrieved 




        

<hr>



### function copyRecursively 

```C++
static bool FileUtils::copyRecursively (
    const QString & sourceFolder,
    const QString & destFolder,
    QgsFeedback * feedback=nullptr,
    bool wipeDestFolder=true
) 
```




<hr>



### function deleteFiles 

```C++
static Q_INVOKABLE QVariantMap FileUtils::deleteFiles (
    const QStringList & filePaths
) 
```



Deletes a list of files with security validation. Only deletes files that pass [**isDeletable()**](classFileUtils.md#function-isdeletable) checks and are within the project or cloud directories. 

**Parameters:**


* `filePaths` List of file paths to delete 



**Returns:**

A map with file paths as keys and boolean success status as values 





        

<hr>



### function fileChecksum 

```C++
static Q_INVOKABLE QByteArray FileUtils::fileChecksum (
    const QString & fileName,
    const QCryptographicHash::Algorithm hashAlgorithm
) 
```



Returns the checksum of a file. An empty QByteArray will be returned if it cannot be calculated. 

**Parameters:**


* `fileName` file name to get checksum of 
* `hashAlgorithm` hash algorithm (md5, sha1, sha256 etc) 



**Returns:**

QByteArray checksum value 





        

<hr>



### function fileEtag 

```C++
static Q_INVOKABLE QString FileUtils::fileEtag (
    const QString & fileName,
    int partSize=8 *1024 *1024
) 
```



Returns an Object Storage (S3) ETag of a file. An empty string will be returned if it cannot be calculated. 

**Parameters:**


* `fileName` file name to get checksum of 
* `partSize` maximum size used to divide the file content into parts 



**Returns:**

QString Etag value 





        

<hr>



### function fileExists 

_Returns true if the_ _filePath_ _exists (false if it's a directory)_
```C++
static Q_INVOKABLE bool FileUtils::fileExists (
    const QString & filePath
) 
```




<hr>



### function fileName 

_Returns the filename of a_ _filePath_ _- if no file name exists it's empty._
```C++
static Q_INVOKABLE QString FileUtils::fileName (
    const QString & filePath
) 
```




<hr>



### function fileSuffix 

_Returns the suffix (extension)_ 
```C++
static Q_INVOKABLE QString FileUtils::fileSuffix (
    const QString & filePath
) 
```




<hr>



### function getFileInfo 

```C++
static Q_INVOKABLE QVariantMap FileUtils::getFileInfo (
    const QString & filePath,
    bool fetchContent=false
) 
```



Gets detailed information about a file including MD5 hash and metadata. Optionally includes the file content when fetchContent is true. This is useful for file validation, caching, and efficient file handling in QML. 

**Parameters:**


* `filePath` The path to the file 
* `fetchContent` Whether to include the file content in the returned information (default: false) 



**Returns:**

A map containing file metadata and optionally its content 





        

<hr>



### function isDeletable 

```C++
static Q_INVOKABLE bool FileUtils::isDeletable (
    const QString & filePath
) 
```



Checks if a file can be safely deleted. Security checks:
* File must be within application or cloud directories
* Only allows deletion of specific file types
* Prevents deletion of folders, project files 

**Parameters:**


  * `filePath` The path to check for deletion eligibility 



**Returns:**

True if the file can be safely deleted, false otherwise 







        

<hr>



### function isImageMimeTypeSupported 

_Returns TRUE if the provided mimetype is a supported image._ 
```C++
static Q_INVOKABLE bool FileUtils::isImageMimeTypeSupported (
    const QString & mimeType
) 
```




<hr>



### function isWithinProjectDirectory 

```C++
static Q_INVOKABLE bool FileUtils::isWithinProjectDirectory (
    const QString & filePath
) 
```



Checks if a file path is securely within the current project directory. Security measures:
* Prevents directory traversal attacks using path normalization
* Handles symbolic links that could escape project boundaries
* Supports cross-platform path comparisons (Windows, macOS, Linux, Android, iOS)
* Validates non-existent files safely for write operations
* Prevents partial directory matching by enforcing complete path containment






**Parameters:**


* `filePath` The path to check 



**Returns:**

True if the path is safely within the current project directory 





        

<hr>



### function mimeTypeName 

_Returns the mimetype of a filepath as string._ 
```C++
static Q_INVOKABLE QString FileUtils::mimeTypeName (
    const QString & filePath
) 
```




<hr>



### function readFileContent 

```C++
static Q_INVOKABLE QByteArray FileUtils::readFileContent (
    const QString & filePath
) 
```



Reads the entire content of a file and returns it as a byte array. 

**Parameters:**


* `filePath` The path to the file to be read 



**Returns:**

The file content as a QByteArray 





        

<hr>



### function representFileSize 

_Returns a human-friendly size from bytes._ 
```C++
static Q_INVOKABLE QString FileUtils::representFileSize (
    qint64 bytes
) 
```




<hr>



### function restrictImageSize 

```C++
static Q_INVOKABLE void FileUtils::restrictImageSize (
    const QString & imagePath,
    int maximumWidthHeight
) 
```



Insures that a given image's width and height are restricted to a maximum size. 

**Parameters:**


* `imagePath` the image file path 
* `maximumWidthHeight` the maximum width and height size 




        

<hr>



### function sanitizeFilePath 

_Tries to convert the given_ _filePath_ _with path to a cross OS valid file name with path._
```C++
static Q_INVOKABLE QString FileUtils::sanitizeFilePath (
    const QString & filePath,
    const QString & replacement=QLatin1String("_")
) 
```




<hr>



### function sanitizeFilePathPart 

_Tries to convert the given_ _filePathPart_ _to a cross OS valid file name part._
```C++
static Q_INVOKABLE QString FileUtils::sanitizeFilePathPart (
    const QString & filePathPart,
    const QString & replacement=QLatin1String("_")
) 
```




<hr>



### function unzip 

```C++
static bool FileUtils::unzip (
    const QString & zipFilename,
    const QString & dir,
    QStringList & files,
    bool checkConsistency
) 
```



Unzip a zip file in an output directory. 

**Parameters:**


* `zip` The zip filename 
* `dir` The output directory 
* `files` The absolute path of unzipped files 
* `checkConsistency` Perform additional stricter consistency checks on the archive, and error if they fail (since QGIS 3.30) 



**Returns:**

FALSE if the zip filename does not exist, the output directory does not exist or is not writable. 





        

<hr>



### function writeFileContent 

```C++
static Q_INVOKABLE bool FileUtils::writeFileContent (
    const QString & filePath,
    const QByteArray & content
) 
```



Writes content to a file. 

**Parameters:**


* `filePath` The path to the file to be written 
* `content` The content to write to the file 



**Returns:**

True if the write operation was successful, false otherwise




**Note:**

Platform restrictions apply:
* On Android: Writing is only permitted within the app's internal storage or properly requested scoped storage locations.
* On iOS: Writing is restricted to the app's sandbox.
* Use [**PlatformUtilities.applicationDirectory()**](classPlatformUtilities.md#function-applicationdirectory) to get a safe write location. 







        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/fileutils.h`

