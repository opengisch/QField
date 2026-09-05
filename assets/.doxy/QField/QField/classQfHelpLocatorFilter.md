

# Class QfHelpLocatorFilter



[**ClassList**](annotated.md) **>** [**QfHelpLocatorFilter**](classQfHelpLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfhelplocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfHelpLocatorFilter**](#function-qfhelplocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfHelpLocatorFilter**](classQfHelpLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  QString | [**description**](#function-description) () override const<br> |
|  QString | [**displayName**](#function-displayname) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  QString | [**name**](#function-name) () override const<br> |
|  QString | [**prefix**](#function-prefix) () override const<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  QString | [**searchUrl**](#function-searchurl) () const<br> |
|  void | [**setSearchUrl**](#function-setsearchurl) (const QString & url) <br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**QfHelpLocatorFilter**](classQfHelpLocatorFilter.md) is a locator filter to search for and display QField documentation pages. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum QfHelpLocatorFilter::ActionOrigin {
    Normal
};
```




<hr>
## Public Functions Documentation




### function QfHelpLocatorFilter 

```C++
explicit QfHelpLocatorFilter::QfHelpLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfHelpLocatorFilter * QfHelpLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfHelpLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfHelpLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfHelpLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString QfHelpLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfHelpLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QfHelpLocatorFilter::priority () override const
```




<hr>



### function searchUrl 

```C++
inline QString QfHelpLocatorFilter::searchUrl () const
```




<hr>



### function setSearchUrl 

```C++
inline void QfHelpLocatorFilter::setSearchUrl (
    const QString & url
) 
```




<hr>



### function triggerResult 

```C++
void QfHelpLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfHelpLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfhelplocatorfilter.h`

