

# Class QfFeatureGatherer



[**ClassList**](annotated.md) **>** [**QfFeatureGatherer**](classQfFeatureGatherer.md)








Inherits the following classes: QThread






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collectedValues**](classQfFeatureGatherer.md#signal-collectedvalues)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureGatherer**](#function-qffeaturegatherer) (QgsFeature feature, QgsRelation & relation, const QgsRelation & nmRelation=QgsRelation()) <br> |
|  QList&lt; QfReferencingFeatureListModelBase::Entry &gt; | [**entries**](#function-entries) () const<br> |
|  void | [**run**](#function-run) () override<br> |
|  void | [**stop**](#function-stop) () <br>_Informs the gatherer to immediately stop collecting values._  |
|  bool | [**wasCanceled**](#function-wascanceled) () const<br> |




























## Public Signals Documentation




### signal collectedValues 

```C++
void QfFeatureGatherer::collectedValues;
```



Emitted when values have been collected 

**Parameters:**


* `values` list of unique matching string values 




        

<hr>
## Public Functions Documentation




### function QfFeatureGatherer 

```C++
inline QfFeatureGatherer::QfFeatureGatherer (
    QgsFeature feature,
    QgsRelation & relation,
    const QgsRelation & nmRelation=QgsRelation()
) 
```




<hr>



### function entries 

```C++
inline QList< QfReferencingFeatureListModelBase::Entry > QfFeatureGatherer::entries () const
```





**Returns:**

the list of entries 





        

<hr>



### function run 

```C++
inline void QfFeatureGatherer::run () override
```




<hr>



### function stop 

_Informs the gatherer to immediately stop collecting values._ 
```C++
inline void QfFeatureGatherer::stop () 
```




<hr>



### function wasCanceled 

```C++
inline bool QfFeatureGatherer::wasCanceled () const
```





**Returns:**

true if collection was canceled before completion 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfreferencingfeaturelistmodel.h`

