

# Class QfFeaturesLocatorFilter



[**ClassList**](annotated.md) **>** [**QfFeaturesLocatorFilter**](classQfFeaturesLocatorFilter.md)



[More...](#detailed-description)

* `#include <qffeatureslocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**PreparedLayer**](structQfFeaturesLocatorFilter_1_1PreparedLayer.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeaturesLocatorFilter**](#function-qffeatureslocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfFeaturesLocatorFilter**](classQfFeaturesLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
|  QString | [**description**](#function-description) () override const<br> |
|  QString | [**displayName**](#function-displayname) () override const<br> |
|  void | [**fetchResults**](#function-fetchresults) (const QString & string, const QgsLocatorContext & context, QgsFeedback \* feedback) override<br> |
|  QString | [**name**](#function-name) () override const<br> |
|  QString | [**prefix**](#function-prefix) () override const<br> |
|  QStringList | [**prepare**](#function-prepare) (const QString & string, const QgsLocatorContext & locatorContext) override<br> |
|  Priority | [**priority**](#function-priority) () override const<br> |
|  void | [**triggerResult**](#function-triggerresult) (const QgsLocatorResult & result) override<br> |
|  void | [**triggerResultFromAction**](#function-triggerresultfromaction) (const QgsLocatorResult & result, const int actionId) override<br> |




























## Detailed Description


[**QfFeaturesLocatorFilter**](classQfFeaturesLocatorFilter.md) is a locator filter to search for features across layers in the project. Reimplemented from QGIS code (app). 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum QfFeaturesLocatorFilter::ActionOrigin {
    Normal,
    OpenForm,
    QfNavigation
};
```




<hr>
## Public Functions Documentation




### function QfFeaturesLocatorFilter 

```C++
explicit QfFeaturesLocatorFilter::QfFeaturesLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfFeaturesLocatorFilter * QfFeaturesLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfFeaturesLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfFeaturesLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfFeaturesLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString QfFeaturesLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfFeaturesLocatorFilter::prefix () override const
```




<hr>



### function prepare 

```C++
QStringList QfFeaturesLocatorFilter::prepare (
    const QString & string,
    const QgsLocatorContext & locatorContext
) override
```




<hr>



### function priority 

```C++
inline Priority QfFeaturesLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void QfFeaturesLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfFeaturesLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qffeatureslocatorfilter.h`

