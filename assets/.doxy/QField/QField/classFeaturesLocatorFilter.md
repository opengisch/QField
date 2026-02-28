

# Class FeaturesLocatorFilter



[**ClassList**](annotated.md) **>** [**FeaturesLocatorFilter**](classFeaturesLocatorFilter.md)



[More...](#detailed-description)

* `#include <featureslocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**PreparedLayer**](structFeaturesLocatorFilter_1_1PreparedLayer.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeaturesLocatorFilter**](#function-featureslocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**FeaturesLocatorFilter**](classFeaturesLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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
enum FeaturesLocatorFilter::ActionOrigin {
    Normal,
    OpenForm,
    Navigation
};
```




<hr>
## Public Functions Documentation




### function FeaturesLocatorFilter 

```C++
explicit FeaturesLocatorFilter::FeaturesLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
FeaturesLocatorFilter * FeaturesLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString FeaturesLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString FeaturesLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void FeaturesLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function name 

```C++
inline QString FeaturesLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString FeaturesLocatorFilter::prefix () override const
```




<hr>



### function prepare 

```C++
QStringList FeaturesLocatorFilter::prepare (
    const QString & string,
    const QgsLocatorContext & locatorContext
) override
```




<hr>



### function priority 

```C++
inline Priority FeaturesLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void FeaturesLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void FeaturesLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/featureslocatorfilter.h`

