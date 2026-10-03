

# Class QfDigitizingLayerBaseModel



[**ClassList**](annotated.md) **>** [**QfDigitizingLayerBaseModel**](classQfDigitizingLayerBaseModel.md)



_A digitizing layers (map layers and markup collections) base model._ 

* `#include <qfdigitizinglayermodel.h>`



Inherits the following classes: QAbstractListModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfDigitizingLayerBaseModel.md#signal-enabledchanged)  <br> |
| signal void | [**markupManagerChanged**](classQfDigitizingLayerBaseModel.md#signal-markupmanagerchanged)  <br> |
| signal void | [**projectChanged**](classQfDigitizingLayerBaseModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDigitizingLayerBaseModel**](#function-qfdigitizinglayerbasemodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**enabled**](#function-enabled) () const<br> |
|  int | [**findCollection**](#function-findcollection) ([**QfMarkupCollection**](classQfMarkupCollection.md) \* collection) const<br> |
|  int | [**findCollectionName**](#function-findcollectionname) (const QString & name) const<br> |
|  int | [**findLayer**](#function-findlayer) (QgsMapLayer \* layer) const<br> |
|  int | [**findLayerName**](#function-findlayername) (const QString & name) const<br> |
|  [**QfMarkupManager**](classQfMarkupManager.md) \* | [**markupManager**](#function-markupmanager) () const<br> |
|  QgsProject \* | [**project**](#function-project) () const<br>_The project from which map layers will be taken from._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setMarkupManager**](#function-setmarkupmanager) ([**QfMarkupManager**](classQfMarkupManager.md) \* markupManager) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |












## Protected Slots

| Type | Name |
| ---: | :--- |
| slot void | [**addCollections**](classQfDigitizingLayerBaseModel.md#slot-addcollections) (const QStringList & collectionUuids) <br> |
| slot void | [**addLayers**](classQfDigitizingLayerBaseModel.md#slot-addlayers) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |
| slot void | [**removeCollections**](classQfDigitizingLayerBaseModel.md#slot-removecollections) (const QStringList & collectionUuids) <br> |
| slot void | [**removeLayers**](classQfDigitizingLayerBaseModel.md#slot-removelayers) (const QStringList & layerIds) <br> |
















## Public Signals Documentation




### signal enabledChanged 

```C++
void QfDigitizingLayerBaseModel::enabledChanged;
```




<hr>



### signal markupManagerChanged 

```C++
void QfDigitizingLayerBaseModel::markupManagerChanged;
```




<hr>



### signal projectChanged 

```C++
void QfDigitizingLayerBaseModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfDigitizingLayerBaseModel 

```C++
explicit QfDigitizingLayerBaseModel::QfDigitizingLayerBaseModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfDigitizingLayerBaseModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function enabled 

```C++
inline bool QfDigitizingLayerBaseModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry and markup manager will be ignored   





        

<hr>



### function findCollection 

```C++
int QfDigitizingLayerBaseModel::findCollection (
    QfMarkupCollection * collection
) const
```



Returns the model row for given markup _collection_. 

**Note:**

If the collection is not found, -1 will be returned   





        

<hr>



### function findCollectionName 

```C++
int QfDigitizingLayerBaseModel::findCollectionName (
    const QString & name
) const
```



Returns the model row for given markup collection _name_. 

**Note:**

If the collection name is not found, -1 will be returned   





        

<hr>



### function findLayer 

```C++
int QfDigitizingLayerBaseModel::findLayer (
    QgsMapLayer * layer
) const
```



Returns the model row for given map _layer_. 

**Note:**

If the map layer is not found, -1 will be returned   





        

<hr>



### function findLayerName 

```C++
int QfDigitizingLayerBaseModel::findLayerName (
    const QString & name
) const
```



Returns the model row for given map layer _name_. 

**Note:**

If the map layer name is not found, -1 will be returned   





        

<hr>



### function markupManager 

```C++
inline QfMarkupManager * QfDigitizingLayerBaseModel::markupManager () const
```




<hr>



### function project 

_The project from which map layers will be taken from._ 
```C++
inline QgsProject * QfDigitizingLayerBaseModel::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfDigitizingLayerBaseModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfDigitizingLayerBaseModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setEnabled 

```C++
void QfDigitizingLayerBaseModel::setEnabled (
    bool enabled
) 
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry and markup manager will be ignored   





        

<hr>



### function setMarkupManager 

```C++
void QfDigitizingLayerBaseModel::setMarkupManager (
    QfMarkupManager * markupManager
) 
```




<hr>



### function setProject 

_The project from which map layers will be taken from._ 
```C++
void QfDigitizingLayerBaseModel::setProject (
    QgsProject * project
) 
```




<hr>
## Protected Slots Documentation




### slot addCollections 

```C++
void QfDigitizingLayerBaseModel::addCollections;
```




<hr>



### slot addLayers 

```C++
void QfDigitizingLayerBaseModel::addLayers;
```




<hr>



### slot removeCollections 

```C++
void QfDigitizingLayerBaseModel::removeCollections;
```




<hr>



### slot removeLayers 

```C++
void QfDigitizingLayerBaseModel::removeLayers;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfdigitizinglayermodel.h`

