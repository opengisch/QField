

# Class DrawingTemplateModel



[**ClassList**](annotated.md) **>** [**DrawingTemplateModel**](classDrawingTemplateModel.md)








Inherits the following classes: QAbstractListModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Role**](#enum-role)  <br> |
| enum  | [**TemplateType**](#enum-templatetype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasProjectTemplate**](classDrawingTemplateModel.md#property-hasprojecttemplate-12)  <br> |
| property QString | [**projectFilePath**](classDrawingTemplateModel.md#property-projectfilepath-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasProjectTemplateChanged**](classDrawingTemplateModel.md#signal-hasprojecttemplatechanged)  <br> |
| signal void | [**projectFilePathChanged**](classDrawingTemplateModel.md#signal-projectfilepathchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DrawingTemplateModel**](#function-drawingtemplatemodel) (QObject \* parent=nullptr) <br> |
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
enum DrawingTemplateModel::Role {
    TemplateTypeRole = Qt::UserRole,
    TemplateTitleRole,
    TemplatePathRole
};
```




<hr>



### enum TemplateType 

```C++
enum DrawingTemplateModel::TemplateType {
    AppTemplate,
    ProjectTemplate
};
```




<hr>
## Public Properties Documentation




### property hasProjectTemplate [1/2]

```C++
bool DrawingTemplateModel::hasProjectTemplate;
```



This property holds whether the model contains project templates. 


        

<hr>



### property projectFilePath [1/2]

```C++
QString DrawingTemplateModel::projectFilePath;
```



This property holds the project file path where project templates will be looked for. 


        

<hr>
## Public Signals Documentation




### signal hasProjectTemplateChanged 

```C++
void DrawingTemplateModel::hasProjectTemplateChanged;
```




<hr>



### signal projectFilePathChanged 

```C++
void DrawingTemplateModel::projectFilePathChanged;
```




<hr>
## Public Functions Documentation




### function DrawingTemplateModel 

```C++
explicit DrawingTemplateModel::DrawingTemplateModel (
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
QVariant DrawingTemplateModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function hasProjectTemplate [2/2]

```C++
bool DrawingTemplateModel::hasProjectTemplate () const
```



This property holds whether the model contains project templates.   


        

<hr>



### function projectFilePath [2/2]

```C++
QString DrawingTemplateModel::projectFilePath () const
```



This property holds the project file path where project templates will be looked for.   


        

<hr>



### function reloadModel 

```C++
Q_INVOKABLE void DrawingTemplateModel::reloadModel () 
```



Reloads the drawing template model. 


        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > DrawingTemplateModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int DrawingTemplateModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function setProjectFilePath 

```C++
void DrawingTemplateModel::setProjectFilePath (
    const QString & path
) 
```



This property holds the project file path where project templates will be looked for.   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/drawingtemplatemodel.h`

