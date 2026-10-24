

# Class QfNetworkReply



[**ClassList**](annotated.md) **>** [**QfNetworkReply**](classQfNetworkReply.md)



[More...](#detailed-description)

* `#include <qfnetworkreply.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentRawReplyChanged**](classQfNetworkReply.md#signal-currentrawreplychanged)  <br> |
| signal void | [**downloadProgress**](classQfNetworkReply.md#signal-downloadprogress) (qint64 bytesReceived, qint64 bytesTotal) <br> |
| signal void | [**encrypted**](classQfNetworkReply.md#signal-encrypted)  <br> |
| signal void | [**errorOccurred**](classQfNetworkReply.md#signal-erroroccurred) (QNetworkReply::NetworkError code) <br> |
| signal void | [**finished**](classQfNetworkReply.md#signal-finished)  <br> |
| signal void | [**redirectAllowed**](classQfNetworkReply.md#signal-redirectallowed)  <br> |
| signal void | [**redirected**](classQfNetworkReply.md#signal-redirected) (const QUrl & url) <br> |
| signal void | [**retry**](classQfNetworkReply.md#signal-retry)  <br> |
| signal void | [**temporaryErrorOccurred**](classQfNetworkReply.md#signal-temporaryerroroccurred) (QNetworkReply::NetworkError code) <br> |
| signal void | [**uploadProgress**](classQfNetworkReply.md#signal-uploadprogress) (qint64 bytesSent, qint64 bytesTotal) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNetworkReply**](#function-qfnetworkreply-12) (const QNetworkAccessManager::Operation operation, const QNetworkRequest & request, const QByteArray & payloadByteArray) <br> |
|   | [**QfNetworkReply**](#function-qfnetworkreply-22) (const QNetworkAccessManager::Operation operation, const QNetworkRequest & request, QHttpMultiPart \* payloadMultiPart) <br> |
|  void | [**abort**](#function-abort) () <br> |
|  QNetworkReply \* | [**currentRawReply**](#function-currentrawreply) () const<br> |
|  void | [**ignoreSslErrors**](#function-ignoresslerrors) (const QList&lt; QSslError &gt; & errors) <br> |
|  bool | [**isFinished**](#function-isfinished) () const<br> |




























## Detailed Description


A wrapper around QNetworkReply that allows retriable requests. 


    
## Public Signals Documentation




### signal currentRawReplyChanged 

```C++
void QfNetworkReply::currentRawReplyChanged;
```



Emitted when reply has changed. 


        

<hr>



### signal downloadProgress 

```C++
void QfNetworkReply::downloadProgress;
```



Replicates `QNetworkReply::downloadProgress` signal. 

**Note:**

Because download may fail mid request and then retried, the bytesReceived may be reset back to 0. 




**Parameters:**


* `bytesReceived` 
* `bytesTotal` 




        

<hr>



### signal encrypted 

```C++
void QfNetworkReply::encrypted;
```



Replicates `QNetworkReply::encrypted` signal. 

**Note:**

May be called multiple times for each retry. 





        

<hr>



### signal errorOccurred 

```C++
void QfNetworkReply::errorOccurred;
```



Emitted when a new error has occured. 

**Parameters:**


* `code` 




        

<hr>



### signal finished 

```C++
void QfNetworkReply::finished;
```



Replicates `QNetworkReply::finished` signal. It is called only once, when the request was successfull, got a final error or ran out of retries. 


        

<hr>



### signal redirectAllowed 

```C++
void QfNetworkReply::redirectAllowed;
```



Replicates `QNetworkReply::redirectAllowed` signal. 


        

<hr>



### signal redirected 

```C++
void QfNetworkReply::redirected;
```



Replicates `QNetworkReply::redirected` signal. 


        

<hr>



### signal retry 

```C++
void QfNetworkReply::retry;
```



Emitted when a new retry is initiated. 


        

<hr>



### signal temporaryErrorOccurred 

```C++
void QfNetworkReply::temporaryErrorOccurred;
```



Emitted when a new temporary error has occured. This is basically emitting the error that has occured during a retry. 

**Parameters:**


* `code` 




        

<hr>



### signal uploadProgress 

```C++
void QfNetworkReply::uploadProgress;
```



Replicates `QNetworkReply::uploadProgress` signal. 

**Note:**

Because upload may fail mid request and then retried, the bytesSent may be reset back to 0. 




**Parameters:**


* `bytesSent` 
* `bytesTotal` 




        

<hr>
## Public Functions Documentation




### function QfNetworkReply [1/2]

```C++
QfNetworkReply::QfNetworkReply (
    const QNetworkAccessManager::Operation operation,
    const QNetworkRequest & request,
    const QByteArray & payloadByteArray
) 
```



A wrapper around QNetworkReply that allows retriable requests. 

**Parameters:**


* `operation` HTTP method 
* `request` the request to be performed 
* `payloadByteArray` the request payload 




        

<hr>



### function QfNetworkReply [2/2]

```C++
QfNetworkReply::QfNetworkReply (
    const QNetworkAccessManager::Operation operation,
    const QNetworkRequest & request,
    QHttpMultiPart * payloadMultiPart
) 
```



A wrapper around QNetworkReply that allows retriable requests. 

**Parameters:**


* `operation` HTTP method 
* `request` the request to be performed 
* `payloadMultiPart` the request payload 




        

<hr>



### function abort 

```C++
void QfNetworkReply::abort () 
```



Aborts the current request and any other retries. Makes the current object into a final state. 


        

<hr>



### function currentRawReply 

```C++
QNetworkReply * QfNetworkReply::currentRawReply () const
```



Get the current `QNetworkReply` object. Note that it might get deleted even if the parent `QfNetworkReply` is not in case of redirect or internal retry. Do not delete it manually. 

**Returns:**

network currentRawReply 





        

<hr>



### function ignoreSslErrors 

```C++
void QfNetworkReply::ignoreSslErrors (
    const QList< QSslError > & errors
) 
```



Reimplements QNetworkReply::ignoreSslErrors. 

**Parameters:**


* `errors` a list of error to be ignored. 




        

<hr>



### function isFinished 

```C++
bool QfNetworkReply::isFinished () const
```



Whether the request reached a final status. 

**Returns:**

true if the request reached a final status. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfnetworkreply.h`

