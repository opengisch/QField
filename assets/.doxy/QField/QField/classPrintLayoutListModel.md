

# Class PrintLayoutListModel



[**ClassList**](annotated.md) **>** [**PrintLayoutListModel**](classPrintLayoutListModel.md)








Inherits the following classes: QAbstractListModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**atlasCoverageLayer**](classPrintLayoutListModel.md#property-atlascoveragelayer-12)  <br> |
| property QgsProject \* | [**project**](classPrintLayoutListModel.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**atlasCoverageLayerChanged**](classPrintLayoutListModel.md#signal-atlascoveragelayerchanged)  <br> |
| signal void | [**projectChanged**](classPrintLayoutListModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**PrintLayoutListModel**](#function-printlayoutlistmodel) (QObject \* parent=nullptr) <br> |
|  QgsVectorLayer \* | [**atlasCoverageLayer**](#function-atlascoveragelayer-22) () const<br>_Returns the vector layer used to filter layouts to match atlas coverage layer._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the current project from which layouts are retrieved._  |
|  Q\_INVOKABLE void | [**reloadModel**](#function-reloadmodel) () <br>_triggered when set another project_  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setAtlasCoverageLayer**](#function-setatlascoveragelayer) (QgsVectorLayer \* layer) <br>_Sets the vector layer used to filter layouts to match atlas coverage layer._  |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the project to be used to retrieve layouts._  |
|  Q\_INVOKABLE const QString | [**titleAt**](#function-titleat) (int row) const<br> |




























## Public Properties Documentation




### property atlasCoverageLayer [1/2]

```C++
QgsVectorLayer * PrintLayoutListModel::atlasCoverageLayer;
```




<hr>



### property project [1/2]

```C++
QgsProject * PrintLayoutListModel::project;
```




<hr>
## Public Signals Documentation




### signal atlasCoverageLayerChanged 

```C++
void PrintLayoutListModel::atlasCoverageLayerChanged;
```




<hr>



### signal projectChanged 

```C++
void PrintLayoutListModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function PrintLayoutListModel 

```C++
explicit PrintLayoutListModel::PrintLayoutListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function atlasCoverageLayer [2/2]

_Returns the vector layer used to filter layouts to match atlas coverage layer._ 
```C++
QgsVectorLayer * PrintLayoutListModel::atlasCoverageLayer () const
```




<hr>



### function data 

```C++
QVariant PrintLayoutListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function project [2/2]

_Returns the current project from which layouts are retrieved._ 
```C++
QgsProject * PrintLayoutListModel::project () const
```




<hr>



### function reloadModel 

_triggered when set another project_ 
```C++
Q_INVOKABLE void PrintLayoutListModel::reloadModel () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > PrintLayoutListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int PrintLayoutListModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setAtlasCoverageLayer 

_Sets the vector layer used to filter layouts to match atlas coverage layer._ 
```C++
void PrintLayoutListModel::setAtlasCoverageLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setProject 

_Sets the project to be used to retrieve layouts._ 
```C++
void PrintLayoutListModel::setProject (
    QgsProject * project
) 
```




<hr>



### function titleAt 

```C++
Q_INVOKABLE const QString PrintLayoutListModel::titleAt (
    int row
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/printlayoutlistmodel.h`

