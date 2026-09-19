

# Class QgsQuickElevationProfileCanvas



[**ClassList**](annotated.md) **>** [**QgsQuickElevationProfileCanvas**](classQgsQuickElevationProfileCanvas.md)



[More...](#detailed-description)

* `#include <qgsquickelevationprofilecanvas.h>`



Inherits the following classes: QQuickItem






















## Public Slots

| Type | Name |
| ---: | :--- |
| slot Q\_INVOKABLE void | [**clear**](classQgsQuickElevationProfileCanvas.md#slot-clear)  <br> |
| slot Q\_INVOKABLE void | [**zoomFull**](classQgsQuickElevationProfileCanvas.md#slot-zoomfull)  <br> |
| slot Q\_INVOKABLE void | [**zoomFullInRatio**](classQgsQuickElevationProfileCanvas.md#slot-zoomfullinratio)  <br> |




## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**axisLabelColor**](classQgsQuickElevationProfileCanvas.md#property-axislabelcolor-12)  <br> |
| property double | [**axisLabelSize**](classQgsQuickElevationProfileCanvas.md#property-axislabelsize-12)  <br> |
| property QColor | [**backgroundColor**](classQgsQuickElevationProfileCanvas.md#property-backgroundcolor-12)  <br> |
| property QColor | [**borderColor**](classQgsQuickElevationProfileCanvas.md#property-bordercolor-12)  <br> |
| property QgsCoordinateReferenceSystem | [**crs**](classQgsQuickElevationProfileCanvas.md#property-crs-12)  <br> |
| property bool | [**isRendering**](classQgsQuickElevationProfileCanvas.md#property-isrendering-12)  <br> |
| property QgsGeometry | [**profileCurve**](classQgsQuickElevationProfileCanvas.md#property-profilecurve-12)  <br> |
| property QgsProject \* | [**project**](classQgsQuickElevationProfileCanvas.md#property-project-12)  <br> |
| property double | [**tolerance**](classQgsQuickElevationProfileCanvas.md#property-tolerance-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeJobCountChanged**](classQgsQuickElevationProfileCanvas.md#signal-activejobcountchanged) (int count) <br>_Emitted when the number of active background jobs changes._  |
| signal void | [**axisLabelColorChanged**](classQgsQuickElevationProfileCanvas.md#signal-axislabelcolorchanged)  <br>_Emitted when the axis label color changes._  |
| signal void | [**axisLabelSizeChanged**](classQgsQuickElevationProfileCanvas.md#signal-axislabelsizechanged)  <br>_Emitted when the axis label size (in point) changes._  |
| signal void | [**backgroundColorChanged**](classQgsQuickElevationProfileCanvas.md#signal-backgroundcolorchanged)  <br>_Emitted when the background color changes._  |
| signal void | [**borderColorChanged**](classQgsQuickElevationProfileCanvas.md#signal-bordercolorchanged)  <br>_Emitted when the border color changes._  |
| signal void | [**crsChanged**](classQgsQuickElevationProfileCanvas.md#signal-crschanged)  <br>_Emitted when the CRS linked to the profile curve geometry changes._  |
| signal void | [**isRenderingChanged**](classQgsQuickElevationProfileCanvas.md#signal-isrenderingchanged)  <br> |
| signal void | [**profileCurveChanged**](classQgsQuickElevationProfileCanvas.md#signal-profilecurvechanged)  <br>_Emitted when the profile curve geometry changes._  |
| signal void | [**projectChanged**](classQgsQuickElevationProfileCanvas.md#signal-projectchanged)  <br>_Emitted when the associated project changes._  |
| signal void | [**toleranceChanged**](classQgsQuickElevationProfileCanvas.md#signal-tolerancechanged)  <br>_Emitted when the tolerance changes._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsQuickElevationProfileCanvas**](#function-qgsquickelevationprofilecanvas) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**axisLabelColor**](#function-axislabelcolor-22) () const<br> |
|  double | [**axisLabelSize**](#function-axislabelsize-22) () const<br> |
|  QColor | [**backgroundColor**](#function-backgroundcolor-22) () const<br> |
|  QColor | [**borderColor**](#function-bordercolor-22) () const<br> |
|  void | [**cancelJobs**](#function-canceljobs) () <br> |
|  QgsCoordinateReferenceSystem | [**crs**](#function-crs-22) () const<br> |
|  bool | [**isRendering**](#function-isrendering-22) () const<br> |
|  QList&lt; QgsMapLayer \* &gt; | [**layers**](#function-layers) () const<br> |
|  Q\_INVOKABLE void | [**populateLayersFromProject**](#function-populatelayersfromproject) () <br> |
|  QgsGeometry | [**profileCurve**](#function-profilecurve-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  Q\_INVOKABLE void | [**refresh**](#function-refresh) () <br> |
|  void | [**setAxisLabelColor**](#function-setaxislabelcolor) (const QColor & color) <br> |
|  void | [**setAxisLabelSize**](#function-setaxislabelsize) (double size) <br> |
|  void | [**setBackgroundColor**](#function-setbackgroundcolor) (const QColor & color) <br> |
|  void | [**setBorderColor**](#function-setbordercolor) (const QColor & color) <br> |
|  void | [**setCrs**](#function-setcrs) (const QgsCoordinateReferenceSystem & crs) <br> |
|  void | [**setProfileCurve**](#function-setprofilecurve) (QgsGeometry curve) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |
|  void | [**setTolerance**](#function-settolerance) (double tolerance) <br> |
|  void | [**setVisiblePlotRange**](#function-setvisibleplotrange) (double minimumDistance, double maximumDistance, double minimumElevation, double maximumElevation) <br> |
|  double | [**tolerance**](#function-tolerance-22) () const<br> |
|  QSGNode \* | [**updatePaintNode**](#function-updatepaintnode) (QSGNode \* oldNode, QQuickItem::UpdatePaintNodeData \*) override<br> |
|  QgsDoubleRange | [**visibleDistanceRange**](#function-visibledistancerange) () const<br> |
|  QgsDoubleRange | [**visibleElevationRange**](#function-visibleelevationrange) () const<br> |
|   | [**~QgsQuickElevationProfileCanvas**](#function-qgsquickelevationprofilecanvas) () <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**geometryChange**](#function-geometrychange) (const QRectF & newGeometry, const QRectF & oldGeometry) override<br> |




## Detailed Description


Implements a visual Qt Quick Item that does elevation profile rendering according to the current project settings and a profile curve geometry. 


    
## Public Properties Documentation




### property axisLabelColor [1/2]

```C++
QColor QgsQuickElevationProfileCanvas::axisLabelColor;
```




<hr>



### property axisLabelSize [1/2]

```C++
double QgsQuickElevationProfileCanvas::axisLabelSize;
```




<hr>



### property backgroundColor [1/2]

```C++
QColor QgsQuickElevationProfileCanvas::backgroundColor;
```




<hr>



### property borderColor [1/2]

```C++
QColor QgsQuickElevationProfileCanvas::borderColor;
```




<hr>



### property crs [1/2]

```C++
QgsCoordinateReferenceSystem QgsQuickElevationProfileCanvas::crs;
```




<hr>



### property isRendering [1/2]

```C++
bool QgsQuickElevationProfileCanvas::isRendering;
```



The isRendering property is set to true while a rendering job is pending for this elevation profile canvas. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property. 


        

<hr>



### property profileCurve [1/2]

```C++
QgsGeometry QgsQuickElevationProfileCanvas::profileCurve;
```




<hr>



### property project [1/2]

```C++
QgsProject * QgsQuickElevationProfileCanvas::project;
```




<hr>



### property tolerance [1/2]

```C++
double QgsQuickElevationProfileCanvas::tolerance;
```




<hr>
## Public Slots Documentation




### slot clear 

```C++
void QgsQuickElevationProfileCanvas::clear;
```



Clears the current profile. 


        

<hr>



### slot zoomFull 

```C++
void QgsQuickElevationProfileCanvas::zoomFull;
```



Zooms to the full extent of the profile. 


        

<hr>



### slot zoomFullInRatio 

```C++
void QgsQuickElevationProfileCanvas::zoomFullInRatio;
```



Zooms to the full extent of the profile while maintaining X and Y axes' length ratio. 

**Note:**

This method only makes sense with CRSes having matching map units and elevation units types. 





        

<hr>
## Public Signals Documentation




### signal activeJobCountChanged 

_Emitted when the number of active background jobs changes._ 
```C++
void QgsQuickElevationProfileCanvas::activeJobCountChanged;
```




<hr>



### signal axisLabelColorChanged 

_Emitted when the axis label color changes._ 
```C++
void QgsQuickElevationProfileCanvas::axisLabelColorChanged;
```




<hr>



### signal axisLabelSizeChanged 

_Emitted when the axis label size (in point) changes._ 
```C++
void QgsQuickElevationProfileCanvas::axisLabelSizeChanged;
```




<hr>



### signal backgroundColorChanged 

_Emitted when the background color changes._ 
```C++
void QgsQuickElevationProfileCanvas::backgroundColorChanged;
```




<hr>



### signal borderColorChanged 

_Emitted when the border color changes._ 
```C++
void QgsQuickElevationProfileCanvas::borderColorChanged;
```




<hr>



### signal crsChanged 

_Emitted when the CRS linked to the profile curve geometry changes._ 
```C++
void QgsQuickElevationProfileCanvas::crsChanged;
```




<hr>



### signal isRenderingChanged 

```C++
void QgsQuickElevationProfileCanvas::isRenderingChanged;
```



The isRendering property is set to true while a rendering job is pending for this map canvas map. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property.   


        

<hr>



### signal profileCurveChanged 

_Emitted when the profile curve geometry changes._ 
```C++
void QgsQuickElevationProfileCanvas::profileCurveChanged;
```




<hr>



### signal projectChanged 

_Emitted when the associated project changes._ 
```C++
void QgsQuickElevationProfileCanvas::projectChanged;
```




<hr>



### signal toleranceChanged 

_Emitted when the tolerance changes._ 
```C++
void QgsQuickElevationProfileCanvas::toleranceChanged;
```




<hr>
## Public Functions Documentation




### function QgsQuickElevationProfileCanvas 

```C++
explicit QgsQuickElevationProfileCanvas::QgsQuickElevationProfileCanvas (
    QQuickItem * parent=nullptr
) 
```



Constructor for QgsElevationProfileCanvas, with the specified _parent_ widget. 


        

<hr>



### function axisLabelColor [2/2]

```C++
QColor QgsQuickElevationProfileCanvas::axisLabelColor () const
```



Returns the axis label color used when rendering the elevation profile.




**See also:** [**setAxisLabelColor**](classQgsQuickElevationProfileCanvas.md#function-setaxislabelcolor) 



        

<hr>



### function axisLabelSize [2/2]

```C++
double QgsQuickElevationProfileCanvas::axisLabelSize () const
```



Returns the axis label size (in point) used when rendering the elevation profile.




**See also:** [**setAxisLabelSize**](classQgsQuickElevationProfileCanvas.md#function-setaxislabelsize) 



        

<hr>



### function backgroundColor [2/2]

```C++
QColor QgsQuickElevationProfileCanvas::backgroundColor () const
```



Returns the background color used when rendering the elevation profile.




**See also:** [**setBackgroundColor**](classQgsQuickElevationProfileCanvas.md#function-setbackgroundcolor) 



        

<hr>



### function borderColor [2/2]

```C++
QColor QgsQuickElevationProfileCanvas::borderColor () const
```



Returns the border color used when rendering the elevation profile.




**See also:** [**setBorderColor**](classQgsQuickElevationProfileCanvas.md#function-setbordercolor) 



        

<hr>



### function cancelJobs 

```C++
void QgsQuickElevationProfileCanvas::cancelJobs () 
```




<hr>



### function crs [2/2]

```C++
inline QgsCoordinateReferenceSystem QgsQuickElevationProfileCanvas::crs () const
```



Returns the crs associated with map coordinates 


        

<hr>



### function isRendering [2/2]

```C++
bool QgsQuickElevationProfileCanvas::isRendering () const
```



The isRendering property is set to true while a rendering job is pending for this elevation profile canvas. It can be used to show a notification icon about an ongoing rendering job. This is a readonly property.   


        

<hr>



### function layers 

```C++
QList< QgsMapLayer * > QgsQuickElevationProfileCanvas::layers () const
```



Returns the list of layers included in the profile.




**See also:** [**layers()**](classQgsQuickElevationProfileCanvas.md#function-layers) 



        

<hr>



### function populateLayersFromProject 

```C++
Q_INVOKABLE void QgsQuickElevationProfileCanvas::populateLayersFromProject () 
```



Populates the current profile with elevation-enabled layers from the associated project. 


        

<hr>



### function profileCurve [2/2]

```C++
inline QgsGeometry QgsQuickElevationProfileCanvas::profileCurve () const
```



Returns the profile curve geometry.


The CRS associated with the curve is retrieved via crs().




**See also:** [**setProfileCurve()**](classQgsQuickElevationProfileCanvas.md#function-setprofilecurve) 



        

<hr>



### function project [2/2]

```C++
inline QgsProject * QgsQuickElevationProfileCanvas::project () const
```



Returns the project associated with the profile. 


        

<hr>



### function refresh 

```C++
Q_INVOKABLE void QgsQuickElevationProfileCanvas::refresh () 
```



Triggers a complete regeneration of the profile, causing the profile extraction to perform in the background. 


        

<hr>



### function setAxisLabelColor 

```C++
void QgsQuickElevationProfileCanvas::setAxisLabelColor (
    const QColor & color
) 
```



Sets the axis label color used when rendering the elevation profile.




**See also:** axisLabelColor 



        

<hr>



### function setAxisLabelSize 

```C++
void QgsQuickElevationProfileCanvas::setAxisLabelSize (
    double size
) 
```



Sets the axis label size (in point) used when rendering the elevation profile.




**See also:** axisLabelSize 



        

<hr>



### function setBackgroundColor 

```C++
void QgsQuickElevationProfileCanvas::setBackgroundColor (
    const QColor & color
) 
```



Sets the background color used when rendering the elevation profile.




**See also:** backgroundColor 



        

<hr>



### function setBorderColor 

```C++
void QgsQuickElevationProfileCanvas::setBorderColor (
    const QColor & color
) 
```



Sets the border color used when rendering the elevation profile.




**See also:** borderColor 



        

<hr>



### function setCrs 

```C++
void QgsQuickElevationProfileCanvas::setCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```



Sets the _crs_ associated with the map coordinates.




**See also:** crs() 



        

<hr>



### function setProfileCurve 

```C++
void QgsQuickElevationProfileCanvas::setProfileCurve (
    QgsGeometry curve
) 
```



Sets the profile _curve_ geometry.


The CRS associated with _curve_ is set via [**setCrs()**](classQgsQuickElevationProfileCanvas.md#function-setcrs).




**See also:** profileCurve() 



        

<hr>



### function setProject 

```C++
void QgsQuickElevationProfileCanvas::setProject (
    QgsProject * project
) 
```



Sets the _project_ associated with the profile.


This must be set before any layers which utilize terrain based elevation settings can be included in the canvas. 


        

<hr>



### function setTolerance 

```C++
void QgsQuickElevationProfileCanvas::setTolerance (
    double tolerance
) 
```



Sets the profile tolerance (in crs() units).


This value determines how far from the profileCurve() is appropriate for inclusion of results. For instance, when a profile is generated for a point vector layer this tolerance distance will dictate how far from the actual profile curve a point can reside within to be included in the results.




**See also:** tolerance() 



        

<hr>



### function setVisiblePlotRange 

```C++
void QgsQuickElevationProfileCanvas::setVisiblePlotRange (
    double minimumDistance,
    double maximumDistance,
    double minimumElevation,
    double maximumElevation
) 
```



Sets the visible area of the plot.




**See also:** [**visibleDistanceRange()**](classQgsQuickElevationProfileCanvas.md#function-visibledistancerange) 


**See also:** [**visibleElevationRange()**](classQgsQuickElevationProfileCanvas.md#function-visibleelevationrange) 



        

<hr>



### function tolerance [2/2]

```C++
inline double QgsQuickElevationProfileCanvas::tolerance () const
```



Returns the tolerance of the profile (in crs() units).


This value determines how far from the profileCurve() is appropriate for inclusion of results. For instance, when a profile is generated for a point vector layer this tolerance distance will dictate how far from the actual profile curve a point can reside within to be included in the results.




**See also:** [**setTolerance()**](classQgsQuickElevationProfileCanvas.md#function-settolerance) 



        

<hr>



### function updatePaintNode 

```C++
QSGNode * QgsQuickElevationProfileCanvas::updatePaintNode (
    QSGNode * oldNode,
    QQuickItem::UpdatePaintNodeData *
) override
```




<hr>



### function visibleDistanceRange 

```C++
QgsDoubleRange QgsQuickElevationProfileCanvas::visibleDistanceRange () const
```



Returns the distance range currently visible in the plot.




**See also:** [**visibleElevationRange()**](classQgsQuickElevationProfileCanvas.md#function-visibleelevationrange) 


**See also:** [**setVisiblePlotRange()**](classQgsQuickElevationProfileCanvas.md#function-setvisibleplotrange) 



        

<hr>



### function visibleElevationRange 

```C++
QgsDoubleRange QgsQuickElevationProfileCanvas::visibleElevationRange () const
```



Returns the elevation range currently visible in the plot.




**See also:** [**visibleDistanceRange()**](classQgsQuickElevationProfileCanvas.md#function-visibledistancerange) 


**See also:** [**setVisiblePlotRange()**](classQgsQuickElevationProfileCanvas.md#function-setvisibleplotrange) 



        

<hr>



### function ~QgsQuickElevationProfileCanvas 

```C++
QgsQuickElevationProfileCanvas::~QgsQuickElevationProfileCanvas () 
```




<hr>
## Protected Functions Documentation




### function geometryChange 

```C++
void QgsQuickElevationProfileCanvas::geometryChange (
    const QRectF & newGeometry,
    const QRectF & oldGeometry
) override
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickelevationprofilecanvas.h`

