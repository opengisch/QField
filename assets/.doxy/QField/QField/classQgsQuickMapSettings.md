

# Class QgsQuickMapSettings



[**ClassList**](annotated.md) **>** [**QgsQuickMapSettings**](classQgsQuickMapSettings.md)



[More...](#detailed-description)

* `#include <qgsquickmapsettings.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**backgroundColor**](classQgsQuickMapSettings.md#property-backgroundcolor-12)  <br> |
| property double | [**bottomMargin**](classQgsQuickMapSettings.md#property-bottommargin-12)  <br> |
| property QgsPoint | [**center**](classQgsQuickMapSettings.md#property-center)  <br> |
| property QgsCoordinateReferenceSystem | [**destinationCrs**](classQgsQuickMapSettings.md#property-destinationcrs-12)  <br> |
| property QgsRectangle | [**extent**](classQgsQuickMapSettings.md#property-extent-12)  <br> |
| property bool | [**isTemporal**](classQgsQuickMapSettings.md#property-istemporal-12)  <br> |
| property QList&lt; QgsMapLayer \* &gt; | [**layers**](classQgsQuickMapSettings.md#property-layers-12)  <br> |
| property double | [**mapUnitsPerPoint**](classQgsQuickMapSettings.md#property-mapunitsperpoint-12)  <br>_Returns the distance in geographical coordinates that equals to one point unit in the map._  |
| property double | [**outputDpi**](classQgsQuickMapSettings.md#property-outputdpi-12)  <br> |
| property QSize | [**outputSize**](classQgsQuickMapSettings.md#property-outputsize-12)  <br> |
| property QgsProject \* | [**project**](classQgsQuickMapSettings.md#property-project-12)  <br> |
| property double | [**rightMargin**](classQgsQuickMapSettings.md#property-rightmargin-12)  <br> |
| property double | [**rotation**](classQgsQuickMapSettings.md#property-rotation-12)  <br> |
| property double | [**scale**](classQgsQuickMapSettings.md#property-scale-12)  <br> |
| property QDateTime | [**temporalBegin**](classQgsQuickMapSettings.md#property-temporalbegin-12)  <br> |
| property QDateTime | [**temporalEnd**](classQgsQuickMapSettings.md#property-temporalend-12)  <br> |
| property QgsRectangle | [**visibleExtent**](classQgsQuickMapSettings.md#property-visibleextent-12)  <br> |
| property double | [**zRangeLower**](classQgsQuickMapSettings.md#property-zrangelower-12)  <br> |
| property double | [**zRangeUpper**](classQgsQuickMapSettings.md#property-zrangeupper-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**backgroundColorChanged**](classQgsQuickMapSettings.md#signal-backgroundcolorchanged)  <br> |
| signal void | [**bottomMarginChanged**](classQgsQuickMapSettings.md#signal-bottommarginchanged)  <br> |
| signal void | [**destinationCrsChanged**](classQgsQuickMapSettings.md#signal-destinationcrschanged)  <br> |
| signal void | [**extentChanged**](classQgsQuickMapSettings.md#signal-extentchanged)  <br> |
| signal void | [**layersChanged**](classQgsQuickMapSettings.md#signal-layerschanged)  <br> |
| signal void | [**mapUnitsPerPointChanged**](classQgsQuickMapSettings.md#signal-mapunitsperpointchanged)  <br>_Returns the distance in geographical coordinates that equals to one point unit in the map._  |
| signal void | [**outputDpiChanged**](classQgsQuickMapSettings.md#signal-outputdpichanged)  <br> |
| signal void | [**outputSizeChanged**](classQgsQuickMapSettings.md#signal-outputsizechanged)  <br> |
| signal void | [**projectChanged**](classQgsQuickMapSettings.md#signal-projectchanged)  <br> |
| signal void | [**rightMarginChanged**](classQgsQuickMapSettings.md#signal-rightmarginchanged)  <br> |
| signal void | [**rotationChanged**](classQgsQuickMapSettings.md#signal-rotationchanged)  <br> |
| signal void | [**temporalStateChanged**](classQgsQuickMapSettings.md#signal-temporalstatechanged)  <br>_Emitted when temporal properties are modified._  |
| signal void | [**visibleExtentChanged**](classQgsQuickMapSettings.md#signal-visibleextentchanged)  <br> |
| signal void | [**zRangeChanged**](classQgsQuickMapSettings.md#signal-zrangechanged)  <br>_Emitted when the Z range's lower and upper values are modified._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QgsQuickMapSettings**](#function-qgsquickmapsettings) (QObject \* parent=nullptr) <br>_Create new map settings._  |
|  QColor | [**backgroundColor**](#function-backgroundcolor-22) () const<br> |
|  double | [**bottomMargin**](#function-bottommargin-22) () const<br> |
|  Q\_INVOKABLE double | [**computeScaleForExtent**](#function-computescaleforextent) (const QgsRectangle & extent, bool handleMargins=false) <br> |
|  Q\_INVOKABLE QPointF | [**coordinateToScreen**](#function-coordinatetoscreen) (const QgsPoint & point) const<br> |
|  QgsCoordinateReferenceSystem | [**destinationCrs**](#function-destinationcrs-22) () const<br> |
|  qreal | [**devicePixelRatio**](#function-devicepixelratio) () const<br> |
|  QgsRectangle | [**extent**](#function-extent-22) () const<br> |
|  Q\_INVOKABLE QgsPoint | [**getCenter**](#function-getcenter) (bool handleMargins=false) const<br>_Returns the center point of the current map extent._  |
|  bool | [**isTemporal**](#function-istemporal-22) () const<br> |
|  QList&lt; QgsMapLayer \* &gt; | [**layers**](#function-layers-22) () const<br> |
|  QgsMapSettings | [**mapSettings**](#function-mapsettings) () const<br>_Clone map settings._  |
|  double | [**mapUnitsPerPoint**](#function-mapunitsperpoint-22) () const<br>_Returns the distance in geographical coordinates that equals to one point unit in the map._  |
|  double | [**outputDpi**](#function-outputdpi-22) () const<br> |
|  QSize | [**outputSize**](#function-outputsize-22) () const<br> |
|  QgsProject \* | [**project**](#function-project-22) () const<br> |
|  double | [**rightMargin**](#function-rightmargin-22) () const<br> |
|  double | [**rotation**](#function-rotation-22) () const<br> |
|  double | [**scale**](#function-scale-22) () const<br> |
|  Q\_INVOKABLE QgsPoint | [**screenToCoordinate**](#function-screentocoordinate) (const QPointF & point) const<br> |
|  void | [**setBackgroundColor**](#function-setbackgroundcolor) (const QColor & color) <br> |
|  void | [**setBottomMargin**](#function-setbottommargin) (double bottomMargin) <br> |
|  Q\_INVOKABLE void | [**setCenter**](#function-setcenter-12) (const QgsPoint & center, bool handleMargins=false) <br> |
|  Q\_INVOKABLE void | [**setCenter**](#function-setcenter-22) (const QPointF & center, bool handleMargins=false) <br> |
|  Q\_INVOKABLE void | [**setCenterToLayer**](#function-setcentertolayer) (QgsMapLayer \* layer, bool shouldZoom=true) <br>_Move current map extent to have center point defined by_ _layer_ _. Optionally only pan to the layer if__shouldZoom_ _is false._ |
|  void | [**setDestinationCrs**](#function-setdestinationcrs) (const QgsCoordinateReferenceSystem & destinationCrs) <br> |
|  void | [**setDevicePixelRatio**](#function-setdevicepixelratio) (const qreal & devicePixelRatio) <br> |
|  Q\_INVOKABLE void | [**setExtent**](#function-setextent) (const QgsRectangle & extent, bool handleMargins=false) <br> |
|  Q\_INVOKABLE void | [**setExtentFromPoints**](#function-setextentfrompoints) (const QVariantList & points, const double & minimumScale=0, bool handleMargins=false) <br>_Move current map extent to center around the list of_ _points_ _provided._ |
|  void | [**setIsTemporal**](#function-setistemporal) (bool temporal) <br> |
|  void | [**setLayers**](#function-setlayers) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |
|  void | [**setOutputDpi**](#function-setoutputdpi) (double outputDpi) <br> |
|  void | [**setOutputSize**](#function-setoutputsize) (QSize outputSize) <br> |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br> |
|  void | [**setRightMargin**](#function-setrightmargin) (double rightMargin) <br> |
|  void | [**setRotation**](#function-setrotation) (double rotation) <br> |
|  void | [**setTemporalBegin**](#function-settemporalbegin) (const QDateTime & begin) <br> |
|  void | [**setTemporalEnd**](#function-settemporalend) (const QDateTime & end) <br> |
|  void | [**setTransformContext**](#function-settransformcontext) (const QgsCoordinateTransformContext & context) <br> |
|  void | [**setZRangeLower**](#function-setzrangelower) (double lower) <br> |
|  void | [**setZRangeUpper**](#function-setzrangeupper) (double upper) <br> |
|  QDateTime | [**temporalBegin**](#function-temporalbegin-22) () const<br> |
|  QDateTime | [**temporalEnd**](#function-temporalend-22) () const<br> |
|  Q\_INVOKABLE QgsCoordinateTransformContext | [**transformContext**](#function-transformcontext) () const<br> |
|  QgsRectangle | [**visibleExtent**](#function-visibleextent-22) () const<br> |
|  double | [**zRangeLower**](#function-zrangelower-22) () const<br> |
|  double | [**zRangeUpper**](#function-zrangeupper-22) () const<br> |
|   | [**~QgsQuickMapSettings**](#function-qgsquickmapsettings) () = default<br> |




























## Detailed Description


The [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) class encapsulates QgsMapSettings class to offer settings of configuration of map rendering via QML properties.


On top of QgsMapSettings functionality, when QgsProject is attached, it automatically loads its default settings from the project. QgsProject should be attached before it is read.




**Note:**

QML Type: MapSettings




**See also:** QgsQuickMapCanvas 



    
## Public Properties Documentation




### property backgroundColor [1/2]

```C++
QColor QgsQuickMapSettings::backgroundColor;
```



The background color used to render the map


The value is set to the project's bacckground color setting on QgsProject::readProject 


        

<hr>



### property bottomMargin [1/2]

```C++
double QgsQuickMapSettings::bottomMargin;
```



The bottom margin used by the map settings when calculating map extent or center. 


        

<hr>



### property center 

```C++
QgsPoint QgsQuickMapSettings::center;
```



Geographical coordinate representing the center point of the current extent 


        

<hr>



### property destinationCrs [1/2]

```C++
QgsCoordinateReferenceSystem QgsQuickMapSettings::destinationCrs;
```



CRS of destination coordinate reference system.


Automatically loaded from project on QgsProject::readProject 


        

<hr>



### property extent [1/2]

```C++
QgsRectangle QgsQuickMapSettings::extent;
```



Geographical coordinates of the rectangle that should be rendered. The actual visible extent used for rendering could be slightly different since the given extent may be expanded in order to fit the aspect ratio of output size. Use [**QgsQuickMapSettings::visibleExtent**](classQgsQuickMapSettings.md#property-visibleextent-12) to get the resulting extent.


Automatically loaded from project on QgsProject::readProject 


        

<hr>



### property isTemporal [1/2]

```C++
bool QgsQuickMapSettings::isTemporal;
```



Returns TRUE if a temporal filtering is enabled 


        

<hr>



### property layers [1/2]

```C++
QList<QgsMapLayer *> QgsQuickMapSettings::layers;
```



Set list of layers for map rendering. The layers must be registered in QgsProject. The layers are stored in the reverse order of how they are rendered (layer with index 0 will be on top)




**Note:**

Any non-spatial layers will be automatically stripped from the list (since they cannot be rendered!).


Not loaded automatically from the associated project 


        

<hr>



### property mapUnitsPerPoint [1/2]

_Returns the distance in geographical coordinates that equals to one point unit in the map._ 
```C++
double QgsQuickMapSettings::mapUnitsPerPoint;
```




<hr>



### property outputDpi [1/2]

```C++
double QgsQuickMapSettings::outputDpi;
```



Output DPI used for conversion between real world units (e.g. mm) and pixels


Automatically loaded from project on QgsProject::readProject 


        

<hr>



### property outputSize [1/2]

```C++
QSize QgsQuickMapSettings::outputSize;
```



The size of the resulting map image


Automatically loaded from project on QgsProject::readProject 


        

<hr>



### property project [1/2]

```C++
QgsProject * QgsQuickMapSettings::project;
```



A project property should be used as a primary source of project all other components in the application. QgsProject should be attached to [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) before it is read (QgsProject::read)


When project is read, map settings (CRS, extent, ...) are automatically set from its DOM. 


        

<hr>



### property rightMargin [1/2]

```C++
double QgsQuickMapSettings::rightMargin;
```



The right margin used by the map settings when calculating map extent or center. 


        

<hr>



### property rotation [1/2]

```C++
double QgsQuickMapSettings::rotation;
```



The rotation of the resulting map image, in degrees clockwise. 


        

<hr>



### property scale [1/2]

```C++
double QgsQuickMapSettings::scale;
```




<hr>



### property temporalBegin [1/2]

```C++
QDateTime QgsQuickMapSettings::temporalBegin;
```



The temporal range's begin (i.e. lower) value 


        

<hr>



### property temporalEnd [1/2]

```C++
QDateTime QgsQuickMapSettings::temporalEnd;
```



The temporal range's end (i.e. higher) value 


        

<hr>



### property visibleExtent [1/2]

```C++
QgsRectangle QgsQuickMapSettings::visibleExtent;
```




<hr>



### property zRangeLower [1/2]

```C++
double QgsQuickMapSettings::zRangeLower;
```



The Z range's lower value; if both the lower and upper value are finite, Z range filtering will occur. 


        

<hr>



### property zRangeUpper [1/2]

```C++
double QgsQuickMapSettings::zRangeUpper;
```



The Z range's upper value; if both the lower and upper value are finite, Z range filtering will occur. 


        

<hr>
## Public Signals Documentation




### signal backgroundColorChanged 

```C++
void QgsQuickMapSettings::backgroundColorChanged;
```



The background color used to render the map


The value is set to the project's bacckground color setting on QgsProject::readProject   


        

<hr>



### signal bottomMarginChanged 

```C++
void QgsQuickMapSettings::bottomMarginChanged;
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### signal destinationCrsChanged 

```C++
void QgsQuickMapSettings::destinationCrsChanged;
```



CRS of destination coordinate reference system.


Automatically loaded from project on QgsProject::readProject   


        

<hr>



### signal extentChanged 

```C++
void QgsQuickMapSettings::extentChanged;
```



Geographical coordinates of the rectangle that should be rendered. The actual visible extent used for rendering could be slightly different since the given extent may be expanded in order to fit the aspect ratio of output size. Use [**QgsQuickMapSettings::visibleExtent**](classQgsQuickMapSettings.md#property-visibleextent-12) to get the resulting extent.


Automatically loaded from project on QgsProject::readProject   


        

<hr>



### signal layersChanged 

```C++
void QgsQuickMapSettings::layersChanged;
```



Set list of layers for map rendering. The layers must be registered in QgsProject. The layers are stored in the reverse order of how they are rendered (layer with index 0 will be on top)




**Note:**

Any non-spatial layers will be automatically stripped from the list (since they cannot be rendered!).


Not loaded automatically from the associated project   


        

<hr>



### signal mapUnitsPerPointChanged 

_Returns the distance in geographical coordinates that equals to one point unit in the map._ 
```C++
void QgsQuickMapSettings::mapUnitsPerPointChanged;
```




<hr>



### signal outputDpiChanged 

```C++
void QgsQuickMapSettings::outputDpiChanged;
```



Output DPI used for conversion between real world units (e.g. mm) and pixels


Automatically loaded from project on QgsProject::readProject   


        

<hr>



### signal outputSizeChanged 

```C++
void QgsQuickMapSettings::outputSizeChanged;
```



The size of the resulting map image


Automatically loaded from project on QgsProject::readProject   


        

<hr>



### signal projectChanged 

```C++
void QgsQuickMapSettings::projectChanged;
```



A project property should be used as a primary source of project all other components in the application. QgsProject should be attached to [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) before it is read (QgsProject::read)


When project is read, map settings (CRS, extent, ...) are automatically set from its DOM.   


        

<hr>



### signal rightMarginChanged 

```C++
void QgsQuickMapSettings::rightMarginChanged;
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### signal rotationChanged 

```C++
void QgsQuickMapSettings::rotationChanged;
```



The rotation of the resulting map image, in degrees clockwise.   


        

<hr>



### signal temporalStateChanged 

_Emitted when temporal properties are modified._ 
```C++
void QgsQuickMapSettings::temporalStateChanged;
```




<hr>



### signal visibleExtentChanged 

```C++
void QgsQuickMapSettings::visibleExtentChanged;
```




<hr>



### signal zRangeChanged 

_Emitted when the Z range's lower and upper values are modified._ 
```C++
void QgsQuickMapSettings::zRangeChanged;
```




<hr>
## Public Functions Documentation




### function QgsQuickMapSettings 

_Create new map settings._ 
```C++
explicit QgsQuickMapSettings::QgsQuickMapSettings (
    QObject * parent=nullptr
) 
```




<hr>



### function backgroundColor [2/2]

```C++
QColor QgsQuickMapSettings::backgroundColor () const
```



The background color used to render the map


The value is set to the project's bacckground color setting on QgsProject::readProject   


        

<hr>



### function bottomMargin [2/2]

```C++
double QgsQuickMapSettings::bottomMargin () const
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### function computeScaleForExtent 

```C++
Q_INVOKABLE double QgsQuickMapSettings::computeScaleForExtent (
    const QgsRectangle & extent,
    bool handleMargins=false
) 
```



Compute the scale that corresponds to the specified _extent_.




**Parameters:**


* `point` A coordinate in pixel / screen coordinates



**Returns:**

A scale value 





        

<hr>



### function coordinateToScreen 

```C++
Q_INVOKABLE QPointF QgsQuickMapSettings::coordinateToScreen (
    const QgsPoint & point
) const
```



Convert a map coordinate to screen pixel coordinates




**Parameters:**


* `point` A coordinate in map coordinates



**Returns:**

A coordinate in pixel / screen space 





        

<hr>



### function destinationCrs [2/2]

```C++
QgsCoordinateReferenceSystem QgsQuickMapSettings::destinationCrs () const
```




<hr>



### function devicePixelRatio 

```C++
qreal QgsQuickMapSettings::devicePixelRatio () const
```



Returns the ratio between physical pixels and device-independent pixels. This value is dependent on the screen the window is on, and may change when the window is moved. Common values are 1.0 on normal displays and 2.0 on Apple "retina" displays. 


        

<hr>



### function extent [2/2]

```C++
QgsRectangle QgsQuickMapSettings::extent () const
```




<hr>



### function getCenter 

_Returns the center point of the current map extent._ 
```C++
Q_INVOKABLE QgsPoint QgsQuickMapSettings::getCenter (
    bool handleMargins=false
) const
```




<hr>



### function isTemporal [2/2]

```C++
bool QgsQuickMapSettings::isTemporal () const
```



Returns TRUE if a temporal filtering is enabled   


        

<hr>



### function layers [2/2]

```C++
QList< QgsMapLayer * > QgsQuickMapSettings::layers () const
```



Returns the list of layers which will be rendered in the map.


The layers are stored in the reverse order of how they are rendered (layer with index 0 will be on top)




**See also:** [**setLayers()**](classQgsQuickMapSettings.md#function-setlayers) 



        

<hr>



### function mapSettings 

_Clone map settings._ 
```C++
QgsMapSettings QgsQuickMapSettings::mapSettings () const
```




<hr>



### function mapUnitsPerPoint [2/2]

_Returns the distance in geographical coordinates that equals to one point unit in the map._ 
```C++
double QgsQuickMapSettings::mapUnitsPerPoint () const
```




<hr>



### function outputDpi [2/2]

```C++
double QgsQuickMapSettings::outputDpi () const
```




<hr>



### function outputSize [2/2]

```C++
QSize QgsQuickMapSettings::outputSize () const
```




<hr>



### function project [2/2]

```C++
QgsProject * QgsQuickMapSettings::project () const
```



A project property should be used as a primary source of project all other components in the application. QgsProject should be attached to [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) before it is read (QgsProject::read)


When project is read, map settings (CRS, extent, ...) are automatically set from its DOM.   


        

<hr>



### function rightMargin [2/2]

```C++
double QgsQuickMapSettings::rightMargin () const
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### function rotation [2/2]

```C++
double QgsQuickMapSettings::rotation () const
```



The rotation of the resulting map image, in degrees clockwise.   


        

<hr>



### function scale [2/2]

```C++
double QgsQuickMapSettings::scale () const
```




<hr>



### function screenToCoordinate 

```C++
Q_INVOKABLE QgsPoint QgsQuickMapSettings::screenToCoordinate (
    const QPointF & point
) const
```



Convert a screen coordinate to a map coordinate




**Parameters:**


* `point` A coordinate in pixel / screen coordinates



**Returns:**

A coordinate in map coordinates 





        

<hr>



### function setBackgroundColor 

```C++
void QgsQuickMapSettings::setBackgroundColor (
    const QColor & color
) 
```



The background color used to render the map


The value is set to the project's bacckground color setting on QgsProject::readProject   


        

<hr>



### function setBottomMargin 

```C++
void QgsQuickMapSettings::setBottomMargin (
    double bottomMargin
) 
```



The bottom margin used by the map settings when calculating map extent or center.   


        

<hr>



### function setCenter [1/2]

```C++
Q_INVOKABLE void QgsQuickMapSettings::setCenter (
    const QgsPoint & center,
    bool handleMargins=false
) 
```



Move current map extent to have center point defined by center 

**Parameters:**


* `center` a QgsPoint center point 
* `handleMargins` set to TRUE for margins to be considered when recentering 




        

<hr>



### function setCenter [2/2]

```C++
Q_INVOKABLE void QgsQuickMapSettings::setCenter (
    const QPointF & center,
    bool handleMargins=false
) 
```



Move current map extent to have center point defined by center 

**Parameters:**


* `center` a QPointF center point 
* `handleMargins` set to TRUE for margins to be considered when recentering 




        

<hr>



### function setCenterToLayer 

_Move current map extent to have center point defined by_ _layer_ _. Optionally only pan to the layer if__shouldZoom_ _is false._
```C++
Q_INVOKABLE void QgsQuickMapSettings::setCenterToLayer (
    QgsMapLayer * layer,
    bool shouldZoom=true
) 
```




<hr>



### function setDestinationCrs 

```C++
void QgsQuickMapSettings::setDestinationCrs (
    const QgsCoordinateReferenceSystem & destinationCrs
) 
```




<hr>



### function setDevicePixelRatio 

```C++
void QgsQuickMapSettings::setDevicePixelRatio (
    const qreal & devicePixelRatio
) 
```



Sets the ratio between physical pixels and device-independent pixels. This value is dependent on the screen the window is on, and may change when the window is moved. Common values are 1.0 on normal displays and 2.0 on Apple "retina" displays. 


        

<hr>



### function setExtent 

```C++
Q_INVOKABLE void QgsQuickMapSettings::setExtent (
    const QgsRectangle & extent,
    bool handleMargins=false
) 
```




<hr>



### function setExtentFromPoints 

_Move current map extent to center around the list of_ _points_ _provided._
```C++
Q_INVOKABLE void QgsQuickMapSettings::setExtentFromPoints (
    const QVariantList & points,
    const double & minimumScale=0,
    bool handleMargins=false
) 
```




<hr>



### function setIsTemporal 

```C++
void QgsQuickMapSettings::setIsTemporal (
    bool temporal
) 
```



Returns TRUE if a temporal filtering is enabled   


        

<hr>



### function setLayers 

```C++
void QgsQuickMapSettings::setLayers (
    const QList< QgsMapLayer * > & layers
) 
```



Sets the list of _layers_ to render in the map.


The layers are stored in the reverse order of how they are rendered (layer with index 0 will be on top)




**Note:**

Any non-spatial layers will be automatically stripped from the list (since they cannot be rendered!).




**See also:** [**layers()**](classQgsQuickMapSettings.md#property-layers-12) 



        

<hr>



### function setOutputDpi 

```C++
void QgsQuickMapSettings::setOutputDpi (
    double outputDpi
) 
```




<hr>



### function setOutputSize 

```C++
void QgsQuickMapSettings::setOutputSize (
    QSize outputSize
) 
```




<hr>



### function setProject 

```C++
void QgsQuickMapSettings::setProject (
    QgsProject * project
) 
```



A project property should be used as a primary source of project all other components in the application. QgsProject should be attached to [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) before it is read (QgsProject::read)


When project is read, map settings (CRS, extent, ...) are automatically set from its DOM.   


        

<hr>



### function setRightMargin 

```C++
void QgsQuickMapSettings::setRightMargin (
    double rightMargin
) 
```



The right margin used by the map settings when calculating map extent or center.   


        

<hr>



### function setRotation 

```C++
void QgsQuickMapSettings::setRotation (
    double rotation
) 
```



The rotation of the resulting map image, in degrees clockwise.   


        

<hr>



### function setTemporalBegin 

```C++
void QgsQuickMapSettings::setTemporalBegin (
    const QDateTime & begin
) 
```



The temporal range's begin (i.e. lower) value   


        

<hr>



### function setTemporalEnd 

```C++
void QgsQuickMapSettings::setTemporalEnd (
    const QDateTime & end
) 
```



The temporal range's end (i.e. higher) value   


        

<hr>



### function setTransformContext 

```C++
void QgsQuickMapSettings::setTransformContext (
    const QgsCoordinateTransformContext & context
) 
```




<hr>



### function setZRangeLower 

```C++
void QgsQuickMapSettings::setZRangeLower (
    double lower
) 
```



The Z range's lower value; if both the lower and upper value are finite, Z range filtering will occur.   


        

<hr>



### function setZRangeUpper 

```C++
void QgsQuickMapSettings::setZRangeUpper (
    double upper
) 
```



The Z range's upper value; if both the lower and upper value are finite, Z range filtering will occur.   


        

<hr>



### function temporalBegin [2/2]

```C++
QDateTime QgsQuickMapSettings::temporalBegin () const
```



The temporal range's begin (i.e. lower) value   


        

<hr>



### function temporalEnd [2/2]

```C++
QDateTime QgsQuickMapSettings::temporalEnd () const
```



The temporal range's end (i.e. higher) value   


        

<hr>



### function transformContext 

```C++
Q_INVOKABLE QgsCoordinateTransformContext QgsQuickMapSettings::transformContext () const
```




<hr>



### function visibleExtent [2/2]

```C++
QgsRectangle QgsQuickMapSettings::visibleExtent () const
```




<hr>



### function zRangeLower [2/2]

```C++
double QgsQuickMapSettings::zRangeLower () const
```



The Z range's lower value; if both the lower and upper value are finite, Z range filtering will occur.   


        

<hr>



### function zRangeUpper [2/2]

```C++
double QgsQuickMapSettings::zRangeUpper () const
```



The Z range's upper value; if both the lower and upper value are finite, Z range filtering will occur.   


        

<hr>



### function ~QgsQuickMapSettings 

```C++
QgsQuickMapSettings::~QgsQuickMapSettings () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickmapsettings.h`

