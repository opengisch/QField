

# Class GeometryUtils



[**ClassList**](annotated.md) **>** [**GeometryUtils**](classGeometryUtils.md)








Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum int | [**GeometryOperationResult**](#enum-geometryoperationresult)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GeometryUtils**](#function-geometryutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE [**GeometryOperationResult**](classGeometryUtils.md#enum-geometryoperationresult) | [**addRingFromRubberband**](#function-addringfromrubberband) (QgsVectorLayer \* layer, QgsFeatureId fid, [**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel) <br>_Adds a ring to a polygon with given_ _fid_ _using the ring in the rubberband model._ |
|  Q\_INVOKABLE QgsRectangle | [**boundingBox**](#function-boundingbox) (const QgsGeometry & geometry) <br>_Returns the bounding box of a given_ _geometry_ _._ |
|  Q\_INVOKABLE QgsPoint | [**centroid**](#function-centroid) (const QgsGeometry & geometry) <br>_Creates a centroid point from a given_ _geometry_ _._ |
|  Q\_INVOKABLE QgsPoint | [**coordinateToPoint**](#function-coordinatetopoint) (const QGeoCoordinate & coor) <br>_Converts QGeoCoordinate to QgsPoint._  |
|  Q\_INVOKABLE QgsGeometry | [**createGeometryFromWkt**](#function-creategeometryfromwkt) (const QString & wkt) <br>_Creates a geometry from a WKT string._  |
|  Q\_INVOKABLE QgsRectangle | [**createRectangleFromPoints**](#function-createrectanglefrompoints) (const QgsPoint & p1, const QgsPoint & p2) <br>_Returns a rectangle from two points._  |
|  Q\_INVOKABLE double | [**distanceBetweenPoints**](#function-distancebetweenpoints) (const QgsPoint & start, const QgsPoint & end) <br>_Returns the distance between a pair of_ _start_ _and__end_ _points._ |
|  Q\_INVOKABLE QgsPoint | [**emptyPoint**](#function-emptypoint) () <br>_Returns an empty (i.e. null) point._  |
|  Q\_INVOKABLE [**GeometryOperationResult**](classGeometryUtils.md#enum-geometryoperationresult) | [**eraseFromRubberband**](#function-erasefromrubberband) (QgsVectorLayer \* layer, QgsFeatureId fid, [**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel) <br>[_**Reshape**_](classReshape.md) _a polygon with given__fid_ _using the ring in the rubberband model._ |
|  Q\_INVOKABLE QgsGeometry | [**lineFromRubberband**](#function-linefromrubberband) ([**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel, const QgsCoordinateReferenceSystem & crs, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) <br>_Returns a QgsGeometry with a line by using the point sequence in the rubberband model._  |
|  Q\_INVOKABLE QgsPoint | [**point**](#function-point) (double x, double y, double z=std::numeric\_limits&lt; double &gt;::quiet\_NaN(), double m=std::numeric\_limits&lt; double &gt;::quiet\_NaN()) <br>_Creates a point from_ _x_ _and__y_ _with optional__z_ _and__values_ _._ |
|  Q\_INVOKABLE QgsGeometry | [**polygonFromRubberband**](#function-polygonfromrubberband) ([**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel, const QgsCoordinateReferenceSystem & crs, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) <br>_Returns a QgsGeometry with a polygon by using the point sequence in the rubberband model._  |
|  Q\_INVOKABLE QgsPoint | [**reprojectPoint**](#function-reprojectpoint) (const QgsPoint & point, const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs) <br>_Returns a reprojected_ _point_ _from the stated__sourceCrs_ _to a__destinationCrs_ _._ |
|  Q\_INVOKABLE QgsPoint | [**reprojectPointToWgs84**](#function-reprojectpointtowgs84) (const QgsPoint & point, const QgsCoordinateReferenceSystem & crs) <br>_Returns a reprojected_ _point_ _from the stated__crs_ _to WGS84._ |
|  Q\_INVOKABLE QgsRectangle | [**reprojectRectangle**](#function-reprojectrectangle) (const QgsRectangle & rectangle, const QgsCoordinateReferenceSystem & sourceCrs, const QgsCoordinateReferenceSystem & destinationCrs) <br>_Returns a reprojected_ _rectangle_ _from the stated__sourceCrs_ _to a__destinationCrs_ _._ |
|  Q\_INVOKABLE [**GeometryOperationResult**](classGeometryUtils.md#enum-geometryoperationresult) | [**reshapeFromRubberband**](#function-reshapefromrubberband) (QgsVectorLayer \* layer, QgsFeatureId fid, [**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel) <br>[_**Reshape**_](classReshape.md) _a polygon with given__fid_ _using the ring in the rubberband model._ |
|  Q\_INVOKABLE [**GeometryOperationResult**](classGeometryUtils.md#enum-geometryoperationresult) | [**splitFeatureFromRubberband**](#function-splitfeaturefromrubberband) (QgsVectorLayer \* layer, QgsFeatureId fid, [**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel) <br> |
|  Q\_INVOKABLE QgsGeometry | [**variableWidthBufferByMFromRubberband**](#function-variablewidthbufferbymfromrubberband) ([**RubberbandModel**](classRubberbandModel.md) \* rubberBandModel, const QgsCoordinateReferenceSystem & crs) <br>_Creates a variable width buffer polygon using M values from a rubberband model._  |


























## Public Types Documentation




### enum GeometryOperationResult 

```C++
enum GeometryUtils::GeometryOperationResult {
    Success = 0,
    NothingHappened = 1000,
    InvalidBaseGeometry,
    InvalidInputGeometryType,
    SelectionIsEmpty,
    SelectionIsGreaterThanOne,
    GeometryEngineError,
    LayerNotEditable,
    AddPartSelectedGeometryNotFound,
    AddPartNotMultiGeometry,
    AddRingNotClosed,
    AddRingNotValid,
    AddRingCrossesExistingRings,
    AddRingNotInExistingFeature,
    SplitCannotSplitPoint
};
```




<hr>
## Public Functions Documentation




### function GeometryUtils 

```C++
explicit GeometryUtils::GeometryUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function addRingFromRubberband 

_Adds a ring to a polygon with given_ _fid_ _using the ring in the rubberband model._
```C++
static Q_INVOKABLE GeometryOperationResult GeometryUtils::addRingFromRubberband (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    RubberbandModel * rubberBandModel
) 
```




<hr>



### function boundingBox 

_Returns the bounding box of a given_ _geometry_ _._
```C++
static inline Q_INVOKABLE QgsRectangle GeometryUtils::boundingBox (
    const QgsGeometry & geometry
) 
```




<hr>



### function centroid 

_Creates a centroid point from a given_ _geometry_ _._
```C++
static Q_INVOKABLE QgsPoint GeometryUtils::centroid (
    const QgsGeometry & geometry
) 
```




<hr>



### function coordinateToPoint 

_Converts QGeoCoordinate to QgsPoint._ 
```C++
static Q_INVOKABLE QgsPoint GeometryUtils::coordinateToPoint (
    const QGeoCoordinate & coor
) 
```




<hr>



### function createGeometryFromWkt 

_Creates a geometry from a WKT string._ 
```C++
static Q_INVOKABLE QgsGeometry GeometryUtils::createGeometryFromWkt (
    const QString & wkt
) 
```




<hr>



### function createRectangleFromPoints 

_Returns a rectangle from two points._ 
```C++
static Q_INVOKABLE QgsRectangle GeometryUtils::createRectangleFromPoints (
    const QgsPoint & p1,
    const QgsPoint & p2
) 
```




<hr>



### function distanceBetweenPoints 

_Returns the distance between a pair of_ _start_ _and__end_ _points._
```C++
static Q_INVOKABLE double GeometryUtils::distanceBetweenPoints (
    const QgsPoint & start,
    const QgsPoint & end
) 
```




<hr>



### function emptyPoint 

_Returns an empty (i.e. null) point._ 
```C++
static inline Q_INVOKABLE QgsPoint GeometryUtils::emptyPoint () 
```




<hr>



### function eraseFromRubberband 

[_**Reshape**_](classReshape.md) _a polygon with given__fid_ _using the ring in the rubberband model._
```C++
static Q_INVOKABLE GeometryOperationResult GeometryUtils::eraseFromRubberband (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    RubberbandModel * rubberBandModel
) 
```




<hr>



### function lineFromRubberband 

_Returns a QgsGeometry with a line by using the point sequence in the rubberband model._ 
```C++
static Q_INVOKABLE QgsGeometry GeometryUtils::lineFromRubberband (
    RubberbandModel * rubberBandModel,
    const QgsCoordinateReferenceSystem & crs,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) 
```




<hr>



### function point 

_Creates a point from_ _x_ _and__y_ _with optional__z_ _and__values_ _._
```C++
static inline Q_INVOKABLE QgsPoint GeometryUtils::point (
    double x,
    double y,
    double z=std::numeric_limits< double >::quiet_NaN(),
    double m=std::numeric_limits< double >::quiet_NaN()
) 
```




<hr>



### function polygonFromRubberband 

_Returns a QgsGeometry with a polygon by using the point sequence in the rubberband model._ 
```C++
static Q_INVOKABLE QgsGeometry GeometryUtils::polygonFromRubberband (
    RubberbandModel * rubberBandModel,
    const QgsCoordinateReferenceSystem & crs,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) 
```




<hr>



### function reprojectPoint 

_Returns a reprojected_ _point_ _from the stated__sourceCrs_ _to a__destinationCrs_ _._
```C++
static Q_INVOKABLE QgsPoint GeometryUtils::reprojectPoint (
    const QgsPoint & point,
    const QgsCoordinateReferenceSystem & sourceCrs,
    const QgsCoordinateReferenceSystem & destinationCrs
) 
```




<hr>



### function reprojectPointToWgs84 

_Returns a reprojected_ _point_ _from the stated__crs_ _to WGS84._
```C++
static Q_INVOKABLE QgsPoint GeometryUtils::reprojectPointToWgs84 (
    const QgsPoint & point,
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>



### function reprojectRectangle 

_Returns a reprojected_ _rectangle_ _from the stated__sourceCrs_ _to a__destinationCrs_ _._
```C++
static Q_INVOKABLE QgsRectangle GeometryUtils::reprojectRectangle (
    const QgsRectangle & rectangle,
    const QgsCoordinateReferenceSystem & sourceCrs,
    const QgsCoordinateReferenceSystem & destinationCrs
) 
```




<hr>



### function reshapeFromRubberband 

[_**Reshape**_](classReshape.md) _a polygon with given__fid_ _using the ring in the rubberband model._
```C++
static Q_INVOKABLE GeometryOperationResult GeometryUtils::reshapeFromRubberband (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    RubberbandModel * rubberBandModel
) 
```




<hr>



### function splitFeatureFromRubberband 

```C++
static Q_INVOKABLE GeometryOperationResult GeometryUtils::splitFeatureFromRubberband (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    RubberbandModel * rubberBandModel
) 
```



Performs a split using the line in the rubberband model. 


        

<hr>



### function variableWidthBufferByMFromRubberband 

_Creates a variable width buffer polygon using M values from a rubberband model._ 
```C++
static Q_INVOKABLE QgsGeometry GeometryUtils::variableWidthBufferByMFromRubberband (
    RubberbandModel * rubberBandModel,
    const QgsCoordinateReferenceSystem & crs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/geometryutils.h`

