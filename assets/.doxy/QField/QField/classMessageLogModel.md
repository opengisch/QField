

# Class MessageLogModel



[**ClassList**](annotated.md) **>** [**MessageLogModel**](classMessageLogModel.md)



[More...](#detailed-description)

* `#include <messagelogmodel.h>`



Inherits the following classes: QAbstractListModel


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MessageLogModel**](#function-messagelogmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br>_Clears any messages from the log._  |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  Q\_INVOKABLE void | [**suppress**](#function-suppress) (const QVariantMap & filters) <br>_activates suppression of messages with specific tag and an expression_  |
|  Q\_INVOKABLE void | [**unsuppress**](#function-unsuppress) (const QVariantMap & filters) <br>_deactivates suppression of messages with specific tag and an expression_  |




























## Detailed Description


This model will connect to the message log and publish any messages received from there. 


    
## Public Functions Documentation




### function MessageLogModel 

```C++
explicit MessageLogModel::MessageLogModel (
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

_Clears any messages from the log._ 
```C++
Q_INVOKABLE void MessageLogModel::clear () 
```




<hr>



### function data 

```C++
QVariant MessageLogModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > MessageLogModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int MessageLogModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function suppress 

_activates suppression of messages with specific tag and an expression_ 
```C++
Q_INVOKABLE void MessageLogModel::suppress (
    const QVariantMap & filters
) 
```




<hr>



### function unsuppress 

_deactivates suppression of messages with specific tag and an expression_ 
```C++
Q_INVOKABLE void MessageLogModel::unsuppress (
    const QVariantMap & filters
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/messagelogmodel.h`

