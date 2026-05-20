

# Class SnappingResult



[**ClassList**](annotated.md) **>** [**SnappingResult**](classSnappingResult.md)






















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isValid**](classSnappingResult.md#property-isvalid-12)  <br> |
| property QgsPoint | [**point**](classSnappingResult.md#property-point-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**SnappingResult**](#function-snappingresult-13) () <br>_construct invalid match_  |
|   | [**SnappingResult**](#function-snappingresult-23) ([**Type**](classSnappingResult.md#enum-type) t, QgsVectorLayer \* vl, QgsFeatureId fid, double dist, const QgsPoint & pt, int vertexIndex=0, const QgsPoint \* edgePoints=nullptr) <br> |
|   | [**SnappingResult**](#function-snappingresult-33) (const QgsPointLocator::Match & match) <br> |
|  double | [**distance**](#function-distance) () const<br> |
|  void | [**edgePoints**](#function-edgepoints) (QgsPoint & pt1, QgsPoint & pt2) const<br>_Only for a valid edge match - obtain endpoints of the edge._  |
|  QgsFeatureId | [**featureId**](#function-featureid) () const<br> |
|  bool | [**hasArea**](#function-hasarea) () const<br> |
|  bool | [**hasEdge**](#function-hasedge) () const<br> |
|  bool | [**hasVertex**](#function-hasvertex) () const<br> |
|  bool | [**isValid**](#function-isvalid-22) () const<br> |
|  QgsVectorLayer \* | [**layer**](#function-layer) () const<br> |
|  bool | [**operator!=**](#function-operator) (const [**SnappingResult**](classSnappingResult.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**SnappingResult**](classSnappingResult.md) & other) const<br> |
|  QgsPoint | [**point**](#function-point-22) () const<br> |
|  void | [**setPoint**](#function-setpoint) (const QgsPoint point) <br>_to set the point individually (e.g. after emmiting Z)_  |
|  [**Type**](classSnappingResult.md#enum-type) | [**type**](#function-type) () const<br> |
|  int | [**vertexIndex**](#function-vertexindex) () const<br>_for vertex / edge match (first vertex of the edge)_  |




























## Public Types Documentation




### enum Type 

```C++
enum SnappingResult::Type {
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
bool SnappingResult::isValid;
```




<hr>



### property point [1/2]

```C++
QgsPoint SnappingResult::point;
```




<hr>
## Public Functions Documentation




### function SnappingResult [1/3]

_construct invalid match_ 
```C++
SnappingResult::SnappingResult () 
```




<hr>



### function SnappingResult [2/3]

```C++
SnappingResult::SnappingResult (
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



### function SnappingResult [3/3]

```C++
explicit SnappingResult::SnappingResult (
    const QgsPointLocator::Match & match
) 
```




<hr>



### function distance 

```C++
double SnappingResult::distance () const
```



for vertex / edge match units depending on what class returns it (geom.cache: layer units, map canvas snapper: dest crs units) 


        

<hr>



### function edgePoints 

_Only for a valid edge match - obtain endpoints of the edge._ 
```C++
void SnappingResult::edgePoints (
    QgsPoint & pt1,
    QgsPoint & pt2
) const
```




<hr>



### function featureId 

```C++
QgsFeatureId SnappingResult::featureId () const
```



The id of the feature to which the snapped geometry belongs. 


        

<hr>



### function hasArea 

```C++
bool SnappingResult::hasArea () const
```




<hr>



### function hasEdge 

```C++
bool SnappingResult::hasEdge () const
```




<hr>



### function hasVertex 

```C++
bool SnappingResult::hasVertex () const
```




<hr>



### function isValid [2/2]

```C++
bool SnappingResult::isValid () const
```




<hr>



### function layer 

```C++
QgsVectorLayer * SnappingResult::layer () const
```



The vector layer where the snap occurred. Will be null if the snap happened on an intersection. 


        

<hr>



### function operator!= 

```C++
inline bool SnappingResult::operator!= (
    const SnappingResult & other
) const
```




<hr>



### function operator== 

```C++
bool SnappingResult::operator== (
    const SnappingResult & other
) const
```




<hr>



### function point [2/2]

```C++
QgsPoint SnappingResult::point () const
```



for vertex / edge match coords depending on what class returns it (geom.cache: layer coords, map canvas snapper: dest coords) 


        

<hr>



### function setPoint 

_to set the point individually (e.g. after emmiting Z)_ 
```C++
void SnappingResult::setPoint (
    const QgsPoint point
) 
```




<hr>



### function type 

```C++
Type SnappingResult::type () const
```




<hr>



### function vertexIndex 

_for vertex / edge match (first vertex of the edge)_ 
```C++
int SnappingResult::vertexIndex () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/snappingresult.h`

