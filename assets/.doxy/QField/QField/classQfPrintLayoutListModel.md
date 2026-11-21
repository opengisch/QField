

# Class QfPrintLayoutListModel



[**ClassList**](annotated.md) **>** [**QfPrintLayoutListModel**](classQfPrintLayoutListModel.md)








Inherits the following classes: QAbstractListModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsVectorLayer \* | [**atlasCoverageLayer**](classQfPrintLayoutListModel.md#property-atlascoveragelayer-12)  <br> |
| property QgsProject \* | [**project**](classQfPrintLayoutListModel.md#property-project-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**atlasCoverageLayerChanged**](classQfPrintLayoutListModel.md#signal-atlascoveragelayerchanged)  <br> |
| signal void | [**projectChanged**](classQfPrintLayoutListModel.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfPrintLayoutListModel**](#function-qfprintlayoutlistmodel) (QObject \* parent=nullptr) <br> |
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
QgsVectorLayer * QfPrintLayoutListModel::atlasCoverageLayer;
```




<hr>



### property project [1/2]

```C++
QgsProject * QfPrintLayoutListModel::project;
```




<hr>
## Public Signals Documentation




### signal atlasCoverageLayerChanged 

```C++
void QfPrintLayoutListModel::atlasCoverageLayerChanged;
```




<hr>



### signal projectChanged 

```C++
void QfPrintLayoutListModel::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfPrintLayoutListModel 

```C++
explicit QfPrintLayoutListModel::QfPrintLayoutListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function atlasCoverageLayer [2/2]

_Returns the vector layer used to filter layouts to match atlas coverage layer._ 
```C++
QgsVectorLayer * QfPrintLayoutListModel::atlasCoverageLayer () const
```




<hr>



### function data 

```C++
QVariant QfPrintLayoutListModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function project [2/2]

_Returns the current project from which layouts are retrieved._ 
```C++
QgsProject * QfPrintLayoutListModel::project () const
```




<hr>



### function reloadModel 

_triggered when set another project_ 
```C++
Q_INVOKABLE void QfPrintLayoutListModel::reloadModel () 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfPrintLayoutListModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfPrintLayoutListModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setAtlasCoverageLayer 

_Sets the vector layer used to filter layouts to match atlas coverage layer._ 
```C++
void QfPrintLayoutListModel::setAtlasCoverageLayer (
    QgsVectorLayer * layer
) 
```




<hr>



### function setProject 

_Sets the project to be used to retrieve layouts._ 
```C++
void QfPrintLayoutListModel::setProject (
    QgsProject * project
) 
```




<hr>



### function titleAt 

```C++
Q_INVOKABLE const QString QfPrintLayoutListModel::titleAt (
    int row
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfprintlayoutlistmodel.h`

