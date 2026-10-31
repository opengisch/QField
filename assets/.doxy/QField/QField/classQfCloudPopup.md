

# Class QfCloudPopup



[**ClassList**](annotated.md) **>** [**QfCloudPopup**](classQfCloudPopup.md)








Inherits the following classes: Popup


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfCloudStatus**](classQfCloudStatus.md) | [**cloudServiceStatus**](classQfCloudPopup.md#property-cloudservicestatus)  <br> |
| property date | [**currentDateTime**](classQfCloudPopup.md#property-currentdatetime)  <br> |
| property bool | [**isSynchronizing**](classQfCloudPopup.md#property-issynchronizing)  <br> |
| property bool | [**isUploading**](classQfCloudPopup.md#property-isuploading)  <br> |
| property string | [**lastSubscriptionUser**](classQfCloudPopup.md#property-lastsubscriptionuser)  <br> |
| property string | [**pendingAction**](classQfCloudPopup.md#property-pendingaction)  <br> |
| property string | [**pendingCreationTitle**](classQfCloudPopup.md#property-pendingcreationtitle)  <br> |
| property string | [**pendingUploadPath**](classQfCloudPopup.md#property-pendinguploadpath)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**cloudify**](#function-cloudify) (title, path) <br> |
|  void | [**fetchSubscriptionInformation**](#function-fetchsubscriptioninformation) () <br> |
|  void | [**goBack**](#function-goback) () <br> |
|  void | [**projectPush**](#function-projectpush) (shouldDownloadUpdates) <br> |
|  void | [**resetCurrentProject**](#function-resetcurrentproject) () <br> |
|  void | [**revertLocalChangesFromCurrentProject**](#function-revertlocalchangesfromcurrentproject) () <br> |
|  void | [**show**](#function-show) () <br> |
|  void | [**showAttachments**](#function-showattachments) () <br> |
|  void | [**showHistory**](#function-showhistory) () <br> |
|  void | [**showStorageBar**](#function-showstoragebar) (usedBytes, totalBytes, plan, thresholdWarningBytes, thresholdCriticalBytes) <br> |




























## Public Properties Documentation




### property cloudServiceStatus 

```C++
QfCloudStatus QfCloudPopup::cloudServiceStatus;
```




<hr>



### property currentDateTime 

```C++
date QfCloudPopup::currentDateTime;
```




<hr>



### property isSynchronizing 

```C++
bool QfCloudPopup::isSynchronizing;
```




<hr>



### property isUploading 

```C++
bool QfCloudPopup::isUploading;
```





**Remark:**

This property is read-only 





        

<hr>



### property lastSubscriptionUser 

```C++
string QfCloudPopup::lastSubscriptionUser;
```




<hr>



### property pendingAction 

```C++
string QfCloudPopup::pendingAction;
```




<hr>



### property pendingCreationTitle 

```C++
string QfCloudPopup::pendingCreationTitle;
```




<hr>



### property pendingUploadPath 

```C++
string QfCloudPopup::pendingUploadPath;
```




<hr>
## Public Functions Documentation




### function cloudify 

```C++
void QfCloudPopup::cloudify (
    title,
    path
) 
```




<hr>



### function fetchSubscriptionInformation 

```C++
void QfCloudPopup::fetchSubscriptionInformation () 
```




<hr>



### function goBack 

```C++
void QfCloudPopup::goBack () 
```




<hr>



### function projectPush 

```C++
void QfCloudPopup::projectPush (
    shouldDownloadUpdates
) 
```




<hr>



### function resetCurrentProject 

```C++
void QfCloudPopup::resetCurrentProject () 
```




<hr>



### function revertLocalChangesFromCurrentProject 

```C++
void QfCloudPopup::revertLocalChangesFromCurrentProject () 
```




<hr>



### function show 

```C++
void QfCloudPopup::show () 
```




<hr>



### function showAttachments 

```C++
void QfCloudPopup::showAttachments () 
```




<hr>



### function showHistory 

```C++
void QfCloudPopup::showHistory () 
```




<hr>



### function showStorageBar 

```C++
void QfCloudPopup::showStorageBar (
    usedBytes,
    totalBytes,
    plan,
    thresholdWarningBytes,
    thresholdCriticalBytes
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qml/QfCloudPopup.qml`

