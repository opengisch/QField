

# Class UrlUtils



[**ClassList**](annotated.md) **>** [**UrlUtils**](classUrlUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**UrlUtils**](#function-urlutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE QString | [**createActionUrl**](#function-createactionurl) (const QString & scheme, const QString & type, const QVariantMap & details) <br> |
|  Q\_INVOKABLE QString | [**createEncodedUrl**](#function-createencodedurl) (const QVariantMap & parameters) <br> |
|  Q\_INVOKABLE QUrl | [**fromString**](#function-fromstring) (const QString & url) <br>_Returns a URL from a_ _url_ _with logic to handle local paths._ |
|  Q\_INVOKABLE QVariantMap | [**getActionDetails**](#function-getactiondetails) (const QString & url) <br> |
|  Q\_INVOKABLE bool | [**isRelativeOrFileUrl**](#function-isrelativeorfileurl) (const QString & url) <br> |
|  Q\_INVOKABLE QString | [**toLocalFile**](#function-tolocalfile) (const QString & url) <br>_Returns a URL from a_ _url_ _with logic to presence or absence of_[file://](file://) _._ |
|  Q\_INVOKABLE QString | [**urlDetail**](#function-urldetail) (const QString & url, const QString & detail) <br> |


























## Public Functions Documentation




### function UrlUtils 

```C++
explicit UrlUtils::UrlUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function createActionUrl 

```C++
static Q_INVOKABLE QString UrlUtils::createActionUrl (
    const QString & scheme,
    const QString & type,
    const QVariantMap & details
) 
```



Returns a QField action url string. 

**Parameters:**


* `scheme` the action scheme, a qfield value will return a "qfield://" scheme an https value will return an "https://qfield.org" scheme and domain name 
* `type` the action type (local or cloud) 
* `details` the action details transformed into URL query parameters 




        

<hr>



### function createEncodedUrl 

```C++
static Q_INVOKABLE QString UrlUtils::createEncodedUrl (
    const QVariantMap & parameters
) 
```



Returns an encoded URL built using parameters' pair of key and value as query items. 


        

<hr>



### function fromString 

_Returns a URL from a_ _url_ _with logic to handle local paths._
```C++
static Q_INVOKABLE QUrl UrlUtils::fromString (
    const QString & url
) 
```




<hr>



### function getActionDetails 

```C++
static Q_INVOKABLE QVariantMap UrlUtils::getActionDetails (
    const QString & url
) 
```



Returns QField action details extracted from a compatible _url_. 


        

<hr>



### function isRelativeOrFileUrl 

```C++
static Q_INVOKABLE bool UrlUtils::isRelativeOrFileUrl (
    const QString & url
) 
```



Checks whether the provided string is a relative _url_ (has no protocol or starts with `file://`). 


        

<hr>



### function toLocalFile 

_Returns a URL from a_ _url_ _with logic to presence or absence of_[file://](file://) _._
```C++
static Q_INVOKABLE QString UrlUtils::toLocalFile (
    const QString & url
) 
```




<hr>



### function urlDetail 

```C++
static Q_INVOKABLE QString UrlUtils::urlDetail (
    const QString & url,
    const QString & detail
) 
```



Returns a _detail_ from an _url_. The possible components are:
* "scheme", e.g. https
* "authority", e.g. qfield.org
* "path", e.g. /my/home.html
* "fileName", e.g. file.zip
* "query", e.g. param=true&other\_parem=0 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/urlutils.h`

