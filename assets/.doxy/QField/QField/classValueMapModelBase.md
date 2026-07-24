

# Class ValueMapModelBase



[**ClassList**](annotated.md) **>** [**ValueMapModelBase**](classValueMapModelBase.md)



[More...](#detailed-description)

* `#include <valuemapmodelbase.h>`



Inherits the following classes: QAbstractListModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapChanged**](classValueMapModelBase.md#signal-mapchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ValueMapModelBase**](#function-valuemapmodelbase) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QVariant | [**keyForValue**](#function-keyforvalue) (const QString & value) const<br> |
|  int | [**keyToIndex**](#function-keytoindex) (const QVariant & key) const<br> |
|  QVariant | [**map**](#function-map) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setMap**](#function-setmap) (const QVariant & map) <br> |




























## Detailed Description


A base model that manages the key/value pairs for a [**ValueMap**](classValueMap.md) widget. 


    
## Public Signals Documentation




### signal mapChanged 

```C++
void ValueMapModelBase::mapChanged;
```



Emitted when the map changes.   


        

<hr>
## Public Functions Documentation




### function ValueMapModelBase 

```C++
explicit ValueMapModelBase::ValueMapModelBase (
    QObject * parent=nullptr
) 
```



Create a new value map model base 


        

<hr>



### function data 

```C++
QVariant ValueMapModelBase::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function keyForValue 

```C++
QVariant ValueMapModelBase::keyForValue (
    const QString & value
) const
```



Returns the key for a value or an invalid QVariant if not found.   


        

<hr>



### function keyToIndex 

```C++
int ValueMapModelBase::keyToIndex (
    const QVariant & key
) const
```



Returns the row (index) of a key or -1 if not found.   


        

<hr>



### function map 

```C++
QVariant ValueMapModelBase::map () const
```



The map, see the property description   


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > ValueMapModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int ValueMapModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setMap 

```C++
void ValueMapModelBase::setMap (
    const QVariant & map
) 
```



The map, see the property description   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/valuemapmodelbase.h`

