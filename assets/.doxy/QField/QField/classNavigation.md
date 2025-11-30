

# Class Navigation



[**ClassList**](annotated.md) **>** [**Navigation**](classNavigation.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**bearing**](classNavigation.md#property-bearing-12)  <br> |
| property QgsPoint | [**destination**](classNavigation.md#property-destination-12)  <br> |
| property int | [**destinationFeatureCurrentVertex**](classNavigation.md#property-destinationfeaturecurrentvertex-12)  <br> |
| property int | [**destinationFeatureVertexCount**](classNavigation.md#property-destinationfeaturevertexcount-12)  <br> |
| property QString | [**destinationName**](classNavigation.md#property-destinationname-12)  <br> |
| property double | [**distance**](classNavigation.md#property-distance-12)  <br> |
| property Qgis::DistanceUnit | [**distanceUnits**](classNavigation.md#property-distanceunits-12)  <br> |
| property bool | [**isActive**](classNavigation.md#property-isactive-12)  <br> |
| property modelChanged QgsPoint | [**location**](classNavigation.md#property-location-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classNavigation.md#property-mapsettings-12)  <br> |
| property [**NavigationModel**](classNavigationModel.md) \* | [**model**](classNavigation.md#property-model-12)  <br> |
| property QgsGeometry | [**path**](classNavigation.md#property-path-12)  <br> |
| property bool | [**proximityAlarm**](classNavigation.md#property-proximityalarm-12)  <br> |
| property double | [**proximityAlarmThreshold**](classNavigation.md#property-proximityalarmthreshold-12)  <br> |
| property double | [**verticalDistance**](classNavigation.md#property-verticaldistance-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**destinationChanged**](classNavigation.md#signal-destinationchanged)  <br> |
| signal void | [**destinationFeatureCurrentVertexChanged**](classNavigation.md#signal-destinationfeaturecurrentvertexchanged)  <br> |
| signal void | [**destinationFeatureVertexCountChanged**](classNavigation.md#signal-destinationfeaturevertexcountchanged)  <br> |
| signal void | [**destinationNameChanged**](classNavigation.md#signal-destinationnamechanged)  <br> |
| signal void | [**detailsChanged**](classNavigation.md#signal-detailschanged)  <br> |
| signal void | [**isActiveChanged**](classNavigation.md#signal-isactivechanged)  <br> |
| signal void | [**locationChanged**](classNavigation.md#signal-locationchanged)  <br> |
| signal void | [**mapSettingsChanged**](classNavigation.md#signal-mapsettingschanged)  <br> |
| signal void | [**modelChanged**](classNavigation.md#signal-modelchanged)  <br> |
| signal void | [**proximityAlarmChanged**](classNavigation.md#signal-proximityalarmchanged)  <br> |
| signal void | [**proximityAlarmThresholdChanged**](classNavigation.md#signal-proximityalarmthresholdchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Navigation**](#function-navigation) () <br> |
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
|  [**NavigationModel**](classNavigationModel.md) \* | [**model**](#function-model-22) () const<br> |
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
|   | [**~Navigation**](#function-navigation) () <br> |




























## Public Properties Documentation




### property bearing [1/2]

```C++
double Navigation::bearing;
```




<hr>



### property destination [1/2]

```C++
QgsPoint Navigation::destination;
```




<hr>



### property destinationFeatureCurrentVertex [1/2]

```C++
int Navigation::destinationFeatureCurrentVertex;
```




<hr>



### property destinationFeatureVertexCount [1/2]

```C++
int Navigation::destinationFeatureVertexCount;
```




<hr>



### property destinationName [1/2]

```C++
QString Navigation::destinationName;
```




<hr>



### property distance [1/2]

```C++
double Navigation::distance;
```




<hr>



### property distanceUnits [1/2]

```C++
Qgis::DistanceUnit Navigation::distanceUnits;
```




<hr>



### property isActive [1/2]

```C++
bool Navigation::isActive;
```




<hr>



### property location [1/2]

```C++
modelChanged QgsPoint Navigation::location;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * Navigation::mapSettings;
```




<hr>



### property model [1/2]

```C++
NavigationModel * Navigation::model;
```




<hr>



### property path [1/2]

```C++
QgsGeometry Navigation::path;
```




<hr>



### property proximityAlarm [1/2]

```C++
bool Navigation::proximityAlarm;
```




<hr>



### property proximityAlarmThreshold [1/2]

```C++
double Navigation::proximityAlarmThreshold;
```




<hr>



### property verticalDistance [1/2]

```C++
double Navigation::verticalDistance;
```




<hr>
## Public Signals Documentation




### signal destinationChanged 

```C++
void Navigation::destinationChanged;
```




<hr>



### signal destinationFeatureCurrentVertexChanged 

```C++
void Navigation::destinationFeatureCurrentVertexChanged;
```




<hr>



### signal destinationFeatureVertexCountChanged 

```C++
void Navigation::destinationFeatureVertexCountChanged;
```




<hr>



### signal destinationNameChanged 

```C++
void Navigation::destinationNameChanged;
```




<hr>



### signal detailsChanged 

```C++
void Navigation::detailsChanged;
```




<hr>



### signal isActiveChanged 

```C++
void Navigation::isActiveChanged;
```




<hr>



### signal locationChanged 

```C++
void Navigation::locationChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void Navigation::mapSettingsChanged;
```




<hr>



### signal modelChanged 

```C++
void Navigation::modelChanged;
```




<hr>



### signal proximityAlarmChanged 

```C++
void Navigation::proximityAlarmChanged;
```




<hr>



### signal proximityAlarmThresholdChanged 

```C++
void Navigation::proximityAlarmThresholdChanged;
```




<hr>
## Public Functions Documentation




### function Navigation 

```C++
Navigation::Navigation () 
```




<hr>



### function bearing [2/2]

```C++
inline double Navigation::bearing () const
```



Returns the bearing between the current location and the destination points 


        

<hr>



### function clear 

```C++
Q_INVOKABLE void Navigation::clear () 
```



Clears any existing destination point and feature. 


        

<hr>



### function clearDestinationFeature 

```C++
Q_INVOKABLE void Navigation::clearDestinationFeature () 
```



Clears the current destination feature, as well as the current destination point. 


        

<hr>



### function destination [2/2]

```C++
QgsPoint Navigation::destination () const
```



Returns the current destination point. 


        

<hr>



### function destinationFeatureCurrentVertex [2/2]

```C++
int Navigation::destinationFeatureCurrentVertex () const
```



Returns the current focused vertex serving as destination point when a feature is used as destination. 

**Note:**

for line and polygon geometries, vertex '0' represents the centroid of the geometry while it presents the first (or only) vertex in point geometries 





        

<hr>



### function destinationFeatureVertexCount [2/2]

```C++
int Navigation::destinationFeatureVertexCount () const
```



Returns the vertex count available to cycle through as destination points when a feature is used as destination. 


        

<hr>



### function destinationName [2/2]

```C++
QString Navigation::destinationName () const
```



Returns the current destination name. 

**See also:** [**setDestinationFeature**](classNavigation.md#function-setdestinationfeature) 



        

<hr>



### function distance [2/2]

```C++
inline double Navigation::distance () const
```



Returns the 2D distance between the current location and the destination points 


        

<hr>



### function distanceUnits [2/2]

```C++
inline Qgis::DistanceUnit Navigation::distanceUnits () const
```



Returns the distance unit used 


        

<hr>



### function isActive [2/2]

```C++
bool Navigation::isActive () const
```



Returns TRUE when navigation is active. 


        

<hr>



### function location [2/2]

```C++
QgsPoint Navigation::location () const
```



Returns the current location point. 


        

<hr>



### function mapSettings [2/2]

```C++
inline QgsQuickMapSettings * Navigation::mapSettings () const
```



Returns the map settings object used to project points and calculate navigation details. 


        

<hr>



### function model [2/2]

```C++
inline NavigationModel * Navigation::model () const
```



Returns the navigation model containing the destination point. 

**Note:**

In the future, the model could contain intermediary stops 





        

<hr>



### function nextDestinationVertex 

```C++
Q_INVOKABLE void Navigation::nextDestinationVertex () 
```



Sets the destination point to the next vertex or centroid of the current destination feature. 

**Note:**

if a destination feature has not been provided, calling this function does nothing 





        

<hr>



### function path [2/2]

```C++
inline QgsGeometry Navigation::path () const
```



Returns the path between the current location and the destination points 


        

<hr>



### function previousDestinationVertex 

```C++
Q_INVOKABLE void Navigation::previousDestinationVertex () 
```



Sets the destination point to the previous vertex or centroid of the current destination feature. 

**Note:**

if a destination feature has not been provided, calling this function does nothing 





        

<hr>



### function proximityAlarm [2/2]

```C++
inline bool Navigation::proximityAlarm () const
```



Returns TRUE if the proximity alarm is enabled. 


        

<hr>



### function proximityAlarmThreshold [2/2]

```C++
inline double Navigation::proximityAlarmThreshold () const
```



Returns the current distance threshold below which the proximity alarm will turn on when enabled. 


        

<hr>



### function setDestination 

```C++
void Navigation::setDestination (
    const QgsPoint & point
) 
```



Sets the current destination _point_. 


        

<hr>



### function setDestinationFeature 

```C++
Q_INVOKABLE void Navigation::setDestinationFeature (
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
void Navigation::setLocation (
    const QgsPoint & point
) 
```



Sets the current location _point_. 

**Note:**

A null/empty geometry will be threated as a lack of available location information 





        

<hr>



### function setMapSettings 

```C++
void Navigation::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



Sets the map settings object used to project points and calculate navigation details. 


        

<hr>



### function setProximityAlarm 

```C++
void Navigation::setProximityAlarm (
    const bool enabled
) 
```



Sets whether the proximity alarm is _enabled_. 


        

<hr>



### function setProximityAlarmThreshold 

```C++
void Navigation::setProximityAlarmThreshold (
    const double & threshold
) 
```



Sets the distance _threshold_ below which the proximity alarm will turn on when enabled. Once the distance threshold is met, the alarm sound will loop at a varying interval based on the distance between the device's current location and the destination. The closer to the destination, the shorter the interval. 


        

<hr>



### function verticalDistance [2/2]

```C++
inline double Navigation::verticalDistance () const
```



Retuens the vertical distance between the current location and the destination points 

**Note:**

if either points miss a Z value, the returned vertical distance value will be NaN 





        

<hr>



### function ~Navigation 

```C++
Navigation::~Navigation () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/navigation.h`

