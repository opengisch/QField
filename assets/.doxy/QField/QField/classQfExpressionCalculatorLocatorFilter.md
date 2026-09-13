

# Class QfExpressionCalculatorLocatorFilter



[**ClassList**](annotated.md) **>** [**QfExpressionCalculatorLocatorFilter**](classQfExpressionCalculatorLocatorFilter.md)



[More...](#detailed-description)

* `#include <qfexpressioncalculatorlocatorfilter.h>`



Inherits the following classes: QgsLocatorFilter














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionOrigin**](#enum-actionorigin)  <br>_Origin of the action which triggers the result._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfExpressionCalculatorLocatorFilter**](#function-qfexpressioncalculatorlocatorfilter) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorBridge, QObject \* parent=nullptr) <br> |
|  [**QfExpressionCalculatorLocatorFilter**](classQfExpressionCalculatorLocatorFilter.md) \* | [**clone**](#function-clone) () override const<br> |
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


[**QfExpressionCalculatorLocatorFilter**](classQfExpressionCalculatorLocatorFilter.md) is a locator filter to type in expressions and copy their returned value. 


    
## Public Types Documentation




### enum ActionOrigin 

_Origin of the action which triggers the result._ 
```C++
enum QfExpressionCalculatorLocatorFilter::ActionOrigin {
    Normal
};
```




<hr>
## Public Functions Documentation




### function QfExpressionCalculatorLocatorFilter 

```C++
explicit QfExpressionCalculatorLocatorFilter::QfExpressionCalculatorLocatorFilter (
    QfLocatorModelSuperBridge * locatorBridge,
    QObject * parent=nullptr
) 
```




<hr>



### function clone 

```C++
QfExpressionCalculatorLocatorFilter * QfExpressionCalculatorLocatorFilter::clone () override const
```




<hr>



### function description 

```C++
inline QString QfExpressionCalculatorLocatorFilter::description () override const
```




<hr>



### function displayName 

```C++
inline QString QfExpressionCalculatorLocatorFilter::displayName () override const
```




<hr>



### function fetchResults 

```C++
void QfExpressionCalculatorLocatorFilter::fetchResults (
    const QString & string,
    const QgsLocatorContext & context,
    QgsFeedback * feedback
) override
```




<hr>



### function flags 

```C++
inline QgsLocatorFilter::Flags QfExpressionCalculatorLocatorFilter::flags () override const
```




<hr>



### function name 

```C++
inline QString QfExpressionCalculatorLocatorFilter::name () override const
```




<hr>



### function prefix 

```C++
inline QString QfExpressionCalculatorLocatorFilter::prefix () override const
```




<hr>



### function priority 

```C++
inline Priority QfExpressionCalculatorLocatorFilter::priority () override const
```




<hr>



### function triggerResult 

```C++
void QfExpressionCalculatorLocatorFilter::triggerResult (
    const QgsLocatorResult & result
) override
```




<hr>



### function triggerResultFromAction 

```C++
void QfExpressionCalculatorLocatorFilter::triggerResultFromAction (
    const QgsLocatorResult & result,
    const int actionId
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qfexpressioncalculatorlocatorfilter.h`

