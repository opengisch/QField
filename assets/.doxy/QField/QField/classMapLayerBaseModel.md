

# Class MapLayerBaseModel



[**ClassList**](annotated.md) **>** [**MapLayerBaseModel**](classMapLayerBaseModel.md)



_A project map layers base model._ 

* `#include <maplayermodel.h>`



Inherits the following classes: QAbstractListModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classMapLayerBaseModel.md#signal-enabledchanged)  <br> |
| signal void | [**projectChanged**](classMapLayerBaseModel.md#signal-projectchanged)  <br> |
| signal void | [**trackingModelChanged**](classMapLayerBaseModel.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapLayerBaseModel**](#function-maplayerbasemodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**enabled**](#function-enabled) () const<br> |
|  QgsProject \* | [**project**](#function-project) () const<br>_The project from which map layers will be taken from._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**TrackingModel**](classTrackingModel.md) \* trackingModel) <br>_The tracking model against which map layers' tracking availability will be checked against._  |
|  [**TrackingModel**](classTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel) () const<br>_The tracking model against which map layers' tracking availability will be checked against._  |












## Protected Slots

| Type | Name |
| ---: | :--- |
| slot void | [**addLayers**](classMapLayerBaseModel.md#slot-addlayers) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |
| slot void | [**removeLayers**](classMapLayerBaseModel.md#slot-removelayers) (const QStringList & layerIds) <br> |
















## Public Signals Documentation




### signal enabledChanged 

```C++
void MapLayerBaseModel::enabledChanged;
```




<hr>



### signal projectChanged 

```C++
void MapLayerBaseModel::projectChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void MapLayerBaseModel::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function MapLayerBaseModel 

```C++
explicit MapLayerBaseModel::MapLayerBaseModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant MapLayerBaseModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function enabled 

```C++
inline bool MapLayerBaseModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function project 

_The project from which map layers will be taken from._ 
```C++
inline QgsProject * MapLayerBaseModel::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > MapLayerBaseModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int MapLayerBaseModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setEnabled 

```C++
void MapLayerBaseModel::setEnabled (
    bool enabled
) 
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function setProject 

_The project from which map layers will be taken from._ 
```C++
void MapLayerBaseModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setTrackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
void MapLayerBaseModel::setTrackingModel (
    TrackingModel * trackingModel
) 
```




<hr>



### function trackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
inline TrackingModel * MapLayerBaseModel::trackingModel () const
```




<hr>
## Protected Slots Documentation




### slot addLayers 

```C++
void MapLayerBaseModel::addLayers;
```




<hr>



### slot removeLayers 

```C++
void MapLayerBaseModel::removeLayers;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/maplayermodel.h`

