

# Class ProcessingAlgorithmsModel



[**ClassList**](annotated.md) **>** [**ProcessingAlgorithmsModel**](classProcessingAlgorithmsModel.md)



_A sort/filter proxy model for providers and algorithms available within QField which automatically sorts the toolbox in a logical fashion and supports filtering the results._ 

* `#include <processingalgorithmsmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Filter**](#enum-filter)  <br>_Available filter flags for filtering the model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property ProcessingAlgorithmsModel::Filters | [**filters**](classProcessingAlgorithmsModel.md#property-filters-12)  <br> |
| property QgsVectorLayer \* | [**inPlaceLayer**](classProcessingAlgorithmsModel.md#property-inplacelayer-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**filtersChanged**](classProcessingAlgorithmsModel.md#signal-filterschanged)  <br> |
| signal void | [**inPlaceLayerChanged**](classProcessingAlgorithmsModel.md#signal-inplacelayerchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingAlgorithmsModel**](#function-processingalgorithmsmodel) (QObject \* parent=nullptr) <br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  ProcessingAlgorithmsModel::Filters | [**filters**](#function-filters-22) () const<br> |
|  QgsVectorLayer \* | [**inPlaceLayer**](#function-inplacelayer-22) () const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & sourceLeft, const QModelIndex & sourceRight) override const<br> |
|  Q\_INVOKABLE void | [**rebuild**](#function-rebuild) () <br>_Rebuilds the algorithms model._  |
|  void | [**setFilters**](#function-setfilters) (ProcessingAlgorithmsModel::Filters filters) <br> |
|  void | [**setInPlaceLayer**](#function-setinplacelayer) (QgsVectorLayer \* layer) <br> |




























## Public Types Documentation




### enum Filter 

_Available filter flags for filtering the model._ 
```C++
enum ProcessingAlgorithmsModel::Filter {
    InPlaceFilter = 1 << 1,
    FavoriteFilter = 1 << 2
};
```




<hr>
## Public Properties Documentation




### property filters [1/2]

```C++
ProcessingAlgorithmsModel::Filters ProcessingAlgorithmsModel::filters;
```




<hr>



### property inPlaceLayer [1/2]

```C++
QgsVectorLayer * ProcessingAlgorithmsModel::inPlaceLayer;
```




<hr>
## Public Signals Documentation




### signal filtersChanged 

```C++
void ProcessingAlgorithmsModel::filtersChanged;
```



Emitted when the active filters have changed 


        

<hr>



### signal inPlaceLayerChanged 

```C++
void ProcessingAlgorithmsModel::inPlaceLayerChanged;
```



Emitted when the in place vector layer has changed 


        

<hr>
## Public Functions Documentation




### function ProcessingAlgorithmsModel 

```C++
explicit ProcessingAlgorithmsModel::ProcessingAlgorithmsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function filterAcceptsRow 

```C++
bool ProcessingAlgorithmsModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

```C++
inline ProcessingAlgorithmsModel::Filters ProcessingAlgorithmsModel::filters () const
```



Returns any filters that affect how toolbox content is filtered. 

**See also:** [**setFilters()**](classProcessingAlgorithmsModel.md#function-setfilters) 



        

<hr>



### function inPlaceLayer [2/2]

```C++
inline QgsVectorLayer * ProcessingAlgorithmsModel::inPlaceLayer () const
```



Returns the vector _layer_ for in-place algorithm filter 


        

<hr>



### function lessThan 

```C++
bool ProcessingAlgorithmsModel::lessThan (
    const QModelIndex & sourceLeft,
    const QModelIndex & sourceRight
) override const
```




<hr>



### function rebuild 

_Rebuilds the algorithms model._ 
```C++
Q_INVOKABLE void ProcessingAlgorithmsModel::rebuild () 
```




<hr>



### function setFilters 

```C++
void ProcessingAlgorithmsModel::setFilters (
    ProcessingAlgorithmsModel::Filters filters
) 
```



Set _filters_ that affect how toolbox content is filtered. 

**See also:** filters() 



        

<hr>



### function setInPlaceLayer 

```C++
void ProcessingAlgorithmsModel::setInPlaceLayer (
    QgsVectorLayer * layer
) 
```



Sets the vector _layer_ for in-place algorithm filter 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/processingalgorithmsmodel.h`

