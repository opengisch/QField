

# Class QfValueMapModelBase



[**ClassList**](annotated.md) **>** [**QfValueMapModelBase**](classQfValueMapModelBase.md)



[More...](#detailed-description)

* `#include <qfvaluemapmodelbase.h>`



Inherits the following classes: QAbstractListModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapChanged**](classQfValueMapModelBase.md#signal-mapchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfValueMapModelBase**](#function-qfvaluemapmodelbase) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QVariant | [**keyForValue**](#function-keyforvalue) (const QString & value) const<br> |
|  int | [**keyToIndex**](#function-keytoindex) (const QVariant & key) const<br> |
|  QVariant | [**map**](#function-map) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setMap**](#function-setmap) (const QVariant & map) <br> |




























## Detailed Description


A base model that manages the key/value pairs for a ValueMap widget. 


    
## Public Signals Documentation




### signal mapChanged 

```C++
void QfValueMapModelBase::mapChanged;
```



Emitted when the map changes.   


        

<hr>
## Public Functions Documentation




### function QfValueMapModelBase 

```C++
explicit QfValueMapModelBase::QfValueMapModelBase (
    QObject * parent=nullptr
) 
```



Create a new value map model base 


        

<hr>



### function data 

```C++
QVariant QfValueMapModelBase::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function keyForValue 

```C++
QVariant QfValueMapModelBase::keyForValue (
    const QString & value
) const
```



Returns the key for a value or an invalid QVariant if not found.   


        

<hr>



### function keyToIndex 

```C++
int QfValueMapModelBase::keyToIndex (
    const QVariant & key
) const
```



Returns the row (index) of a key or -1 if not found.   


        

<hr>



### function map 

```C++
QVariant QfValueMapModelBase::map () const
```



The map, see the property description   


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfValueMapModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfValueMapModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setMap 

```C++
void QfValueMapModelBase::setMap (
    const QVariant & map
) 
```



The map, see the property description   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfvaluemapmodelbase.h`

