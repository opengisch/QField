

# Class QfGeometry



[**ClassList**](annotated.md) **>** [**QfGeometry**](classQfGeometry.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](classQfGeometry.md#property-rubberbandmodel-12)  <br> |
| property QgsVectorLayer \* | [**vectorLayer**](classQfGeometry.md#property-vectorlayer-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**rubberbandModelChanged**](classQfGeometry.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**vectorLayerChanged**](classQfGeometry.md#signal-vectorlayerchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGeometry**](#function-qfgeometry) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**applyRubberband**](#function-applyrubberband) () <br> |
|  QgsGeometry | [**asQgsGeometry**](#function-asqgsgeometry) () const<br> |
|  [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br> |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel) <br> |
|  void | [**setVectorLayer**](#function-setvectorlayer) (QgsVectorLayer \* vectorLayer) <br> |
|  void | [**updateRubberband**](#function-updaterubberband) (const QgsGeometry & geometry) <br> |
|  QgsVectorLayer \* | [**vectorLayer**](#function-vectorlayer-22) () const<br> |




























## Public Properties Documentation




### property rubberbandModel [1/2]

```C++
QfRubberbandModel * QfGeometry::rubberbandModel;
```




<hr>



### property vectorLayer [1/2]

```C++
QgsVectorLayer * QfGeometry::vectorLayer;
```




<hr>
## Public Signals Documentation




### signal rubberbandModelChanged 

```C++
void QfGeometry::rubberbandModelChanged;
```




<hr>



### signal vectorLayerChanged 

```C++
void QfGeometry::vectorLayerChanged;
```




<hr>
## Public Functions Documentation




### function QfGeometry 

```C++
explicit QfGeometry::QfGeometry (
    QObject * parent=nullptr
) 
```




<hr>



### function applyRubberband 

```C++
Q_INVOKABLE void QfGeometry::applyRubberband () 
```




<hr>



### function asQgsGeometry 

```C++
QgsGeometry QfGeometry::asQgsGeometry () const
```




<hr>



### function rubberbandModel [2/2]

```C++
QfRubberbandModel * QfGeometry::rubberbandModel () const
```




<hr>



### function setRubberbandModel 

```C++
void QfGeometry::setRubberbandModel (
    QfRubberbandModel * rubberbandModel
) 
```




<hr>



### function setVectorLayer 

```C++
void QfGeometry::setVectorLayer (
    QgsVectorLayer * vectorLayer
) 
```




<hr>



### function updateRubberband 

```C++
void QfGeometry::updateRubberband (
    const QgsGeometry & geometry
) 
```




<hr>



### function vectorLayer [2/2]

```C++
QgsVectorLayer * QfGeometry::vectorLayer () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfgeometry.h`

