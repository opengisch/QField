

# Class QfRubberband



[**ClassList**](annotated.md) **>** [**QfRubberband**](classQfRubberband.md)








Inherits the following classes: [QfRubberbandShape](classQfRubberbandShape.md)


Inherited by the following classes: [Rubberband](classRubberband.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**activePolyline**](classQfRubberband.md#property-activepolyline)  <br> |
| property bool | [**showVertices**](classQfRubberband.md#property-showvertices)  <br> |


## Public Properties inherited from QfRubberbandShape

See [QfRubberbandShape](classQfRubberbandShape.md)

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








## Public Signals inherited from QfRubberbandShape

See [QfRubberbandShape](classQfRubberbandShape.md)

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








## Public Functions inherited from QfRubberbandShape

See [QfRubberbandShape](classQfRubberbandShape.md)

| Type | Name |
| ---: | :--- |
|   | [**QfRubberbandShape**](classQfRubberbandShape.md#function-qfrubberbandshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](classQfRubberbandShape.md#function-color-22) () const<br>_Color of the rubberband._  |
|  bool | [**freeze**](classQfRubberbandShape.md#function-freeze-22) () const<br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  Qgis::GeometryType | [**geometryType**](classQfRubberbandShape.md#function-geometrytype-22) () const<br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  float | [**lineWidth**](classQfRubberbandShape.md#function-linewidth-22) () const<br>_Line width of the rubberband._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfRubberbandShape.md#function-mapsettings-22) () const<br> |
|  [**QfRubberbandModel**](classQfRubberbandModel.md) \* | [**model**](classQfRubberbandShape.md#function-model-22) () const<br> |
|  QColor | [**outlineColor**](classQfRubberbandShape.md#function-outlinecolor-22) () const<br>_Color of the rubberband outline._  |
|  QList&lt; QPolygonF &gt; | [**polylines**](classQfRubberbandShape.md#function-polylines-22) () const<br>_List of polylines representing the rubber band._  |
|  Qgis::GeometryType | [**polylinesType**](classQfRubberbandShape.md#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](classQfRubberbandShape.md#function-setcolor) (const QColor & color) <br>_Color of the rubberband._  |
|  void | [**setFreeze**](classQfRubberbandShape.md#function-setfreeze) (bool freeze) <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  void | [**setGeometryType**](classQfRubberbandShape.md#function-setgeometrytype) (const Qgis::GeometryType geometryType) <br>[_**QfGeometry**_](classQfGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  void | [**setLineWidth**](classQfRubberbandShape.md#function-setlinewidth) (float width) <br>_Line width of the rubberband._  |
|  void | [**setMapSettings**](classQfRubberbandShape.md#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](classQfRubberbandShape.md#function-setmodel) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* model) <br> |
|  void | [**setOutlineColor**](classQfRubberbandShape.md#function-setoutlinecolor) (const QColor & color) <br>_Color of the rubberband outline._  |
|  void | [**setVertexModel**](classQfRubberbandShape.md#function-setvertexmodel) ([**QfVertexModel**](classQfVertexModel.md) \* vertexModel) <br> |
|  [**QfVertexModel**](classQfVertexModel.md) \* | [**vertexModel**](classQfRubberbandShape.md#function-vertexmodel-22) () const<br> |






















































## Public Properties Documentation




### property activePolyline 

```C++
var QfRubberband::activePolyline;
```




<hr>



### property showVertices 

```C++
bool QfRubberband::showVertices;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qml/QfRubberband.qml`

