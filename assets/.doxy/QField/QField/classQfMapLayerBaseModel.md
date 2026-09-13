

# Class QfMapLayerBaseModel



[**ClassList**](annotated.md) **>** [**QfMapLayerBaseModel**](classQfMapLayerBaseModel.md)



_A project map layers base model._ 

* `#include <qfmaplayermodel.h>`



Inherits the following classes: QAbstractListModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfMapLayerBaseModel.md#signal-enabledchanged)  <br> |
| signal void | [**projectChanged**](classQfMapLayerBaseModel.md#signal-projectchanged)  <br> |
| signal void | [**trackingModelChanged**](classQfMapLayerBaseModel.md#signal-trackingmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMapLayerBaseModel**](#function-qfmaplayerbasemodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**enabled**](#function-enabled) () const<br> |
|  QgsProject \* | [**project**](#function-project) () const<br>_The project from which map layers will be taken from._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |
|  void | [**setTrackingModel**](#function-settrackingmodel) ([**QfTrackingModel**](classQfTrackingModel.md) \* trackingModel) <br>_The tracking model against which map layers' tracking availability will be checked against._  |
|  [**QfTrackingModel**](classQfTrackingModel.md) \* | [**trackingModel**](#function-trackingmodel) () const<br>_The tracking model against which map layers' tracking availability will be checked against._  |












## Protected Slots

| Type | Name |
| ---: | :--- |
| slot void | [**addLayers**](classQfMapLayerBaseModel.md#slot-addlayers) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |
| slot void | [**removeLayers**](classQfMapLayerBaseModel.md#slot-removelayers) (const QStringList & layerIds) <br> |
















## Public Signals Documentation




### signal enabledChanged 

```C++
void QfMapLayerBaseModel::enabledChanged;
```




<hr>



### signal projectChanged 

```C++
void QfMapLayerBaseModel::projectChanged;
```




<hr>



### signal trackingModelChanged 

```C++
void QfMapLayerBaseModel::trackingModelChanged;
```




<hr>
## Public Functions Documentation




### function QfMapLayerBaseModel 

```C++
explicit QfMapLayerBaseModel::QfMapLayerBaseModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfMapLayerBaseModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function enabled 

```C++
inline bool QfMapLayerBaseModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry will be ignored   





        

<hr>



### function project 

_The project from which map layers will be taken from._ 
```C++
inline QgsProject * QfMapLayerBaseModel::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfMapLayerBaseModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfMapLayerBaseModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setEnabled 

```C++
void QfMapLayerBaseModel::setEnabled (
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
void QfMapLayerBaseModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setTrackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
void QfMapLayerBaseModel::setTrackingModel (
    QfTrackingModel * trackingModel
) 
```




<hr>



### function trackingModel 

_The tracking model against which map layers' tracking availability will be checked against._ 
```C++
inline QfTrackingModel * QfMapLayerBaseModel::trackingModel () const
```




<hr>
## Protected Slots Documentation




### slot addLayers 

```C++
void QfMapLayerBaseModel::addLayers;
```




<hr>



### slot removeLayers 

```C++
void QfMapLayerBaseModel::removeLayers;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmaplayermodel.h`

