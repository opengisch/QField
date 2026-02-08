

# Class FeatureIterator



[**ClassList**](annotated.md) **>** [**FeatureIterator**](classFeatureIterator.md)



[More...](#detailed-description)

* `#include <layerutils.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureIterator**](#function-featureiterator) (QgsVectorLayer \* layer=nullptr, const QgsFeatureRequest & request=QgsFeatureRequest()) <br> |
|  Q\_INVOKABLE void | [**close**](#function-close) () <br> |
|  Q\_INVOKABLE bool | [**hasNext**](#function-hasnext) () <br> |
|  Q\_INVOKABLE QgsFeature | [**next**](#function-next) () <br> |




























## Detailed Description


A class providing a feature iterator interface to be used within QML/javascript environment.


Users of this class must manually call its close() once feature iteration is finished. 


    
## Public Functions Documentation




### function FeatureIterator 

```C++
inline FeatureIterator::FeatureIterator (
    QgsVectorLayer * layer=nullptr,
    const QgsFeatureRequest & request=QgsFeatureRequest()
) 
```




<hr>



### function close 

```C++
inline Q_INVOKABLE void FeatureIterator::close () 
```




<hr>



### function hasNext 

```C++
inline Q_INVOKABLE bool FeatureIterator::hasNext () 
```




<hr>



### function next 

```C++
inline Q_INVOKABLE QgsFeature FeatureIterator::next () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/layerutils.h`

