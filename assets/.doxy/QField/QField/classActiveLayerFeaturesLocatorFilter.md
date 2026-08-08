

# Class ActiveLayerFeaturesLocatorFilter



[**ClassList**](annotated.md) **>** [**ActiveLayerFeaturesLocatorFilter**](classActiveLayerFeaturesLocatorFilter.md)



[More...](#detailed-description)

* `#include <activelayerfeatureslocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |
| enum  | [**ResultType**](#enum-resulttype)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ActiveLayerFeaturesLocatorFilter**](#function-activelayerfeatureslocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**ActiveLayerFeaturesLocatorFilter**](classActiveLayerFeaturesLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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


[**FeaturesLocatorFilter**](classFeaturesLocatorFilter.md) is a locator filter to search for features across layers in the project. Reimplemented from QGIS code (app). 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum ActiveLayerFeaturesLocatorFilter::ActionOrigin {
    Normal,
    OpenForm,
    Navigation
};
```




<hr>



### enum ResultType 

```C++
enum ActiveLayerFeaturesLocatorFilter::ResultType {
    Feature,
    FieldRestriction
};
```




<hr>
## Public Functions Documentation




### function ActiveLayerFeaturesLocatorFilter 

```C++
explicit ActiveLayerFeaturesLocatorFilter::ActiveLayerFeaturesLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
ActiveLayerFeaturesLocatorFilter * ActiveLayerFeaturesLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString ActiveLayerFeaturesLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString ActiveLayerFeaturesLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void ActiveLayerFeaturesLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString ActiveLayerFeaturesLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString ActiveLayerFeaturesLocatorFilter::prefix () override const
```




<hr>



### function prepare 

```C++
QStringList ActiveLayerFeaturesLocatorFilter::prepare (
    const QString & string,
    const QgsLocatorContext & locatorContext
) override
```




<hr>



### function priority 

```C++
inline Priority ActiveLayerFeaturesLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void ActiveLayerFeaturesLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void ActiveLayerFeaturesLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/activelayerfeatureslocatorfilter.h`

