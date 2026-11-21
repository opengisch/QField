

# Class QfSensorListModel



[**ClassList**](annotated.md) **>** [**QfSensorListModel**](classQfSensorListModel.md)








Inherits the following classes: QSortFilterProxyModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsProject \* | [**project**](classQfSensorListModel.md#property-project-12)  <br> |
| property bool | [**showConnectedOnly**](classQfSensorListModel.md#property-showconnectedonly-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**projectChanged**](classQfSensorListModel.md#signal-projectchanged)  <br>_Emitted when the current project has changed._  |
| signal void | [**sensorErrorOccurred**](classQfSensorListModel.md#signal-sensorerroroccurred) (const QString & errorString) <br>_Emitted when a sensor error has occurred. An_ _errorString_ _provides details on the error._ |
| signal void | [**showConnectedOnlyChanged**](classQfSensorListModel.md#signal-showconnectedonlychanged)  <br>_Emitted when the show connected only filter has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSensorListModel**](#function-qfsensorlistmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**connectSensorId**](#function-connectsensorid) (const QString & id) const<br> |
|  Q\_INVOKABLE void | [**disconnectSensorId**](#function-disconnectsensorid) (const QString & id) const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the current project from which sensors are retrieved._  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the project to be used to retrieve sensors._  |
|  void | [**setShowConnectedOnly**](#function-setshowconnectedonly) (bool showConnectedOnly) <br>_Sets whether the model should only show connected sensors._  |
|  bool | [**showConnectedOnly**](#function-showconnectedonly-22) () const<br>_Returns TRUE when the model only shows connected sensors._  |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int source\_row, const QModelIndex & source\_parent) override const<br> |




## Public Properties Documentation




### property project [1/2]

```C++
QgsProject * QfSensorListModel::project;
```




<hr>



### property showConnectedOnly [1/2]

```C++
bool QfSensorListModel::showConnectedOnly;
```




<hr>
## Public Signals Documentation




### signal projectChanged 

_Emitted when the current project has changed._ 
```C++
void QfSensorListModel::projectChanged;
```




<hr>



### signal sensorErrorOccurred 

_Emitted when a sensor error has occurred. An_ _errorString_ _provides details on the error._
```C++
void QfSensorListModel::sensorErrorOccurred;
```




<hr>



### signal showConnectedOnlyChanged 

_Emitted when the show connected only filter has changed._ 
```C++
void QfSensorListModel::showConnectedOnlyChanged;
```




<hr>
## Public Functions Documentation




### function QfSensorListModel 

```C++
explicit QfSensorListModel::QfSensorListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function connectSensorId 

```C++
Q_INVOKABLE void QfSensorListModel::connectSensorId (
    const QString & id
) const
```



Connects a sensor. 

**Parameters:**


* `id` The unique ID string of the sensor 




        

<hr>



### function disconnectSensorId 

```C++
Q_INVOKABLE void QfSensorListModel::disconnectSensorId (
    const QString & id
) const
```



Disconnects a sensor. 

**Parameters:**


* `id` The unique ID string of the sensor 




        

<hr>



### function project [2/2]

_Returns the current project from which sensors are retrieved._ 
```C++
QgsProject * QfSensorListModel::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfSensorListModel::roleNames () override const
```




<hr>



### function setProject 

_Sets the project to be used to retrieve sensors._ 
```C++
void QfSensorListModel::setProject (
    QgsProject * project
) 
```




<hr>



### function setShowConnectedOnly 

_Sets whether the model should only show connected sensors._ 
```C++
void QfSensorListModel::setShowConnectedOnly (
    bool showConnectedOnly
) 
```




<hr>



### function showConnectedOnly [2/2]

_Returns TRUE when the model only shows connected sensors._ 
```C++
inline bool QfSensorListModel::showConnectedOnly () const
```




<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
bool QfSensorListModel::filterAcceptsRow (
    int source_row,
    const QModelIndex & source_parent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfsensorlistmodel.h`

