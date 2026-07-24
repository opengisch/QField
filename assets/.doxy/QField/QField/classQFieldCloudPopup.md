

# Class QFieldCloudPopup



[**ClassList**](annotated.md) **>** [**QFieldCloudPopup**](classQFieldCloudPopup.md)








Inherits the following classes: Popup


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QFieldCloudStatus**](classQFieldCloudStatus.md) | [**cloudServiceStatus**](classQFieldCloudPopup.md#property-cloudservicestatus)  <br> |
| property date | [**currentDateTime**](classQFieldCloudPopup.md#property-currentdatetime)  <br> |
| property string | [**lastSubscriptionUser**](classQFieldCloudPopup.md#property-lastsubscriptionuser)  <br> |
| property string | [**pendingAction**](classQFieldCloudPopup.md#property-pendingaction)  <br> |
| property string | [**pendingCreationTitle**](classQFieldCloudPopup.md#property-pendingcreationtitle)  <br> |
| property string | [**pendingUploadPath**](classQFieldCloudPopup.md#property-pendinguploadpath)  <br> |








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
|  void | [**showStorageBar**](#function-showstoragebar) (usedBytes, totalBytes, plan, thresholdWarningBytes, thresholdCriticalBytes) <br> |




























## Public Properties Documentation




### property cloudServiceStatus 

```C++
QFieldCloudStatus QFieldCloudPopup::cloudServiceStatus;
```




<hr>



### property currentDateTime 

```C++
date QFieldCloudPopup::currentDateTime;
```




<hr>



### property lastSubscriptionUser 

```C++
string QFieldCloudPopup::lastSubscriptionUser;
```




<hr>



### property pendingAction 

```C++
string QFieldCloudPopup::pendingAction;
```




<hr>



### property pendingCreationTitle 

```C++
string QFieldCloudPopup::pendingCreationTitle;
```




<hr>



### property pendingUploadPath 

```C++
string QFieldCloudPopup::pendingUploadPath;
```




<hr>
## Public Functions Documentation




### function cloudify 

```C++
void QFieldCloudPopup::cloudify (
    title,
    path
) 
```




<hr>



### function fetchSubscriptionInformation 

```C++
void QFieldCloudPopup::fetchSubscriptionInformation () 
```




<hr>



### function goBack 

```C++
void QFieldCloudPopup::goBack () 
```




<hr>



### function projectPush 

```C++
void QFieldCloudPopup::projectPush (
    shouldDownloadUpdates
) 
```




<hr>



### function resetCurrentProject 

```C++
void QFieldCloudPopup::resetCurrentProject () 
```




<hr>



### function revertLocalChangesFromCurrentProject 

```C++
void QFieldCloudPopup::revertLocalChangesFromCurrentProject () 
```




<hr>



### function show 

```C++
void QFieldCloudPopup::show () 
```




<hr>



### function showStorageBar 

```C++
void QFieldCloudPopup::showStorageBar (
    usedBytes,
    totalBytes,
    plan,
    thresholdWarningBytes,
    thresholdCriticalBytes
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/QFieldCloudPopup.qml`

