

# Class RubberbandModel



[**ClassList**](annotated.md) **>** [**RubberbandModel**](classRubberbandModel.md)



[More...](#detailed-description)

* `#include <rubberbandmodel.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsCoordinateReferenceSystem | [**crs**](classRubberbandModel.md#property-crs-12)  <br>_Coordinate reference system._  |
| property QgsPoint | [**currentCoordinate**](classRubberbandModel.md#property-currentcoordinate-12)  <br>_Current vertex point._  |
| property int | [**currentCoordinateIndex**](classRubberbandModel.md#property-currentcoordinateindex-12)  <br>_Current vertex index._  |
| property QDateTime | [**currentPositionTimestamp**](classRubberbandModel.md#property-currentpositiontimestamp-12)  <br>_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_  |
| property QgsPoint | [**firstCoordinate**](classRubberbandModel.md#property-firstcoordinate-12)  <br>_First vertex point._  |
| property bool | [**frozen**](classRubberbandModel.md#property-frozen-12)  <br>_Freeze the rubberband so it doesn't get modified while panning map._  |
| property Qgis::GeometryType | [**geometryType**](classRubberbandModel.md#property-geometrytype-12)  <br>[_**Geometry**_](classGeometry.md) _type of the model._ |
| property QgsPoint | [**lastCoordinate**](classRubberbandModel.md#property-lastcoordinate-12)  <br>_Last vertex point._  |
| property double | [**measureValue**](classRubberbandModel.md#property-measurevalue-12)  <br>_measureValue defines the M value of the coordinates_  |
| property QgsPoint | [**penultimateCoordinate**](classRubberbandModel.md#property-penultimatecoordinate-12)  <br>_Penultimate (last but one) vertex point._  |
| property QgsVectorLayer \* | [**vectorLayer**](classRubberbandModel.md#property-vectorlayer-12)  <br>_The vector layer the model is for._  |
| property int | [**vertexCount**](classRubberbandModel.md#property-vertexcount-12)  <br>_Total count of vertices._  |
| property QVector&lt; QgsPoint &gt; | [**vertices**](classRubberbandModel.md#property-vertices-12)  <br>_All vertex points._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**crsChanged**](classRubberbandModel.md#signal-crschanged)  <br> |
| signal void | [**currentCoordinateChanged**](classRubberbandModel.md#signal-currentcoordinatechanged)  <br> |
| signal void | [**currentCoordinateIndexChanged**](classRubberbandModel.md#signal-currentcoordinateindexchanged)  <br> |
| signal void | [**currentPositionTimestampChanged**](classRubberbandModel.md#signal-currentpositiontimestampchanged)  <br> |
| signal void | [**frozenChanged**](classRubberbandModel.md#signal-frozenchanged)  <br>_Freeze the rubberband so it doesn't get modified while panning map._  |
| signal void | [**geometryTypeChanged**](classRubberbandModel.md#signal-geometrytypechanged)  <br> |
| signal void | [**measureValueChanged**](classRubberbandModel.md#signal-measurevaluechanged)  <br> |
| signal void | [**vectorLayerChanged**](classRubberbandModel.md#signal-vectorlayerchanged)  <br> |
| signal void | [**vertexChanged**](classRubberbandModel.md#signal-vertexchanged) (int index) <br> |
| signal void | [**vertexCountChanged**](classRubberbandModel.md#signal-vertexcountchanged)  <br> |
| signal void | [**verticesInserted**](classRubberbandModel.md#signal-verticesinserted) (int index, int count) <br> |
| signal void | [**verticesRemoved**](classRubberbandModel.md#signal-verticesremoved) (int index, int count) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RubberbandModel**](#function-rubberbandmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**addVertex**](#function-addvertex) () <br>_Add vertex from the current position._  |
|  Q\_INVOKABLE void | [**addVertexFromPoint**](#function-addvertexfrompoint) (const QgsPoint & point) <br>_Add vertex with a given point._  |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br>_Coordinate reference system._  |
|  QgsPoint | [**currentCoordinate**](#function-currentcoordinate-22) () const<br>_Current vertex point._  |
|  int | [**currentCoordinateIndex**](#function-currentcoordinateindex-22) () const<br>_Current vertex index._  |
|  QgsPoint | [**currentPoint**](#function-currentpoint) (const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType=Qgis::WkbType::PointZ) const<br>_Returns the current vertex point transformed to the given_ _crs_ _._ |
|  QDateTime | [**currentPositionTimestamp**](#function-currentpositiontimestamp-22) () const<br>_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_  |
|  QgsPoint | [**firstCoordinate**](#function-firstcoordinate-22) () const<br>_First vertex point._  |
|  QVector&lt; QgsPointXY &gt; | [**flatPointSequence**](#function-flatpointsequence) (const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()) const<br>_Returns all vertices points keeping only X and Y coordinates and transformed to the given_ _crs_ _._ |
|  bool | [**frozen**](#function-frozen-22) () const<br>_Freeze the rubberband so it doesn't get modified while panning map._  |
|  Qgis::GeometryType | [**geometryType**](#function-geometrytype-22) () const<br>[_**Geometry**_](classGeometry.md) _type of the model._ |
|  void | [**insertVertices**](#function-insertvertices) (int index, int count) <br>_Inserts vertices._  |
|  bool | [**isEmpty**](#function-isempty) () const<br>_Returns whether there is at least one vertex in the model._  |
|  QgsPoint | [**lastCoordinate**](#function-lastcoordinate-22) () const<br>_Last vertex point._  |
|  double | [**measureValue**](#function-measurevalue-22) () const<br>_measureValue defines the M value of the coordinates_  |
|  QgsPoint | [**penultimateCoordinate**](#function-penultimatecoordinate-22) () const<br>_Penultimate (last but one) vertex point._  |
|  Q\_INVOKABLE QgsPointSequence | [**pointSequence**](#function-pointsequence) (const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem(), Qgis::WkbType wkbType=Qgis::WkbType::Point, bool closeLine=false) const<br> |
|  Q\_INVOKABLE void | [**removeVertex**](#function-removevertex) () <br>_Remove the vertex at the current index._  |
|  void | [**removeVertices**](#function-removevertices) (int index, int count, bool keepLast=true) <br>_Remove_ _count_ _vertices starting at__index_ _._ |
|  Q\_INVOKABLE void | [**reset**](#function-reset) (bool keepLast=true) <br>_Reset the model, remove all vertices and restart the vertex index._  |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br>_Coordinate reference system._  |
|  void | [**setCurrentCoordinate**](#function-setcurrentcoordinate) (const QgsPoint & currentCoordinate) <br>_Current vertex point._  |
|  void | [**setCurrentCoordinateIndex**](#function-setcurrentcoordinateindex) (int currentCoordinateIndex) <br>_Current vertex index._  |
|  void | [**setCurrentPositionTimestamp**](#function-setcurrentpositiontimestamp) (const QDateTime & currentPositionTimestamp) <br>_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_  |
|  void | [**setDataFromGeometry**](#function-setdatafromgeometry) (QgsGeometry geometry, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()) <br> |
|  void | [**setFrozen**](#function-setfrozen) (const bool & frozen) <br>_Freeze the rubberband so it doesn't get modified while panning map._  |
|  void | [**setGeometryType**](#function-setgeometrytype) (const Qgis::GeometryType & geometryType) <br>[_**Geometry**_](classGeometry.md) _type of the model._ |
|  void | [**setMeasureValue**](#function-setmeasurevalue) (const double measureValue) <br>_measureValue defines the M value of the coordinates_  |
|  void | [**setVectorLayer**](#function-setvectorlayer) (QgsVectorLayer \* vectorLayer) <br>_The vector layer the model is for._  |
|  void | [**setVertex**](#function-setvertex) (int index, QgsPoint coordinate) <br>_Sets a vertex point at given index._  |
|  QgsVectorLayer \* | [**vectorLayer**](#function-vectorlayer-22) () const<br>_The vector layer the model is for._  |
|  QgsPoint | [**vertexAt**](#function-vertexat) (int index, const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()) const<br>_Returns the vertex point at a given_ _index_ _transformed to the given__crs_ _._ |
|  int | [**vertexCount**](#function-vertexcount-22) () const<br>_Total count of vertices._  |
|  QVector&lt; QgsPoint &gt; | [**vertices**](#function-vertices-22) () const<br>_All vertex points._  |
|  QVector&lt; QgsPoint &gt; | [**verticesCopy**](#function-verticescopy) (bool skipCurrentPoint=false) const<br>_Returns a copy of all vertice points in the model, with the option to skip the current vertex._  |




























## Detailed Description


This model manages a list of vertices. It can be used as a linestring or as a ring in a polygon. 


    
## Public Properties Documentation




### property crs [1/2]

_Coordinate reference system._ 
```C++
QgsCoordinateReferenceSystem RubberbandModel::crs;
```




<hr>



### property currentCoordinate [1/2]

_Current vertex point._ 
```C++
QgsPoint RubberbandModel::currentCoordinate;
```




<hr>



### property currentCoordinateIndex [1/2]

_Current vertex index._ 
```C++
int RubberbandModel::currentCoordinateIndex;
```




<hr>



### property currentPositionTimestamp [1/2]

_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_ 
```C++
QDateTime RubberbandModel::currentPositionTimestamp;
```




<hr>



### property firstCoordinate [1/2]

_First vertex point._ 
```C++
QgsPoint RubberbandModel::firstCoordinate;
```




<hr>



### property frozen [1/2]

_Freeze the rubberband so it doesn't get modified while panning map._ 
```C++
bool RubberbandModel::frozen;
```




<hr>



### property geometryType [1/2]

[_**Geometry**_](classGeometry.md) _type of the model._
```C++
Qgis::GeometryType RubberbandModel::geometryType;
```




<hr>



### property lastCoordinate [1/2]

_Last vertex point._ 
```C++
QgsPoint RubberbandModel::lastCoordinate;
```




<hr>



### property measureValue [1/2]

_measureValue defines the M value of the coordinates_ 
```C++
double RubberbandModel::measureValue;
```




<hr>



### property penultimateCoordinate [1/2]

_Penultimate (last but one) vertex point._ 
```C++
QgsPoint RubberbandModel::penultimateCoordinate;
```




<hr>



### property vectorLayer [1/2]

_The vector layer the model is for._ 
```C++
QgsVectorLayer * RubberbandModel::vectorLayer;
```




<hr>



### property vertexCount [1/2]

_Total count of vertices._ 
```C++
int RubberbandModel::vertexCount;
```




<hr>



### property vertices [1/2]

_All vertex points._ 
```C++
QVector<QgsPoint> RubberbandModel::vertices;
```




<hr>
## Public Signals Documentation




### signal crsChanged 

```C++
void RubberbandModel::crsChanged;
```




<hr>



### signal currentCoordinateChanged 

```C++
void RubberbandModel::currentCoordinateChanged;
```




<hr>



### signal currentCoordinateIndexChanged 

```C++
void RubberbandModel::currentCoordinateIndexChanged;
```




<hr>



### signal currentPositionTimestampChanged 

```C++
void RubberbandModel::currentPositionTimestampChanged;
```




<hr>



### signal frozenChanged 

_Freeze the rubberband so it doesn't get modified while panning map._ 
```C++
void RubberbandModel::frozenChanged;
```




<hr>



### signal geometryTypeChanged 

```C++
void RubberbandModel::geometryTypeChanged;
```




<hr>



### signal measureValueChanged 

```C++
void RubberbandModel::measureValueChanged;
```




<hr>



### signal vectorLayerChanged 

```C++
void RubberbandModel::vectorLayerChanged;
```




<hr>



### signal vertexChanged 

```C++
void RubberbandModel::vertexChanged;
```




<hr>



### signal vertexCountChanged 

```C++
void RubberbandModel::vertexCountChanged;
```




<hr>



### signal verticesInserted 

```C++
void RubberbandModel::verticesInserted;
```




<hr>



### signal verticesRemoved 

```C++
void RubberbandModel::verticesRemoved;
```




<hr>
## Public Functions Documentation




### function RubberbandModel 

```C++
explicit RubberbandModel::RubberbandModel (
    QObject * parent=nullptr
) 
```




<hr>



### function addVertex 

_Add vertex from the current position._ 
```C++
Q_INVOKABLE void RubberbandModel::addVertex () 
```




<hr>



### function addVertexFromPoint 

_Add vertex with a given point._ 
```C++
Q_INVOKABLE void RubberbandModel::addVertexFromPoint (
    const QgsPoint & point
) 
```




<hr>



### function crs [2/2]

_Coordinate reference system._ 
```C++
QgsCoordinateReferenceSystem RubberbandModel::crs () const
```




<hr>



### function currentCoordinate [2/2]

_Current vertex point._ 
```C++
QgsPoint RubberbandModel::currentCoordinate () const
```




<hr>



### function currentCoordinateIndex [2/2]

_Current vertex index._ 
```C++
int RubberbandModel::currentCoordinateIndex () const
```




<hr>



### function currentPoint 

_Returns the current vertex point transformed to the given_ _crs_ _._
```C++
QgsPoint RubberbandModel::currentPoint (
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem(),
    Qgis::WkbType wkbType=Qgis::WkbType::PointZ
) const
```




<hr>



### function currentPositionTimestamp [2/2]

_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_ 
```C++
QDateTime RubberbandModel::currentPositionTimestamp () const
```




<hr>



### function firstCoordinate [2/2]

_First vertex point._ 
```C++
QgsPoint RubberbandModel::firstCoordinate () const
```




<hr>



### function flatPointSequence 

_Returns all vertices points keeping only X and Y coordinates and transformed to the given_ _crs_ _._
```C++
QVector< QgsPointXY > RubberbandModel::flatPointSequence (
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()
) const
```




<hr>



### function frozen [2/2]

_Freeze the rubberband so it doesn't get modified while panning map._ 
```C++
bool RubberbandModel::frozen () const
```




<hr>



### function geometryType [2/2]

[_**Geometry**_](classGeometry.md) _type of the model._
```C++
Qgis::GeometryType RubberbandModel::geometryType () const
```




<hr>



### function insertVertices 

_Inserts vertices._ 
```C++
void RubberbandModel::insertVertices (
    int index,
    int count
) 
```




<hr>



### function isEmpty 

_Returns whether there is at least one vertex in the model._ 
```C++
bool RubberbandModel::isEmpty () const
```




<hr>



### function lastCoordinate [2/2]

_Last vertex point._ 
```C++
QgsPoint RubberbandModel::lastCoordinate () const
```




<hr>



### function measureValue [2/2]

_measureValue defines the M value of the coordinates_ 
```C++
double RubberbandModel::measureValue () const
```




<hr>



### function penultimateCoordinate [2/2]

_Penultimate (last but one) vertex point._ 
```C++
QgsPoint RubberbandModel::penultimateCoordinate () const
```




<hr>



### function pointSequence 

```C++
Q_INVOKABLE QgsPointSequence RubberbandModel::pointSequence (
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem(),
    Qgis::WkbType wkbType=Qgis::WkbType::Point,
    bool closeLine=false
) const
```



The target CRS into which points should be reprojected. To retrieve unprojected points pass an invalid QgsCoordinateReferenceSystem object.


By default coordinates will be returned unprojected. 


        

<hr>



### function removeVertex 

_Remove the vertex at the current index._ 
```C++
Q_INVOKABLE void RubberbandModel::removeVertex () 
```




<hr>



### function removeVertices 

_Remove_ _count_ _vertices starting at__index_ _._
```C++
void RubberbandModel::removeVertices (
    int index,
    int count,
    bool keepLast=true
) 
```




<hr>



### function reset 

_Reset the model, remove all vertices and restart the vertex index._ 
```C++
Q_INVOKABLE void RubberbandModel::reset (
    bool keepLast=true
) 
```




<hr>



### function setCrs 

_Coordinate reference system._ 
```C++
void RubberbandModel::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function setCurrentCoordinate 

_Current vertex point._ 
```C++
void RubberbandModel::setCurrentCoordinate (
    const QgsPoint & currentCoordinate
) 
```




<hr>



### function setCurrentCoordinateIndex 

_Current vertex index._ 
```C++
void RubberbandModel::setCurrentCoordinateIndex (
    int currentCoordinateIndex
) 
```




<hr>



### function setCurrentPositionTimestamp 

_currentPositionTimestamp is used externally by tracking, not (yet) stored in the coordinates (m) by the rubberbandmodel itself_ 
```C++
void RubberbandModel::setCurrentPositionTimestamp (
    const QDateTime & currentPositionTimestamp
) 
```




<hr>



### function setDataFromGeometry 

```C++
void RubberbandModel::setDataFromGeometry (
    QgsGeometry geometry,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()
) 
```



Sets the model data to match a given _geometry_ 

**Note:**

rings and multiparts are discarded 





        

<hr>



### function setFrozen 

_Freeze the rubberband so it doesn't get modified while panning map._ 
```C++
void RubberbandModel::setFrozen (
    const bool & frozen
) 
```




<hr>



### function setGeometryType 

[_**Geometry**_](classGeometry.md) _type of the model._
```C++
void RubberbandModel::setGeometryType (
    const Qgis::GeometryType & geometryType
) 
```




<hr>



### function setMeasureValue 

_measureValue defines the M value of the coordinates_ 
```C++
void RubberbandModel::setMeasureValue (
    const double measureValue
) 
```




<hr>



### function setVectorLayer 

_The vector layer the model is for._ 
```C++
void RubberbandModel::setVectorLayer (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function setVertex 

_Sets a vertex point at given index._ 
```C++
void RubberbandModel::setVertex (
    int index,
    QgsPoint coordinate
) 
```




<hr>



### function vectorLayer [2/2]

_The vector layer the model is for._ 
```C++
QgsVectorLayer * RubberbandModel::vectorLayer () const
```




<hr>



### function vertexAt 

_Returns the vertex point at a given_ _index_ _transformed to the given__crs_ _._
```C++
QgsPoint RubberbandModel::vertexAt (
    int index,
    const QgsCoordinateReferenceSystem & crs=QgsCoordinateReferenceSystem()
) const
```




<hr>



### function vertexCount [2/2]

_Total count of vertices._ 
```C++
int RubberbandModel::vertexCount () const
```




<hr>



### function vertices [2/2]

_All vertex points._ 
```C++
QVector< QgsPoint > RubberbandModel::vertices () const
```




<hr>



### function verticesCopy 

_Returns a copy of all vertice points in the model, with the option to skip the current vertex._ 
```C++
QVector< QgsPoint > RubberbandModel::verticesCopy (
    bool skipCurrentPoint=false
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/rubberbandmodel.h`

