

# Class NetworkReply



[**ClassList**](annotated.md) **>** [**NetworkReply**](classNetworkReply.md)



[More...](#detailed-description)

* `#include <networkreply.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentRawReplyChanged**](classNetworkReply.md#signal-currentrawreplychanged)  <br> |
| signal void | [**downloadProgress**](classNetworkReply.md#signal-downloadprogress) (qint64 bytesReceived, qint64 bytesTotal) <br> |
| signal void | [**encrypted**](classNetworkReply.md#signal-encrypted)  <br> |
| signal void | [**errorOccurred**](classNetworkReply.md#signal-erroroccurred) (QNetworkReply::NetworkError code) <br> |
| signal void | [**finished**](classNetworkReply.md#signal-finished)  <br> |
| signal void | [**redirectAllowed**](classNetworkReply.md#signal-redirectallowed)  <br> |
| signal void | [**redirected**](classNetworkReply.md#signal-redirected) (const QUrl & url) <br> |
| signal void | [**retry**](classNetworkReply.md#signal-retry)  <br> |
| signal void | [**temporaryErrorOccurred**](classNetworkReply.md#signal-temporaryerroroccurred) (QNetworkReply::NetworkError code) <br> |
| signal void | [**uploadProgress**](classNetworkReply.md#signal-uploadprogress) (qint64 bytesSent, qint64 bytesTotal) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NetworkReply**](#function-networkreply-12) (const QNetworkAccessManager::Operation operation, const QNetworkRequest & request, const QByteArray & payloadByteArray) <br> |
|   | [**NetworkReply**](#function-networkreply-22) (const QNetworkAccessManager::Operation operation, const QNetworkRequest & request, QHttpMultiPart \* payloadMultiPart) <br> |
|  void | [**abort**](#function-abort) () <br> |
|  QNetworkReply \* | [**currentRawReply**](#function-currentrawreply) () const<br> |
|  void | [**ignoreSslErrors**](#function-ignoresslerrors) (const QList&lt; QSslError &gt; & errors) <br> |
|  bool | [**isFinished**](#function-isfinished) () const<br> |




























## Detailed Description


A wrapper around QNetworkReply that allows retriable requests. 


    
## Public Signals Documentation




### signal currentRawReplyChanged 

```C++
void NetworkReply::currentRawReplyChanged;
```



Emitted when reply has changed. 


        

<hr>



### signal downloadProgress 

```C++
void NetworkReply::downloadProgress;
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
void NetworkReply::encrypted;
```



Replicates `QNetworkReply::encrypted` signal. 

**Note:**

May be called multiple times for each retry. 





        

<hr>



### signal errorOccurred 

```C++
void NetworkReply::errorOccurred;
```



Emitted when a new error has occured. 

**Parameters:**


* `code` 




        

<hr>



### signal finished 

```C++
void NetworkReply::finished;
```



Replicates `QNetworkReply::finished` signal. It is called only once, when the request was successfull, got a final error or ran out of retries. 


        

<hr>



### signal redirectAllowed 

```C++
void NetworkReply::redirectAllowed;
```



Replicates `QNetworkReply::redirectAllowed` signal. 


        

<hr>



### signal redirected 

```C++
void NetworkReply::redirected;
```



Replicates `QNetworkReply::redirected` signal. 


        

<hr>



### signal retry 

```C++
void NetworkReply::retry;
```



Emitted when a new retry is initiated. 


        

<hr>



### signal temporaryErrorOccurred 

```C++
void NetworkReply::temporaryErrorOccurred;
```



Emitted when a new temporary error has occured. This is basically emitting the error that has occured during a retry. 

**Parameters:**


* `code` 




        

<hr>



### signal uploadProgress 

```C++
void NetworkReply::uploadProgress;
```



Replicates `QNetworkReply::uploadProgress` signal. 

**Note:**

Because upload may fail mid request and then retried, the bytesSent may be reset back to 0. 




**Parameters:**


* `bytesSent` 
* `bytesTotal` 




        

<hr>
## Public Functions Documentation




### function NetworkReply [1/2]

```C++
NetworkReply::NetworkReply (
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



### function NetworkReply [2/2]

```C++
NetworkReply::NetworkReply (
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
void NetworkReply::abort () 
```



Aborts the current request and any other retries. Makes the current object into a final state. 


        

<hr>



### function currentRawReply 

```C++
QNetworkReply * NetworkReply::currentRawReply () const
```



Get the current `QNetworkReply` object. Note that it might get deleted even if the parent `NetworkReply` is not in case of redirect or internal retry. Do not delete it manually. 

**Returns:**

network currentRawReply 





        

<hr>



### function ignoreSslErrors 

```C++
void NetworkReply::ignoreSslErrors (
    const QList< QSslError > & errors
) 
```



Reimplements QNetworkReply::ignoreSslErrors. 

**Parameters:**


* `errors` a list of error to be ignored. 




        

<hr>



### function isFinished 

```C++
bool NetworkReply::isFinished () const
```



Whether the request reached a final status. 

**Returns:**

true if the request reached a final status. 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/networkreply.h`

