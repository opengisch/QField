

# Class QfNavigationModel



[**ClassList**](annotated.md) **>** [**QfNavigationModel**](classQfNavigationModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |
| enum  | [**Types**](#enum-types)  <br> |
















## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**destinationChanged**](classQfNavigationModel.md#signal-destinationchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNavigationModel**](#function-qfnavigationmodel) () <br> |
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
enum QfNavigationModel::Roles {
    Point = Qt::UserRole + 1,
    PointType
};
```




<hr>



### enum Types 

```C++
enum QfNavigationModel::Types {
    Destination,
    Intermediate
};
```




<hr>
## Public Signals Documentation




### signal destinationChanged 

```C++
void QfNavigationModel::destinationChanged;
```




<hr>
## Public Functions Documentation




### function QfNavigationModel 

```C++
QfNavigationModel::QfNavigationModel () 
```




<hr>



### function clear 

```C++
void QfNavigationModel::clear () 
```




<hr>



### function data 

```C++
QVariant QfNavigationModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function destination 

```C++
QgsPoint QfNavigationModel::destination () const
```




<hr>



### function points 

```C++
inline QgsPointSequence QfNavigationModel::points () const
```




<hr>



### function restore 

```C++
void QfNavigationModel::restore () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfNavigationModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfNavigationModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function save 

```C++
void QfNavigationModel::save () 
```




<hr>



### function setCrs 

```C++
void QfNavigationModel::setCrs (
    QgsCoordinateReferenceSystem crs
) 
```




<hr>



### function setDestination 

```C++
void QfNavigationModel::setDestination (
    const QgsPoint & point
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfnavigationmodel.h`

