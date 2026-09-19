

# Class QfNavigation



[**ClassList**](annotated.md) **>** [**QfNavigation**](classQfNavigation.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**bearing**](classQfNavigation.md#property-bearing-12)  <br> |
| property QgsPoint | [**destination**](classQfNavigation.md#property-destination-12)  <br> |
| property int | [**destinationFeatureCurrentVertex**](classQfNavigation.md#property-destinationfeaturecurrentvertex-12)  <br> |
| property int | [**destinationFeatureVertexCount**](classQfNavigation.md#property-destinationfeaturevertexcount-12)  <br> |
| property QString | [**destinationName**](classQfNavigation.md#property-destinationname-12)  <br> |
| property double | [**distance**](classQfNavigation.md#property-distance-12)  <br> |
| property Qgis::DistanceUnit | [**distanceUnits**](classQfNavigation.md#property-distanceunits-12)  <br> |
| property bool | [**isActive**](classQfNavigation.md#property-isactive-12)  <br> |
| property modelChanged QgsPoint | [**location**](classQfNavigation.md#property-location-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfNavigation.md#property-mapsettings-12)  <br> |
| property [**QfNavigationModel**](classQfNavigationModel.md) \* | [**model**](classQfNavigation.md#property-model-12)  <br> |
| property QgsGeometry | [**path**](classQfNavigation.md#property-path-12)  <br> |
| property bool | [**proximityAlarm**](classQfNavigation.md#property-proximityalarm-12)  <br> |
| property double | [**proximityAlarmThreshold**](classQfNavigation.md#property-proximityalarmthreshold-12)  <br> |
| property double | [**verticalDistance**](classQfNavigation.md#property-verticaldistance-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**destinationChanged**](classQfNavigation.md#signal-destinationchanged)  <br> |
| signal void | [**destinationFeatureCurrentVertexChanged**](classQfNavigation.md#signal-destinationfeaturecurrentvertexchanged)  <br> |
| signal void | [**destinationFeatureVertexCountChanged**](classQfNavigation.md#signal-destinationfeaturevertexcountchanged)  <br> |
| signal void | [**destinationNameChanged**](classQfNavigation.md#signal-destinationnamechanged)  <br> |
| signal void | [**detailsChanged**](classQfNavigation.md#signal-detailschanged)  <br> |
| signal void | [**isActiveChanged**](classQfNavigation.md#signal-isactivechanged)  <br> |
| signal void | [**locationChanged**](classQfNavigation.md#signal-locationchanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfNavigation.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classQfNavigation.md#signal-modelchanged)  <br> |
| signal void | [**proximityAlarmChanged**](classQfNavigation.md#signal-proximityalarmchanged)  <br> |
| signal void | [**proximityAlarmThresholdChanged**](classQfNavigation.md#signal-proximityalarmthresholdchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNavigation**](#function-qfnavigation) () <br> |
|  double | [**bearing**](#function-bearing-22) () const<br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  Q\_INVOKABLE void | [**clearDestinationFeature**](#function-cleardestinationfeature) () <br> |
|  QgsPoint | [**destination**](#function-destination-22) () const<br> |
|  int | [**destinationFeatureCurrentVertex**](#function-destinationfeaturecurrentvertex-22) () const<br> |
|  int | [**destinationFeatureVertexCount**](#function-destinationfeaturevertexcount-22) () const<br> |
|  QString | [**destinationName**](#function-destinationname-22) () const<br> |
|  double | [**distance**](#function-distance-22) () const<br> |
|  Qgis::DistanceUnit | [**distanceUnits**](#function-distanceunits-22) () const<br> |
|  bool | [**isActive**](#function-isactive-22) () const<br> |
|  QgsPoint | [**location**](#function-location-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  [**QfNavigationModel**](classQfNavigationModel.md) \* | [**model**](#function-model-22) () const<br> |
|  Q\_INVOKABLE void | [**nextDestinationVertex**](#function-nextdestinationvertex) () <br> |
|  QgsGeometry | [**path**](#function-path-22) () const<br> |
|  Q\_INVOKABLE void | [**previousDestinationVertex**](#function-previousdestinationvertex) () <br> |
|  bool | [**proximityAlarm**](#function-proximityalarm-22) () const<br> |
|  double | [**proximityAlarmThreshold**](#function-proximityalarmthreshold-22) () const<br> |
|  void | [**setDestination**](#function-setdestination) (const QgsPoint & point) <br> |
|  Q\_INVOKABLE void | [**setDestinationFeature**](#function-setdestinationfeature) (const QgsFeature & feature, QgsVectorLayer \* layer) <br> |
|  void | [**setLocation**](#function-setlocation) (const QgsPoint & point) <br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setProximityAlarm**](#function-setproximityalarm) (const bool enabled) <br> |
|  void | [**setProximityAlarmThreshold**](#function-setproximityalarmthreshold) (const double & threshold) <br> |
|  double | [**verticalDistance**](#function-verticaldistance-22) () const<br> |
|   | [**~QfNavigation**](#function-qfnavigation) () <br> |




























## Public Properties Documentation




### property bearing [1/2]

```C++
double QfNavigation::bearing;
```




<hr>



### property destination [1/2]

```C++
QgsPoint QfNavigation::destination;
```




<hr>



### property destinationFeatureCurrentVertex [1/2]

```C++
int QfNavigation::destinationFeatureCurrentVertex;
```




<hr>



### property destinationFeatureVertexCount [1/2]

```C++
int QfNavigation::destinationFeatureVertexCount;
```




<hr>



### property destinationName [1/2]

```C++
QString QfNavigation::destinationName;
```




<hr>



### property distance [1/2]

```C++
double QfNavigation::distance;
```




<hr>



### property distanceUnits [1/2]

```C++
Qgis::DistanceUnit QfNavigation::distanceUnits;
```




<hr>



### property isActive [1/2]

```C++
bool QfNavigation::isActive;
```




<hr>



### property location [1/2]

```C++
modelChanged QgsPoint QfNavigation::location;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfNavigation::mapSettings;
```




<hr>



### property model [1/2]

```C++
QfNavigationModel * QfNavigation::model;
```




<hr>



### property path [1/2]

```C++
QgsGeometry QfNavigation::path;
```




<hr>



### property proximityAlarm [1/2]

```C++
bool QfNavigation::proximityAlarm;
```




<hr>



### property proximityAlarmThreshold [1/2]

```C++
double QfNavigation::proximityAlarmThreshold;
```




<hr>



### property verticalDistance [1/2]

```C++
double QfNavigation::verticalDistance;
```




<hr>
## Public Signals Documentation




### signal destinationChanged 

```C++
void QfNavigation::destinationChanged;
```




<hr>



### signal destinationFeatureCurrentVertexChanged 

```C++
void QfNavigation::destinationFeatureCurrentVertexChanged;
```




<hr>



### signal destinationFeatureVertexCountChanged 

```C++
void QfNavigation::destinationFeatureVertexCountChanged;
```




<hr>



### signal destinationNameChanged 

```C++
void QfNavigation::destinationNameChanged;
```




<hr>



### signal detailsChanged 

```C++
void QfNavigation::detailsChanged;
```




<hr>



### signal isActiveChanged 

```C++
void QfNavigation::isActiveChanged;
```




<hr>



### signal locationChanged 

```C++
void QfNavigation::locationChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfNavigation::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void QfNavigation::modelChanged;
```




<hr>



### signal proximityAlarmChanged 

```C++
void QfNavigation::proximityAlarmChanged;
```




<hr>



### signal proximityAlarmThresholdChanged 

```C++
void QfNavigation::proximityAlarmThresholdChanged;
```




<hr>
## Public Functions Documentation




### function QfNavigation 

```C++
QfNavigation::QfNavigation () 
```




<hr>



### function bearing [2/2]

```C++
inline double QfNavigation::bearing () const
```



Returns the bearing between the current location and the destination points 


        

<hr>



### function clear 

```C++
Q_INVOKABLE void QfNavigation::clear () 
```



Clears any existing destination point and feature. 


        

<hr>



### function clearDestinationFeature 

```C++
Q_INVOKABLE void QfNavigation::clearDestinationFeature () 
```



Clears the current destination feature, as well as the current destination point. 


        

<hr>



### function destination [2/2]

```C++
QgsPoint QfNavigation::destination () const
```



Returns the current destination point. 


        

<hr>



### function destinationFeatureCurrentVertex [2/2]

```C++
int QfNavigation::destinationFeatureCurrentVertex () const
```



Returns the current focused vertex serving as destination point when a feature is used as destination. 

**Note:**

for line and polygon geometries, vertex '0' represents the centroid of the geometry while it presents the first (or only) vertex in point geometries 





        

<hr>



### function destinationFeatureVertexCount [2/2]

```C++
int QfNavigation::destinationFeatureVertexCount () const
```



Returns the vertex count available to cycle through as destination points when a feature is used as destination. 


        

<hr>



### function destinationName [2/2]

```C++
QString QfNavigation::destinationName () const
```



Returns the current destination name. 

**See also:** [**setDestinationFeature**](classQfNavigation.md#function-setdestinationfeature) 



        

<hr>



### function distance [2/2]

```C++
inline double QfNavigation::distance () const
```



Returns the 2D distance between the current location and the destination points 


        

<hr>



### function distanceUnits [2/2]

```C++
inline Qgis::DistanceUnit QfNavigation::distanceUnits () const
```



Returns the distance unit used 


        

<hr>



### function isActive [2/2]

```C++
bool QfNavigation::isActive () const
```



Returns TRUE when navigation is active. 


        

<hr>



### function location [2/2]

```C++
QgsPoint QfNavigation::location () const
```



Returns the current location point. 


        

<hr>



### function mapSettings [2/2]

```C++
inline QgsQuickMapSettings * QfNavigation::mapSettings () const
```



Returns the map settings object used to project points and calculate navigation details. 


        

<hr>



### function model [2/2]

```C++
inline QfNavigationModel * QfNavigation::model () const
```



Returns the navigation model containing the destination point. 

**Note:**

In the future, the model could contain intermediary stops 





        

<hr>



### function nextDestinationVertex 

```C++
Q_INVOKABLE void QfNavigation::nextDestinationVertex () 
```



Sets the destination point to the next vertex or centroid of the current destination feature. 

**Note:**

if a destination feature has not been provided, calling this function does nothing 





        

<hr>



### function path [2/2]

```C++
inline QgsGeometry QfNavigation::path () const
```



Returns the path between the current location and the destination points 


        

<hr>



### function previousDestinationVertex 

```C++
Q_INVOKABLE void QfNavigation::previousDestinationVertex () 
```



Sets the destination point to the previous vertex or centroid of the current destination feature. 

**Note:**

if a destination feature has not been provided, calling this function does nothing 





        

<hr>



### function proximityAlarm [2/2]

```C++
inline bool QfNavigation::proximityAlarm () const
```



Returns TRUE if the proximity alarm is enabled. 


        

<hr>



### function proximityAlarmThreshold [2/2]

```C++
inline double QfNavigation::proximityAlarmThreshold () const
```



Returns the current distance threshold below which the proximity alarm will turn on when enabled. 


        

<hr>



### function setDestination 

```C++
void QfNavigation::setDestination (
    const QgsPoint & point
) 
```



Sets the current destination _point_. 


        

<hr>



### function setDestinationFeature 

```C++
Q_INVOKABLE void QfNavigation::setDestinationFeature (
    const QgsFeature & feature,
    QgsVectorLayer * layer
) 
```



Sets a provided feature as navigation destination, which allows for users to cycle through the feature centroid and its individual vertices as destination point. 

**Parameters:**


* `feature` the feature used as destination 
* `layer` the vector layer associated to the feature 




        

<hr>



### function setLocation 

```C++
void QfNavigation::setLocation (
    const QgsPoint & point
) 
```



Sets the current location _point_. 

**Note:**

A null/empty geometry will be threated as a lack of available location information 





        

<hr>



### function setMapSettings 

```C++
void QfNavigation::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



Sets the map settings object used to project points and calculate navigation details. 


        

<hr>



### function setProximityAlarm 

```C++
void QfNavigation::setProximityAlarm (
    const bool enabled
) 
```



Sets whether the proximity alarm is _enabled_. 


        

<hr>



### function setProximityAlarmThreshold 

```C++
void QfNavigation::setProximityAlarmThreshold (
    const double & threshold
) 
```



Sets the distance _threshold_ below which the proximity alarm will turn on when enabled. Once the distance threshold is met, the alarm sound will loop at a varying interval based on the distance between the device's current location and the destination. The closer to the destination, the shorter the interval. 


        

<hr>



### function verticalDistance [2/2]

```C++
inline double QfNavigation::verticalDistance () const
```



Retuens the vertical distance between the current location and the destination points 

**Note:**

if either points miss a Z value, the returned vertical distance value will be NaN 





        

<hr>



### function ~QfNavigation 

```C++
QfNavigation::~QfNavigation () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfnavigation.h`

