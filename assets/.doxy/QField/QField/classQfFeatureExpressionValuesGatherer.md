

# Class QfFeatureExpressionValuesGatherer



[**ClassList**](annotated.md) **>** [**QfFeatureExpressionValuesGatherer**](classQfFeatureExpressionValuesGatherer.md)



[More...](#detailed-description)

* `#include <qffeatureexpressionvaluesgatherer.h>`



Inherits the following classes: QThread












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Entry**](structQfFeatureExpressionValuesGatherer_1_1Entry.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureExpressionValuesGatherer**](#function-qffeatureexpressionvaluesgatherer) (QgsVectorLayer \* layer, const QString & displayExpression=QString(), const QgsFeatureRequest & request=QgsFeatureRequest(), const QStringList & identifierFields=QStringList()) <br> |
|  QVariant | [**data**](#function-data) () const<br> |
|  QVector&lt; [**Entry**](structQfFeatureExpressionValuesGatherer_1_1Entry.md) &gt; | [**entries**](#function-entries) () const<br> |
|  QgsFeatureRequest | [**request**](#function-request) () const<br> |
|  void | [**run**](#function-run) () override<br> |
|  void | [**setData**](#function-setdata) (const QVariant & data) <br> |
|  void | [**stop**](#function-stop) () <br>_Informs the gatherer to immediately stop collecting values._  |
|  bool | [**wasCanceled**](#function-wascanceled) () const<br>_Returns TRUE if collection was canceled before completion._  |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  [**Entry**](structQfFeatureExpressionValuesGatherer_1_1Entry.md) | [**nullEntry**](#function-nullentry) (QgsVectorLayer \* layer) <br> |






## Protected Attributes

| Type | Name |
| ---: | :--- |
|  QVector&lt; [**Entry**](structQfFeatureExpressionValuesGatherer_1_1Entry.md) &gt; | [**mEntries**](#variable-mentries)  <br> |




















## Detailed Description


Gathers features with substring matching on an expression. 

**Note:**

This is a is an exact copy of QGIS' QgsFeatureExpressionValuesGatherer 





    
## Public Functions Documentation




### function QfFeatureExpressionValuesGatherer 

```C++
inline explicit QfFeatureExpressionValuesGatherer::QfFeatureExpressionValuesGatherer (
    QgsVectorLayer * layer,
    const QString & displayExpression=QString(),
    const QgsFeatureRequest & request=QgsFeatureRequest(),
    const QStringList & identifierFields=QStringList()
) 
```



Constructor 

**Parameters:**


* `layer` the vector layer 
* `displayExpression` if empty, the display expression is taken from the layer definition 
* `request` the request to perform 
* `identifierFields` an optional list of fields name to be save in a variant list for an easier reuse 




        

<hr>



### function data 

```C++
inline QVariant QfFeatureExpressionValuesGatherer::data () const
```



Internal data, use for whatever you want. 


        

<hr>



### function entries 

```C++
inline QVector< Entry > QfFeatureExpressionValuesGatherer::entries () const
```




<hr>



### function request 

```C++
inline QgsFeatureRequest QfFeatureExpressionValuesGatherer::request () const
```




<hr>



### function run 

```C++
inline void QfFeatureExpressionValuesGatherer::run () override
```




<hr>



### function setData 

```C++
inline void QfFeatureExpressionValuesGatherer::setData (
    const QVariant & data
) 
```



Internal data, use for whatever you want. 


        

<hr>



### function stop 

_Informs the gatherer to immediately stop collecting values._ 
```C++
inline void QfFeatureExpressionValuesGatherer::stop () 
```




<hr>



### function wasCanceled 

_Returns TRUE if collection was canceled before completion._ 
```C++
inline bool QfFeatureExpressionValuesGatherer::wasCanceled () const
```




<hr>
## Public Static Functions Documentation




### function nullEntry 

```C++
static inline Entry QfFeatureExpressionValuesGatherer::nullEntry (
    QgsVectorLayer * layer
) 
```




<hr>
## Protected Attributes Documentation




### variable mEntries 

```C++
QVector<Entry> QfFeatureExpressionValuesGatherer::mEntries;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeatureexpressionvaluesgatherer.h`

