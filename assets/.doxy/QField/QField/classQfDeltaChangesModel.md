

# Class QfDeltaChangesModel



[**ClassList**](annotated.md) **>** [**QfDeltaChangesModel**](classQfDeltaChangesModel.md)



[More...](#detailed-description)

* `#include <qfdeltachangesmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |
| enum  | [**DeltaMethod**](#enum-deltamethod)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](classQfDeltaChangesModel.md#property-deltafilewrapper-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**deltaFileWrapperChanged**](classQfDeltaChangesModel.md#signal-deltafilewrapperchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDeltaChangesModel**](#function-qfdeltachangesmodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br>_Returns the data at given_ _index_ _with given__role_ _._ |
|  [**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* | [**deltaFileWrapper**](#function-deltafilewrapper-22) () const<br>_Returns the attached delta file wrapper, if any._  |
|  Q\_INVOKABLE int | [**layerChangesCount**](#function-layerchangescount) (const QString & layerId) const<br>_Returns how many pending changes the layer with the given_ _layerId_ _carries._ |
|  Q\_INVOKABLE Qgis::GeometryType | [**layerGeometryType**](#function-layergeometrytype) (const QString & layerId) const<br>_Returns the geometry type of the layer with the given_ _layerId_ _._ |
|  Q\_INVOKABLE QString | [**layerName**](#function-layername) (const QString & layerId) const<br>_Returns the name of the layer with the given_ _layerId_ _._ |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br>_Returns the model role names._  |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br>_Returns number of rows._  |
|  void | [**setDeltaFileWrapper**](#function-setdeltafilewrapper) ([**QfDeltaFileWrapper**](classQfDeltaFileWrapper.md) \* deltaFileWrapper) <br>_Attaches the_ _deltaFileWrapper_ _the rows are read from, pass NULLPTR to detach and clear the model._ |




























## Detailed Description


A list model of the deltas not uploaded to QFieldCloud yet, one row per changed feature, sorted so that the rows of a layer are adjacent and a view can section them by LayerId. 


    
## Public Types Documentation




### enum ColumnRole 

```C++
enum QfDeltaChangesModel::ColumnRole {
    LayerIdRole = Qt::UserRole + 1,
    MethodRole,
    FeatureNameRole,
    AttributeChangesRole,
    HasGeometryChangeRole
};
```




<hr>



### enum DeltaMethod 

```C++
enum QfDeltaChangesModel::DeltaMethod {
    UnknownMethod,
    CreateMethod,
    PatchMethod,
    DeleteMethod
};
```




<hr>
## Public Properties Documentation




### property deltaFileWrapper [1/2]

```C++
QfDeltaFileWrapper * QfDeltaChangesModel::deltaFileWrapper;
```




<hr>
## Public Signals Documentation




### signal deltaFileWrapperChanged 

```C++
void QfDeltaChangesModel::deltaFileWrapperChanged;
```




<hr>
## Public Functions Documentation




### function QfDeltaChangesModel 

```C++
explicit QfDeltaChangesModel::QfDeltaChangesModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

_Returns the data at given_ _index_ _with given__role_ _._
```C++
QVariant QfDeltaChangesModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deltaFileWrapper [2/2]

_Returns the attached delta file wrapper, if any._ 
```C++
QfDeltaFileWrapper * QfDeltaChangesModel::deltaFileWrapper () const
```




<hr>



### function layerChangesCount 

_Returns how many pending changes the layer with the given_ _layerId_ _carries._
```C++
Q_INVOKABLE int QfDeltaChangesModel::layerChangesCount (
    const QString & layerId
) const
```




<hr>



### function layerGeometryType 

_Returns the geometry type of the layer with the given_ _layerId_ _._
```C++
Q_INVOKABLE Qgis::GeometryType QfDeltaChangesModel::layerGeometryType (
    const QString & layerId
) const
```




<hr>



### function layerName 

_Returns the name of the layer with the given_ _layerId_ _._
```C++
Q_INVOKABLE QString QfDeltaChangesModel::layerName (
    const QString & layerId
) const
```




<hr>



### function roleNames 

_Returns the model role names._ 
```C++
QHash< int, QByteArray > QfDeltaChangesModel::roleNames () override const
```




<hr>



### function rowCount 

_Returns number of rows._ 
```C++
int QfDeltaChangesModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setDeltaFileWrapper 

_Attaches the_ _deltaFileWrapper_ _the rows are read from, pass NULLPTR to detach and clear the model._
```C++
void QfDeltaChangesModel::setDeltaFileWrapper (
    QfDeltaFileWrapper * deltaFileWrapper
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/qfdeltachangesmodel.h`

