

# Class ExpressionVariableModel



[**ClassList**](annotated.md) **>** [**ExpressionVariableModel**](classExpressionVariableModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |
| enum  | [**VariableScope**](#enum-variablescope)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsProject \* | [**currentProject**](classExpressionVariableModel.md#property-currentproject-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentProjectChanged**](classExpressionVariableModel.md#signal-currentprojectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionVariableModel**](#function-expressionvariablemodel) (QObject \* parent=nullptr) <br> |
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
enum ExpressionVariableModel::Roles {
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
enum ExpressionVariableModel::VariableScope {
    GlobalScope,
    ProjectScope
};
```




<hr>
## Public Properties Documentation




### property currentProject [1/2]

```C++
QgsProject * ExpressionVariableModel::currentProject;
```




<hr>
## Public Signals Documentation




### signal currentProjectChanged 

```C++
void ExpressionVariableModel::currentProjectChanged;
```




<hr>
## Public Functions Documentation




### function ExpressionVariableModel 

```C++
explicit ExpressionVariableModel::ExpressionVariableModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addVariable 

```C++
Q_INVOKABLE int ExpressionVariableModel::addVariable (
    VariableScope scope,
    const QString & name,
    const QString & value
) 
```




<hr>



### function currentProject [2/2]

```C++
QgsProject * ExpressionVariableModel::currentProject () const
```



Returns the current project used to retrieve variables from. 


        

<hr>



### function reloadVariables 

```C++
Q_INVOKABLE void ExpressionVariableModel::reloadVariables () 
```




<hr>



### function removeVariable 

```C++
Q_INVOKABLE void ExpressionVariableModel::removeVariable (
    VariableScope scope,
    const QString & name
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > ExpressionVariableModel::roleNames () override const
```




<hr>



### function save 

```C++
Q_INVOKABLE void ExpressionVariableModel::save () 
```




<hr>



### function setCurrentProject 

```C++
void ExpressionVariableModel::setCurrentProject (
    QgsProject * project
) 
```



Sets the project used to retrieve variables from. 


        

<hr>



### function setData 

```C++
bool ExpressionVariableModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/expressionvariablemodel.h`

