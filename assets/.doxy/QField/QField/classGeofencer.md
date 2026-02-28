

# Class Geofencer



[**ClassList**](annotated.md) **>** [**Geofencer**](classGeofencer.md)



[More...](#detailed-description)

* `#include <geofencer.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Behaviors**](#enum-behaviors)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classGeofencer.md#property-active-12)  <br> |
| property QgsVectorLayer \* | [**areasLayer**](classGeofencer.md#property-areaslayer-12)  <br> |
| property Behaviors | [**behavior**](classGeofencer.md#property-behavior-12)  <br> |
| property bool | [**isAlerting**](classGeofencer.md#property-isalerting-12)  <br> |
| property bool | [**isWithin**](classGeofencer.md#property-iswithin-12)  <br> |
| property QString | [**isWithinAreaName**](classGeofencer.md#property-iswithinareaname-12)  <br> |
| property QString | [**lastWithinAreaName**](classGeofencer.md#property-lastwithinareaname-12)  <br> |
| property QgsPoint | [**position**](classGeofencer.md#property-position-12)  <br> |
| property QgsCoordinateReferenceSystem | [**positionCrs**](classGeofencer.md#property-positioncrs-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classGeofencer.md#signal-activechanged)  <br> |
| signal void | [**areasLayerChanged**](classGeofencer.md#signal-areaslayerchanged)  <br> |
| signal void | [**behaviorChanged**](classGeofencer.md#signal-behaviorchanged)  <br> |
| signal void | [**invertLogicChanged**](classGeofencer.md#signal-invertlogicchanged)  <br> |
| signal void | [**isAlertingChanged**](classGeofencer.md#signal-isalertingchanged)  <br> |
| signal void | [**isWithinChanged**](classGeofencer.md#signal-iswithinchanged)  <br> |
| signal void | [**positionChanged**](classGeofencer.md#signal-positionchanged)  <br> |
| signal void | [**positionCrsChanged**](classGeofencer.md#signal-positioncrschanged)  <br> |
| signal void | [**projectChanged**](classGeofencer.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Geofencer**](#function-geofencer) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  Q\_INVOKABLE void | [**applyProjectSettings**](#function-applyprojectsettings) (QgsProject \* project) <br> |
|  QgsVectorLayer \* | [**areasLayer**](#function-areaslayer-22) () const<br> |
|  Behaviors | [**behavior**](#function-behavior-22) () const<br> |
|  bool | [**isAlerting**](#function-isalerting-22) () const<br> |
|  bool | [**isWithin**](#function-iswithin-22) () const<br> |
|  QString | [**isWithinAreaName**](#function-iswithinareaname-22) () const<br> |
|  QString | [**lastWithinAreaName**](#function-lastwithinareaname-22) () const<br> |
|  QgsPoint | [**position**](#function-position-22) () const<br> |
|  QgsCoordinateReferenceSystem | [**positionCrs**](#function-positioncrs-22) () const<br> |
|  void | [**setActive**](#function-setactive) (bool active) <br> |
|  void | [**setAreasLayer**](#function-setareaslayer) (QgsVectorLayer \* layer) <br> |
|  void | [**setBehavior**](#function-setbehavior) (Behaviors behavior) <br> |
|  void | [**setPosition**](#function-setposition) (const QgsPoint & position) <br> |
|  void | [**setPositionCrs**](#function-setpositioncrs) (const QgsCoordinateReferenceSystem & crs) <br> |
| virtual  | [**~Geofencer**](#function-geofencer) () <br> |




























## Detailed Description


This class provides an interface to manage geofencing of areas as well as providing feedback whenever the position trespasses into or out of those areas. 


    
## Public Types Documentation




### enum Behaviors 

```C++
enum Geofencer::Behaviors {
    AlertWhenInsideGeofencedArea = 1,
    AlertWhenOutsideGeofencedArea,
    InformWhenEnteringLeavingGeofencedArea
};
```




<hr>
## Public Properties Documentation




### property active [1/2]

```C++
bool Geofencer::active;
```




<hr>



### property areasLayer [1/2]

```C++
QgsVectorLayer * Geofencer::areasLayer;
```




<hr>



### property behavior [1/2]

```C++
Behaviors Geofencer::behavior;
```




<hr>



### property isAlerting [1/2]

```C++
bool Geofencer::isAlerting;
```




<hr>



### property isWithin [1/2]

```C++
bool Geofencer::isWithin;
```




<hr>



### property isWithinAreaName [1/2]

```C++
QString Geofencer::isWithinAreaName;
```




<hr>



### property lastWithinAreaName [1/2]

```C++
QString Geofencer::lastWithinAreaName;
```




<hr>



### property position [1/2]

```C++
QgsPoint Geofencer::position;
```




<hr>



### property positionCrs [1/2]

```C++
QgsCoordinateReferenceSystem Geofencer::positionCrs;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void Geofencer::activeChanged;
```




<hr>



### signal areasLayerChanged 

```C++
void Geofencer::areasLayerChanged;
```




<hr>



### signal behaviorChanged 

```C++
void Geofencer::behaviorChanged;
```




<hr>



### signal invertLogicChanged 

```C++
void Geofencer::invertLogicChanged;
```




<hr>



### signal isAlertingChanged 

```C++
void Geofencer::isAlertingChanged;
```




<hr>



### signal isWithinChanged 

```C++
void Geofencer::isWithinChanged;
```




<hr>



### signal positionChanged 

```C++
void Geofencer::positionChanged;
```




<hr>



### signal positionCrsChanged 

```C++
void Geofencer::positionCrsChanged;
```




<hr>



### signal projectChanged 

```C++
void Geofencer::projectChanged;
```




<hr>
## Public Functions Documentation




### function Geofencer 

```C++
explicit Geofencer::Geofencer (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
inline bool Geofencer::active () const
```



Returns TRUE when geofencing is active. 

**See also:** [**setActive**](classGeofencer.md#function-setactive) 



        

<hr>



### function applyProjectSettings 

```C++
Q_INVOKABLE void Geofencer::applyProjectSettings (
    QgsProject * project
) 
```



Sets the polygon layer holding areas from a given _project_. 


        

<hr>



### function areasLayer [2/2]

```C++
inline QgsVectorLayer * Geofencer::areasLayer () const
```



Returns the polygon layer holding areas. 


        

<hr>



### function behavior [2/2]

```C++
inline Behaviors Geofencer::behavior () const
```



Returns thegeofencing behavior. 


        

<hr>



### function isAlerting [2/2]

```C++
inline bool Geofencer::isAlerting () const
```



Returns TRUE when the geofencer's behavior is triggering an alarm. 


        

<hr>



### function isWithin [2/2]

```C++
bool Geofencer::isWithin () const
```



Returns whether the current position is within an area. 


        

<hr>



### function isWithinAreaName [2/2]

```C++
QString Geofencer::isWithinAreaName () const
```



Returns the area name within which the current position overlaps. If no overlap, an empty string will be returned. 


        

<hr>



### function lastWithinAreaName [2/2]

```C++
QString Geofencer::lastWithinAreaName () const
```



Returns the last area name within which the position had overlapped. If no overlap has yet to occur, an empty string will be returned. 


        

<hr>



### function position [2/2]

```C++
inline QgsPoint Geofencer::position () const
```



Returns the position to be used to check for overlap with areas. 


        

<hr>



### function positionCrs [2/2]

```C++
inline QgsCoordinateReferenceSystem Geofencer::positionCrs () const
```



Returns the position's coordinate reference system (CRS). 


        

<hr>



### function setActive 

```C++
void Geofencer::setActive (
    bool active
) 
```



Sets the geofencing _active_ state. 

**See also:** active 



        

<hr>



### function setAreasLayer 

```C++
void Geofencer::setAreasLayer (
    QgsVectorLayer * layer
) 
```



Sets the polygon layer holding areas. 


        

<hr>



### function setBehavior 

```C++
void Geofencer::setBehavior (
    Behaviors behavior
) 
```



Sets the geofencing behavior. 


        

<hr>



### function setPosition 

```C++
void Geofencer::setPosition (
    const QgsPoint & position
) 
```



Sets the _position_ to be used to check for overlap with areas. 


        

<hr>



### function setPositionCrs 

```C++
void Geofencer::setPositionCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```



Sets the position's coordinate reference system (CRS). 


        

<hr>



### function ~Geofencer 

```C++
virtual Geofencer::~Geofencer () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/geofencer.h`

