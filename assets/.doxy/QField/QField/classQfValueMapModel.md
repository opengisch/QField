

# Class QfValueMapModel



[**ClassList**](annotated.md) **>** [**QfValueMapModel**](classQfValueMapModel.md)



[More...](#detailed-description)

* `#include <qfvaluemapmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ValueMapRoles**](#enum-valuemaproles)  <br>_The roles provided by this model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QVariant | [**valueMap**](classQfValueMapModel.md#property-valuemap)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapChanged**](classQfValueMapModel.md#signal-mapchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfValueMapModel**](#function-qfvaluemapmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QVariant | [**keyForValue**](#function-keyforvalue) (const QString & value) const<br> |
|  Q\_INVOKABLE int | [**keyToIndex**](#function-keytoindex) (const QVariant & key) const<br> |
|  QVariant | [**map**](#function-map) () const<br> |
|  void | [**setMap**](#function-setmap) (const QVariant & map) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |




## Detailed Description


A model that manages the key/value pairs for a ValueMap widget. 


    
## Public Types Documentation




### enum ValueMapRoles 

_The roles provided by this model._ 
```C++
enum QfValueMapModel::ValueMapRoles {
    KeyRole = Qt::UserRole + 1,
    ValueRole
};
```




<hr>
## Public Properties Documentation




### property valueMap 

```C++
QVariant QfValueMapModel::valueMap;
```



A list of QVariantMap, wrapped in a QVariant.


Like this:


[{'CH': 'Switzerland'}, {'DE': 'Germany'}, {'FR': 'France'}] 


        

<hr>
## Public Signals Documentation




### signal mapChanged 

```C++
void QfValueMapModel::mapChanged;
```



Emitted when the map changes. 


        

<hr>
## Public Functions Documentation




### function QfValueMapModel 

```C++
explicit QfValueMapModel::QfValueMapModel (
    QObject * parent=nullptr
) 
```



Create a new value map model 


        

<hr>



### function keyForValue 

```C++
Q_INVOKABLE QVariant QfValueMapModel::keyForValue (
    const QString & value
) const
```



Returns the key for a value or an invalid QVariant if not found. 


        

<hr>



### function keyToIndex 

```C++
Q_INVOKABLE int QfValueMapModel::keyToIndex (
    const QVariant & key
) const
```



Returns the row (index) of a key or -1 if not found. 


        

<hr>



### function map 

```C++
QVariant QfValueMapModel::map () const
```



The map, see the property description 


        

<hr>



### function setMap 

```C++
void QfValueMapModel::setMap (
    const QVariant & map
) 
```



The map, see the property description 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool QfValueMapModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfvaluemapmodel.h`

