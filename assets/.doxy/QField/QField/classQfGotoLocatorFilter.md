

# Class QfGotoLocatorFilter



[**ClassList**](annotated.md) **>** [**QfGotoLocatorFilter**](classQfGotoLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfgotolocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGotoLocatorFilter**](#function-qfgotolocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfGotoLocatorFilter**](classQfGotoLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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
enum QfGotoLocatorFilter::ActionOrigin {
    Normal,
    QfNavigation
};
```




<hr>
## Public Functions Documentation




### function QfGotoLocatorFilter 

```C++
explicit QfGotoLocatorFilter::QfGotoLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfGotoLocatorFilter * QfGotoLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfGotoLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfGotoLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfGotoLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags QfGotoLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString QfGotoLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfGotoLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QfGotoLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void QfGotoLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfGotoLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfgotolocatorfilter.h`

