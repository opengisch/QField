

# Class VertexModel



[**ClassList**](annotated.md) **>** [**VertexModel**](classVertexModel.md)



[More...](#detailed-description)

* `#include <vertexmodel.h>`



Inherits the following classes: QAbstractListModel












## Classes

| Type | Name |
| ---: | :--- |
| struct | [**Vertex**](structVertexModel_1_1Vertex.md) <br> |
| struct | [**VertexChange**](structVertexModel_1_1VertexChange.md) <br> |


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
| property bool | [**canAddVertex**](classVertexModel.md#property-canaddvertex-12)  <br>_determines if one can add vertex_  |
| property bool | [**canNextVertex**](classVertexModel.md#property-cannextvertex-12)  <br>_determines if one can go to next vertex_  |
| property bool | [**canPreviousVertex**](classVertexModel.md#property-canpreviousvertex-12)  <br>_determines if one can go to previous vertex_  |
| property bool | [**canRemoveVertex**](classVertexModel.md#property-canremovevertex-12)  <br>_determines if one can remove current vertex_  |
| property bool | [**canUndo**](classVertexModel.md#property-canundo-12)  <br>_returns TRUE if an undo operation is available_  |
| property QgsCoordinateReferenceSystem | [**crs**](classVertexModel.md#property-crs-12)  <br> |
| property QgsPoint | [**currentPoint**](classVertexModel.md#property-currentpoint-12)  <br>_The current point being edited._  |
| property int | [**currentVertexIndex**](classVertexModel.md#property-currentvertexindex-12)  <br> |
| property bool | [**dirty**](classVertexModel.md#property-dirty-12)  <br>_determines if the model has changes_  |
| property bool | [**editingAllowed**](classVertexModel.md#property-editingallowed-12)  <br>_determines if the model allows editing the geometry_  |
| property EditingMode | [**editingMode**](classVertexModel.md#property-editingmode-12)  <br>_The current mode._  |
| property QgsGeometry | [**geometry**](classVertexModel.md#property-geometry-12)  <br> |
| property bool | [**isHovering**](classVertexModel.md#property-ishovering)  <br>_determines if the map is currently being hovered (then when moving the map, it will not move directly a vertex if the mode is AddVertex)_  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classVertexModel.md#property-mapsettings-12)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| property int | [**ringCount**](classVertexModel.md#property-ringcount-12)  <br>_number of rings in the model_  |
| property int | [**vertexCount**](classVertexModel.md#property-vertexcount-12)  <br>_number of vertices in the model_  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**canAddVertexChanged**](classVertexModel.md#signal-canaddvertexchanged)  <br>_determines if one can add vertex_  |
| signal void | [**canNextVertexChanged**](classVertexModel.md#signal-cannextvertexchanged)  <br>_determines if one can go to next vertex_  |
| signal void | [**canPreviousVertexChanged**](classVertexModel.md#signal-canpreviousvertexchanged)  <br>_determines if one can go to previous vertex_  |
| signal void | [**canRemoveVertexChanged**](classVertexModel.md#signal-canremovevertexchanged)  <br>_determines if one can remove current vertex_  |
| signal void | [**crsChanged**](classVertexModel.md#signal-crschanged)  <br>_Emitted when the coordinate reference system has changed._  |
| signal void | [**currentPointChanged**](classVertexModel.md#signal-currentpointchanged)  <br>_The current point being edited._  |
| signal void | [**currentVertexIndexChanged**](classVertexModel.md#signal-currentvertexindexchanged)  <br> |
| signal void | [**dirtyChanged**](classVertexModel.md#signal-dirtychanged)  <br>_determines if the model has changes_  |
| signal void | [**editingModeChanged**](classVertexModel.md#signal-editingmodechanged)  <br>_The current mode._  |
| signal void | [**geometryChanged**](classVertexModel.md#signal-geometrychanged)  <br>_Emitted when the geometry has changed._  |
| signal void | [**geometryTypeChanged**](classVertexModel.md#signal-geometrytypechanged)  <br>_Returns the geometry type._  |
| signal void | [**historyChanged**](classVertexModel.md#signal-historychanged)  <br>_Emitted when the history has been modified._  |
| signal void | [**mapSettingsChanged**](classVertexModel.md#signal-mapsettingschanged)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| signal void | [**ringCountChanged**](classVertexModel.md#signal-ringcountchanged)  <br>_number of rings in the model_  |
| signal void | [**vertexCountChanged**](classVertexModel.md#signal-vertexcountchanged)  <br>_number of vertices in the model_  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**VertexModel**](#function-vertexmodel) (QObject \* parent=nullptr) <br> |
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
|  QList&lt; [**VertexChange**](structVertexModel_1_1VertexChange.md) &gt; | [**history**](#function-history) (bool transformPoints=false, bool includeAvailableRedos=false) const<br> |
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
|  [**Vertex**](structVertexModel_1_1Vertex.md) | [**vertex**](#function-vertex) (int row) const<br> |
|  int | [**vertexCount**](#function-vertexcount-22) () const<br>_number of vertices in the model_  |
|  QList&lt; [**Vertex**](structVertexModel_1_1Vertex.md) &gt; | [**vertices**](#function-vertices) () const<br>_Returns a list of vertices._  |
|  QVector&lt; QgsPoint &gt; | [**verticesAdded**](#function-verticesadded) () const<br>_Returns a list of added vertices not found in linked geometry._  |
|  QVector&lt; QgsPoint &gt; | [**verticesDeleted**](#function-verticesdeleted) () const<br>_Returns a list of added vertices not found in linked geometry._  |
|  QVector&lt; QPair&lt; QgsPoint, QgsPoint &gt; &gt; | [**verticesMoved**](#function-verticesmoved) () const<br>_Returns a list of moved vertices found in linked geometry._  |
|   | [**~VertexModel**](#function-vertexmodel) () override<br> |




























## Detailed Description


The [**VertexModel**](classVertexModel.md) class is a model to highlight and edit vertices. The model is used in map coordinates. There are different modes: no editing, edit (move/remove) nodes, add nodes (to be implemented)


The model holds all vertices and the candidates for new vertices. If you need the existing nodes, use [**flatVertices()**](classVertexModel.md#function-flatvertices). 


    
## Public Types Documentation




### enum ColumnRole 

```C++
enum VertexModel::ColumnRole {
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
enum VertexModel::EditingMode {
    NoEditing,
    EditVertex,
    AddVertex
};
```




<hr>



### enum PointType 

```C++
enum VertexModel::PointType {
    ExistingVertex,
    NewVertexSegment,
    NewVertexExtending
};
```




<hr>



### enum VertexChangeType 

```C++
enum VertexModel::VertexChangeType {
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
bool VertexModel::canAddVertex;
```




<hr>



### property canNextVertex [1/2]

_determines if one can go to next vertex_ 
```C++
bool VertexModel::canNextVertex;
```




<hr>



### property canPreviousVertex [1/2]

_determines if one can go to previous vertex_ 
```C++
bool VertexModel::canPreviousVertex;
```




<hr>



### property canRemoveVertex [1/2]

_determines if one can remove current vertex_ 
```C++
bool VertexModel::canRemoveVertex;
```




<hr>



### property canUndo [1/2]

_returns TRUE if an undo operation is available_ 
```C++
bool VertexModel::canUndo;
```




<hr>



### property crs [1/2]

```C++
QgsCoordinateReferenceSystem VertexModel::crs;
```



The coordinate reference system in which the geometry is 


        

<hr>



### property currentPoint [1/2]

_The current point being edited._ 
```C++
QgsPoint VertexModel::currentPoint;
```





**See also:** [**editingMode**](classVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS 



        

<hr>



### property currentVertexIndex [1/2]

```C++
int VertexModel::currentVertexIndex;
```



The index of the currently active vertex. If no vertex is selected, this is -1. 


        

<hr>



### property dirty [1/2]

_determines if the model has changes_ 
```C++
bool VertexModel::dirty;
```




<hr>



### property editingAllowed [1/2]

_determines if the model allows editing the geometry_ 
```C++
bool VertexModel::editingAllowed;
```




<hr>



### property editingMode [1/2]

_The current mode._ 
```C++
EditingMode VertexModel::editingMode;
```




<hr>



### property geometry [1/2]

```C++
QgsGeometry VertexModel::geometry;
```



The geometry in layer coordinates 


        

<hr>



### property isHovering 

_determines if the map is currently being hovered (then when moving the map, it will not move directly a vertex if the mode is AddVertex)_ 
```C++
bool VertexModel::isHovering;
```




<hr>



### property mapSettings [1/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * VertexModel::mapSettings;
```




<hr>



### property ringCount [1/2]

_number of rings in the model_ 
```C++
int VertexModel::ringCount;
```




<hr>



### property vertexCount [1/2]

_number of vertices in the model_ 
```C++
int VertexModel::vertexCount;
```




<hr>
## Public Signals Documentation




### signal canAddVertexChanged 

_determines if one can add vertex_ 
```C++
void VertexModel::canAddVertexChanged;
```




<hr>



### signal canNextVertexChanged 

_determines if one can go to next vertex_ 
```C++
void VertexModel::canNextVertexChanged;
```




<hr>



### signal canPreviousVertexChanged 

_determines if one can go to previous vertex_ 
```C++
void VertexModel::canPreviousVertexChanged;
```




<hr>



### signal canRemoveVertexChanged 

_determines if one can remove current vertex_ 
```C++
void VertexModel::canRemoveVertexChanged;
```




<hr>



### signal crsChanged 

_Emitted when the coordinate reference system has changed._ 
```C++
void VertexModel::crsChanged;
```




<hr>



### signal currentPointChanged 

_The current point being edited._ 
```C++
void VertexModel::currentPointChanged;
```





**See also:** [**editingMode**](classVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### signal currentVertexIndexChanged 

```C++
void VertexModel::currentVertexIndexChanged;
```




<hr>



### signal dirtyChanged 

_determines if the model has changes_ 
```C++
void VertexModel::dirtyChanged;
```




<hr>



### signal editingModeChanged 

_The current mode._ 
```C++
void VertexModel::editingModeChanged;
```




<hr>



### signal geometryChanged 

_Emitted when the geometry has changed._ 
```C++
void VertexModel::geometryChanged;
```




<hr>



### signal geometryTypeChanged 

_Returns the geometry type._ 
```C++
void VertexModel::geometryTypeChanged;
```




<hr>



### signal historyChanged 

_Emitted when the history has been modified._ 
```C++
void VertexModel::historyChanged;
```




<hr>



### signal mapSettingsChanged 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void VertexModel::mapSettingsChanged;
```




<hr>



### signal ringCountChanged 

_number of rings in the model_ 
```C++
void VertexModel::ringCountChanged;
```




<hr>



### signal vertexCountChanged 

_number of vertices in the model_ 
```C++
void VertexModel::vertexCountChanged;
```




<hr>
## Public Functions Documentation




### function VertexModel 

```C++
explicit VertexModel::VertexModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addToHistory 

```C++
void VertexModel::addToHistory (
    VertexChangeType type
) 
```




<hr>



### function addVertexNearestToPosition 

_Adds a new vertex on the segment having its mid-point nearest to the_ _mapPoint_ _._
```C++
Q_INVOKABLE void VertexModel::addVertexNearestToPosition (
    const QgsPoint & mapPoint
) 
```




<hr>



### function canAddVertex [2/2]

_determines if one can add vertex_ 
```C++
bool VertexModel::canAddVertex () 
```




<hr>



### function canNextVertex [2/2]

_determines if one can go to next vertex_ 
```C++
bool VertexModel::canNextVertex () 
```




<hr>



### function canPreviousVertex [2/2]

_determines if one can go to previous vertex_ 
```C++
bool VertexModel::canPreviousVertex () 
```




<hr>



### function canRemoveVertex [2/2]

_determines if one can remove current vertex_ 
```C++
bool VertexModel::canRemoveVertex () 
```




<hr>



### function canUndo [2/2]

```C++
bool VertexModel::canUndo () 
```




<hr>



### function clear 

_This will clear the data._ 
```C++
Q_INVOKABLE void VertexModel::clear () 
```




<hr>



### function clearHistory 

```C++
void VertexModel::clearHistory () 
```




<hr>



### function columnCount 

```C++
int VertexModel::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function crs [2/2]

```C++
QgsCoordinateReferenceSystem VertexModel::crs () const
```



The coordinate reference system in which the geometry is 


        

<hr>



### function currentPoint [2/2]

_The current point being edited._ 
```C++
QgsPoint VertexModel::currentPoint () const
```





**See also:** [**editingMode**](classVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### function currentVertexIndex [2/2]

```C++
int VertexModel::currentVertexIndex () const
```




<hr>



### function data 

```C++
QVariant VertexModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function dirty [2/2]

_determines if the model has changes_ 
```C++
bool VertexModel::dirty () const
```




<hr>



### function editingAllowed [2/2]

_determines if the model allows editing the geometry_ 
```C++
bool VertexModel::editingAllowed () const
```




<hr>



### function editingMode [2/2]

_The current mode._ 
```C++
EditingMode VertexModel::editingMode () const
```




<hr>



### function flatVertices 

```C++
QVector< QgsPoint > VertexModel::flatVertices (
    int ringId=-1
) const
```



Returns a list of point (segment vertex, if any, will be skipped) For a polygon, if ringId is not given the current ring will be returned 


        

<hr>



### function geometry [2/2]

```C++
QgsGeometry VertexModel::geometry () const
```



The geometry in layer coordinates 


        

<hr>



### function geometryType 

_Returns the geometry type._ 
```C++
Qgis::GeometryType VertexModel::geometryType () const
```




<hr>



### function history 

```C++
QList< VertexChange > VertexModel::history (
    bool transformPoints=false,
    bool includeAvailableRedos=false
) const
```




<hr>



### function index 

```C++
QModelIndex VertexModel::index (
    int row,
    int column,
    const QModelIndex & parent
) override const
```




<hr>



### function mapSettings [2/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * VertexModel::mapSettings () 
```




<hr>



### function next 

_next vertex or segment_ 
```C++
Q_INVOKABLE void VertexModel::next () 
```




<hr>



### function parent 

```C++
QModelIndex VertexModel::parent (
    const QModelIndex & child
) override const
```




<hr>



### function previous 

_previous vertex or segment_ 
```C++
Q_INVOKABLE void VertexModel::previous () 
```




<hr>



### function removeCurrentVertex 

```C++
Q_INVOKABLE void VertexModel::removeCurrentVertex () 
```




<hr>



### function reset 

_Reset the model to its original geometry._ 
```C++
Q_INVOKABLE void VertexModel::reset () 
```




<hr>



### function ringCount [2/2]

_number of rings in the model_ 
```C++
int VertexModel::ringCount () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > VertexModel::roleNames () override const
```




<hr>



### function rowCount 

```C++
int VertexModel::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function selectVertexAtPosition [1/2]

_Selects the vertex at the given screen_ _point_ _within a given__threshold_ _._
```C++
Q_INVOKABLE void VertexModel::selectVertexAtPosition (
    const QPointF & point,
    double threshold,
    bool autoInsert=true
) 
```




<hr>



### function selectVertexAtPosition [2/2]

_Selects the vertex at the given_ _mapPoint_ _within a given__threshold_ _._
```C++
void VertexModel::selectVertexAtPosition (
    const QgsPoint & mapPoint,
    double threshold,
    bool autoInsert=true
) 
```




<hr>



### function setCrs 

```C++
void VertexModel::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```



The coorinate reference system in which the geometry is 


        

<hr>



### function setCurrentPoint 

_The current point being edited._ 
```C++
void VertexModel::setCurrentPoint (
    const QgsPoint & point
) 
```





**See also:** [**editingMode**](classVertexModel.md#property-editingmode-12). The expected CRS to read/write is the map canvas CRS   



        

<hr>



### function setCurrentVertexIndex 

```C++
void VertexModel::setCurrentVertexIndex (
    qsizetype currentIndex
) 
```




<hr>



### function setEditingMode 

_The current mode._ 
```C++
void VertexModel::setEditingMode (
    EditingMode mode
) 
```




<hr>



### function setGeometry 

```C++
void VertexModel::setGeometry (
    const QgsGeometry & geometry
) 
```



The geometry in layer coordinates 


        

<hr>



### function setMapSettings 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void VertexModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function undoHistory 

```C++
Q_INVOKABLE void VertexModel::undoHistory () 
```




<hr>



### function updateGeometry 

```C++
Q_INVOKABLE void VertexModel::updateGeometry (
    const QgsGeometry & geometry
) 
```



sets the geometry to the given _geometry_ but preserves the index of the current vertex this is used to update the original geometry while still editing the model 

**See also:** `geometry` property 



        

<hr>



### function vertex 

```C++
Vertex VertexModel::vertex (
    int row
) const
```




<hr>



### function vertexCount [2/2]

_number of vertices in the model_ 
```C++
int VertexModel::vertexCount () const
```




<hr>



### function vertices 

_Returns a list of vertices._ 
```C++
QList< Vertex > VertexModel::vertices () const
```




<hr>



### function verticesAdded 

_Returns a list of added vertices not found in linked geometry._ 
```C++
QVector< QgsPoint > VertexModel::verticesAdded () const
```




<hr>



### function verticesDeleted 

_Returns a list of added vertices not found in linked geometry._ 
```C++
QVector< QgsPoint > VertexModel::verticesDeleted () const
```




<hr>



### function verticesMoved 

_Returns a list of moved vertices found in linked geometry._ 
```C++
QVector< QPair< QgsPoint, QgsPoint > > VertexModel::verticesMoved () const
```




<hr>



### function ~VertexModel 

```C++
VertexModel::~VertexModel () override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/vertexmodel.h`

