

# Class GotoLocatorFilter



[**ClassList**](annotated.md) **>** [**GotoLocatorFilter**](classGotoLocatorFilter.md)



[More...](#detailed-description)

* `#include <gotolocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GotoLocatorFilter**](#function-gotolocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**GotoLocatorFilter**](classGotoLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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
enum GotoLocatorFilter::ActionOrigin {
    Normal,
    Navigation
};
```




<hr>
## Public Functions Documentation




### function GotoLocatorFilter 

```C++
explicit GotoLocatorFilter::GotoLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
GotoLocatorFilter * GotoLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString GotoLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString GotoLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void GotoLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags GotoLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString GotoLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString GotoLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority GotoLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void GotoLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void GotoLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/gotolocatorfilter.h`

