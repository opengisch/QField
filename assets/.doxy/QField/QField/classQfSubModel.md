

# Class QfSubModel



[**ClassList**](annotated.md) **>** [**QfSubModel**](classQfSubModel.md)








Inherits the following classes: QAbstractItemModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classQfSubModel.md#property-enabled-12)  <br> |
| property QAbstractItemModel \* | [**model**](classQfSubModel.md#property-model-12)  <br> |
| property QModelIndex | [**rootIndex**](classQfSubModel.md#property-rootindex-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classQfSubModel.md#signal-enabledchanged)  <br> |
| signal void | [**modelChanged**](classQfSubModel.md#signal-modelchanged)  <br> |
| signal void | [**rootIndexChanged**](classQfSubModel.md#signal-rootindexchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSubModel**](#function-qfsubmodel) (QObject \* parent=nullptr) <br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent) override const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**enabled**](#function-enabled-22) () const<br> |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent) override const<br> |
|  bool | [**isInSubModel**](#function-isinsubmodel) (const QModelIndex & sourceIndex) const<br> |
|  QAbstractItemModel \* | [**model**](#function-model-22) () const<br> |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  QModelIndex | [**rootIndex**](#function-rootindex-22) () const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br> |
|  void | [**setModel**](#function-setmodel) (QAbstractItemModel \* model) <br> |
|  void | [**setRootIndex**](#function-setrootindex) (const QModelIndex & rootIndex) <br> |




























## Public Properties Documentation




### property enabled [1/2]

```C++
bool QfSubModel::enabled;
```




<hr>



### property model [1/2]

```C++
QAbstractItemModel * QfSubModel::model;
```




<hr>



### property rootIndex [1/2]

```C++
QModelIndex QfSubModel::rootIndex;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void QfSubModel::enabledChanged;
```




<hr>



### signal modelChanged 

```C++
void QfSubModel::modelChanged;
```




<hr>



### signal rootIndexChanged 

```C++
void QfSubModel::rootIndexChanged;
```




<hr>
## Public Functions Documentation




### function QfSubModel 

```C++
explicit QfSubModel::QfSubModel (
    QObject * parent=nullptr
) 
```




<hr>



### function columnCount 

```C++
int QfSubModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function data 

```C++
QVariant QfSubModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function enabled [2/2]

```C++
inline bool QfSubModel::enabled () const
```




<hr>



### function index 

```C++
QModelIndex QfSubModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function isInSubModel 

```C++
bool QfSubModel::isInSubModel (
    const QModelIndex & sourceIndex
) const
```




<hr>



### function model [2/2]

```C++
QAbstractItemModel * QfSubModel::model () const
```




<hr>



### function parent 

```C++
QModelIndex QfSubModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfSubModel::roleNames () override const
```




<hr>



### function rootIndex [2/2]

```C++
QModelIndex QfSubModel::rootIndex () const
```




<hr>



### function rowCount 

```C++
int QfSubModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setData 

```C++
bool QfSubModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setEnabled 

```C++
void QfSubModel::setEnabled (
    bool enabled
) 
```




<hr>



### function setModel 

```C++
void QfSubModel::setModel (
    QAbstractItemModel * model
) 
```




<hr>



### function setRootIndex 

```C++
void QfSubModel::setRootIndex (
    const QModelIndex & rootIndex
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfsubmodel.h`

