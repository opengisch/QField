

# Class SubModel



[**ClassList**](annotated.md) **>** [**SubModel**](classSubModel.md)








Inherits the following classes: QAbstractItemModel


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**enabled**](classSubModel.md#property-enabled-12)  <br> |
| property QAbstractItemModel \* | [**model**](classSubModel.md#property-model-12)  <br> |
| property QModelIndex | [**rootIndex**](classSubModel.md#property-rootindex-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**enabledChanged**](classSubModel.md#signal-enabledchanged)  <br> |
| signal void | [**modelChanged**](classSubModel.md#signal-modelchanged)  <br> |
| signal void | [**rootIndexChanged**](classSubModel.md#signal-rootindexchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SubModel**](#function-submodel) (QObject \* parent=nullptr) <br> |
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
bool SubModel::enabled;
```




<hr>



### property model [1/2]

```C++
QAbstractItemModel * SubModel::model;
```




<hr>



### property rootIndex [1/2]

```C++
QModelIndex SubModel::rootIndex;
```




<hr>
## Public Signals Documentation




### signal enabledChanged 

```C++
void SubModel::enabledChanged;
```




<hr>



### signal modelChanged 

```C++
void SubModel::modelChanged;
```




<hr>



### signal rootIndexChanged 

```C++
void SubModel::rootIndexChanged;
```




<hr>
## Public Functions Documentation




### function SubModel 

```C++
explicit SubModel::SubModel (
    QObject * parent=nullptr
) 
```




<hr>



### function columnCount 

```C++
int SubModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function data 

```C++
QVariant SubModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function enabled [2/2]

```C++
inline bool SubModel::enabled () const
```




<hr>



### function index 

```C++
QModelIndex SubModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function isInSubModel 

```C++
bool SubModel::isInSubModel (
    const QModelIndex & sourceIndex
) const
```




<hr>



### function model [2/2]

```C++
QAbstractItemModel * SubModel::model () const
```




<hr>



### function parent 

```C++
QModelIndex SubModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > SubModel::roleNames () override const
```




<hr>



### function rootIndex [2/2]

```C++
QModelIndex SubModel::rootIndex () const
```




<hr>



### function rowCount 

```C++
int SubModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setData 

```C++
bool SubModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setEnabled 

```C++
void SubModel::setEnabled (
    bool enabled
) 
```




<hr>



### function setModel 

```C++
void SubModel::setModel (
    QAbstractItemModel * model
) 
```




<hr>



### function setRootIndex 

```C++
void SubModel::setRootIndex (
    const QModelIndex & rootIndex
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/submodel.h`

