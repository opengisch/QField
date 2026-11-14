

# Class QfDigitizingLayerModel



[**ClassList**](annotated.md) **>** [**QfDigitizingLayerModel**](classQfDigitizingLayerModel.md)



_A digitizing layers (map layers and markup collections) model._ 

* `#include <qfdigitizinglayermodel.h>`



Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**LayerType**](#enum-layertype)  <br> |
| enum  | [**Role**](#enum-role)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classQfDigitizingLayerModel.md#property-enabled-12)  <br> |
| property [**QfMarkupManager**](classQfMarkupManager.md) \* | [**markups**](classQfDigitizingLayerModel.md#property-markups)  <br>_The markup manager from which markup collections will be taken from._  |
| property QgsProject \* | [**project**](classQfDigitizingLayerModel.md#property-project-12)  <br>_The project from which map layers will be taken from._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfDigitizingLayerModel.md#signal-enabledchanged)  <br> |
| signal void | [**markupManagerChanged**](classQfDigitizingLayerModel.md#signal-markupmanagerchanged)  <br> |
| signal void | [**projectChanged**](classQfDigitizingLayerModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDigitizingLayerModel**](#function-qfdigitizinglayermodel) (QObject \* parent=nullptr) <br>_The constructor._  |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  Q\_INVOKABLE int | [**findCollection**](#function-findcollection) ([**QfMarkupCollection**](classQfMarkupCollection.md) \* collection) const<br> |
|  Q\_INVOKABLE int | [**findCollectionName**](#function-findcollectionname) (const QString & name) const<br> |
|  Q\_INVOKABLE int | [**findLayer**](#function-findlayer) (QgsMapLayer \* layer) const<br> |
|  Q\_INVOKABLE int | [**findLayerName**](#function-findlayername) (const QString & name) const<br> |
|  Q\_INVOKABLE QVariantMap | [**get**](#function-get) (int row) const<br> |
|  bool | [**lessThan**](#function-lessthan) (const QModelIndex & left, const QModelIndex & right) override const<br> |
|  [**QfMarkupManager**](classQfMarkupManager.md) \* | [**markupManager**](#function-markupmanager) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_The project from which map layers will be taken from._  |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setMarkupManager**](#function-setmarkupmanager) ([**QfMarkupManager**](classQfMarkupManager.md) \* markupManager) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_The project from which map layers will be taken from._  |




























## Public Types Documentation




### enum LayerType 

```C++
enum QfDigitizingLayerModel::LayerType {
    MapLayer = 1,
    MarkupCollection
};
```




<hr>



### enum Role 

```C++
enum QfDigitizingLayerModel::Role {
    IdRole = Qt::UserRole + 1,
    NameRole,
    LayerTypeRole,
    LayerRole,
    GeometryTypeRole
};
```




<hr>
## Public Properties Documentation




### property enabled [1/2]

```C++
bool QfDigitizingLayerModel::enabled;
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry and markup manager will be ignored 





        

<hr>



### property markups 

_The markup manager from which markup collections will be taken from._ 
```C++
QfMarkupManager * QfDigitizingLayerModel::markups;
```




<hr>



### property project [1/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * QfDigitizingLayerModel::project;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void QfDigitizingLayerModel::enabledChanged;
```




<hr>



### signal markupManagerChanged 

```C++
void QfDigitizingLayerModel::markupManagerChanged;
```




<hr>



### signal projectChanged 

```C++
void QfDigitizingLayerModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfDigitizingLayerModel 

_The constructor._ 
```C++
explicit QfDigitizingLayerModel::QfDigitizingLayerModel (
    QObject * parent=nullptr
) 
```




<hr>



### function enabled [2/2]

```C++
bool QfDigitizingLayerModel::enabled () const
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry and markup manager will be ignored   





        

<hr>



### function findCollection 

```C++
Q_INVOKABLE int QfDigitizingLayerModel::findCollection (
    QfMarkupCollection * collection
) const
```



Returns the model row for given markup _collection_. 

**Note:**

If the collection is not found, -1 will be returned 





        

<hr>



### function findCollectionName 

```C++
Q_INVOKABLE int QfDigitizingLayerModel::findCollectionName (
    const QString & name
) const
```



Returns the model row for given markup collection _name_. 

**Note:**

If the collection name is not found, -1 will be returned 





        

<hr>



### function findLayer 

```C++
Q_INVOKABLE int QfDigitizingLayerModel::findLayer (
    QgsMapLayer * layer
) const
```



Returns the model row for given map _layer_. 

**Note:**

If the map layer is not found, -1 will be returned 





        

<hr>



### function findLayerName 

```C++
Q_INVOKABLE int QfDigitizingLayerModel::findLayerName (
    const QString & name
) const
```



Returns the model row for given map layer _name_. 

**Note:**

If the map layer name is not found, -1 will be returned 





        

<hr>



### function get 

```C++
Q_INVOKABLE QVariantMap QfDigitizingLayerModel::get (
    int row
) const
```



Returns a model data map for a given _row_. 


        

<hr>



### function lessThan 

```C++
bool QfDigitizingLayerModel::lessThan (
    const QModelIndex & left,
    const QModelIndex & right
) override const
```




<hr>



### function markupManager 

```C++
QfMarkupManager * QfDigitizingLayerModel::markupManager () const
```




<hr>



### function project [2/2]

_The project from which map layers will be taken from._ 
```C++
QgsProject * QfDigitizingLayerModel::project () const
```




<hr>



### function setEnabled 

```C++
void QfDigitizingLayerModel::setEnabled (
    bool enabled
) 
```



The enabled state of the model. 

**Note:**

When disabled, any changes to the project's map layers registry and markup manager will be ignored   





        

<hr>



### function setMarkupManager 

```C++
void QfDigitizingLayerModel::setMarkupManager (
    QfMarkupManager * markupManager
) 
```




<hr>



### function setProject 

_The project from which map layers will be taken from._ 
```C++
void QfDigitizingLayerModel::setProject (
    QgsProject * project
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfdigitizinglayermodel.h`

