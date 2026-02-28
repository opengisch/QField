

# Class GridModel



[**ClassList**](annotated.md) **>** [**GridModel**](classGridModel.md)



[More...](#detailed-description)

* `#include <gridmodel.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**annotationColor**](classGridModel.md#property-annotationcolor-12)  <br> |
| property bool | [**annotationHasOutline**](classGridModel.md#property-annotationhasoutline-12)  <br> |
| property QColor | [**annotationOutlineColor**](classGridModel.md#property-annotationoutlinecolor-12)  <br> |
| property int | [**annotationPrecision**](classGridModel.md#property-annotationprecision-12)  <br> |
| property QList&lt; [**GridAnnotation**](classGridAnnotation.md) &gt; | [**annotations**](classGridModel.md#property-annotations-12)  <br> |
| property bool | [**autoColor**](classGridModel.md#property-autocolor-12)  <br> |
| property bool | [**enabled**](classGridModel.md#property-enabled-12)  <br> |
| property bool | [**indeterminate**](classGridModel.md#property-indeterminate-12)  <br> |
| property QColor | [**majorLineColor**](classGridModel.md#property-majorlinecolor-12)  <br> |
| property QListQList&lt; QPointF &gt; | [**majorLines**](classGridModel.md#property-majorlines-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classGridModel.md#property-mapsettings-12)  <br> |
| property QColor | [**markerColor**](classGridModel.md#property-markercolor-12)  <br> |
| property QList&lt; QPointF &gt; | [**markers**](classGridModel.md#property-markers-12)  <br> |
| property QColor | [**minorLineColor**](classGridModel.md#property-minorlinecolor-12)  <br> |
| property QListQList&lt; QPointF &gt; | [**minorLines**](classGridModel.md#property-minorlines-12)  <br> |
| property bool | [**prepareAnnotations**](classGridModel.md#property-prepareannotations-12)  <br> |
| property bool | [**prepareLines**](classGridModel.md#property-preparelines-12)  <br> |
| property bool | [**prepareMarkers**](classGridModel.md#property-preparemarkers-12)  <br> |
| property double | [**xInterval**](classGridModel.md#property-xinterval-12)  <br> |
| property double | [**xOffset**](classGridModel.md#property-xoffset-12)  <br> |
| property double | [**yInterval**](classGridModel.md#property-yinterval-12)  <br> |
| property double | [**yOffset**](classGridModel.md#property-yoffset-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**annotationColorChanged**](classGridModel.md#signal-annotationcolorchanged)  <br>_Emitted when the grid annotation text color has changed._  |
| signal void | [**annotationHasOutlineChanged**](classGridModel.md#signal-annotationhasoutlinechanged)  <br>_Emitted when the grid annotation text outline visibility has changed._  |
| signal void | [**annotationOutlineColorChanged**](classGridModel.md#signal-annotationoutlinecolorchanged)  <br>_Emitted when the grid annotation text outline color has changed._  |
| signal void | [**annotationPrecisionChanged**](classGridModel.md#signal-annotationprecisionchanged)  <br>_Emitted when the grid annotation text decimal precision has changed._  |
| signal void | [**annotationsChanged**](classGridModel.md#signal-annotationschanged)  <br>_Emitted when the grid annotations have changed._  |
| signal void | [**autoColorChanged**](classGridModel.md#signal-autocolorchanged)  <br>_Emitted when the grid line and marker colors are automatically assigned._  |
| signal void | [**enabledChanged**](classGridModel.md#signal-enabledchanged)  <br>_Emitted when the grid enabled setting has changed._  |
| signal void | [**indeterminateChanged**](classGridModel.md#signal-indeterminatechanged)  <br>_Emitted when the grid indeterminate setting has changed._  |
| signal void | [**majorLineColorChanged**](classGridModel.md#signal-majorlinecolorchanged)  <br>_Emitted when the grid major line color has changed._  |
| signal void | [**majorLinesChanged**](classGridModel.md#signal-majorlineschanged)  <br>_Emitted when the grid major lines have changed._  |
| signal void | [**mapSettingsChanged**](classGridModel.md#signal-mapsettingschanged)  <br>_Emitted when the map settings object has changed._  |
| signal void | [**markerColorChanged**](classGridModel.md#signal-markercolorchanged)  <br>_Emitted when the grid marker color has changed._  |
| signal void | [**markersChanged**](classGridModel.md#signal-markerschanged)  <br>_Emitted when the grid markers have changed._  |
| signal void | [**minorLineColorChanged**](classGridModel.md#signal-minorlinecolorchanged)  <br>_Emitted when the grid minor line color has changed._  |
| signal void | [**minorLinesChanged**](classGridModel.md#signal-minorlineschanged)  <br>_Emitted when the grid minor lines have changed._  |
| signal void | [**prepareAnnotationsChanged**](classGridModel.md#signal-prepareannotationschanged)  <br>_Emitted when grid annotations preparation setting has changed._  |
| signal void | [**prepareLinesChanged**](classGridModel.md#signal-preparelineschanged)  <br>_Emitted when grid lines preparation setting has changed._  |
| signal void | [**prepareMarkersChanged**](classGridModel.md#signal-preparemarkerschanged)  <br>_Emitted when grid markers preparation setting has changed._  |
| signal void | [**xIntervalChanged**](classGridModel.md#signal-xintervalchanged)  <br>_Emitted when the grid X interval has changed._  |
| signal void | [**xOffsetChanged**](classGridModel.md#signal-xoffsetchanged)  <br>_Emitted when the grid X offset has changed._  |
| signal void | [**yIntervalChanged**](classGridModel.md#signal-yintervalchanged)  <br>_Emitted when the grid Y interval has changed._  |
| signal void | [**yOffsetChanged**](classGridModel.md#signal-yoffsetchanged)  <br>_Emitted when the grid Y offset has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GridModel**](#function-gridmodel) (QObject \* parent=nullptr) <br>_Default constructor._  |
|  QColor | [**annotationColor**](#function-annotationcolor-22) () const<br>_Returns the grid annotation text color._  |
|  bool | [**annotationHasOutline**](#function-annotationhasoutline-22) () const<br>_Returns whether annotation text have an outline color._  |
|  QColor | [**annotationOutlineColor**](#function-annotationoutlinecolor-22) () const<br>_Returns the grid annotation text outline color._  |
|  int | [**annotationPrecision**](#function-annotationprecision-22) () const<br>_Returns the annotation text decimal precision._  |
|  QList&lt; [**GridAnnotation**](classGridAnnotation.md) &gt; | [**annotations**](#function-annotations-22) () const<br>_Returns the grid annotations._  |
|  bool | [**autoColor**](#function-autocolor-22) () const<br> |
|  bool | [**enabled**](#function-enabled-22) () const<br>_Returns TRUE when grid elements will be prepared._  |
|  bool | [**indeterminate**](#function-indeterminate-22) () const<br> |
|  QColor | [**majorLineColor**](#function-majorlinecolor-22) () const<br>_Returns the grid major line color._  |
|  QList&lt; QList&lt; QPointF &gt; &gt; | [**majorLines**](#function-majorlines-22) () const<br>_Returns the grid major lines._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings object._  |
|  QColor | [**markerColor**](#function-markercolor-22) () const<br>_Returns the grid marker color._  |
|  QList&lt; QPointF &gt; | [**markers**](#function-markers-22) () const<br>_Returns the grid markers._  |
|  QColor | [**minorLineColor**](#function-minorlinecolor-22) () const<br>_Returns the grid minor line color._  |
|  QList&lt; QList&lt; QPointF &gt; &gt; | [**minorLines**](#function-minorlines-22) () const<br>_Returns the grid minor lines._  |
|  bool | [**prepareAnnotations**](#function-prepareannotations-22) () const<br>_Returns whether grid annotations will be prepared._  |
|  bool | [**prepareLines**](#function-preparelines-22) () const<br>_Returns whether grid lines will be prepared._  |
|  bool | [**prepareMarkers**](#function-preparemarkers-22) () const<br>_Returns whether grid markers will be prepared._  |
|  void | [**setAnnotationColor**](#function-setannotationcolor) (const QColor & color) <br>_Sets the grid annotation text color._  |
|  void | [**setAnnotationHasOutline**](#function-setannotationhasoutline) (bool hasOutline) <br>_Sets whether annotation text have an outline color._  |
|  void | [**setAnnotationOutlineColor**](#function-setannotationoutlinecolor) (const QColor & color) <br>_Sets the grid annotation text outline color._  |
|  void | [**setAnnotationPrecision**](#function-setannotationprecision) (int precision) <br>_Sets the annotation text decimal precision._  |
|  void | [**setAutoColor**](#function-setautocolor) (bool autoColor) <br> |
|  void | [**setEnabled**](#function-setenabled) (bool enabled) <br>_Sets whether grid elements will be prepared._  |
|  void | [**setIndeterminate**](#function-setindeterminate) (bool indeterminate) <br> |
|  void | [**setMajorLineColor**](#function-setmajorlinecolor) (const QColor & color) <br>_Sets the grid major line color._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Sets the map settings object._  |
|  void | [**setMarkerColor**](#function-setmarkercolor) (const QColor & color) <br>_Sets the grid marker color._  |
|  void | [**setMinorLineColor**](#function-setminorlinecolor) (const QColor & color) <br>_Sets the grid minor line color._  |
|  void | [**setPrepareAnnotations**](#function-setprepareannotations) (bool prepare) <br>_Sets whether grid annotations will be prepared._  |
|  void | [**setPrepareLines**](#function-setpreparelines) (bool prepare) <br>_Sets whether grid lines will be prepared._  |
|  void | [**setPrepareMarkers**](#function-setpreparemarkers) (bool prepare) <br>_Sets whether grid markers will be prepared._  |
|  void | [**setXInterval**](#function-setxinterval) (double interval) <br>_Sets the grid X interval._  |
|  void | [**setXOffset**](#function-setxoffset) (double offset) <br>_Sets the grid X interval._  |
|  void | [**setYInterval**](#function-setyinterval) (double interval) <br>_Sets the grid Y interval._  |
|  void | [**setYOffset**](#function-setyoffset) (double offset) <br>_Sets the grid Y interval._  |
|  double | [**xInterval**](#function-xinterval-22) () const<br>_Returns the grid X interval._  |
|  double | [**xOffset**](#function-xoffset-22) () const<br>_Returns the grid X interval._  |
|  double | [**yInterval**](#function-yinterval-22) () const<br>_Returns the grid Y interval._  |
|  double | [**yOffset**](#function-yoffset-22) () const<br>_Returns the grid Y interval._  |




























## Detailed Description


This model manages lists of lines, markers, and annotations representing a grid. 


    
## Public Properties Documentation




### property annotationColor [1/2]

```C++
QColor GridModel::annotationColor;
```




<hr>



### property annotationHasOutline [1/2]

```C++
bool GridModel::annotationHasOutline;
```




<hr>



### property annotationOutlineColor [1/2]

```C++
QColor GridModel::annotationOutlineColor;
```




<hr>



### property annotationPrecision [1/2]

```C++
int GridModel::annotationPrecision;
```




<hr>



### property annotations [1/2]

```C++
QList<GridAnnotation> GridModel::annotations;
```




<hr>



### property autoColor [1/2]

```C++
bool GridModel::autoColor;
```




<hr>



### property enabled [1/2]

```C++
bool GridModel::enabled;
```




<hr>



### property indeterminate [1/2]

```C++
bool GridModel::indeterminate;
```




<hr>



### property majorLineColor [1/2]

```C++
QColor GridModel::majorLineColor;
```




<hr>



### property majorLines [1/2]

```C++
QListQList<QPointF> GridModel::majorLines;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * GridModel::mapSettings;
```




<hr>



### property markerColor [1/2]

```C++
QColor GridModel::markerColor;
```




<hr>



### property markers [1/2]

```C++
QList<QPointF> GridModel::markers;
```




<hr>



### property minorLineColor [1/2]

```C++
QColor GridModel::minorLineColor;
```




<hr>



### property minorLines [1/2]

```C++
QListQList<QPointF> GridModel::minorLines;
```




<hr>



### property prepareAnnotations [1/2]

```C++
bool GridModel::prepareAnnotations;
```




<hr>



### property prepareLines [1/2]

```C++
bool GridModel::prepareLines;
```




<hr>



### property prepareMarkers [1/2]

```C++
bool GridModel::prepareMarkers;
```




<hr>



### property xInterval [1/2]

```C++
double GridModel::xInterval;
```




<hr>



### property xOffset [1/2]

```C++
double GridModel::xOffset;
```




<hr>



### property yInterval [1/2]

```C++
double GridModel::yInterval;
```




<hr>



### property yOffset [1/2]

```C++
double GridModel::yOffset;
```




<hr>
## Public Signals Documentation




### signal annotationColorChanged 

_Emitted when the grid annotation text color has changed._ 
```C++
void GridModel::annotationColorChanged;
```




<hr>



### signal annotationHasOutlineChanged 

_Emitted when the grid annotation text outline visibility has changed._ 
```C++
void GridModel::annotationHasOutlineChanged;
```




<hr>



### signal annotationOutlineColorChanged 

_Emitted when the grid annotation text outline color has changed._ 
```C++
void GridModel::annotationOutlineColorChanged;
```




<hr>



### signal annotationPrecisionChanged 

_Emitted when the grid annotation text decimal precision has changed._ 
```C++
void GridModel::annotationPrecisionChanged;
```




<hr>



### signal annotationsChanged 

_Emitted when the grid annotations have changed._ 
```C++
void GridModel::annotationsChanged;
```




<hr>



### signal autoColorChanged 

_Emitted when the grid line and marker colors are automatically assigned._ 
```C++
void GridModel::autoColorChanged;
```




<hr>



### signal enabledChanged 

_Emitted when the grid enabled setting has changed._ 
```C++
void GridModel::enabledChanged;
```




<hr>



### signal indeterminateChanged 

_Emitted when the grid indeterminate setting has changed._ 
```C++
void GridModel::indeterminateChanged;
```




<hr>



### signal majorLineColorChanged 

_Emitted when the grid major line color has changed._ 
```C++
void GridModel::majorLineColorChanged;
```




<hr>



### signal majorLinesChanged 

_Emitted when the grid major lines have changed._ 
```C++
void GridModel::majorLinesChanged;
```




<hr>



### signal mapSettingsChanged 

_Emitted when the map settings object has changed._ 
```C++
void GridModel::mapSettingsChanged;
```




<hr>



### signal markerColorChanged 

_Emitted when the grid marker color has changed._ 
```C++
void GridModel::markerColorChanged;
```




<hr>



### signal markersChanged 

_Emitted when the grid markers have changed._ 
```C++
void GridModel::markersChanged;
```




<hr>



### signal minorLineColorChanged 

_Emitted when the grid minor line color has changed._ 
```C++
void GridModel::minorLineColorChanged;
```




<hr>



### signal minorLinesChanged 

_Emitted when the grid minor lines have changed._ 
```C++
void GridModel::minorLinesChanged;
```




<hr>



### signal prepareAnnotationsChanged 

_Emitted when grid annotations preparation setting has changed._ 
```C++
void GridModel::prepareAnnotationsChanged;
```




<hr>



### signal prepareLinesChanged 

_Emitted when grid lines preparation setting has changed._ 
```C++
void GridModel::prepareLinesChanged;
```




<hr>



### signal prepareMarkersChanged 

_Emitted when grid markers preparation setting has changed._ 
```C++
void GridModel::prepareMarkersChanged;
```




<hr>



### signal xIntervalChanged 

_Emitted when the grid X interval has changed._ 
```C++
void GridModel::xIntervalChanged;
```




<hr>



### signal xOffsetChanged 

_Emitted when the grid X offset has changed._ 
```C++
void GridModel::xOffsetChanged;
```




<hr>



### signal yIntervalChanged 

_Emitted when the grid Y interval has changed._ 
```C++
void GridModel::yIntervalChanged;
```




<hr>



### signal yOffsetChanged 

_Emitted when the grid Y offset has changed._ 
```C++
void GridModel::yOffsetChanged;
```




<hr>
## Public Functions Documentation




### function GridModel 

_Default constructor._ 
```C++
explicit GridModel::GridModel (
    QObject * parent=nullptr
) 
```




<hr>



### function annotationColor [2/2]

_Returns the grid annotation text color._ 
```C++
inline QColor GridModel::annotationColor () const
```




<hr>



### function annotationHasOutline [2/2]

_Returns whether annotation text have an outline color._ 
```C++
inline bool GridModel::annotationHasOutline () const
```




<hr>



### function annotationOutlineColor [2/2]

_Returns the grid annotation text outline color._ 
```C++
inline QColor GridModel::annotationOutlineColor () const
```




<hr>



### function annotationPrecision [2/2]

_Returns the annotation text decimal precision._ 
```C++
inline int GridModel::annotationPrecision () const
```




<hr>



### function annotations [2/2]

_Returns the grid annotations._ 
```C++
inline QList< GridAnnotation > GridModel::annotations () const
```




<hr>



### function autoColor [2/2]

```C++
inline bool GridModel::autoColor () const
```



Returns whether grid line and marker colors will be automatically assigned to colors derived from the map canvas background color. 


        

<hr>



### function enabled [2/2]

_Returns TRUE when grid elements will be prepared._ 
```C++
inline bool GridModel::enabled () const
```




<hr>



### function indeterminate [2/2]

```C++
inline bool GridModel::indeterminate () const
```



Returns TRUE when grid will adopt an indeterminate behavior and ignore the interval and offset values. 


        

<hr>



### function majorLineColor [2/2]

_Returns the grid major line color._ 
```C++
inline QColor GridModel::majorLineColor () const
```




<hr>



### function majorLines [2/2]

_Returns the grid major lines._ 
```C++
inline QList< QList< QPointF > > GridModel::majorLines () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings object._ 
```C++
inline QgsQuickMapSettings * GridModel::mapSettings () const
```




<hr>



### function markerColor [2/2]

_Returns the grid marker color._ 
```C++
inline QColor GridModel::markerColor () const
```




<hr>



### function markers [2/2]

_Returns the grid markers._ 
```C++
inline QList< QPointF > GridModel::markers () const
```




<hr>



### function minorLineColor [2/2]

_Returns the grid minor line color._ 
```C++
inline QColor GridModel::minorLineColor () const
```




<hr>



### function minorLines [2/2]

_Returns the grid minor lines._ 
```C++
inline QList< QList< QPointF > > GridModel::minorLines () const
```




<hr>



### function prepareAnnotations [2/2]

_Returns whether grid annotations will be prepared._ 
```C++
inline bool GridModel::prepareAnnotations () const
```




<hr>



### function prepareLines [2/2]

_Returns whether grid lines will be prepared._ 
```C++
inline bool GridModel::prepareLines () const
```




<hr>



### function prepareMarkers [2/2]

_Returns whether grid markers will be prepared._ 
```C++
inline bool GridModel::prepareMarkers () const
```




<hr>



### function setAnnotationColor 

_Sets the grid annotation text color._ 
```C++
void GridModel::setAnnotationColor (
    const QColor & color
) 
```




<hr>



### function setAnnotationHasOutline 

_Sets whether annotation text have an outline color._ 
```C++
void GridModel::setAnnotationHasOutline (
    bool hasOutline
) 
```




<hr>



### function setAnnotationOutlineColor 

_Sets the grid annotation text outline color._ 
```C++
void GridModel::setAnnotationOutlineColor (
    const QColor & color
) 
```




<hr>



### function setAnnotationPrecision 

_Sets the annotation text decimal precision._ 
```C++
void GridModel::setAnnotationPrecision (
    int precision
) 
```




<hr>



### function setAutoColor 

```C++
void GridModel::setAutoColor (
    bool autoColor
) 
```



Set whether grid line and marker colors will be automatically assigned to colors derived from the map canvas background color. 


        

<hr>



### function setEnabled 

_Sets whether grid elements will be prepared._ 
```C++
void GridModel::setEnabled (
    bool enabled
) 
```




<hr>



### function setIndeterminate 

```C++
void GridModel::setIndeterminate (
    bool indeterminate
) 
```



Sets whether the grid will adopt an indeterminate behavior and ignore the interval and offset values. 


        

<hr>



### function setMajorLineColor 

_Sets the grid major line color._ 
```C++
void GridModel::setMajorLineColor (
    const QColor & color
) 
```




<hr>



### function setMapSettings 

_Sets the map settings object._ 
```C++
void GridModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setMarkerColor 

_Sets the grid marker color._ 
```C++
void GridModel::setMarkerColor (
    const QColor & color
) 
```




<hr>



### function setMinorLineColor 

_Sets the grid minor line color._ 
```C++
void GridModel::setMinorLineColor (
    const QColor & color
) 
```




<hr>



### function setPrepareAnnotations 

_Sets whether grid annotations will be prepared._ 
```C++
void GridModel::setPrepareAnnotations (
    bool prepare
) 
```




<hr>



### function setPrepareLines 

_Sets whether grid lines will be prepared._ 
```C++
void GridModel::setPrepareLines (
    bool prepare
) 
```




<hr>



### function setPrepareMarkers 

_Sets whether grid markers will be prepared._ 
```C++
void GridModel::setPrepareMarkers (
    bool prepare
) 
```




<hr>



### function setXInterval 

_Sets the grid X interval._ 
```C++
void GridModel::setXInterval (
    double interval
) 
```




<hr>



### function setXOffset 

_Sets the grid X interval._ 
```C++
void GridModel::setXOffset (
    double offset
) 
```




<hr>



### function setYInterval 

_Sets the grid Y interval._ 
```C++
void GridModel::setYInterval (
    double interval
) 
```




<hr>



### function setYOffset 

_Sets the grid Y interval._ 
```C++
void GridModel::setYOffset (
    double offset
) 
```




<hr>



### function xInterval [2/2]

_Returns the grid X interval._ 
```C++
inline double GridModel::xInterval () const
```




<hr>



### function xOffset [2/2]

_Returns the grid X interval._ 
```C++
inline double GridModel::xOffset () const
```




<hr>



### function yInterval [2/2]

_Returns the grid Y interval._ 
```C++
inline double GridModel::yInterval () const
```




<hr>



### function yOffset [2/2]

_Returns the grid Y interval._ 
```C++
inline double GridModel::yOffset () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/gridmodel.h`

