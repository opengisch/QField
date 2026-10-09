

# Class QfSerialPortModel



[**ClassList**](annotated.md) **>** [**QfSerialPortModel**](classQfSerialPortModel.md)



[More...](#detailed-description)

* `#include <qfserialportmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br>_The roles provided by this model._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSerialPortModel**](#function-qfserialportmodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role=Qt::DisplayRole) override const<br> |
|  Q\_INVOKABLE int | [**findIndexFromName**](#function-findindexfromname) (const QString & name) const<br> |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |




























## Detailed Description


A model that provides serial ports available on the device. 


    
## Public Types Documentation




### enum Roles 

_The roles provided by this model._ 
```C++
enum QfSerialPortModel::Roles {
    PortNameRole = Qt::UserRole + 1,
    DescriptionRole
};
```




<hr>
## Public Functions Documentation




### function QfSerialPortModel 

```C++
explicit QfSerialPortModel::QfSerialPortModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfSerialPortModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function findIndexFromName 

```C++
Q_INVOKABLE int QfSerialPortModel::findIndexFromName (
    const QString & name
) const
```



Returns the row index for a given serial port _name_. 


        

<hr>



### function refresh 

```C++
Q_INVOKABLE void QfSerialPortModel::refresh () 
```



Refresh the list of available serial ports. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfSerialPortModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfSerialPortModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfserialportmodel.h`

