

# Class NavigationModel



[**ClassList**](annotated.md) **>** [**NavigationModel**](classNavigationModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |
| enum  | [**Types**](#enum-types)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**destinationChanged**](classNavigationModel.md#signal-destinationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**NavigationModel**](#function-navigationmodel) () <br> |
|  void | [**clear**](#function-clear) () <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  QgsPoint | [**destination**](#function-destination) () const<br> |
|  QgsPointSequence | [**points**](#function-points) () const<br> |
|  void | [**restore**](#function-restore) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**save**](#function-save) () <br> |
|  void | [**setCrs**](#function-setcrs) (QgsCoordinateReferenceSystem crs) <br> |
|  void | [**setDestination**](#function-setdestination) (const QgsPoint & point) <br> |




























## Public Types Documentation




### enum Roles 

```C++
enum NavigationModel::Roles {
    Point = Qt::UserRole + 1,
    PointType
};
```




<hr>



### enum Types 

```C++
enum NavigationModel::Types {
    Destination,
    Intermediate
};
```




<hr>
## Public Signals Documentation




### signal destinationChanged 

```C++
void NavigationModel::destinationChanged;
```




<hr>
## Public Functions Documentation




### function NavigationModel 

```C++
NavigationModel::NavigationModel () 
```




<hr>



### function clear 

```C++
void NavigationModel::clear () 
```




<hr>



### function data 

```C++
QVariant NavigationModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function destination 

```C++
QgsPoint NavigationModel::destination () const
```




<hr>



### function points 

```C++
inline QgsPointSequence NavigationModel::points () const
```




<hr>



### function restore 

```C++
void NavigationModel::restore () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > NavigationModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int NavigationModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function save 

```C++
void NavigationModel::save () 
```




<hr>



### function setCrs 

```C++
void NavigationModel::setCrs (
    QgsCoordinateReferenceSystem crs
) 
```




<hr>



### function setDestination 

```C++
void NavigationModel::setDestination (
    const QgsPoint & point
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/navigationmodel.h`

