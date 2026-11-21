

# Class QfLocatorFiltersModel



[**ClassList**](annotated.md) **>** [**QfLocatorFiltersModel**](classQfLocatorFiltersModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br>_Custom model roles._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* | [**locatorModelSuperBridge**](classQfLocatorFiltersModel.md#property-locatormodelsuperbridge-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**locatorModelSuperBridgeChanged**](classQfLocatorFiltersModel.md#signal-locatormodelsuperbridgechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocatorFiltersModel**](#function-qflocatorfiltersmodel) () <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QgsLocatorFilter \* | [**filterForIndex**](#function-filterforindex) (const QModelIndex & index) const<br> |
|  [**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* | [**locatorModelSuperBridge**](#function-locatormodelsuperbridge-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  Q\_INVOKABLE void | [**setGeocoderLocatorFiltersDefaulByPosition**](#function-setgeocoderlocatorfiltersdefaulbyposition) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & position) <br> |
|  void | [**setLocatorModelSuperBridge**](#function-setlocatormodelsuperbridge) ([**QfLocatorModelSuperBridge**](classQfLocatorModelSuperBridge.md) \* locatorModelSuperBridge) <br> |




























## Public Types Documentation




### enum Role 

_Custom model roles._ 
```C++
enum QfLocatorFiltersModel::Role {
    NameRole = Qt::UserRole + 1,
    DescriptionRole,
    PrefixRole,
    ActiveRole,
    DefaultRole
};
```




<hr>
## Public Properties Documentation




### property locatorModelSuperBridge [1/2]

```C++
QfLocatorModelSuperBridge * QfLocatorFiltersModel::locatorModelSuperBridge;
```




<hr>
## Public Signals Documentation




### signal locatorModelSuperBridgeChanged 

```C++
void QfLocatorFiltersModel::locatorModelSuperBridgeChanged;
```




<hr>
## Public Functions Documentation




### function QfLocatorFiltersModel 

```C++
QfLocatorFiltersModel::QfLocatorFiltersModel () 
```



Constructor for QgsLocatorFiltersModel. 


        

<hr>



### function data 

```C++
QVariant QfLocatorFiltersModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function filterForIndex 

```C++
QgsLocatorFilter * QfLocatorFiltersModel::filterForIndex (
    const QModelIndex & index
) const
```




<hr>



### function locatorModelSuperBridge [2/2]

```C++
QfLocatorModelSuperBridge * QfLocatorFiltersModel::locatorModelSuperBridge () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfLocatorFiltersModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfLocatorFiltersModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setData 

```C++
bool QfLocatorFiltersModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setGeocoderLocatorFiltersDefaulByPosition 

```C++
Q_INVOKABLE void QfLocatorFiltersModel::setGeocoderLocatorFiltersDefaulByPosition (
    const QfGnssPositionInformation & position
) 
```




<hr>



### function setLocatorModelSuperBridge 

```C++
void QfLocatorFiltersModel::setLocatorModelSuperBridge (
    QfLocatorModelSuperBridge * locatorModelSuperBridge
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qflocatormodelsuperbridge.h`

