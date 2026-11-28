

# Class QfNewsParser



[**ClassList**](annotated.md) **>** [**QfNewsParser**](classQfNewsParser.md)



_A class fetching RSS feeds and generating news items published within the last MAXIMUM\_AGE\_DAYS days, minus those already dismissed._ 

* `#include <qfnewsparser.h>`



Inherits the following classes: QObject




















## Public Static Attributes

| Type | Name |
| ---: | :--- |
|  const QString | [**DISMISSED\_UUIDS\_SETTINGS\_KEY**](#variable-dismissed_uuids_settings_key)   = `QStringLiteral( "/QField/News/DismissedUuids" )`<br>_Dismissed news items settings key (exposed for testing purpose)_  |






## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classQfNewsParser.md#property-enabled-12)  <br> |
| property QUrl | [**feedUrl**](classQfNewsParser.md#property-feedurl-12)  <br> |
| property bool | [**isFetching**](classQfNewsParser.md#property-isfetching-12)  <br> |
| property QList&lt; [**QfNewsItem**](classQfNewsItem.md) &gt; | [**items**](classQfNewsParser.md#property-items-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfNewsParser.md#signal-enabledchanged)  <br>_Emitted when news have been enabled or disabled._  |
| signal void | [**feedUrlChanged**](classQfNewsParser.md#signal-feedurlchanged)  <br>_Emitted when the RSS feed URL has changed._  |
| signal void | [**isFetchingChanged**](classQfNewsParser.md#signal-isfetchingchanged)  <br>_Emitted when a fetching operation has begun or ended._  |
| signal void | [**itemsChanged**](classQfNewsParser.md#signal-itemschanged)  <br>_Emitted when the list of news items has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNewsParser**](#function-qfnewsparser) (QObject \* parent=nullptr) <br>_The news parser constructor._  |
|  Q\_INVOKABLE void | [**dismiss**](#function-dismiss) (const QString & uuid) <br> |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  QUrl | [**feedUrl**](#function-feedurl-22) () const<br>_Returns the RSS feed URL._  |
|  Q\_INVOKABLE void | [**fetch**](#function-fetch) () <br> |
|  bool | [**isFetching**](#function-isfetching-22) () const<br>_Returns TRUE while the RSS feed is being fetched._  |
|  QList&lt; [**QfNewsItem**](classQfNewsItem.md) &gt; | [**items**](#function-items-22) () const<br>_Returns the undismissed news items published within the last MAXIMUM\_AGE\_DAYS days, most recent first._  |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setFeedUrl**](#function-setfeedurl) (const QUrl & url) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QList&lt; [**QfNewsItem**](classQfNewsItem.md) &gt; | [**parseFeed**](#function-parsefeed) (const QByteArray & data) <br> |


























## Public Static Attributes Documentation




### variable DISMISSED\_UUIDS\_SETTINGS\_KEY 

_Dismissed news items settings key (exposed for testing purpose)_ 
```C++
const QString QfNewsParser::DISMISSED_UUIDS_SETTINGS_KEY;
```




<hr>
## Public Properties Documentation




### property enabled [1/2]

```C++
bool QfNewsParser::enabled;
```




<hr>



### property feedUrl [1/2]

```C++
QUrl QfNewsParser::feedUrl;
```




<hr>



### property isFetching [1/2]

```C++
bool QfNewsParser::isFetching;
```




<hr>



### property items [1/2]

```C++
QList<QfNewsItem> QfNewsParser::items;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

_Emitted when news have been enabled or disabled._ 
```C++
void QfNewsParser::enabledChanged;
```




<hr>



### signal feedUrlChanged 

_Emitted when the RSS feed URL has changed._ 
```C++
void QfNewsParser::feedUrlChanged;
```




<hr>



### signal isFetchingChanged 

_Emitted when a fetching operation has begun or ended._ 
```C++
void QfNewsParser::isFetchingChanged;
```




<hr>



### signal itemsChanged 

_Emitted when the list of news items has changed._ 
```C++
void QfNewsParser::itemsChanged;
```




<hr>
## Public Functions Documentation




### function QfNewsParser 

_The news parser constructor._ 
```C++
explicit QfNewsParser::QfNewsParser (
    QObject * parent=nullptr
) 
```




<hr>



### function dismiss 

```C++
Q_INVOKABLE void QfNewsParser::dismiss (
    const QString & uuid
) 
```



Dismisses the news item matching a given _uuid_, it will no longer be part of the news items. 


        

<hr>



### function enabled [2/2]

```C++
bool QfNewsParser::enabled () const
```



Returns TRUE if the user has opted into receiving news. 


        

<hr>



### function feedUrl [2/2]

_Returns the RSS feed URL._ 
```C++
inline QUrl QfNewsParser::feedUrl () const
```




<hr>



### function fetch 

```C++
Q_INVOKABLE void QfNewsParser::fetch () 
```



Fetches the RSS feed and refreshes the news items. 

**Note:**

Nothing will be fetched when news are disabled. 





        

<hr>



### function isFetching [2/2]

_Returns TRUE while the RSS feed is being fetched._ 
```C++
inline bool QfNewsParser::isFetching () const
```




<hr>



### function items [2/2]

_Returns the undismissed news items published within the last MAXIMUM\_AGE\_DAYS days, most recent first._ 
```C++
inline QList< QfNewsItem > QfNewsParser::items () const
```




<hr>



### function setEnabled 

```C++
void QfNewsParser::setEnabled (
    bool enabled
) 
```



Sets whether the user has opted into receiving news. 


        

<hr>



### function setFeedUrl 

```C++
void QfNewsParser::setFeedUrl (
    const QUrl & url
) 
```



Sets the RSS feed URL. News items are cleared and, if available, restored from the last successful fetch of that feed until the next [**fetch()**](classQfNewsParser.md#function-fetch) call. 


        

<hr>
## Public Static Functions Documentation




### function parseFeed 

```C++
static QList< QfNewsItem > QfNewsParser::parseFeed (
    const QByteArray & data
) 
```



Parses an RSS feed _data_ content and returns its news items. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfnewsparser.h`

