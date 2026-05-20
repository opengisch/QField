

# Struct QFieldCloudProject::FileTransfer



[**ClassList**](annotated.md) **>** [**QFieldCloudProject**](classQFieldCloudProject.md) **>** [**FileTransfer**](structQFieldCloudProject_1_1FileTransfer.md)


























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
|  QPointer&lt; [**NetworkReply**](classNetworkReply.md) &gt; | [**networkReply**](#variable-networkreply)  <br> |
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
qint64 QFieldCloudProject::FileTransfer::bytesTotal;
```




<hr>



### variable bytesTransferred 

```C++
qint64 QFieldCloudProject::FileTransfer::bytesTransferred;
```




<hr>



### variable error 

```C++
QNetworkReply::NetworkError QFieldCloudProject::FileTransfer::error;
```




<hr>



### variable etag 

```C++
QString QFieldCloudProject::FileTransfer::etag;
```




<hr>



### variable fileName 

```C++
QString QFieldCloudProject::FileTransfer::fileName;
```




<hr>



### variable isFinished 

```C++
bool QFieldCloudProject::FileTransfer::isFinished;
```




<hr>



### variable lastRedirectUrl 

```C++
QUrl QFieldCloudProject::FileTransfer::lastRedirectUrl;
```




<hr>



### variable networkReply 

```C++
QPointer<NetworkReply> QFieldCloudProject::FileTransfer::networkReply;
```




<hr>



### variable partialFilePath 

```C++
QString QFieldCloudProject::FileTransfer::partialFilePath;
```




<hr>



### variable projectId 

```C++
QString QFieldCloudProject::FileTransfer::projectId;
```




<hr>



### variable redirectsCount 

```C++
int QFieldCloudProject::FileTransfer::redirectsCount;
```




<hr>



### variable resumableDownload 

```C++
bool QFieldCloudProject::FileTransfer::resumableDownload;
```




<hr>



### variable retryCount 

```C++
int QFieldCloudProject::FileTransfer::retryCount;
```




<hr>



### variable tmpFile 

```C++
QString QFieldCloudProject::FileTransfer::tmpFile;
```




<hr>
## Public Functions Documentation




### function FileTransfer [1/2]

```C++
inline QFieldCloudProject::FileTransfer::FileTransfer (
    const QString & fileName,
    const qint64 bytesTotal,
    const QString & projectId,
    const QString & etag
) 
```




<hr>



### function FileTransfer [2/2]

```C++
QFieldCloudProject::FileTransfer::FileTransfer () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfieldcloudproject.h`

