

# Class QfGridModel



[**ClassList**](annotated.md) **>** [**QfGridModel**](classQfGridModel.md)



[More...](#detailed-description)

* `#include <qfgridmodel.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**annotationColor**](classQfGridModel.md#property-annotationcolor-12)  <br> |
| property bool | [**annotationHasOutline**](classQfGridModel.md#property-annotationhasoutline-12)  <br> |
| property QColor | [**annotationOutlineColor**](classQfGridModel.md#property-annotationoutlinecolor-12)  <br> |
| property int | [**annotationPrecision**](classQfGridModel.md#property-annotationprecision-12)  <br> |
| property QList&lt; [**QfGridAnnotation**](classQfGridAnnotation.md) &gt; | [**annotations**](classQfGridModel.md#property-annotations-12)  <br> |
| property bool | [**autoColor**](classQfGridModel.md#property-autocolor-12)  <br> |
| property bool | [**enabled**](classQfGridModel.md#property-enabled-12)  <br> |
| property bool | [**indeterminate**](classQfGridModel.md#property-indeterminate-12)  <br> |
| property QColor | [**majorLineColor**](classQfGridModel.md#property-majorlinecolor-12)  <br> |
| property QString | [**majorLinesPath**](classQfGridModel.md#property-majorlinespath-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfGridModel.md#property-mapsettings-12)  <br> |
| property QColor | [**markerColor**](classQfGridModel.md#property-markercolor-12)  <br> |
| property QString | [**markersPath**](classQfGridModel.md#property-markerspath-12)  <br> |
| property QColor | [**minorLineColor**](classQfGridModel.md#property-minorlinecolor-12)  <br> |
| property QString | [**minorLinesPath**](classQfGridModel.md#property-minorlinespath-12)  <br> |
| property bool | [**prepareAnnotations**](classQfGridModel.md#property-prepareannotations-12)  <br> |
| property bool | [**prepareLines**](classQfGridModel.md#property-preparelines-12)  <br> |
| property bool | [**prepareMarkers**](classQfGridModel.md#property-preparemarkers-12)  <br> |
| property double | [**xInterval**](classQfGridModel.md#property-xinterval-12)  <br> |
| property double | [**xOffset**](classQfGridModel.md#property-xoffset-12)  <br> |
| property double | [**yInterval**](classQfGridModel.md#property-yinterval-12)  <br> |
| property double | [**yOffset**](classQfGridModel.md#property-yoffset-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**annotationColorChanged**](classQfGridModel.md#signal-annotationcolorchanged)  <br>_Emitted when the grid annotation text color has changed._  |
| signal void | [**annotationHasOutlineChanged**](classQfGridModel.md#signal-annotationhasoutlinechanged)  <br>_Emitted when the grid annotation text outline visibility has changed._  |
| signal void | [**annotationOutlineColorChanged**](classQfGridModel.md#signal-annotationoutlinecolorchanged)  <br>_Emitted when the grid annotation text outline color has changed._  |
| signal void | [**annotationPrecisionChanged**](classQfGridModel.md#signal-annotationprecisionchanged)  <br>_Emitted when the grid annotation text decimal precision has changed._  |
| signal void | [**annotationsChanged**](classQfGridModel.md#signal-annotationschanged)  <br>_Emitted when the grid annotations have changed._  |
| signal void | [**autoColorChanged**](classQfGridModel.md#signal-autocolorchanged)  <br>_Emitted when the grid line and marker colors are automatically assigned._  |
| signal void | [**enabledChanged**](classQfGridModel.md#signal-enabledchanged)  <br>_Emitted when the grid enabled setting has changed._  |
| signal void | [**indeterminateChanged**](classQfGridModel.md#signal-indeterminatechanged)  <br>_Emitted when the grid indeterminate setting has changed._  |
| signal void | [**majorLineColorChanged**](classQfGridModel.md#signal-majorlinecolorchanged)  <br>_Emitted when the grid major line color has changed._  |
| signal void | [**majorLinesChanged**](classQfGridModel.md#signal-majorlineschanged)  <br>_Emitted when the grid major lines have changed._  |
| signal void | [**mapSettingsChanged**](classQfGridModel.md#signal-mapsettingschanged)  <br>_Emitted when the map settings object has changed._  |
| signal void | [**markerColorChanged**](classQfGridModel.md#signal-markercolorchanged)  <br>_Emitted when the grid marker color has changed._  |
| signal void | [**markersChanged**](classQfGridModel.md#signal-markerschanged)  <br>_Emitted when the grid markers have changed._  |
| signal void | [**minorLineColorChanged**](classQfGridModel.md#signal-minorlinecolorchanged)  <br>_Emitted when the grid minor line color has changed._  |
| signal void | [**minorLinesChanged**](classQfGridModel.md#signal-minorlineschanged)  <br>_Emitted when the grid minor lines have changed._  |
| signal void | [**prepareAnnotationsChanged**](classQfGridModel.md#signal-prepareannotationschanged)  <br>_Emitted when grid annotations preparation setting has changed._  |
| signal void | [**prepareLinesChanged**](classQfGridModel.md#signal-preparelineschanged)  <br>_Emitted when grid lines preparation setting has changed._  |
| signal void | [**prepareMarkersChanged**](classQfGridModel.md#signal-preparemarkerschanged)  <br>_Emitted when grid markers preparation setting has changed._  |
| signal void | [**xIntervalChanged**](classQfGridModel.md#signal-xintervalchanged)  <br>_Emitted when the grid X interval has changed._  |
| signal void | [**xOffsetChanged**](classQfGridModel.md#signal-xoffsetchanged)  <br>_Emitted when the grid X offset has changed._  |
| signal void | [**yIntervalChanged**](classQfGridModel.md#signal-yintervalchanged)  <br>_Emitted when the grid Y interval has changed._  |
| signal void | [**yOffsetChanged**](classQfGridModel.md#signal-yoffsetchanged)  <br>_Emitted when the grid Y offset has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGridModel**](#function-qfgridmodel) (QObject \* parent=nullptr) <br>_Default constructor._  |
|  QColor | [**annotationColor**](#function-annotationcolor-22) () const<br>_Returns the grid annotation text color._  |
|  bool | [**annotationHasOutline**](#function-annotationhasoutline-22) () const<br>_Returns whether annotation text have an outline color._  |
|  QColor | [**annotationOutlineColor**](#function-annotationoutlinecolor-22) () const<br>_Returns the grid annotation text outline color._  |
|  int | [**annotationPrecision**](#function-annotationprecision-22) () const<br>_Returns the annotation text decimal precision._  |
|  QList&lt; [**QfGridAnnotation**](classQfGridAnnotation.md) &gt; | [**annotations**](#function-annotations-22) () const<br>_Returns the grid annotations._  |
|  bool | [**autoColor**](#function-autocolor-22) () const<br> |
|  bool | [**enabled**](#function-enabled-22) () const<br>_Returns TRUE when grid elements will be prepared._  |
|  bool | [**indeterminate**](#function-indeterminate-22) () const<br> |
|  QColor | [**majorLineColor**](#function-majorlinecolor-22) () const<br>_Returns the grid major line color._  |
|  QString | [**majorLinesPath**](#function-majorlinespath-22) () const<br>_Returns the grid major lines._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings object._  |
|  QColor | [**markerColor**](#function-markercolor-22) () const<br>_Returns the grid marker color._  |
|  QString | [**markersPath**](#function-markerspath-22) () const<br>_Returns the grid markers._  |
|  QColor | [**minorLineColor**](#function-minorlinecolor-22) () const<br>_Returns the grid minor line color._  |
|  QString | [**minorLinesPath**](#function-minorlinespath-22) () const<br>_Returns the grid minor lines._  |
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
QColor QfGridModel::annotationColor;
```




<hr>



### property annotationHasOutline [1/2]

```C++
bool QfGridModel::annotationHasOutline;
```




<hr>



### property annotationOutlineColor [1/2]

```C++
QColor QfGridModel::annotationOutlineColor;
```




<hr>



### property annotationPrecision [1/2]

```C++
int QfGridModel::annotationPrecision;
```




<hr>



### property annotations [1/2]

```C++
QList<QfGridAnnotation> QfGridModel::annotations;
```




<hr>



### property autoColor [1/2]

```C++
bool QfGridModel::autoColor;
```




<hr>



### property enabled [1/2]

```C++
bool QfGridModel::enabled;
```




<hr>



### property indeterminate [1/2]

```C++
bool QfGridModel::indeterminate;
```




<hr>



### property majorLineColor [1/2]

```C++
QColor QfGridModel::majorLineColor;
```




<hr>



### property majorLinesPath [1/2]

```C++
QString QfGridModel::majorLinesPath;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfGridModel::mapSettings;
```




<hr>



### property markerColor [1/2]

```C++
QColor QfGridModel::markerColor;
```




<hr>



### property markersPath [1/2]

```C++
QString QfGridModel::markersPath;
```




<hr>



### property minorLineColor [1/2]

```C++
QColor QfGridModel::minorLineColor;
```




<hr>



### property minorLinesPath [1/2]

```C++
QString QfGridModel::minorLinesPath;
```




<hr>



### property prepareAnnotations [1/2]

```C++
bool QfGridModel::prepareAnnotations;
```




<hr>



### property prepareLines [1/2]

```C++
bool QfGridModel::prepareLines;
```




<hr>



### property prepareMarkers [1/2]

```C++
bool QfGridModel::prepareMarkers;
```




<hr>



### property xInterval [1/2]

```C++
double QfGridModel::xInterval;
```




<hr>



### property xOffset [1/2]

```C++
double QfGridModel::xOffset;
```




<hr>



### property yInterval [1/2]

```C++
double QfGridModel::yInterval;
```




<hr>



### property yOffset [1/2]

```C++
double QfGridModel::yOffset;
```




<hr>
## Public Signals Documentation




### signal annotationColorChanged 

_Emitted when the grid annotation text color has changed._ 
```C++
void QfGridModel::annotationColorChanged;
```




<hr>



### signal annotationHasOutlineChanged 

_Emitted when the grid annotation text outline visibility has changed._ 
```C++
void QfGridModel::annotationHasOutlineChanged;
```




<hr>



### signal annotationOutlineColorChanged 

_Emitted when the grid annotation text outline color has changed._ 
```C++
void QfGridModel::annotationOutlineColorChanged;
```




<hr>



### signal annotationPrecisionChanged 

_Emitted when the grid annotation text decimal precision has changed._ 
```C++
void QfGridModel::annotationPrecisionChanged;
```




<hr>



### signal annotationsChanged 

_Emitted when the grid annotations have changed._ 
```C++
void QfGridModel::annotationsChanged;
```




<hr>



### signal autoColorChanged 

_Emitted when the grid line and marker colors are automatically assigned._ 
```C++
void QfGridModel::autoColorChanged;
```




<hr>



### signal enabledChanged 

_Emitted when the grid enabled setting has changed._ 
```C++
void QfGridModel::enabledChanged;
```




<hr>



### signal indeterminateChanged 

_Emitted when the grid indeterminate setting has changed._ 
```C++
void QfGridModel::indeterminateChanged;
```




<hr>



### signal majorLineColorChanged 

_Emitted when the grid major line color has changed._ 
```C++
void QfGridModel::majorLineColorChanged;
```




<hr>



### signal majorLinesChanged 

_Emitted when the grid major lines have changed._ 
```C++
void QfGridModel::majorLinesChanged;
```




<hr>



### signal mapSettingsChanged 

_Emitted when the map settings object has changed._ 
```C++
void QfGridModel::mapSettingsChanged;
```




<hr>



### signal markerColorChanged 

_Emitted when the grid marker color has changed._ 
```C++
void QfGridModel::markerColorChanged;
```




<hr>



### signal markersChanged 

_Emitted when the grid markers have changed._ 
```C++
void QfGridModel::markersChanged;
```




<hr>



### signal minorLineColorChanged 

_Emitted when the grid minor line color has changed._ 
```C++
void QfGridModel::minorLineColorChanged;
```




<hr>



### signal minorLinesChanged 

_Emitted when the grid minor lines have changed._ 
```C++
void QfGridModel::minorLinesChanged;
```




<hr>



### signal prepareAnnotationsChanged 

_Emitted when grid annotations preparation setting has changed._ 
```C++
void QfGridModel::prepareAnnotationsChanged;
```




<hr>



### signal prepareLinesChanged 

_Emitted when grid lines preparation setting has changed._ 
```C++
void QfGridModel::prepareLinesChanged;
```




<hr>



### signal prepareMarkersChanged 

_Emitted when grid markers preparation setting has changed._ 
```C++
void QfGridModel::prepareMarkersChanged;
```




<hr>



### signal xIntervalChanged 

_Emitted when the grid X interval has changed._ 
```C++
void QfGridModel::xIntervalChanged;
```




<hr>



### signal xOffsetChanged 

_Emitted when the grid X offset has changed._ 
```C++
void QfGridModel::xOffsetChanged;
```




<hr>



### signal yIntervalChanged 

_Emitted when the grid Y interval has changed._ 
```C++
void QfGridModel::yIntervalChanged;
```




<hr>



### signal yOffsetChanged 

_Emitted when the grid Y offset has changed._ 
```C++
void QfGridModel::yOffsetChanged;
```




<hr>
## Public Functions Documentation




### function QfGridModel 

_Default constructor._ 
```C++
explicit QfGridModel::QfGridModel (
    QObject * parent=nullptr
) 
```




<hr>



### function annotationColor [2/2]

_Returns the grid annotation text color._ 
```C++
inline QColor QfGridModel::annotationColor () const
```




<hr>



### function annotationHasOutline [2/2]

_Returns whether annotation text have an outline color._ 
```C++
inline bool QfGridModel::annotationHasOutline () const
```




<hr>



### function annotationOutlineColor [2/2]

_Returns the grid annotation text outline color._ 
```C++
inline QColor QfGridModel::annotationOutlineColor () const
```




<hr>



### function annotationPrecision [2/2]

_Returns the annotation text decimal precision._ 
```C++
inline int QfGridModel::annotationPrecision () const
```




<hr>



### function annotations [2/2]

_Returns the grid annotations._ 
```C++
inline QList< QfGridAnnotation > QfGridModel::annotations () const
```




<hr>



### function autoColor [2/2]

```C++
inline bool QfGridModel::autoColor () const
```



Returns whether grid line and marker colors will be automatically assigned to colors derived from the map canvas background color. 


        

<hr>



### function enabled [2/2]

_Returns TRUE when grid elements will be prepared._ 
```C++
inline bool QfGridModel::enabled () const
```




<hr>



### function indeterminate [2/2]

```C++
inline bool QfGridModel::indeterminate () const
```



Returns TRUE when grid will adopt an indeterminate behavior and ignore the interval and offset values. 


        

<hr>



### function majorLineColor [2/2]

_Returns the grid major line color._ 
```C++
inline QColor QfGridModel::majorLineColor () const
```




<hr>



### function majorLinesPath [2/2]

_Returns the grid major lines._ 
```C++
inline QString QfGridModel::majorLinesPath () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings object._ 
```C++
inline QgsQuickMapSettings * QfGridModel::mapSettings () const
```




<hr>



### function markerColor [2/2]

_Returns the grid marker color._ 
```C++
inline QColor QfGridModel::markerColor () const
```




<hr>



### function markersPath [2/2]

_Returns the grid markers._ 
```C++
inline QString QfGridModel::markersPath () const
```




<hr>



### function minorLineColor [2/2]

_Returns the grid minor line color._ 
```C++
inline QColor QfGridModel::minorLineColor () const
```




<hr>



### function minorLinesPath [2/2]

_Returns the grid minor lines._ 
```C++
inline QString QfGridModel::minorLinesPath () const
```




<hr>



### function prepareAnnotations [2/2]

_Returns whether grid annotations will be prepared._ 
```C++
inline bool QfGridModel::prepareAnnotations () const
```




<hr>



### function prepareLines [2/2]

_Returns whether grid lines will be prepared._ 
```C++
inline bool QfGridModel::prepareLines () const
```




<hr>



### function prepareMarkers [2/2]

_Returns whether grid markers will be prepared._ 
```C++
inline bool QfGridModel::prepareMarkers () const
```




<hr>



### function setAnnotationColor 

_Sets the grid annotation text color._ 
```C++
void QfGridModel::setAnnotationColor (
    const QColor & color
) 
```




<hr>



### function setAnnotationHasOutline 

_Sets whether annotation text have an outline color._ 
```C++
void QfGridModel::setAnnotationHasOutline (
    bool hasOutline
) 
```




<hr>



### function setAnnotationOutlineColor 

_Sets the grid annotation text outline color._ 
```C++
void QfGridModel::setAnnotationOutlineColor (
    const QColor & color
) 
```




<hr>



### function setAnnotationPrecision 

_Sets the annotation text decimal precision._ 
```C++
void QfGridModel::setAnnotationPrecision (
    int precision
) 
```




<hr>



### function setAutoColor 

```C++
void QfGridModel::setAutoColor (
    bool autoColor
) 
```



Set whether grid line and marker colors will be automatically assigned to colors derived from the map canvas background color. 


        

<hr>



### function setEnabled 

_Sets whether grid elements will be prepared._ 
```C++
void QfGridModel::setEnabled (
    bool enabled
) 
```




<hr>



### function setIndeterminate 

```C++
void QfGridModel::setIndeterminate (
    bool indeterminate
) 
```



Sets whether the grid will adopt an indeterminate behavior and ignore the interval and offset values. 


        

<hr>



### function setMajorLineColor 

_Sets the grid major line color._ 
```C++
void QfGridModel::setMajorLineColor (
    const QColor & color
) 
```




<hr>



### function setMapSettings 

_Sets the map settings object._ 
```C++
void QfGridModel::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setMarkerColor 

_Sets the grid marker color._ 
```C++
void QfGridModel::setMarkerColor (
    const QColor & color
) 
```




<hr>



### function setMinorLineColor 

_Sets the grid minor line color._ 
```C++
void QfGridModel::setMinorLineColor (
    const QColor & color
) 
```




<hr>



### function setPrepareAnnotations 

_Sets whether grid annotations will be prepared._ 
```C++
void QfGridModel::setPrepareAnnotations (
    bool prepare
) 
```




<hr>



### function setPrepareLines 

_Sets whether grid lines will be prepared._ 
```C++
void QfGridModel::setPrepareLines (
    bool prepare
) 
```




<hr>



### function setPrepareMarkers 

_Sets whether grid markers will be prepared._ 
```C++
void QfGridModel::setPrepareMarkers (
    bool prepare
) 
```




<hr>



### function setXInterval 

_Sets the grid X interval._ 
```C++
void QfGridModel::setXInterval (
    double interval
) 
```




<hr>



### function setXOffset 

_Sets the grid X interval._ 
```C++
void QfGridModel::setXOffset (
    double offset
) 
```




<hr>



### function setYInterval 

_Sets the grid Y interval._ 
```C++
void QfGridModel::setYInterval (
    double interval
) 
```




<hr>



### function setYOffset 

_Sets the grid Y interval._ 
```C++
void QfGridModel::setYOffset (
    double offset
) 
```




<hr>



### function xInterval [2/2]

_Returns the grid X interval._ 
```C++
inline double QfGridModel::xInterval () const
```




<hr>



### function xOffset [2/2]

_Returns the grid X interval._ 
```C++
inline double QfGridModel::xOffset () const
```




<hr>



### function yInterval [2/2]

_Returns the grid Y interval._ 
```C++
inline double QfGridModel::yInterval () const
```




<hr>



### function yOffset [2/2]

_Returns the grid Y interval._ 
```C++
inline double QfGridModel::yOffset () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfgridmodel.h`

