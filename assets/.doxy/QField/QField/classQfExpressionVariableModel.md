

# Class QfExpressionVariableModel



[**ClassList**](annotated.md) **>** [**QfExpressionVariableModel**](classQfExpressionVariableModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |
| enum  | [**VariableScope**](#enum-variablescope)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsProject \* | [**currentProject**](classQfExpressionVariableModel.md#property-currentproject-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentProjectChanged**](classQfExpressionVariableModel.md#signal-currentprojectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfExpressionVariableModel**](#function-qfexpressionvariablemodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE int | [**addVariable**](#function-addvariable) (VariableScope scope, const QString & name, const QString & value) <br> |
|  QgsProject \* | [**currentProject**](#function-currentproject-22) () const<br> |
|  Q\_INVOKABLE void | [**reloadVariables**](#function-reloadvariables) () <br> |
|  Q\_INVOKABLE void | [**removeVariable**](#function-removevariable) (VariableScope scope, const QString & name) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  Q\_INVOKABLE void | [**save**](#function-save) () <br> |
|  void | [**setCurrentProject**](#function-setcurrentproject) (QgsProject \* project) <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |




























## Public Types Documentation




### enum Roles 

```C++
enum QfExpressionVariableModel::Roles {
    VariableEditableRole = Qt::EditRole,
    VariableNameRole = Qt::UserRole,
    VariableValueRole = Qt::UserRole + 1,
    VariableScopeRole = Qt::UserRole + 2,
    VariableOriginalNameRole = Qt::UserRole + 3
};
```




<hr>



### enum VariableScope 

```C++
enum QfExpressionVariableModel::VariableScope {
    GlobalScope,
    ProjectScope
};
```




<hr>
## Public Properties Documentation




### property currentProject [1/2]

```C++
QgsProject * QfExpressionVariableModel::currentProject;
```




<hr>
## Public Signals Documentation




### signal currentProjectChanged 

```C++
void QfExpressionVariableModel::currentProjectChanged;
```




<hr>
## Public Functions Documentation




### function QfExpressionVariableModel 

```C++
explicit QfExpressionVariableModel::QfExpressionVariableModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addVariable 

```C++
Q_INVOKABLE int QfExpressionVariableModel::addVariable (
    VariableScope scope,
    const QString & name,
    const QString & value
) 
```




<hr>



### function currentProject [2/2]

```C++
QgsProject * QfExpressionVariableModel::currentProject () const
```



Returns the current project used to retrieve variables from. 


        

<hr>



### function reloadVariables 

```C++
Q_INVOKABLE void QfExpressionVariableModel::reloadVariables () 
```




<hr>



### function removeVariable 

```C++
Q_INVOKABLE void QfExpressionVariableModel::removeVariable (
    VariableScope scope,
    const QString & name
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfExpressionVariableModel::roleNames () override const
```




<hr>



### function save 

```C++
Q_INVOKABLE void QfExpressionVariableModel::save () 
```




<hr>



### function setCurrentProject 

```C++
void QfExpressionVariableModel::setCurrentProject (
    QgsProject * project
) 
```



Sets the project used to retrieve variables from. 


        

<hr>



### function setData 

```C++
bool QfExpressionVariableModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfexpressionvariablemodel.h`

