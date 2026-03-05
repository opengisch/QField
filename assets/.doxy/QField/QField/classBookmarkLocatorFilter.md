

# Class BookmarkLocatorFilter



[**ClassList**](annotated.md) **>** [**BookmarkLocatorFilter**](classBookmarkLocatorFilter.md)



[More...](#detailed-description)

* `#include <bookmarklocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**BookmarkLocatorFilter**](#function-bookmarklocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**BookmarkLocatorFilter**](classBookmarkLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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


[**GotoLocatorFilter**](classGotoLocatorFilter.md) is a locator filter to search for and display coordinates. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum BookmarkLocatorFilter::ActionOrigin {
    Normal
};
```




<hr>
## Public Functions Documentation




### function BookmarkLocatorFilter 

```C++
explicit BookmarkLocatorFilter::BookmarkLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
BookmarkLocatorFilter * BookmarkLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString BookmarkLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString BookmarkLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void BookmarkLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags BookmarkLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString BookmarkLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString BookmarkLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority BookmarkLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void BookmarkLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void BookmarkLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/bookmarklocatorfilter.h`

