

# Class Rubberband



[**ClassList**](annotated.md) **>** [**Rubberband**](classRubberband.md)








Inherits the following classes: [RubberbandShape](classRubberbandShape.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**showVertices**](classRubberband.md#property-showvertices)  <br> |


## Public Properties inherited from RubberbandShape

See [RubberbandShape](classRubberbandShape.md)

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








## Public Signals inherited from RubberbandShape

See [RubberbandShape](classRubberbandShape.md)

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








## Public Functions inherited from RubberbandShape

See [RubberbandShape](classRubberbandShape.md)

| Type | Name |
| ---: | :--- |
|   | [**RubberbandShape**](classRubberbandShape.md#function-rubberbandshape) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**color**](classRubberbandShape.md#function-color-22) () const<br>[_**Color**_](classColor.md) _of the rubberband._ |
|  bool | [**freeze**](classRubberbandShape.md#function-freeze-22) () const<br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  Qgis::GeometryType | [**geometryType**](classRubberbandShape.md#function-geometrytype-22) () const<br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  float | [**lineWidth**](classRubberbandShape.md#function-linewidth-22) () const<br>_Line width of the rubberband._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classRubberbandShape.md#function-mapsettings-22) () const<br> |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**model**](classRubberbandShape.md#function-model-22) () const<br> |
|  QColor | [**outlineColor**](classRubberbandShape.md#function-outlinecolor-22) () const<br>[_**Color**_](classColor.md) _of the rubberband outline._ |
|  QList&lt; QList&lt; QPointF &gt; &gt; | [**polylines**](classRubberbandShape.md#function-polylines-22) () const<br>_List of polylines representing the rubber band._  |
|  Qgis::GeometryType | [**polylinesType**](classRubberbandShape.md#function-polylinestype-22) () const<br>_The geometry type associated to the polylines._  |
|  void | [**setColor**](classRubberbandShape.md#function-setcolor) (const QColor & color) <br>[_**Color**_](classColor.md) _of the rubberband._ |
|  void | [**setFreeze**](classRubberbandShape.md#function-setfreeze) (bool freeze) <br>_When set to TRUE, changes to the linked rubber band or vertex model as well as map settings will be ignored and the rubber band shape data will be left untouched._  |
|  void | [**setGeometryType**](classRubberbandShape.md#function-setgeometrytype) (const Qgis::GeometryType geometryType) <br>[_**Geometry**_](classGeometry.md) _type used to render the rubber band (if not provided or set to null geometry, the type provided by the rubber band or vertex model will be used)_ |
|  void | [**setLineWidth**](classRubberbandShape.md#function-setlinewidth) (float width) <br>_Line width of the rubberband._  |
|  void | [**setMapSettings**](classRubberbandShape.md#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setModel**](classRubberbandShape.md#function-setmodel) ([**RubberbandModel**](classRubberbandModel.md) \* model) <br> |
|  void | [**setOutlineColor**](classRubberbandShape.md#function-setoutlinecolor) (const QColor & color) <br>[_**Color**_](classColor.md) _of the rubberband outline._ |
|  void | [**setVertexModel**](classRubberbandShape.md#function-setvertexmodel) ([**VertexModel**](classVertexModel.md) \* vertexModel) <br> |
|  [**VertexModel**](classVertexModel.md) \* | [**vertexModel**](classRubberbandShape.md#function-vertexmodel-22) () const<br> |






















































## Public Properties Documentation




### property showVertices 

```C++
bool Rubberband::showVertices;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/Rubberband.qml`

