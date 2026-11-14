

# Class QfGeofencer



[**ClassList**](annotated.md) **>** [**QfGeofencer**](classQfGeofencer.md)



[More...](#detailed-description)

* `#include <qfgeofencer.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Behaviors**](#enum-behaviors)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classQfGeofencer.md#property-active-12)  <br> |
| property QgsVectorLayer \* | [**areasLayer**](classQfGeofencer.md#property-areaslayer-12)  <br> |
| property Behaviors | [**behavior**](classQfGeofencer.md#property-behavior-12)  <br> |
| property bool | [**isAlerting**](classQfGeofencer.md#property-isalerting-12)  <br> |
| property bool | [**isWithin**](classQfGeofencer.md#property-iswithin-12)  <br> |
| property QString | [**isWithinAreaName**](classQfGeofencer.md#property-iswithinareaname-12)  <br> |
| property QString | [**lastWithinAreaName**](classQfGeofencer.md#property-lastwithinareaname-12)  <br> |
| property QgsPoint | [**position**](classQfGeofencer.md#property-position-12)  <br> |
| property QgsCoordinateReferenceSystem | [**positionCrs**](classQfGeofencer.md#property-positioncrs-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classQfGeofencer.md#signal-activechanged)  <br> |
| signal void | [**areasLayerChanged**](classQfGeofencer.md#signal-areaslayerchanged)  <br> |
| signal void | [**behaviorChanged**](classQfGeofencer.md#signal-behaviorchanged)  <br> |
| signal void | [**invertLogicChanged**](classQfGeofencer.md#signal-invertlogicchanged)  <br> |
| signal void | [**isAlertingChanged**](classQfGeofencer.md#signal-isalertingchanged)  <br> |
| signal void | [**isWithinChanged**](classQfGeofencer.md#signal-iswithinchanged)  <br> |
| signal void | [**positionChanged**](classQfGeofencer.md#signal-positionchanged)  <br> |
| signal void | [**positionCrsChanged**](classQfGeofencer.md#signal-positioncrschanged)  <br> |
| signal void | [**projectChanged**](classQfGeofencer.md#signal-projectchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGeofencer**](#function-qfgeofencer) (QObject \* parent=nullptr) <br> |
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
| virtual  | [**~QfGeofencer**](#function-qfgeofencer) () <br> |




























## Detailed Description


This class provides an interface to manage geofencing of areas as well as providing feedback whenever the position trespasses into or out of those areas. 


    
## Public Types Documentation




### enum Behaviors 

```C++
enum QfGeofencer::Behaviors {
    AlertWhenInsideGeofencedArea = 1,
    AlertWhenOutsideGeofencedArea,
    InformWhenEnteringLeavingGeofencedArea
};
```




<hr>
## Public Properties Documentation




### property active [1/2]

```C++
bool QfGeofencer::active;
```




<hr>



### property areasLayer [1/2]

```C++
QgsVectorLayer * QfGeofencer::areasLayer;
```




<hr>



### property behavior [1/2]

```C++
Behaviors QfGeofencer::behavior;
```




<hr>



### property isAlerting [1/2]

```C++
bool QfGeofencer::isAlerting;
```




<hr>



### property isWithin [1/2]

```C++
bool QfGeofencer::isWithin;
```




<hr>



### property isWithinAreaName [1/2]

```C++
QString QfGeofencer::isWithinAreaName;
```




<hr>



### property lastWithinAreaName [1/2]

```C++
QString QfGeofencer::lastWithinAreaName;
```




<hr>



### property position [1/2]

```C++
QgsPoint QfGeofencer::position;
```




<hr>



### property positionCrs [1/2]

```C++
QgsCoordinateReferenceSystem QfGeofencer::positionCrs;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

```C++
void QfGeofencer::activeChanged;
```




<hr>



### signal areasLayerChanged 

```C++
void QfGeofencer::areasLayerChanged;
```




<hr>



### signal behaviorChanged 

```C++
void QfGeofencer::behaviorChanged;
```




<hr>



### signal invertLogicChanged 

```C++
void QfGeofencer::invertLogicChanged;
```




<hr>



### signal isAlertingChanged 

```C++
void QfGeofencer::isAlertingChanged;
```




<hr>



### signal isWithinChanged 

```C++
void QfGeofencer::isWithinChanged;
```




<hr>



### signal positionChanged 

```C++
void QfGeofencer::positionChanged;
```




<hr>



### signal positionCrsChanged 

```C++
void QfGeofencer::positionCrsChanged;
```




<hr>



### signal projectChanged 

```C++
void QfGeofencer::projectChanged;
```




<hr>
## Public Functions Documentation




### function QfGeofencer 

```C++
explicit QfGeofencer::QfGeofencer (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
inline bool QfGeofencer::active () const
```



Returns TRUE when geofencing is active. 

**See also:** [**setActive**](classQfGeofencer.md#function-setactive) 



        

<hr>



### function applyProjectSettings 

```C++
Q_INVOKABLE void QfGeofencer::applyProjectSettings (
    QgsProject * project
) 
```



Sets the polygon layer holding areas from a given _project_. 


        

<hr>



### function areasLayer [2/2]

```C++
inline QgsVectorLayer * QfGeofencer::areasLayer () const
```



Returns the polygon layer holding areas. 


        

<hr>



### function behavior [2/2]

```C++
inline Behaviors QfGeofencer::behavior () const
```



Returns thegeofencing behavior. 


        

<hr>



### function isAlerting [2/2]

```C++
inline bool QfGeofencer::isAlerting () const
```



Returns TRUE when the geofencer's behavior is triggering an alarm. 


        

<hr>



### function isWithin [2/2]

```C++
bool QfGeofencer::isWithin () const
```



Returns whether the current position is within an area. 


        

<hr>



### function isWithinAreaName [2/2]

```C++
QString QfGeofencer::isWithinAreaName () const
```



Returns the area name within which the current position overlaps. If no overlap, an empty string will be returned. 


        

<hr>



### function lastWithinAreaName [2/2]

```C++
QString QfGeofencer::lastWithinAreaName () const
```



Returns the last area name within which the position had overlapped. If no overlap has yet to occur, an empty string will be returned. 


        

<hr>



### function position [2/2]

```C++
inline QgsPoint QfGeofencer::position () const
```



Returns the position to be used to check for overlap with areas. 


        

<hr>



### function positionCrs [2/2]

```C++
inline QgsCoordinateReferenceSystem QfGeofencer::positionCrs () const
```



Returns the position's coordinate reference system (CRS). 


        

<hr>



### function setActive 

```C++
void QfGeofencer::setActive (
    bool active
) 
```



Sets the geofencing _active_ state. 

**See also:** active 



        

<hr>



### function setAreasLayer 

```C++
void QfGeofencer::setAreasLayer (
    QgsVectorLayer * layer
) 
```



Sets the polygon layer holding areas. 


        

<hr>



### function setBehavior 

```C++
void QfGeofencer::setBehavior (
    Behaviors behavior
) 
```



Sets the geofencing behavior. 


        

<hr>



### function setPosition 

```C++
void QfGeofencer::setPosition (
    const QgsPoint & position
) 
```



Sets the _position_ to be used to check for overlap with areas. 


        

<hr>



### function setPositionCrs 

```C++
void QfGeofencer::setPositionCrs (
    const QgsCoordinateReferenceSystem & crs
) 
```



Sets the position's coordinate reference system (CRS). 


        

<hr>



### function ~QfGeofencer 

```C++
virtual QfGeofencer::~QfGeofencer () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/positioning/qfgeofencer.h`

