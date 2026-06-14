

# Class SerialPortModel



[**ClassList**](annotated.md) **>** [**SerialPortModel**](classSerialPortModel.md)



[More...](#detailed-description)

* `#include <serialportmodel.h>`



Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br>_The roles provided by this model._  |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SerialPortModel**](#function-serialportmodel) (QObject \* parent=nullptr) <br> |
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
enum SerialPortModel::Roles {
    PortNameRole = Qt::UserRole + 1,
    DescriptionRole
};
```




<hr>
## Public Functions Documentation




### function SerialPortModel 

```C++
explicit SerialPortModel::SerialPortModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant SerialPortModel::data (
    const QModelIndex & index,
    int role=Qt::DisplayRole
) override const
```




<hr>



### function findIndexFromName 

```C++
Q_INVOKABLE int SerialPortModel::findIndexFromName (
    const QString & name
) const
```



Returns the row index for a given serial port _name_. 


        

<hr>



### function refresh 

```C++
Q_INVOKABLE void SerialPortModel::refresh () 
```



Refresh the list of available serial ports. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > SerialPortModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int SerialPortModel::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/serialportmodel.h`

