

# Class QfSnappingResult



[**ClassList**](annotated.md) **>** [**QfSnappingResult**](classQfSnappingResult.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isValid**](classQfSnappingResult.md#property-isvalid-12)  <br> |
| property QgsPoint | [**point**](classQfSnappingResult.md#property-point-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfSnappingResult**](#function-qfsnappingresult-13) () <br>_construct invalid match_  |
|   | [**QfSnappingResult**](#function-qfsnappingresult-23) ([**Type**](classQfSnappingResult.md#enum-type) t, QgsVectorLayer \* vl, QgsFeatureId fid, double dist, const QgsPoint & pt, int vertexIndex=0, const QgsPoint \* edgePoints=nullptr) <br> |
|   | [**QfSnappingResult**](#function-qfsnappingresult-33) (const QgsPointLocator::Match & match) <br> |
|  double | [**distance**](#function-distance) () const<br> |
|  void | [**edgePoints**](#function-edgepoints) (QgsPoint & pt1, QgsPoint & pt2) const<br>_Only for a valid edge match - obtain endpoints of the edge._  |
|  QgsFeatureId | [**featureId**](#function-featureid) () const<br> |
|  bool | [**hasArea**](#function-hasarea) () const<br> |
|  bool | [**hasEdge**](#function-hasedge) () const<br> |
|  bool | [**hasVertex**](#function-hasvertex) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  QgsVectorLayer \* | [**layer**](#function-layer) () const<br> |
|  bool | [**operator!=**](#function-operator) (const [**QfSnappingResult**](classQfSnappingResult.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfSnappingResult**](classQfSnappingResult.md) & other) const<br> |
|  QgsPoint | [**point**](#function-point-22) () const<br> |
|  void | [**setPoint**](#function-setpoint) (const QgsPoint point) <br>_to set the point individually (e.g. after emmiting Z)_  |
|  [**Type**](classQfSnappingResult.md#enum-type) | [**type**](#function-type) () const<br> |
|  int | [**vertexIndex**](#function-vertexindex) () const<br>_for vertex / edge match (first vertex of the edge)_  |




























## Public Types Documentation




### enum Type 

```C++
enum QfSnappingResult::Type {
    Invalid = 0,
    Vertex = 1,
    Edge = 2,
    Area = 4,
    All = Vertex | Edge | Area
};
```



The type of a snap result or the filter type for a snap request. 


        

<hr>
## Public Properties Documentation




### property isValid [1/2]

```C++
bool QfSnappingResult::isValid;
```




<hr>



### property point [1/2]

```C++
QgsPoint QfSnappingResult::point;
```




<hr>
## Public Functions Documentation




### function QfSnappingResult [1/3]

_construct invalid match_ 
```C++
QfSnappingResult::QfSnappingResult () 
```




<hr>



### function QfSnappingResult [2/3]

```C++
QfSnappingResult::QfSnappingResult (
    Type t,
    QgsVectorLayer * vl,
    QgsFeatureId fid,
    double dist,
    const QgsPoint & pt,
    int vertexIndex=0,
    const QgsPoint * edgePoints=nullptr
) 
```




<hr>



### function QfSnappingResult [3/3]

```C++
explicit QfSnappingResult::QfSnappingResult (
    const QgsPointLocator::Match & match
) 
```




<hr>



### function distance 

```C++
double QfSnappingResult::distance () const
```



for vertex / edge match units depending on what class returns it (geom.cache: layer units, map canvas snapper: dest crs units) 


        

<hr>



### function edgePoints 

_Only for a valid edge match - obtain endpoints of the edge._ 
```C++
void QfSnappingResult::edgePoints (
    QgsPoint & pt1,
    QgsPoint & pt2
) const
```




<hr>



### function featureId 

```C++
QgsFeatureId QfSnappingResult::featureId () const
```



The id of the feature to which the snapped geometry belongs. 


        

<hr>



### function hasArea 

```C++
bool QfSnappingResult::hasArea () const
```




<hr>



### function hasEdge 

```C++
bool QfSnappingResult::hasEdge () const
```




<hr>



### function hasVertex 

```C++
bool QfSnappingResult::hasVertex () const
```




<hr>



### function isValid [2/2]

```C++
bool QfSnappingResult::isValid () const
```




<hr>



### function layer 

```C++
QgsVectorLayer * QfSnappingResult::layer () const
```



The vector layer where the snap occurred. Will be null if the snap happened on an intersection. 


        

<hr>



### function operator!= 

```C++
inline bool QfSnappingResult::operator!= (
    const QfSnappingResult & other
) const
```




<hr>



### function operator== 

```C++
bool QfSnappingResult::operator== (
    const QfSnappingResult & other
) const
```




<hr>



### function point [2/2]

```C++
QgsPoint QfSnappingResult::point () const
```



for vertex / edge match coords depending on what class returns it (geom.cache: layer coords, map canvas snapper: dest coords) 


        

<hr>



### function setPoint 

_to set the point individually (e.g. after emmiting Z)_ 
```C++
void QfSnappingResult::setPoint (
    const QgsPoint point
) 
```




<hr>



### function type 

```C++
Type QfSnappingResult::type () const
```




<hr>



### function vertexIndex 

_for vertex / edge match (first vertex of the edge)_ 
```C++
int QfSnappingResult::vertexIndex () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfsnappingresult.h`

