

# Class MapToScreen



[**ClassList**](annotated.md) **>** [**MapToScreen**](classMapToScreen.md)



_The_ [_**MapToScreen**_](classMapToScreen.md) _class transform a map point to screen coordinates. The map point CRS is the one from the map settings. Screen point will be automatically updated on map extent changes._

* `#include <maptoscreen.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property double | [**mapDistance**](classMapToScreen.md#property-mapdistance-12)  <br>_the distance in map unit_  |
| property QgsPoint | [**mapPoint**](classMapToScreen.md#property-mappoint-12)  <br>_the point in map coordinates_  |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classMapToScreen.md#property-mapsettings-12)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| property double | [**screenDistance**](classMapToScreen.md#property-screendistance-12)  <br>_the distance in screen coordinates (read-only)_  |
| property QPointF | [**screenPoint**](classMapToScreen.md#property-screenpoint-12)  <br>_the point in screen coordinates (read-only)_  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**mapDistanceChanged**](classMapToScreen.md#signal-mapdistancechanged)  <br>_the distance in map unit_  |
| signal void | [**mapPointChanged**](classMapToScreen.md#signal-mappointchanged)  <br>_the point in map coordinates_  |
| signal void | [**mapSettingsChanged**](classMapToScreen.md#signal-mapsettingschanged)  <br>_Map settings is used to define the map canvas CRS and detect any extent change._  |
| signal void | [**screenDistanceChanged**](classMapToScreen.md#signal-screendistancechanged)  <br>_the distance in screen coordinates (read-only)_  |
| signal void | [**screenPointChanged**](classMapToScreen.md#signal-screenpointchanged)  <br>_the point in screen coordinates (read-only)_  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**MapToScreen**](#function-maptoscreen) (QObject \* parent=nullptr) <br> |
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
double MapToScreen::mapDistance;
```




<hr>



### property mapPoint [1/2]

_the point in map coordinates_ 
```C++
QgsPoint MapToScreen::mapPoint;
```




<hr>



### property mapSettings [1/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * MapToScreen::mapSettings;
```




<hr>



### property screenDistance [1/2]

_the distance in screen coordinates (read-only)_ 
```C++
double MapToScreen::screenDistance;
```




<hr>



### property screenPoint [1/2]

_the point in screen coordinates (read-only)_ 
```C++
QPointF MapToScreen::screenPoint;
```




<hr>
## Public Signals Documentation




### signal mapDistanceChanged 

_the distance in map unit_ 
```C++
void MapToScreen::mapDistanceChanged;
```




<hr>



### signal mapPointChanged 

_the point in map coordinates_ 
```C++
void MapToScreen::mapPointChanged;
```




<hr>



### signal mapSettingsChanged 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void MapToScreen::mapSettingsChanged;
```




<hr>



### signal screenDistanceChanged 

_the distance in screen coordinates (read-only)_ 
```C++
void MapToScreen::screenDistanceChanged;
```




<hr>



### signal screenPointChanged 

_the point in screen coordinates (read-only)_ 
```C++
void MapToScreen::screenPointChanged;
```




<hr>
## Public Functions Documentation




### function MapToScreen 

```C++
explicit MapToScreen::MapToScreen (
    QObject * parent=nullptr
) 
```




<hr>



### function mapDistance [2/2]

_the distance in map unit_ 
```C++
double MapToScreen::mapDistance () const
```




<hr>



### function mapPoint [2/2]

_the point in map coordinates_ 
```C++
QgsPoint MapToScreen::mapPoint () const
```




<hr>



### function mapSettings [2/2]

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
QgsQuickMapSettings * MapToScreen::mapSettings () const
```




<hr>



### function screenDistance [2/2]

_the distance in screen coordinates (read-only)_ 
```C++
double MapToScreen::screenDistance () const
```




<hr>



### function screenPoint [2/2]

_the point in screen coordinates (read-only)_ 
```C++
QPointF MapToScreen::screenPoint () const
```




<hr>



### function setMapDistance 

_the distance in map unit_ 
```C++
void MapToScreen::setMapDistance (
    const double distance
) 
```




<hr>



### function setMapPoint 

_the point in map coordinates_ 
```C++
void MapToScreen::setMapPoint (
    const QgsPoint & point
) 
```




<hr>



### function setMapSettings 

_Map settings is used to define the map canvas CRS and detect any extent change._ 
```C++
void MapToScreen::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/maptoscreen.h`

