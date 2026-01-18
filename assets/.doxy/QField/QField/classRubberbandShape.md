

# Class RubberbandShape



[**ClassList**](annotated.md) **>** [**RubberbandShape**](classRubberbandShape.md)



_The_ [_**RubberbandShape**_](classRubberbandShape.md) _class is used to provide the shape data to draw rubber bands on the map canvas using the QML Shape item. It is aimed to be used with either a_[_**VertexModel**_](classVertexModel.md) _or a_[_**RubberbandModel**_](classRubberbandModel.md) _._

* `#include <rubberbandshape.h>`



Inherits the following classes: QQuickItem


Inherited by the following classes: [Rubberband](classRubberband.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classRubberbandShape.md#property-color-12)  <br>[_**Color**_](classColor.md) _of the rubberband._ |
| property bool | [**freeze**](classRubberbandShape.md#property-freeze-12)  <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
| property Qgis::GeometryType | [**geometryType**](classRubberbandShape.md#property-geometrytype-12)  <br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
| property qreal | [**lineWidth**](classRubberbandShape.md#property-linewidth-12)  <br>_Line width of the rubberband._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classRubberbandShape.md#property-mapsettings-12)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) \* | [**model**](classRubberbandShape.md#property-model-12)  <br> |
| property QColor | [**outlineColor**](classRubberbandShape.md#property-outlinecolor-12)  <br>[_**Color**_](classColor.md) _of the rubberband outline._ |
| property QListQList&lt; QPointF &gt; | [**polylines**](classRubberbandShape.md#property-polylines-12)  <br>_List of polylines representing the rubber band._  |
| property Qgis::GeometryType | [**polylinesType**](classRubberbandShape.md#property-polylinestype-12)  <br>_The geometry type associated to the polylines._  |
| property [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](classRubberbandShape.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classRubberbandShape.md#signal-colorchanged)  <br>[_**Color**_](classColor.md) _of the rubberband._ |
| signal void | [**freezeChanged**](classRubberbandShape.md#signal-freezechanged)  <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
| signal void | [**geometryTypeChanged**](classRubberbandShape.md#signal-geometrytypechanged)  <br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
| signal void | [**lineWidthChanged**](classRubberbandShape.md#signal-linewidthchanged)  <br>_Line width of the rubberband._  |
| signal void | [**mapSettingsChanged**](classRubberbandShape.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classRubberbandShape.md#signal-modelchanged)  <br> |
| signal void | [**outlineColorChanged**](classRubberbandShape.md#signal-outlinecolorchanged)  <br>[_**Color**_](classColor.md) _of the rubberband outline._ |
| signal void | [**polylinesChanged**](classRubberbandShape.md#signal-polylineschanged)  <br>_List of polylines representing the rubber band._  |
| signal void | [**polylinesTypeChanged**](classRubberbandShape.md#signal-polylinestypechanged)  <br>_The geometry type associated to the polylines._  |
| signal void | [**vertexModelChanged**](classRubberbandShape.md#signal-vertexmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RubberbandShape**](#function-rubberbandshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](#function-color-22) () const<br>[_**Color**_](classColor.md) _of the rubberband._ |
|  bool | [**freeze**](#function-freeze-22) () const<br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  Qgis::GeometryType | [**geometryType**](#function-geometrytype-22) () const<br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  float | [**lineWidth**](#function-linewidth-22) () const<br>_Line width of the rubberband._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**model**](#function-model-22) () const<br> |
|  QColor | [**outlineColor**](#function-outlinecolor-22) () const<br>[_**Color**_](classColor.md) _of the rubberband outline._ |
|  QList&lt; QList&lt; QPointF &gt; &gt; | [**polylines**](#function-polylines-22) () const<br>_List of polylines representing the rubber band._  |
|  Qgis::GeometryType | [**polylinesType**](#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br>[_**Color**_](classColor.md) _of the rubberband._ |
|  void | [**setFreeze**](#function-setfreeze) (bool freeze) <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  void | [**setGeometryType**](#function-setgeometrytype) (const Qgis::GeometryType geometryType) <br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br>_Line width of the rubberband._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](#function-setmodel) ([**RubberbandModel**](classRubberbandModel.md) \* model) <br> |
|  void | [**setOutlineColor**](#function-setoutlinecolor) (const QColor & color) <br>[_**Color**_](classColor.md) _of the rubberband outline._ |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**VertexModel**](classVertexModel.md) \* vertexModel) <br> |
|  [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () const<br> |




























## Public Properties Documentation




### property color [1/2]

[_**Color**_](classColor.md) _of the rubberband._
```C++
QColor RubberbandShape::color;
```




<hr>



### property freeze [1/2]

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
bool RubberbandShape::freeze;
```




<hr>



### property geometryType [1/2]

[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
Qgis::GeometryType RubberbandShape::geometryType;
```




<hr>



### property lineWidth [1/2]

_Line width of the rubberband._ 
```C++
qreal RubberbandShape::lineWidth;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * RubberbandShape::mapSettings;
```




<hr>



### property model [1/2]

```C++
RubberbandModel * RubberbandShape::model;
```




<hr>



### property outlineColor [1/2]

[_**Color**_](classColor.md) _of the rubberband outline._
```C++
QColor RubberbandShape::outlineColor;
```




<hr>



### property polylines [1/2]

_List of polylines representing the rubber band._ 
```C++
QListQList<QPointF> RubberbandShape::polylines;
```




<hr>



### property polylinesType [1/2]

_The geometry type associated to the polylines._ 
```C++
Qgis::GeometryType RubberbandShape::polylinesType;
```




<hr>



### property vertexModel [1/2]

```C++
VertexModel * RubberbandShape::vertexModel;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

[_**Color**_](classColor.md) _of the rubberband._
```C++
void RubberbandShape::colorChanged;
```




<hr>



### signal freezeChanged 

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
void RubberbandShape::freezeChanged;
```




<hr>



### signal geometryTypeChanged 

[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
void RubberbandShape::geometryTypeChanged;
```




<hr>



### signal lineWidthChanged 

_Line width of the rubberband._ 
```C++
void RubberbandShape::lineWidthChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void RubberbandShape::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void RubberbandShape::modelChanged;
```




<hr>



### signal outlineColorChanged 

[_**Color**_](classColor.md) _of the rubberband outline._
```C++
void RubberbandShape::outlineColorChanged;
```




<hr>



### signal polylinesChanged 

_List of polylines representing the rubber band._ 
```C++
void RubberbandShape::polylinesChanged;
```




<hr>



### signal polylinesTypeChanged 

_The geometry type associated to the polylines._ 
```C++
void RubberbandShape::polylinesTypeChanged;
```




<hr>



### signal vertexModelChanged 

```C++
void RubberbandShape::vertexModelChanged;
```




<hr>
## Public Functions Documentation




### function RubberbandShape 

```C++
explicit RubberbandShape::RubberbandShape (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function color [2/2]

[_**Color**_](classColor.md) _of the rubberband._
```C++
QColor RubberbandShape::color () const
```




<hr>



### function freeze [2/2]

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
bool RubberbandShape::freeze () const
```




<hr>



### function geometryType [2/2]

[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
inline Qgis::GeometryType RubberbandShape::geometryType () const
```




<hr>



### function lineWidth [2/2]

_Line width of the rubberband._ 
```C++
float RubberbandShape::lineWidth () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * RubberbandShape::mapSettings () const
```




<hr>



### function model [2/2]

```C++
RubberbandModel * RubberbandShape::model () const
```




<hr>



### function outlineColor [2/2]

[_**Color**_](classColor.md) _of the rubberband outline._
```C++
QColor RubberbandShape::outlineColor () const
```




<hr>



### function polylines [2/2]

_List of polylines representing the rubber band._ 
```C++
inline QList< QList< QPointF > > RubberbandShape::polylines () const
```




<hr>



### function polylinesType [2/2]

_The geometry type associated to the polylines._ 
```C++
inline Qgis::GeometryType RubberbandShape::polylinesType () const
```




<hr>



### function setColor 

[_**Color**_](classColor.md) _of the rubberband._
```C++
void RubberbandShape::setColor (
    const QColor & color
) 
```




<hr>



### function setFreeze 

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
void RubberbandShape::setFreeze (
    bool freeze
) 
```




<hr>



### function setGeometryType 

[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
void RubberbandShape::setGeometryType (
    const Qgis::GeometryType geometryType
) 
```




<hr>



### function setLineWidth 

_Line width of the rubberband._ 
```C++
void RubberbandShape::setLineWidth (
    float width
) 
```




<hr>



### function setMapSettings 

```C++
void RubberbandShape::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setModel 

```C++
void RubberbandShape::setModel (
    RubberbandModel * model
) 
```




<hr>



### function setOutlineColor 

[_**Color**_](classColor.md) _of the rubberband outline._
```C++
void RubberbandShape::setOutlineColor (
    const QColor & color
) 
```




<hr>



### function setVertexModel 

```C++
void RubberbandShape::setVertexModel (
    VertexModel * vertexModel
) 
```




<hr>



### function vertexModel [2/2]

```C++
VertexModel * RubberbandShape::vertexModel () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/rubberbandshape.h`

