

# Class GeometryEditorsModel



[**ClassList**](annotated.md) **>** [**GeometryEditorsModel**](classGeometryEditorsModel.md)








Inherits the following classes: QStandardItemModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ActionRoles**](#enum-actionroles)  <br> |
| enum  | [**GeometryType**](#enum-geometrytype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](classGeometryEditorsModel.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**vertexModelChanged**](classGeometryEditorsModel.md#signal-vertexmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GeometryEditorsModel**](#function-geometryeditorsmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addEditor**](#function-addeditor) (const QString & name, const QString & icon, const QString & toolbar, SupportedGeometries supportedGeometries=AllGeometries) <br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**VertexModel**](classVertexModel.md) \* vertexModel) <br> |
|  [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () const<br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE bool | [**supportsGeometry**](#function-supportsgeometry) (const QgsGeometry & geometry, const SupportedGeometries & supportedGeometries) <br> |


























## Public Types Documentation




### enum ActionRoles 

```C++
enum GeometryEditorsModel::ActionRoles {
    NameRole = Qt::UserRole + 1,
    IconPathRole,
    ToolbarRole,
    SupportedGeometriesRole
};
```




<hr>



### enum GeometryType 

```C++
enum GeometryEditorsModel::GeometryType {
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
VertexModel * GeometryEditorsModel::vertexModel;
```




<hr>
## Public Signals Documentation




### signal vertexModelChanged 

```C++
void GeometryEditorsModel::vertexModelChanged;
```




<hr>
## Public Functions Documentation




### function GeometryEditorsModel 

```C++
explicit GeometryEditorsModel::GeometryEditorsModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addEditor 

```C++
Q_INVOKABLE void GeometryEditorsModel::addEditor (
    const QString & name,
    const QString & icon,
    const QString & toolbar,
    SupportedGeometries supportedGeometries=AllGeometries
) 
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > GeometryEditorsModel::roleNames () override const
```




<hr>



### function setVertexModel 

```C++
void GeometryEditorsModel::setVertexModel (
    VertexModel * vertexModel
) 
```




<hr>



### function vertexModel [2/2]

```C++
inline VertexModel * GeometryEditorsModel::vertexModel () const
```




<hr>
## Public Static Functions Documentation




### function supportsGeometry 

```C++
static Q_INVOKABLE bool GeometryEditorsModel::supportsGeometry (
    const QgsGeometry & geometry,
    const SupportedGeometries & supportedGeometries
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/geometryeditorsmodel.h`

