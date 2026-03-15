

# Class LocatorActionsModel



[**ClassList**](annotated.md) **>** [**LocatorActionsModel**](classLocatorActionsModel.md)



[More...](#detailed-description)

* `#include <locatormodelsuperbridge.h>`



Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionRoles**](#enum-actionroles)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**LocatorActionsModel**](#function-locatoractionsmodel-12) (QObject \* parent=nullptr) <br> |
|   | [**LocatorActionsModel**](#function-locatoractionsmodel-22) (int rows, int columns, QObject \* parent=nullptr) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |




























## Detailed Description


[**LocatorActionsModel**](classLocatorActionsModel.md) is a model used to dislay additional actions from the result of a filter. These are displayed as contextual menu in QGIS while they are shown in a row of buttons in QField. 


    
## Public Types Documentation




### enum ActionRoles 

```C++
enum LocatorActionsModel::ActionRoles {
    IdRole = Qt::UserRole + 1,
    IconPathRole
};
```




<hr>
## Public Functions Documentation




### function LocatorActionsModel [1/2]

```C++
explicit LocatorActionsModel::LocatorActionsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function LocatorActionsModel [2/2]

```C++
LocatorActionsModel::LocatorActionsModel (
    int rows,
    int columns,
    QObject * parent=nullptr
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > LocatorActionsModel::roleNames () override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/locator/locatormodelsuperbridge.h`

