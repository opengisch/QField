

# Class ExternalStorage



[**ClassList**](annotated.md) **>** [**ExternalStorage**](classExternalStorage.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**fetchedContent**](classExternalStorage.md#property-fetchedcontent-12)  <br> |
| property QString | [**lastError**](classExternalStorage.md#property-lasterror-12)  <br> |
| property Qgis::ContentStatus | [**status**](classExternalStorage.md#property-status-12)  <br> |
| property QString | [**type**](classExternalStorage.md#property-type-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fetchedContentChanged**](classExternalStorage.md#signal-fetchedcontentchanged)  <br> |
| signal void | [**lastErrorChanged**](classExternalStorage.md#signal-lasterrorchanged)  <br> |
| signal void | [**statusChanged**](classExternalStorage.md#signal-statuschanged)  <br> |
| signal void | [**typeChanged**](classExternalStorage.md#signal-typechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExternalStorage**](#function-externalstorage) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**fetch**](#function-fetch) (const QString & url, const QString & authenticationConfigurationId) <br> |
|  QString | [**fetchedContent**](#function-fetchedcontent-22) () const<br> |
|  QString | [**lastError**](#function-lasterror-22) () const<br> |
|  void | [**setType**](#function-settype) (const QString & type) <br> |
|  Qgis::ContentStatus | [**status**](#function-status-22) () const<br> |
|  QString | [**type**](#function-type-22) () const<br> |




























## Public Properties Documentation




### property fetchedContent [1/2]

```C++
QString ExternalStorage::fetchedContent;
```




<hr>



### property lastError [1/2]

```C++
QString ExternalStorage::lastError;
```




<hr>



### property status [1/2]

```C++
Qgis::ContentStatus ExternalStorage::status;
```




<hr>



### property type [1/2]

```C++
QString ExternalStorage::type;
```




<hr>
## Public Signals Documentation




### signal fetchedContentChanged 

```C++
void ExternalStorage::fetchedContentChanged;
```




<hr>



### signal lastErrorChanged 

```C++
void ExternalStorage::lastErrorChanged;
```




<hr>



### signal statusChanged 

```C++
void ExternalStorage::statusChanged;
```




<hr>



### signal typeChanged 

```C++
void ExternalStorage::typeChanged;
```




<hr>
## Public Functions Documentation




### function ExternalStorage 

```C++
explicit ExternalStorage::ExternalStorage (
    QObject * parent=nullptr
) 
```




<hr>



### function fetch 

```C++
Q_INVOKABLE void ExternalStorage::fetch (
    const QString & url,
    const QString & authenticationConfigurationId
) 
```



Triggers a fetch operation to download the content from an external storage and make it available locally. 

**Parameters:**


* `url` the remote URL of the content 
* `authenticationConfigurationId` the authentication configuration ID used to connect to the external storage endpoint 




        

<hr>



### function fetchedContent [2/2]

```C++
QString ExternalStorage::fetchedContent () const
```



Returns the file path of a successfully fetched content operation. 


        

<hr>



### function lastError [2/2]

```C++
QString ExternalStorage::lastError () const
```



Returns the last error emitted by an external storage operation. 


        

<hr>



### function setType 

```C++
void ExternalStorage::setType (
    const QString & type
) 
```



Sets the current external storage type string. The type string must be tied to an external storage object that was added in the QgsApplication::externalStorageRegistry(). 


        

<hr>



### function status [2/2]

```C++
Qgis::ContentStatus ExternalStorage::status () const
```



Returns the current status of the external storage object. When a fetch operation has been triggered, the status will reflect the last fetched content operation. 


        

<hr>



### function type [2/2]

```C++
QString ExternalStorage::type () const
```



Returns the current external storage type string. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/externalstorage.h`

