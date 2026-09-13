

# Class QfActiveLayerFeaturesLocatorFilter



[**ClassList**](annotated.md) **>** [**QfActiveLayerFeaturesLocatorFilter**](classQfActiveLayerFeaturesLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfactivelayerfeatureslocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |
| enum  | [**ResultType**](#enum-resulttype)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfActiveLayerFeaturesLocatorFilter**](#function-qfactivelayerfeatureslocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfActiveLayerFeaturesLocatorFilter**](classQfActiveLayerFeaturesLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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
enum QfActiveLayerFeaturesLocatorFilter::ActionOrigin {
    Normal,
    OpenForm,
    QfNavigation
};
```




<hr>



### enum ResultType 

```C++
enum QfActiveLayerFeaturesLocatorFilter::ResultType {
    Feature,
    FieldRestriction
};
```




<hr>
## Public Functions Documentation




### function QfActiveLayerFeaturesLocatorFilter 

```C++
explicit QfActiveLayerFeaturesLocatorFilter::QfActiveLayerFeaturesLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfActiveLayerFeaturesLocatorFilter * QfActiveLayerFeaturesLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfActiveLayerFeaturesLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfActiveLayerFeaturesLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfActiveLayerFeaturesLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString QfActiveLayerFeaturesLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfActiveLayerFeaturesLocatorFilter::prefix () override const
```




<hr>



### function prepare 

```C++
QStringList QfActiveLayerFeaturesLocatorFilter::prepare (
    const QString & string,
    const QgsLocatorContext & locatorContext
) override
```




<hr>



### function priority 

```C++
inline Priority QfActiveLayerFeaturesLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void QfActiveLayerFeaturesLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfActiveLayerFeaturesLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfactivelayerfeatureslocatorfilter.h`

