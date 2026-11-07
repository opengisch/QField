

# Class QfLocatorActionsModel



[**ClassList**](annotated.md) **>** [**QfLocatorActionsModel**](classQfLocatorActionsModel.md)



[More...](#detailed-description)

* `#include <qflocatormodelsuperbridge.h>`



Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionRoles**](#enum-actionroles)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfLocatorActionsModel**](#function-qflocatoractionsmodel-12) (QObject \* parent=nullptr) <br> |
|   | [**QfLocatorActionsModel**](#function-qflocatoractionsmodel-22) (int rows, int columns, QObject \* parent=nullptr) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |




























## Detailed Description


[**QfLocatorActionsModel**](classQfLocatorActionsModel.md) is a model used to dislay additional actions from the result of a filter. These are displayed as contextual menu in QGIS while they are shown in a row of buttons in QField. 


    
## Public Types Documentation




### enum ActionRoles 

```C++
enum QfLocatorActionsModel::ActionRoles {
    IdRole = Qt::UserRole + 1,
    IconPathRole
};
```




<hr>
## Public Functions Documentation




### function QfLocatorActionsModel [1/2]

```C++
explicit QfLocatorActionsModel::QfLocatorActionsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function QfLocatorActionsModel [2/2]

```C++
QfLocatorActionsModel::QfLocatorActionsModel (
    int rows,
    int columns,
    QObject * parent=nullptr
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfLocatorActionsModel::roleNames () override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/qflocatormodelsuperbridge.h`

