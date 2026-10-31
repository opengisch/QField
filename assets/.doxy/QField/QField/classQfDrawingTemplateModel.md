

# Class QfDrawingTemplateModel



[**ClassList**](annotated.md) **>** [**QfDrawingTemplateModel**](classQfDrawingTemplateModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br> |
| enum  | [**TemplateType**](#enum-templatetype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasProjectTemplate**](classQfDrawingTemplateModel.md#property-hasprojecttemplate-12)  <br> |
| property QString | [**projectFilePath**](classQfDrawingTemplateModel.md#property-projectfilepath-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasProjectTemplateChanged**](classQfDrawingTemplateModel.md#signal-hasprojecttemplatechanged)  <br> |
| signal void | [**projectFilePathChanged**](classQfDrawingTemplateModel.md#signal-projectfilepathchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDrawingTemplateModel**](#function-qfdrawingtemplatemodel) (QObject \* parent=nullptr) <br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**hasProjectTemplate**](#function-hasprojecttemplate-22) () const<br> |
|  QString | [**projectFilePath**](#function-projectfilepath-22) () const<br> |
|  Q\_INVOKABLE void | [**reloadModel**](#function-reloadmodel) () <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  void | [**setProjectFilePath**](#function-setprojectfilepath) (const QString & path) <br> |




























## Public Types Documentation




### enum Role 

```C++
enum QfDrawingTemplateModel::Role {
    TemplateTypeRole = Qt::UserRole,
    TemplateTitleRole,
    TemplatePathRole
};
```




<hr>



### enum TemplateType 

```C++
enum QfDrawingTemplateModel::TemplateType {
    AppTemplate,
    ProjectTemplate
};
```




<hr>
## Public Properties Documentation




### property hasProjectTemplate [1/2]

```C++
bool QfDrawingTemplateModel::hasProjectTemplate;
```



This property holds whether the model contains project templates. 


        

<hr>



### property projectFilePath [1/2]

```C++
QString QfDrawingTemplateModel::projectFilePath;
```



This property holds the project file path where project templates will be looked for. 


        

<hr>
## Public Signals Documentation




### signal hasProjectTemplateChanged 

```C++
void QfDrawingTemplateModel::hasProjectTemplateChanged;
```




<hr>



### signal projectFilePathChanged 

```C++
void QfDrawingTemplateModel::projectFilePathChanged;
```




<hr>
## Public Functions Documentation




### function QfDrawingTemplateModel 

```C++
explicit QfDrawingTemplateModel::QfDrawingTemplateModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant QfDrawingTemplateModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function hasProjectTemplate [2/2]

```C++
bool QfDrawingTemplateModel::hasProjectTemplate () const
```



This property holds whether the model contains project templates.   


        

<hr>



### function projectFilePath [2/2]

```C++
QString QfDrawingTemplateModel::projectFilePath () const
```



This property holds the project file path where project templates will be looked for.   


        

<hr>



### function reloadModel 

```C++
Q_INVOKABLE void QfDrawingTemplateModel::reloadModel () 
```



Reloads the drawing template model. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfDrawingTemplateModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfDrawingTemplateModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setProjectFilePath 

```C++
void QfDrawingTemplateModel::setProjectFilePath (
    const QString & path
) 
```



This property holds the project file path where project templates will be looked for.   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfdrawingtemplatemodel.h`

