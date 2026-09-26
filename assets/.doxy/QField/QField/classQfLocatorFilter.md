

# Class QfLocatorFilter



[**ClassList**](annotated.md) **>** [**QfLocatorFilter**](classQfLocatorFilter.md)



[More...](#detailed-description)

* `#include <qflocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**delay**](classQfLocatorFilter.md#property-delay-12)  <br> |
| property QString | [**description**](classQfLocatorFilter.md#property-description-12)  <br> |
| property QString | [**displayName**](classQfLocatorFilter.md#property-displayname-12)  <br> |
| property [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* | [**locatorBridge**](classQfLocatorFilter.md#property-locatorbridge-12)  <br> |
| property QString | [**name**](classQfLocatorFilter.md#property-name-12)  <br> |
| property QVariantMap | [**parameters**](classQfLocatorFilter.md#property-parameters-12)  <br> |
| property QString | [**prefix**](classQfLocatorFilter.md#property-prefix-12)  <br> |
| property QUrl | [**source**](classQfLocatorFilter.md#property-source-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**delayChanged**](classQfLocatorFilter.md#signal-delaychanged)  <br>_Emitted when the fetch result delay has changed._  |
| signal void | [**descriptionChanged**](classQfLocatorFilter.md#signal-descriptionchanged)  <br>_Emitted when the description has changed._  |
| signal void | [**displayNameChanged**](classQfLocatorFilter.md#signal-displaynamechanged)  <br>_Emitted when the display name has changed._  |
| signal void | [**locatorBridgeChanged**](classQfLocatorFilter.md#signal-locatorbridgechanged)  <br>_Emitted when the locator bridge has changed._  |
| signal void | [**nameChanged**](classQfLocatorFilter.md#signal-namechanged)  <br>_Emitted when the name has changed._  |
| signal void | [**parametersChanged**](classQfLocatorFilter.md#signal-parameterschanged)  <br>_Emitted when the parameters object has changed._  |
| signal void | [**prefixChanged**](classQfLocatorFilter.md#signal-prefixchanged)  <br>_Emitted when the prefix has changed._  |
| signal void | [**sourceChanged**](classQfLocatorFilter.md#signal-sourcechanged)  <br>_Emitted when the source has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocatorFilter**](#function-qflocatorfilter) (QObject \* parent=nullptr) <br> |
|  [**QfLocatorFilter**](classQfLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br>_Clone the locator filter._  |
|  int | [**delay**](#function-delay-22) () const<br>_Returns the delay before which the fetching of results is triggered._  |
|  QString | [**description**](#function-description-22) () override const<br> |
|  QString | [**displayName**](#function-displayname-22) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* | [**locatorBridge**](#function-locatorbridge-22) () const<br> |
|  QString | [**name**](#function-name-22) () override const<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () const<br> |
|  QString | [**prefix**](#function-prefix-22) () override const<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  void | [**setDelay**](#function-setdelay) (int delay) <br>_Sets the delay before which the fetching of results is triggered._  |
|  void | [**setDescription**](#function-setdescription) (const QString & description) <br> |
|  void | [**setDisplayName**](#function-setdisplayname) (const QString & displayName) <br> |
|  void | [**setLocatorBridge**](#function-setlocatorbridge) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |
|  void | [**setPrefix**](#function-setprefix) (const QString & prefix) <br> |
|  void | [**setSource**](#function-setsource) (const QUrl & source) <br> |
|  QUrl | [**source**](#function-source-22) () const<br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**QfLocatorFilter**](classQfLocatorFilter.md) is a locator filter item for QField plugins to integrate with locator searches. 


    
## Public Properties Documentation




### property delay [1/2]

```C++
int QfLocatorFilter::delay;
```




<hr>



### property description [1/2]

```C++
QString QfLocatorFilter::description;
```




<hr>



### property displayName [1/2]

```C++
QString QfLocatorFilter::displayName;
```




<hr>



### property locatorBridge [1/2]

```C++
QfLocatorModelSuperBridge * QfLocatorFilter::locatorBridge;
```




<hr>



### property name [1/2]

```C++
QString QfLocatorFilter::name;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap QfLocatorFilter::parameters;
```




<hr>



### property prefix [1/2]

```C++
QString QfLocatorFilter::prefix;
```




<hr>



### property source [1/2]

```C++
QUrl QfLocatorFilter::source;
```




<hr>
## Public Signals Documentation




### signal delayChanged 

_Emitted when the fetch result delay has changed._ 
```C++
void QfLocatorFilter::delayChanged;
```




<hr>



### signal descriptionChanged 

_Emitted when the description has changed._ 
```C++
void QfLocatorFilter::descriptionChanged;
```




<hr>



### signal displayNameChanged 

_Emitted when the display name has changed._ 
```C++
void QfLocatorFilter::displayNameChanged;
```




<hr>



### signal locatorBridgeChanged 

_Emitted when the locator bridge has changed._ 
```C++
void QfLocatorFilter::locatorBridgeChanged;
```




<hr>



### signal nameChanged 

_Emitted when the name has changed._ 
```C++
void QfLocatorFilter::nameChanged;
```




<hr>



### signal parametersChanged 

_Emitted when the parameters object has changed._ 
```C++
void QfLocatorFilter::parametersChanged;
```




<hr>



### signal prefixChanged 

_Emitted when the prefix has changed._ 
```C++
void QfLocatorFilter::prefixChanged;
```




<hr>



### signal sourceChanged 

_Emitted when the source has changed._ 
```C++
void QfLocatorFilter::sourceChanged;
```




<hr>
## Public Functions Documentation




### function QfLocatorFilter 

```C++
explicit QfLocatorFilter::QfLocatorFilter (
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

_Clone the locator filter._ 
```C++
QfLocatorFilter * QfLocatorFilter::clone () override const
```




<hr>



### function delay [2/2]

_Returns the delay before which the fetching of results is triggered._ 
```C++
inline int QfLocatorFilter::delay () const
```




<hr>



### function description [2/2]

```C++
inline QString QfLocatorFilter::description () override const
```




<hr>



### function displayName [2/2]

```C++
inline QString QfLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function locatorBridge [2/2]

```C++
inline QfLocatorModelSuperBridge * QfLocatorFilter::locatorBridge () const
```



Returns the locator bridge object. 


        

<hr>



### function name [2/2]

```C++
inline QString QfLocatorFilter::name () override const
```




<hr>



### function parameters [2/2]

```C++
inline QVariantMap QfLocatorFilter::parameters () const
```



Returns additional locator filter parameters which will possed onto the source QML that will process the locator filter results. 


        

<hr>



### function prefix [2/2]

```C++
inline QString QfLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QfLocatorFilter::priority () override const
```




<hr>



### function setDelay 

_Sets the delay before which the fetching of results is triggered._ 
```C++
void QfLocatorFilter::setDelay (
    int delay
) 
```




<hr>



### function setDescription 

```C++
void QfLocatorFilter::setDescription (
    const QString & description
) 
```



Sets a description for the filter. 


        

<hr>



### function setDisplayName 

```C++
void QfLocatorFilter::setDisplayName (
    const QString & displayName
) 
```



Sets a translated, user-friendly name for the filter. 


        

<hr>



### function setLocatorBridge 

```C++
void QfLocatorFilter::setLocatorBridge (
    QfLocatorModelSuperBridge * locatorBridge
) 
```



Sets the locator bridge object. 


        

<hr>



### function setName 

```C++
void QfLocatorFilter::setName (
    const QString & name
) 
```



Sets the unique name of the filter. This should be an untranslated string identifying the filter. 


        

<hr>



### function setParameters 

```C++
void QfLocatorFilter::setParameters (
    const QVariantMap & parameters
) 
```



Sets additional locator filter parameters which will possed onto the source QML that will process the locator filter results. 


        

<hr>



### function setPrefix 

```C++
void QfLocatorFilter::setPrefix (
    const QString & prefix
) 
```



Sets the search prefix character(s) for this filter. Prefix a search with these characters will restrict the locator search to only include results from this filter. 

**Note:**

The prefix must be &gt;= 3 characters otherwise it will be ignored. 





        

<hr>



### function setSource 

```C++
void QfLocatorFilter::setSource (
    const QUrl & source
) 
```



Setsthe source QML file which will process the locator filter results. 


        

<hr>



### function source [2/2]

```C++
inline QUrl QfLocatorFilter::source () const
```



Returns the source QML file which will process the locator filter results. 


        

<hr>



### function triggerResult 

```C++
void QfLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qflocatorfilter.h`

