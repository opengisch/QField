

# Class ExpressionCalculatorLocatorFilter



[**ClassList**](annotated.md) **>** [**ExpressionCalculatorLocatorFilter**](classExpressionCalculatorLocatorFilter.md)



[More...](#detailed-description)

* `#include <expressioncalculatorlocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionCalculatorLocatorFilter**](#function-expressioncalculatorlocatorfilter) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**ExpressionCalculatorLocatorFilter**](classExpressionCalculatorLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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


[**ExpressionCalculatorLocatorFilter**](classExpressionCalculatorLocatorFilter.md) is a locator filter to type in expressions and copy their returned value. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum ExpressionCalculatorLocatorFilter::ActionOrigin {
    Normal
};
```




<hr>
## Public Functions Documentation




### function ExpressionCalculatorLocatorFilter 

```C++
explicit ExpressionCalculatorLocatorFilter::ExpressionCalculatorLocatorFilter (
    LocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
ExpressionCalculatorLocatorFilter * ExpressionCalculatorLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString ExpressionCalculatorLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString ExpressionCalculatorLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void ExpressionCalculatorLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags ExpressionCalculatorLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString ExpressionCalculatorLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString ExpressionCalculatorLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority ExpressionCalculatorLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void ExpressionCalculatorLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void ExpressionCalculatorLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/expressioncalculatorlocatorfilter.h`

