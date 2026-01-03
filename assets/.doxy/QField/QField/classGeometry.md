

# Class Geometry



[**ClassList**](annotated.md) **>** [**Geometry**](classGeometry.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](classGeometry.md#property-rubberbandmodel-12)  <br> |
| property QgsVectorLayer \* | [**vectorLayer**](classGeometry.md#property-vectorlayer-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**rubberbandModelChanged**](classGeometry.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**vectorLayerChanged**](classGeometry.md#signal-vectorlayerchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Geometry**](#function-geometry) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**applyRubberband**](#function-applyrubberband) () <br> |
|  QgsGeometry | [**asQgsGeometry**](#function-asqgsgeometry) () const<br> |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br> |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel) <br> |
|  void | [**setVectorLayer**](#function-setvectorlayer) (QgsVectorLayer \* vectorLayer) <br> |
|  void | [**updateRubberband**](#function-updaterubberband) (const QgsGeometry & geometry) <br> |
|  QgsVectorLayer \* | [**vectorLayer**](#function-vectorlayer-22) () const<br> |




























## Public Properties Documentation




### property rubberbandModel [1/2]

```C++
RubberbandModel * Geometry::rubberbandModel;
```




<hr>



### property vectorLayer [1/2]

```C++
QgsVectorLayer * Geometry::vectorLayer;
```




<hr>
## Public Signals Documentation




### signal rubberbandModelChanged 

```C++
void Geometry::rubberbandModelChanged;
```




<hr>



### signal vectorLayerChanged 

```C++
void Geometry::vectorLayerChanged;
```




<hr>
## Public Functions Documentation




### function Geometry 

```C++
explicit Geometry::Geometry (
    QObject * parent=nullptr
) 
```




<hr>



### function applyRubberband 

```C++
Q_INVOKABLE void Geometry::applyRubberband () 
```




<hr>



### function asQgsGeometry 

```C++
QgsGeometry Geometry::asQgsGeometry () const
```




<hr>



### function rubberbandModel [2/2]

```C++
RubberbandModel * Geometry::rubberbandModel () const
```




<hr>



### function setRubberbandModel 

```C++
void Geometry::setRubberbandModel (
    RubberbandModel * rubberbandModel
) 
```




<hr>



### function setVectorLayer 

```C++
void Geometry::setVectorLayer (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function updateRubberband 

```C++
void Geometry::updateRubberband (
    const QgsGeometry & geometry
) 
```




<hr>



### function vectorLayer [2/2]

```C++
QgsVectorLayer * Geometry::vectorLayer () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/geometry.h`

