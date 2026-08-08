

# Class LocatorFiltersModel



[**ClassList**](annotated.md) **>** [**LocatorFiltersModel**](classLocatorFiltersModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br>_Custom model roles._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* | [**locatorModelSuperBridge**](classLocatorFiltersModel.md#property-locatormodelsuperbridge-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**locatorModelSuperBridgeChanged**](classLocatorFiltersModel.md#signal-locatormodelsuperbridgechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LocatorFiltersModel**](#function-locatorfiltersmodel) () <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  QgsLocatorFilter \* | [**filterForIndex**](#function-filterforindex) (const QModelIndex & index) const<br> |
|  [**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* | [**locatorModelSuperBridge**](#function-locatormodelsuperbridge-22) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  Q\_INVOKABLE void | [**setGeocoderLocatorFiltersDefaulByPosition**](#function-setgeocoderlocatorfiltersdefaulbyposition) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & position) <br> |
|  void | [**setLocatorModelSuperBridge**](#function-setlocatormodelsuperbridge) ([**LocatorModelSuperBridge**](classLocatorModelSuperBridge.md) \* locatorModelSuperBridge) <br> |




























## Public Types Documentation




### enum Role 

_Custom model roles._ 
```C++
enum LocatorFiltersModel::Role {
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
LocatorModelSuperBridge * LocatorFiltersModel::locatorModelSuperBridge;
```




<hr>
## Public Signals Documentation




### signal locatorModelSuperBridgeChanged 

```C++
void LocatorFiltersModel::locatorModelSuperBridgeChanged;
```




<hr>
## Public Functions Documentation




### function LocatorFiltersModel 

```C++
LocatorFiltersModel::LocatorFiltersModel () 
```



Constructor for QgsLocatorFiltersModel. 


        

<hr>



### function data 

```C++
QVariant LocatorFiltersModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function filterForIndex 

```C++
QgsLocatorFilter * LocatorFiltersModel::filterForIndex (
    const QModelIndex & index
) const
```




<hr>



### function locatorModelSuperBridge [2/2]

```C++
LocatorModelSuperBridge * LocatorFiltersModel::locatorModelSuperBridge () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > LocatorFiltersModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int LocatorFiltersModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setData 

```C++
bool LocatorFiltersModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setGeocoderLocatorFiltersDefaulByPosition 

```C++
Q_INVOKABLE void LocatorFiltersModel::setGeocoderLocatorFiltersDefaulByPosition (
    const GnssPositionInformation & position
) 
```




<hr>



### function setLocatorModelSuperBridge 

```C++
void LocatorFiltersModel::setLocatorModelSuperBridge (
    LocatorModelSuperBridge * locatorModelSuperBridge
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/locatormodelsuperbridge.h`

