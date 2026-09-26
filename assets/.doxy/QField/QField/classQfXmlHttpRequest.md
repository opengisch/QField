

# Class QfXmlHttpRequest



[**ClassList**](annotated.md) **>** [**QfXmlHttpRequest**](classQfXmlHttpRequest.md)



[More...](#detailed-description)

* `#include <qfxmlhttprequest.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum uchar | [**ReadyState**](#enum-readystate)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QJSValue | [**onaborted**](classQfXmlHttpRequest.md#property-onaborted-12)  <br> |
| property QJSValue | [**ondownloadprogress**](classQfXmlHttpRequest.md#property-ondownloadprogress-12)  <br> |
| property QJSValue | [**onerror**](classQfXmlHttpRequest.md#property-onerror-12)  <br> |
| property QJSValue | [**onload**](classQfXmlHttpRequest.md#property-onload-12)  <br> |
| property QJSValue | [**onloadend**](classQfXmlHttpRequest.md#property-onloadend-12)  <br> |
| property QJSValue | [**onreadystatechange**](classQfXmlHttpRequest.md#property-onreadystatechange-12)  <br> |
| property QJSValue | [**onredirected**](classQfXmlHttpRequest.md#property-onredirected-12)  <br> |
| property QJSValue | [**ontimeout**](classQfXmlHttpRequest.md#property-ontimeout-12)  <br> |
| property QJSValue | [**onuploadprogress**](classQfXmlHttpRequest.md#property-onuploadprogress-12)  <br> |
| property [**ReadyState**](classQfXmlHttpRequest.md#enum-readystate) | [**readyState**](classQfXmlHttpRequest.md#property-readystate-12)  <br> |
| property QVariant | [**response**](classQfXmlHttpRequest.md#property-response-12)  <br> |
| property QString | [**responseText**](classQfXmlHttpRequest.md#property-responsetext-12)  <br> |
| property QString | [**responseType**](classQfXmlHttpRequest.md#property-responsetype-12)  <br> |
| property QUrl | [**responseUrl**](classQfXmlHttpRequest.md#property-responseurl-12)  <br> |
| property int | [**status**](classQfXmlHttpRequest.md#property-status-12)  <br> |
| property QString | [**statusText**](classQfXmlHttpRequest.md#property-statustext-12)  <br> |
| property int | [**timeout**](classQfXmlHttpRequest.md#property-timeout-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**readyStateChanged**](classQfXmlHttpRequest.md#signal-readystatechanged)  <br> |
| signal void | [**responseChanged**](classQfXmlHttpRequest.md#signal-responsechanged)  <br>_Emitted whenever response-related properties change._  |
| signal void | [**timeoutChanged**](classQfXmlHttpRequest.md#signal-timeoutchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfXmlHttpRequest**](#function-qfxmlhttprequest) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**abort**](#function-abort) () <br> |
|  Q\_INVOKABLE QString | [**getAllResponseHeaders**](#function-getallresponseheaders) () const<br> |
|  Q\_INVOKABLE QString | [**getResponseHeader**](#function-getresponseheader) (const QString & name) const<br> |
|  QJSValue | [**onaborted**](#function-onaborted-22) () const<br> |
|  QJSValue | [**ondownloadprogress**](#function-ondownloadprogress-22) () const<br> |
|  QJSValue | [**onerror**](#function-onerror-22) () const<br> |
|  QJSValue | [**onload**](#function-onload-22) () const<br> |
|  QJSValue | [**onloadend**](#function-onloadend-22) () const<br> |
|  QJSValue | [**onreadystatechange**](#function-onreadystatechange-22) () const<br> |
|  QJSValue | [**onredirected**](#function-onredirected-22) () const<br> |
|  QJSValue | [**ontimeout**](#function-ontimeout-22) () const<br> |
|  QJSValue | [**onuploadprogress**](#function-onuploadprogress-22) () const<br> |
|  Q\_INVOKABLE void | [**open**](#function-open) (const QString & method, const QUrl & url) <br> |
|  [**ReadyState**](classQfXmlHttpRequest.md#enum-readystate) | [**readyState**](#function-readystate-22) () const<br> |
|  QVariant | [**response**](#function-response-22) () const<br> |
|  QString | [**responseText**](#function-responsetext-22) () const<br> |
|  QString | [**responseType**](#function-responsetype-22) () const<br> |
|  QUrl | [**responseUrl**](#function-responseurl-22) () const<br> |
|  Q\_INVOKABLE void | [**send**](#function-send) (const QVariant & body=QVariant()) <br> |
|  void | [**setOnaborted**](#function-setonaborted) (const QJSValue & callback) <br> |
|  void | [**setOndownloadprogress**](#function-setondownloadprogress) (const QJSValue & callback) <br> |
|  void | [**setOnerror**](#function-setonerror) (const QJSValue & callback) <br> |
|  void | [**setOnload**](#function-setonload) (const QJSValue & callback) <br> |
|  void | [**setOnloadend**](#function-setonloadend) (const QJSValue & callback) <br> |
|  void | [**setOnreadystatechange**](#function-setonreadystatechange) (const QJSValue & callback) <br> |
|  void | [**setOnredirected**](#function-setonredirected) (const QJSValue & callback) <br> |
|  void | [**setOntimeout**](#function-setontimeout) (const QJSValue & callback) <br> |
|  void | [**setOnuploadprogress**](#function-setonuploadprogress) (const QJSValue & callback) <br> |
|  Q\_INVOKABLE void | [**setRequestHeader**](#function-setrequestheader) (const QString & headerName, const QString & headerValue) <br> |
|  void | [**setTimeout**](#function-settimeout) (int milliseconds) <br> |
|  int | [**status**](#function-status-22) () const<br> |
|  QString | [**statusText**](#function-statustext-22) () const<br> |
|  int | [**timeout**](#function-timeout-22) () const<br> |
|   | [**~QfXmlHttpRequest**](#function-qfxmlhttprequest) () override<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE [**QfXmlHttpRequest**](classQfXmlHttpRequest.md) \* | [**newRequest**](#function-newrequest) (QObject \* parent=nullptr) <br> |


























## Detailed Description


This class exposes a lightweight, XMLHttpRequest-like API to QML for making HTTP requests in a predictable way. It follows the familiar [**open()**](classQfXmlHttpRequest.md#function-open), [**setRequestHeader()**](classQfXmlHttpRequest.md#function-setrequestheader), [**send()**](classQfXmlHttpRequest.md#function-send), and [**abort()**](classQfXmlHttpRequest.md#function-abort) workflow while exposing request state, response data, and lifecycle callbacks as QML properties.


Payload encoding is deterministic. QByteArray payloads are transmitted as-is. QString payloads are sent as UTF-8 text. QVariantMap and QVariantList payloads are encoded as JSON. When the caller does not explicitly set a Content-Type header, an appropriate Content-Type is inferred from the payload type.


Multipart uploads are supported when the body contains "file://..." URL strings, or when the caller sets a Content-Type header starting with "multipart/form-data". Local file uploads are restricted to paths inside the current project directory or inside the QFieldCloud local directory.


Requests created for QML usage are expected to be short-lived. The object schedules deleteLater() automatically once the request lifecycle ends (success, error, abort, or timeout).




**Note:**

This is not a full browser XMLHttpRequest implementation. It focuses on a clean QML API and predictable behavior within QField. 





    
## Public Types Documentation




### enum ReadyState 

```C++
enum QfXmlHttpRequest::ReadyState {
    Unsent = 0,
    Opened,
    HeadersReceived,
    Loading,
    Done
};
```



ReadyState values aligned with browser XMLHttpRequest. 


        

<hr>
## Public Properties Documentation




### property onaborted [1/2]

```C++
QJSValue QfXmlHttpRequest::onaborted;
```



Called when the request is aborted. 


        

<hr>



### property ondownloadprogress [1/2]

```C++
QJSValue QfXmlHttpRequest::ondownloadprogress;
```



Called during download progress as (receivedBytes, totalBytes). 


        

<hr>



### property onerror [1/2]

```C++
QJSValue QfXmlHttpRequest::onerror;
```



Called for network-level failures as (errorCode, errorMessage). 


        

<hr>



### property onload [1/2]

```C++
QJSValue QfXmlHttpRequest::onload;
```



Called when the request completes and a status code is available.




**Note:**

HTTP errors such as 404 or 500 still trigger onload. Callers should inspect status. 





        

<hr>



### property onloadend [1/2]

```C++
QJSValue QfXmlHttpRequest::onloadend;
```



Called at the end of the request lifecycle for success, error, abort, or timeout. 


        

<hr>



### property onreadystatechange [1/2]

```C++
QJSValue QfXmlHttpRequest::onreadystatechange;
```



Called whenever readyState changes. 


        

<hr>



### property onredirected [1/2]

```C++
QJSValue QfXmlHttpRequest::onredirected;
```



Called when a redirect is reported by Qt as (urlString). 


        

<hr>



### property ontimeout [1/2]

```C++
QJSValue QfXmlHttpRequest::ontimeout;
```



Called when the timeout triggers. 


        

<hr>



### property onuploadprogress [1/2]

```C++
QJSValue QfXmlHttpRequest::onuploadprogress;
```



Called during upload progress as (sentBytes, totalBytes). 


        

<hr>



### property readyState [1/2]

```C++
ReadyState QfXmlHttpRequest::readyState;
```



The current request state aligned with browser XMLHttpRequest readyState values. 


        

<hr>



### property response [1/2]

```C++
QVariant QfXmlHttpRequest::response;
```



The parsed response value when the response is JSON, otherwise this equals responseText. 


        

<hr>



### property responseText [1/2]

```C++
QString QfXmlHttpRequest::responseText;
```



The response body decoded as UTF-8 text. 


        

<hr>



### property responseType [1/2]

```C++
QString QfXmlHttpRequest::responseType;
```



The response Content-Type value as provided by the server. 


        

<hr>



### property responseUrl [1/2]

```C++
QUrl QfXmlHttpRequest::responseUrl;
```



The final response URL. This may differ from the requested URL when redirects were followed. 


        

<hr>



### property status [1/2]

```C++
int QfXmlHttpRequest::status;
```



The HTTP status code of the final response. For network-level failures, this is 0. 


        

<hr>



### property statusText [1/2]

```C++
QString QfXmlHttpRequest::statusText;
```



The HTTP reason phrase of the final response when available. 


        

<hr>



### property timeout [1/2]

```C++
int QfXmlHttpRequest::timeout;
```



The request timeout in milliseconds. A value of 0 disables the timeout.


When the timeout triggers, the request is aborted and ontimeout and onloadend are called. 


        

<hr>
## Public Signals Documentation




### signal readyStateChanged 

```C++
void QfXmlHttpRequest::readyStateChanged;
```



The current request state aligned with browser XMLHttpRequest readyState values.   


        

<hr>



### signal responseChanged 

_Emitted whenever response-related properties change._ 
```C++
void QfXmlHttpRequest::responseChanged;
```




<hr>



### signal timeoutChanged 

```C++
void QfXmlHttpRequest::timeoutChanged;
```



The request timeout in milliseconds. A value of 0 disables the timeout.


When the timeout triggers, the request is aborted and ontimeout and onloadend are called.   


        

<hr>
## Public Functions Documentation




### function QfXmlHttpRequest 

```C++
explicit QfXmlHttpRequest::QfXmlHttpRequest (
    QObject * parent=nullptr
) 
```




<hr>



### function abort 

```C++
Q_INVOKABLE void QfXmlHttpRequest::abort () 
```



Aborts the current request if one is active.




**Note:**

This triggers onaborted and onloadend if set and transitions readyState to Done. 





        

<hr>



### function getAllResponseHeaders 

```C++
Q_INVOKABLE QString QfXmlHttpRequest::getAllResponseHeaders () const
```



Returns all response headers in HTTP-style format. This is available only after the request finishes. 


        

<hr>



### function getResponseHeader 

```C++
Q_INVOKABLE QString QfXmlHttpRequest::getResponseHeader (
    const QString & name
) const
```



Returns a response header value by name. This is available only after the request finishes. 


        

<hr>



### function onaborted [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onaborted () const
```



Called when the request is aborted.   


        

<hr>



### function ondownloadprogress [2/2]

```C++
inline QJSValue QfXmlHttpRequest::ondownloadprogress () const
```



Called during download progress as (receivedBytes, totalBytes).   


        

<hr>



### function onerror [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onerror () const
```



Called for network-level failures as (errorCode, errorMessage).   


        

<hr>



### function onload [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onload () const
```



Called when the request completes and a status code is available.




**Note:**

HTTP errors such as 404 or 500 still trigger onload. Callers should inspect status.   





        

<hr>



### function onloadend [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onloadend () const
```



Called at the end of the request lifecycle for success, error, abort, or timeout.   


        

<hr>



### function onreadystatechange [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onreadystatechange () const
```



Called whenever readyState changes.   


        

<hr>



### function onredirected [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onredirected () const
```



Called when a redirect is reported by Qt as (urlString).   


        

<hr>



### function ontimeout [2/2]

```C++
inline QJSValue QfXmlHttpRequest::ontimeout () const
```



Called when the timeout triggers.   


        

<hr>



### function onuploadprogress [2/2]

```C++
inline QJSValue QfXmlHttpRequest::onuploadprogress () const
```



Called during upload progress as (sentBytes, totalBytes).   


        

<hr>



### function open 

```C++
Q_INVOKABLE void QfXmlHttpRequest::open (
    const QString & method,
    const QUrl & url
) 
```



Prepares the request. This must be called before [**send()**](classQfXmlHttpRequest.md#function-send).


Calling [**open()**](classQfXmlHttpRequest.md#function-open) while a request is active aborts the active request. Supported methods are GET, POST, PUT, and DELETE. 


        

<hr>



### function readyState [2/2]

```C++
inline ReadyState QfXmlHttpRequest::readyState () const
```



The current request state aligned with browser XMLHttpRequest readyState values.   


        

<hr>



### function response [2/2]

```C++
inline QVariant QfXmlHttpRequest::response () const
```



The parsed response value when the response is JSON, otherwise this equals responseText.   


        

<hr>



### function responseText [2/2]

```C++
inline QString QfXmlHttpRequest::responseText () const
```



The response body decoded as UTF-8 text.   


        

<hr>



### function responseType [2/2]

```C++
inline QString QfXmlHttpRequest::responseType () const
```



The response Content-Type value as provided by the server.   


        

<hr>



### function responseUrl [2/2]

```C++
inline QUrl QfXmlHttpRequest::responseUrl () const
```



The final response URL. This may differ from the requested URL when redirects were followed.   


        

<hr>



### function send 

```C++
Q_INVOKABLE void QfXmlHttpRequest::send (
    const QVariant & body=QVariant()
) 
```



Starts the request.


The body can be provided as QByteArray, QString, QVariantMap, or QVariantList. QByteArray is sent as-is, QString is encoded as UTF-8 text, and QVariantMap/QVariantList are JSON-encoded. If multipart upload is desired, the caller can pass a QVariantMap containing "file://..." string values, or can set a Content-Type header starting with "multipart/form-data". Local file URLs are accepted only when they resolve inside the current project directory or the QFieldCloud local directory. 


        

<hr>



### function setOnaborted 

```C++
inline void QfXmlHttpRequest::setOnaborted (
    const QJSValue & callback
) 
```



Called when the request is aborted.   


        

<hr>



### function setOndownloadprogress 

```C++
inline void QfXmlHttpRequest::setOndownloadprogress (
    const QJSValue & callback
) 
```



Called during download progress as (receivedBytes, totalBytes).   


        

<hr>



### function setOnerror 

```C++
inline void QfXmlHttpRequest::setOnerror (
    const QJSValue & callback
) 
```



Called for network-level failures as (errorCode, errorMessage).   


        

<hr>



### function setOnload 

```C++
inline void QfXmlHttpRequest::setOnload (
    const QJSValue & callback
) 
```



Called when the request completes and a status code is available.




**Note:**

HTTP errors such as 404 or 500 still trigger onload. Callers should inspect status.   





        

<hr>



### function setOnloadend 

```C++
inline void QfXmlHttpRequest::setOnloadend (
    const QJSValue & callback
) 
```



Called at the end of the request lifecycle for success, error, abort, or timeout.   


        

<hr>



### function setOnreadystatechange 

```C++
inline void QfXmlHttpRequest::setOnreadystatechange (
    const QJSValue & callback
) 
```



Called whenever readyState changes.   


        

<hr>



### function setOnredirected 

```C++
inline void QfXmlHttpRequest::setOnredirected (
    const QJSValue & callback
) 
```



Called when a redirect is reported by Qt as (urlString).   


        

<hr>



### function setOntimeout 

```C++
inline void QfXmlHttpRequest::setOntimeout (
    const QJSValue & callback
) 
```



Called when the timeout triggers.   


        

<hr>



### function setOnuploadprogress 

```C++
inline void QfXmlHttpRequest::setOnuploadprogress (
    const QJSValue & callback
) 
```



Called during upload progress as (sentBytes, totalBytes).   


        

<hr>



### function setRequestHeader 

```C++
Q_INVOKABLE void QfXmlHttpRequest::setRequestHeader (
    const QString & headerName,
    const QString & headerValue
) 
```



Sets a raw HTTP request header.


Headers are only applied after [**open()**](classQfXmlHttpRequest.md#function-open) and before [**send()**](classQfXmlHttpRequest.md#function-send). Newline protection is applied to prevent CR/LF header injection. 


        

<hr>



### function setTimeout 

```C++
void QfXmlHttpRequest::setTimeout (
    int milliseconds
) 
```



The request timeout in milliseconds. A value of 0 disables the timeout.


When the timeout triggers, the request is aborted and ontimeout and onloadend are called.   


        

<hr>



### function status [2/2]

```C++
inline int QfXmlHttpRequest::status () const
```



The HTTP status code of the final response. For network-level failures, this is 0.   


        

<hr>



### function statusText [2/2]

```C++
inline QString QfXmlHttpRequest::statusText () const
```



The HTTP reason phrase of the final response when available.   


        

<hr>



### function timeout [2/2]

```C++
inline int QfXmlHttpRequest::timeout () const
```



The request timeout in milliseconds. A value of 0 disables the timeout.


When the timeout triggers, the request is aborted and ontimeout and onloadend are called.   


        

<hr>



### function ~QfXmlHttpRequest 

```C++
QfXmlHttpRequest::~QfXmlHttpRequest () override
```




<hr>
## Public Static Functions Documentation




### function newRequest 

```C++
static Q_INVOKABLE QfXmlHttpRequest * QfXmlHttpRequest::newRequest (
    QObject * parent=nullptr
) 
```



Creates a new request instance. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfxmlhttprequest.h`

