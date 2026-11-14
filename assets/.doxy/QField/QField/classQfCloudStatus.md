

# Class QfCloudStatus



[**ClassList**](annotated.md) **>** [**QfCloudStatus**](classQfCloudStatus.md)



[More...](#detailed-description)

* `#include <qfcloudstatus.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**StatusType**](#enum-statustype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**detailsMessage**](classQfCloudStatus.md#property-detailsmessage-12)  <br> |
| property bool | [**hasProblem**](classQfCloudStatus.md#property-hasproblem-12)  <br> |
| property QString | [**statusMessage**](classQfCloudStatus.md#property-statusmessage-12)  <br> |
| property QString | [**statusPageUrl**](classQfCloudStatus.md#property-statuspageurl-12)  <br> |
| property StatusType | [**statusType**](classQfCloudStatus.md#property-statustype-12)  <br> |
| property QString | [**url**](classQfCloudStatus.md#property-url-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**statusUpdated**](classQfCloudStatus.md#signal-statusupdated)  <br> |
| signal void | [**urlChanged**](classQfCloudStatus.md#signal-urlchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudStatus**](#function-qfcloudstatus) (QObject \* parent=nullptr) <br> |
|  QString | [**detailsMessage**](#function-detailsmessage-22) () const<br> |
|  bool | [**hasProblem**](#function-hasproblem-22) () const<br> |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br> |
|  void | [**setUrl**](#function-seturl) (const QString & url) <br> |
|  QString | [**statusMessage**](#function-statusmessage-22) () const<br> |
|  QString | [**statusPageUrl**](#function-statuspageurl-22) () const<br> |
|  StatusType | [**statusType**](#function-statustype-22) () const<br> |
|  QString | [**url**](#function-url-22) () const<br> |




























## Detailed Description


[**QfCloudStatus**](classQfCloudStatus.md) fetches the QFieldCloud service status endpoint and exposes the health state of the service to QML.


It fetches the /api/v1/status/ endpoint once per session and provides properties to determine whether the service is degraded, has ongoing incidents, or is under maintenance. 


    
## Public Types Documentation




### enum StatusType 

```C++
enum QfCloudStatus::StatusType {
    Ok,
    Maintenance,
    Degraded,
    Incident
};
```




<hr>
## Public Properties Documentation




### property detailsMessage [1/2]

```C++
QString QfCloudStatus::detailsMessage;
```




<hr>



### property hasProblem [1/2]

```C++
bool QfCloudStatus::hasProblem;
```




<hr>



### property statusMessage [1/2]

```C++
QString QfCloudStatus::statusMessage;
```




<hr>



### property statusPageUrl [1/2]

```C++
QString QfCloudStatus::statusPageUrl;
```




<hr>



### property statusType [1/2]

```C++
StatusType QfCloudStatus::statusType;
```




<hr>



### property url [1/2]

```C++
QString QfCloudStatus::url;
```




<hr>
## Public Signals Documentation




### signal statusUpdated 

```C++
void QfCloudStatus::statusUpdated;
```




<hr>



### signal urlChanged 

```C++
void QfCloudStatus::urlChanged;
```




<hr>
## Public Functions Documentation




### function QfCloudStatus 

```C++
explicit QfCloudStatus::QfCloudStatus (
    QObject * parent=nullptr
) 
```




<hr>



### function detailsMessage [2/2]

```C++
QString QfCloudStatus::detailsMessage () const
```



Returns additional details (incident/maintenance message) if available. 


        

<hr>



### function hasProblem [2/2]

```C++
bool QfCloudStatus::hasProblem () const
```



Returns true if the service has any problem (degraded, incident, or maintenance). 


        

<hr>



### function refresh 

```C++
Q_INVOKABLE void QfCloudStatus::refresh () 
```



Manually triggers a status fetch. 


        

<hr>



### function setUrl 

```C++
void QfCloudStatus::setUrl (
    const QString & url
) 
```



Sets the base URL of the QFieldCloud server and triggers a status fetch. 


        

<hr>



### function statusMessage [2/2]

```C++
QString QfCloudStatus::statusMessage () const
```



Returns a short user-facing message summarizing the service status. 


        

<hr>



### function statusPageUrl [2/2]

```C++
QString QfCloudStatus::statusPageUrl () const
```



Returns the URL to the status page (e.g. [https://status.qfield.cloud/](https://status.qfield.cloud/)). 


        

<hr>



### function statusType [2/2]

```C++
StatusType QfCloudStatus::statusType () const
```



Returns the current status type of the QFieldCloud service. 


        

<hr>



### function url [2/2]

```C++
QString QfCloudStatus::url () const
```



Returns the base URL of the QFieldCloud server. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudstatus.h`

