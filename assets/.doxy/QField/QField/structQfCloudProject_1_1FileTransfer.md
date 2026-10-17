

# Struct QfCloudProject::FileTransfer



[**ClassList**](annotated.md) **>** [**QfCloudProject**](classQfCloudProject.md) **>** [**FileTransfer**](structQfCloudProject_1_1FileTransfer.md)


























## Public Attributes

| Type | Name |
| ---: | :--- |
|  qint64 | [**bytesTotal**](#variable-bytestotal)   = `0`<br> |
|  qint64 | [**bytesTransferred**](#variable-bytestransferred)   = `0`<br> |
|  QNetworkReply::NetworkError | [**error**](#variable-error)   = `QNetworkReply::NoError`<br> |
|  QString | [**etag**](#variable-etag)  <br> |
|  QString | [**fileName**](#variable-filename)  <br> |
|  bool | [**isFinished**](#variable-isfinished)   = `false`<br> |
|  QUrl | [**lastRedirectUrl**](#variable-lastredirecturl)  <br> |
|  QPointer&lt; [**QfNetworkReply**](classQfNetworkReply.md) &gt; | [**networkReply**](#variable-networkreply)  <br> |
|  QString | [**partialFilePath**](#variable-partialfilepath)  <br> |
|  QString | [**projectId**](#variable-projectid)  <br> |
|  int | [**redirectsCount**](#variable-redirectscount)   = `0`<br> |
|  bool | [**resumableDownload**](#variable-resumabledownload)   = `true`<br> |
|  int | [**retryCount**](#variable-retrycount)   = `0`<br> |
|  QString | [**tmpFile**](#variable-tmpfile)  <br> |
















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FileTransfer**](#function-filetransfer-12) (const QString & fileName, const qint64 bytesTotal, const QString & projectId, const QString & etag) <br> |
|   | [**FileTransfer**](#function-filetransfer-22) () = default<br> |




























## Public Attributes Documentation




### variable bytesTotal 

```C++
qint64 QfCloudProject::FileTransfer::bytesTotal;
```




<hr>



### variable bytesTransferred 

```C++
qint64 QfCloudProject::FileTransfer::bytesTransferred;
```




<hr>



### variable error 

```C++
QNetworkReply::NetworkError QfCloudProject::FileTransfer::error;
```




<hr>



### variable etag 

```C++
QString QfCloudProject::FileTransfer::etag;
```




<hr>



### variable fileName 

```C++
QString QfCloudProject::FileTransfer::fileName;
```




<hr>



### variable isFinished 

```C++
bool QfCloudProject::FileTransfer::isFinished;
```




<hr>



### variable lastRedirectUrl 

```C++
QUrl QfCloudProject::FileTransfer::lastRedirectUrl;
```




<hr>



### variable networkReply 

```C++
QPointer<QfNetworkReply> QfCloudProject::FileTransfer::networkReply;
```




<hr>



### variable partialFilePath 

```C++
QString QfCloudProject::FileTransfer::partialFilePath;
```




<hr>



### variable projectId 

```C++
QString QfCloudProject::FileTransfer::projectId;
```




<hr>



### variable redirectsCount 

```C++
int QfCloudProject::FileTransfer::redirectsCount;
```




<hr>



### variable resumableDownload 

```C++
bool QfCloudProject::FileTransfer::resumableDownload;
```




<hr>



### variable retryCount 

```C++
int QfCloudProject::FileTransfer::retryCount;
```




<hr>



### variable tmpFile 

```C++
QString QfCloudProject::FileTransfer::tmpFile;
```




<hr>
## Public Functions Documentation




### function FileTransfer [1/2]

```C++
inline QfCloudProject::FileTransfer::FileTransfer (
    const QString & fileName,
    const qint64 bytesTotal,
    const QString & projectId,
    const QString & etag
) 
```




<hr>



### function FileTransfer [2/2]

```C++
QfCloudProject::FileTransfer::FileTransfer () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudproject.h`

