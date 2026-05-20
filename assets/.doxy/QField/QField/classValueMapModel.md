

# Class ValueMapModel



[**ClassList**](annotated.md) **>** [**ValueMapModel**](classValueMapModel.md)



[More...](#detailed-description)

* `#include <valuemapmodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ValueMapRoles**](#enum-valuemaproles)  <br>_The roles provided by this model._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QVariant | [**valueMap**](classValueMapModel.md#property-valuemap)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapChanged**](classValueMapModel.md#signal-mapchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ValueMapModel**](#function-valuemapmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QVariant | [**keyForValue**](#function-keyforvalue) (const QString & value) const<br> |
|  Q\_INVOKABLE int | [**keyToIndex**](#function-keytoindex) (const QVariant & key) const<br> |
|  QVariant | [**map**](#function-map) () const<br> |
|  void | [**setMap**](#function-setmap) (const QVariant & map) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |




## Detailed Description


A model that manages the key/value pairs for a [**ValueMap**](classValueMap.md) widget. 


    
## Public Types Documentation




### enum ValueMapRoles 

_The roles provided by this model._ 
```C++
enum ValueMapModel::ValueMapRoles {
    KeyRole = Qt::UserRole + 1,
    ValueRole
};
```




<hr>
## Public Properties Documentation




### property valueMap 

```C++
QVariant ValueMapModel::valueMap;
```



A list of QVariantMap, wrapped in a QVariant.


Like this:


[{'CH': 'Switzerland'}, {'DE': 'Germany'}, {'FR': 'France'}] 


        

<hr>
## Public Signals Documentation




### signal mapChanged 

```C++
void ValueMapModel::mapChanged;
```



Emitted when the map changes. 


        

<hr>
## Public Functions Documentation




### function ValueMapModel 

```C++
explicit ValueMapModel::ValueMapModel (
    QObject * parent=nullptr
) 
```



Create a new value map model 


        

<hr>



### function keyForValue 

```C++
Q_INVOKABLE QVariant ValueMapModel::keyForValue (
    const QString & value
) const
```



Returns the key for a value or an invalid QVariant if not found. 


        

<hr>



### function keyToIndex 

```C++
Q_INVOKABLE int ValueMapModel::keyToIndex (
    const QVariant & key
) const
```



Returns the row (index) of a key or -1 if not found. 


        

<hr>



### function map 

```C++
QVariant ValueMapModel::map () const
```



The map, see the property description 


        

<hr>



### function setMap 

```C++
void ValueMapModel::setMap (
    const QVariant & map
) 
```



The map, see the property description 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool ValueMapModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/valuemapmodel.h`

