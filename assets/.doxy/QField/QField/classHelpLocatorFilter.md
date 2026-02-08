

# Class HelpLocatorFilter



[**ClassList**](annotated.md) **>** [**HelpLocatorFilter**](classHelpLocatorFilter.md)



[More...](#detailed-description)

* `#include <helplocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**HelpLocatorFilter**](#function-helplocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**HelpLocatorFilter**](classHelpLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  QString | [**description**](#function-description) () override const<br> |
|  QString | [**displayName**](#function-displayname) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  QString | [**name**](#function-name) () override const<br> |
|  QString | [**prefix**](#function-prefix) () override const<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**HelpLocatorFilter**](classHelpLocatorFilter.md) is a locator filter to search for and display QField documentation pages. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum HelpLocatorFilter::ActionOrigin {
    Normal
};
```




<hr>
## Public Functions Documentation




### function HelpLocatorFilter 

```C++
explicit HelpLocatorFilter::HelpLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
HelpLocatorFilter * HelpLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString HelpLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString HelpLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void HelpLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString HelpLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString HelpLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority HelpLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void HelpLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void HelpLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/helplocatorfilter.h`

