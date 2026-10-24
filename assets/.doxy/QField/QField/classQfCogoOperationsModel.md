

# Class QfCogoOperationsModel



[**ClassList**](annotated.md) **>** [**QfCogoOperationsModel**](classQfCogoOperationsModel.md)



_A model holding the list of COGO operations available in the COGO registry._ 

* `#include <qfcogooperationsmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**OperationRoles**](#enum-operationroles)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoOperationsModel**](#function-qfcogooperationsmodel) (QObject \* parent=nullptr) <br> |
|  void | [**buildModel**](#function-buildmodel) () <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE QVariantMap | [**get**](#function-get-12) (int row) const<br> |
|  Q\_INVOKABLE QVariantMap | [**get**](#function-get-22) (const QString & name) const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |




























## Public Types Documentation




### enum OperationRoles 

```C++
enum QfCogoOperationsModel::OperationRoles {
    NameRole = Qt::UserRole + 1,
    DisplayNameRole,
    IconRole
};
```




<hr>
## Public Functions Documentation




### function QfCogoOperationsModel 

```C++
explicit QfCogoOperationsModel::QfCogoOperationsModel (
    QObject * parent=nullptr
) 
```



The COGO operations model constructor. 


        

<hr>



### function buildModel 

```C++
void QfCogoOperationsModel::buildModel () 
```



Builds the model against the COGO operations available in the COGO registry. 


        

<hr>



### function data 

```C++
QVariant QfCogoOperationsModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function get [1/2]

```C++
Q_INVOKABLE QVariantMap QfCogoOperationsModel::get (
    int row
) const
```



Returns a COGO operation data map for a given _row_. 


        

<hr>



### function get [2/2]

```C++
Q_INVOKABLE QVariantMap QfCogoOperationsModel::get (
    const QString & name
) const
```



Returns a COGO operation data map matching the given _name_. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfCogoOperationsModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfCogoOperationsModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperationsmodel.h`

