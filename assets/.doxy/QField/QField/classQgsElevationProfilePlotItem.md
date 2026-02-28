

# Class QgsElevationProfilePlotItem



[**ClassList**](annotated.md) **>** [**QgsElevationProfilePlotItem**](classQgsElevationProfilePlotItem.md)








Inherits the following classes: Qgs2DPlot


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsElevationProfilePlotItem**](#function-qgselevationprofileplotitem) ([**QgsQuickElevationProfileCanvas**](classQgsQuickElevationProfileCanvas.md) \* canvas) <br> |
|  QRectF | [**plotArea**](#function-plotarea) () <br> |
|  bool | [**redrawResults**](#function-redrawresults) (const QString & sourceId) <br> |
|  void | [**renderContent**](#function-rendercontent) (QgsRenderContext & rc, const QRectF & plotArea) override<br> |
|  void | [**setRenderer**](#function-setrenderer) (QgsProfilePlotRenderer \* renderer) <br> |
|  void | [**updatePlot**](#function-updateplot) () <br> |
|  void | [**updateRect**](#function-updaterect) () <br> |




























## Public Functions Documentation




### function QgsElevationProfilePlotItem 

```C++
inline explicit QgsElevationProfilePlotItem::QgsElevationProfilePlotItem (
    QgsQuickElevationProfileCanvas * canvas
) 
```




<hr>



### function plotArea 

```C++
inline QRectF QgsElevationProfilePlotItem::plotArea () 
```




<hr>



### function redrawResults 

```C++
inline bool QgsElevationProfilePlotItem::redrawResults (
    const QString & sourceId
) 
```




<hr>



### function renderContent 

```C++
inline void QgsElevationProfilePlotItem::renderContent (
    QgsRenderContext & rc,
    const QRectF & plotArea
) override
```




<hr>



### function setRenderer 

```C++
inline void QgsElevationProfilePlotItem::setRenderer (
    QgsProfilePlotRenderer * renderer
) 
```




<hr>



### function updatePlot 

```C++
inline void QgsElevationProfilePlotItem::updatePlot () 
```




<hr>



### function updateRect 

```C++
inline void QgsElevationProfilePlotItem::updateRect () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickelevationprofilecanvas.cpp`

