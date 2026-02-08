

# Class MapLayerModel



[**ClassList**](annotated.md) **>** [**MapLayerModel**](classMapLayerModel.md)



_A project map layers model within filtering capabilities._ 

* `#include <maplayermodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classMapLayerModel.md#property-enabled-12)  <br> |
| property Qgis::LayerFilters | [**filters**](classMapLayerModel.md#property-filters-12)  <br>_The map layer filters._  |
| property QgsProject \* | [**project**](classMapLayerModel.md#property-project-12)  <br>_The project from which map layers will be taken from._  |
| property bool | [**requiresTrackingAvailability**](classMapLayerModel.md#property-requirestrackingavailability-12)  <br>_The tracking availability requirement filter._  |
| property [**TrackingModel**](classTrackingModel.md) \* | [**trackingModel**](classMapLayerModel.md#property-trackingmodel-12)  <br>_The tracking model against which map layers' tracking availability will be checked against._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classMapLayerModel.md#signal-enabledchanged)  <br> |
| signal void | [**filtersChanged**](classMapLayerModel.md#signal-filterschanged)  <br> |
| signal void | [**projectChanged**](classMapLayerModel.md#signal-projectchanged)  <br> |
| signal void | [**requiresTrackingAvailabilityChanged**](classMapLayerModel.md#signal-requirestrackingavailabilitychanged)  <br> |
| signal void | [**trackingModelChanged**](classMapLayerModel.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapLayerModel**](#function-maplayermodel) (QObject \* parent=nullptr) <br>_The constructor._  |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  Qgis::LayerFilters | [**filters**](#function-filters-22) () const<br>_The map layer filters._  |
|  Q\_INVOKABLE int | [**findLayer**](#function-findlayer) (QgsMapLayer \* layer) const<br> |
|  Q\_INVOKABLE QVariantMap | [**get**](#function-get) (int row) const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & left, const QModelIndex & right) override const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_The project from which map layers will be taken from._  |
|  bool | [**requiresTrackingAvailability**](#function-requirestrackingavailability-22) () const<br>_The tracking availability requirement filter._  |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setFilters**](#function-setfilters) (Qgis::LayerFilters filters) <br>_The map layer filters._  |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |
|  void | [**setRequiresTrackingAvailability**](#function-setrequirestrackingavailability) (bool requiresTrackingAvailability) <br>_The tracking availability requirement filter._  |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**TrackingModel**](classTrackingModel.md) \* trackingModel) <br>_The tracking model against which map layers' tracking availability will be checked against._  |
|  [**TrackingModel**](classTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel-22) () const<br>_The tracking model against which map layers' tracking availability will be checked against._  |




























## Public Types Documentation




### enum Role 

```C++
enum MapLayerModel::Role {
    IdRole = Qt::UserRole + 1,
    NameRole,
    LayerRole,
    LayerTypeRole,
    GeometryTypeRole
};
```




<hr>
## Public Properties Documentation




### property enabled [1/2]

```C++
bool MapLayerModel::enabled;
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored 





        

<hr>



### property filters [1/2]

_The map layer filters._ 
```C++
Qgis::LayerFilters MapLayerModel::filters;
```




<hr>



### property project [1/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * MapLayerModel::project;
```




<hr>



### property requiresTrackingAvailability [1/2]

_The tracking availability requirement filter._ 
```C++
bool MapLayerModel::requiresTrackingAvailability;
```




<hr>



### property trackingModel [1/2]

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
TrackingModel * MapLayerModel::trackingModel;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void MapLayerModel::enabledChanged;
```




<hr>



### signal filtersChanged 

```C++
void MapLayerModel::filtersChanged;
```




<hr>



### signal projectChanged 

```C++
void MapLayerModel::projectChanged;
```




<hr>



### signal requiresTrackingAvailabilityChanged 

```C++
void MapLayerModel::requiresTrackingAvailabilityChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void MapLayerModel::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function MapLayerModel 

_The constructor._ 
```C++
explicit MapLayerModel::MapLayerModel (
    QObject * parent=nullptr
) 
```




<hr>



### function enabled [2/2]

```C++
bool MapLayerModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function filterAcceptsRow 

```C++
bool MapLayerModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

_The map layer filters._ 
```C++
inline Qgis::LayerFilters MapLayerModel::filters () const
```




<hr>



### function findLayer 

```C++
Q_INVOKABLE int MapLayerModel::findLayer (
    QgsMapLayer * layer
) const
```



Returns the model row for given map _layer_. 

**Note:**

If the map layer is not found, -1 will be returned 





        

<hr>



### function get 

```C++
Q_INVOKABLE QVariantMap MapLayerModel::get (
    int row
) const
```



Returns a model data map for a given _row_. 


        

<hr>



### function lessThan 

```C++
bool MapLayerModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```




<hr>



### function project [2/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * MapLayerModel::project () const
```




<hr>



### function requiresTrackingAvailability [2/2]

_The tracking availability requirement filter._ 
```C++
inline bool MapLayerModel::requiresTrackingAvailability () const
```




<hr>



### function setEnabled 

```C++
void MapLayerModel::setEnabled (
    bool enabled
) 
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function setFilters 

_The map layer filters._ 
```C++
void MapLayerModel::setFilters (
    Qgis::LayerFilters filters
) 
```




<hr>



### function setProject 

_The project from which map layers will be taken from._ 
```C++
void MapLayerModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setRequiresTrackingAvailability 

_The tracking availability requirement filter._ 
```C++
void MapLayerModel::setRequiresTrackingAvailability (
    bool requiresTrackingAvailability
) 
```




<hr>



### function setTrackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
void MapLayerModel::setTrackingModel (
    TrackingModel * trackingModel
) 
```




<hr>



### function trackingModel [2/2]

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
TrackingModel * MapLayerModel::trackingModel () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/maplayermodel.h`

