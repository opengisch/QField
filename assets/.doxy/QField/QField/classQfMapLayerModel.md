

# Class QfMapLayerModel



[**ClassList**](annotated.md) **>** [**QfMapLayerModel**](classQfMapLayerModel.md)



_A project map layers model within filtering capabilities._ 

* `#include <qfmaplayermodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classQfMapLayerModel.md#property-enabled-12)  <br> |
| property Qgis::LayerFilters | [**filters**](classQfMapLayerModel.md#property-filters-12)  <br>_The map layer filters._  |
| property QgsProject \* | [**project**](classQfMapLayerModel.md#property-project-12)  <br>_The project from which map layers will be taken from._  |
| property bool | [**requiresTrackingAvailability**](classQfMapLayerModel.md#property-requirestrackingavailability-12)  <br>_The tracking availability requirement filter._  |
| property [**QfTrackingModel**](classQfTrackingModel.md) \* | [**trackingModel**](classQfMapLayerModel.md#property-trackingmodel-12)  <br>_The tracking model against which map layers' tracking availability will be checked against._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfMapLayerModel.md#signal-enabledchanged)  <br> |
| signal void | [**filtersChanged**](classQfMapLayerModel.md#signal-filterschanged)  <br> |
| signal void | [**projectChanged**](classQfMapLayerModel.md#signal-projectchanged)  <br> |
| signal void | [**requiresTrackingAvailabilityChanged**](classQfMapLayerModel.md#signal-requirestrackingavailabilitychanged)  <br> |
| signal void | [**trackingModelChanged**](classQfMapLayerModel.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMapLayerModel**](#function-qfmaplayermodel) (QObject \* parent=nullptr) <br>_The constructor._  |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |
|  Qgis::LayerFilters | [**filters**](#function-filters-22) () const<br>_The map layer filters._  |
|  Q\_INVOKABLE int | [**findLayer**](#function-findlayer) (QgsMapLayer \* layer) const<br> |
|  Q\_INVOKABLE int | [**findLayerName**](#function-findlayername) (const QString & name) const<br> |
|  Q\_INVOKABLE QVariantMap | [**get**](#function-get) (int row) const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & left, const QModelIndex & right) override const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_The project from which map layers will be taken from._  |
|  bool | [**requiresTrackingAvailability**](#function-requirestrackingavailability-22) () const<br>_The tracking availability requirement filter._  |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setFilters**](#function-setfilters) (Qgis::LayerFilters filters) <br>_The map layer filters._  |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |
|  void | [**setRequiresTrackingAvailability**](#function-setrequirestrackingavailability) (bool requiresTrackingAvailability) <br>_The tracking availability requirement filter._  |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**QfTrackingModel**](classQfTrackingModel.md) \* trackingModel) <br>_The tracking model against which map layers' tracking availability will be checked against._  |
|  [**QfTrackingModel**](classQfTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel-22) () const<br>_The tracking model against which map layers' tracking availability will be checked against._  |




























## Public Types Documentation




### enum Role 

```C++
enum QfMapLayerModel::Role {
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
bool QfMapLayerModel::enabled;
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored 





        

<hr>



### property filters [1/2]

_The map layer filters._ 
```C++
Qgis::LayerFilters QfMapLayerModel::filters;
```




<hr>



### property project [1/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * QfMapLayerModel::project;
```




<hr>



### property requiresTrackingAvailability [1/2]

_The tracking availability requirement filter._ 
```C++
bool QfMapLayerModel::requiresTrackingAvailability;
```




<hr>



### property trackingModel [1/2]

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
QfTrackingModel * QfMapLayerModel::trackingModel;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void QfMapLayerModel::enabledChanged;
```




<hr>



### signal filtersChanged 

```C++
void QfMapLayerModel::filtersChanged;
```




<hr>



### signal projectChanged 

```C++
void QfMapLayerModel::projectChanged;
```




<hr>



### signal requiresTrackingAvailabilityChanged 

```C++
void QfMapLayerModel::requiresTrackingAvailabilityChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void QfMapLayerModel::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function QfMapLayerModel 

_The constructor._ 
```C++
explicit QfMapLayerModel::QfMapLayerModel (
    QObject * parent=nullptr
) 
```




<hr>



### function enabled [2/2]

```C++
bool QfMapLayerModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function filterAcceptsRow 

```C++
bool QfMapLayerModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>



### function filters [2/2]

_The map layer filters._ 
```C++
inline Qgis::LayerFilters QfMapLayerModel::filters () const
```




<hr>



### function findLayer 

```C++
Q_INVOKABLE int QfMapLayerModel::findLayer (
    QgsMapLayer * layer
) const
```



Returns the model row for given map _layer_. 

**Note:**

If the map layer is not found, -1 will be returned 





        

<hr>



### function findLayerName 

```C++
Q_INVOKABLE int QfMapLayerModel::findLayerName (
    const QString & name
) const
```



Returns the model row for given map layer _name_. 

**Note:**

If the map layer name is not found, -1 will be returned 





        

<hr>



### function get 

```C++
Q_INVOKABLE QVariantMap QfMapLayerModel::get (
    int row
) const
```



Returns a model data map for a given _row_. 


        

<hr>



### function lessThan 

```C++
bool QfMapLayerModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```




<hr>



### function project [2/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * QfMapLayerModel::project () const
```




<hr>



### function requiresTrackingAvailability [2/2]

_The tracking availability requirement filter._ 
```C++
inline bool QfMapLayerModel::requiresTrackingAvailability () const
```




<hr>



### function setEnabled 

```C++
void QfMapLayerModel::setEnabled (
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
void QfMapLayerModel::setFilters (
    Qgis::LayerFilters filters
) 
```




<hr>



### function setProject 

_The project from which map layers will be taken from._ 
```C++
void QfMapLayerModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setRequiresTrackingAvailability 

_The tracking availability requirement filter._ 
```C++
void QfMapLayerModel::setRequiresTrackingAvailability (
    bool requiresTrackingAvailability
) 
```




<hr>



### function setTrackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
void QfMapLayerModel::setTrackingModel (
    QfTrackingModel * trackingModel
) 
```




<hr>



### function trackingModel [2/2]

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
QfTrackingModel * QfMapLayerModel::trackingModel () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmaplayermodel.h`

