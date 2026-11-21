

# Class QfVertexModel



[**ClassList**](annotated.md) **>** [**QfVertexModel**](classQfVertexModel.md)



[More...](#detailed-description)

* `#include <qfvertexmodel.h>`



Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Vertex**](structQfVertexModel_1_1Vertex.md) <br> |
| struct | [**VertexChange**](structQfVertexModel_1_1VertexChange.md) <br> |


## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**ColumnRole**](#enum-columnrole)  <br> |
| enum  | [**EditingMode**](#enum-editingmode)  <br> |
| enum  | [**PointType**](#enum-pointtype)  <br> |
| enum  | [**VertexChangeType**](#enum-vertexchangetype)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**canAddVertex**](classQfVertexModel.md#property-canaddvertex-12)  <br>_determines if one can add vertex_  |
| property bool | [**canNextVertex**](classQfVertexModel.md#property-cannextvertex-12)  <br>_determines if one can go to next vertex_  |
| property bool | [**canPreviousVertex**](classQfVertexModel.md#property-canpreviousvertex-12)  <br>_determines if one can go to previous vertex_  |
| property bool | [**canRemoveVertex**](classQfVertexModel.md#property-canremovevertex-12)  <br>_determines if one can remove current vertex_  |
| property bool | [**canUndo**](classQfVertexModel.md#property-canundo-12)  <br>_returns TRUE if an undo operation is available_  |
| property QgsCoordinateReferenceSystem | [**crs**](classQfVertexModel.md#property-crs-12)  <br> |
| property QgsPoint | [**currentPoint**](classQfVertexModel.md#property-currentpoint-12)  <br>_The current point being edited._  |
| property int | [**currentVertexIndex**](classQfVertexModel.md#property-currentvertexindex-12)  <br> |
| property bool | [**dirty**](classQfVertexModel.md#property-dirty-12)  <br>_determines if the model has changes_  |
| property bool | [**editingAllowed**](classQfVertexModel.md#property-editingallowed-12)  <br>_determines if the model allows editing the geometry_  |
| property EditingMode | [**editingMode**](classQfVertexModel.md#property-editingmode-12)  <br>_The current mode._  |
| property QgsGeometry | [**geometry**](classQfVertexModel.md#property-geometry-12)  <br> |
| property bool | [**isHovering**](classQfVertexModel.md#property-ishovering)  <br>_determines if the map is currently being hovered (then when moving the map, it will not move directly a vertex if the mode is AddVertex)_  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfVertexModel.md#property-mapsettings-12)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| property int | [**ringCount**](classQfVertexModel.md#property-ringcount-12)  <br>_number of rings in the model_  |
| property int | [**vertexCount**](classQfVertexModel.md#property-vertexcount-12)  <br>_number of vertices in the model_  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**canAddVertexChanged**](classQfVertexModel.md#signal-canaddvertexchanged)  <br>_determines if one can add vertex_  |
| signal void | [**canNextVertexChanged**](classQfVertexModel.md#signal-cannextvertexchanged)  <br>_determines if one can go to next vertex_  |
| signal void | [**canPreviousVertexChanged**](classQfVertexModel.md#signal-canpreviousvertexchanged)  <br>_determines if one can go to previous vertex_  |
| signal void | [**canRemoveVertexChanged**](classQfVertexModel.md#signal-canremovevertexchanged)  <br>_determines if one can remove current vertex_  |
| signal void | [**crsChanged**](classQfVertexModel.md#signal-crschanged)  <br>_Emitted when the coordinate reference system has changed._  |
| signal void | [**currentPointChanged**](classQfVertexModel.md#signal-currentpointchanged)  <br>_The current point being edited._  |
| signal void | [**currentVertexIndexChanged**](classQfVertexModel.md#signal-currentvertexindexchanged)  <br> |
| signal void | [**dirtyChanged**](classQfVertexModel.md#signal-dirtychanged)  <br>_determines if the model has changes_  |
| signal void | [**editingModeChanged**](classQfVertexModel.md#signal-editingmodechanged)  <br>_The current mode._  |
| signal void | [**geometryChanged**](classQfVertexModel.md#signal-geometrychanged)  <br>_Emitted when the geometry has changed._  |
| signal void | [**geometryTypeChanged**](classQfVertexModel.md#signal-geometrytypechanged)  <br>_Returns the geometry type._  |
| signal void | [**historyChanged**](classQfVertexModel.md#signal-historychanged)  <br>_Emitted when the history has been modified._  |
| signal void | [**mapSettingsChanged**](classQfVertexModel.md#signal-mapsettingschanged)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| signal void | [**ringCountChanged**](classQfVertexModel.md#signal-ringcountchanged)  <br>_number of rings in the model_  |
| signal void | [**vertexCountChanged**](classQfVertexModel.md#signal-vertexcountchanged)  <br>_number of vertices in the model_  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfVertexModel**](#function-qfvertexmodel) (QObject \* parent=nullptr) <br> |
|  void | [**addToHistory**](#function-addtohistory) (VertexChangeType type) <br> |
|  Q\_INVOKABLE void | [**addVertexNearestToPosition**](#function-addvertexnearesttoposition) (const QgsPoint & mapPoint) <br>_Adds a new vertex on the segment having its mid-point nearest to the_ _mapPoint_ _._ |
|  bool | [**canAddVertex**](#function-canaddvertex-22) () <br>_determines if one can add vertex_  |
|  bool | [**canNextVertex**](#function-cannextvertex-22) () <br>_determines if one can go to next vertex_  |
|  bool | [**canPreviousVertex**](#function-canpreviousvertex-22) () <br>_determines if one can go to previous vertex_  |
|  bool | [**canRemoveVertex**](#function-canremovevertex-22) () <br>_determines if one can remove current vertex_  |
|  bool | [**canUndo**](#function-canundo-22) () <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br>_This will clear the data._  |
|  void | [**clearHistory**](#function-clearhistory) () <br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent) override const<br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  QgsPoint | [**currentPoint**](#function-currentpoint-22) () const<br>_The current point being edited._  |
|  int | [**currentVertexIndex**](#function-currentvertexindex-22) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**dirty**](#function-dirty-22) () const<br>_determines if the model has changes_  |
|  bool | [**editingAllowed**](#function-editingallowed-22) () const<br>_determines if the model allows editing the geometry_  |
|  EditingMode | [**editingMode**](#function-editingmode-22) () const<br>_The current mode._  |
|  QVector&lt; QgsPoint &gt; | [**flatVertices**](#function-flatvertices) (int ringId=-1) const<br> |
|  QgsGeometry | [**geometry**](#function-geometry-22) () const<br> |
|  Qgis::GeometryType | [**geometryType**](#function-geometrytype) () const<br>_Returns the geometry type._  |
|  QList&lt; [**VertexChange**](structQfVertexModel_1_1VertexChange.md) &gt; | [**history**](#function-history) (bool transformPoints=false, bool includeAvailableRedos=false) const<br> |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent) override const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
|  Q\_INVOKABLE void | [**next**](#function-next) () <br>_next vertex or segment_  |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
|  Q\_INVOKABLE void | [**previous**](#function-previous) () <br>_previous vertex or segment_  |
|  Q\_INVOKABLE void | [**removeCurrentVertex**](#function-removecurrentvertex) () <br> |
|  Q\_INVOKABLE void | [**reset**](#function-reset) () <br>_Reset the model to its original geometry._  |
|  int | [**ringCount**](#function-ringcount-22) () const<br>_number of rings in the model_  |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  Q\_INVOKABLE void | [**selectVertexAtPosition**](#function-selectvertexatposition-12) (const QPointF & point, double threshold, bool autoInsert=true) <br>_Selects the vertex at the given screen_ _point_ _within a given__threshold_ _._ |
|  void | [**selectVertexAtPosition**](#function-selectvertexatposition-22) (const QgsPoint & mapPoint, double threshold, bool autoInsert=true) <br>_Selects the vertex at the given_ _mapPoint_ _within a given__threshold_ _._ |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setCurrentPoint**](#function-setcurrentpoint) (const QgsPoint & point) <br>_The current point being edited._  |
|  void | [**setCurrentVertexIndex**](#function-setcurrentvertexindex) (qsizetype currentIndex) <br> |
|  void | [**setEditingMode**](#function-seteditingmode) (EditingMode mode) <br>_The current mode._  |
|  void | [**setGeometry**](#function-setgeometry) (const QgsGeometry & geometry) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
|  Q\_INVOKABLE void | [**undoHistory**](#function-undohistory) () <br> |
|  Q\_INVOKABLE void | [**updateGeometry**](#function-updategeometry) (const QgsGeometry & geometry) <br> |
|  [**Vertex**](structQfVertexModel_1_1Vertex.md) | [**vertex**](#function-vertex) (int row) const<br> |
|  int | [**vertexCount**](#function-vertexcount-22) () const<br>_number of vertices in the model_  |
|  QList&lt; [**Vertex**](structQfVertexModel_1_1Vertex.md) &gt; | [**vertices**](#function-vertices) () const<br>_Returns a list of vertices._  |
|  QVector&lt; QgsPoint &gt; | [**verticesAdded**](#function-verticesadded) () const<br>_Returns a list of added vertices not found in linked geometry._  |
|  QVector&lt; QgsPoint &gt; | [**verticesDeleted**](#function-verticesdeleted) () const<br>_Returns a list of added vertices not found in linked geometry._  |
|  QVector&lt; QPair&lt; QgsPoint, QgsPoint &gt; &gt; | [**verticesMoved**](#function-verticesmoved) () const<br>_Returns a list of moved vertices found in linked geometry._  |
|   | [**~QfVertexModel**](#function-qfvertexmodel) () override<br> |




























## Detailed Description


The [**QfVertexModel**](classQfVertexModel.md) class is a model to highlight and edit vertices. The model is used in map coordinates. There are different modes: no editing, edit (move/remove) nodes, add nodes (to be implemented)


The model holds all vertices and the candidates for new vertices. If you need the existing nodes, use [**flatVertices()**](classQfVertexModel.md#function-flatvertices). 


    
## Public Types Documentation




### enum ColumnRole 

```C++
enum QfVertexModel::ColumnRole {
    PointRole = Qt::UserRole + 1,
    CurrentVertexRole,
    OriginalPointRole,
    ExistingVertexRole,
    RingIdRole
};
```




<hr>



### enum EditingMode 

```C++
enum QfVertexModel::EditingMode {
    NoEditing,
    EditVertex,
    AddVertex
};
```




<hr>



### enum PointType 

```C++
enum QfVertexModel::PointType {
    ExistingVertex,
    NewVertexSegment,
    NewVertexExtending
};
```




<hr>



### enum VertexChangeType 

```C++
enum QfVertexModel::VertexChangeType {
    NoChange,
    VertexMove,
    VertexAddition,
    VertexDeletion
};
```




<hr>
## Public Properties Documentation




### property canAddVertex [1/2]

_determines if one can add vertex_ 
```C++
bool QfVertexModel::canAddVertex;
```




<hr>



### property canNextVertex [1/2]

_determines if one can go to next vertex_ 
```C++
bool QfVertexModel::canNextVertex;
```




<hr>



### property canPreviousVertex [1/2]

_determines if one can go to previous vertex_ 
```C++
bool QfVertexModel::canPreviousVertex;
```




<hr>



### property canRemoveVertex [1/2]

_determines if one can remove current vertex_ 
```C++
bool QfVertexModel::canRemoveVertex;
```




<hr>



### property canUndo [1/2]

_returns TRUE if an undo operation is available_ 
```C++
bool QfVertexModel::canUndo;
```




<hr>



### property crs [1/2]

```C++
QgsCoordinateReferenceSystem QfVertexModel::crs;
```



The coordinate reference system in which the geometry is 


        

<hr>



### property currentPoint [1/2]

_The current point being edited._ 
```C++
QgsPoint QfVertexModel::currentPoint;
```





**See also:** [**editingMode**](classQfVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS 



        

<hr>



### property currentVertexIndex [1/2]

```C++
int QfVertexModel::currentVertexIndex;
```



The index of the currently active vertex. If no vertex is selected, this is -1. 


        

<hr>



### property dirty [1/2]

_determines if the model has changes_ 
```C++
bool QfVertexModel::dirty;
```




<hr>



### property editingAllowed [1/2]

_determines if the model allows editing the geometry_ 
```C++
bool QfVertexModel::editingAllowed;
```




<hr>



### property editingMode [1/2]

_The current mode._ 
```C++
EditingMode QfVertexModel::editingMode;
```




<hr>



### property geometry [1/2]

```C++
QgsGeometry QfVertexModel::geometry;
```



The geometry in layer coordinates 


        

<hr>



### property isHovering 

_determines if the map is currently being hovered (then when moving the map, it will not move directly a vertex if the mode is AddVertex)_ 
```C++
bool QfVertexModel::isHovering;
```




<hr>



### property mapSettings [1/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * QfVertexModel::mapSettings;
```




<hr>



### property ringCount [1/2]

_number of rings in the model_ 
```C++
int QfVertexModel::ringCount;
```




<hr>



### property vertexCount [1/2]

_number of vertices in the model_ 
```C++
int QfVertexModel::vertexCount;
```




<hr>
## Public Signals Documentation




### signal canAddVertexChanged 

_determines if one can add vertex_ 
```C++
void QfVertexModel::canAddVertexChanged;
```




<hr>



### signal canNextVertexChanged 

_determines if one can go to next vertex_ 
```C++
void QfVertexModel::canNextVertexChanged;
```




<hr>



### signal canPreviousVertexChanged 

_determines if one can go to previous vertex_ 
```C++
void QfVertexModel::canPreviousVertexChanged;
```




<hr>



### signal canRemoveVertexChanged 

_determines if one can remove current vertex_ 
```C++
void QfVertexModel::canRemoveVertexChanged;
```




<hr>



### signal crsChanged 

_Emitted when the coordinate reference system has changed._ 
```C++
void QfVertexModel::crsChanged;
```




<hr>



### signal currentPointChanged 

_The current point being edited._ 
```C++
void QfVertexModel::currentPointChanged;
```





**See also:** [**editingMode**](classQfVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### signal currentVertexIndexChanged 

```C++
void QfVertexModel::currentVertexIndexChanged;
```




<hr>



### signal dirtyChanged 

_determines if the model has changes_ 
```C++
void QfVertexModel::dirtyChanged;
```




<hr>



### signal editingModeChanged 

_The current mode._ 
```C++
void QfVertexModel::editingModeChanged;
```




<hr>



### signal geometryChanged 

_Emitted when the geometry has changed._ 
```C++
void QfVertexModel::geometryChanged;
```




<hr>



### signal geometryTypeChanged 

_Returns the geometry type._ 
```C++
void QfVertexModel::geometryTypeChanged;
```




<hr>



### signal historyChanged 

_Emitted when the history has been modified._ 
```C++
void QfVertexModel::historyChanged;
```




<hr>



### signal mapSettingsChanged 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void QfVertexModel::mapSettingsChanged;
```




<hr>



### signal ringCountChanged 

_number of rings in the model_ 
```C++
void QfVertexModel::ringCountChanged;
```




<hr>



### signal vertexCountChanged 

_number of vertices in the model_ 
```C++
void QfVertexModel::vertexCountChanged;
```




<hr>
## Public Functions Documentation




### function QfVertexModel 

```C++
explicit QfVertexModel::QfVertexModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addToHistory 

```C++
void QfVertexModel::addToHistory (
    VertexChangeType type
) 
```




<hr>



### function addVertexNearestToPosition 

_Adds a new vertex on the segment having its mid-point nearest to the_ _mapPoint_ _._
```C++
Q_INVOKABLE void QfVertexModel::addVertexNearestToPosition (
    const QgsPoint & mapPoint
) 
```




<hr>



### function canAddVertex [2/2]

_determines if one can add vertex_ 
```C++
bool QfVertexModel::canAddVertex () 
```




<hr>



### function canNextVertex [2/2]

_determines if one can go to next vertex_ 
```C++
bool QfVertexModel::canNextVertex () 
```




<hr>



### function canPreviousVertex [2/2]

_determines if one can go to previous vertex_ 
```C++
bool QfVertexModel::canPreviousVertex () 
```




<hr>



### function canRemoveVertex [2/2]

_determines if one can remove current vertex_ 
```C++
bool QfVertexModel::canRemoveVertex () 
```




<hr>



### function canUndo [2/2]

```C++
bool QfVertexModel::canUndo () 
```




<hr>



### function clear 

_This will clear the data._ 
```C++
Q_INVOKABLE void QfVertexModel::clear () 
```




<hr>



### function clearHistory 

```C++
void QfVertexModel::clearHistory () 
```




<hr>



### function columnCount 

```C++
int QfVertexModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem QfVertexModel::crs () const
```



The coordinate reference system in which the geometry is 


        

<hr>



### function currentPoint [2/2]

_The current point being edited._ 
```C++
QgsPoint QfVertexModel::currentPoint () const
```





**See also:** [**editingMode**](classQfVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### function currentVertexIndex [2/2]

```C++
int QfVertexModel::currentVertexIndex () const
```




<hr>



### function data 

```C++
QVariant QfVertexModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function dirty [2/2]

_determines if the model has changes_ 
```C++
bool QfVertexModel::dirty () const
```




<hr>



### function editingAllowed [2/2]

_determines if the model allows editing the geometry_ 
```C++
bool QfVertexModel::editingAllowed () const
```




<hr>



### function editingMode [2/2]

_The current mode._ 
```C++
EditingMode QfVertexModel::editingMode () const
```




<hr>



### function flatVertices 

```C++
QVector< QgsPoint > QfVertexModel::flatVertices (
    int ringId=-1
) const
```



Returns a list of point (segment vertex, if any, will be skipped) For a polygon, if ringId is not given the current ring will be returned 


        

<hr>



### function geometry [2/2]

```C++
QgsGeometry QfVertexModel::geometry () const
```



The geometry in layer coordinates 


        

<hr>



### function geometryType 

_Returns the geometry type._ 
```C++
Qgis::GeometryType QfVertexModel::geometryType () const
```




<hr>



### function history 

```C++
QList< VertexChange > QfVertexModel::history (
    bool transformPoints=false,
    bool includeAvailableRedos=false
) const
```




<hr>



### function index 

```C++
QModelIndex QfVertexModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function mapSettings [2/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * QfVertexModel::mapSettings () 
```




<hr>



### function next 

_next vertex or segment_ 
```C++
Q_INVOKABLE void QfVertexModel::next () 
```




<hr>



### function parent 

```C++
QModelIndex QfVertexModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function previous 

_previous vertex or segment_ 
```C++
Q_INVOKABLE void QfVertexModel::previous () 
```




<hr>



### function removeCurrentVertex 

```C++
Q_INVOKABLE void QfVertexModel::removeCurrentVertex () 
```




<hr>



### function reset 

_Reset the model to its original geometry._ 
```C++
Q_INVOKABLE void QfVertexModel::reset () 
```




<hr>



### function ringCount [2/2]

_number of rings in the model_ 
```C++
int QfVertexModel::ringCount () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfVertexModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfVertexModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function selectVertexAtPosition [1/2]

_Selects the vertex at the given screen_ _point_ _within a given__threshold_ _._
```C++
Q_INVOKABLE void QfVertexModel::selectVertexAtPosition (
    const QPointF & point,
    double threshold,
    bool autoInsert=true
) 
```




<hr>



### function selectVertexAtPosition [2/2]

_Selects the vertex at the given_ _mapPoint_ _within a given__threshold_ _._
```C++
void QfVertexModel::selectVertexAtPosition (
    const QgsPoint & mapPoint,
    double threshold,
    bool autoInsert=true
) 
```




<hr>



### function setCrs 

```C++
void QfVertexModel::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```



The coorinate reference system in which the geometry is 


        

<hr>



### function setCurrentPoint 

_The current point being edited._ 
```C++
void QfVertexModel::setCurrentPoint (
    const QgsPoint & point
) 
```





**See also:** [**editingMode**](classQfVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### function setCurrentVertexIndex 

```C++
void QfVertexModel::setCurrentVertexIndex (
    qsizetype currentIndex
) 
```




<hr>



### function setEditingMode 

_The current mode._ 
```C++
void QfVertexModel::setEditingMode (
    EditingMode mode
) 
```




<hr>



### function setGeometry 

```C++
void QfVertexModel::setGeometry (
    const QgsGeometry & geometry
) 
```



The geometry in layer coordinates 


        

<hr>



### function setMapSettings 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void QfVertexModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function undoHistory 

```C++
Q_INVOKABLE void QfVertexModel::undoHistory () 
```




<hr>



### function updateGeometry 

```C++
Q_INVOKABLE void QfVertexModel::updateGeometry (
    const QgsGeometry & geometry
) 
```



sets the geometry to the given _geometry_ but preserves the index of the current vertex this is used to update the original geometry while still editing the model 

**See also:** `geometry` property 



        

<hr>



### function vertex 

```C++
Vertex QfVertexModel::vertex (
    int row
) const
```




<hr>



### function vertexCount [2/2]

_number of vertices in the model_ 
```C++
int QfVertexModel::vertexCount () const
```




<hr>



### function vertices 

_Returns a list of vertices._ 
```C++
QList< Vertex > QfVertexModel::vertices () const
```




<hr>



### function verticesAdded 

_Returns a list of added vertices not found in linked geometry._ 
```C++
QVector< QgsPoint > QfVertexModel::verticesAdded () const
```




<hr>



### function verticesDeleted 

_Returns a list of added vertices not found in linked geometry._ 
```C++
QVector< QgsPoint > QfVertexModel::verticesDeleted () const
```




<hr>



### function verticesMoved 

_Returns a list of moved vertices found in linked geometry._ 
```C++
QVector< QPair< QgsPoint, QgsPoint > > QfVertexModel::verticesMoved () const
```




<hr>



### function ~QfVertexModel 

```C++
QfVertexModel::~QfVertexModel () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfvertexmodel.h`

