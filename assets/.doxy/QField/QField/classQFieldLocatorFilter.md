

# Class QFieldLocatorFilter



[**ClassList**](annotated.md) **>** [**QFieldLocatorFilter**](classQFieldLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfieldlocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter


























## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**delay**](classQFieldLocatorFilter.md#property-delay-12)  <br> |
| property QString | [**description**](classQFieldLocatorFilter.md#property-description-12)  <br> |
| property QString | [**displayName**](classQFieldLocatorFilter.md#property-displayname-12)  <br> |
| property [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* | [**locatorBridge**](classQFieldLocatorFilter.md#property-locatorbridge-12)  <br> |
| property QString | [**name**](classQFieldLocatorFilter.md#property-name-12)  <br> |
| property QVariantMap | [**parameters**](classQFieldLocatorFilter.md#property-parameters-12)  <br> |
| property QString | [**prefix**](classQFieldLocatorFilter.md#property-prefix-12)  <br> |
| property QUrl | [**source**](classQFieldLocatorFilter.md#property-source-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**delayChanged**](classQFieldLocatorFilter.md#signal-delaychanged)  <br>_Emitted when the fetch result delay has changed._  |
| signal void | [**descriptionChanged**](classQFieldLocatorFilter.md#signal-descriptionchanged)  <br>_Emitted when the description has changed._  |
| signal void | [**displayNameChanged**](classQFieldLocatorFilter.md#signal-displaynamechanged)  <br>_Emitted when the display name has changed._  |
| signal void | [**locatorBridgeChanged**](classQFieldLocatorFilter.md#signal-locatorbridgechanged)  <br>_Emitted when the locator bridge has changed._  |
| signal void | [**nameChanged**](classQFieldLocatorFilter.md#signal-namechanged)  <br>_Emitted when the name has changed._  |
| signal void | [**parametersChanged**](classQFieldLocatorFilter.md#signal-parameterschanged)  <br>_Emitted when the parameters object has changed._  |
| signal void | [**prefixChanged**](classQFieldLocatorFilter.md#signal-prefixchanged)  <br>_Emitted when the prefix has changed._  |
| signal void | [**sourceChanged**](classQFieldLocatorFilter.md#signal-sourcechanged)  <br>_Emitted when the source has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QFieldLocatorFilter**](#function-qfieldlocatorfilter) (QObject \* parent=nullptr) <br> |
|  [**QFieldLocatorFilter**](classQFieldLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br>_Clone the locator filter._  |
|  int | [**delay**](#function-delay-22) () const<br>_Returns the delay before which the fetching of results is triggered._  |
|  QString | [**description**](#function-description-22) () override const<br> |
|  QString | [**displayName**](#function-displayname-22) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* | [**locatorBridge**](#function-locatorbridge-22) () const<br> |
|  QString | [**name**](#function-name-22) () override const<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () const<br> |
|  QString | [**prefix**](#function-prefix-22) () override const<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  void | [**setDelay**](#function-setdelay) (int delay) <br>_Sets the delay before which the fetching of results is triggered._  |
|  void | [**setDescription**](#function-setdescription) (const QString & description) <br> |
|  void | [**setDisplayName**](#function-setdisplayname) (const QString & displayName) <br> |
|  void | [**setLocatorBridge**](#function-setlocatorbridge) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |
|  void | [**setPrefix**](#function-setprefix) (const QString & prefix) <br> |
|  void | [**setSource**](#function-setsource) (const QUrl & source) <br> |
|  QUrl | [**source**](#function-source-22) () const<br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**QFieldLocatorFilter**](classQFieldLocatorFilter.md) is a locator filter item for QField plugins to integrate with locator searches. 


    
## Public Properties Documentation




### property delay [1/2]

```C++
int QFieldLocatorFilter::delay;
```




<hr>



### property description [1/2]

```C++
QString QFieldLocatorFilter::description;
```




<hr>



### property displayName [1/2]

```C++
QString QFieldLocatorFilter::displayName;
```




<hr>



### property locatorBridge [1/2]

```C++
LocatorModelSuperBridge * QFieldLocatorFilter::locatorBridge;
```




<hr>



### property name [1/2]

```C++
QString QFieldLocatorFilter::name;
```




<hr>



### property parameters [1/2]

```C++
QVariantMap QFieldLocatorFilter::parameters;
```




<hr>



### property prefix [1/2]

```C++
QString QFieldLocatorFilter::prefix;
```




<hr>



### property source [1/2]

```C++
QUrl QFieldLocatorFilter::source;
```




<hr>
## Public Signals Documentation




### signal delayChanged 

_Emitted when the fetch result delay has changed._ 
```C++
void QFieldLocatorFilter::delayChanged;
```




<hr>



### signal descriptionChanged 

_Emitted when the description has changed._ 
```C++
void QFieldLocatorFilter::descriptionChanged;
```




<hr>



### signal displayNameChanged 

_Emitted when the display name has changed._ 
```C++
void QFieldLocatorFilter::displayNameChanged;
```




<hr>



### signal locatorBridgeChanged 

_Emitted when the locator bridge has changed._ 
```C++
void QFieldLocatorFilter::locatorBridgeChanged;
```




<hr>



### signal nameChanged 

_Emitted when the name has changed._ 
```C++
void QFieldLocatorFilter::nameChanged;
```




<hr>



### signal parametersChanged 

_Emitted when the parameters object has changed._ 
```C++
void QFieldLocatorFilter::parametersChanged;
```




<hr>



### signal prefixChanged 

_Emitted when the prefix has changed._ 
```C++
void QFieldLocatorFilter::prefixChanged;
```




<hr>



### signal sourceChanged 

_Emitted when the source has changed._ 
```C++
void QFieldLocatorFilter::sourceChanged;
```




<hr>
## Public Functions Documentation




### function QFieldLocatorFilter 

```C++
explicit QFieldLocatorFilter::QFieldLocatorFilter (
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

_Clone the locator filter._ 
```C++
QFieldLocatorFilter * QFieldLocatorFilter::clone () override const
```




<hr>



### function delay [2/2]

_Returns the delay before which the fetching of results is triggered._ 
```C++
inline int QFieldLocatorFilter::delay () const
```




<hr>



### function description [2/2]

```C++
inline QString QFieldLocatorFilter::description () override const
```




<hr>



### function displayName [2/2]

```C++
inline QString QFieldLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QFieldLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function locatorBridge [2/2]

```C++
inline LocatorModelSuperBridge * QFieldLocatorFilter::locatorBridge () const
```



Returns the locator bridge object. 


        

<hr>



### function name [2/2]

```C++
inline QString QFieldLocatorFilter::name () override const
```




<hr>



### function parameters [2/2]

```C++
inline QVariantMap QFieldLocatorFilter::parameters () const
```



Returns additional locator filter parameters which will possed onto the source QML that will process the locator filter results. 


        

<hr>



### function prefix [2/2]

```C++
inline QString QFieldLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QFieldLocatorFilter::priority () override const
```




<hr>



### function setDelay 

_Sets the delay before which the fetching of results is triggered._ 
```C++
void QFieldLocatorFilter::setDelay (
    int delay
) 
```




<hr>



### function setDescription 

```C++
void QFieldLocatorFilter::setDescription (
    const QString & description
) 
```



Sets a description for the filter. 


        

<hr>



### function setDisplayName 

```C++
void QFieldLocatorFilter::setDisplayName (
    const QString & displayName
) 
```



Sets a translated, user-friendly name for the filter. 


        

<hr>



### function setLocatorBridge 

```C++
void QFieldLocatorFilter::setLocatorBridge (
    LocatorModelSuperBridge * locatorBridge
) 
```



Sets the locator bridge object. 


        

<hr>



### function setName 

```C++
void QFieldLocatorFilter::setName (
    const QString & name
) 
```



Sets the unique name of the filter. This should be an untranslated string identifying the filter. 


        

<hr>



### function setParameters 

```C++
void QFieldLocatorFilter::setParameters (
    const QVariantMap & parameters
) 
```



Sets additional locator filter parameters which will possed onto the source QML that will process the locator filter results. 


        

<hr>



### function setPrefix 

```C++
void QFieldLocatorFilter::setPrefix (
    const QString & prefix
) 
```



Sets the search prefix character(s) for this filter. Prefix a search with these characters will restrict the locator search to only include results from this filter. 

**Note:**

The prefix must be &gt;= 3 characters otherwise it will be ignored. 





        

<hr>



### function setSource 

```C++
void QFieldLocatorFilter::setSource (
    const QUrl & source
) 
```



Setsthe source QML file which will process the locator filter results. 


        

<hr>



### function source [2/2]

```C++
inline QUrl QFieldLocatorFilter::source () const
```



Returns the source QML file which will process the locator filter results. 


        

<hr>



### function triggerResult 

```C++
void QFieldLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QFieldLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfieldlocatorfilter.h`

