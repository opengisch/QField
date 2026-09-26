

# Class QfBookmarkLocatorFilter



[**ClassList**](annotated.md) **>** [**QfBookmarkLocatorFilter**](classQfBookmarkLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfbookmarklocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfBookmarkLocatorFilter**](#function-qfbookmarklocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfBookmarkLocatorFilter**](classQfBookmarkLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  QString | [**description**](#function-description) () override const<br> |
|  QString | [**displayName**](#function-displayname) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  QgsLocatorFilter::Flags | [**flags**](#function-flags) () override const<br> |
|  QString | [**name**](#function-name) () override const<br> |
|  QString | [**prefix**](#function-prefix) () override const<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**QfGotoLocatorFilter**](classQfGotoLocatorFilter.md) is a locator filter to search for and display coordinates. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum QfBookmarkLocatorFilter::ActionOrigin {
    Normal,
    QfNavigation
};
```




<hr>
## Public Functions Documentation




### function QfBookmarkLocatorFilter 

```C++
explicit QfBookmarkLocatorFilter::QfBookmarkLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfBookmarkLocatorFilter * QfBookmarkLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfBookmarkLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfBookmarkLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfBookmarkLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags QfBookmarkLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString QfBookmarkLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfBookmarkLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QfBookmarkLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void QfBookmarkLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfBookmarkLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfbookmarklocatorfilter.h`

