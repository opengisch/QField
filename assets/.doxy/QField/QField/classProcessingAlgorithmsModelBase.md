

# Class ProcessingAlgorithmsModelBase



[**ClassList**](annotated.md) **>** [**ProcessingAlgorithmsModelBase**](classProcessingAlgorithmsModelBase.md)



_A model for providers and algorithms available within QField._ [More...](#detailed-description)

* `#include <processingalgorithmsmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br>_Roles to get the data of the model._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ProcessingAlgorithmsModelBase**](#function-processingalgorithmsmodelbase) (QObject \* parent=nullptr) <br> |
|  const QgsProcessingAlgorithm \* | [**algorithmForIndex**](#function-algorithmforindex) (const QModelIndex & index) const<br>_Returns the processing algorithm for the given index._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE void | [**rebuild**](#function-rebuild) () <br>_Rebuilds the algorithms model._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |




























## Detailed Description


See ProcessingAlgorithmsProxyModel for a sorted, filterable version of this model. 


    
## Public Types Documentation




### enum Role 

_Roles to get the data of the model._ 
```C++
enum ProcessingAlgorithmsModelBase::Role {
    AlgorithmIdRole = Qt::UserRole,
    AlgorithmGroupRole,
    AlgorithmNameRole,
    AlgorithmSvgIconRole,
    AlgorithmFlagsRole,
    AlgorithmFavoriteRole
};
```




<hr>
## Public Functions Documentation




### function ProcessingAlgorithmsModelBase 

```C++
explicit ProcessingAlgorithmsModelBase::ProcessingAlgorithmsModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function algorithmForIndex 

_Returns the processing algorithm for the given index._ 
```C++
const QgsProcessingAlgorithm * ProcessingAlgorithmsModelBase::algorithmForIndex (
    const QModelIndex & index
) const
```




<hr>



### function data 

```C++
QVariant ProcessingAlgorithmsModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function rebuild 

_Rebuilds the algorithms model._ 
```C++
Q_INVOKABLE void ProcessingAlgorithmsModelBase::rebuild () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > ProcessingAlgorithmsModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int ProcessingAlgorithmsModelBase::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setData 

```C++
bool ProcessingAlgorithmsModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/processing/processingalgorithmsmodel.h`

