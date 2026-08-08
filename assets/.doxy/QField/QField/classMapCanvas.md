

# Class MapCanvas



[**ClassList**](annotated.md) **>** [**MapCanvas**](classMapCanvas.md)








Inherits the following classes: [QfMapCanvas](classQfMapCanvas.md)








































## Public Properties inherited from QfMapCanvas

See [QfMapCanvas](classQfMapCanvas.md)

| Type | Name |
| ---: | :--- |
| property real | [**bottomMargin**](classQfMapCanvas.md#property-bottommargin)  <br> |
| property bool | [**forceDeferredLayersRepaint**](classQfMapCanvas.md#property-forcedeferredlayersrepaint)  <br> |
| property bool | [**freehandDigitizing**](classQfMapCanvas.md#property-freehanddigitizing)  <br> |
| property bool | [**hovered**](classQfMapCanvas.md#property-hovered)  <br> |
| property bool | [**incrementalRendering**](classQfMapCanvas.md#property-incrementalrendering)  <br> |
| property bool | [**interactive**](classQfMapCanvas.md#property-interactive)  <br> |
| property bool | [**isMapRotationEnabled**](classQfMapCanvas.md#property-ismaprotationenabled)  <br> |
| property bool | [**isRendering**](classQfMapCanvas.md#property-isrendering)  <br> |
| property bool | [**jumping**](classQfMapCanvas.md#property-jumping)  <br> |
| property [**QgsQuickMapCanvasMap**](classQgsQuickMapCanvasMap.md) | [**mapCanvasWrapper**](classQfMapCanvas.md#property-mapcanvaswrapper-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) | [**mapSettings**](classQfMapCanvas.md#property-mapsettings)  <br> |
| property bool | [**mouseAsTouchScreen**](classQfMapCanvas.md#property-mouseastouchscreen)  <br>_Treat mouse and touchpad input as touch screen input._  |
| property bool | [**pinched**](classQfMapCanvas.md#property-pinched)  <br> |
| property bool | [**previewJobsEnabled**](classQfMapCanvas.md#property-previewjobsenabled)  <br> |
| property list&lt; int &gt; | [**previewJobsQuadrants**](classQfMapCanvas.md#property-previewjobsquadrants)  <br> |
| property real | [**quality**](classQfMapCanvas.md#property-quality)  <br> |
| property real | [**rightMargin**](classQfMapCanvas.md#property-rightmargin)  <br> |
| property bool | [**smooth**](classQfMapCanvas.md#property-smooth)  <br> |








## Public Signals inherited from QfMapCanvas

See [QfMapCanvas](classQfMapCanvas.md)

| Type | Name |
| ---: | :--- |
| signal void | [**aboutToWheelZoom**](classQfMapCanvas.md#signal-abouttowheelzoom)  <br>_Emitted when a zoom action is about to occur, allowing for pre-zoom adjustments._  |
| signal void | [**clicked**](classQfMapCanvas.md#signal-clicked) (var point, var type) <br>_This signal is emitted independently of double tap / click._  |
| signal void | [**confirmedClicked**](classQfMapCanvas.md#signal-confirmedclicked) (var point) <br>_This signal is only emitted if there is no double tap/click coming after a short delay._  |
| signal void | [**doubleClicked**](classQfMapCanvas.md#signal-doubleclicked) (var point, var type) <br>_This signal is emitted on a double tap._  |
| signal void | [**longPressReleased**](classQfMapCanvas.md#signal-longpressreleased) (var type) <br>_Emitted when a release happens after a long press._  |
| signal void | [**longPressed**](classQfMapCanvas.md#signal-longpressed) (var point, var type) <br> |
| signal void | [**rightClicked**](classQfMapCanvas.md#signal-rightclicked) (var point, var type) <br>_This signal is emitted when a right mouse/stylus click occurs._  |








## Public Functions inherited from QfMapCanvas

See [QfMapCanvas](classQfMapCanvas.md)

| Type | Name |
| ---: | :--- |
|  void | [**freeze**](classQfMapCanvas.md#function-freeze) (id) <br> |
|  void | [**jumpTo**](classQfMapCanvas.md#function-jumpto) (point, scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**jumpToPosition**](classQfMapCanvas.md#function-jumptoposition) (positionSource, scale=-1, rotation=-1, handleMargins=false, callback=null) <br> |
|  void | [**refresh**](classQfMapCanvas.md#function-refresh) (ignoreFreeze) <br> |
|  void | [**stopRendering**](classQfMapCanvas.md#function-stoprendering) () <br> |
|  void | [**unfreeze**](classQfMapCanvas.md#function-unfreeze) (id) <br> |
|  void | [**zoomIn**](classQfMapCanvas.md#function-zoomin) (point) <br> |
|  void | [**zoomOut**](classQfMapCanvas.md#function-zoomout) (point) <br> |























































------------------------------
The documentation for this class was generated from the following file `src/app/qml_compat/MapCanvas.qml`

