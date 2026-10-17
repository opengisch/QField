

# Class QfGeometryEditorsModel



[**ClassList**](annotated.md) **>** [**QfGeometryEditorsModel**](classQfGeometryEditorsModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionRoles**](#enum-actionroles)  <br> |
| enum  | [**GeometryType**](#enum-geometrytype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](classQfGeometryEditorsModel.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**vertexModelChanged**](classQfGeometryEditorsModel.md#signal-vertexmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGeometryEditorsModel**](#function-qfgeometryeditorsmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addEditor**](#function-addeditor) (const QString & name, const QString & icon, const QString & toolbar, SupportedGeometries supportedGeometries=AllGeometries) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**QfVertexModel**](classQfVertexModel.md) \* vertexModel) <br> |
|  [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**supportsGeometry**](#function-supportsgeometry) (const QgsGeometry & geometry, const SupportedGeometries & supportedGeometries) <br> |


























## Public Types Documentation




### enum ActionRoles 

```C++
enum QfGeometryEditorsModel::ActionRoles {
    NameRole = Qt::UserRole + 1,
    IconPathRole,
    ToolbarRole,
    SupportedGeometriesRole
};
```




<hr>



### enum GeometryType 

```C++
enum QfGeometryEditorsModel::GeometryType {
    Point = 1,
    Line = 2,
    Polygon = 4,
    AllGeometries = Point | Line | Polygon
};
```




<hr>
## Public Properties Documentation




### property vertexModel [1/2]

```C++
QfVertexModel * QfGeometryEditorsModel::vertexModel;
```




<hr>
## Public Signals Documentation




### signal vertexModelChanged 

```C++
void QfGeometryEditorsModel::vertexModelChanged;
```




<hr>
## Public Functions Documentation




### function QfGeometryEditorsModel 

```C++
explicit QfGeometryEditorsModel::QfGeometryEditorsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addEditor 

```C++
Q_INVOKABLE void QfGeometryEditorsModel::addEditor (
    const QString & name,
    const QString & icon,
    const QString & toolbar,
    SupportedGeometries supportedGeometries=AllGeometries
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfGeometryEditorsModel::roleNames () override const
```




<hr>



### function setVertexModel 

```C++
void QfGeometryEditorsModel::setVertexModel (
    QfVertexModel * vertexModel
) 
```




<hr>



### function vertexModel [2/2]

```C++
inline QfVertexModel * QfGeometryEditorsModel::vertexModel () const
```




<hr>
## Public Static Functions Documentation




### function supportsGeometry 

```C++
static Q_INVOKABLE bool QfGeometryEditorsModel::supportsGeometry (
    const QgsGeometry & geometry,
    const SupportedGeometries & supportedGeometries
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfgeometryeditorsmodel.h`

