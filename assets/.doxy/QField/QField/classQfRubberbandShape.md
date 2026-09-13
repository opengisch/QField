

# Class QfRubberbandShape



[**ClassList**](annotated.md) **>** [**QfRubberbandShape**](classQfRubberbandShape.md)



_The_ [_**QfRubberbandShape**_](classQfRubberbandShape.md) _class is used to provide the shape data to draw rubber bands on the map canvas using the QML Shape item. It is aimed to be used with either a_[_**QfVertexModel**_](classQfVertexModel.md) _or a_[_**QfRubberbandModel**_](classQfRubberbandModel.md) _._

* `#include <qfrubberbandshape.h>`



Inherits the following classes: QQuickItem


Inherited by the following classes: [QfRubberband](classQfRubberband.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfRubberbandShape.md#property-color-12)  <br>_Color of the rubberband._  |
| property bool | [**freeze**](classQfRubberbandShape.md#property-freeze-12)  <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
| property Qgis::GeometryType | [**geometryType**](classQfRubberbandShape.md#property-geometrytype-12)  <br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
| property qreal | [**lineWidth**](classQfRubberbandShape.md#property-linewidth-12)  <br>_Line width of the rubberband._  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfRubberbandShape.md#property-mapsettings-12)  <br> |
| property [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**model**](classQfRubberbandShape.md#property-model-12)  <br> |
| property QColor | [**outlineColor**](classQfRubberbandShape.md#property-outlinecolor-12)  <br>_Color of the rubberband outline._  |
| property QList&lt; QPolygonF &gt; | [**polylines**](classQfRubberbandShape.md#property-polylines-12)  <br>_List of polylines representing the rubber band._  |
| property Qgis::GeometryType | [**polylinesType**](classQfRubberbandShape.md#property-polylinestype-12)  <br>_The geometry type associated to the polylines._  |
| property [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](classQfRubberbandShape.md#property-vertexmodel-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**colorChanged**](classQfRubberbandShape.md#signal-colorchanged)  <br>_Color of the rubberband._  |
| signal void | [**freezeChanged**](classQfRubberbandShape.md#signal-freezechanged)  <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
| signal void | [**geometryTypeChanged**](classQfRubberbandShape.md#signal-geometrytypechanged)  <br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
| signal void | [**lineWidthChanged**](classQfRubberbandShape.md#signal-linewidthchanged)  <br>_Line width of the rubberband._  |
| signal void | [**mapSettingsChanged**](classQfRubberbandShape.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classQfRubberbandShape.md#signal-modelchanged)  <br> |
| signal void | [**outlineColorChanged**](classQfRubberbandShape.md#signal-outlinecolorchanged)  <br>_Color of the rubberband outline._  |
| signal void | [**polylinesChanged**](classQfRubberbandShape.md#signal-polylineschanged)  <br>_List of polylines representing the rubber band._  |
| signal void | [**polylinesTypeChanged**](classQfRubberbandShape.md#signal-polylinestypechanged)  <br>_The geometry type associated to the polylines._  |
| signal void | [**vertexModelChanged**](classQfRubberbandShape.md#signal-vertexmodelchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfRubberbandShape**](#function-qfrubberbandshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](#function-color-22) () const<br>_Color of the rubberband._  |
|  bool | [**freeze**](#function-freeze-22) () const<br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  Qgis::GeometryType | [**geometryType**](#function-geometrytype-22) () const<br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  float | [**lineWidth**](#function-linewidth-22) () const<br>_Line width of the rubberband._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**model**](#function-model-22) () const<br> |
|  QColor | [**outlineColor**](#function-outlinecolor-22) () const<br>_Color of the rubberband outline._  |
|  QList&lt; QPolygonF &gt; | [**polylines**](#function-polylines-22) () const<br>_List of polylines representing the rubber band._  |
|  Qgis::GeometryType | [**polylinesType**](#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](#function-setcolor) (const QColor & color) <br>_Color of the rubberband._  |
|  void | [**setFreeze**](#function-setfreeze) (bool freeze) <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  void | [**setGeometryType**](#function-setgeometrytype) (const Qgis::GeometryType geometryType) <br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  void | [**setLineWidth**](#function-setlinewidth) (float width) <br>_Line width of the rubberband._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](#function-setmodel) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* model) <br> |
|  void | [**setOutlineColor**](#function-setoutlinecolor) (const QColor & color) <br>_Color of the rubberband outline._  |
|  void | [**setVertexModel**](#function-setvertexmodel) ([**QfVertexModel**](classQfVertexModel.md) \* vertexModel) <br> |
|  [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](#function-vertexmodel-22) () const<br> |




























## Public Properties Documentation




### property color [1/2]

_Color of the rubberband._ 
```C++
QColor QfRubberbandShape::color;
```




<hr>



### property freeze [1/2]

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
bool QfRubberbandShape::freeze;
```




<hr>



### property geometryType [1/2]

[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
Qgis::GeometryType QfRubberbandShape::geometryType;
```




<hr>



### property lineWidth [1/2]

_Line width of the rubberband._ 
```C++
qreal QfRubberbandShape::lineWidth;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfRubberbandShape::mapSettings;
```




<hr>



### property model [1/2]

```C++
QfRubberbandModel * QfRubberbandShape::model;
```




<hr>



### property outlineColor [1/2]

_Color of the rubberband outline._ 
```C++
QColor QfRubberbandShape::outlineColor;
```




<hr>



### property polylines [1/2]

_List of polylines representing the rubber band._ 
```C++
QList<QPolygonF> QfRubberbandShape::polylines;
```




<hr>



### property polylinesType [1/2]

_The geometry type associated to the polylines._ 
```C++
Qgis::GeometryType QfRubberbandShape::polylinesType;
```




<hr>



### property vertexModel [1/2]

```C++
QfVertexModel * QfRubberbandShape::vertexModel;
```




<hr>
## Public Signals Documentation




### signal colorChanged 

_Color of the rubberband._ 
```C++
void QfRubberbandShape::colorChanged;
```




<hr>



### signal freezeChanged 

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
void QfRubberbandShape::freezeChanged;
```




<hr>



### signal geometryTypeChanged 

[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
void QfRubberbandShape::geometryTypeChanged;
```




<hr>



### signal lineWidthChanged 

_Line width of the rubberband._ 
```C++
void QfRubberbandShape::lineWidthChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfRubberbandShape::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void QfRubberbandShape::modelChanged;
```




<hr>



### signal outlineColorChanged 

_Color of the rubberband outline._ 
```C++
void QfRubberbandShape::outlineColorChanged;
```




<hr>



### signal polylinesChanged 

_List of polylines representing the rubber band._ 
```C++
void QfRubberbandShape::polylinesChanged;
```




<hr>



### signal polylinesTypeChanged 

_The geometry type associated to the polylines._ 
```C++
void QfRubberbandShape::polylinesTypeChanged;
```




<hr>



### signal vertexModelChanged 

```C++
void QfRubberbandShape::vertexModelChanged;
```




<hr>
## Public Functions Documentation




### function QfRubberbandShape 

```C++
explicit QfRubberbandShape::QfRubberbandShape (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function color [2/2]

_Color of the rubberband._ 
```C++
QColor QfRubberbandShape::color () const
```




<hr>



### function freeze [2/2]

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
bool QfRubberbandShape::freeze () const
```




<hr>



### function geometryType [2/2]

[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
inline Qgis::GeometryType QfRubberbandShape::geometryType () const
```




<hr>



### function lineWidth [2/2]

_Line width of the rubberband._ 
```C++
float QfRubberbandShape::lineWidth () const
```




<hr>



### function mapSettings [2/2]

```C++
QgsQuickMapSettings * QfRubberbandShape::mapSettings () const
```




<hr>



### function model [2/2]

```C++
QfRubberbandModel * QfRubberbandShape::model () const
```




<hr>



### function outlineColor [2/2]

_Color of the rubberband outline._ 
```C++
QColor QfRubberbandShape::outlineColor () const
```




<hr>



### function polylines [2/2]

_List of polylines representing the rubber band._ 
```C++
inline QList< QPolygonF > QfRubberbandShape::polylines () const
```




<hr>



### function polylinesType [2/2]

_The geometry type associated to the polylines._ 
```C++
inline Qgis::GeometryType QfRubberbandShape::polylinesType () const
```




<hr>



### function setColor 

_Color of the rubberband._ 
```C++
void QfRubberbandShape::setColor (
    const QColor & color
) 
```




<hr>



### function setFreeze 

_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._ 
```C++
void QfRubberbandShape::setFreeze (
    bool freeze
) 
```




<hr>



### function setGeometryType 

[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_
```C++
void QfRubberbandShape::setGeometryType (
    const Qgis::GeometryType geometryType
) 
```




<hr>



### function setLineWidth 

_Line width of the rubberband._ 
```C++
void QfRubberbandShape::setLineWidth (
    float width
) 
```




<hr>



### function setMapSettings 

```C++
void QfRubberbandShape::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setModel 

```C++
void QfRubberbandShape::setModel (
    QfRubberbandModel * model
) 
```




<hr>



### function setOutlineColor 

_Color of the rubberband outline._ 
```C++
void QfRubberbandShape::setOutlineColor (
    const QColor & color
) 
```




<hr>



### function setVertexModel 

```C++
void QfRubberbandShape::setVertexModel (
    QfVertexModel * vertexModel
) 
```




<hr>



### function vertexModel [2/2]

```C++
QfVertexModel * QfRubberbandShape::vertexModel () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfrubberbandshape.h`

