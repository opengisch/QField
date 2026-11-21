

# Class QfCloudAttachmentsWatcher



[**ClassList**](annotated.md) **>** [**QfCloudAttachmentsWatcher**](classQfCloudAttachmentsWatcher.md)



[More...](#detailed-description)

* `#include <qfcloudattachmentswatcher.h>`



Inherits the following classes: QObject






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**queueChanged**](classQfCloudAttachmentsWatcher.md#signal-queuechanged)  <br>_Emitted when the pending attachments queue has changed._  |
| signal void | [**queueFinished**](classQfCloudAttachmentsWatcher.md#signal-queuefinished)  <br>_Emitted when an outstanding pending attachments queue has been emptied._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCloudAttachmentsWatcher**](#function-qfcloudattachmentswatcher) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active) () const<br>_Returns TRUE when the pending attachments queue is being watched._  |
|  [**QfCloudConnection**](classQfCloudConnection.md) \* | [**cloudConnection**](#function-cloudconnection) () const<br>_Returns the cloud connection whose user's pending attachments queue is watched._  |
|  void | [**setActive**](#function-setactive) (bool active) <br>_Sets whether the pending attachments queue is being watched._  |
|  void | [**setCloudConnection**](#function-setcloudconnection) ([**QfCloudConnection**](classQfCloudConnection.md) \* cloudConnection) <br>_Sets the_ _cloudConnection_ _whose user's pending attachments queue is watched._ |
|   | [**~QfCloudAttachmentsWatcher**](#function-qfcloudattachmentswatcher) () override<br> |




























## Detailed Description


Watches the QFieldCloud pending attachments queue and signals when it has changed. 


    
## Public Signals Documentation




### signal queueChanged 

_Emitted when the pending attachments queue has changed._ 
```C++
void QfCloudAttachmentsWatcher::queueChanged;
```




<hr>



### signal queueFinished 

_Emitted when an outstanding pending attachments queue has been emptied._ 
```C++
void QfCloudAttachmentsWatcher::queueFinished;
```




<hr>
## Public Functions Documentation




### function QfCloudAttachmentsWatcher 

```C++
explicit QfCloudAttachmentsWatcher::QfCloudAttachmentsWatcher (
    QObject * parent=nullptr
) 
```




<hr>



### function active 

_Returns TRUE when the pending attachments queue is being watched._ 
```C++
bool QfCloudAttachmentsWatcher::active () const
```




<hr>



### function cloudConnection 

_Returns the cloud connection whose user's pending attachments queue is watched._ 
```C++
QfCloudConnection * QfCloudAttachmentsWatcher::cloudConnection () const
```




<hr>



### function setActive 

_Sets whether the pending attachments queue is being watched._ 
```C++
void QfCloudAttachmentsWatcher::setActive (
    bool active
) 
```




<hr>



### function setCloudConnection 

_Sets the_ _cloudConnection_ _whose user's pending attachments queue is watched._
```C++
void QfCloudAttachmentsWatcher::setCloudConnection (
    QfCloudConnection * cloudConnection
) 
```




<hr>



### function ~QfCloudAttachmentsWatcher 

```C++
QfCloudAttachmentsWatcher::~QfCloudAttachmentsWatcher () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfcloudattachmentswatcher.h`

