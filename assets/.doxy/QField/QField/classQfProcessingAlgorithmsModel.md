

# Class QfProcessingAlgorithmsModel



[**ClassList**](annotated.md) **>** [**QfProcessingAlgorithmsModel**](classQfProcessingAlgorithmsModel.md)



_A sort/filter proxy model for providers and algorithms available within QField which automatically sorts the toolbox in a logical fashion and supports filtering the results._ 

* `#include <qfprocessingalgorithmsmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Filter**](#enum-filter)  <br>_Available filter flags for filtering the model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QfProcessingAlgorithmsModel::Filters | [**filters**](classQfProcessingAlgorithmsModel.md#property-filters-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classQfProcessingAlgorithmsModel.md#property-inplacelayer-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filtersChanged**](classQfProcessingAlgorithmsModel.md#signal-filterschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classQfProcessingAlgorithmsModel.md#signal-inplacelayerchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfProcessingAlgorithmsModel**](#function-qfprocessingalgorithmsmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  QfProcessingAlgorithmsModel::Filters | [**filters**](#function-filters-22) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer-22) () const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |
|  Q\_INVOKABLE void | [**rebuild**](#function-rebuild) () <br>_Rebuilds the algorithms model._  |
|  void | [**setFilters**](#function-setfilters) (QfProcessingAlgorithmsModel::Filters filters) <br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |




























## Public Types Documentation




### enum Filter 

_Available filter flags for filtering the model._ 
```C++
enum QfProcessingAlgorithmsModel::Filter {
    InPlaceFilter = 1 << 1,
    FavoriteFilter = 1 << 2
};
```




<hr>
## Public Properties Documentation




### property filters [1/2]

```C++
QfProcessingAlgorithmsModel::Filters QfProcessingAlgorithmsModel::filters;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * QfProcessingAlgorithmsModel::inPlaceLayer;
```




<hr>
## Public Signals Documentation




### signal filtersChanged 

```C++
void QfProcessingAlgorithmsModel::filtersChanged;
```



Emitted when the active filters have changed 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void QfProcessingAlgorithmsModel::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>
## Public Functions Documentation




### function QfProcessingAlgorithmsModel 

```C++
explicit QfProcessingAlgorithmsModel::QfProcessingAlgorithmsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function filterAcceptsRow 

```C++
bool QfProcessingAlgorithmsModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

```C++
inline QfProcessingAlgorithmsModel::Filters QfProcessingAlgorithmsModel::filters () const
```



Returns any filters that affect how toolbox content is filtered. 

**See also:** [**setFilters()**](classQfProcessingAlgorithmsModel.md#function-setfilters) 



        

<hr>



### function inPlaceLayer [2/2]

```C++
inline QgsVectorLayer * QfProcessingAlgorithmsModel::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithm filter 


        

<hr>



### function lessThan 

```C++
bool QfProcessingAlgorithmsModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>



### function rebuild 

_Rebuilds the algorithms model._ 
```C++
Q_INVOKABLE void QfProcessingAlgorithmsModel::rebuild () 
```




<hr>



### function setFilters 

```C++
void QfProcessingAlgorithmsModel::setFilters (
    QfProcessingAlgorithmsModel::Filters filters
) 
```



Set _filters_ that affect how toolbox content is filtered. 

**See also:** filters() 



        

<hr>



### function setInPlaceLayer 

```C++
void QfProcessingAlgorithmsModel::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/qfprocessingalgorithmsmodel.h`

