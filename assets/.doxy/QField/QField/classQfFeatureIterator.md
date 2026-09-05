

# Class QfFeatureIterator



[**ClassList**](annotated.md) **>** [**QfFeatureIterator**](classQfFeatureIterator.md)



[More...](#detailed-description)

* `#include <qflayerutils.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureIterator**](#function-qffeatureiterator) (QgsVectorLayer \* layer=nullptr, const QgsFeatureRequest & request=QgsFeatureRequest()) <br> |
|  Q\_INVOKABLE void | [**close**](#function-close) () <br> |
|  Q\_INVOKABLE bool | [**hasNext**](#function-hasnext) () <br> |
|  Q\_INVOKABLE QgsFeature | [**next**](#function-next) () <br> |




























## Detailed Description


A class providing a feature iterator interface to be used within QML/javascript environment.


Users of this class must manually call its close() once feature iteration is finished. 


    
## Public Functions Documentation




### function QfFeatureIterator 

```C++
inline QfFeatureIterator::QfFeatureIterator (
    QgsVectorLayer * layer=nullptr,
    const QgsFeatureRequest & request=QgsFeatureRequest()
) 
```




<hr>



### function close 

```C++
inline Q_INVOKABLE void QfFeatureIterator::close () 
```




<hr>



### function hasNext 

```C++
inline Q_INVOKABLE bool QfFeatureIterator::hasNext () 
```




<hr>



### function next 

```C++
inline Q_INVOKABLE QgsFeature QfFeatureIterator::next () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qflayerutils.h`

