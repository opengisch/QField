

# Class FeatureGatherer



[**ClassList**](annotated.md) **>** [**FeatureGatherer**](classFeatureGatherer.md)








Inherits the following classes: QThread






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collectedValues**](classFeatureGatherer.md#signal-collectedvalues)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureGatherer**](#function-featuregatherer) (QgsFeature feature, QgsRelation & relation, const QgsRelation & nmRelation=QgsRelation()) <br> |
|  QList&lt; ReferencingFeatureListModelBase::Entry &gt; | [**entries**](#function-entries) () const<br> |
|  void | [**run**](#function-run) () override<br> |
|  void | [**stop**](#function-stop) () <br>_Informs the gatherer to immediately stop collecting values._  |
|  bool | [**wasCanceled**](#function-wascanceled) () const<br> |




























## Public Signals Documentation




### signal collectedValues 

```C++
void FeatureGatherer::collectedValues;
```



Emitted when values have been collected 

**Parameters:**


* `values` list of unique matching string values 




        

<hr>
## Public Functions Documentation




### function FeatureGatherer 

```C++
inline FeatureGatherer::FeatureGatherer (
    QgsFeature feature,
    QgsRelation & relation,
    const QgsRelation & nmRelation=QgsRelation()
) 
```




<hr>



### function entries 

```C++
inline QList< ReferencingFeatureListModelBase::Entry > FeatureGatherer::entries () const
```





**Returns:**

the list of entries 





        

<hr>



### function run 

```C++
inline void FeatureGatherer::run () override
```




<hr>



### function stop 

_Informs the gatherer to immediately stop collecting values._ 
```C++
inline void FeatureGatherer::stop () 
```




<hr>



### function wasCanceled 

```C++
inline bool FeatureGatherer::wasCanceled () const
```





**Returns:**

true if collection was canceled before completion 





        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/referencingfeaturelistmodel.h`

