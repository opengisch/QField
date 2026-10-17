

# Class QfMapToScreen



[**ClassList**](annotated.md) **>** [**QfMapToScreen**](classQfMapToScreen.md)



_The_ [_**QfMapToScreen**_](classQfMapToScreen.md) _class transform a map point to screen coordinates. The map point CRS is the one from the map settings. Screen point will be automatically updated on map extent changes._

* `#include <qfmaptoscreen.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**mapDistance**](classQfMapToScreen.md#property-mapdistance-12)  <br>_the distance in map unit_  |
| property QgsPoint | [**mapPoint**](classQfMapToScreen.md#property-mappoint-12)  <br>_the point in map coordinates_  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfMapToScreen.md#property-mapsettings-12)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| property double | [**screenDistance**](classQfMapToScreen.md#property-screendistance-12)  <br>_the distance in screen coordinates (read-only)_  |
| property QPointF | [**screenPoint**](classQfMapToScreen.md#property-screenpoint-12)  <br>_the point in screen coordinates (read-only)_  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapDistanceChanged**](classQfMapToScreen.md#signal-mapdistancechanged)  <br>_the distance in map unit_  |
| signal void | [**mapPointChanged**](classQfMapToScreen.md#signal-mappointchanged)  <br>_the point in map coordinates_  |
| signal void | [**mapSettingsChanged**](classQfMapToScreen.md#signal-mapsettingschanged)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| signal void | [**screenDistanceChanged**](classQfMapToScreen.md#signal-screendistancechanged)  <br>_the distance in screen coordinates (read-only)_  |
| signal void | [**screenPointChanged**](classQfMapToScreen.md#signal-screenpointchanged)  <br>_the point in screen coordinates (read-only)_  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMapToScreen**](#function-qfmaptoscreen) (QObject \* parent=nullptr) <br> |
|  double | [**mapDistance**](#function-mapdistance-22) () const<br>_the distance in map unit_  |
|  QgsPoint | [**mapPoint**](#function-mappoint-22) () const<br>_the point in map coordinates_  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
|  double | [**screenDistance**](#function-screendistance-22) () const<br>_the distance in screen coordinates (read-only)_  |
|  QPointF | [**screenPoint**](#function-screenpoint-22) () const<br>_the point in screen coordinates (read-only)_  |
|  void | [**setMapDistance**](#function-setmapdistance) (const double distance) <br>_the distance in map unit_  |
|  void | [**setMapPoint**](#function-setmappoint) (const QgsPoint & point) <br>_the point in map coordinates_  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |




























## Public Properties Documentation




### property mapDistance [1/2]

_the distance in map unit_ 
```C++
double QfMapToScreen::mapDistance;
```




<hr>



### property mapPoint [1/2]

_the point in map coordinates_ 
```C++
QgsPoint QfMapToScreen::mapPoint;
```




<hr>



### property mapSettings [1/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * QfMapToScreen::mapSettings;
```




<hr>



### property screenDistance [1/2]

_the distance in screen coordinates (read-only)_ 
```C++
double QfMapToScreen::screenDistance;
```




<hr>



### property screenPoint [1/2]

_the point in screen coordinates (read-only)_ 
```C++
QPointF QfMapToScreen::screenPoint;
```




<hr>
## Public Signals Documentation




### signal mapDistanceChanged 

_the distance in map unit_ 
```C++
void QfMapToScreen::mapDistanceChanged;
```




<hr>



### signal mapPointChanged 

_the point in map coordinates_ 
```C++
void QfMapToScreen::mapPointChanged;
```




<hr>



### signal mapSettingsChanged 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void QfMapToScreen::mapSettingsChanged;
```




<hr>



### signal screenDistanceChanged 

_the distance in screen coordinates (read-only)_ 
```C++
void QfMapToScreen::screenDistanceChanged;
```




<hr>



### signal screenPointChanged 

_the point in screen coordinates (read-only)_ 
```C++
void QfMapToScreen::screenPointChanged;
```




<hr>
## Public Functions Documentation




### function QfMapToScreen 

```C++
explicit QfMapToScreen::QfMapToScreen (
    QObject * parent=nullptr
) 
```




<hr>



### function mapDistance [2/2]

_the distance in map unit_ 
```C++
double QfMapToScreen::mapDistance () const
```




<hr>



### function mapPoint [2/2]

_the point in map coordinates_ 
```C++
QgsPoint QfMapToScreen::mapPoint () const
```




<hr>



### function mapSettings [2/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * QfMapToScreen::mapSettings () const
```




<hr>



### function screenDistance [2/2]

_the distance in screen coordinates (read-only)_ 
```C++
double QfMapToScreen::screenDistance () const
```




<hr>



### function screenPoint [2/2]

_the point in screen coordinates (read-only)_ 
```C++
QPointF QfMapToScreen::screenPoint () const
```




<hr>



### function setMapDistance 

_the distance in map unit_ 
```C++
void QfMapToScreen::setMapDistance (
    const double distance
) 
```




<hr>



### function setMapPoint 

_the point in map coordinates_ 
```C++
void QfMapToScreen::setMapPoint (
    const QgsPoint & point
) 
```




<hr>



### function setMapSettings 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void QfMapToScreen::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmaptoscreen.h`

